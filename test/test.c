#include "common.h"
#include "console.h"
#include "string.h"
#include "debug.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "pmm.h"
#include "vmm.h"
#include "heap.h"
#include "task.h"
#include "thread.h"
#include "keyboard.h"


void pmm_test()
{
	printk_color(rc_black, rc_red, "\nThe Count of Physical Memory Page is: %u\n\n", phy_page_count);
	
	uint32_t allc_addr1 = NULL;

	uint32_t allc_addr2 = NULL;

	printk_color(rc_black, rc_light_brown, "Test Physical Memory Alloc :\n");
	
	allc_addr1 = pmm_malloc();

	allc_addr2 = pmm_malloc();

	printk("\n");
	pmm_free(allc_addr1);
	pmm_free(allc_addr2);

}

void kb_test()
{
	// 初始化键盘驱动
	init_keyboard_driver();
	
	// 解除对 INTR 中断的屏蔽
	asm volatile("sti");

	char ch;
	int color = rc_green;
	while (1) {
		ch = keyboard_getchar();
		if (ch) {
			if (ch == '\b') {
				console_putc_color(ch, rc_black, color);
				console_putc_color(' ', rc_black, rc_black);
			}
			console_putc_color(ch, rc_black, color);
		}
	}
}

int flag = 0;

int thread(void *arg)
{
	while (1) {
		if (flag == 1) {
			printk_color(rc_black, rc_green, "B");
			flag = 0;
		}
	}

	return 0;
}


void thread_test()
{
	init_sched();


	kernel_thread(thread, NULL);

	// 开启中断
	enable_intr();
	// int cnt = 0;
	while (1) {
		// if(cnt==100){
		// 	kthread_exit();
		// 	break;
		// }
		if (flag == 0) {
			printk_color(rc_black, rc_red, "A");
			flag = 1;
			// cnt++;
		}
	}
}