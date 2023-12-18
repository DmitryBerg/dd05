#include "uart.h"

#define BASE 0xE0000000

#define RXEMPTY 2
#define RXTOUT_FLAG (1<<8)

int* pUART = (int*)BASE;

#define SR     pUART[0x2C/4]
#define RXTOUT pUART[0x1C/4]

void set_uart(){
    RXTOUT=50;    // receive timeout
}

int uart(){
    
if(SR & RXEMPTY){ return 1;}
return 0;
}