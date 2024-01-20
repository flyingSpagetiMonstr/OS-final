/* WARNING: modifying code will influence function addresses -> failure*/

#include <stdint.h>
#include "stdio.h"
#include "string.h"

// #define OFFSET 0xD
// #define _OFFSET (ebp - ((void*)buf))

// #define LEN (offset + 2*sizeof(void*))

#define MAX (0xff)

void vulnerability(char *input, int get_offset);
void dest(void);
void delay(int time);


uint32_t offset = 0;
void *target = dest; // addr of dest


int main(int argc, char *argv[])
{
    char payload[MAX] = {0};
    vulnerability(payload, 1);

    // *(void**)(payload + offset) = target;
    // printf(" payload: %x\n", *(int*)(payload + offset));
    *(uint32_t*)(payload + offset) = (uint32_t)target;
    printf(" payload: %x\n", *(uint32_t*)(payload + offset));

    vulnerability(payload, 0);
}

void vulnerability(char *input, int get_offset)
{
    char buf[0x1] = {0};

   // get ebp:
    void* ebp = 0;
    asm volatile("movl %%ebp, %0": "=r" (ebp));
    // calc eip by ebp:
    void **eip = (((void**)ebp) + 1); // addr of eip
    printf("eip: %x\n", (uint32_t)(*eip));
    // printf("dest: %x\n", target);

    if (get_offset)
    {    
        offset = (uint32_t)eip - (uint32_t)buf;
        printf("offset: %x\n", offset);
        return;
    }

    // ===========================================
    printf(" input: %x\n", *(uint32_t*)(input + offset));
    printf(" target: %x\n", target);
    memcpy(buf, input, offset + sizeof(void*));
    // ===========================================
    // memcpy(eip, &target, sizeof(target));

    return;
}

void dest(void)
{
    printf("In dest now.\n");
    int i, j, k;
	for(k=0;k<0xf;k++)
		for(i=0;i<10;i++)
			for(j=0;j<10000;j++){}
    printf("Already in dest for long.\n");
    exit(0);
}


// <- buf

// <- eip = buf + offset


// old_ebp <- ebp
// eip