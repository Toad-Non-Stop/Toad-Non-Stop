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
#include <sys/mman.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>
#include <string>
#include <iostream>

int count(char* str) {
	int i=0;
	while(str[i]!='\0')
	{
		i++;
	}
	return i;
}

void game(int listenfd, int cl1, int cl2) {
    while (1) {
		int check = 0;
		check = htons (check);
		write (cl1, &check, sizeof(int)); // отпраляем статус начала0
		check = 1;
		check = htons (check);
		write (cl2, &check, sizeof(int)); // отпраляем статус начала1
		char word[100];
		read (cl1, &check, sizeof(int)); // получаем длинну строки
		check = ntohs (check);
		int lengt = check;
		read (cl1, word, (lengt+1)*sizeof(char)); //получем само слово
		std::cout << "Word: " << word << '\n';
		if(word[0] == '/' && word[1] == 's') {
			std::cout << "Wait for new connection" << '\n';
			cl1 = accept(listenfd, (struct sockaddr*)NULL, NULL); //ждем подключеня 1 клиента
			check = 0;
			check = htons (check);
			write (cl1, &check, sizeof(int));
			read (cl1, &check, sizeof(int)); // получаем длинну строки
			check = ntohs (check);
			lengt = check;
			read (cl1, word, (lengt+1)*sizeof(char)); //получем само слово
			std::cout << "Word: " << word << '\n';
		}
		printf("%d\n", lengt);
		lengt = htons (lengt);
		write (cl2, &lengt, sizeof(int)); // прередаем длинну слова угадывающему
		lengt = ntohs (lengt);
		int failAtt = 0;
		while (1)
		{
			char leter;
			read (cl2, &leter, sizeof(char)); // читаем букву

			//конец игры при победе первого и свап клиентов
			if (leter == '\0') {
				check = 0;
				check = htons(check);
				write (cl1, &check, sizeof(int)); // отправляем первому сообщ о победе
				check = ntohs(check);
				int swap;
				swap = cl1;
				cl1 = cl2;
				cl2 = swap;
				break;
			}

			//конец игры при победе второго и свап клиентов
			if(failAtt == 5) {
				check = 9999;
				check = htons(check);
				write (cl2, &check, sizeof(int)); // отправляем проигрыш
				check = 1;
				check = htons(check);
				write (cl1, &check, sizeof(int)); // отправляем первому сообщ о проирыше
				check = ntohs(check);
				int swap;
				swap = cl1;
				cl1 = cl2;
				cl2 = swap;
				break;
			}

			int numLet = 0;
			for (int i=0; i<lengt; i++)
			{
				if (word[i]==leter) numLet++;
			}
			numLet = htons(numLet);
			write (cl2, &numLet, sizeof(int)); // отправляем число найденных букв

			if (numLet == 0) failAtt++;

			for (int i=0; i<lengt; i++)
			{
				if (word[i]==leter) 
				{
					i=htons(i);
					write (cl2, &i, sizeof(int)); // отправляем координаты каждой буквы
					i=ntohs(i);
				}
			}
		}
    }
}




char words[][100] = {"pain", "shoot", "dead", "fun", "illusion", "demention", "implementation"};
int numword = 7;
void gamePs(int listenfd, int cl1) {
	while (1) {
		int check = 1;
		check = htons (check);
		write (cl1, &check, sizeof(int)); // отпраляем статус начала0
		srand((unsigned) (time(0)));
		char* word = words[rand()%numword];
		int lengt = count(word);
		std::cout << "Word: " << word << '\n';
		printf("%d\n", lengt);
		lengt = htons (lengt);
		write (cl1, &lengt, sizeof(int)); // прередаем длинну слова угадывающему
		lengt = ntohs (lengt);
		int failAtt = 0;
		while (1)
		{
			char leter;
			read (cl1, &leter, sizeof(char)); // читаем букву
			if (leter == '\0') {
				break;
			}
			if(failAtt == 5) {
			check = 9999;
			check = htons(check);
			write (cl1, &check, sizeof(int)); // отправляем проигрыш
				break;
			}
			int numLet = 0;
			for (int i=0; i<lengt; i++)
			{
				if (word[i]==leter) numLet++;
			}
			numLet = htons(numLet);
			write (cl1, &numLet, sizeof(int)); // отправляем число найденных букв
			if (numLet == 0) failAtt++; //увеличение количества ошибок
			for (int i=0; i<lengt; i++)
			{
				if (word[i]==leter) 
				{
					i = htons(i);
					write (cl1, &i, sizeof(int)); // отправляем координаты каждой совпадающей буквы
					i = ntohs(i);
				}
			}
		}
    }
}



int main(int argc, char *argv[]) {
	int listenfd = 0;
	struct sockaddr_in serv_addr;
	char sendBuff[1025];
	time_t ticks;
	listenfd = socket(AF_INET, SOCK_STREAM, 0); //инициализация сокета
	memset(&serv_addr, '0', sizeof(serv_addr)); //обнуление serv_addr
	memset(sendBuff, '0', sizeof(sendBuff)); //обнуление sendBuff

	//настройки сокета
	serv_addr.sin_family = AF_INET; //IPv4
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //ip
	serv_addr.sin_port = htons(atoi(argv[1])); //порт
	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); //состыковка сокета и настройки сокета
	listen(listenfd, 10); //прослушивание сокета максимум на 10 клиентов

	int cl1 = accept(listenfd, (struct sockaddr*)NULL, NULL); //ждем подключеня 1 клиента
	int check;
	read (cl1, &check, sizeof(int)); //получение типа игры для первого клиента
    check = ntohs(check);

	if (check == 1) {
    	int cl2 = accept(listenfd, (struct sockaddr*)NULL, NULL); //ждем подключеня 2 клиента
        game(listenfd, cl1, cl2);
	}
	else
		gamePs(listenfd, cl1);
}
