#include "config.h"

void imx_iomux_v3_setup_pad(unsigned long long pad)
{
	void *base = (void *)IOMUXC_BASE_ADDR;

	unsigned int mux_ctrl_ofs = (pad & MUX_CTRL_OFS_MASK) >> MUX_CTRL_OFS_SHIFT;
	unsigned int mux_mode = (pad & MUX_MODE_MASK) >> MUX_MODE_SHIFT;
	unsigned int sel_input_ofs =
		(pad & MUX_SEL_INPUT_OFS_MASK) >> MUX_SEL_INPUT_OFS_SHIFT;
	unsigned int sel_input =
		(pad & MUX_SEL_INPUT_MASK) >> MUX_SEL_INPUT_SHIFT;
	unsigned int pad_ctrl_ofs =
		(pad & MUX_PAD_CTRL_OFS_MASK) >> MUX_PAD_CTRL_OFS_SHIFT;
	unsigned int pad_ctrl = (pad & MUX_PAD_CTRL_MASK) >> MUX_PAD_CTRL_SHIFT;
	if (mux_ctrl_ofs)
	{
		__raw_writel(mux_mode, base + mux_ctrl_ofs);
	}

	if (sel_input_ofs)
	{
		__raw_writel(sel_input, base + sel_input_ofs);
	}

	if (!(pad_ctrl & NO_PAD_CTRL) && pad_ctrl_ofs)
	{
		__raw_writel(pad_ctrl, base + pad_ctrl_ofs);
	}
}

void imx_iomux_init(void)
{
	imx_iomux_v3_setup_pad(0x0361623000650280);
	imx_iomux_v3_setup_pad(0x0b61623920654284);
}

void mxc_serial_setbrg(void)
{
	__REG(UART_PHYS + UFCR) = 4 << 7; /* divide input clock by 2 */
	__REG(UART_PHYS + UBIR) = 0xf;
	__REG(UART_PHYS + UBMR) = 347;
}

void mxc_serial_putc(const char c)
{
	__REG(UART_PHYS + UTXD) = c;
	
	/* wait for transmitter to be ready */
	while (!(__REG(UART_PHYS + UTS) & UTS_TXEMPTY))
		;

	/* If \n, also do \r */
	if (c == '\n')
		mxc_serial_putc ('\r');
}

/*
 * Initialise the serial port with the given baudrate. The settings
 * are always 8 data bits, no parity, 1 stop bit, no start bits.
 *
 */
void mxc_serial_init(void)
{
	__REG(UART_PHYS + UCR1) = 0x0;
	__REG(UART_PHYS + UCR2) = 0x0;

	while (!(__REG(UART_PHYS + UCR2) & UCR2_SRST));

	__REG(UART_PHYS + UCR3) = 0x0704 | UCR3_ADNIMP;
	__REG(UART_PHYS + UCR4) = 0x8000;
	__REG(UART_PHYS + UESC) = 0x002b;
	__REG(UART_PHYS + UTIM) = 0x0;

	__REG(UART_PHYS + UTS) = 0x0;

	mxc_serial_setbrg();

	__REG(UART_PHYS + UCR2) = UCR2_WS | UCR2_IRTS | UCR2_RXEN | UCR2_TXEN | UCR2_SRST;
	
	__REG(UART_PHYS + UCR1) = UCR1_UARTEN;

	return;
}

void printint(int xx, int base, int sign)
{
	static char digits[] = "0123456789abcdef";
	char buf[16];
	int i;
	unsigned int x;

	if(sign && (sign = xx < 0))
		x = -xx;
	else
		x = xx;

	i = 0;
	do{
		buf[i++] = digits[x % base];
	}while((x /= base) != 0);

	if(sign)
		buf[i++] = '-';

	while(--i >= 0)
		mxc_serial_putc(buf[i]);
}

void cprintf(char *fmt, ...)
{
	int i, c;
	unsigned int *argp;
	char *s;

	argp = (unsigned int*)(void*)(&fmt + 1);
	for(i = 0; (c = fmt[i] & 0xff) != 0; i++)
	{
		if(c != '%')
		{
			mxc_serial_putc(c);
			continue;
		}
		c = fmt[++i] & 0xff;
		if(c == 0)
			break;
		switch(c)
		{
		case 'd':
			printint(*argp++, 10, 1);
			break;
		case 'x':
		case 'p':
			printint(*argp++, 16, 0);
			break;
		case 's':
			if((s = (char*)*argp++) == 0)
				s = "(null)";
			for(; *s; s++)
				mxc_serial_putc(*s);
			break;
		case '%':
			mxc_serial_putc('%');
			break;
		default:
			// Print unknown % sequence to draw attention.
			mxc_serial_putc('%');
			mxc_serial_putc(c);
			break;
		}
	}
}

