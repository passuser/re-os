#include "memory.h"
#include "stdint.h"
#include "printf.h"

#define   PG_SIZE           4096 
#define   MEM_BITMAP_BASE   0XC009A000
#define   K_HEAP_SATART     0XC0100000

/*生成两个实例用于管理系统内存池和用户内存池*/
struct pool {
    
    struct bitmap pool_bitmap;
    uint32_t phy_addr_start;
    uint32_t pool_size;
};

struct pool kernel_pool,user_pool;
struct virtual_addr kernel_vaddr;

/*初始化内存池*/
static void mem_pool_init (uint32_t all_mem){

    put_str ("memory pool init start!\n");
    uint32_t page_table_size = PG_SIZE * 256;
    uint32_t used_mem = page_table_size + 0x100000;
    uint32_t free_mem = all_mem - used_mem;
    uint16_t all_free_pages = free_mem / PG_SIZE;

    uint16_t kernel_free_pages = all_free_pages / 2;
    uint16_t user_free_pages = all_free_pages - kernel_free_pages;

    uint32_t kbm_length = kernel_free_pages / 8;
    uint32_t ubm_length = user_free_pages / 8;
    uint32_t kp_start = used_mem;
//内核内存池的起始地址
    uint32_t up_start = kp_start + kernel_free_pages;
//用户内存池的起始地址
    kernel_pool.phy_addr_start = kp_start;
    user_pool.phy_addr_start = up_start;

    kernel_pool.pool_size = kernel_free_pages * PG_SIZE;
    user_pool.pool_size = user_free_pages * PG_SIZE;

    kernel_pool.pool_bitmap.btmp_bytes_len = kbm_length;
    user_pool.pool_bitmap.btmp_bytes_len = ubm_length;

    kernel_pool.pool_bitmap.bits = (void*) MEM_BITMAP_BASE;
    user_pool.pool_bitmap.bits = (void*) (MEM_BITMAP_BASE + kbm_length);

    put_str ("kernel pool bitmap start:0x");
    put_int ((int) kernel_pool.pool_bitmap.bits);
    put_str ("\n");
    put_str ("kernel pool phy_addr start:0x");
    put_int (kernel_pool.phy_addr_start);
    put_str ("\n");
    put_str ("user pool bitmap start:0x");
    put_int ((int)user_pool.pool_bitmap.bits);
    put_str ("\n");
    put_str ("user pool phy_addr start:0x");
    put_int ((int) user_pool.phy_addr_start);
    put_str ("\n");

    //位图置零
    bitmap_init (&kernel_pool.pool_bitmap);
    bitmap_init (&user_pool.pool_bitmap);

//初始化内核虚拟地址位图，按实际物理内存生成数组
    kernel_vaddr.vaddr_bitmap.btmp_bytes_len = kbm_length;
    kernel_vaddr.vaddr_bitmap.bits = (void*) (MEM_BITMAP_BASE + kbm_length + ubm_length);
    kernel_vaddr.vaddr_start = K_HEAP_SATART;
    bitmap_init (&kernel_vaddr.vaddr_bitmap);
    put_str ("memory pool init done!\n");
}

void mem_init () {
  
    put_str ("memory init start.\n");
    uint32_t mem_bytes_total = (*(uint32_t*)(0xb00));
    mem_pool_init (mem_bytes_total);
    put_str ("memory init done.\n");
}


