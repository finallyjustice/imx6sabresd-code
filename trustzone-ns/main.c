#include "mxc_serial.h"

#define __REG(x)        (*((volatile unsigned long *)(x)))

// Central Security Unit
void csu_init(void)
{
	unsigned long reg;
	/* For Peripheral access policy, This is may be similar with TZPC function in my opinion
	   it can be decide device secure or non-secure on the AXI-APB bus dynamically
	   now setting all peripheral can be access in non-secure
	   but it will change for secure access restrict, example the internal ram access permit
	*/
	for(reg = 0x021C0000; reg < 0x021C00A0; reg = reg + 4)
		__REG(reg) = 0x00ff00ff;

	// for OCRAM secure status setting
	// start address it 0, it means all internal ram will be use as secure ram  
	__REG(0x020e0028) = 0x00003810;
}

void bootmain(void)
{
	imx_iomux_init();
	mxc_serial_init();
	csu_init();

	cprintf("Test TrustZone NS-bit\n");

	int scr_val = 1;

	__asm__ volatile(
			"mrc p15, 0, r0, c1, c1, 0\n\t"
			"mov %0, r0\n\t"
			: "=r"(scr_val)
			: 
			: "r0"
	);

	if(scr_val & 0x1)
		cprintf("TrustZone NS Bit is 1 - Normal\n");
	else
		cprintf("TrustZone NS Bit is 0 - Secure\n");

	cprintf("SCR Reg: 0x%x\n", scr_val);

	cprintf("Begin Set TrustZone NS Bit to 1\n");
	
	__asm__ volatile(
			"mrc p15, 0, r0, c1, c1, 0\n\t"
			"mov r1, r0\n\t"
			"add r0, r1, #1\n\t"
			"mcr p15, 0, r0, c1, c1, 0\n\t"
			: 
			: 
			: "r0","r1"
	);

	cprintf("End Set TrustZone NS Bit to 1\n");
	
	cprintf("We cannot read SCR in Normal World!\n");
	scr_val = 0;

	__asm__ volatile(
			"mrc p15, 0, r0, c1, c1, 0\n\t"
			"mov %0, r0\n\t"
			: "=r"(scr_val)
			: 
			: "r0"
	);

	cprintf("Read TrustZone NS Bit\n");

	if(scr_val & 0x1)
		cprintf("TrustZone NS Bit is 1\n");
	else
		cprintf("TrustZone NS Bit is 0\n");


	while(1);
	return;
}
