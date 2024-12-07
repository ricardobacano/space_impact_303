#ifndef BOSSPHASE2_H
#define BOSSPHASE2_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include "Square.h"
#include "FreezeShot.h"

typedef struct BossPhase2 {
    float x, y;                // Posição do boss
    float speed;               // Velocidade de movimento do boss
    int hp;                    // Vida do boss
    bool laser_active;         // Estado do laser
    bool freeze_active;        // Estado do tiro congelante
    float laser_timer;         // Controle do laser
    float freeze_timer;        // Controle do tiro congelante
    float laser_cooldown;      // Tempo entre ativações do laser
    float freeze_cooldown;     // Tempo entre ativações do tiro congelante
    float laser_duration;      // Duração do laser
    ALLEGRO_BITMAP *sprite;    // Sprite do boss
    FreezeShot *freeze_shots;  // Lista encadeada de tiros congelantes
} BossPhase2;

// Inicializa o boss
BossPhase2 *create_boss_phase2(float x, float y, float speed, int hp, ALLEGRO_BITMAP *sprite);

// Atualiza o estado do boss
void update_boss_phase2(BossPhase2 *boss, square *player, float delta_time, ALLEGRO_BITMAP *freeze_sprite);

// Desenha o boss e seus ataques
void draw_boss_phase2(BossPhase2 *boss, ALLEGRO_BITMAP *boss_sprite, ALLEGRO_BITMAP *laser_sprite, FreezeShot *freeze_shots, ALLEGRO_BITMAP *freeze_shot_sprite, bool debug_mode);

// Ativa o tiro congelante
void shoot_freezing_bullet(BossPhase2 *boss, square *player, ALLEGRO_BITMAP *freeze_sprite);

// Detecta e desativa o laser em caso de colisão com o jogador
bool detect_laser_collision(BossPhase2 *boss, square *player);

// Libera os recursos do boss
void destroy_boss_phase2(BossPhase2 *boss);

#endif
