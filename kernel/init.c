#include "init.h"
#include "printf.h"
#include "interrupt.h"

void init_all()
{
	put_str("init_all\n");
	idt_init();
}

