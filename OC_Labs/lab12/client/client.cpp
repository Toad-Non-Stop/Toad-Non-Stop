#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char* argv[])
{
	char* addr = argv[1];
	int port = atoi(argv[2]);
	int sockfd;
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
	printf("Available commands\n");
	printf("exit - closing the client program\n");
	printf("Enter the file name or command: \n");
	char buffer[256];
	while (1)
	{
		scanf("%s", buffer);
		if (strcmp(buffer, "exit") == 0)
		{
			close(sockfd);
			exit(0);
		}
		else
		{
			long find_s;
			write(sockfd, buffer, strlen(buffer) + 1);
			int prov = read(sockfd, &find_s, sizeof(find_s));
			if (prov == 0)
			{
				close(sockfd);
				exit(0);
			}
			if (find_s < 0)
			{
				printf("The file was not found on the server. \n");
			}
			else
			{
				printf("The file is found. \n");
				printf("File download... \n");

				long fsize = find_s;
				char package[fsize];
				bzero(package, fsize);
				FILE* find_file = fopen(buffer, "w");
				if (find_file == NULL)
				{
					printf("Error opening package. \n");
					exit(0);
				}
				for (int i = 0; i < fsize; i++)
				{
					printf("%c", package[i]);
				}
				printf("\n");
				int p = 0;
				while (p < fsize)
				{
					p += read(sockfd, package, sizeof(package));
					fprintf(find_file, "%s", package);
				}
				for (int i = 0; i < fsize; i++)
				{
						printf("%c", package[i]);
				}
				printf("\n");
				fclose(find_file);
				printf("Package received. File: %s!\n", buffer);
			}
		}
	}
	close(sockfd);
}


