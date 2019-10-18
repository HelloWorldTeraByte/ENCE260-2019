#ifndef GAME_H
#define GAME_H

#include "system.h"
#include "players.h"

#define PACER_RATE 500
#define MESSAGE_RATE 10

//Encoding for the ir transmission
#define ENC_BEGIN 'B'
#define ENC_CONF 'C'
#define ENC_GL 'L'

#define ENC_MR 'd'
#define ENC_ML 'a'
#define ENC_RH 'z'
#define ENC_BRH 'Z'
#define ENC_LH 'x'
#define ENC_BLH 'X'
#define ENC_J 'v'
#define ENC_BJ 'V'

#define BEGIN_WAIT 2500
#define MAX_STATE_TICKS 250         //The amount of time each unstable state can last
#define MAX_BLOCK_TICKS 250         //The amount of time where the player can block the enemy 

#define LH_DAMAGE 10                //The damage caused by the left hook
#define RH_DAMAGE 10                //The damage caused by the right hook
#define J_DAMAGE  5                 //The damage caused by the jab

//The states the game can be in
typedef enum {STATE_WAIT, STATE_BEGIN, STATE_PLAY, STATE_OVER} game_state_t;

//The game data to keep track of the game status
typedef struct {
    bool rival_ready;
    bool ally_ready;
    bool begin_init;
    bool over_init;

    bool r_hook_damage;
    bool l_hook_damage;
    bool jab_damage;

    bool rival_won;
} game_data_t;


//Initialize every module used by the program
void init_system(void);

//Initialize the tinygl graphics module 
void init_graphics(void);

#endif
