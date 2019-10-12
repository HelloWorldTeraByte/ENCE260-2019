/* Author(s): Randipa Gunathilake, Mike Mika
*  Date: 07/10/2019 
*/

#ifndef PLAYERS_H
#define PLAYERS_H

#include "system.h"
#include "game.h"

// A player that has the property including health, its position that is at and it previous position 
typedef struct {
    uint8_t health;
    uint8_t pos;
    uint8_t prev_pos;
} Player;

void draw_enemy(Player p, player_state_t state); 
void draw_ally(Player player);

void draw_enemy_state(Player p, player_state_t state);
void draw_enemy_body(Player p); 
void clear_enemy(void);

void set_player_pos(Player *player, uint8_t position);
void move_player_left(Player *player);
void move_player_right(Player *player);

#endif
