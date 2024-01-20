#include "stdio.h"
#include "string.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Meow!\n");
		return 1;
	}

	char filename[10] = {0};
	memcpy(filename, argv[1], sizeof(filename));

	// printf("argv: %s: \n", argv[1]);
	// printf("argv: %s: \n", argv[1]);
	// printf("filename: %s: \n", filename);
	// printf("filename: %s: \n", filename);

	int fd = open(filename, O_RDWR);
	if (fd == -1) { printf("Unable to open, maybe no such file\n"); return 1; }

	unsigned char buf[16 * 16] = {0};
	int size = read(fd, buf, sizeof(buf));

	printf("-----");
	for (int j = 0; j < 16; j++)
	{
		printf(" --");
	}
	printf("\n");
	printf("\\\\COL");
	for (int j = 0; j < 16; j++)
	{
		printf(" %02x", j);
	}
	printf("\n");
	printf("ROW\\\\");
	for (int j = 0; j < 16; j++)
	{
		printf(" ==");
	}
	printf("\n");

	for (int i = 0; i < 16; i++)
	{
		printf("%02x ||", i);
		for (int j = 0; j < 16; j++)
		{
			if (i * 16 + j < size)
			{
				printf(" %02x", buf[i*16 + j]);
			}			
		}
		printf("\n");
	}

	return 0;
}