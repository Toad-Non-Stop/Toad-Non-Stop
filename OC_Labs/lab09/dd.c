#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc, char* argv[])
{
	char* Filename = argv[1];
	int kol = atoi(argv[2]);

	FILE* fin = fopen(Filename, "r");
	if (NULL == fin)
	{
		perror("cannot open file");
		return -1;
	}

	int data[kol];
	int res = fread(data, sizeof(int), kol, fin);
	printf("fread result = %i\n", res);
	for (int i = 0; i < res ; i++)
	{
		printf("random number = %i (0x%x)\n", data[i], data[i]);
	}

	fclose(fin);
	return 0;
}
