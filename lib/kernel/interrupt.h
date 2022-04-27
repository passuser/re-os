#ifndef _KERNEL_INTERRUPT_H 
#define _KERNEL_INTERRUPT_H 
//#include "stdio.h"
#include "stdint.h"


typedef  void*  intr_handler;
void idt_init (void);

enum intr_status{ INTR_ON, INTR_OFF};
enum intr_status intr_get_status(void); 
enum intr_status intr_set_status(enum intr_status);
enum intr_status intr_enable(void);
enum intr_status intr_disable(void);

#endif
