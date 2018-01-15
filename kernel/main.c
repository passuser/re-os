#include "printf.h"
#include "init.h"
void  main(void)
{   
	init_all();
	asm volatile ("sti");
	while (1);
}

