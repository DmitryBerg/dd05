#include "xscugic.h"

#define scutimer      ( (int volatile*)0xf8f00600)
#define scutimer_load       scutimer[0]
#define scutimer_counter  scutimer[1]
#define scutimer_control   scutimer[2]
#define scutimer_interrupt scutimer[3]

#define PRESCALER 0
#define ENABLE      1
#define AUTOLOAD 2
#define IT_ENABLE  4
#define FLAGS (ENABLE|AUTOLOAD|IT_ENABLE)
#define PERIOD 1000000000                // 2 ms with prescaler = 0
int volatile*pgpio = (int*)0xE000A000;
#define ledon pgpio[0x10]=0;
#define ledoff pgpio[0x10]=1;

void set_scutimer(void);
int scutimer_tick(void);


void set_scutimer(void){

	scutimer_load     = PERIOD-1;
	scutimer_control = (PRESCALER*0x100) | FLAGS;
	scutimer_interrupt = 1;

	pgpio[0x81]=1;                    // zLite PS LED pin
	pgpio[0x82]=1;

	// If needed Set the interrup 29 in setup_interrupts
}

void setio(void){
	volatile int *p;

	p=(void*)0xF8000008; *p=0xDF0D;	// unlock
	p=(void*)0xF8000700; *p=0x1600;
	p=(void*)0xF8000004; *p=0x767B;   // lock

	p=(void*)0xE000A204; *p=1;				// DIR out
	p=(void*)0xE000A208; *p=1;             // OE
}
void setled(int val){
	volatile int *p;
	p=(void*)0xE000A040; *p=val;
}
// polling the scutimer interrupt flag
int scutime(void){
	if(scutimer_interrupt & 1){ scutimer_interrupt = 1;
	return 1;
	}
	return 0;
}
//int scutimer_tick(void){
//	if(scutimer_interrupt){
//		scutimer_interrupt=1; return 1;
//	}
//	return 0;
//}
