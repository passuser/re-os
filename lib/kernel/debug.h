#ifndef _KERNEL_DEBUG_H 
#define _KERNEL_DEBUG_H
void panic_spin(char* filename,int line,const char* func,const char* condition);
#define PANIC(...) panic_span(_FILE_,_LINE_,_func_,_VA_ARGS_)
#ifdef NDEBUG
   #define ASSERT(CONDITION) ((void)0)
#else
#define ASSERT(CONDITION)  if(CONDITION){}else{PANIC(#CONDITION);}
#endif
#endif
