#ifndef __SHOOTER_ENEMY__
#define __SHOOTER_ENEMY__

#include <allegro5/allegro.h>
#include "BulletEnemy.h"
#include "Square.h"
#include "Score.h"
#include "Explosion.h"
#include "Shield.h"
#include "ShieldBar.h"

#define ENEMY_SPEED 2  

typedef struct ShooterEnemy {
    float x, y;
    int hp;  
    float hitbox_width;        
    float hitbox_height;
    bullet_enemy *shots;  
    struct ShooterEnemy *next;
    Explosion *explosion;
} ShooterEnemy;

ShooterEnemy* create_shooter_enemy(float x, float y, int hp);

void update_shooter_enemy(ShooterEnemy **head);

void draw_shooter_enemies(ShooterEnemy *head, ALLEGRO_BITMAP *shooter_enemy_sprite, bool debug_mode);

void move_shooter_bullets(ShooterEnemy *head, square *player);

void shooter_enemy_shoot(ShooterEnemy *enemy);

void destroy_shooter_enemy(ShooterEnemy *enemy);

void destroy_all_shooter_enemies(ShooterEnemy *head);

unsigned char check_kill_shooter_enemies(square *player, ShooterEnemy **shooter_enemies, Score *score, Explosion **explosions, int *enemies_destroyed);

unsigned char check_collision_with_shooter_enemies(float x, float y, ShooterEnemy *shooter_enemies);

unsigned char check_collision_with_player_shooter_enemy(square *player, ShooterEnemy **shooter_enemies);

unsigned char check_collision_with_shooter_enemies(float x, float y, ShooterEnemy *shooter_enemies);

#endif
