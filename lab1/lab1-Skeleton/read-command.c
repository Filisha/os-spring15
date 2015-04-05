// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"
#include "alloc.h"
#include "list.h"
//#include "strutils.h"

#include <error.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct command_stream{
  cmd_node_t * node;
};

bool isword(char c){
  return strchr("!%+,-./:@^_", c) || isalpha(c) || isdigit(c);
}

bool isnewop(char c){
  return strchr("|;&()\n",c);
}

command_t make_simple_command(char * stream){
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
  
  bool opflag = false;  
  int chunk = 16;
  char * buffer = (char*)checked_malloc(chunk);
  int b = 0; // buffer index
  char c;
  char pvc; //previous character
  while((c = get_next_byte(file_stream)) != EOF){
    if(isnewop(c)){
      command_t cmd_a = make_simple_command(buffer);
      cmd_list_push(cmd_list, cmd_a);
      if(opflag){
        opflag = false;
        switch(pvc){

        }
      }
      else{
        pvc = c; //in case of || and &&
        opflag = true;
        continue;
      }
    }   
    if(b >= chunk){
      chunk *= 2;
      buffer = (char*)checked_realloc(buffer, chunk*sizeof(char**));
    }
    buffer[b++] = c;
  }
  //printf("buffer = %s, size=%d\n", buffer, b);
  command_t cmd_a = make_simple_command(buffer);
  

  /*cmd_list_t cmd_list = cmd_list_new();
  cmd_list_push(cmd_list, cmd_a);
  printf("%s\n", cmd_list_peek(cmd_list)->u.word[0]);
  
  str_list_t op_list = str_list_new();
  str_list_push(op_list, "&&");
  STRING str = str_list_pop(op_list);
  printf("%d\n", str_list_size(op_list) );


  cmd_list_free(cmd_list);
  str_list_free(op_list);*/

  return 0;
}

command_t
read_command_stream (command_stream_t s)
{
  /* FIXME: Replace this with your implementation too.  */
  //error (1, 0, "command reading not yet implemented");
  return 0;
}


/*int size = 0;
  while(get_next_byte(file_stream) != EOF){
    size++;
  }
  rewind(file_stream);

  char * input = (char*) checked_malloc(size);

  int i = 0;
  char c;
  while((c = getc(file_stream) )!= EOF){
    input[i++] = c;
  }
  input[i] = '\0';
  //2 stacks to store commands and operators
  trim(input);*/

  //char * stream = input;

