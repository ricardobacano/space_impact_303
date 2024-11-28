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

    // Movimento padrão
    shot->y += 10; // Movimento para baixo (ajustável conforme necessário)

    // Debug: Verifique a posição do tiro após o movimento
    printf("Tiro em movimento - Posição: (%.2d, %.2d)\n", shot->x, shot->y);

    // Se a bala for do tipo dividido, ela pode ter diferentes comportamentos
    if (shot->type == BOSS_SHOT_TYPE_SPLIT) {
        shot->x += shot->direction * 5; // Movimenta para os lados
        if (shot->x <= 0 || shot->x >= 800) { // Inverte a direção se atingir as bordas da tela
            shot->direction *= -1;
        }
    }
}

// Função para desenhar as balas do Boss
void boss_shot_draw(boss_shot *shot, ALLEGRO_BITMAP *bullet_sprite) {
    if (shot == NULL || bullet_sprite == NULL) return;

    // Desenha o tiro com o sprite fornecido
    al_draw_bitmap(
        bullet_sprite, 
        shot->x - al_get_bitmap_width(bullet_sprite) / 2,  // Centraliza em X
        shot->y - al_get_bitmap_height(bullet_sprite) / 2, // Centraliza em Y
        0
    );
}

// Função para destruir uma bala do Boss
void boss_shot_destroy(boss_shot *shot) {
    if (shot != NULL) {
        // Debug: Verifique quando um tiro é destruído
        printf("Destruindo tiro na posição: (%.2d, %.2d)\n", shot->x, shot->y);

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
