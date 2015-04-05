// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"
#include "alloc.h"
#include "list.h"

#include <error.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef cmd_list_t command_stream;

bool isword(char c){
  return strchr("!%+,-./:@^_", c) || isalpha(c) || isdigit(c);
}

bool isnewop(char c){
  return strchr("|;&",c);
}

void syntax_error(char col){
  printf("syntax error near `%c'\n", col );
  exit(1);
}

bool isblankstr(char * stream){
  if(strlen(stream) == 0)
    return true;
  int i=0;
  bool blank = false;
  for (i = 0; i < strlen(stream); i++){
    if(!strchr("\t ", stream[i]))
      blank = false;
  }

  return blank;
}

STRING tostring(void * fp){
  char c;
  int s;
  while((c = getc(fp)) != EOF){
    s++;
  }
  STRING str = (STRING) checked_malloc(s+1);
  s = 0;
  rewind((FILE*)fp);
  while((c = getc(fp)) != EOF){
    str[s++] = c;
  }
  str[s] = '\0';
  rewind((FILE*)fp);
  return str;
}

command_t make_simple_command(char * stream){
  if(isblankstr(stream)){
    return NULL;
  }
  int wchunk = 4; // initial size for words array
  char ** words = (char**)checked_malloc(wchunk*sizeof(char**));
  int bchunk = 16; // initial size for buffer(each word)
  char * wbuffer = (char*)checked_malloc(bchunk);
  bool wflag = false;
  int wordcount=0;
  int i=0; // stream index
  int w=0; // words index
  int b=0; // buffer index 

  for(i=0; i < strlen(stream)-1; i++){
    if(strchr("\t ", stream[i])){
      if(wflag){
        wflag = false;
        if(w >= wchunk){
          wchunk *= 2;
          words = (char**)checked_realloc(words,wchunk);
        }
        words[w++] = wbuffer;
        wbuffer = NULL;
      }
      continue;
    }
    else if(isword(stream[i])){
      //putchar(stream[i]);
      if(!wflag){
        wordcount++;
        b = 0; //reset wbuffer index
        bchunk = 16; //reset buffer chunk
        wflag = true;
        if(wbuffer == NULL)
          wbuffer = (char*)checked_malloc(bchunk);

        wbuffer[b++] = stream[i];
      }
      else{
        if(b >= bchunk){
          bchunk *= 2;
          wbuffer = (char*)checked_realloc(wbuffer, bchunk);          
        }
        wbuffer[b++] = stream[i];
      }
    }
  }
  //at the end of stream
  if(wbuffer){
    wordcount++;
    words[w++] = wbuffer;
  }
  if(wordcount == 0)
    return NULL;
  command_t cmd = (command_t) malloc(sizeof(command_t));
  cmd->type = SIMPLE_COMMAND;
  cmd->status = -1;
  cmd->u.word = (char**)checked_malloc((wordcount+1)*sizeof(char**));

  for(i=0; i < wordcount; i++){
    cmd->u.word[i] = words[i];
  }

  cmd->u.word[i] = 0; //needed for execvp
  return cmd;
}

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  //get file size
  void * file_stream = get_next_byte_argument;
  cmd_list_t cmd_list = cmd_list_new();
  str_list_t op_list = str_list_new();

  int chunk = 16;
  char * buffer = (char*)checked_malloc(chunk);
  int b = 0; // buffer index
  char c;
  // convert file stream to string
  STRING input = tostring(file_stream);

  while(*input != '\0'){
    c = *input++;
    //if word or space or tab, put it in the buffer
    if(isword(c) || strchr("\t ",c)){
      if(b >= chunk){
        chunk *= 2;
        buffer = (char*)checked_realloc(buffer, chunk*sizeof(char**));
      }
      if(buffer == NULL)
        buffer = (char*)checked_malloc(chunk);
      buffer[b++] = c;
    }
    //if it's operator, push simple command and the operator
    else if(isnewop(c)){
      if(!isblankstr(buffer)){
        command_t cmd_a = make_simple_command(buffer);
        cmd_list_push(cmd_list, cmd_a);
        b = 0;
        buffer = NULL;
        chunk = 16;
      }
      else
        syntax_error(c);
      // *input is now next character
      if(c == '|' && *input == '|'){
        printf("OR\n" );
        str_list_push(op_list, "||");
        input++;
      }
      else if(c == '&' && *input == '&'){
        printf("AND\n");
        str_list_push(op_list, "&&");
        input++;
      }
      else if(isword(*input) || strchr("\t ", *input)){
        switch(c){
          case ';': str_list_push(op_list, ";");
            break;
          case '|' : str_list_push(op_list, "|");
            break;
          case '&' :str_list_push(op_list, "&");
            break;
          case '(' :str_list_push(op_list, "(");
            break;
          case ')' :str_list_push(op_list, ")");
            break;
          default:
            syntax_error(c);
        }
      }
    }
  }
  //printf("buffer = %s, size=%d\n", buffer, b);
  if(buffer){
    buffer[b] = c;
    command_t cmd_b = make_simple_command(buffer);
    //command_t cmd_b = NULL;
    if(cmd_b)
      cmd_list_push(cmd_list, cmd_b);
  }
  //printf("cmd size: %d\n", cmd_list_size(cmd_list));
  printf("%s\n", cmd_list_peek(cmd_list)->u.word[0]);

  //printf("%d\n", str_list_size(op_list));
  //str_list_pop(op_list);
  //printf("%s\n",str_list_peek(op_list));


  cmd_list_free(cmd_list);
  str_list_free(op_list);

  return 0;
}

command_t
read_command_stream (command_stream_t s)
{
  /* FIXME: Replace this with your implementation too.  */
  //error (1, 0, "command reading not yet implemented");
  return 0;
}
