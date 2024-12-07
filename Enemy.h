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

// movimentaação dos inimigos 
void move_enemies(Enemy *head, float speed);

// atualiza os inimigos (destroi cso saia da tela e gera outro)
void update_enemies(Enemy **enemies, float speed);

// desenho dos inimigos
void draw_enemies(Enemy *head, ALLEGRO_BITMAP* enemy_sprite, bool debug_mode);

// limpa a memoria
void destroy_enemy(Enemy *enemy);

// destroi todos os inimigos (fila inteira)
void destroy_all_enemies(Enemy **head);

// colisão com o player
void check_collision_with_player(square *player, Enemy **enemies);

// testa se o inmigo morreu
unsigned char check_kill(square *player, Enemy **enemies, Score *score, Explosion **explosion, int *enemies_destroyed);
// testa com o propio inimigo (não sobresair um encima do outro)
unsigned char check_collision_with_enemies(float x, float y, Enemy *enemies);

// função geral pra todos os inimgos não se sobresairem
unsigned char check_collision_between_all_enemies(float x, float y, Enemy *enemies, ShooterEnemy *shooter_enemies);

#endif  
