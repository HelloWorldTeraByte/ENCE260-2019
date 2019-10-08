/* Author(s): Randipa Gunathilake, Mike Mika
 *  Date: 07/10/2019 
 */

#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "timer.h"
#include "../fonts/font3x5_1.h"

#include "players.h"

#define PACER_RATE 500
#define MESSAGE_RATE 10


typedef enum {STATE_WAIT, STATE_BEGIN, STATE_PLAY, STATE_OVER} game_state_t;

typedef struct {
	bool rival_ready;
	bool ally_ready;
} game_data_t;

void 
init_graphics()
{
	tinygl_init(PACER_RATE);
	tinygl_font_set(&font3x5_1);
	tinygl_text_speed_set(MESSAGE_RATE);
	tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
	tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
}

void 
init_system()
{
	system_init();              // initialised the system module
	pacer_init(PACER_RATE);     // initialised a rate for the time taken to excecute
	ir_uart_init();
	navswitch_init();           // initial the navigation switch
}

int 
main(void)
{
	init_system();
	init_graphics();
	
	game_state_t game_state = STATE_WAIT;
	game_data_t game_data = {false, false};

	Player rival;
	Player ally;

	set_player_pos(&rival, 3);  // set the players position in the matrix platform
	set_player_pos(&ally, 3);   // set the players position in the matrix platform

	tinygl_text("WAITING");

	while(1) {
		pacer_wait();           // count up to until the counter is equal to the TCNT.
		navswitch_update();     // navigation updates the North and South buttons that is declared 

		if(game_state == STATE_WAIT) {
			tinygl_update();

			if(ir_uart_read_ready_p()) {
				uint8_t data; 
				data = ir_uart_getc();         // gets the character and stores it in data
				if(data == 'B') {
					game_data.rival_ready = true;
				}
			}

			if(game_data.ally_ready && game_data.rival_ready) {
					game_state = STATE_BEGIN;
			}
			
		}
		if(game_state == STATE_BEGIN) {
			//tinygl_text_mode_set(TINYGL_TEXT_MODE_STEP);
			tinygl_text("3");

			tinygl_update();
			tinygl_clear();
			game_state = STATE_PLAY;
		}

		if(game_state == STATE_PLAY) {
			draw_enemy(rival);
			draw_ally(ally);
			tinygl_update();
		}

		// if true the North Switch is pressed
		if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
			switch (game_state) {
				case STATE_WAIT:
					break;

				case STATE_PLAY:
					set_player_pos(&rival, rival.pos-1);      // moves the player rival to the left
					break;

				case STATE_OVER:
					break;

				default:
					break;
			}
			//ir_uart_putc ('1');
		}

		// if true the North Switch is pressed
		if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
			switch (game_state) {
				case STATE_WAIT:
					break;

				case STATE_PLAY:
					set_player_pos(&rival, rival.pos+1);      // moves the player rival to the right
					break;

				case STATE_OVER:
					break;

				default:
					break;
			}
			//ir_uart_putc ('2');
		}

		if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
			switch (game_state) {
				case STATE_WAIT:
					game_data.ally_ready = true;
					ir_uart_putc('B');
					break;

				case STATE_PLAY:
					set_player_pos(&rival, rival.pos+1);      // moves the player rival to the right
					break;

				case STATE_OVER:
					break;

				default:
					break;
			}
			//ir_uart_putc ('2');
		}

		// Is the character ready to be recived
		if (ir_uart_read_ready_p ()) {
			uint8_t data; 
			data = ir_uart_getc ();         // gets the character and stores it in data
		}

	}

	return 0;
}
