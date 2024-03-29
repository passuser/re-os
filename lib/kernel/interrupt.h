#ifndef _KERNEL_INTERRUPT_H 
#define _KERNEL_INTERRUPT_H 
#include "stdint.h"

typedef  void*  intr_handler;
enum intr_status{
  INTR_OFF,
  INTR_ON
};//枚举量按顺序赋值0，1，2，3

void idt_init (void);
void register_handler(uint8_t vector_no,intr_handler function);

enum intr_status intr_get_status(void); 
enum intr_status intr_set_status(enum intr_status);
enum intr_status intr_enable(void);
enum intr_status intr_disable(void);

#endif
