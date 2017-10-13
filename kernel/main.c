#include "printf.h"
#include "init.h"
void  main(void)
{   put_str("This is kernel!\na\t\nfirst\n");
	init_all();
	put_str("success!");
	asm volatile ("sti");
	while (1);
}

