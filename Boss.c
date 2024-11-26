#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "Boss.h"
#include "Bullet.h"

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
void boss_shoot(Boss *boss) {
    if (boss == NULL) return;

    if (boss->cooldown == 0) {
        bullet *new_bullet = bullet_create(boss->x - BOSS_WIDTH / 2, boss->y, 0, 15, NULL);
        if (new_bullet) {
            new_bullet->next = boss->shots;
            boss->shots = new_bullet;
        }
        boss->cooldown = BOSS_SHOT_COOLDOWN;  // Reinicia o cooldown após atirar
    }
}

// Atualiza o Boss (movimento e tiros)
void update_boss(Boss *boss) {
    if (boss == NULL) return;

    // Atualiza a barra de vida do Boss
    healthbar_update(boss->health_bar, boss->hp);

    // Movimento do Boss
    boss->y += boss->direction * BOSS_MOVE_SPEED;

    // Se o boss atingir os limites da tela, inverte a direção
    if (boss->y <= BOSS_HEIGHT / 2) {
        boss->direction = 1;  // Muda para baixo se atingir o topo
    } else if (boss->y >= Y_SCREEN - BOSS_HEIGHT / 2) {
        boss->direction = -1; // Muda para cima se atingir a parte inferior
    }

    // Reduz cooldown para poder atirar novamente
    if (boss->cooldown > 0) {
        boss->cooldown--;
    } else if (boss->cooldown == 0 && boss->hp > 0) {
        // O Boss atira mais rápido que inimigos comuns quando está pronto
        if (boss->cooldown % BOSS_SHOT_RATE == 0) {
            boss_shoot(boss);
        }
    }

    // Atualiza a posição dos tiros do Boss
    bullet *prev_bullet = NULL;
    bullet *current_bullet = boss->shots;

    while (current_bullet != NULL) {
        bullet_move(current_bullet);

        // Remova os tiros que saem da tela
        if (current_bullet->x < 0) {
            if (prev_bullet) {
                prev_bullet->next = current_bullet->next;
            } else {
                boss->shots = current_bullet->next;
            }
            bullet *to_destroy = current_bullet;
            current_bullet = current_bullet->next;
            bullet_destroy(to_destroy);
        } else {
            prev_bullet = current_bullet;
            current_bullet = current_bullet->next;
        }
    }
}

// Desenhar o Boss na tela
void draw_boss(Boss *boss) {
    if (!boss || !boss->sprite) return;

    // Desenha o sprite centralizado na posição do boss
    al_draw_bitmap(
        boss->sprite,
        boss->x - al_get_bitmap_width(boss->sprite) / 2,  // Centraliza em X
        boss->y - al_get_bitmap_height(boss->sprite) / 2, // Centraliza em Y
        0                                                // Sem flags adicionais
    );

    // Desenha a barra de vida do boss
    healthbar_draw(boss->health_bar);
}

// Função para destruir o Boss e liberar a memória alocada
void destroy_boss(Boss *boss) {
    if (boss == NULL) return;

    // Destruir os tiros do Boss
    bullet *current_bullet = boss->shots;
    while (current_bullet != NULL) {
        bullet *next_bullet = current_bullet->next;
        bullet_destroy(current_bullet);
        current_bullet = next_bullet;
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