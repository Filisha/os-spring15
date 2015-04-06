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

bool isspecial(char c){
  return strchr("|;&()",c);
}
bool isoperator(char c){
  if(strchr(";|&", c))
    return true;
}
char * make_op(char c, char * input){
  if(c == '|' && *input == '|' ){
    return "||";
    input++;
  }
  else if(c == '&' && *input == '&'){
    return "&&";
    input++;
  }
  else if(isword(*input) || strchr("\t ", *input)){
    switch(c){
      case ';' : return ";";
      case '|' : return "|";
      case '&' : return "&";
    }
  }
  return NULL;
}

void syntax_error(char col){
  printf("syntax error near `%c'\n", col );
  exit(1);
}

bool isblankstr(char * stream){
  if(strlen(stream) == 0)
    return true;
  int i=0;
  bool blank = true;
  for (i = 0; i < strlen(stream); i++){
    //if(stream[i] != ' ' || stream[i] != '\t')
    if(!strchr("\t \0",stream[i]))
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
enum { SEMICOLON, ANDOR, PIPE};
int op_preced(char * a, char * b){
  int op1; int op2;
  if(a == ";")
    op1 = SEMICOLON;
  else if( a == "&&" || a == "||")
    op1 = ANDOR;
  else if(a == "|")
    op1 = PIPE;
  if(b == ";")
    op1 = SEMICOLON;
  else if( b == "&&" || b == "||")
    op1 = ANDOR;
  else if(b == "|")
    op1 = PIPE;

  return op2 - op1;
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
  command_t cmd = (command_t) malloc(sizeof(struct command));
  cmd->type = SIMPLE_COMMAND;
  cmd->status = -1;
  cmd->u.word = (char**)checked_malloc((wordcount+1)*sizeof(char**));

  for(i=0; i < wordcount; i++){
    cmd->u.word[i] = words[i];
  }

  cmd->u.word[i] = 0; //needed for execvp
  return cmd;
}
typedef enum command_type cmd_type;
cmd_type get_cmd_type(STRING op){
  if(op == "&&") return AND_COMMAND;
  if(op == "||") return OR_COMMAND; 
  if(op == ";" ) return SEQUENCE_COMMAND;
  if(op == "|" ) return PIPE_COMMAND;
}
command_t combine_commands(command_t cmd_left, command_t cmd_right, STRING op){
  cmd_type type = get_cmd_type(op);
  command_t cmd = (command_t)checked_malloc(sizeof(struct command));
  cmd->type = type;
  cmd->status = -1;
  cmd->output = NULL;
  cmd->input = NULL;
  cmd->u.command[0] = cmd_left;
  cmd->u.command[1] = cmd_right;

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
    //if word, space, or tab, put it in the buffer
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
    else if(isspecial(c)){
      //1)
      if(!isblankstr(buffer)){
        command_t cmd_a = make_simple_command(buffer);
        if(cmd_a)
          cmd_list_push(cmd_list, cmd_a);
        b = 0;
        buffer = NULL;
        chunk = 16;
      }
      // if the left hand buffer of operator is empty it's a syntax error
      else
        syntax_error(c);
      // 2)
      if( c == '('){
        str_list_push(op_list, "(");
        buffer = NULL;
        chunk = 16;
        b = 0;
        continue;
      }
      // 3)
      else if(isoperator(c)){
        // 3-a)if operator stack is empty
        //input is the next character
        char * op = make_op(c, input);
        if(op_list->size == 0){          
          str_list_push(op_list, op);
          buffer = NULL;
          chunk = 16;
          b = 0;
          continue;
        }
        // 4-a) if operator stack is not empty
        else{
          // start popping high precedence ops
          while(op_preced(op, str_list_peek(op_list) ) >= 0 &&
               op_list->size > 0 && str_list_peek(op_list) != "("){

            char * op = str_list_pop(op_list);
            if(cmd_list->size == 0)
              syntax_error(c);
            //4a) ++ continued
            command_t cmd_right = cmd_list_pop(cmd_list);
            command_t cmd_left = cmd_list_pop(cmd_list);
            command_t cmd_combined = combine_commands(cmd_left, cmd_right, op);
            cmd_list_push(cmd_list, cmd_combined);
          }
          //4c)
          str_list_push(op_list, op);
          
          //STRING top_op = str_list_pop(op_list);
        }
      }


      
    }
  }
  //printf("buffer =%s, size=%d\n", buffer, b);
  if(!isblankstr(buffer)){
    buffer[b] = c;
    command_t cmd_b = make_simple_command(buffer);
    //command_t cmd_b = NULL;
    if(cmd_b)
      cmd_list_push(cmd_list, cmd_b);
  }
  //printf("cmd size: %d\n", cmd_list_size(cmd_list));
  //printf("%s\n", cmd_list_peek(cmd_list)->u.word[0]);
  cmd_list->cursor = cmd_list->head;
  int i=0;
  for(i=0;i< cmd_list_size(cmd_list); i++){
    printf("%s\n", cmd_list->cursor->cmd->u.word[0]);
    cmd_list->cursor = cmd_list->cursor->next;
  }

  //printf("%d\n", str_list_size(op_list));
  //str_list_pop(op_list);
  printf("%s\n",str_list_peek(op_list));


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
