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


struct StrmsgBuf
{
	long mtype;
	int numbers[4];
};

struct Arguments
{
	int msgId;
};

int comparator(const void* i, const void* j)
{
	return *(int *)i - *(int *)j;
}



void swap(int* numbers, int i, int j)
{
	int s = numbers[i];
	numbers[i] = numbers[j];
	numbers[j] = s;
}

int nSet(int* a, int n)
{
	int j = n - 2;
	while (j != -1 && a[j] >= a[j + 1])
	{
		j--;
	}
	if (j == -1)
		return 0;
	int k = n - 1;
	while (a[j] >= a[k]) k--;
	swap(a, j, k);
	int l = j + 1, r = n - 1;
	while (l < r)
		swap(a, l++, r--);
	return 1;
}


void* child_thread(void* Arg)
{
	struct Arguments* Arguments = (struct Arguments*)Arg;
	int msgId = Arguments->msgId;
	struct StrmsgBuf Message;

	msgrcv(msgId, &Message, sizeof(Message), 1, 0);
	qsort(Message.numbers, 4, sizeof(int), comparator);

	Message.mtype = 101;
	msgsnd(msgId, &Message, sizeof(Message), 0);
	while (nSet(Message.numbers, 4))
	{
		Message.mtype = 101;
		msgsnd(msgId, &Message, sizeof(Message), 0);
	}

	Message.numbers[0] = -1;
	Message.mtype = 101;
	msgsnd(msgId, &Message, sizeof(Message), 0);
	return NULL;


}


void* parent_thread(void* Arg)
{
	int numbers[4];
	printf("Initial data: ");	
	srand((unsigned)(time(0)));
	for (size_t i = 0; i < 4; ++i)
	{
		numbers[i] = rand() % 10;
		printf("%d ", numbers[i]);
	}
	printf("\n");

	struct Arguments* args = (struct Arguments*)Arg;
	int msgId = args->msgId;
	struct StrmsgBuf Message;
	memcpy(Message.numbers, numbers, 4 * sizeof(int));
	Message.mtype = 1;
	msgsnd(msgId, &Message, sizeof(Message), 0);

	int k = 1;


	do
	{
		size_t size = msgrcv(msgId, &Message, sizeof(Message), 101, 0);

		if (Message.numbers[0] != -1)
		{
			printf("Combination %i: ", k);
			for (size_t i = 0; i < 4; ++i)
			{
				printf("%d ", Message.numbers[i]);
			}
	        	printf("\n");
			k++;
		}

	} while (Message.numbers[0] != -1);


	msgctl(msgId, IPC_RMID, NULL);
	return NULL;
}




int main() 
{
	int msgId = msgget(IPC_PRIVATE, 0600 | IPC_CREAT);
	if (msgId < 0)
	{
		perror("error with msgget()");
		return -1;
	}
	
	struct Arguments *args;
	args->msgId = msgId;

	pthread_t ParentThread, ChildThread;

	pthread_create(&ParentThread, NULL, parent_thread, (void *)args);
	pthread_create(&ChildThread, NULL, child_thread, (void*)args);

	pthread_join(ParentThread, NULL);

}

