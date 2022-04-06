#ifndef SPI_H_
#define SPI_H_

#define ADC_CS_FD12_gc 0x04
#define ADC_CS_FD34_gc 0x05

#define ADC_RS_FH_gc 0x20
#define ADC_RS_FL_gc 0x30
#define ADC_RS_MODE_gc 0x10
#define ADC_RS_DATA_gc 0x50

#define ADC_DRDY_bm 0x80
#define ADC_RDEN_bm 0x08

#define ADC_FH_BST_bm 0x20
#define ADC_FH_WL_bm 0x40

#define ADC_MR_GAIN8_gc 0x0c
#define ADC_MR_GAIN64_gc 0x18
#define ADC_MR_GAIN128_gc 0x1c
#define ADC_MR_SC_gc 0x20
#define ADC_MR_FSSC_gc 0xe0

void spi_init(void);
uint8_t spi_rw(uint8_t out_data);

#endif /* SPI_H_ */