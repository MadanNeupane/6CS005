#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>
#include <time.h>

/******************************************************************************
This program uses "brute force" algorithm which is often called trial and error method. Works on passwords that consist only of 3 uppercase letters and a 2 digit integer.


  Compile the program:
    cc -o Three-Initials-Two-Digits Three-Initials-Two-Digits.c -lcrypt

  Run the program:
    ./Three-Initials-Two-Digits > 2.3.3txt


******************************************************************************/
int total_passwords = 4;

char *three_initials[] = {
    "$6$AS$1mYI1AoWnumQr1TU5VoPouGGT0IGe4jKnKYY6SS7o1pbKWAJz.19AXQqRQHgH9Hwp3Zgy.MsRuZj/bHdvcNS41",
    "$6$AS$rO24ikvdmsECaZyd.CCYw0tZbkaFSRVtP6.wo5x0YpAj0x9MM0.yUnzNyO5.7S3nL3lhe51pFAJPsiXqDUhOw.",
    "$6$AS$0GXUr.SZM.cvaxC1VrWK4BfmxNdnJkOK2d8IDYtuRB24JZFQs0USc7NbHvurd6MQDb0gcdNa9qFNE9R9W/TMv0",
    "$6$AS$354OkqXmJ.3nx3eks0eFxZed0lg91NNwt5E533IDXd.tvqIEMV/hOwafCYiT9KCRGfHauhqJ32JTBLQxMNE2G/"};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length)
{
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
This function "crack" can crack the kind of password explained above. All combinations
 that are tried are showed and when the password is found, it is denoted by #, it is put at the 
 start of the line. 
*/

void crack(char *salt_and_encrypted)
{
  int a, b, c, d; // Loop counters
  char salt[7];   // String used in hashing the password. Need space for \0
  char plain[7];  // The combination of letters currently being checked
  char *enc;      // Pointer to the encrypted password
  int count = 0;  // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for (a = 'A'; a <= 'Z'; a++)
  {
    for (b = 'A'; b <= 'Z'; b++)
    {
      for (c = 'A'; c <= 'Z'; c++)
      {
        for (d = 0; d <= 99; d++)
        {
          sprintf(plain, "%c%c%c%02d", a, b, c, d);
          enc = (char *)crypt(plain, salt);
          count++;
          if (strcmp(salt_and_encrypted, enc) == 0)
          {
            printf("#%-8d%s %s\n", count, plain, enc);
          }
          else
          {
            printf(" %-8d%s %s\n", count, plain, enc);
          }
        }
      }
    }
  }
  printf("%d Solutions Explored\n", count);
}

//Calculating time difference
int time_difference(struct timespec *start, struct timespec *finish,
                    long long int *difference)
{
  long long int ds = finish->tv_sec - start->tv_sec;
  long long int dn = finish->tv_nsec - start->tv_nsec;

  if (dn < 0)
  {
    ds--;
    dn += 1000000000;
  }
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

//main
int main()
{
  int k;
  struct timespec start, finish;
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);

  //loops for encrypted passwords
  for (k = 0; k < total_passwords; k < k++)
  {
    crack(three_initials[k]);
  }

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time Elapsed: %lldns or %0.9lfs\n", time_elapsed,
         (time_elapsed / 1.0e9));

  return 0;
}
