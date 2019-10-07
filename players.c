#include "players.h"
#include "tinygl.h"

void draw_enemy(Player p) 
{
    // TODO: Make sure that the -1 and +1 doesnt go off the display
    tinygl_draw_line(tinygl_point(0,p.prev_pos), tinygl_point(2,p.prev_pos), 0);
    tinygl_draw_line(tinygl_point(1,p.prev_pos-1), tinygl_point(1,p.prev_pos+1), 0);
    tinygl_draw_point(tinygl_point(3,p.prev_pos-1), 0);
    tinygl_draw_point(tinygl_point(3,p.prev_pos+1), 0);


    tinygl_draw_line(tinygl_point(0,p.pos), tinygl_point(2,p.pos), 1);
    tinygl_draw_line(tinygl_point(1,p.pos-1), tinygl_point(1,p.pos+1), 1);
    tinygl_draw_point(tinygl_point(3,p.pos-1), 1);
    tinygl_draw_point(tinygl_point(3,p.pos+1), 1);
}

void set_player_pos(Player *player, uint8_t position){
    player->prev_pos = player->pos;
    player->pos = position; 
}

void draw_ally(Player player)
{
    tinygl_draw_point(tinygl_point(4, player.prev_pos), 0);
    tinygl_draw_point(tinygl_point(4, player.pos), 1);
}