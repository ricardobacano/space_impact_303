#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro_primitives.h>
#include "Nuke.h"

// Função para criar a Nuke
Nuke* create_nuke(float max_x, float max_y) {
    Nuke *new_nuke = (Nuke*) malloc(sizeof(Nuke));
    if (!new_nuke) return NULL;

    // Define uma posição aleatória para a Nuke dentro dos limites da tela
    new_nuke->x = rand() % (int)(max_x - 50) + 50;  // Posição aleatória no eixo X
    new_nuke->y = rand() % (int)(max_y - 50) + 50;  // Posição aleatória no eixo Y

    new_nuke->active = false;  // A Nuke começa inativa
    new_nuke->next = NULL;     // Inicializa o ponteiro `next` como NULL
    return new_nuke;
}

// Função para desenhar todas as Nukes na tela
void draw_nuke(Nuke *nuke, ALLEGRO_BITMAP *nuke_sprite) {
    if (nuke->active) {  // Certifica-se de desenhar apenas as Nukes ativas
        // Desenha o sprite da Nuke na posição definida por nuke->x, nuke->y
        al_draw_bitmap(nuke_sprite, nuke->x, nuke->y, 0);
    }
}
// Função para destruir uma Nuke
void destroy_nuke(Nuke *nuke) {
    if (nuke) {
        if (nuke->icon) al_destroy_bitmap(nuke->icon);  // Destrói o ícone da Nuke
        free(nuke);  // Libera a memória da Nuke
    }
}

// Função para destruir todas as Nukes
void destroy_all_nukes(Nuke *head) {
    Nuke *current = head;
    while (current != NULL) {
        Nuke *next = current->next;
        destroy_nuke(current);
        current = next;
    }
}

// Função de colisão entre o jogador e a Nuke
bool check_collision_with_nuke(square *player, Nuke *nuke) {
    float player_left = player->x - player->side / 2;
    float player_right = player->x + player->side / 2;
    float player_top = player->y - player->side / 2;
    float player_bottom = player->y + player->side / 2;

    float nuke_left = nuke->x - 25;  // Ajuste dependendo do tamanho do ícone da Nuke
    float nuke_right = nuke->x + 25;
    float nuke_top = nuke->y - 25;
    float nuke_bottom = nuke->y + 25;

    // Verifica a colisão
    return player_right >= nuke_left && player_left <= nuke_right &&
           player_bottom >= nuke_top && player_top <= nuke_bottom;
}

// Função para ativar a Nuke e destruir todos os inimigos visíveis
void activate_nuke(Nuke *nuke, Enemy **enemies) {
    if (nuke->active) {
        // Chama uma função para destruir todos os inimigos na tela
        destroy_all_enemies(enemies);  // A função `destroy_all_enemies` precisa ser implementada

        printf("Nuke ativada! Todos os inimigos foram destruídos.\n");
    }
}
