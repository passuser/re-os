#ifndef _THREAD_H
#define _THREAD_H
#include "stdint.h"
#include "list.h"

typedef void thread_func(void*);

//枚举进程或线程状态
enum task_status {
    task_running,
    task_died,
    task_ready,
    task_blocked,
    task_waiting,
    task_hanging
};

//中断时上下文环境
struct intr_stack {
    uint32_t vec_no;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp_dummy;//esp会时刻变化，需要随时压入保存
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;
//从低特权进入高特权时压入
    uint32_t err_code;
    void (*eip) (void);
    uint32_t cs;
    uint32_t eflags;
    void* esp;
    uint32_t ss;
};

//线程栈
struct thread_stack {
    uint32_t ebx;
    uint32_t ebp;
    uint32_t edi;
    uint32_t esi;

    void (*eip) (thread_func* func,void* func_arg);


    void (*unused_retaddr); //空白占位
    thread_func* function;
    void* func_arg;
};

struct task_struct {
    uint32_t* self_kernel_stack;
    enum task_status status;
    char name[16];
    uint8_t priority;
    uint8_t ticks;  // 每次在处理器上执行的时间滴答数
    uint32_t elapsed_ticks; //总占用滴答数
    struct list_elem general_tag;
    struct list_elem all_list_tag;
    uint32_t page_vaddr;//页表虚拟地址
    uint32_t stack_magic;
};

struct task_struct* running_thread(void);
void thread_create(struct task_struct* pthread,thread_func function,\
void*  func_arg);
//void kernel_thread(thread_func* function,void* func_arg);
void init_thread(struct task_struct* pthread,char* name,int priority);
struct task_struct* thread_start(char* name,int priority,thread_func function,\
void* func_arg);
//void make_main_thread(void);
//void general_intr_handler(uint8_t vec_nr);
void schedule(void);
void thread_block(enum task_status stat);
void thread_unblock(struct task_struct* pthread);
void thread_init(void);
#endif
