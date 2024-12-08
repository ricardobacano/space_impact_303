#ifndef BOSSPHASE2_H
#define BOSSPHASE2_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include "Square.h"
#include "FreezeShot.h"

typedef struct BossPhase2 {
    float x, y;               // Posição do boss
    float speed;              // Velocidade de movimento
    int hp;                   // Vida do boss
    bool laser_active;        // Indica se o laser está ativo
    float laser_timer;        // Temporizador do laser
    float laser_cooldown;     // Cooldown entre lasers
    float laser_duration;     // Duração do laser
    float freeze_timer;       // Temporizador para tiros congelantes
    float freeze_cooldown;    // Cooldown entre tiros congelantes
    ALLEGRO_BITMAP *sprite;   // Sprite do boss
    FreezeShot *freeze_shots; // Lista de tiros congelantes
} BossPhase2;

// Inicializa o boss
BossPhase2 *create_boss_phase2(float x, float y, float speed, int hp, ALLEGRO_BITMAP *sprite);

// Atualiza o estado do boss
void update_boss_phase2(BossPhase2 *boss, square *player, float delta_time, ALLEGRO_BITMAP *freeze_sprite);

// Desenha o boss e seus ataques
void draw_boss_phase2(BossPhase2 *boss, ALLEGRO_BITMAP *boss_sprite, ALLEGRO_BITMAP *laser_sprite, FreezeShot *freeze_shots, ALLEGRO_BITMAP *freeze_sprite, bool debug_mode);

// Atira um tiro congelante
void shoot_freezing_bullet(BossPhase2 *boss, square *player, ALLEGRO_BITMAP *freeze_sprite);

// Libera os recursos do boss
void destroy_boss_phase2(BossPhase2 *boss);

#endif
    