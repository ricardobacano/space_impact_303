#ifndef __BOSS_SHOT__
#define __BOSS_SHOT__

#include <allegro5/allegro.h>

#include "Square.h"

#define BOSS_SHOT_TYPE_NORMAL 0
#define BOSS_SHOT_TYPE_SPLIT 1
#define BOSS_SHOT_TYPE_SLOW 4
#define BOSS_SHOT_TYPE_FAST 3
#define BOSS_FAST_BULLET_SPEED 8.0f  
#define BOSS_SLOW_BULLET_SPEED 10.0f  

typedef struct boss_shot {
    unsigned short x;   
    unsigned short y;  
    unsigned char damage;
    unsigned char type;  
    int direction;
    bool is_freeze;        
    ALLEGRO_BITMAP *sprite;    
    struct boss_shot* next;
} boss_shot;

boss_shot* boss_shot_create(unsigned short x, unsigned short y, unsigned char type, unsigned char damage);

// movimento dos tiros
void boss_shot_move(boss_shot *shot);

// libera memoria dos tiro
void boss_shot_destroy(boss_shot *shot);

// desenho dos tiro
void boss_shot_draw(boss_shot *shot, ALLEGRO_BITMAP *bullet_sprite);

// colisão com o player
void boss_shot_handle_collision(boss_shot *shot, square *player);

#endif
