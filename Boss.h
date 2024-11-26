#ifndef __BOSS_H__
#define __BOSS_H__

#include "Bullet.h"
#include "HealthBar.h"
#include "Square.h"

#define BOSS_WIDTH 100
#define BOSS_HEIGHT 60
#define BOSS_MAX_HP 300
#define BOSS_SHOT_COOLDOWN 100  // Tempo de recarga entre os disparos do boss
#define BOSS_SHOT_RATE 10       // Número de frames entre cada disparo enquanto ele está atirando
#define BOSS_MOVE_SPEED 2       // Velocidade de movimento vertical do boss

typedef struct Boss {
    int direction;
    float x, y;              // Posição do boss
    unsigned int hp;         // Vida do boss
    unsigned int cooldown;   // Tempo de espera entre fases de disparo
    bullet *shots;           // Lista encadeada de tiros do boss
    HealthBar *health_bar;   // Barra de vida do boss
    ALLEGRO_BITMAP *sprite;      // Adicione um ponteiro para o sprite
} Boss;

// Funções para mcreate_bossanipular o Boss
Boss* create_boss(float x, float y, ALLEGRO_BITMAP *sprite);
void boss_shoot(Boss *boss);
void update_boss(Boss *boss);
void destroy_boss(Boss *boss);
void draw_boss(Boss *boss);
void check_boss_collision_with_player(square *player, Boss *boss);

#endif
