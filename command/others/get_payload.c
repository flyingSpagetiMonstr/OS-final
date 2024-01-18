#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elf.h"

void process(char *tmp);
int is_elf(Elf32_Ehdr elf_ehdr);
int n_cmp(char *a, char *b, int n);

unsigned char payload[] = {
    0x8d, 0x4c, 0x24, 0x04, 0x83, 0xe4, 0xf0, 0xff, 0x71, 0xfc, 0x55, 0x89, 0xe5, 0x53, 0x51, 0x83, 0xec, 0x10, 0xe8, 0x60, 0x00, 0x00, 0x00, 0x81, 0xc3, 0xe9, 0x2f, 0x00, 0x00, 0xc7, 0x45, 0xec, 0x2d, 0x49, 0x6e, 0x66, 0xc7, 0x45, 0xf0, 0x65, 0x63, 0x74, 0x65, 0xc7, 0x45, 0xf4, 0x64, 0x2d, 0x0a, 0x00, 0xc7, 0x83, 0x0c, 0x00, 0x00, 0x00, 0x6e, 0x10, 0x00, 0x00, 0xc7, 0x83, 0x10, 0x00, 0x00, 0x00, 0xe4, 0x17, 0x00, 0x00, 0x8b, 0x83, 0x0c, 0x00, 0x00, 0x00, 0x83, 0xec, 0x0c, 0x8d, 0x55, 0xec, 0x52, 0xff, 0xd0, 0x83, 0xc4, 0x10, 0x8b, 0x83, 0x10, 0x00, 0x00, 0x00, 0x83, 0xec, 0x0c, 0x6a, 0x00, 0xff, 0xd0, 0x83, 0xc4, 0x10, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x8d, 0x65, 0xf8, 0x59, 0x5b, 0x5d, 0x8d, 0x61, 0xfc, 0xc3, 0x8b, 0x1c, 0x24, 0xc3, 0x66, 0x90, 0x66, 0x90, 0x90, 0x50, 0x51, 0xe8, 0x79, 0xff, 0xff, 0xff, 0xf4
};

int main()
{
    process("../target");
}

int n_cmp(char *a, char *b, int n) {
    int i = 0;
    for (i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            return 1;
        }
    }
    return 0;
}

int is_elf(Elf32_Ehdr elf_ehdr) {
    // e_ident = 0x7fELF
    return (n_cmp((char*)elf_ehdr.e_ident, (char*)ELFMAG, SELFMAG) == 0);
}

void process(char *tmp)
{
    Elf32_Ehdr elf_ehdr;
    // Program Header Table 结构体
    Elf32_Shdr elf_shdr;
    // Elf32_Sym elf_sym;

    // 不读驱动 不读kernel.bin
    FILE* old_file = fopen(tmp, "rb");
    fread(&elf_ehdr,  sizeof(elf_ehdr), 1, old_file);

    // 判断是否是一个 ELF 文件
    printf("%s,", tmp);
    if (!is_elf(elf_ehdr)){printf("it is not a ELF\n"); return;}
    
    printf("it is a ELF , attacking\n");

    int e_sho_off = elf_ehdr.e_shoff;
    int section_num = elf_ehdr.e_shnum;
    int e_shstr_header_index = elf_ehdr.e_shstrndx;//节头名字表的索引
    int e_shstr_off = e_sho_off + e_shstr_header_index * sizeof(elf_shdr);

    fseek(old_file, e_shstr_off, SEEK_SET);//定位到节头名字表节头
    fread(&elf_shdr,  sizeof(elf_shdr), 1, old_file);
    int shstr_section_off = elf_shdr.sh_offset;
    char shstr_buf[1000];
    fseek(old_file, shstr_section_off, SEEK_SET);
    fread(shstr_buf, 1000, 1, old_file);//定位到shstr_tab

    fseek(old_file, e_sho_off, SEEK_SET);//定位到节头表
    int text_off = 0;
    for (int i = 0; i < section_num; i++)
    {
        fread(&elf_shdr,  sizeof(elf_shdr), 1, old_file);
        if (strcmp(shstr_buf + elf_shdr.sh_name, ".text") == 0)
        {
            text_off = i;
            break;
        }
    }
    int text_offset = e_sho_off + sizeof(elf_shdr) * text_off;  // 找到代码节节头的偏移
    fseek(old_file, text_offset, SEEK_SET);// 定位到text节头的位置
    fread(&elf_shdr,  sizeof(elf_shdr), 1, old_file);  //读出text节头
    int text_section_offset = elf_shdr.sh_offset;  // 找到代码段的偏移
    int text_size = elf_shdr.sh_size;

    // char shellcode[] = { 0x66,0x87,0xdb };
    fseek(old_file, text_section_offset, SEEK_SET);
    unsigned char buf[1000] = {0};
    fread(buf, text_size, 1, old_file);
    printf("%d\n", text_size);
    // for (int i = 0; i < text_size; i++)
    for (int i = 0; i < 10; i++)
    {
        // fwrite(buf, text_size, 1, stdout);
        printf("0x%02x, ", buf[i]);
    }
}