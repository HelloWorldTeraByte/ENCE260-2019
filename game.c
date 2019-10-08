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
#include "ir_com.h"

#define PACER_RATE 500
#define MESSAGE_RATE 10


typedef enum {STATE_WAIT, STATE_BEGIN, STATE_PLAY, STATE_OVER} game_state_t;

typedef struct {
	bool rival_ready;
	bool ally_ready;
	bool begin_init;
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
	timer_init();
	navswitch_init();           // initial the navigation switch
}

int 
main(void)
{
	init_system();
	init_graphics();
	
	game_state_t game_state = STATE_WAIT;
	game_data_t game_data = {false, false, false};

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
				if(data == 'b') {
					game_data.rival_ready = true;
				}
			}

			if(game_data.ally_ready && game_data.rival_ready) {
					game_state = STATE_BEGIN;
			}
			
		}

		if(game_state == STATE_BEGIN) {
			//TODO: Fix the timing
			timer_tick_t start;
			timer_tick_t now;

			if(!game_data.begin_init) {
				tinygl_text_mode_set(TINYGL_TEXT_MODE_STEP);
				tinygl_clear();
				tinygl_text("3 2 1 GO");
				start = timer_get();
			}

			if(!game_data.begin_init)
				game_data.begin_init = true;
			
			tinygl_update();

			//now = timer_wait_until(now + (timer_tick_t)(TIMER_RATE * 2));

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
					move_player_right(&ally);      // moves the player rival to the left
					ir_uart_putc('d');
					break;

				case STATE_OVER:
					break;

				default:
					break;
			}
		}

		// if true the North Switch is pressed
		if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
			switch (game_state) {
				case STATE_WAIT:
					break;

				case STATE_PLAY:
					move_player_left(&ally);      // moves the player rival to the left
					ir_uart_putc('a');
					break;

				case STATE_OVER:
					break;

				default:
					break;
			}
		}

		// if true the North Switch is pressed
		if (navswitch_push_event_p(NAVSWITCH_EAST)) {
			switch (game_state) {
				case STATE_WAIT:
					break;

				case STATE_PLAY:
					break;

				case STATE_OVER:
					break;
				default:
					break;
			}
		}

		if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
			switch (game_state) {
				case STATE_WAIT:
					game_data.ally_ready = true;
					ir_uart_putc('b');
					break;

				case STATE_PLAY:
					break;

				case STATE_OVER:
					break;

				default:
					break;
			}
		}

		// Is the character ready to be recived
		if (ir_uart_read_ready_p ()) {
			uint8_t data; 
			data = ir_uart_getc();         // gets the character and stores it in data
			switch (data) {
				case 'a':
					move_player_left(&rival);      // moves the player rival to the left, the player movement is inverted
					break;
				case 'd':
					move_player_right(&rival);     // moves the player rival to the right, the player movement is inverted
					break;
				default:
					break;
			}
		}

	}

	return 0;
}
