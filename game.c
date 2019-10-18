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
    ir_uart_init();             // initialised IR driver
    timer_init();               // initialised timer
    navswitch_init();           // initialised the navigation switch
    button_init();              // initialised the button
    led_init();                 // initialised led
}


int
main(void)
{
    init_system();              // initialised the system
    init_graphics();            // initialised the graphics

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
    set_player_pos(&rival, 3);      // set the inital players position in the matrix platform
    set_player_pos(&ally, 3);       // set the initial players position in the matrix platform

    uint16_t ticks = 0;     // how many ticks to count at its maximum limit.
    uint16_t r_ticks = 0;   // how many ticks counted for a right jab. r means right.
    uint16_t l_ticks = 0;   // how many ticks counted for a left jab. l means left.
    uint16_t j_ticks = 0;   // how many ticks counted for a jab. j means jab.

    uint16_t ally_state_ticks = 0;      // how many ally_state ticks to count at its maximum limit ticks
    uint16_t rival_state_ticks = 0;     // how many rival state to count at its maximum limit ticks
    uint8_t ally_unstable_state = 0;    // how many ally unstable to count at its maximum limit state
    uint8_t rival_unstable_state = 0;   // how many rival unstable to count at its maximum limit state

    tinygl_text("WAITING");
    // make a while loop that runs the tinygl_update() until enough timer is elapsed.
    while(1) {
        pacer_wait();                                                       // count up to until the counter is equal to the TCNT.
        navswitch_update();                                                 // navigation updates the North and South buttons that is declared
        button_update();                                                    // updates the button when it is high or low is declared
        
        if(game_state == STATE_WAIT) {                                     // if the game state is on a waiting state then it will update the tingl text saying that it is WAITING"
            tinygl_update();      

            if(ir_uart_read_ready_p()) {                                  // Check is it true that something was inadvertently read.  
                uint8_t data;
                data = ir_uart_getc();                                   // gets the character and stores it in data
                
                if(data == ENC_BEGIN) {                                  // check to see if the first character equals the character that is stored in ENC_BEGIN which is "B"
                    game_data.rival_ready = true;                        // set the rival to true.
                }
            }

            if(game_data.ally_ready && game_data.rival_ready) {         // if both oposition are ready to play then the game state would begin.
                    game_state = STATE_BEGIN;                           // set the game state to begin now.
            }

        }

        if(game_state == STATE_BEGIN) {                         // once the game state is in the Begining process
            tinygl_update();                                    // the tinygl updates the tinygl text.
            ticks++;                                           

            if(!game_data.begin_init) {                         // if game data is false then it will begin to display the text 3 2 1 GO
                tinygl_text_mode_set(TINYGL_TEXT_MODE_STEP);    // Set the message display mode including step or scroll
                tinygl_clear();                                 // clears text display
                tinygl_text("3 2 1 GO");                        // message to display
            }

            if(!game_data.begin_init)                           //  game_data is false
                game_data.begin_init = true;                    // set the game_data to true

            if(ticks > BEGIN_WAIT) {                            // if the number of ticks is over the limit that is given
                tinygl_clear();                                 // it Clear display.
                game_state = STATE_PLAY;                        // set the game state to play
                ticks = 0;                                      // set the ticks back to 0.
            }
        }


        /*  This is to set how the two players would function this include punching, making a hook and block in 
                anticipation of both players try to hit each other
              */
        if(game_state == STATE_PLAY) {      
            tinygl_update();            // Update display (refresh display and update message).           

            //TODO: Move this to the players module
            unstable_state_mang(&ally_unstable_state, &ally_state_ticks, &ally.state);
            unstable_state_mang(&rival_unstable_state, &rival_state_ticks, &rival.state);

            if(game_data.r_hook_damage)  
                r_ticks++;                              // increment right_ticks by 1
            if(game_data.l_hook_damage)
                l_ticks++;                              // increment left_ticks by 1
            if(game_data.jab_damage)
                j_ticks++;                              // increment jab_ticks by 1

            if(r_ticks > MAX_BLOCK_TICKS) {             // if r for right ticks is above Max block ticks 
                r_ticks = 0;                            // reset the r ticks to 0
                ally.health -= RH_DAMAGE;               // decrease ally health by the Right Hook for (RH) damage which is 10
                game_data.r_hook_damage = false;        // We should no longer do damage
            }
            if(l_ticks > MAX_BLOCK_TICKS) {             // l means left for left ticks if it is above Max block ticks 
                l_ticks = 0;                            // the l_ticks is reset to 0
                ally.health -= LH_DAMAGE;               // decrease ally health by the Left Hook for (LH) damage which is 10
                game_data.l_hook_damage = false;        // We should no longer do damage

            }
            if(j_ticks > MAX_BLOCK_TICKS) {             // j means jab for j_ticks if it is above Max block ticks 
                j_ticks = 0;                            // reset jab ticks to 0
                ally.health -= J_DAMAGE;                // reduce ally health by jab damage which is 5
                game_data.jab_damage = false;           // We should no longer do damage
            }

            if(ally.health < 5) {                       // if ally health is less 5     
                game_state = STATE_OVER;                // the game state is set to state Over or game over
                game_data.rival_won = true;             // it lets the rival or the opposition side that you have won so sets it to true  
            }
            /* This is shows when the Left hook(STATE_LHOOK) or Right hook (STATE_RHOOK) or Jab (jab_damage)is activited*/

            if(ally.pos == rival.pos) {                              // if the position of the ally matches the position of the rival. 
                switch(rival.state) {                               //  For the rival state the following is as follows
                    case STATE_RHOOK:                               // For the STATE_RHOOK meaning Right Hook 
                        game_data.r_hook_damage = true;             // STATE_RHOOK it is set to true
                        break;
                    case STATE_LHOOK:                               // For the STATE_LHOOK meaning Right Hook if it is in this state
                        game_data.l_hook_damage = true;             // STATE_LHOOK  is set to true
                        break;
                    case STATE_JAB:                                 // For the STATE_JAB meaning Jab if it is in this state
                        game_data.jab_damage = true;                // jab_damage  is set to true
                        break;
                    default:
                        break;
                }
            }
            // switching in the case of ally state 
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
          
            //Draw the player and the ally with tinygl
            draw_enemy(rival, rival.state);
            draw_ally(ally);

            //Set the LED accoring to the health of the player ally
            spwm_duty_set(&health_spwm1, ally.health);
            led_set(LED1, spwm_update(&health_spwm1));
       }
        /* display the rival message text that  either you have won or not*/
       if(game_state == STATE_OVER) {                               // shows the game status when it is over
            tinygl_update();                                        // Update display (refresh display and update message). 

           if(!game_data.over_init) {               
                ir_com_send_char(ENC_GL);                           // transmit a character 'L'
                tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);      // Set the message display mode including step or scroll
                tinygl_clear();                                     // clears text display                  
                if(game_data.rival_won)                         
                    tinygl_text("YOU LOST");                        // message to display
                if(!game_data.rival_won)
                    tinygl_text("YOU WON");                         // message to display
            }
            if(!game_data.over_init)
                game_data.over_init = true;                     
       }


        // if true the North Switch is pressed
        if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
            
            switch (game_state) {               // if the game_state is default that is status waiting then it breaks.
                case STATE_WAIT:
                    break;

                case STATE_PLAY:                // the game_state is in the status of playing
                    if(ally.state == STATE_IDLE && ir_com_send_char(ENC_MR) == 1) {  
                        move_player_right(&ally);               // move player to the right
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

        // if the South Switch is pressed move to the left for the ally
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
        
        //Check if the navswitch has been pressed in
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
            //Only send confirmation if the data recived is within the expected range
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