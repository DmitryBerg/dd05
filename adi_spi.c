#include "adi_spi.h"
#include "stdio.h"
#include "string.h"



typedef  struct{
	int cfg,
	isr,
	ien,
	idis,
	imsk,
	er,
	del,
	tx,
	rx,
	slaveCnt,
	txth,
	rxth;
} t_spi;

t_spi* volatile adi_spi = (t_spi*)ADI_SPI_PLL;

#define BAUD (3<<3)
#define MSTR 1
#define PERISEL (1<<9)
#define CPHA    (1<<2)
#define CLRSEL  (0xF<<10)
#define SEL0     (0xE<<10)
#define SEL0_    (0xE<<10)
#define CFG      ( 0x20000|MSTR|PERISEL|(1<<14)|BAUD )


#define READ      0x80
#define STREAM    0x60
#define RXFIFO    0x10
//#define adi_spi ((t_spi*)adi_spi)


#define GPIO_BASE 0xE000A000
#define pEMIO ((int*)(GPIO_BASE+0x68))

int HD_ID(){
    return *pEMIO;
}


int    spi_isr;
char spi_dump[32];
int adi_spi_rd(int adr){
	adi_spi->isr=0xFF;
	adi_spi->cfg=CFG | SEL0 ;
	adi_spi->rxth=2;
	adi_spi->er=1;

	adi_spi->tx=READ|((adr>>8)&0x0F);
	adi_spi->tx=adr&0xFF;
    for(;;){
    	if(adi_spi->isr & RXFIFO) break;
    }
    spi_isr = adi_spi->isr;

    adi_spi->er=0;
    adi_spi->cfg = CFG | SEL0_ ;

    adi_spi->rxth=1;
    adi_spi->isr=0xFF;
    adi_spi->er=1;
    adi_spi->tx=0xFF;
    spi_dump[0]=adi_spi->rx;   // read previously received data
    spi_dump[1]=adi_spi->rx;
    for(;;){
    	if(adi_spi->isr & 0x10) break;
    }
    int a; a=adi_spi->rx;
    spi_dump[2]=a;
    adi_spi->cfg= CFG | CLRSEL;
    adi_spi->er=0;
	return a;
}

void adi_spi_clear(){
	adi_spi->isr=0xFF;
	adi_spi->rxth=1;
	while(adi_spi->isr & 0x10){
	spi_dump[0] = adi_spi->rx;
	}
}

void adi_spi_write(int adr,int data){

	adi_spi->cfg=CFG | SEL0 ;
	adi_spi->rxth=3;
	adi_spi->isr=0xFF;
	adi_spi->er=1;

	adi_spi->tx=((adr>>8)&0x0F);
	adi_spi->tx=adr&0xFF;
	adi_spi->tx=data;
    for(;;){
    	if(adi_spi->isr & RXFIFO) break;
    }
   spi_dump[0]= adi_spi->rx;
   spi_dump[1]= adi_spi->rx;
   spi_dump[2]= adi_spi->rx;
    adi_spi->er=0;
    adi_spi->cfg = CFG | CLRSEL ;

}

#define IOUPDATE1 *((int*)0xE000A010)=0xFFFE0001
#define IOUPDATE0 *((int*)0xE000A010)=0xFFFE0000

void dds_spi_write(int adr,int data){

	adi_spi->rxth=5;
	adi_spi->isr=0xFF;
	adi_spi->er=1;

	adi_spi->tx=((adr)&0x1F);
	adi_spi->tx=data>>24;
	adi_spi->tx=data>>16;    
	adi_spi->tx=data>> 8;
	adi_spi->tx=data>> 0;    
    for(;;){
    	if(adi_spi->isr & RXFIFO) break;
    }
IOUPDATE1;    
   spi_dump[0]= adi_spi->rx;
   spi_dump[1]= adi_spi->rx;
   spi_dump[2]= adi_spi->rx;
   spi_dump[3]= adi_spi->rx;
   spi_dump[4]= adi_spi->rx;      
    adi_spi->er=0;

IOUPDATE0;    
}

void dds_select(int val){
    adi_spi->cfg = CFG | ((val&0x7)<<10);
}

//***********************************************
unsigned int adi_spi_rd_dds(int adr){
	adi_spi->isr=0xFF;

	adi_spi->rxth=1;
	adi_spi->er=1;

	adi_spi->tx=READ|((adr)&0x1F);
	    for(;;){
    	if(adi_spi->isr & RXFIFO) break;
    }
    *((int*)0xE000A010) = 0xFFFD0002;    
    spi_dump[0]=adi_spi->rx;

    adi_spi->rxth=4;
    adi_spi->isr=0xFF;

    adi_spi->tx=0xFF;
    adi_spi->tx=0xFF;
    adi_spi->tx=0xFF;
    adi_spi->tx=0xFF;

    for(;;){
    	if(adi_spi->isr & 0x10) break;
    }
    *((int*)0xE000A010) = 0xFFFD0000;    
    
    spi_dump[1]=adi_spi->rx;
    spi_dump[2]=adi_spi->rx;
    spi_dump[3]=adi_spi->rx;
    spi_dump[4]=adi_spi->rx;
        
    adi_spi->er=0;
    unsigned int a; 
    a=0x1000000*spi_dump[1]+0x10000*spi_dump[2]           
    +0x100*spi_dump[3]+spi_dump[4];
return a;
}

void dds_cfr_read(tDDS_CFR *p){
dds_select(0x6);   
p->CFR1 = adi_spi_rd_dds(0);
p->CFR2 = adi_spi_rd_dds(1);
p->CFR3 = adi_spi_rd_dds(2);
dds_select(0xFF);   
}

#define DRV0_1 (01<<28)
#define VCO7   (7<<24)
#define ICP7   (7<<19)
#define REFCLKDIVBYPASS (1<<15)
#define REFCLKRESETB    (1<<14)
#define PFDRESET        (1<<10)
#define PLLENABLE        (1<<8)

#define CFG1 (DRV0_1 | VCO7 | ICP7 | REFCLKRESETB)   // pll off
#define CFG2 (DRV0_1 | VCO7 | ICP7 | PLLENABLE | (40<<1))        // pll 1000MHz
#define CFG3 (DRV0_1 | VCO7 | ICP7 | PLLENABLE | (35<<1))        // pll 1000MHz

void dds_set1(){
    dds_select(6);
    dds_spi_write(2,CFG1);
    dds_select(7);
}
void dds_set2(){
    dds_select(6);
    dds_spi_write(2,CFG2);    
    dds_select(7);
}

void dds_set3(){
    dds_select(6);
    dds_spi_write(2,CFG3);    
    dds_select(7);
}

void dds_pfdreset(){
    unsigned int A;
    dds_select(6);
    A = adi_spi_rd_dds(2);    
    dds_spi_write(2,A | PFDRESET);    
    dds_spi_write(2,A );        
    dds_select(7);
}