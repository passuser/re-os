#include "string.h"
#include "global.h"
#include "debug.h"


/*将dst_起始的size个字节设为value */
void memset (void* dst_,uint8_t value,uint32_t size){

    ASSERT (dst_ != NULL);
    uint8_t* dst = (uint8_t*) dst_;
    while ( size-- > 0){
        *dst++ = value;
    }
}

/*将src_起始的字节复制到dst_*/
void memcopy (void* dst_,const void* src_,uint32_t size){

    ASSERT (src_ != NULL && dst_ != NULL);
    uint8_t* dst = dst_;
    const uint8_t* src = src_;
    while (size-- > 0){
        *dst++ = *src++;
    }
}

/*连续比较以地址a_和地址b_开头size个字节，若相等则返回0,a_大于b_,则返回+1，a_小于b_返回-1 */
int8_t memcmp (const void* a_,const void* b_,uint32_t size){
    ASSERT (a_ != NULL && b_ != NULL);
    const char* a = a_;
    const char* b = b_;
    while ( size-- > 0){
        if (*a != *b){
      return  (*a > *b) ?  1 :  -1;
        }
        a++;
        b++;
    }
    return 0;
}

/*将字符串从srt_复制到dst_*/
char* strcopy (char* dst_,const char* src_){

    ASSERT (src_ != NULL && dst_ != NULL);
    char* r = dst_;
    while ((*dst_++ = *src_++));
    return r;
}

/* 返回字符串长度 */
uint32_t strlen(const char* str ){
    
    ASSERT (str != NULL);
    const char* p = str;
    while (*p++);
    return (p - str -1);
}

/*比较字符串，a大于b，返回1，相等返回0，小于返回-1 */
int8_t strcmp (const char* a,const char* b){

    ASSERT (a != NULL && b != NULL);
    while (*a != 0 && *a == *b){
        a++;
        b++;
    }
    return (*a < *b) ? -1 : (*a < *b);
}

/* 从左向右查找字符串str中首次出现字符ch的地址  */
char* strchr (const char* str,uint8_t ch){

    ASSERT (str != NULL);
    while (*str != 0){
        if (*str == ch){
            return (char*)str;
        }
        str++;
    }
    return NULL;
}

/*从右向左查找字符串str中首次出现字符ch的地址*/
char* strrchr (const char* str,uint8_t ch){

    ASSERT (str != NULL);
    const char* last_str = NULL;
    while ( *str != 0){
        if (*str == ch){
            last_str = str;
        }
        str++;
    }
    return (char*)last_str;
}

/*将字符串src_拼接到dst_后面，并返回拼接字符串的地址*/
char* strcat (char* dst_,const char* src_){

    ASSERT(dst_ != NULL && src_ != NULL);
    char* str = dst_;
    while (*str++);
    --str;
    while(*str++ = *src_++);
    return dst_;
}

/*在字符串str中查找字符ch出现的次数*/
uint32_t strchrs (const char* str,uint8_t ch){

    ASSERT (str != NULL);
    const char* p = str;
    uint32_t n = 0;
    while (*p != 0){
        if (*p == ch){
            n++;
        }
        p++;
    }
    return n;
}






