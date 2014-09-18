#define MUX_CTRL_OFS_SHIFT  0
#define MUX_CTRL_OFS_MASK   ((unsigned long long)0xfff << MUX_CTRL_OFS_SHIFT)
#define MUX_PAD_CTRL_OFS_SHIFT  12
#define MUX_PAD_CTRL_OFS_MASK   ((unsigned long long)0xfff << \
		MUX_PAD_CTRL_OFS_SHIFT)
#define MUX_SEL_INPUT_OFS_SHIFT 24
#define MUX_SEL_INPUT_OFS_MASK  ((unsigned long long)0xfff << \
		MUX_SEL_INPUT_OFS_SHIFT)

#define MUX_MODE_SHIFT      36
#define MUX_MODE_MASK       ((unsigned long long)0x1f << MUX_MODE_SHIFT)
#define MUX_PAD_CTRL_SHIFT  41
#define MUX_PAD_CTRL_MASK   ((unsigned long long)0x3ffff << MUX_PAD_CTRL_SHIFT)
#define MUX_SEL_INPUT_SHIFT 59
#define MUX_SEL_INPUT_MASK  ((unsigned long long)0xf << MUX_SEL_INPUT_SHIFT)

#define AIPS1_ARB_BASE_ADDR             0x02000000
#define ATZ1_BASE_ADDR              AIPS1_ARB_BASE_ADDR
#define AIPS1_OFF_BASE_ADDR         (ATZ1_BASE_ADDR + 0x80000)
#define IOMUXC_BASE_ADDR            (AIPS1_OFF_BASE_ADDR + 0x60000)
#define NO_PAD_CTRL     (1 << 17)

/*  
 * Generic virtual read/write.  Note that we don't support half-word
 * read/writes.  We define __arch_*[bl] here, and leave __arch_*w
 * to the architecture specific code.
 */ 
#define __arch_getb(a)          (*(volatile unsigned char *)(a))
#define __arch_getw(a)          (*(volatile unsigned short *)(a))
#define __arch_getl(a)          (*(volatile unsigned int *)(a))
#define __arch_getq(a)          (*(volatile unsigned long long *)(a))

#define __arch_putb(v,a)        (*(volatile unsigned char *)(a) = (v))
#define __arch_putw(v,a)        (*(volatile unsigned short *)(a) = (v))
#define __arch_putl(v,a)        (*(volatile unsigned int *)(a) = (v))
#define __arch_putq(v,a)        (*(volatile unsigned long long *)(a) = (v))

#define __raw_writeb(v,a)   __arch_putb(v,a)
#define __raw_writew(v,a)   __arch_putw(v,a)
#define __raw_writel(v,a)   __arch_putl(v,a)
#define __raw_writeq(v,a)   __arch_putq(v,a)

#define __raw_readb(a)      __arch_getb(a)
#define __raw_readw(a)      __arch_getw(a)
#define __raw_readl(a)      __arch_getl(a)
#define __raw_readq(a)      __arch_getq(a)

#define CONFIG_MXC_UART_BASE    UART1_BASE
#define UART1_BASE              (ATZ1_BASE_ADDR + 0x20000)
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

#define __REG(x)     (*((volatile unsigned int *)(x)))

#define UART_PHYS   CONFIG_MXC_UART_BASE

/* Register definitions */
#define URXD  0x0  /* Receiver Register */
#define UTXD  0x40 /* Transmitter Register */
#define UCR1  0x80 /* Control Register 1 */
#define UCR2  0x84 /* Control Register 2 */
#define UCR3  0x88 /* Control Register 3 */
#define UCR4  0x8c /* Control Register 4 */
#define UFCR  0x90 /* FIFO Control Register */
#define USR1  0x94 /* Status Register 1 */
#define USR2  0x98 /* Status Register 2 */
#define UESC  0x9c /* Escape Character Register */
#define UTIM  0xa0 /* Escape Timer Register */
#define UBIR  0xa4 /* BRM Incremental Register */
#define UBMR  0xa8 /* BRM Modulator Register */
#define UBRC  0xac /* Baud Rate Count Register */
#define UTS   0xb4 /* UART Test Register (mx31) */

