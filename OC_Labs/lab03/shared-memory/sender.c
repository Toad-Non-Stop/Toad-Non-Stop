#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>

const int NUMBER = 20;

int main()
{
	srand((unsigned)(time(0))); //для случайных значений в rand()
	//создание анонимного сегмента разделяемой памяти и возвращение его идентификатора
	int memId = shmget(IPC_PRIVATE, sizeof(int) * NUMBER, 0600|IPC_CREAT|IPC_EXCL); 
	int* array = (int*)shmat(memId, 0, 0); //привязка зарезервированного места к массиву
	
	//запись 20 рандомных чисел в массив
	for (size_t i = 0; i < NUMBER; i++)
	{
		//присвоение ячейке массива остатка от деления рандомного числа на 300
		array[i] = rand() % 300; 
	}
	char callbuf[1024];
	sprintf(callbuf, "./receiver %i %i", memId, NUMBER); //сохранение строки в буфер
	system(callbuf); //вызов файла receiver с параметрами memId & NUMBER

	return 0;
}

