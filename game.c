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

#include "game.h"
#include "players.h"
#include "ir_com.h"
#include "button.h"
#include "led.h"
#include "spwm.h"

//Initialse the tinygl graphics library
void
init_graphics(void)
{
    tinygl_init(PACER_RATE);
    tinygl_font_set(&font3x5_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
}

//Initalise the system
void
init_system(void)
{
    system_init();              // initialised the system module
    pacer_init(PACER_RATE);     // initialised a rate for the time taken to excecute
    ir_uart_init();             // initialised  IR driver.
    timer_init();               // initialised 
    navswitch_init();           // initial the navigation switch
    button_init();
    led_init();
}


int
main(void)
{
    init_system();
    init_graphics();

    //LED to show the health of the ally
    spwm_t health_spwm1 = {.duty = 100, .period = 200};

    game_state_t game_state = STATE_WAIT;
    game_data_t game_data = {false, false, false, false, false, false, false, false};

    Player rival;       //Rival is the opponent
    Player ally;        //Ally is the player

    ally.health = 100;
    rival.health = 100;

    ally.state = STATE_IDLE;        //Initally the player is in idle state
    rival.state = STATE_IDLE;       //The rival is also in the idle state
    set_player_pos(&rival, 3);  // set the inital players position in the matrix platform
    set_player_pos(&ally, 3);   // set the initial players position in the matrix platform

    uint16_t ticks = 0;     // how many ticks to count at its maximum limit.
    uint16_t r_ticks = 0;   // how many ticks counted on a right jab. r means right.
    uint16_t l_ticks = 0;
    uint16_t j_ticks = 0;

    uint16_t ally_state_ticks = 0;
    uint16_t rival_state_ticks = 0;
    uint8_t ally_unstable_state = 0;
    uint8_t rival_unstable_state = 0;

    tinygl_text("WAITING");
    while(1) {
        pacer_wait();           // count up to until the counter is equal to the TCNT.
        navswitch_update();     // navigation updates the North and South buttons that is declared
        button_update();

        if(game_state == STATE_WAIT) {
            tinygl_update();

            if(ir_uart_read_ready_p()) {
                uint8_t data;
                data = ir_uart_getc();         // gets the character and stores it in data
                // make your own while loop that runs the tinygl_update() until enough timer is elapsed look timer.h.
                if(data == ENC_BEGIN) {
                    game_data.rival_ready = true;
                }
            }

            if(game_data.ally_ready && game_data.rival_ready) {
                    game_state = STATE_BEGIN;
            }

        }

        if(game_state == STATE_BEGIN) {
            tinygl_update();
            ticks++;

            if(!game_data.begin_init) {
                tinygl_text_mode_set(TINYGL_TEXT_MODE_STEP);
                tinygl_clear();
                tinygl_text("3 2 1 GO");
            }

            if(!game_data.begin_init)
                game_data.begin_init = true;

            if(ticks > BEGIN_WAIT) {
                tinygl_clear();
                game_state = STATE_PLAY;
                ticks = 0;
            }
        }

        if(game_state == STATE_PLAY) {
            tinygl_update();

            //TODO: Move this to the players module
            unstable_state_mang(&ally_unstable_state, &ally_state_ticks, &ally.state);
            unstable_state_mang(&rival_unstable_state, &rival_state_ticks, &rival.state);

            if(game_data.r_hook_damage)
                r_ticks++;
            if(game_data.l_hook_damage)
                l_ticks++;
            if(game_data.jab_damage)
                j_ticks++;

            if(r_ticks > MAX_BLOCK_TICKS) {
                r_ticks = 0;
                ally.health -= RH_DAMAGE;
                game_data.r_hook_damage = false;
            }
            if(l_ticks > MAX_BLOCK_TICKS) {
                l_ticks = 0;
                ally.health -= LH_DAMAGE;
                game_data.l_hook_damage = false;

            }
            if(j_ticks > MAX_BLOCK_TICKS) {
                j_ticks = 0;
                ally.health -= J_DAMAGE;
                game_data.jab_damage = false;
            }

            if(ally.health < 5) {
                game_state = STATE_OVER;
                game_data.rival_won = true;
            }

            if(ally.pos == rival.pos) {
                switch(rival.state) {
                    case STATE_RHOOK:
                        game_data.r_hook_damage = true;
                        break;
                    case STATE_LHOOK:
                        game_data.l_hook_damage = true;
                        break;
                    case STATE_JAB:
                        game_data.jab_damage = true;
                        break;
                    default:
                        break;
                }
            }

            switch(ally.state) {
                case STATE_B_RHOOK:
                    game_data.r_hook_damage = false;
                    r_ticks = 0;
                    break;
                case STATE_B_LHOOK:
                    game_data.l_hook_damage = false;
                    l_ticks = 0;
                    break;
                case STATE_B_JAB:
                    game_data.jab_damage = false;
                    j_ticks = 0;
                    break;
                default:
                    break;
            }
          
            //Update the matrix
            draw_enemy(rival, rival.state);
            draw_ally(ally);

            //Set the led accoring to the health of the player ally
            spwm_duty_set(&health_spwm1, ally.health);
            led_set(LED1, spwm_update(&health_spwm1));
       }

       if(game_state == STATE_OVER) {
            tinygl_update();

           if(!game_data.over_init) {
                ir_com_send_char(ENC_GL);
                tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
                tinygl_clear();
                if(game_data.rival_won)
                    tinygl_text("YOU LOST");
                if(!game_data.rival_won)
                    tinygl_text("YOU WON");
            }
            if(!game_data.over_init)
                game_data.over_init = true;
       }


        // if true the North Switch is pressed
        if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
            
            switch (game_state) {
                case STATE_WAIT:
                    break;

                case STATE_PLAY:
                    if(ally.state == STATE_IDLE && ir_com_send_char(ENC_MR) == 1) {
                        move_player_right(&ally);
                        ally.state = STATE_MV_R;
                        ally_unstable_state = 1;
                    }
                    break;

                case STATE_OVER:
                    break;

                default:
                    break;
            }
        }

        // if true the South Switch is pressed
        if(navswitch_push_event_p(NAVSWITCH_SOUTH)) {
            switch (game_state) {
                case STATE_WAIT:
                    break;

                case STATE_PLAY:
                    if(ally.state == STATE_IDLE && ir_com_send_char(ENC_ML) == 1) {
                        move_player_left(&ally);
                        ally.state = STATE_MV_L;
                        ally_unstable_state = 1;
                    }
                    break;

                case STATE_OVER:
                    break;

                default:
                    break;
            }
        }

        // if true the East Switch is pressed
        if(navswitch_push_event_p(NAVSWITCH_EAST)) {
            switch (game_state) {
                case STATE_WAIT:
                    break;

                case STATE_PLAY:
                    if(button_down_p(0)) {
                        if(ally.state == STATE_IDLE && ir_com_send_char(ENC_BRH) == 1)  {
                            ally.state = STATE_B_RHOOK;
                            ally_unstable_state = 1;
                        }
                    } else {
                        if(ally.state == STATE_IDLE && ir_com_send_char(ENC_RH) == 1)  {
                            ally.state = STATE_RHOOK;
                            ally_unstable_state = 1;
                        }
                    }
                    break;
                    
                case STATE_OVER:
                    break;

                default:
                    break;
            }
        }

        // if true the WEST Switch is pressed
        if(navswitch_push_event_p(NAVSWITCH_WEST)) {
            switch (game_state) {
                case STATE_WAIT:
                    break;

                case STATE_PLAY:
                    if(button_down_p(0)) {
                        if(ally.state == STATE_IDLE && ir_com_send_char(ENC_BLH) == 1)  {
                            ally.state = STATE_B_LHOOK;
                            ally_unstable_state = 1;
                        }
                    } else {
                        if(ally.state == STATE_IDLE && ir_com_send_char(ENC_LH) == 1)  {
                            ally.state = STATE_LHOOK;
                            ally_unstable_state = 1;
                        }
                    }
                    break;

                case STATE_OVER:
                    break;
                default:
                    break;
            }
        }

        if(navswitch_push_event_p(NAVSWITCH_PUSH)) {
            switch (game_state) {
                case STATE_WAIT:
                    game_data.ally_ready = true;
                    ir_uart_putc(ENC_BEGIN);
                    break;

                case STATE_PLAY:
                    if(button_down_p(0)) {
                        if(ally.state == STATE_IDLE && ir_com_send_char(ENC_BJ) == 1)  {
                            ally.state = STATE_B_JAB;
                            ally_unstable_state = 1;
                        }
                    } else {
                        if(ally.state == STATE_IDLE && ir_com_send_char(ENC_J) == 1)  {
                            ally.state = STATE_JAB;
                            ally_unstable_state = 1;
                        }
                    }
                    break;

                case STATE_OVER:
                    break;

                default:
                    break;
            }
        }

        // Read in IR data
        if(ir_uart_read_ready_p()) {
            uint8_t data;
            data = ir_uart_getc();         // gets the character and stores it in data
            if((data >= 'a' && data <= 'z') || (data >= 'A' && data <= 'Z'))
                ir_uart_putc(ENC_CONF);              //Send Confirmation back

            switch(data) {
                case ENC_ML:
                    rival.state = STATE_MV_L;
                    rival_unstable_state = 1;
                    move_player_left(&rival);      // moves the player rival to the left, the player movement is inverted
                    break;
                case ENC_MR:
                    rival.state = STATE_MV_R;
                    rival_unstable_state = 1;
                    move_player_right(&rival);     // moves the player rival to the right, the player movement is inverted
                    break;

                case ENC_RH:
                    rival.state = STATE_RHOOK;
                    rival_unstable_state = 1;
                    break;
                case ENC_BRH:
                    rival.state = STATE_B_RHOOK;
                    rival_unstable_state = 1;
                    break;

                case ENC_LH:
                    rival.state = STATE_LHOOK;
                    rival_unstable_state = 1;
                    break;
                case ENC_BLH:
                    rival.state = STATE_B_LHOOK;
                    rival_unstable_state = 1;
                    break;

                case ENC_J:
                    rival.state = STATE_JAB;
                    rival_unstable_state = 1;
                    break;
                case ENC_BJ:
                    rival.state = STATE_B_JAB;
                    rival_unstable_state = 1;
                    break;

                case ENC_GL:
                    game_state = STATE_OVER;
                    game_data.rival_won = false;
                    break;

                default:
                    break;
            }
        }

    }

    return 0;
}