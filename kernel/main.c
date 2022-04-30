#include "printf.h"
#include "init.h"
#include "debug.h"
int  main(void)
{   
	init_all();
  void* addr = get_kernel_pages(6);
  put_str ("\n get kernel pages start vaddr:0x");
  put_int ((uint32_t) addr);
  put_str ("\n");
//    ASSERT(1 == 2);
	while (1);
  return 0;
}

