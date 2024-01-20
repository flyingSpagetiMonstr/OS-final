#include "stdio.h"

int atoi(const char *str);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("One argument (pid) pls!\n");
		return 1;
	}

	int pid = atoi(argv[1]);
    printf("Killing...\n");
    kill(pid);
    printf("End\n");
	return 0;
}

int atoi(const char *str) 
{
    int result = 0;
    int sign = 1;
    int i = 0;

    // Handle leading whitespace
    while (str[i] == ' ') {
        i++;
    }

    // Handle sign
    if (str[i] == '-' || str[i] == '+') {
        sign = (str[i++] == '-') ? -1 : 1;
    }

    // Convert digits to integer
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i++] - '0');
    }

    return result * sign;
}