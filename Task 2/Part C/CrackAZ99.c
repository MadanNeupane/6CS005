#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>
#include <time.h>

/******************************************************************************
  

This program uses "brute force" algorithm which is often called trial and error method. Works on passwords that consist only of 2 uppercase letters and a 2 digit integer.

  Compile the program:
    cc -o CrackAZ99 CrackAZ99.c -lcrypt

  Run the program:
    ./CrackAZ99 > 2.3.1.txt

******************************************************************************/
int total_passwords = 4;

char *encrypted_passwords[] = {
    "$6$AS$RpBqfxKaD5wax.zvXKR/mSE0xfLqIawh1hJe6wqsv4yAvijb9.5Pzi.3He9RXuSR8F6D6brdsSuTANEkbvQ7J/",
    "$6$AS$Wq/Hj4KhNwZr1YN4mADffSKg9o.MeOjfguBTYt6T/E3uoqVNCHgxusgP1/eCHI/69I81SCk7qGGVGqhDzeME41",
    "$6$AS$.SlxNBSvDk.5HkSmVRwAvM.Xpfz953CMSRhi48i8FoNrzygWJwZMN7qjnzbmc9H6tbmXzG2jzfEolhLZz3IF90",
    "$6$AS$szID7Ehv2yOOhoBVvtSQlqHvrG/LvxfFS2hHTGZ.9klASTOn37.EU4y.5GyYH26S6.v6eQyAeQKCKICGNPHvx1"};

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
  int a, b, c;   // Loop counters
  char salt[7];  // String used in hashing the password. Need space for \0
  char plain[7]; // The combination of letters currently being checked
  char *enc;     // Pointer to the encrypted password
  int count = 0; // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for (a = 'A'; a <= 'Z'; a++)
  {
    for (b = 'A'; b <= 'Z'; b++)
    {
      for (c = 0; c <= 99; c++)
      {
        sprintf(plain, "%c%c%02d", a, b, c);
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
  printf("%d Solutions Explored:\n", count);
}

//calculating the time difference
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
    crack(encrypted_passwords[k]);
  }

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time Elapsed: %lldns or %0.9lfs\n", time_elapsed,
         (time_elapsed / 1.0e9));

  return 0;
}
