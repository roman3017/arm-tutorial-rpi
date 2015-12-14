extern void uart_init(void);
extern void uart_flush(void);
extern void uart_send(unsigned int);
extern unsigned int uart_recv(void);

extern void printstr(char *s);
extern void printint(int xx, int base, int sign);
extern void cprintf(char *fmt, ...);
