#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

//объявление счетчиков сигналов
int signal_0 = 0, signal_1 = 0, signal_2 = 0;

//3 аналогичные функции обработки сигналов
void handl_int_0(int nsig) //функция принимает аргументов номер сигнала
{
    printf("received signal: %d\n", nsig); //вывод информации о сигнале
    signal_0++;	//увеличение счетчика срабатывания сигналов
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
    signal(SIGTERM, SIG_IGN); //игнорирование сигнала завершения
    signal(SIGINT, handl_int_0); //при сигнале прерывания запустить обработчик 0
    signal(SIGUSR1, handl_int_1); //при SIGUSR1 запустить обработчик 1
    signal(SIGUSR2, handl_int_2); //при SIGUSR2 запустить обработчик 2
    for(;;) //бесконечный цикл
    {
		time_t seconds = time(NULL); //счетчик времени
		struct tm* timeinfo = localtime(&seconds); //время с учетом региональных настроек
		printf(asctime(timeinfo)); //вывод времени
		//вывод информации о количестве сработанных сигналов
		printf("SIGINT: %d\n", signal_0);
		printf("SIGUSR1: %d\n", signal_1);
		printf("SIGUSR2: %d\n", signal_2);
			printf("\n");
		if (signal_0 >= 5) //при срабатывании сигнала 0 больше 4 раз 
		{
			//вывод конечной информации о количестве срабатывания сигналов
			printf("FINAL:\n");
			printf("SIGINT: %d\n", signal_0);
			printf("SIGUSR1: %d\n", signal_1);
			printf("SIGUSR2: %d\n",signal_2);
			return 0;
		}
		sleep(1); //задержка на 1мс
    }
    return 0;
}

