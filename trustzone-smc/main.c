#include "mxc_serial.h"

// register the secure monitor vector
extern void init_secure_monitor();

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

// code in TrustZone Normal World
void normal_world()
{
	while(1)
	{
		cprintf("$$ Say hello in TrustZone ^^Normal^^ World!\n");
		// transit to Secure Monitor (Dst: Secure World)
		asm volatile("smc #0\n\t");
	}
}

void bootmain(void)
{
	imx_iomux_init();
	mxc_serial_init();
	csu_init();

	cprintf("Test ARM TrustZone on i.MX53 Quick Start Board.\n");

	// register the TrustZone secure monitor vector
	cprintf("Register the TrustZone Secure Monitor Vector.\n");
	init_secure_monitor(normal_world);
	cprintf("Secure Monitor Vector Registration Successfully!\n");

	int i;
	for(i=0; i<10; i++)
	{
		cprintf("## Say hello in TrustZone ^^Secure^^ World! %d\n", i);
		// transit to Secure Monitor (Dst: Normal World)
		asm volatile("smc #0\n\t");
	};

	cprintf("Finish the TrustZone world-transition test! Congrat!!!!\n");

	while(1);
	return;
}
