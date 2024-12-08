#include "BossPhase2.h"
#include <stdlib.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

// Inicializa o boss
BossPhase2 *create_boss_phase2(float x, float y, float speed, int hp, ALLEGRO_BITMAP *sprite) {
    BossPhase2 *boss = (BossPhase2 *)malloc(sizeof(BossPhase2));
    if (!boss) return NULL;

    boss->x = x;
    boss->y = y;
    boss->speed = speed;
    boss->hp = hp;
    boss->sprite = sprite;
    boss->laser_active = false;
    boss->freeze_timer = 0.0f;
    boss->laser_timer = 0.0f;
    boss->laser_cooldown = 5.0f; // Tempo entre lasers
    boss->freeze_cooldown = 2.0f; // Tempo entre tiros congelantes
    boss->laser_duration = 2.0f; // Duração do laser
    boss->freeze_shots = NULL;

    return boss;
}
// Atualiza o estado do boss
void update_boss_phase2(BossPhase2 *boss, square *player, float delta_time, ALLEGRO_BITMAP *freeze_sprite) {
    if (!boss) return;

    // Controle do laser
    if (boss->laser_active) {
        boss->laser_timer -= delta_time;
        boss->speed = 0; // Boss para de se mover enquanto o laser está ativo
        if (boss->laser_timer <= 0) {
            boss->laser_active = false; // Desativa o laser
            boss->laser_cooldown = 5.0f; // Reinicia o cooldown do laser
        }
    } else {
        boss->laser_cooldown -= delta_time;
        if (boss->laser_cooldown <= 0) {
            boss->laser_active = true;
            boss->laser_timer = boss->laser_duration; // Ativa o laser
        }
    }

    // Controle dos tiros congelantes
    boss->freeze_timer -= delta_time;
    if (boss->freeze_timer <= 0) {
        shoot_freezing_bullet(boss, player, freeze_sprite); // Dispara um tiro congelante
        boss->freeze_timer = boss->freeze_cooldown; // Reinicia o cooldown dos tiros congelantes
    }

    // Movimento do boss (somente se o laser não estiver ativo)
    if (!boss->laser_active) {
        boss->y += boss->speed * delta_time;
        if (boss->y > 600) boss->y = 0; // Reinicia a posição do boss no eixo Y
    }
}
// Desenha o boss e seus ataques
void draw_boss_phase2(BossPhase2 *boss, ALLEGRO_BITMAP *boss_sprite, ALLEGRO_BITMAP *laser_sprite, FreezeShot *freeze_shots, ALLEGRO_BITMAP *freeze_sprite, bool debug_mode) {
    if (!boss || !boss_sprite) return;

    // Desenha o sprite do boss
    al_draw_bitmap(boss_sprite, boss->x - al_get_bitmap_width(boss_sprite) / 2, boss->y - al_get_bitmap_height(boss_sprite) / 2, 0);

    // Desenha o laser, se ativo
    if (boss->laser_active) {
        float laser_x = boss->x - al_get_bitmap_width(boss_sprite) / 2; // Sai do lado esquerdo do boss
        float laser_y = boss->y;
        al_draw_line(laser_x, 0, laser_x, 600, al_map_rgb(255, 0, 0), 5); // Desenha o laser como uma linha
    }

    // Desenha os tiros congelantes
    draw_freeze_shots(boss->freeze_shots, debug_mode);

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

void shoot_freezing_bullet(BossPhase2 *boss, square *player, ALLEGRO_BITMAP *freeze_sprite) {
    if (!boss) return;

    FreezeShot *new_shot = create_freeze_shot(boss->x, boss->y, 200.0f, 20.0f, 20.0f, freeze_sprite); // Configurações do tiro congelante
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
        destroy_freeze_shots(&boss->freeze_shots); // Libera os tiros congelantes
        free(boss);
    }
}
