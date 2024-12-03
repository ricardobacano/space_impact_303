#ifndef __ENEMY__  
#define __ENEMY__  

#include <allegro5/allegro.h>
#include "Square.h"
#include "Score.h"
#include "Explosion.h"
#include "Shield.h"
#include "ShooterEnemy.h"

typedef struct Enemy {
    float x, y;            
    unsigned char hp;  
    float hitbox_width; 
    float hitbox_height;   
    struct Enemy *next; 
    struct Explosion *explosions;  
} Enemy;

Enemy* create_enemy(float x, float y, float hitbox_width, float hitbox_height, unsigned char hp);
void move_enemies(Enemy *head, float speed);
void update_enemies(Enemy **enemies, float speed);
void draw_enemies(Enemy *head, ALLEGRO_BITMAP* enemy_sprite, bool debug_mode);
void destroy_enemy(Enemy *enemy);
void destroy_all_enemies(Enemy **head);
void check_collision_with_player(square *player, Enemy **enemies);
unsigned char check_kill(square *player, Enemy **enemies, Score *score, Explosion **explosions);
unsigned char check_collision_with_enemies(float x, float y, Enemy *enemies);
unsigned char check_collision_between_all_enemies(float x, float y, Enemy *enemies, ShooterEnemy *shooter_enemies);

#endif  
