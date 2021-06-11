#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <string.h>
#include <time.h>

//функция инициализации семафора
void initSemValue(int semId, int n)
{
	struct sembuf op; //создание переменной
	//инициализация полей структуры
	op.sem_op = 1;
	op.sem_flg = 0;
	op.sem_num = n;
	semop(semId, &op, 1); //инициализация семафора
}

//функция попытки ожидания семафора
void tryToGetSemOrWait(int semId, int n)
{
	struct sembuf op;
	op.sem_op = -1;
	op.sem_flg = 0;
	op.sem_num = n;
	semop(semId, &op, 1);
}

//создание семафора
void releaseSem(int semId, int n)
{
	initSemValue(semId, n);
}

int main(int argc, char* argv[])
{
	//получение значений аргументов
	const int N = atoi(argv[1]);
	const int min = atoi(argv[2]);
	const int max = atoi(argv[3]);

	int memId = shmget(IPC_PRIVATE, 4 * N, 0600 | IPC_CREAT | IPC_EXCL); //получение идентификатора зарезервированного участка памяти
	if (memId <= 0) //ошибка резервирования
	{
		printf("error with shmget()\n");
		return -1;
	}

	int* memNumbers = (int *)shmat(memId, 0, 0); //объявление массива в зарезервированном участке памяти
	if (NULL == memNumbers) //ошибка создания массива
	{
		printf("error with shmat()\n");
		return -2;
	}

	printf("Initial data: \n");
	srand((unsigned)(time(0)));
	for (int i = 0; i < N; i++) //заполнение массива данными
	{
		memNumbers[i] = min + rand() % (max - min + 1);
		printf("%d ", memNumbers[i]);
	}
	printf("\n");

	const size_t semCount = N; //количество семафоров (получается из аргументов)
	int semId = semget(IPC_PRIVATE, semCount, 0600 | IPC_CREAT); //создание семафоров
	if (semId < 0) //ошибка создания семафоров
	{
		perror("error with semget()");
		return -1;
	}
	else
	{
		printf("semaphore set id = %i\n", semId);
	}

	pid_t childId = fork(); //создание процесса-потомка, аналогичного нынешнему процессу

	if (childId < 0) //ошибка создания процесса-потомка
	{
		perror("error with fork()\n");
	}

	else if (childId > 0) //процесс-родитель
	{
		for (int i = 0; i < N; ++i)
		{
			printf("Parent: Freeing semaphore %i\n", i);
			releaseSem(semId, i); //инициализация семафора
		}

		int status = 0;
		int m = 1;
		while (waitpid(-1, &status, 0 | WNOHANG) == 0)
		{
			printf("Step %i: ", m);
			for (int i = 0; i < N; i++)
			{
				tryToGetSemOrWait(semId, i);
				printf("%d ", memNumbers[i]);
				releaseSem(semId, i);
			}
			printf("\n");
			m++;
		}
		printf("\nSort completed\n");
		for (int i = 0; i < N; i++)
		{
			tryToGetSemOrWait(semId, i);
			printf("%d\n", memNumbers[i]);
			releaseSem(semId, i);
		}

		shmctl(memId, IPC_RMID, NULL);
		semctl(semId, IPC_RMID, 0);
	}

	else if (childId == 0)
	{
		int key = 0;
		int first = 0;

		for (int second = 1; second < N; second++)
		{
			tryToGetSemOrWait(semId, second);
			key = memNumbers[second];
			releaseSem(semId, second);
			first = second - 1;

			while (first>=0 && memNumbers[first]>key)
			{
				tryToGetSemOrWait(semId, first);
				tryToGetSemOrWait(semId, first+1);
				memNumbers[first + 1] = memNumbers[first];
				releaseSem(semId, first);
				releaseSem(semId, first+1);
				first = first - 1;
				tryToGetSemOrWait(semId, first + 1);
				memNumbers[first + 1] = key;
				releaseSem(semId, first + 1);
			}
		}
		exit(0);
	}
}
