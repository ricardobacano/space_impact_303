#ifndef __BOSS_H__
#define __BOSS_H__

#include "Square.h"
#include "HealthBar.h"
#include "BossShot.h"  // Aqui você inclui apenas uma vez o arquivo BossShot.h

#define SCREEN_HEIGHT 600

#define BOSS_WIDTH 100
#define BOSS_HEIGHT 60
#define BOSS_MAX_HP 300
#define BOSS_SHOT_COOLDOWN 100  // Tempo de recarga entre os disparos do boss
#define BOSS_SHOT_RATE 10       // Número de frames entre cada disparo enquanto ele está atirando
#define BOSS_MOVE_SPEED 0.8       // Velocidade de movimento vertical do boss

// Estrutura do Boss
typedef struct Boss {
    int direction;
    float x, y;              // Posição do boss
    unsigned int hp;         // Vida do boss
    unsigned int cooldown;   // Tempo de espera entre fases de disparo
    boss_shot *shots;        // Lista encadeada de tiros do boss
    HealthBar *health_bar;   // Barra de vida do boss
    ALLEGRO_BITMAP *sprite;  // Sprite do boss
    ALLEGRO_BITMAP *bullet_sprite;  // Sprite do tiro do boss (adicionei esse campo)
} Boss;


// Funções para manipular o Boss
Boss* create_boss(float x, float y, ALLEGRO_BITMAP *sprite);
void boss_shoot(Boss *boss, ALLEGRO_BITMAP *fast_bullet_sprite, ALLEGRO_BITMAP *slow_bullet_sprite);
void update_boss(Boss *boss, square *player);
void destroy_boss(Boss *boss);         // Função para destruir o Boss e liberar a memória
void draw_boss(Boss *boss);            // Função para desenhar o Boss na tela
void check_boss_collision_with_player(square *player, Boss *boss);  // Função para verificar colisões com o jogador
int update_boss_shooting(Boss *boss, int frame_count, int shoot_pattern, ALLEGRO_BITMAP *fast_bullet_sprite, ALLEGRO_BITMAP *slow_bullet_sprite);


#endif
