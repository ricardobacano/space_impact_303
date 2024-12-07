#include "BossPhase2.h"
#include <stdlib.h>
#include <allegro5/allegro_primitives.h>

// Inicializa o boss
BossPhase2 *create_boss_phase2(float x, float y, float speed, int hp, ALLEGRO_BITMAP *sprite) {
    BossPhase2 *boss = (BossPhase2 *)malloc(sizeof(BossPhase2));
    if (!boss) return NULL;

    boss->x = x;
    boss->y = y;
    boss->speed = speed;
    boss->hp = hp;
    boss->laser_active = false;
    boss->freeze_active = false;
    boss->laser_timer = 0;
    boss->freeze_timer = 0;
    boss->laser_cooldown = 5.0f; // Tempo entre lasers
    boss->freeze_cooldown = 3.0f; // Tempo entre tiros congelantes
    boss->laser_duration = 2.0f; // Duração do laser

    return boss;
}

// Atualiza o estado do boss
void update_boss_phase2(BossPhase2 *boss, square *player, float delta_time, ALLEGRO_BITMAP *freeze_sprite) {
    // Movimento do boss (vai de cima para baixo)
    boss->y += boss->speed * delta_time;
    if (boss->y >= 600) boss->y = 0; // Reinicia a posição do boss no eixo Y

    // Controle do laser
    if (boss->laser_active) {
        boss->laser_timer -= delta_time;
        if (boss->laser_timer <= 0) {
            boss->laser_active = false; // Desativa o laser após a duração
        }
    } else {
        boss->laser_cooldown -= delta_time;
        if (boss->laser_cooldown <= 0) {
            boss->laser_active = true;
            boss->laser_timer = boss->laser_duration; // Reinicia o tempo do laser
            boss->laser_cooldown = 5.0f; // Tempo até o próximo laser
        }
    }

    // Controle do tiro congelante
    boss->freeze_timer -= delta_time;
    if (boss->freeze_timer <= 0) {
        shoot_freezing_bullet(boss, player, freeze_sprite); // Passa o sprite do tiro congelante
        boss->freeze_timer = boss->freeze_cooldown; // Tempo até o próximo tiro congelante
    }
}

// Desenha o boss e seus ataques
void draw_boss_phase2(BossPhase2 *boss, ALLEGRO_BITMAP *boss_sprite, ALLEGRO_BITMAP *laser_sprite, FreezeShot *freeze_shots, ALLEGRO_BITMAP *freeze_shot_sprite, bool debug_mode) {
    if (!boss || !boss_sprite) return;  // Verifica se o boss e o sprite do boss são válidos

    // Desenha o sprite do boss
    al_draw_bitmap(boss_sprite, boss->x - al_get_bitmap_width(boss_sprite) / 2, boss->y - al_get_bitmap_height(boss_sprite) / 2, 0);

    // Se o boss tiver um laser ativo
    if (boss->laser_active) {
        // Posição do laser
        float laser_x = boss->x;
        float laser_y = boss->y - al_get_bitmap_height(laser_sprite) / 2;

        // Desenha o laser
        al_draw_bitmap(laser_sprite, laser_x, laser_y, 0);
    }

    // Se houver tiros congelantes, desenha-os
    FreezeShot *current_shot = freeze_shots;
    while (current_shot != NULL) {
        if (current_shot->active) {
            // Desenha o tiro congelante
            al_draw_bitmap(freeze_shot_sprite, current_shot->x - current_shot->width / 2, current_shot->y - current_shot->height / 2, 0);
        }
        current_shot = current_shot->next;
    }

    // Se estiver no modo debug, desenha um contorno ao redor do boss
    if (debug_mode) {
        al_draw_rectangle(
            boss->x - al_get_bitmap_width(boss_sprite) / 2,
            boss->y - al_get_bitmap_height(boss_sprite) / 2,
            boss->x + al_get_bitmap_width(boss_sprite) / 2,
            boss->y + al_get_bitmap_height(boss_sprite) / 2,
            al_map_rgb(255, 0, 0),  // Cor vermelha para a borda
            2  // Espessura da linha
        );
    }
}


// Ativa o tiro congelante
void shoot_freezing_bullet(BossPhase2 *boss, square *player, ALLEGRO_BITMAP *freeze_sprite) {
    // Cria um novo tiro congelante na posição do boss
    FreezeShot *new_shot = create_freeze_shot(
        boss->x, 
        boss->y, 
        200.0f,  // Velocidade do tiro congelante
        10.0f,   // Largura do tiro congelante
        10.0f,   // Altura do tiro congelante
        freeze_sprite // Passa o sprite do tiro congelante
    );

    if (!new_shot) return;

    // Adiciona o tiro à lista de tiros congelantes
    new_shot->next = boss->freeze_shots;
    boss->freeze_shots = new_shot;
}


// Detecta e desativa o laser em caso de colisão com o jogador
bool detect_laser_collision(BossPhase2 *boss, square *player) {
    if (boss->laser_active && 
        player->x >= boss->x - 5 && player->x <= boss->x + 5) { // Verifica colisão no eixo X
        boss->laser_active = false; // Desativa o laser
        return true; // Indica que houve colisão
    }
    return false;
}

// Libera os recursos do boss
void destroy_boss_phase2(BossPhase2 *boss) {
    if (boss) {
        free(boss);
    }
}
