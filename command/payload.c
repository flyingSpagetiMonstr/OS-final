int (*printf)(const char *fmt, ...);
void (*exit)(int status);

int main()
{
    char buf[] = "-Infected-\n";
    printf = (void*)0x106E;
    printf(buf);

    exit = (void*)0x17E4;
    exit(0);
}