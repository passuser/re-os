#include "../lib/kernel/init.h"
#include "printf.h"
#include "../lib/kernel/memory.h"
#include "../lib/kernel/thread.h"
#include "../lib/kernel/interrupt.h"
#include "../lib/kernel/console.h"

void k_thread_a(void*);
void k_thread_b(void*);

//主进程
int  main(void){  
  put_str("I am kernel.\n");
	init_all();
  thread_start("k_thread_a", 31, k_thread_a, "argA ");
  thread_start("k_thread_b", 8, k_thread_b, "argB ");
  intr_enable();
	while (1){
    console_put_str("main ");
  }
  return 0;
}

//线程A
void k_thread_a(void* arg){
    char* para = arg;
    while(1){
      console_put_str(para);
    }
}

//线程B
void k_thread_b(void* arg){
    char* para = arg;
    while(1){
      console_put_str(para);
    }
}

