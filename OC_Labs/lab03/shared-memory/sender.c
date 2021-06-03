#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>

const int NUMBER = 20;

int main()
{
  srand((unsigned)(time(0)));
  int memId = shmget(IPC_PRIVATE, sizeof(int) * NUMBER, 0600|IPC_CREAT|IPC_EXCL);
  int* array = (int*)shmat(memId, 0, 0);
  for (size_t i = 0; i < NUMBER; i++)
  {
     array[i] = rand() % 300;
  }
  char callbuf[1024];
  sprintf(callbuf, "./receiver %i %i", memId, NUMBER);
  system(callbuf);
 
  return 0;
}

