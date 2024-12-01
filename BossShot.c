#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include "BossShot.h"

// Função para criar um tiro do Boss
boss_shot* boss_shot_create(unsigned short x, unsigned short y, unsigned char type, unsigned char damage) {
    boss_shot* new_shot = (boss_shot*) malloc(sizeof(boss_shot));
    if (!new_shot) return NULL;

    new_shot->x = x;
    new_shot->y = y;
    new_shot->damage = damage;
    new_shot->type = type;
    new_shot->direction = 1;  // Inicializa direção para o tipo dividido
    new_shot->next = NULL;
    
    return new_shot;
}

// Função para mover a bala do Boss
void boss_shot_move(boss_shot *shot) {
    if (shot == NULL) return;

    // Define as velocidades com base no tipo do tiro
    float speed_x = (shot->type == BOSS_SHOT_TYPE_FAST) ? BOSS_FAST_BULLET_SPEED : 0;
    float speed_y = (shot->type == BOSS_SHOT_TYPE_SPLIT) ? BOSS_SLOW_BULLET_SPEED : BOSS_FAST_BULLET_SPEED;

    // Movimento para frente
    if (shot->type == BOSS_SHOT_TYPE_FAST) {
        shot->x -= speed_x; // Move para a esquerda
    }

    // Movimento de bumerangue
    if (shot->type == BOSS_SHOT_TYPE_SPLIT) {
        shot->x -= speed_x; // Move para a esquerda
        shot->y += shot->direction * speed_y; // Movimento em Y

        // Verifica o alcance e inverte direção para efeito de bumerangue
        if (shot->y <= 0 || shot->y >= Y_SCREEN) {
            shot->direction *= -1; // Inverte direção em Y
        }

        // Movimento de retorno em X
        if (shot->x <= X_SCREEN / 2) { 
            shot->x += speed_x; // Volta para a direita
        }
    }
}



// Função para desenhar as balas do Boss
void boss_shot_draw(boss_shot *shot, ALLEGRO_BITMAP *bullet_sprite) {
    if (!shot || !bullet_sprite) {
        printf("Erro: Tiro ou sprite do tiro nulo.\n");
        return;
    }

    // Desenhar a bala na posição correta
    al_draw_bitmap(
        bullet_sprite,
        shot->x - al_get_bitmap_width(bullet_sprite) / 2,
        shot->y - al_get_bitmap_height(bullet_sprite) / 2,
        0
    );
}

// Função para destruir uma bala do Boss
void boss_shot_destroy(boss_shot *shot) {
    if (shot != NULL) {
        // Debug: Verifique quando um tiro é destruído

        free(shot);
    }
}

// Função para verificar colisão da bala com o jogador
void boss_shot_handle_collision(boss_shot *shot, square *player) {
    if (!shot || !player) return;

    // Verifique se o tiro do Boss colide com o jogador
    if ((shot->x >= player->x - player->side / 2) && 
        (shot->x <= player->x + player->side / 2) && 
        (shot->y >= player->y - player->side / 2) && 
        (shot->y <= player->y + player->side / 2)) {

        // Lógica para dano ao jogador
        player->hp -= shot->damage;

        // Remover o tiro
        shot->next = NULL;  // Desconectar ou destruir o tiro se necessário
    }
}
