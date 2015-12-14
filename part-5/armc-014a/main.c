#include "uart.h"

int bootmain(int arg)
{
        cprintf("bootmain %x\n\n", arg);
        return arg;
}
