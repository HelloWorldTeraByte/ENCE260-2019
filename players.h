#ifndef PLAYERS_H
#define PLAYERS_H

#include "system.h"

typedef struct {
    uint8_t health;
    uint8_t pos;
    uint8_t prev_pos;
} Player;

void draw_enemy(Player p);
void draw_ally(Player player);

void set_player_pos(Player *player, uint8_t position);

#endif