#include "init.h"
#include "printf.h"
#include "interrupt.h"
#include "timer.h"
#include "memory.h"
#include "console.h"
#include "thread.h"
#include "keyboard.h"

void init_all(void)
{
	put_str("init_all start!\n");
	idt_init();
  mem_init ();
  thread_init();
	timer_init();
  console_init();
  keyboard_init();
}

