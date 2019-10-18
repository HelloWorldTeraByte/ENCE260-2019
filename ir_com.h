#ifndef IR_COM
#define IR_COM

#include "system.h"

#define RETRY_AMOUNT 10000

//Send the passed in character using ir uart module
//Returns 1 if the send char is succesfuly recived by the other UCFK
int8_t ir_com_send_char(uint8_t c);

#endif