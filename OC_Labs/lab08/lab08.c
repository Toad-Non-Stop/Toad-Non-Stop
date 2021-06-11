#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>


int main(int argc, char* argv[]) 
{
	//получение имен файлов из аргументов 
	char* IN_file_NAME = argv[1];
	char* OUT_file_NAME = argv[2];

	//открытие файлов для чтения/записи (при необходимости их создание)
	int input_file = open(IN_file_NAME, O_RDWR | O_CREAT, 0600);
        if (input_file < 0)
        {
        	perror("error with open of map file");
       		return -1;
        }
	int output_file = open(OUT_file_NAME, O_RDWR | O_CREAT, 0600);
        if (output_file < 0)
        {
        	perror("error with open of map file");
       		return -1;
        }

	struct stat *st_1; //объявление структуры, хранящей информацию о файле
	fstat (input_file, st_1); //связывание структуры st_1 и открытого файла input_file
	int max_size = st_1->st_size; //получение размера открытого документа
	//получение начального адреса участка памяти файла
	char *data = (char *)mmap(NULL, max_size, PROT_WRITE | PROT_READ, MAP_SHARED, input_file, 0);
    if (MAP_FAILED == data)
    {
        perror("error with mmap");
        return -2;
    }

	lseek (output_file, 0, SEEK_SET); //передвижение указателя на файл output_file
	write (output_file, data, max_size); //запись информации в выходной файл
	ftruncate(output_file, max_size); //усечение файла до размера max_size
    munmap(data, max_size); //отключение отображения объекта в памяти

	//закрытие файлов 
	close (input_file);
	close (output_file);
	
}
