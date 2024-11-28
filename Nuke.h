#ifndef NUKE_H
#define NUKE_H

#include <allegro5/allegro5.h>
#include <stdbool.h>

#include "Square.h"
#include "Enemy.h"

typedef struct Nuke {
    float x, y;  // Posição da Nuke no mapa
    ALLEGRO_BITMAP *icon;  // Ícone da Nuke
    bool active;  // Se a Nuke foi pega e ativada
    struct Nuke* next;  // Ponteiro para o próximo item na lista
} Nuke;

// Funções
Nuke* create_nuke(float max_x, float max_y);  // Função para criar uma nova Nuke
void draw_nuke(Nuke *head, ALLEGRO_BITMAP *nuke_icon);  // Função para desenhar todas as Nukes
void destroy_nuke(Nuke *nuke);  // Função para destruir uma Nuke
void destroy_all_nukes(Nuke *head);  // Função para destruir todas as Nukes

bool check_collision_with_nuke(square *player, Nuke *nuke);  // Função de colisão com o jogador
void activate_nuke(Nuke *nuke, Enemy **enemies);  // Ativa a Nuke e destrói todos os inimigos visíveis

#endif  // NUKE_H
