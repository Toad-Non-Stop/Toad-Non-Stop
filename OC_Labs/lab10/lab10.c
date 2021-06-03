#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int signal_0 = 0, signal_1 = 0, signal_2 = 0;

void handl_int_0(int nsig)
{
    printf("received signal: %d\n", nsig);
    signal_0++;	
}

void handl_int_1(int nsig)
{
    printf("received signal: %d\n", nsig);
    signal_1++;
}

void handl_int_2(int nsig)
{
    printf("received signal: %d\n", nsig);
    signal_2++;
}

int main()
{
    signal(SIGTERM, SIG_IGN);
    signal(SIGINT, handl_int_0);
    signal(SIGUSR1, handl_int_1);
    signal(SIGUSR2, handl_int_2);
    for(;;)
    {
	time_t seconds = time(NULL);
	struct tm* timeinfo = localtime(&seconds);
	printf(asctime(timeinfo));
	printf("SIGINT: %d\n", signal_0);
	printf("SIGUSR1: %d\n", signal_1);
	printf("SIGUSR2: %d\n", signal_2);
        printf("\n");
	if (signal_0 >= 5)
	{
		printf("FINAL:\n");
		printf("SIGINT: %d\n", signal_0);
		printf("SIGUSR1: %d\n", signal_1);
		printf("SIGUSR2: %d\n",signal_2);
		return 0;
	}
        sleep(1);
    }
    return 0;
}

