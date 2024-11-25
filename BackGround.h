#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <allegro5/allegro5.h>

// Função para inicializar o fundo
void background_init(const char* filepath);

// Função para atualizar a posição do fundo
void background_update(float speed);

// Função para desenhar o fundo na tela
void background_draw();

// Função para destruir os recursos do fundo
void background_destroy();

#endif // BACKGROUND_H
