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
    system_init();              // initialised the system modula
    pacer_init(PACER_RATE);     //  initialised a rate for the time taken to excecute
    navswitch_init();           // initial the navigation switch
    
    tinygl_init(PACER_RATE);
    tinygl_font_set(&font3x5_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_STEP);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
    

    Player rival;
    Player ally;
    set_player_pos(&rival, 3);  // set the players position in the matrix platform
    set_player_pos(&ally, 3);   // set the players position in the matrix platform
    

    while(1) {
        pacer_wait();           // count up to until the counter is equal to the TCNT.
        navswitch_update();     // navigation updates the North and South buttons that is declared 

        // if true the North Switch is pressed
        if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
            set_player_pos(&rival, rival.pos+1);      // moves the player rival to the left
            //ir_uart_putc ('1');
        }

        // if true the North Switch is pressed
        if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
            set_player_pos(&rival, rival.pos-1);    // moves the player rival to the right
            //ir_uart_putc ('2');
        }

        // Is the character to be transmitted
        if (ir_uart_read_ready_p ()) {
            uint8_t data; 
            data = ir_uart_getc ();         // gets the character and stores it in data
        }
        
        draw_enemy(rival);
        draw_ally(ally);
        tinygl_update();
    }

    return 0;
}