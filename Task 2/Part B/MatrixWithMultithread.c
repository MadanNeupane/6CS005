#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_SIZE 1024

/******************************************************************************

  Compile the program:
    cc -o MatrixWithMultithread MatrixWithMultithread.c -pthread

  Run the program:
    ./MatrixWithMultithread

******************************************************************************/

typedef struct row_cols
{
    int x, y;
} args;

int matrix1[MAX_SIZE][MAX_SIZE];
int matrix2[MAX_SIZE][MAX_SIZE];
int result[MAX_SIZE][MAX_SIZE];

int max_thread;

void *multiply_param(void *arg)
{
    args *param = arg;

    for (int c = 0; c < MAX_SIZE; c++)
    {
        result[param->x][param->y] += matrix1[param->x][c] * matrix2[c][param->y];
    }
}

void multiply_matrix()
{

    int x = 0, y = 0;

    while (x < MAX_SIZE)
    {
        y = 0;

        while (y < MAX_SIZE)
        {
            pthread_t threads[max_thread];
            args param[max_thread];

            for (int i = 0; i < max_thread; i++)
            {

                if (y >= MAX_SIZE)
                {
                    break;
                }
                param[i].x = x;
                param[i].y = y;
                pthread_create(&threads[i], NULL, multiply_param, (void *)&param[i]);
                pthread_join(threads[i], NULL);
                y++;
            }
        }
        x++;
    }
}

int time_difference(struct timespec *start, struct timespec *finish, long long int *difference)
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

int main()
{

    for (int i = 0; i < MAX_SIZE; i++)
    {
        for (int j = 0; j < MAX_SIZE; j++)
        {
            matrix1[i][j] = rand() % 10;
            matrix2[i][j] = rand() % 10;
            result[i][j] = 0;
        }
    }

    printf("Enter the desired number of threads: ");
    scanf("%d", &max_thread);

    printf("Matrix1:%ld\nMatrix2:%ld\n", sizeof(matrix1) / sizeof(matrix1[0]), sizeof(matrix2) / sizeof(matrix2[0]));
    printf("Threads:%d\n", max_thread);

    struct timespec start, finish;
    long long int time_elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);
    clock_gettime(CLOCK_MONOTONIC, &finish);

    multiply_matrix();

    time_difference(&start, &finish, &time_elapsed);
    printf("Time Elapsed: %lldns or %0.9lfs\n", time_elapsed, (time_elapsed / 1.0e9));
}

