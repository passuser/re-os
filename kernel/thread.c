#include "thread.h"
#include "string.h"
#include "debug.h"
#include "interrupt.h"
#include "global.h"
#include "memory.h"
#include "printf.h"

struct task_struct* main_thread;
struct list thread_ready_list;
struct list thread_all_list;
static struct list_elem* thread_tag;

extern void switch_to(struct task_struct* cur,struct task_struct* next);

struct task_struct* running_thread(){
    uint32_t esp;
    asm ("mov %%esp,%0":"=g"(esp));
    return (struct task_struct*)(esp & 0xfffff000);
};//获取当前进程pcb

static void kernel_thread(thread_func* function,void* func_arg){
    intr_enable();
    function(func_arg);  
};



void thread_create(struct task_struct* pthread,thread_func function,void*  func_arg){
      pthread->self_kernel_stack = pthread->self_kernel_stack - sizeof(struct intr_stack);
      pthread->self_kernel_stack = pthread->self_kernel_stack - sizeof(struct thread_stack);
      struct thread_stack* kernel_thread_stack = (struct thread_stack*)pthread->self_kernel_stack;
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
};

struct task_struct* thread_start(char* name,int priority,thread_func function,void* func_arg){
      struct task_struct* thread = get_kernel_pages(1);
      init_thread(thread,name,priority);
      thread_create(thread,function,func_arg);
      ASSERT(!elem_find(&thread_ready_list,&thread->general_tag));
      list_append(&thread_ready_list,&thread->general_tag);
      ASSERT(!elem_find(&thread_all_list,&thread->all_list_tag));
      list_append(&thread_all_list,&thread->all_list_tag);
//      asm volatile("movl %0,%%esp;pop %%ebx;pop %%ebp;pop %%edi;\
          pop %%esi;ret"::"g"(thread->self_kernel_stack):"memory");
      return thread;
};

static void make_main_thread(void){
    main_thread = running_thread();
    init_thread(main_thread,"main",31);
    ASSERT(!elem_find(&thread_all_list,&main_thread->all_list_tag));
    list_append(&thread_all_list,&main_thread->all_list_tag);
};

static void general_intr_handler(uint8_t vec_nr){
    if (vec_nr == 0x27 || vec_nr == 0x2f){
      return;
    }
    set_cursor(0);
    int cursor_pos = 0;
    while(cursor_pos < 320){
      put_str(" ");
      cursor_pos++;
    };
    set_cursor(0);
    put_str("!!!!!!!     excetion message begin     !!!!!!!\n");
    set_cursor(88);
    //put_str(intr_name[vec_nr]);
    if(vec_nr == 14){
      int page_fault_vaddr = 0;
      asm("movl %%cr2,%0":"=r"(page_fault_vaddr));
      put_str("\npage fault addr is ");
      put_int(page_fault_vaddr);
    }
    put_str("\n!!!!!!!    excetion message end      !!!!!!!!\n");
    while(1);
};

void schedule(){
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
};


void thread_init(void){
    put_str("thread_init start!\n");
    list_init(&thread_ready_list);
    list_init(&thread_all_list);
    make_main_thread();
    put_str("thread_init done!\n");
};
