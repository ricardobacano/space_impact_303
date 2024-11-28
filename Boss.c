#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include "Boss.h"

Boss* create_boss(float x, float y, ALLEGRO_BITMAP *sprite) {
    Boss *new_boss = (Boss*) malloc(sizeof(Boss));
    if (!new_boss) return NULL;

    new_boss->x = x;
    new_boss->y = y;
    new_boss->hp = BOSS_MAX_HP;
    new_boss->cooldown = BOSS_SHOT_COOLDOWN;
    new_boss->shots = NULL;
    new_boss->direction = 1;  // Inicialmente, o boss vai se mover para baixo

    // Cria a barra de vida fixa no topo da tela
    new_boss->health_bar = healthbar_create(X_SCREEN - 130, 20, new_boss->hp);  // Coloca a barra no topo e define o valor máximo de HP
    new_boss->sprite = sprite; 

    return new_boss;
}

// Função para o Boss atirar
void boss_shoot(Boss *boss, ALLEGRO_BITMAP *fast_bullet_sprite, ALLEGRO_BITMAP *slow_bullet_sprite) {
    if (boss == NULL) return;

    if (boss->cooldown == 0) {
        // Debug: Verifique se a função boss_shoot foi chamada
        printf("Boss atirando! Posição: (%.2f, %.2f)\n", boss->x, boss->y);

        // Tipo de tiro rápido
        boss_shot *new_fast_bullet = boss_shot_create(boss->x - BOSS_WIDTH / 2, boss->y, BOSS_SHOT_TYPE_FAST, 10);
        if (new_fast_bullet) {
            new_fast_bullet->next = boss->shots;
            boss->shots = new_fast_bullet;
            new_fast_bullet->sprite = fast_bullet_sprite;  // Atribui o sprite do tiro rápido

            // Debug: Verifique se o tiro rápido foi criado
            printf("Criado tiro rápido! Posição: (%.2d, %.2d)\n", new_fast_bullet->x, new_fast_bullet->y);
        }

        // Tipo de tiro lento
        boss_shot *new_slow_bullet = boss_shot_create(boss->x - BOSS_WIDTH / 2, boss->y, BOSS_SHOT_TYPE_SLOW, 5);
        if (new_slow_bullet) {
            new_slow_bullet->next = boss->shots;
            boss->shots = new_slow_bullet;
            new_slow_bullet->sprite = slow_bullet_sprite;  // Atribui o sprite do tiro lento

            // Debug: Verifique se o tiro lento foi criado
            printf("Criado tiro lento! Posição: (%.2d, %.2d)\n", new_slow_bullet->x, new_slow_bullet->y);
        }

        boss->cooldown = BOSS_SHOT_COOLDOWN;  // Reinicia o cooldown após atirar
    }
}


int update_boss_shooting(Boss *boss, int frame_count, int shoot_pattern, ALLEGRO_BITMAP *fast_bullet_sprite, ALLEGRO_BITMAP *slow_bullet_sprite) {
    // Define o intervalo para alternar entre os padrões de disparo
    if (frame_count % 150 == 0) {
        // Alterna entre os padrões de disparo (1 = lento, 2 = rápido)
        if (shoot_pattern == 1) {
            shoot_pattern = 2;  // Muda para o padrão de disparo rápido
            printf("Novo padrão de disparo: Rapido (2)\n");
        } else {
            shoot_pattern = 1;  // Muda para o padrão de disparo lento
            printf("Novo padrão de disparo: Lento (1)\n");
        }
    }

    // Dependendo do padrão de disparo, chama a função de disparo do Boss
    if (shoot_pattern == 1) {
        // Disparo mais lento
        if (frame_count % BOSS_SHOT_RATE == 0) {
            boss_shoot(boss, slow_bullet_sprite, fast_bullet_sprite);  // Passa o sprite do tiro lento e rápido
        }
    } else if (shoot_pattern == 2) {
        // Disparo mais rápido
        if (frame_count % (BOSS_SHOT_RATE / 2) == 0) {
            boss_shoot(boss, fast_bullet_sprite, slow_bullet_sprite);  // Passa o sprite do tiro rápido
        }
    }

    return shoot_pattern;  // Retorna o novo valor de shoot_pattern
}


void update_boss(Boss *boss, square *player) {
    if (boss == NULL || player == NULL) return;

    // Calcular a diferença entre a posição do boss e a do jogador
    float delta_x = player->x - boss->x;
    float delta_y = player->y - boss->y;

    // Calcular a distância entre o boss e o jogador
    float distance = sqrt(delta_x * delta_x + delta_y * delta_y);

    // Evitar que o boss se mova a uma velocidade superior à distância
    if (distance != 0) {
        // Normalizar a direção
        float direction_x = delta_x / distance;
        float direction_y = delta_y / distance;

        // Mover o boss na direção do jogador
        boss->x += direction_x * BOSS_MOVE_SPEED;
        boss->y += direction_y * BOSS_MOVE_SPEED;
    }

    // Verifica se o boss está dentro dos limites da tela
    if (boss->x < 0) boss->x = 0;
    if (boss->x > X_SCREEN) boss->x = X_SCREEN;
    if (boss->y < 0) boss->y = 0;
    if (boss->y > Y_SCREEN) boss->y = Y_SCREEN;
}

// Desenhar o Boss na tela
void draw_boss(Boss *boss) {
    if (!boss || !boss->sprite) return;

    // Desenha o sprite do Boss
    al_draw_bitmap(
        boss->sprite,
        boss->x - al_get_bitmap_width(boss->sprite) / 2,  // Centraliza em X
        boss->y - al_get_bitmap_height(boss->sprite) / 2, // Centraliza em Y
        0
    );

    // Desenha a barra de vida do Boss
    healthbar_draw(boss->health_bar);

    // Desenha os tiros do Boss
    boss_shot *current_shot = boss->shots;
    while (current_shot != NULL) {
        // Debug: Verifique a posição do tiro antes de desenhá-lo
        printf("Desenhando tiro em posição: (%.2d, %.2d)\n", current_shot->x, current_shot->y);

        boss_shot_draw(current_shot, current_shot->sprite);  // Usa o sprite correto para cada tipo de tiro
        current_shot = current_shot->next;  // Avança para o próximo tiro
    }
}


// Função para destruir o Boss e liberar a memória alocada
void destroy_boss(Boss *boss) {
    if (boss == NULL) return;

    // Destruir os tiros do Boss
    boss_shot *current_shot = boss->shots;
    while (current_shot != NULL) {
        boss_shot *next_bullet = current_shot->next;
        boss_shot_destroy(current_shot);
        current_shot = next_bullet;
    }

    // Destruir a barra de vida
    healthbar_destroy(boss->health_bar);

    free(boss);
}

// Função para verificar colisão do Boss com o jogador
void check_boss_collision_with_player(square *player, Boss *boss) {
    if (boss == NULL || player == NULL) return;

    // Verifica se há colisão do jogador com o boss
    if ((player->x + player->side / 2 >= boss->x - BOSS_WIDTH / 2) &&
        (player->x - player->side / 2 <= boss->x + BOSS_WIDTH / 2) &&
        (player->y + player->side / 2 >= boss->y - BOSS_HEIGHT / 2) &&
        (player->y - player->side / 2 <= boss->y + BOSS_HEIGHT / 2)) {

        printf("Jogador colidiu com o Boss!\n");
        player->hp -= 30;  // Reduz a vida do jogador em 30 ao colidir com o Boss
    }
}
