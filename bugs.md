- strcmp "INIT"
- command/ printf(argv[1])::OK printf(argv[1])::BUG 
- hd driver: delay
- ATOMICITY & asserts
```c
block(p_who_wanna_recv);

disp_color_str(" :asserting: ", RED);
		assert(p_who_wanna_recv->p_flags == RECEIVING);
disp_color_str(" :asserted: ", RED);
```
```c
PUBLIC int fork()
{
	MESSAGE msg;
	msg.type = FORK;

	send_recv(BOTH, TASK_MM, &msg);
	// assert(msg.type == SYSCALL_RET);
	// assert(msg.RETVAL == 0);

	return msg.PID;
}
```
```c
if (fork() != 0) 
{	/* parent */
    if (!block_shell)
    {
    	delay(0xf);
    	if (fork() != 0)
		{
		// ...
}}}
```
- disp_str newline
- no supporting fork without wait||exit

- ...


- k/m lines of code

# logs
- type.h: extern ticker_created;
