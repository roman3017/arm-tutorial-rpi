#include "pl011.h"

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#define UART0_BASE   0x20201000

void uart_send( unsigned int c )
{
	pl011_putc(c, UART0_BASE);
}

unsigned int uart_recv( void )
{
	return pl011_getchar(UART0_BASE);
}

void uart_flush ( void )
{
	pl011_flush(UART0_BASE);
}

void uart_init ( void )
{
	pl011_init(UART0_BASE, 3000000, 115200);
}

void printstr(char *s)
{
	int i = 0;
	while(s[i])
	{
		if(s[i] == '\n')
			uart_send('\r');
		uart_send(s[i]);
		i++;
	}
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
	do {
		buf[i++] = digits[x % base];
	} while((x /= base) != 0);

	if(sign)
		buf[i++] = '-';

	while(--i >= 0)
		uart_send(buf[i]);
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
			if(c == '\n')
				uart_send('\r');
			uart_send(c);
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
					uart_send(*s);
				break;
			case '%':
				uart_send('%');
				break;
			default:
				uart_send('%');
				uart_send(c);
				break;
		}
	}
        uart_flush();
}
