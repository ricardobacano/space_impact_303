#ifndef __BULLET_ENEMY__
#define __BULLET_ENEMY__

#define BULLET_ENEMY_MOVE 6

typedef struct bullet_enemy {
    unsigned short x;  
    unsigned short y;  
    unsigned char damage;
    struct bullet_enemy *next;  
} bullet_enemy;

bullet_enemy* bullet_enemy_create(unsigned short x, unsigned short y);
void bullet_enemy_move(bullet_enemy *elements);
void bullet_enemy_destroy(bullet_enemy *element);

#endif
