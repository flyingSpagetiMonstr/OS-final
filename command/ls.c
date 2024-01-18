#include "stdio.h"

int main(int argc, char const *argv[])
{
    char ls_content[RESULT_MAX] = {0};
    // fs_readdir(); ls_content
    readdir("/", ls_content);
    printf("%s\n", ls_content);
    return 0;
}
