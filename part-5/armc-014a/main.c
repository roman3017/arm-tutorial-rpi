#include "uart.h"

int bootmain(int arg)
{
        cprintf("bootmain %s%x\n\n", "0x", arg);
        return arg;
}
