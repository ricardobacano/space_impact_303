#include "BossPhase2.h"
#include <stdlib.h>
#include <allegro5/allegro_primitives.h>

// Inicializa o boss
BossPhase2 *create_boss_phase2(float x, float y, float speed, int hp) {
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
void update_boss_phase2(BossPhase2 *boss, square *player, float delta_time) {
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
        shoot_freezing_bullet(boss, player);
        boss->freeze_timer = boss->freeze_cooldown; // Tempo até o próximo tiro congelante
    }
}

// Desenha o boss e seus ataques
void draw_boss_phase2(BossPhase2 *boss, ALLEGRO_BITMAP *sprite, ALLEGRO_BITMAP *laser_sprite) {
    // Desenha o boss
    al_draw_bitmap(sprite, boss->x, boss->y, 0);

    // Desenha o laser, se ativo
    if (boss->laser_active) {
        al_draw_line(boss->x, boss->y, boss->x, 600, al_map_rgb(255, 0, 0), 5); // Linha representando o laser
    }
}

// Ativa o tiro congelante
void shoot_freezing_bullet(BossPhase2 *boss, square *player) {
    // Detecta se o jogador está no caminho do tiro congelante
    if (abs(player->x - boss->x) < 50) { // Verifica se o jogador está na linha do tiro
        player->control->right = 0;
        player->control->left = 0;
        player->control->up = 0;
        player->control->down = 0;
        boss->freeze_active = true; // Marca que o jogador está congelado
    }
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
