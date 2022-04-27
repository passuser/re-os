#include "bitmap.h"
#include "stdint.h"
#include "string.h"
#include "printf.h"
#include "interrupt.h"
#include "debug.h"

/*位图初始化*/
void bitmap_init (struct bitmap* btmp){
    memset(btmp->bits,0,btmp->btmp_bytes_len);
}

/* 判断bit_idx位是否为1，若为1返回true，否则false*/
_Bool bitmap_scan_test (struct bitmap* btmp,int32_t bit_idx){

    uint32_t bytes_idx = bit_idx / 8;
    uint32_t bits_odd = bit_idx % 8;
    return (_Bool) (btmp->bits[bytes_idx] & (BITMAP_MASK << bits_odd));
}

/*在位图中申请连续ch个位，成功返回起始数组下标，失败返回-1*/
char bitmap_scan (struct bitmap* btmp,uint32_t ch){

    uint32_t bytes_idx = 0;
    while (0xff == btmp->bits[bytes_idx] && (bytes_idx < btmp->btmp_bytes_len)){
        bytes_idx++;
    }
    ASSERT (bytes_idx < btmp->btmp_bytes_len);
    if (bytes_idx ==  btmp->btmp_bytes_len){
        return -1;
    }
    int bits_idx = 0;
    while ((BITMAP_MASK << bits_idx ) & btmp->bits[bytes_idx]){
        bits_idx++;
    }
    int bits_idx_start = bytes_idx * 8 + bits_idx;
    if (ch == 1){
        return bits_idx_start;
    }
    uint32_t bits_left = btmp->btmp_bytes_len * 8 - bits_idx_start;
    uint32_t bits_next = bits_idx_start + 1;
    uint32_t count = 1;

    bits_idx_start = -1;
    while (bits_left-- > 0){
        if (!(bitmap_scan_test(btmp,bits_next))){
            count++;
        } else {
              count = 0;
        }
        if (count == ch){
            bits_idx_start = bits_next - ch + 1;
            break;
        }
        bits_next++;
    }
    return bits_idx_start;
}

/* 将位图bits_idx位设为value*/
void bitmap_set (struct bitmap* btmp,uint32_t bits_idx,_Bool value){
    
    uint32_t bytes_idx = bits_idx / 8;
    uint32_t bits_odd = bits_idx % 8;
    if (value) {
        btmp->bits[bytes_idx] = btmp->bits[bytes_idx] | (value << bits_odd) ;
    } else {
        btmp->bits[bytes_idx] &= ~(value << bits_odd);
    }
}

