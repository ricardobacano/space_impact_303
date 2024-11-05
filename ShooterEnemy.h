#ifndef __SHOOTER_ENEMY__
#define __SHOOTER_ENEMY__

#include "Bullet.h"
#include "Square.h"
#include "Score.h"

typedef struct ShooterEnemy {
    float x, y;              
    unsigned char hp;         
    unsigned char timer;      
    bullet *shots;           
    struct ShooterEnemy *next; 
} ShooterEnemy;

ShooterEnemy* create_shooter_enemy(float x, float y, unsigned char hp);
void update_shooter_enemy(ShooterEnemy **head);
void move_shooter_bullets(ShooterEnemy *head, square *player);
void destroy_shooter_enemy(ShooterEnemy *enemy);
void destroy_all_shooter_enemies(ShooterEnemy *head);
unsigned char check_kill_shooter_enemies(square *player, ShooterEnemy **shooter_enemies, Score *score);

#endif
