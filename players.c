/* Author(s): Randipa Gunathilake, Mike Mika
*  Date: 07/10/2019 
*/

#include "players.h"
#include "tinygl.h"

void 
draw_enemy(Player p, player_state_t state) 
{
    clear_enemy();
    draw_enemy_body(p);
    draw_enemy_state(p, state);
}

void 
draw_enemy_body(Player p)
{
    tinygl_draw_line(tinygl_point(0,p.pos), tinygl_point(2,p.pos), 1);
    tinygl_draw_line(tinygl_point(1,p.pos-1), tinygl_point(1,p.pos+1), 1);
    tinygl_draw_point(tinygl_point(3,p.pos-1), 1);
    tinygl_draw_point(tinygl_point(3,p.pos+1), 1);
}

void 
draw_enemy_state(Player p, player_state_t state)
{
    switch (state) {
        case STATE_IDLE:
            //clear_enemy();
            //draw_enemy_body(p);
            break;
        case STATE_RHOOK:
            tinygl_draw_point(tinygl_point(1,p.pos-2), 1);
            break;
        case STATE_B_RHOOK:
            tinygl_draw_line(tinygl_point(0,p.pos-2), tinygl_point(3,p.pos-2), 1);
            break;

        case STATE_LHOOK:
            tinygl_draw_point(tinygl_point(1,p.pos+2), 1);
            break;
        case STATE_B_LHOOK:
            tinygl_draw_line(tinygl_point(0,p.pos+2), tinygl_point(3,p.pos+2), 1);
            break;

        case STATE_JAB:
            tinygl_draw_line(tinygl_point(0,p.pos-1), tinygl_point(2,p.pos+1), 1);
            tinygl_draw_line(tinygl_point(0,p.pos+1), tinygl_point(2,p.pos-1), 1);
            break;
        case STATE_B_JAB:
            tinygl_draw_line(tinygl_point(0,p.pos), tinygl_point(2,p.pos), 1);
            tinygl_draw_line(tinygl_point(0,p.pos+1), tinygl_point(2,p.pos-1), 1);
            break;

        default:
            break;
    }
}

void 
clear_enemy(void)
{
    //tinygl_draw_line(tinygl_point(0,p.prev_pos), tinygl_point(2,p.prev_pos), 0); 
    //tinygl_draw_line(tinygl_point(1,p.prev_pos-1), tinygl_point(1,p.prev_pos+1), 0);
    //tinygl_draw_point(tinygl_point(3,p.prev_pos-1), 0);
    //tinygl_draw_point(tinygl_point(3,p.prev_pos+1), 0);

    int i;
    for(i = 0; i < 4; i++) {
        tinygl_draw_line(tinygl_point(i, 0), tinygl_point(i,6), 0); 
    }
}

// to clear out the position and to move to another set of matrix pin.
void 
draw_ally(Player player)
{
    tinygl_draw_point(tinygl_point(4, player.prev_pos), 0);    // to clear out the previous movement of the player.
    tinygl_draw_point(tinygl_point(4, player.pos), 1);        // to move to that current position.
}


// set the player position as it moves from left to right
void 
set_player_pos(Player *player, uint8_t pos) { 
    if(pos > 1 && pos < TINYGL_HEIGHT-2) {   
        player->prev_pos = player->pos;                     // set the previous postion to the current position 
        player->pos = pos;                                  // set the current position after the previous position
    }
}

void
move_player_right(Player *player)
{
    set_player_pos(player, player->pos-1);
}

void
move_player_left(Player *player)
{
    set_player_pos(player, player->pos+1);
}

