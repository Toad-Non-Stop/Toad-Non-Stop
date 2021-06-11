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
	qsort(mem, NUMBER, sizeof(int), comparator); //функция сортировки
	for (size_t i = 0; i < NUMBER; i++)
	{
		printf("%d ", mem[i]);
	}
	printf("\n");

}


int main()
{
	srand((unsigned)(time(0))); //для генерации случайных чисел
	int memId = shmget(IPC_PRIVATE, sizeof(int) * NUMBER, 0600|IPC_CREAT|IPC_EXCL); //резервирование памяти для массива
	int* array = (int*)shmat(memId, 0, 0); //объявление массива, ссылаемого на зарезервированный участок памяти
	//заполнение массива случайными числами и вывод этих чисел на экран
	for (size_t i = 0; i < NUMBER; i++)
	{
		array[i] = rand() % 300;
		printf("%d ", array[i]);
	}
	printf("\n");

	//создание новый процесс, аналогичный нынешнему
	pid_t childId = fork();
	if (childId < 0) //проверка на успешностсь создания процесса
	{
		perror("error with fork()\n");
	}

	else if (childId > 0) //если процесс - родитель
	{
		waitpid(childId, NULL, 0); //ожидание завершения потомка с id = childID
	}

	else if (childId == 0) //если процесс - потомок
	{
		Sortirovka(array); //сортировка массива потомка
	}

	shmdt(array); //отключение сегмента от адресного пространства процесса

	return 0;
}

