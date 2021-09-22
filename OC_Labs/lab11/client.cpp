#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>

void Random_array(int *array, int N, int Min, int Max)
{
	srand((unsigned)(time(0)));
	for (int i = 0; i < N; i++)
	{
		array[i] = Min + rand() % (Max - Min + 1);
	}
}

void Print_array(int* array, int p)
{
	for (size_t i = 0; i < p; i++)
	{
		printf("%d ", array[i]);
	}
	printf("\n");
}


int main(int argc, char* argv[])
{
	int Size = 0, Max, Min;

	while (Size <= 0)
	{
		printf("Введите размер массива: ");
		scanf("%d", &Size);
	}
	printf("Введите минимальное значение массива: ");
	scanf("%d", &Min);
	printf("Введите максимальное значение массива: ");
	scanf("%d", &Max);

	char* addr = argv[1];
	int port = atoi(argv[2]);
	int sockfd;
	char sendline[Size * sizeof(int)], recvline[Size * sizeof(int)];
	struct sockaddr_in servaddr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Socket creation failed \n");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = inet_addr(addr);

	if (connect(sockfd, (struct sockaddr*) & servaddr, sizeof(servaddr)) < 0)
	{
		perror("Connect faild \n");
		close(sockfd);
		exit(1);
	}

	printf("Connection sucsess\n");

	Random_array((int*)sendline, Size, Min, Max);
	printf("Сгенерированый массив случайных чиссел: \n");
	Print_array((int*)sendline, Size);

	write(sockfd, &Size, sizeof(int));
	write(sockfd, sendline, sizeof(sendline));
	printf("Array sent. \n");
	time_t startTime = time(NULL);

	int prov = read(sockfd, recvline, sizeof(recvline));
	if (prov == 0)
	{
		perror("Read error \n");
		close(sockfd);
		exit(0);
	}
	printf("Sorted array received.\n");
	time_t endTime = time(NULL);
	printf("Отсортированный массив чисел:\n");
	Print_array((int*)recvline, Size);

	if (endTime - startTime > 0)
	{
		printf("На работу затрачено %ld секунд\n", endTime - startTime);
	}
	else
	{
		printf("На работу затрачено менее 1 секунды\n");
	}
	close(sockfd);
	return 0;

}









	

