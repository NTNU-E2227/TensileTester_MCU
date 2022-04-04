#ifndef F_CPU
#define F_CPU 20000000
#endif

#include <avr/io.h>
#include <util/delay.h>

#include "SPI.h"

void spi_init(void)
{
	PORTC.OUT &= ~PIN4_bm;
	
    SPI0.CTRLB = SPI_BUFEN_bm | SPI_BUFWR_bm | SPI_SSD_bm | SPI_MODE_3_gc;
    SPI0.CTRLA = SPI_MASTER_bm | SPI_PRESC_DIV16_gc | SPI_ENABLE_bm;
	
	_delay_ms(10);
	PORTC.OUT |= PIN4_bm;
	
	spi_rw(0xff);
	
	spi_rw(ADC_RS_FH_gc | ADC_CS_FD12_gc); //Write to filter high on fd channel 1/2
	spi_rw(ADC_FH_WL_bm | 0x01); //Word length 24-bit
	spi_rw(ADC_RS_FL_gc | ADC_CS_FD12_gc); //Write to filter low on fd channel 1/2
	spi_rw(0x80); //Filter to 50 Hz
	spi_rw(ADC_RS_MODE_gc | ADC_CS_FD12_gc); //Write to mode register on fd channel 1/2
	spi_rw(ADC_MR_SC_gc); //Run self calibration
	
	spi_rw(0xff);
	
	
	spi_rw(ADC_RS_FH_gc | ADC_FH_BST_bm | ADC_CS_FD34_gc); //Write to filter high on fd channel 3/4
	spi_rw(ADC_FH_WL_bm | 0x01); //Word length 24-bit
	spi_rw(ADC_RS_FL_gc | ADC_CS_FD34_gc); //Write to filter low on fd channel 3/4
	spi_rw(0x80); //Filter to 50 Hz
	spi_rw(ADC_RS_MODE_gc | ADC_CS_FD34_gc); //Write to mode register on fd channel 3/4
	spi_rw(ADC_MR_SC_gc | ADC_MR_GAIN128_gc); //Run self calibration, 128x gain
	
	spi_rw(0xff);
	
}

uint8_t spi_rw(uint8_t out_data)
{
	while (!(SPI0.INTFLAGS & SPI_DREIF_bm));
	SPI0.DATA = out_data;
	while (!(SPI0.INTFLAGS & SPI_RXCIF_bm));
	return SPI0.DATA;
}
