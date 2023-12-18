/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include <string.h>
#include "platform.h"
#include "xil_printf.h"

#include "scu_timer.h"
#include "adi_spi.h"
#include "uart.h"

void interpret(char*s);

int main()
{
    int *p=(int *)0xe000a010;
    
    init_platform();

    printf("DDS control\n\r");
    printf("HD ID %8.8X\n\r",HD_ID());    

    set_scutimer();
    set_uart();
    
    *((int*)0xE000A288)=3;   //OEN
    *((int*)0xE000A284)=3;   //OEN    
    
    *p=0xFFFE0001;
dds_select(0xE);
dds_spi_write(2, 0x1F3F4100+(40<<1));

dds_select(0xF);

int k=0;    
for(int i=0;;)    
{   
    if(uart()){
    char  s[128];
    char c;
    c=getc(stdin);
    if(c==0xA){continue;};
    if(c==0xD){
    printf("-> %s\n\r",s);
    interpret(s);
    k=0; s[k]=0;    continue;
    }
    
    s[k++]=c; s[k]=0;
    }
    
    if(scutime()==99){
unsigned int a[4];
dds_select(0xE);
    dds_spi_write(0, 4);
    for(int j=0;j<4;j++){
    a[j]=adi_spi_rd_dds(j);};
dds_select(0xFF);

    printf("tick %d\n\r%8.8X %8.8X\n\r",i++,a[0],a[1]);
    printf("%8.8X %8.8X\n\r",a[2],a[3]);    
}
}

    cleanup_platform();
    return 0;
}

void _read_cfr(){
    tDDS_CFR A; 
    dds_cfr_read(&A);
    printf("CFR1   %8.8X\n\r",A.CFR1);
    printf("CFR1   %8.8X\n\r",A.CFR2);
    printf("CFR1   %8.8X\n\r",A.CFR3);        
    }



void interpret(char*s){

char *s1;
s1= strtok(s," ");

if(strncmp(s1,"nfo?",4)==0){
printf("DDS monitot v4.0\n\r") ;      
return;   
}
if(strncmp(s1,"cfr?",4)==0){
printf("HD ID %8.8X\n\r",HD_ID());        
printf("CFR\n\r") ;   
_read_cfr();   
return;   
}
if(strncmp(s1,"set1",4)==0){
printf("CFR\n\r") ;
dds_set1();   
_read_cfr();   
return;   
}
if(strncmp(s1,"set2",4)==0){
printf("CFR\n\r") ;
dds_set2();   
_read_cfr();   
return;   
}
if(strncmp(s1,"set3",4)==0){
printf("CFR\n\r") ;
dds_set3();   
_read_cfr();   
return;   
}
if(strncmp(s1,"pfdr",4)==0){
printf("CFR\n\r") ;
dds_pfdreset();   
_read_cfr();   
return;   
}
if(strncmp(s1,"plln",4)==0){
    s1=strtok(NULL, " ");
unsigned int N;    
    sscanf(s1,"%4u",&N) ;   
dds_select(6);
unsigned int A = adi_spi_rd_dds(2);
A&=~0xFF; A|=(N&0x7F)<<1;
dds_spi_write(2,A);
dds_select(0xf);

return;   
}

/*
for(;;){
    if(s1==NULL) break;    
    printf("%s\n\r",s1);
    s1= strtok(NULL," ");
}
*/

}