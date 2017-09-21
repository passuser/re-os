#include "interrupt.h"
#include "stdint.h"
#include "global.h"
#include "io.h"


#define PIC_M_CTRL  0x20
#define PIC_M_DATA  0x21
#define PIC_S_CTRL  0xa0
#define PIC_S_CTRL  0xa1
#define IDT_DESC_CNT 0x21
struct gate_desc
{
	uint16_t    func_offset_low_word;
	uint16_t    selector;
	uint8_t     dcount;
	uint8_t     attribute;
	uint16_t    func_offset_high_word;
};

static void make_idt_desc(struct gate_desc* p_gdesc,uint8_t attr,intr_handler function);
static struct gate_desc idt[IDT_DESC_CNT];
extern intr_handler intr_entry_table[IDT_DESC_CNT];





static void pic_init(void)
{
    outb (PIC_M_CTRL,0x11);
    outb (PIC_M_DATA,0x20);
    outb (PIC_M_DATA,0x4);
    outb (PIC_M_DATA,0x1);
    outb (PIC_S_CTRL,0x11);
    outb (PIC_S_DATA,0x28);
    outb (PIC_S_DATA,0x2);
    outb (PIC_S_DATA,0x1);
    outb (PIC_M_DATA,0xfe);
    outb (PIC_S_DATA,0xff);
    put_str ("   pic_init  done\n");
}

static void make_idt_desc(struct gate_desc* p_gdesc,uint8_t attr, intr_handler function)
{
	p_gdesc -> func_offset_low_word = (uint32_t)function & 0xffff;
	p_gdesc -> selector = SELECTOR_K_CODE;
	p_gdesc -> dcount = 0;
        p_gdesc -> attribute = attr;
	p_gdesc -> func_offset_high_word = ((uint32_t)function & 0xffff0000) >> 16;
}
static idt_desc_init(void){
	int i;
	for(i = 0;i < IDT_DESC_CNT;i++){
	make_idt_desc(idt[i],IDT_DESC_ATTR_DPL0,intr_entry_table[i]);
	}
	put_str ("   idt_desc_init  done\n");
}
void idt_init(){
	put_str ("idt_init start\n");
	idt_desc_init();
	pic_init();

	uint64_t idt_operand = ((sizeof(idt) - 1) | ((uint64_t)((uint32_t) << 16)));
	asm volatile ("lidt %0":: "m" (idt_operand));
	put_str("idt_init done\n");
}


