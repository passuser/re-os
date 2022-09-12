#include "../lib/kernel/thread.h"
#include "string.h"
#include "../lib/kernel/debug.h"
#include "../lib/kernel/interrupt.h"
#include "../lib/kernel/global.h"
#include "../lib/kernel/memory.h"
#include "printf.h"

struct task_struct* main_thread;
struct list thread_ready_list;
struct list thread_all_list;
static struct list_elem* thread_tag;

extern void switch_to(struct task_struct* cur,struct task_struct* next);

struct task_struct* running_thread(void){
    uint32_t esp;
    asm ("mov %%esp,%0":"=g"(esp));
    return (struct task_struct*)(esp & 0xfffff000);
}//获取当前进程pcb

static void kernel_thread(thread_func* function,void* func_arg){
    intr_enable();
    function(func_arg);  
}//回调函数



void thread_create(struct task_struct* pthread,thread_func function,\
void*  func_arg){
      pthread->self_kernel_stack = pthread->self_kernel_stack -\
sizeof(struct intr_stack);
      pthread->self_kernel_stack = pthread->self_kernel_stack - \
sizeof(struct thread_stack);
      struct thread_stack* kernel_thread_stack = (struct thread_stack*)\
pthread->self_kernel_stack;
      kernel_thread_stack->eip = kernel_thread;
      kernel_thread_stack->function = function;
      kernel_thread_stack->func_arg = func_arg;
      kernel_thread_stack->ebx = 0;
      kernel_thread_stack->ebp = 0;
      kernel_thread_stack->esi = 0;
      kernel_thread_stack->edi = 0;
};

void init_thread(struct task_struct* pthread,char* name,int priority){
    memset(pthread,0,sizeof(*pthread));
    strcopy(pthread->name,name);
    if(pthread == main_thread){
      pthread->status = task_running;
    }else{
        pthread->status = task_ready;
    }
    pthread->self_kernel_stack = (uint32_t*)((uint32_t)pthread + PG_SIZE);
    pthread->priority = priority;
    pthread->ticks = priority;
    pthread->elapsed_ticks = 0;
    pthread->page_vaddr = NULL;
    pthread->stack_magic = 0x19900036;
}

struct task_struct* thread_start(char* name,int priority,thread_func function,\
void* func_arg){
      struct task_struct* thread = get_kernel_pages(1);
      init_thread(thread,name,priority);
      thread_create(thread,function,func_arg);
      ASSERT(!elem_find(&thread_ready_list,&thread->general_tag));
      list_append(&thread_ready_list,&thread->general_tag);
      ASSERT(!elem_find(&thread_all_list,&thread->all_list_tag));
      list_append(&thread_all_list,&thread->all_list_tag);
//      put_str("breakpoint\n");
      return thread;
}

static void make_main_thread(void){
    main_thread = running_thread();
    init_thread(main_thread, "main", 31);
    ASSERT(!elem_find(&thread_all_list,&main_thread->all_list_tag));
    list_append(&thread_all_list,&main_thread->all_list_tag);
}


void schedule(void){
    ASSERT(intr_get_status() == INTR_OFF);
    struct task_struct* cur = running_thread();
    if (cur->status == task_running){
      ASSERT(!elem_find(&thread_ready_list,&cur->general_tag));
      list_append(&thread_ready_list,&cur->general_tag);
      cur->ticks = cur->priority;
      cur->status = task_ready; 
    }else{
      
    }
    ASSERT(!list_empty(&thread_ready_list));
    thread_tag = NULL;
    thread_tag = list_pop(&thread_ready_list);
    struct task_struct* next = elem2entry(struct task_struct,\
general_tag,thread_tag);
    next->status = task_running;
    switch_to(cur,next);
}

void thread_block(enum task_status stat){
    ASSERT(((stat == task_blocked) || (stat == task_waiting)\
|| (stat == task_hanging)));
    enum intr_status old_status = intr_disable();
    struct task_struct* cur_thread = running_thread();
    cur_thread->status = stat;
    schedule();
    intr_set_status(old_status);
}

void thread_unblock(struct task_struct* pthread){
    enum intr_status old_status = intr_disable();
    ASSERT(((pthread->status == task_blocked) || (pthread->status\
== task_hanging) || (pthread->status == task_waiting)));
    if (pthread->status != task_ready){
      ASSERT(!elem_find(&thread_ready_list,&pthread->general_tag));
      if (elem_find(&thread_ready_list,&pthread->general_tag)){
        PANIC("thread_unblock: blocked thread in ready_list\n");
      }
      list_push(&thread_ready_list,&pthread->general_tag);
      pthread->status = task_ready;
    }
    intr_set_status(old_status);
}

void thread_init(void){
    put_str("thread_init start!\n");
    list_init(&thread_ready_list);
    list_init(&thread_all_list);
    make_main_thread();
    put_str("thread_init done!\n");
}
