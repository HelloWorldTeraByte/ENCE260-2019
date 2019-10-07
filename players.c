#include "players.h"
#include "tinygl.h"

void draw_enemy(Player p) 
{
    // TODO: Make sure that the -1 and +1 doesnt go off the display
    tinygl_draw_line(tinygl_point(0,p.prev_pos), tinygl_point(2,p.prev_pos), 0); 
    tinygl_draw_line(tinygl_point(1,p.prev_pos-1), tinygl_point(1,p.prev_pos+1), 0);
    tinygl_draw_point(tinygl_point(3,p.prev_pos-1), 0);
    tinygl_draw_point(tinygl_point(3,p.prev_pos+1), 0);

    // TODO: Make sure that the player does not go out of bound, so-1 and +1 doesnt go off the display matrix
    tinygl_draw_line(tinygl_point(0,p.pos), tinygl_point(2,p.pos), 1);
    tinygl_draw_line(tinygl_point(1,p.pos-1), tinygl_point(1,p.pos+1), 1);
    tinygl_draw_point(tinygl_point(3,p.pos-1), 1);
    tinygl_draw_point(tinygl_point(3,p.pos+1), 1);
}
// to clear out the position and to move to another set of matrix pin.
void draw_ally(Player player)
{
    tinygl_draw_point(tinygl_point(4, player.prev_pos), 0);    // to clear out the previous movement of the player.
    tinygl_draw_point(tinygl_point(4, player.pos), 1);        // to move to that current position.
}


// set the player position as it moves from left to right
void set_player_pos(Player *player, uint8_t pos){ 
    if(pos > 1 && pos < TINYGL_HEIGHT-2) {   
        player->prev_pos = player->pos;                     // set the previous postion to the current position 
        player->pos = pos;                                  // set the current position after the previous position
    }
}

