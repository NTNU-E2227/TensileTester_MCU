#ifndef F_CPU
#define F_CPU 20000000
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#include "UART.h"
#include "SPI.h"

void uart_init(void)
{
	cli();
	_PROTECTED_WRITE((CLKCTRL.MCLKCTRLB), (0 << CLKCTRL_PEN_bp));
	USART3.BAUD = BAUD_FROM_RATE(BAUDRATE);
	USART3.CTRLA = USART_RXCIE_bm;
	USART3.CTRLB = USART_TXEN_bm | USART_RXEN_bm;
	USART3.CTRLC = USART_SBMODE_2BIT_gc | USART_CHSIZE_8BIT_gc;
	sei();
}

void uart_putchar(char c)
{
	while (!(USART3.STATUS & USART_DREIF_bm))
	;
	USART3.TXDATAL = c;
}

void uart_putdata(uint32_t length, uint32_t force)
{
	uart_putchar(length >> 16);
	uart_putchar((length >> 8) & 0xff);
	uart_putchar(length & 0xff);
	uart_putchar(force >> 16);
	uart_putchar((force >> 8) & 0xff);
	uart_putchar(force & 0xff);
	uart_putchar('\n');
}

void com_handler(uint32_t in_data)
{
	if (in_data == 's')
	{
		PORTC.OUT &= ~PIN0_bm;
	}
	if (in_data == 'r')
	{
		spi_init();
	}
	else if ((in_data >> 16) == 'l')
	{
		PORTC.OUT |= PIN0_bm | PIN1_bm;
		TCA0.SINGLE.CMP0BUF = in_data & 0xffff;
	}
	else if ((in_data >> 16) == 'u')
	{
		PORTC.OUT |= PIN0_bm;
		PORTC.OUT &= ~PIN1_bm;
		TCA0.SINGLE.CMP0BUF = in_data & 0xffff;
	}
}