/* UART Control Register Bit Fields.*/
#define  URXD_CHARRDY    (1<<15)
#define  URXD_ERR        (1<<14)
#define  URXD_OVRRUN     (1<<13)
#define  URXD_FRMERR     (1<<12)
#define  URXD_BRK        (1<<11)
#define  URXD_PRERR      (1<<10)
#define  URXD_RX_DATA    (0xFF)
#define  UCR1_ADEN       (1<<15) /* Auto dectect interrupt */
#define  UCR1_ADBR       (1<<14) /* Auto detect baud rate */
#define  UCR1_TRDYEN     (1<<13) /* Transmitter ready interrupt enable */
#define  UCR1_IDEN       (1<<12) /* Idle condition interrupt */
#define  UCR1_RRDYEN     (1<<9)	 /* Recv ready interrupt enable */
#define  UCR1_RDMAEN     (1<<8)	 /* Recv ready DMA enable */
#define  UCR1_IREN       (1<<7)	 /* Infrared interface enable */
#define  UCR1_TXMPTYEN   (1<<6)	 /* Transimitter empty interrupt enable */
#define  UCR1_RTSDEN     (1<<5)	 /* RTS delta interrupt enable */
#define  UCR1_SNDBRK     (1<<4)	 /* Send break */
#define  UCR1_TDMAEN     (1<<3)	 /* Transmitter ready DMA enable */
#define  UCR1_UARTCLKEN  (1<<2)	 /* UART clock enabled */
#define  UCR1_DOZE       (1<<1)	 /* Doze */
#define  UCR1_UARTEN     (1<<0)	 /* UART enabled */
#define  UCR2_ESCI	 (1<<15) /* Escape seq interrupt enable */
#define  UCR2_IRTS	 (1<<14) /* Ignore RTS pin */
#define  UCR2_CTSC	 (1<<13) /* CTS pin control */
#define  UCR2_CTS        (1<<12) /* Clear to send */
#define  UCR2_ESCEN      (1<<11) /* Escape enable */
#define  UCR2_PREN       (1<<8)  /* Parity enable */
#define  UCR2_PROE       (1<<7)  /* Parity odd/even */
#define  UCR2_STPB       (1<<6)	 /* Stop */
#define  UCR2_WS         (1<<5)	 /* Word size */
#define  UCR2_RTSEN      (1<<4)	 /* Request to send interrupt enable */
#define  UCR2_TXEN       (1<<2)	 /* Transmitter enabled */
#define  UCR2_RXEN       (1<<1)	 /* Receiver enabled */
#define  UCR2_SRST	 (1<<0)	 /* SW reset */
#define  UCR3_DTREN	 (1<<13) /* DTR interrupt enable */
#define  UCR3_PARERREN   (1<<12) /* Parity enable */
#define  UCR3_FRAERREN   (1<<11) /* Frame error interrupt enable */
#define  UCR3_DSR        (1<<10) /* Data set ready */
#define  UCR3_DCD        (1<<9)  /* Data carrier detect */
#define  UCR3_RI         (1<<8)  /* Ring indicator */
#define  UCR3_ADNIMP     (1<<7)  /* Autobaud Detection Not Improved */
#define  UCR3_RXDSEN	 (1<<6)  /* Receive status interrupt enable */
#define  UCR3_AIRINTEN   (1<<5)  /* Async IR wake interrupt enable */
#define  UCR3_AWAKEN	 (1<<4)  /* Async wake interrupt enable */
#define  UCR3_REF25	 (1<<3)  /* Ref freq 25 MHz */
#define  UCR3_REF30	 (1<<2)  /* Ref Freq 30 MHz */
#define  UCR3_INVT	 (1<<1)  /* Inverted Infrared transmission */
#define  UCR3_BPEN	 (1<<0)  /* Preset registers enable */
#define  UCR4_CTSTL_32   (32<<10) /* CTS trigger level (32 chars) */
#define  UCR4_INVR	 (1<<9)  /* Inverted infrared reception */
#define  UCR4_ENIRI	 (1<<8)  /* Serial infrared interrupt enable */
#define  UCR4_WKEN	 (1<<7)  /* Wake interrupt enable */
#define  UCR4_REF16	 (1<<6)  /* Ref freq 16 MHz */
#define  UCR4_IRSC	 (1<<5)  /* IR special case */
#define  UCR4_TCEN	 (1<<3)  /* Transmit complete interrupt enable */
#define  UCR4_BKEN	 (1<<2)  /* Break condition interrupt enable */
#define  UCR4_OREN	 (1<<1)  /* Receiver overrun interrupt enable */
#define  UCR4_DREN	 (1<<0)  /* Recv data ready interrupt enable */
#define  UFCR_RXTL_SHF   0       /* Receiver trigger level shift */
#define  UFCR_RFDIV      (7<<7)  /* Reference freq divider mask */
#define  UFCR_TXTL_SHF   10      /* Transmitter trigger level shift */
#define  USR1_PARITYERR  (1<<15) /* Parity error interrupt flag */
#define  USR1_RTSS	 (1<<14) /* RTS pin status */
#define  USR1_TRDY	 (1<<13) /* Transmitter ready interrupt/dma flag */
#define  USR1_RTSD	 (1<<12) /* RTS delta */
#define  USR1_ESCF	 (1<<11) /* Escape seq interrupt flag */
#define  USR1_FRAMERR    (1<<10) /* Frame error interrupt flag */
#define  USR1_RRDY       (1<<9)	 /* Receiver ready interrupt/dma flag */
#define  USR1_TIMEOUT    (1<<7)	 /* Receive timeout interrupt status */
#define  USR1_RXDS	 (1<<6)	 /* Receiver idle interrupt flag */
#define  USR1_AIRINT	 (1<<5)	 /* Async IR wake interrupt flag */
#define  USR1_AWAKE	 (1<<4)	 /* Aysnc wake interrupt flag */
#define  USR2_ADET	 (1<<15) /* Auto baud rate detect complete */
#define  USR2_TXFE	 (1<<14) /* Transmit buffer FIFO empty */
#define  USR2_DTRF	 (1<<13) /* DTR edge interrupt flag */
#define  USR2_IDLE	 (1<<12) /* Idle condition */
#define  USR2_IRINT	 (1<<8)	 /* Serial infrared interrupt flag */
#define  USR2_WAKE	 (1<<7)	 /* Wake */
#define  USR2_RTSF	 (1<<4)	 /* RTS edge interrupt flag */
#define  USR2_TXDC	 (1<<3)	 /* Transmitter complete */
#define  USR2_BRCD	 (1<<2)	 /* Break condition */
#define  USR2_ORE        (1<<1)	 /* Overrun error */
#define  USR2_RDR        (1<<0)	 /* Recv data ready */
#define  UTS_FRCPERR	 (1<<13) /* Force parity error */
#define  UTS_LOOP        (1<<12) /* Loop tx and rx */
#define  UTS_TXEMPTY	 (1<<6)	 /* TxFIFO empty */
#define  UTS_RXEMPTY	 (1<<5)	 /* RxFIFO empty */
#define  UTS_TXFULL	 (1<<4)	 /* TxFIFO full */
#define  UTS_RXFULL	 (1<<3)	 /* RxFIFO full */
#define  UTS_SOFTRST	 (1<<0)	 /* Software reset */

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
int mxc_serial_init(void)
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

	mxc_serial_putc('\n');
	mxc_serial_putc('H');
	mxc_serial_putc('E');
	mxc_serial_putc('L');
	mxc_serial_putc('L');
	mxc_serial_putc('O');
	mxc_serial_putc('\n');

	mxc_serial_putc('W');
	mxc_serial_putc('O');
	mxc_serial_putc('R');
	mxc_serial_putc('L');
	mxc_serial_putc('D');
	mxc_serial_putc('\n');

	return 0;
}

void bootmain(void)
{
	imx_iomux_v3_setup_pad(0x0361623000650280);
	imx_iomux_v3_setup_pad(0x0b61623920654284);
	mxc_serial_init();

	while(1);
	return;
}
