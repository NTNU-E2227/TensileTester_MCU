#define F_CPU 20000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#include "UART.h"
#include "PWM.h"
#include "SPI.h"

ISR(USART3_RXC_vect)
{
	static uint32_t buffer = 0;
	uint8_t input = USART3.RXDATAL;
	
	if (input == '\n')
	{
		com_handler(buffer);
		buffer = 0;
	}
	else
	{
		buffer = (buffer << 8) | input;
	}
}

int main(void)
{
	PORTA.DIR = PIN4_bm | PIN6_bm | PIN7_bm;
	PORTB.DIR = PIN0_bm;
	PORTC.DIR = PIN4_bm | PIN1_bm | PIN0_bm;
	PORTD.DIR = PIN0_bm;
	
	PORTA.PIN5CTRL = PORT_PULLUPEN_bm; //MISO
	PORTB.PIN0CTRL = PORT_PULLUPEN_bm; //UART RX
	PORTC.PIN2CTRL = PORT_PULLUPEN_bm; //DRDY
	
	PORTC.OUT = PIN4_bm;//reset pin high
	
	uart_init();
	pwm_init();
	spi_init();

	while (1) {
		if (!(PORTD.IN & PIN1_bm)) {PORTC.OUT &= ~PIN0_bm;}
		
		if (!(PORTC.IN & PIN2_bm))
		{
			spi_rw(ADC_RS_DATA_gc | ADC_RDEN_bm | ADC_CS_FD12_gc); // Read from data register on fd channel 1/2
			uint32_t length_data = 0;
			length_data = spi_rw(0xff);
			length_data = (length_data << 8) | spi_rw(0xff);
			length_data = (length_data << 8) | spi_rw(0xff);
			
			while (PORTC.IN & PIN2_bm);
			
			
			spi_rw(ADC_RS_DATA_gc | ADC_RDEN_bm | ADC_CS_FD34_gc); // Read from data register on fd channel 3/4
			uint32_t stress_data = 0;
			stress_data = spi_rw(0xff);
			stress_data = (stress_data << 8) | spi_rw(0xff);
			stress_data = (stress_data << 8) | spi_rw(0xff);
			
			uart_putdata(length_data,stress_data);
		}
		
	}
}