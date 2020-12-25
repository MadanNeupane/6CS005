#include <stdio.h>
#include <cuda_runtime_api.h>
#include <time.h>

/******************************************************************************

  Compile the program:
    nvcc -o PasswordCrackUsingCuda PasswordCrackUsingCuda.cu 

  Run the program:
	./PasswordCrackUsingCuda > 3.1.txt

******************************************************************************/

__device__ int is_matched(char *password)
{
	char password_4[] = "CS2505";
	char password_2[] = "HP7719";
	char password_1[] = "MN0009";
	char password_3[] = "NY2021";

	char *p1 = password;
	char *p2 = password;
	char *p3 = password;
	char *p4 = password;

	char *w1 = password_1;
	char *w2 = password_2;
	char *w3 = password_3;
	char *w4 = password_4;

	while (*p1 == *w1)
	{

		if (*p1 == '\0')
		{

			printf("Password Found : %s\n", password_1);
			return 1;
		}
		p1++;
		w1++;
	}

	while (*p2 == *w2)
	{

		if (*p2 == '\0')
		{

			printf("Password Found : %s\n", password_2);
			return 1;
		}
		p2++;
		w2++;
	}

	while (*p3 == *w3)
	{

		if (*p3 == '\0')
		{

			printf("Password Found : %s\n", password_3);
			return 1;
		}
		p3++;
		w3++;
	}

	while (*p4 == *w4)
	{

		if (*p4 == '\0')
		{

			printf("Password Found : %s\n", password_4);
			return 1;
		}
		p4++;
		w4++;
	}
	return 0;
}

__global__ void kernel()
{

	char alphabets[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

	char numbers[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

	char password[7];
	password[6] = '\0';

	int a, b, c, d;

	for (a = 0; a < 10; a++)
	{

		for (b = 0; b < 10; b++)
		{

			for (c = 0; c < 10; c++)
			{

				for (d = 0; d < 10; d++)
				{

					password[0] = alphabets[blockIdx.x];
					password[1] = alphabets[threadIdx.x];
					password[2] = numbers[a];
					password[3] = numbers[b];
					password[4] = numbers[c];
					password[5] = numbers[d];

					if (is_matched(password))
					{

						printf("Password Found: %s\n", password);
					}
				}
			}
		}
	}
}

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

int main(int argc, char *argv[])
{
	struct timespec start, finish;
	long long int time_elapsed;

	clock_gettime(CLOCK_MONOTONIC, &start);

	kernel<<<26, 26>>>();
	cudaDeviceSynchronize();

	clock_gettime(CLOCK_MONOTONIC, &finish);
	time_difference(&start, &finish, &time_elapsed);
	printf("Time Elapsed: %lldns or %0.9lfs\n",
		   time_elapsed, (time_elapsed / 1.0e9));

	return 0;
}
