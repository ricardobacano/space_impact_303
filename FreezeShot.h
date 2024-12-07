#ifndef FREEZE_SHOT_H
#define FREEZE_SHOT_H

#include <allegro5/allegro.h>
#include "Square.h"

typedef struct FreezeShot {
    float x, y;                 // Posição do tiro
    float speed;                // Velocidade do tiro
    float width, height;        // Dimensões do tiro
    bool is_active;             // Status do tiro
    struct FreezeShot *next;    // Próximo tiro na lista
    ALLEGRO_BITMAP *sprite;     // Sprite do tiro congelante
} FreezeShot;

// Cria um novo tiro congelante
FreezeShot *create_freeze_shot(float x, float y, float speed, float width, float height, ALLEGRO_BITMAP *sprite);

// Atualiza a posição dos tiros congelantes
void update_freeze_shots(FreezeShot **shots, float delta_time, float screen_width, float screen_height);

// Desenha os tiros congelantes
void draw_freeze_shots(FreezeShot *shots, bool debug_mode);

// Verifica colisão com o jogador
bool check_freeze_shot_collision(FreezeShot *shot, square *player);

// Destroi a lista de tiros congelantes
void destroy_freeze_shots(FreezeShot **shots);

#endif // FREEZE_SHOT_H
