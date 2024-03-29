/*************************************************************************//**
 *****************************************************************************
 * @file   lib/exec.c
 * @brief  
 * @author Forrest Y. Yu
 * @date   Tue May  6 14:26:09 2008
 *****************************************************************************
 *****************************************************************************/

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

/*****************************************************************************
 *                                exec
 *****************************************************************************/
/**
 * Executes the program pointed by path.
 * 
 * @param path  The full path of the file to be executed.
 * 
 * @return  Zero if successful, otherwise -1.
 *****************************************************************************/
PUBLIC int exec(const char * path)
{
	MESSAGE msg;
	msg.type	= EXEC;
	msg.PATHNAME	= (void*)path;
	msg.NAME_LEN	= strlen(path);
	msg.BUF		= 0;
	msg.BUF_LEN	= 0;

	send_recv(BOTH, TASK_MM, &msg);
	assert(msg.type == SYSCALL_RET);

	return msg.RETVAL;
}

/*****************************************************************************
 *                                execl
 *****************************************************************************/
PUBLIC int execl(const char *path, const char *arg, ...)
{
	va_list parg = (va_list)(&arg);
	char **p = (char**)parg;
	return execv(path, p);
}

/*****************************************************************************
 *                                execv
 *****************************************************************************/
PUBLIC int execv(const char *path, char * argv[])
{
	char **p = argv;
	char arg_stack[PROC_ORIGIN_STACK];
	int stack_len = 0;

	while(*p++) {
		assert(stack_len + 2 * sizeof(char*) < PROC_ORIGIN_STACK);
		stack_len += sizeof(char*);
	}
// printl("ini len: %d\n", stack_len);

	*((int*)(&arg_stack[stack_len])) = 0;
	stack_len += sizeof(char*);
// printl("with null: %d\n", stack_len);


	char ** q = (char**)arg_stack;
	for (p = argv; *p != 0; p++) {
		*q++ = &arg_stack[stack_len];

		assert(stack_len + strlen(*p) + 1 < PROC_ORIGIN_STACK);
		strcpy(&arg_stack[stack_len], *p);
		stack_len += strlen(*p);
// printl("+=len p: %d\n", stack_len);
		arg_stack[stack_len] = 0;
		stack_len++;
	}

	MESSAGE msg;
	msg.type	= EXEC;
	msg.PATHNAME	= (void*)path;
	msg.NAME_LEN	= strlen(path);
	msg.BUF		= (void*)arg_stack;
	msg.BUF_LEN	= stack_len;

	// int NNN = ((int*)arg_stack)[0];
	// printl("usr/NNN: %d\n", NNN);
	// printl("final stack_len: %d\n", stack_len);
	// char** argssss = (char**)arg_stack;
	// for (int i = 0; i < stack_len; i++)
	// {
	// 	printl("arg[%d]: %s\n", i, argssss[i]);
	// }
	
	send_recv(BOTH, TASK_MM, &msg);
	assert(msg.type == SYSCALL_RET);

	return msg.RETVAL;
}

