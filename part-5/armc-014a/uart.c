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
	while(*s)
		uart_send(*s++);
        uart_flush();
}

static int divr(unsigned num, unsigned den, unsigned *rem)
{
        int ret;
        ret = 0;
        while (num >= den)
        {
                num -= den;
                ret++;
        }
        *rem = num;
        return ret;
}

void printint(int xx, unsigned base, int sign)
{
	static char digits[] = "0123456789abcdef";
	char buf[16];
	unsigned i, x, r;

	if(sign && (sign = xx < 0))
		x = -xx;
	else
		x = xx;

	i = 0;
	do {
                x = divr(x, base, &r);
		buf[i++] = digits[r];
	} while(x != 0);

	if(sign)
		buf[i++] = '-';

	while(i-- > 0)
		uart_send(buf[i]);

        uart_flush();
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
				printstr(s);
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
