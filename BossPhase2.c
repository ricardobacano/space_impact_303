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
void draw_boss_phase2(BossPhase2 *boss, bool debug_mode) {
    if (!boss || !boss->sprite) return;

    // Desenha o boss com o sprite fornecido
    al_draw_bitmap(
        boss->sprite,
        boss->x - al_get_bitmap_width(boss->sprite) / 2,
        boss->y - al_get_bitmap_height(boss->sprite) / 2,
        0
    );

    // Desenha o laser, se ativo
    if (boss->laser_active) {
        al_draw_line(
            boss->x,
            boss->y + al_get_bitmap_height(boss->sprite) / 2, // Início do laser na parte inferior do boss
            boss->x,
            600, // Até o final da tela
            al_map_rgb(255, 0, 0),
            5 // Espessura do laser
        );
    }

    // Desenha os tiros congelantes
    FreezeShot *current_freeze_shot = boss->freeze_shots;
    while (current_freeze_shot != NULL) {
        if (current_freeze_shot->sprite) {
            al_draw_bitmap(
                current_freeze_shot->sprite,
                current_freeze_shot->x - al_get_bitmap_width(current_freeze_shot->sprite) / 2,
                current_freeze_shot->y - al_get_bitmap_height(current_freeze_shot->sprite) / 2,
                0
            );
        } else {
            al_draw_filled_circle(
                current_freeze_shot->x,
                current_freeze_shot->y,
                10, // Raio do tiro congelante (caso não tenha sprite)
                al_map_rgb(0, 0, 255)
            );
        }
        current_freeze_shot = current_freeze_shot->next;
    }

    // Modo de depuração: Desenhar hitbox
    if (debug_mode) {
        // Hitbox do boss
        al_draw_rectangle(
            boss->x - al_get_bitmap_width(boss->sprite) / 2,
            boss->y - al_get_bitmap_height(boss->sprite) / 2,
            boss->x + al_get_bitmap_width(boss->sprite) / 2,
            boss->y + al_get_bitmap_height(boss->sprite) / 2,
            al_map_rgb(255, 0, 0), // Cor da hitbox
            1                      // Espessura da linha
        );

        // Hitboxes dos tiros congelantes
        FreezeShot *debug_freeze_shot = boss->freeze_shots;
        while (debug_freeze_shot != NULL) {
            al_draw_circle(
                debug_freeze_shot->x,
                debug_freeze_shot->y,
                debug_freeze_shot->radius, // Usando o raio do tiro congelante
                al_map_rgb(0, 255, 0),    // Cor verde para depuração
                1
            );
            debug_freeze_shot = debug_freeze_shot->next;
        }

        // Hitbox do laser
        if (boss->laser_active) {
            al_draw_rectangle(
                boss->x - 2.5, // Largura do laser / 2
                boss->y + al_get_bitmap_height(boss->sprite) / 2,
                boss->x + 2.5,
                600, // Fim da tela
                al_map_rgb(255, 255, 0), // Cor amarela para depuração
                1
            );
        }
    }
}

// Ativa o tiro congelante
void shoot_freezing_bullet(BossPhase2 *boss, square *player, ALLEGRO_BITMAP *freeze_sprite) {
    // Cria um novo tiro congelante na posição do boss
    FreezeShot *new_shot = create_freeze_shot(
        boss->x, 
        boss->y, 
        10.0f, // Raio da hitbox
        200.0f, // Velocidade do tiro
        freeze_sprite
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
