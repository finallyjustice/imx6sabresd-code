#include "mxc_serial.h"

void bootmain(void)
{
	imx_iomux_init();
	mxc_serial_init();

	cprintf("Hello World! %d\n", 11234);
	cprintf("i.MX6 Sabre Smart Device\n");

	while(1);
	return;
}
