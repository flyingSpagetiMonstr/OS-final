// #include "type.h"
// #include "stdio.h"
// #include "const.h"
// #include "protect.h"
// #include "string.h"
// #include "fs.h"
// #include "proc.h"
// #include "tty.h"
// #include "console.h"
// #include "global.h"
// #include "keyboard.h"
// #include "proto.h"
// #include "hd.h"
// #include "fs.h"

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
#include "keyboard.h"
#include "proto.h"


PUBLIC void fs_readdir() {

    char filename[MAX_PATH] = {0};
    char result[RESULT_MAX] = {0};
    struct inode* inode;
    int index = 0;

    phys_copy((void*)va2la(TASK_FS, result),
        (void*)va2la(fs_msg.source, fs_msg.PATHNAME),
        min(fs_msg.NAME_LEN, sizeof(result)));

    if (strip_path(filename, result, &inode) != 0) {
        memset(result, 0, sizeof(result));
        goto ret;
    }

    int n_block_0 = inode->i_start_sect;
    int n_blocks = (inode->i_size + SECTOR_SIZE - 1) / SECTOR_SIZE;

    struct dir_entry* pde;
    for (int i = 0; i < n_blocks; i++) {
        RD_SECT(inode->i_dev, n_block_0 + i);
        pde = (struct dir_entry*)fsbuf;
        for (int j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++, pde++) {
            if (index != 0)
            {
                result[index] = ' ';
            }            
            index ++;
            memcpy(result + index, pde->name, strlen(pde->name));
            index += strlen(pde->name);
        }
    }
    result[index] = 0;

ret: 
    phys_copy((void*)va2la(fs_msg.source, fs_msg.BUF),
        (void*)va2la(TASK_FS, result),
        min(fs_msg.BUF_LEN, sizeof(result)));
}