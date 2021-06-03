#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

const int NUMBER = 20;


int comparator(const void* a, const void* b)
{
  return ( *((int*)a) - *((int*)b) );
}


void Sortirovka(int *mem)
{
  qsort(mem, NUMBER, sizeof(int), comparator);
  for (size_t i = 0; i < NUMBER; i++)
  {
     printf("%d ", mem[i]);
  }
  printf("\n");

}


int main()
{
  srand((unsigned)(time(0)));
  int memId = shmget(IPC_PRIVATE, sizeof(int) * NUMBER, 0600|IPC_CREAT|IPC_EXCL);
  int* array = (int*)shmat(memId, 0, 0);
  for (size_t i = 0; i < NUMBER; i++)
  {
     array[i] = rand() % 300;
     printf("%d ", array[i]);
  }
  printf("\n");


  pid_t childId = fork();
  if (childId < 0)
  {
     perror("error with fork()\n");
  }

  else if (childId > 0)
  {
     waitpid(childId, NULL, 0);
  }

  else if (childId == 0)
  {
    Sortirovka(array);
  }
  
  shmdt(array);

  return 0;
}

