#ifndef __ENEMY__  
#define __ENEMY__  

#include <allegro5/allegro.h>
#include "Square.h"
#include "Score.h"

#define ENEMY_SPEED 2  

typedef struct Enemy {
    float x, y;            
    unsigned char hp;     
    struct Enemy *next;    
} Enemy;

Enemy* create_enemy(float x, float y, unsigned char hp);
void move_enemies(Enemy *head, float speed);
void update_enemies(Enemy **enemies, float speed);
void draw_enemies(Enemy *head);
void destroy_enemy(Enemy *enemy);
void destroy_all_enemies(Enemy *head);
void check_collision_with_player(square *player, Enemy **enemies);

unsigned char check_kill(square *player, Enemy **enemies, Score *score);

#endif  
