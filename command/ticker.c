#include <stdio.h>
#include <string.h>

void delay(int time);

int main(int argc, char *argv[])
{
	char filename[10] = {0};

	if (argc == 1)
	{
		sprintf(filename, "record");
	}
	else if (argc == 2)
	{
		memcpy(filename, argv[1], sizeof(filename));
	}
	else
	{
		printf("TOO MANY ARGS\n");
		return 1;
	}

	int fd = open(filename, O_RDWR);
	if (fd == -1)
	{
		fd = open(filename, O_CREAT | O_RDWR);
	}

    for (int i = 0; i <= 0x1f; i++)    
    {
		write(fd, &i, sizeof(i));
		close(fd);
        delay(0xf);
		fd = open(filename, O_RDWR);
    }
	close(fd);
}

void delay(int time)
{
	int i, j, k;
	for(k=0;k<time;k++){
		/*for(i=0;i<10000;i++){	for Virtual PC	*/
		for(i=0;i<10;i++){/*	for Bochs	*/
			for(j=0;j<10000;j++){}
		}
	}
}