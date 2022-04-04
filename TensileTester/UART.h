#ifndef UART_H_
#define UART_H_

#define BAUDRATE 115200
#define BAUD_FROM_RATE(RATE) ((4UL * (F_CPU))/(RATE))

void uart_init(void);
void uart_putchar(char c);
void uart_putdata(uint32_t length, uint32_t force);
void com_handler(uint32_t in_data);

#endif /* UART_H_ */