#include "../lib/kernel/init.h"
#include "printf.h"
#include "../lib/kernel/memory.h"
#include "../lib/kernel/thread.h"
#include "../lib/kernel/interrupt.h"
#include "../lib/kernel/console.h"
#include "keyboard.h"
#include "ioqueue.h"

void k_thread_a(void*);
void k_thread_b(void*);

//主进程
int  main(void){  
  put_str("I am kernel.\n");
	init_all();
  thread_start("consumer_a", 31, k_thread_a, " argA");
  thread_start("consumer_b", 31, k_thread_b, " argB");
  intr_enable();
	while (1);
}

//线程A
void k_thread_a(void* arg){
    char* para = arg;
    while(1){
      enum intr_status old_status = intr_disable();
      if(!ioq_empty(&kbd_buf)){
        console_put_str(arg);
        char byte = ioq_getchar(&kbd_buf);
        console_put_str(byte);
      }
      intr_set_status(old_status);
    }
}

//线程B
void k_thread_b(void* arg){
    char* para = arg;
    while(1){
      enum intr_status old_status = intr_disable();
      if(!ioq_empty(&kbd_buf)){
        console_put_str(arg);
        char byte = ioq_getchar(&kbd_buf);
        console_put_str(byte);
      }
      intr_set_status(old_status);
    }
}


