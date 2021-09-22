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
#include <time.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
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

	if (bind(sockfd, (const struct sockaddr*) & servaddr, sizeof(servaddr)) < 0)
	{
		servaddr.sin_port = 0;

		if (bind(sockfd, (const struct sockaddr*) & servaddr, sizeof(servaddr)) < 0)
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

	if (fork() == 0)
	{
		while (1)
		{
			int newsockfd;
			struct sockaddr_in cliaddr;
			socklen_t client_lenght = sizeof(cliaddr);
			if ((newsockfd = accept(sockfd, (struct sockaddr*) & cliaddr, &client_lenght)) < 0)
			{
				perror("Accept failed");
				close(sockfd);
				exit(1);
			}
			if (fork() == 0)
			{
				continue;
			}
			while (1)
			{
				char in[256];
				int prov = read(newsockfd, in, sizeof(in));
				if (prov == 0)
				{
					close(newsockfd);
					exit(0);
				}
				if (in[0] == '.')
				{
					printf("Close \n");
					close(newsockfd);
					exit(0);
				}
				
				FILE* find_file = fopen(in, "r");
				if (find_file == NULL)
				{
					long find_s = -1;
                    printf("fopen error \n");
					write(newsockfd, &find_s, sizeof(find_s));
				}
				else
				{
					
					long start = ftell(find_file);
					fseek(find_file, 0L, SEEK_END);
					long end = ftell(find_file);
					fseek(find_file, start, SEEK_SET);

					long find_s = end;
					write(newsockfd, &find_s, sizeof(find_s));

					char buffer[find_s];
					fread(buffer, sizeof(char), find_s, find_file);
					fclose(find_file);
					write(newsockfd, buffer, sizeof(buffer));
				}
			}

		}

	}
	else
	{
		printf("Available commands: \n");
		printf("exit - closing the server program\n");
		char commands[100];
		while(1)
		{
			scanf("%s", commands);
			if (strcmp(commands, "exit") == 0)
			{
				close(sockfd);
				exit(0);
			}
			else
			{
				printf("I don't know this command. \n");
				printf("Try to enter the command again. \n");
			}
		}
	}

}
