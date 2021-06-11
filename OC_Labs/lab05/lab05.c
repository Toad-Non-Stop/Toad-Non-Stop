#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

const char* NAME = "stroka";

int comparator(const void* a, const void* b)
{
    return (*((int*)b) - *((int*)a));
}

//функция создания и инициализации массива
int* randomazing(int N)
{
    srand((unsigned) (time(0)));

    int *array = malloc(sizeof(int) * N); //выделение памяти для массива
	//заполнение массива
    for (int i = 0; i < N; ++i)
    {
        array[i] = rand() % 300;
    }
    
    return array;
}

//функция вывода массива
void Out_numbers(int* array, int N)
{
    for (int i = 0; i < N; ++i)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main(int argv, char *argc[])
{
    int N = atoi(argc[1]); //получение размера массива из аргументов
    int *array = randomazing(N); //создание массива 
    Out_numbers(array, N); //вывод массива

    mknod(NAME, S_IFIFO | 0666, 0); //создание канала с именем "stroka"
    int p[2]; //массив, представляющий 2 файловых дескриптора
    pipe(p); //создание безымянного канала

    int childId = fork(); //создание процесса-потомка, аналогичного нынешнему процессу

    if (childId == 0) //процесс-потомок
    {
        close(p[1]); //закрытие дескриптора выходного файла
        int fifo = open(NAME, O_WRONLY); //открытие потока только для записи через канал "stroka"
        int *mem = malloc(sizeof(int) * N); //резервирование места под массив размерности N
        read(p[0], mem, sizeof(int) * N); //считывание значений в массив из входного потока
        close(p[0]); //закрытие входного потока
        qsort(mem, N, sizeof(int), comparator); //сортировка считанного массива
        write(fifo, mem, sizeof(int) * N); //запись отсортированного массива в поток "sroka"
    }
    else if (childId > 0) //процесс-родитель
    {
        close(p[0]); //закрытие входного потока
        int fifo = open(NAME, O_RDONLY); //открытие потока только для записи через канал "stroka"
        write(p[1], array, sizeof(int) * N); //запись значений в массив из входного потока 
        close(p[1]); //закрытие входного потока
        int *sortirovka = malloc(sizeof(int) * N); //резервирование места под массив
        read(fifo, sortirovka, sizeof(int) * N); //чтение массива из потока "stroka"
        Out_numbers(sortirovka, N); //вывод массива
        unlink(NAME); //удаление канала "stroka"
    }
    else if (childId < 0) //ошибка создания потомка
    {
        perror("error with fork()\n");
    }
}
     
    

