#ifndef __BULLET__
#define __BULLET__

#define BULLET_MOVE 10

typedef struct bullet {
    unsigned short x;                   
    unsigned short y;                    
    unsigned char trajectory;          
    unsigned char damage;               
    struct bullet *next;               
} bullet;                             

bullet* bullet_create(unsigned short x, unsigned short y, unsigned char trajectory, unsigned char damage, bullet *next);

// movimentação do tiro
void bullet_move(bullet *elements);

// libera a memoria da bala 
void bullet_destroy(bullet *element);

#endif
