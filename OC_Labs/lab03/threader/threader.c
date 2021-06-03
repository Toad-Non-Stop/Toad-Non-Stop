#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *mission1()
{
  for (int i = 1; i <= 10; i++)
  {
     printf("Hello Threads(%i), ", i);
     sleep(1);
  }
}

void *mission2()
{
  for (int i = 1; i <= 12; i++)
  {
     printf("This is iteration %i, ", i);
     sleep(2);
  }
}


int main() 
{
  pthread_t thread1, thread2;
  int res1 = pthread_create(&thread1, NULL, mission1, NULL);
  int res2 = pthread_create(&thread2, NULL, mission2, NULL);
  
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  printf("\n");
  
  return 0;
}
