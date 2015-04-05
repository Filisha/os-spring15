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

bool isspecial(char c){
  switch c:
    case '|':
      return true;
    case '&&':
      return true;
    default:
      return false;
}
//get_next_byte is a function pointer that simply invokes getc()
command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  //get file size
  void * file_stream = get_next_byte_argument;

  int size = 0;
  while(!feof(file_stream)){
    size++;
    getc(file_stream);
  }
  rewind(file_stream);

  char * input = (char*) checked_malloc(size);

  int i = 0;
  while(!feof(file_stream)){
    input[i++] = getc(file_stream);
  }
  input[i-1] = '\0';
  //2 stacks to store commands and operators
  trim(input);

  char * stream = input;
  


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
  error (1, 0, "command reading not yet implemented");
  return 0;
}
