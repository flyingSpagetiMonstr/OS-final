/**
 * @file kill.c
 */


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


PUBLIC void kill(int pid)
{
    // disp_color_str("KILLING\n", RED);
	MESSAGE msg;
	msg.type = KILL;
	msg.PID = pid;
	// msg.STATUS = KILLED;
    // return;
	send_recv(BOTH, TASK_MM, &msg);
	assert(msg.type == SYSCALL_RET);
}
