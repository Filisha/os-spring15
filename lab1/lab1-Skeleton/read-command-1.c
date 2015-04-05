// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"
#include "alloc.h"
#include "list.h"
#include "strutils.h"

#include <error.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct command_stream{
  node_t * node;
};

bool isword(char c){
  return strchr("!%+,-./:@^_", c) || isalpha(c) || isdigit(c);
}

bool isop(char c){
  return strchr("|;&()",c);
}

command_t make_simple_command(char** words, int wordcount){
  struct command* cmd = (struct command*) malloc(sizeof(struct command));
  cmd->type = SIMPLE_COMMAND;
  cmd->status = -1;
  cmd->output = NULL;
  cmd->u.word = (char**)checked_malloc(wordcount*sizeof(char**));
  int i=0;
  for(i=0; i < wordcount; i++){
    printf("%s ", words[i]);
    cmd->u.word[i] = words[i];
  }
  putchar('\n');

  cmd->u.word[wordcount] = 0;//for lab1b

  return cmd;
}
//get_next_byte is a function pointer that simply invokes getc()
command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  //get file size
  void * file_stream = get_next_byte_argument;

  bool wflag = false;
  bool lflag = false;
  enum command_type type;
  
  int chunk = 4;
  char * buffer;
  char ** words = (char**)checked_malloc(chunk*sizeof(char**));
  int i = 0;int w = 0;
  char c;
  while((c = get_next_byte(file_stream)) != EOF){
    if(strchr("\t ", c)){
      if(wflag){
        wflag = false;
        words[w++] = buffer;
        buffer = NULL;
      }
      continue;
    }

    else if(isword(c)){
      if(!wflag){
        i = 0;
        wflag = true;
        buffer = (char*)checked_malloc(chunk);
      }
      if(i >= chunk){
        chunk *= 2;
        buffer = (char*)checked_realloc(buffer, chunk*sizeof(char**));
      }
      buffer[i++] = c;
    }
    else if(isop(c)){
      char pc = c;
      if(!lflag && c == '|' || c == '&'){
        lflag = true;
        type = PIPE_COMMAND;
        continue;
      }
      if(lflag && pc == '|' && c == '|'){
        type = OR_COMMAND;
        lflag = false;
      }
      if(lflag && pc == '&' && c == '&'){
        type = AND_COMMAND;
        lflag = false;
      }
      if(c == ';'){
        type = SEQUENCE_COMMAND;
      }
      if(c == '('){
        type = SUBSHELL_COMMAND;
      }
    }
  }
  printf("%d\n", w);
  //last word
  if(buffer){
    words[w++] = buffer;
  }

  command_t cmd_a= make_simple_command(words, w);

  list_t * cmd_list = list_new();
  list_t * op_list = list_new();

  list_free(cmd_list);
  list_free(op_list);

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

