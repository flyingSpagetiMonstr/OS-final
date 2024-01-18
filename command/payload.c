static int (*printf)(const char *fmt, ...);
static void	(*exit)(int status);

int main()
{
    char buf[] = "-Infected-\n";
    printf = 0x106E;
    exit = 0x17E4;
    printf(buf);
    exit(0);
}