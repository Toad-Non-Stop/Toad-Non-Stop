#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc, char* argv[])
{
	//получение аргументов
	char* Filename = argv[1];
	int kol = atoi(argv[2]);

	//открытие файла для чтения
	FILE* fin = fopen(Filename, "r");
	if (NULL == fin) //при ошибке открытия файла
	{
		perror("cannot open file");
		return -1;
	}

	int data[kol]; //создание массива размерности kol (получено из аргументов)
	int res = fread(data, sizeof(int), kol, fin); //чтение из файла в дату kol элементов
	printf("fread result = %i\n", res); //вывод количества считанных элементов
	//вывод всего считанного массива
	for (int i = 0; i < res ; i++)
	{
		printf("random number = %i (0x%x)\n", data[i], data[i]);
	}

	fclose(fin); //закрытие файла
	return 0;
}
