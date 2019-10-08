#include "ir_uart.h"
#include "ir_com.h"

int8_t ir_com_send_char(uint8_t c)
{
    ir_uart_putc(c);
    if (ir_uart_read_ready_p()) {
			uint8_t data; 
			data = ir_uart_getc();         // gets the character and stores it in data
    }
    return 0;
}