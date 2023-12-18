#ifndef ADI_SPI_H
#define ADI_SPI_H

#define ADI_SPI_PLL   ((int*)0xE0006000)
#define ADI_SPI_ADC ((int*)0xE0007000)
//extern int *adi_spi;

typedef struct {
    unsigned int CFR1,CFR2,CFR3 ;
} tDDS_CFR;
typedef tDDS_CFR* pDDS_CFR;


extern int adi_spi_rd(int adr);
extern void dds_select(int val);
unsigned int adi_spi_rd_dds(int adr);
extern void adi_spi_clear();
extern void adi_spi_write(int adr,int data);
extern int HD_ID();

extern void ad9715_init();
extern void adi_pll_header();
extern void adi_spi_soft_reset();

void dds_spi_write(int adr,int data);
//
// Read N bytes into the spi_dump buffer
//
extern void adi_spi_readn(int addr, int len);
extern void adi_spi_dump(int addr);

void dds_cfr_read(tDDS_CFR *p);

void dds_set1();    // set up for direct xtal reference
void dds_set2();    // set up for 1G VCO
void dds_set3();    // set up for 875M VCO
void dds_pfdreset();
#endif
