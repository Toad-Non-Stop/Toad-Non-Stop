#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>

//функция для сортировки
int comparator(const void* a, const void* b)
{
  return ( *((int*)a) - *((int*)b) );
}

int main (int argv, char* argc[])
{
  int memId = atoi(argc[1]); //получение сегмента разделяемой памяти из принимаемых аргументов
  const int NUMBER = atoi(argc[2]); //получение размера массива из принимаемых аргументов
  int* mem = (int*)shmat(memId, 0, 0); //объявление массива, ссылаемого на тот же участок памяти, что и в прошлом файле
  //вывод массива, созданного в прошлом файле
  for (size_t i = 0; i < NUMBER; i++)
  {
     printf("%i ", mem[i]);
  }
  printf("\n");
  qsort(mem, NUMBER, sizeof(int), comparator); //сортировка массива по возрастанию
  //вывод отсортированного массива
  for (size_t i = 0; i < NUMBER; i++)
  {
     printf("%i ", mem[i]); 
  }  
  printf("\n");
  
  return 0;
}
