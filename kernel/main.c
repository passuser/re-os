#include "init.h"
#include "printf.h"
#include "memory.h"

int  main(void)
{   
	init_all();
  void* addr = get_kernel_pages(3);
  put_str ("\n get kernel pages start vaddr:0x");
  put_int ((uint32_t)addr);
  put_str ("\n");
	while (1);
  return 0;
}

