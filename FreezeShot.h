#ifndef FREEZESHOT_H
#define FREEZESHOT_H

#include <allegro5/allegro.h>
#include "Square.h"

typedef struct FreezeShot {
    float x, y;             // Posição do tiro congelante
    float width, height;    // Largura e altura do tiro
    float speed;            // Velocidade do tiro
    bool active;            // Indica se o tiro está ativo
    ALLEGRO_BITMAP *sprite; // Sprite do tiro congelante
    struct FreezeShot *next; // Ponteiro para o próximo tiro (lista encadeada)
} FreezeShot;

// Cria um novo tiro congelante
FreezeShot *create_freeze_shot(float x, float y, float speed, float width, float height, ALLEGRO_BITMAP *sprite);

// Atualiza a posição dos tiros congelantes
void update_freeze_shots(FreezeShot **shots, float delta_time, float screen_width, float screen_height);

// Desenha os tiros congelantes
void draw_freeze_shots(FreezeShot *shots, bool debug_mode);

// Detecta colisões entre os tiros congelantes e o jogador
bool check_freeze_shot_collision(FreezeShot *shot, square *player);

// Libera a memória dos tiros congelantes
void destroy_freeze_shots(FreezeShot **shots);

#endif
