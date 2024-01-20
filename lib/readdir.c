#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"


PUBLIC void readdir(char path[], char result[RESULT_MAX]) 
{
    MESSAGE msg;
    msg.type = SEARCH;
    msg.PATHNAME = path; // directory/path name 
    msg.NAME_LEN = strlen(path);
    msg.BUF = result; // where the results shall be stored
    msg.BUF_LEN = RESULT_MAX;
    send_recv(BOTH, TASK_FS, &msg);
    result[RESULT_MAX-1] = '\0';
    return; 
}