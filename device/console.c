#include "printf.h"
#include "stdint.h"
#include "../lib/kernel/sync.h"
#include "../lib/kernel/thread.h"
#include "../lib/kernel/console.h"

static struct lock console_lock;

void console_init(void){
    lock_init(&console_lock);
}

//获取终端
void console_acquire(void){
    lock_acquire(&console_lock);
}

//释放终端
void console_release(void){
    lock_release(&console_lock);
}

//终端输出字符串
void console_put_str(char* str){
  console_acquire();
  put_str(str);
  console_release();
}

//终端输出字符
void console_put_char(uint8_t char_asci){
    console_acquire();
    put_char(char_asci);
    console_release();
}

//终端输出十六进制数
void console_put_int(uint32_t num){
    console_acquire();
    put_int(num);
    console_release();
}




