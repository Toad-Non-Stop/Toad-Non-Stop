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
#include <fcntl.h>
#include <limits.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

int comparator(const void* x, const void* y)
{
	return (*((int *) y) - *((int *) x));
}



int main(int argc, char *argv[])
{
	int sockfd;
	int port = atoi(argv[1]);
	struct sockaddr_in servaddr;

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Socket creation failed \n");
		exit(1);
	}

	if (bind(sockfd, (struct sockaddr*) & servaddr, sizeof(servaddr)) < 0)
	{
		servaddr.sin_port = 0;

		if (bind(sockfd, (struct sockaddr*) & servaddr, sizeof(servaddr)) < 0)
		{
			perror("bind failed");
			exit(1);
		}
	}

	if (listen(sockfd, 10) < 0)
	{
		perror("Listen failed \n");
		close(sockfd);
		exit(1);
	}

	socklen_t server_length = sizeof(servaddr);
	getsockname(sockfd, (struct sockaddr*) & servaddr, &server_length);
	printf("Server start on port: %d\n", ntohs(servaddr.sin_port));

	while (1)
	{
		struct sockaddr_in cliaddr;
		socklen_t client_lenght = sizeof(cliaddr);
		int newsockfd = accept(sockfd, (struct sockaddr*) & cliaddr, &client_lenght);
		int lenght;
		read(newsockfd, &lenght, sizeof(int));

		int mas[lenght];
		read(newsockfd, &mas, lenght * sizeof(int));

		qsort(mas, lenght, sizeof(int), comparator);
		write(newsockfd, mas, lenght * sizeof(int));
		printf("Sort sent.\n");

		close(newsockfd);
		return 0;
	}
}



















