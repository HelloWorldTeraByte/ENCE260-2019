#include "ir_uart.h"
#include "ir_com.h"

//Returns 1 if the transmissino is recived by the other kit or 0 otherwise
int8_t ir_com_send_char(uint8_t c)
{
    ir_uart_putc(c);

    uint16_t retry_num = 0;
    while(retry_num < RETRY_AMOUNT) {
        if(ir_uart_read_ready_p()) {
		    uint8_t data; 
		    data = ir_uart_getc();         // gets the character and stores it in data
            if(data == 'C')
                return 1; 
        }
        retry_num++;
    }
    return 0;
}