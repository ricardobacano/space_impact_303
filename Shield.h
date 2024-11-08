#ifndef SHIELD_H
#define SHIELD_H

#include <allegro5/allegro.h>
#include "Square.h"

typedef struct {
    bool is_active;
    float duration; // Duração em segundos
    float start_time;
} Shield;

// Funções de inicialização e manipulação do escudo
Shield* shield_create();
void shield_activate(Shield *shield);
void shield_update(Shield *shield, float current_time);
void shield_draw(Shield *shield, square *player);
void shield_destroy(Shield *shield);

#endif // SHIELD_H
