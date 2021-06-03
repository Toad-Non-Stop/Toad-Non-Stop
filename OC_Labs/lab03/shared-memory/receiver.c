#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>


int comparator(const void* a, const void* b)
{
  return ( *((int*)a) - *((int*)b) );
}

int main (int argv, char* argc[])
{
  int memId = atoi(argc[1]);
  const int NUMBER = atoi(argc[2]);
  int* mem = (int*)shmat(memId, 0, 0);
  for (size_t i = 0; i < NUMBER; i++)
  {
     printf("%i ", mem[i]);
  }
  printf("\n");
  qsort(mem, NUMBER, sizeof(int), comparator);
  for (size_t i = 0; i < NUMBER; i++)
  {
     printf("%i ", mem[i]); 
  }  
  printf("\n");
  
  return 0;
}
