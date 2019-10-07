/* Author(s): Randipa Gunathilake, Mike Mika
*  Date: 07/10/2019 
*/

#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"
// #include "led.h"
// #include "spwm.h"

#include "players.h"

#define PACER_RATE 500
#define MESSAGE_RATE 10

int main(void)
{
    system_init();
    pacer_init(PACER_RATE);
    navswitch_init();
    
    tinygl_init(PACER_RATE);
    tinygl_font_set(&font3x5_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_STEP);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
    

    Player rival;
    Player ally;
    set_player_pos(&rival, 3);
    set_player_pos(&ally, 3);
    

    while(1) {
        pacer_wait();
        navswitch_update();        
        if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
            set_player_pos(&rival, rival.pos+1);
            //ir_uart_putc ('1');
        }
        if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
            set_player_pos(&rival, rival.pos-1);
            //ir_uart_putc ('2');
        }

        if (ir_uart_read_ready_p ()) {
            uint8_t data;
            data = ir_uart_getc ();
        }
        
        draw_enemy(rival);
        draw_ally(ally);
        tinygl_update();
    }

    return 0;
}