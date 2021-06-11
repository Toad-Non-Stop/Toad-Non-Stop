#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//функция выводит "Hello Threads(..)" 10 раз, где вместо точек номер итерации цикла
void *mission1()
{
	for (int i = 1; i <= 10; i++)
	{
		printf("Hello Threads(%i), ", i);
		sleep(1); //задержка на 1мс
	}
}

//функция выводит "This is iteration .." 10 раз, где вместо точек номер итерации цикла
void *mission2()
{
	for (int i = 1; i <= 12; i++)
	{
		printf("This is iteration %i, ", i);
		sleep(2); //задержка на 2 мс
	}
}


int main()
{
	pthread_t thread1, thread2; // объявление идентификатора потока

	//создание двух потоков для выполнения фунций mission1 & mission2 
	int res1 = pthread_create(&thread1, NULL, mission1, NULL);
	int res2 = pthread_create(&thread2, NULL, mission2, NULL);

	//ожидание завершения выполнения функций в потоках
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("\n");

	return 0;
}
