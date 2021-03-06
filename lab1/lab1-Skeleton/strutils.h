#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

//pre: gets operator char *
//post: returns 0 if equal, 1 if a>b, -1 if a<b
int op_cmp(char * a, char * b){
  if(a == b)
    return 0;
}

void rtrim(char *str)
{
  size_t n;
  n = strlen(str);
  while (n > 0 && isspace((unsigned char)str[n - 1])) {
    n--;
  }
  str[n] = '\0';
}
 
void ltrim(char *str)
{
  size_t n;
  n = 0;
  while (str[n] != '\0' && isspace((unsigned char)str[n])) {
    n++;
  }
  memmove(str, str + n, strlen(str) - n + 1);
}
 
void trim(char *str)
{
  rtrim(str);
  ltrim(str);
}
