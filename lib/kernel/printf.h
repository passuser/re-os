#ifndef _LIB_KERNEL_PRINT_H
#define _LIB_KERNEL_PRINT_H
#include "stdint.h"

void put_char(uint8_t char_asci);
void put_str(const char* message);
void put_int(uint32_t num);
void set_cursor(uint8_t count);
#endif
