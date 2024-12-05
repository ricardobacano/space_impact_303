#ifndef __BULLET_ENEMY__
#define __BULLET_ENEMY__

#include <allegro5/allegro_primitives.h>
#include <stdio.h>

#define BULLET_ENEMY_MOVE 10

typedef struct bullet_enemy {
    unsigned short x;  
    unsigned short y;  
    unsigned char damage;
    struct bullet_enemy *next; 
    unsigned char type;    
    unsigned char owner;   
    int direction;         
} bullet_enemy;

bullet_enemy* bullet_enemy_create(unsigned short x, unsigned short y, unsigned char type, unsigned char damage, unsigned char owner);

// atualização do movimento do tiro do inimigo
void bullet_enemy_move(bullet_enemy *elements);

// libera a memoria da bala do inimigo
void bullet_enemy_destroy(bullet_enemy *element);

#endif
