#ifndef PLAYERS_H
#define PLAYERS_H

#include "system.h"

// A player that has the property including health, its position that is at and it previous position 
typedef struct {
    uint8_t health;
    uint8_t pos;
    uint8_t prev_pos;
} Player;

void draw_enemy(Player p); 
void draw_ally(Player player);

void set_player_pos(Player *player, uint8_t position);

#endif