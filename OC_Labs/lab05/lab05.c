#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

const char* NAME = "stroka";

int comparator(const void* a, const void* b)
{
    return (*((int*)b) - *((int*)a));
}

int* randomazing(int N)
{
    srand((unsigned) (time(0)));

    int *array = malloc(sizeof(int) * N);
    for (int i = 0; i < N; ++i)
    {
        array[i] = rand() % 300;
    }
    
    return array;
}

void Out_numbers(int* array, int N)
{
    for (int i = 0; i < N; ++i)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main(int argv, char *argc[])
{
    int N = atoi(argc[1]);
    int *array = randomazing(N);
    Out_numbers(array, N);

    mknod(NAME, S_IFIFO | 0666, 0);
    int p[2];
    pipe(p);

    int childId = fork();

    if (childId == 0)
    {
        close(p[1]);
        int fifo = open(NAME, O_WRONLY);
        int *mem = malloc(sizeof(int) * N);
        read(p[0], mem, sizeof(int) * N);
        close(p[0]);
        qsort(mem, N, sizeof(int), comparator);
        write(fifo, mem, sizeof(int) * N);
    }
    else if (childId > 0)
    {
        close(p[0]);
        int fifo = open(NAME, O_RDONLY);
        write(p[1], array, sizeof(int) * N);
        close(p[1]);
        int *sortirovka = malloc(sizeof(int) * N);
        read(fifo, sortirovka, sizeof(int) * N);
        Out_numbers(sortirovka, N);
        unlink(NAME);
    }
    else if (childId < 0)
    {
        perror("error with fork()\n");
    }
}
     
    

