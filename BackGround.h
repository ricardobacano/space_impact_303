#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <allegro5/allegro5.h>

// inicia o background
void background_init(const char* filepath);

// atualiza a posição do background
void background_update(float speed);

// desenha o background
void background_draw();

// libera memoria do background
void background_destroy();

#endif 
