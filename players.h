/* Author(s): Randipa Gunathilake, Mike Mika
*  Date: 07/10/2019 
*/

#ifndef PLAYERS_H
#define PLAYERS_H

#include "system.h"
#include "game.h"

//Keep track of the state the player is in
typedef enum {STATE_IDLE, STATE_MV_L, STATE_MV_R,
                STATE_RHOOK, STATE_LHOOK, STATE_JAB,
                STATE_B_RHOOK, STATE_B_LHOOK, STATE_B_JAB
} player_state_t;

// A player that has the property including health, its position that is at and it previous position 
typedef struct {
    int16_t health;
    uint8_t pos;            //Playes position
    uint8_t prev_pos;       //Players previous position
    player_state_t state;   //Player state 
} Player;

void draw_enemy(Player p, player_state_t state); 
void draw_ally(Player player);

void set_player_pos(Player *player, uint8_t position);
void move_player_left(Player *player);
void move_player_right(Player *player);

void unstable_state_mang(uint8_t *unstable_state, uint16_t *state_ticks, player_state_t *state);

#endif
