#ifndef FREEZESHOT_H
#define FREEZESHOT_H

#include <allegro5/allegro.h>
#include "Square.h"

typedef struct FreezeShot {
    float x, y;             // Posição do tiro congelante
    float radius;           // Raio da hitbox
    float speed;            // Velocidade do tiro
    bool active;            // Indica se o tiro está ativo
    ALLEGRO_BITMAP *sprite; // Sprite do tiro congelante
    struct FreezeShot *next; // Ponteiro para o próximo tiro (lista encadeada)
} FreezeShot;

// Cria um novo tiro congelante
FreezeShot *create_freeze_shot(float x, float y, float radius, float speed, ALLEGRO_BITMAP *sprite);

// Atualiza a posição dos tiros congelantes
void update_freeze_shots(FreezeShot **shots, float delta_time);

// Desenha os tiros congelantes
void draw_freeze_shots(FreezeShot *shots);

// Detecta colisões entre os tiros congelantes e o jogador
bool detect_freeze_collision(FreezeShot *shots, square *player);

// Libera a memória dos tiros congelantes
void destroy_freeze_shots(FreezeShot **shots);

#endif
