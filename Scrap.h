#ifndef __SCRAP_H__
#define __SCRAP_H__

#include <allegro5/allegro.h>

typedef struct Scrap {
    float x, y;
    unsigned char collected;
    struct Scrap *next;
} Scrap;

Scrap* create_scrap(float x, float y);

// função pra sucata se mover no mapa (direita pra esquerda)
void move_scrap(Scrap *head, float speed);

// desenha a sucata (sprite)
void draw_scrap(Scrap *head, ALLEGRO_BITMAP* scrap_sprite);

// função para a sucata não se sobresair uma na outra
unsigned char check_collision_with_scrap(float x, float y, Scrap *scrap_list);

// destroi uma sucata (colisoes ou fora dos limites da tela)
void destroy_scrap(Scrap *scrap);

// destroi a fila inteira de sucata
void destroy_all_scrap(Scrap *head);

// desenha o contador de sucata na tela
void scrap_count_draw(int scrap_count); 

#endif
 