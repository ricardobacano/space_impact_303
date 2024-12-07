#ifndef BOSSPHASE2_H
#define BOSSPHASE2_H

#include <stdbool.h>
#include "Square.h"

typedef struct {
    float x, y;          // Posição do boss
    float speed;         // Velocidade de movimento do boss
    int hp;              // Vida do boss
    bool laser_active;   // Estado do laser
    bool freeze_active;  // Estado do tiro congelante
    float laser_timer;   // Controle do laser
    float freeze_timer;  // Controle do tiro congelante
    float laser_cooldown;    // Tempo entre ativações do laser
    float freeze_cooldown;   // Tempo entre ativações do congelante
    float laser_duration;    // Duração do laser
} BossPhase2;

// Inicializa o boss
BossPhase2 *create_boss_phase2(float x, float y, float speed, int hp);

// Atualiza o estado do boss
void update_boss_phase2(BossPhase2 *boss, square *player, float delta_time);

// Desenha o boss e seus ataques
void draw_boss_phase2(BossPhase2 *boss, ALLEGRO_BITMAP *sprite, ALLEGRO_BITMAP *laser_sprite);

// Ativa o tiro congelante
void shoot_freezing_bullet(BossPhase2 *boss, square *player);

// Detecta e desativa o laser em caso de colisão com o jogador
bool detect_laser_collision(BossPhase2 *boss, square *player);

// Libera os recursos do boss
void destroy_boss_phase2(BossPhase2 *boss);

#endif
