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
	while (!(USART3.STATUS & USART_DREIF_bm));
	USART3.TXDATAL = c;
}

void uart_putdata(uint32_t length, uint32_t force)
{
	char fourbit_to_ascii[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	for (uint8_t i=0; i < 6 ; i++)
	{
		uart_putchar( fourbit_to_ascii[ (length >> 4*(5 - i)) & 0xf ] );
	}
	for (uint8_t i=0; i < 6 ; i++)
	{
		uart_putchar( fourbit_to_ascii[ (force >> 4*(5 - i)) & 0xf ] );
	}
	uart_putchar('\n');
}

uint8_t ascii_hex_to_byte(uint8_t ascii_byte)
{
	if (ascii_byte < 0x3a) return ascii_byte - 0x30;
	if (ascii_byte < 0x37) return ascii_byte - 0x30;
	return ascii_byte - 0x57;
}

void com_handler(uint64_t in_data)
{
	if (in_data == 's')
	{
		PORTC.OUT &= ~PIN0_bm;
	}
	else if (in_data == 'r')
	{
		spi_init();
	}
	else if ((in_data >> 32) == 'l')
	{
		uint32_t speed = 0;
		speed = ascii_hex_to_byte((in_data >> 24) & 0xff) << 12;
		speed |= ascii_hex_to_byte((in_data >> 16) & 0xff) << 8;
		speed |= ascii_hex_to_byte((in_data >> 8) & 0xff) << 4;
		speed |= ascii_hex_to_byte((in_data) & 0xff);
		PORTC.OUT |= PIN0_bm | PIN1_bm;
		TCA0.SINGLE.CMP0BUF = speed;
	}
	else if ((in_data >> 32) == 'u')
	{
		uint32_t speed = 0;
		speed = ascii_hex_to_byte((in_data >> 24) & 0xff) << 12;
		speed |= ascii_hex_to_byte((in_data >> 16) & 0xff) << 8;
		speed |= ascii_hex_to_byte((in_data >> 8) & 0xff) << 4;
		speed |= ascii_hex_to_byte((in_data) & 0xff);
		PORTC.OUT |= PIN0_bm;
		PORTC.OUT &= ~PIN1_bm;
		TCA0.SINGLE.CMP0BUF = speed;
	}
}