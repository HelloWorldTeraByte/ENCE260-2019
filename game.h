#ifndef GAME_H
#define GAME_H

#define PACER_RATE 500
#define MESSAGE_RATE 10

#define BEGIN_WAIT 2500
#define MAX_STATE_TICKS 500


typedef enum {STATE_WAIT, STATE_BEGIN, STATE_PLAY, STATE_OVER} game_state_t;

typedef enum {STATE_IDLE, STATE_MV_L, STATE_MV_R,
                STATE_RHOOK, STATE_LHOOK, STATE_JAB,
                STATE_B_RHOOK, STATE_B_LHOOK, STATE_B_JAB
} player_state_t;

typedef struct {
    bool rival_ready;
    bool ally_ready;
    bool begin_init;
} game_data_t;

void init_system(void);
void init_graphics(void);

#endif
