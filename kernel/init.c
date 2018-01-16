#include "init.h"
#include "printf.h"
#include "interrupt.h"
#include "timer.h"

void init_all(void)
{
	put_str("init_all start!\n");
	idt_init();
	timer_init();   
}

