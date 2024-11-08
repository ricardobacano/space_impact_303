#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include "Shield.h"

#define SHIELD_RADIUS_OFFSET 15  // Aumenta o tamanho do escudo em relação ao jogador
#define SHIELD_COLOR al_map_rgba(135, 206, 235, 100) // Cor azul translúcida
#define SHIELD_DURATION 15.0f // Duração do escudo em segundos

Shield* shield_create() {
    Shield *new_shield = (Shield*) malloc(sizeof(Shield));
    if (!new_shield) return NULL;

    new_shield->is_active = false;
    new_shield->duration = SHIELD_DURATION;
    new_shield->start_time = 0;

    return new_shield;
}

void shield_activate(Shield *shield) {
    if (!shield->is_active) {
        shield->is_active = true;
        shield->start_time = al_get_time();
    }
}

void shield_update(Shield *shield, float current_time) {
    if (shield->is_active && (current_time - shield->start_time >= shield->duration)) {
        shield->is_active = false;
    }
}

void shield_draw(Shield *shield, square *player) {
    if (shield->is_active) {
        al_draw_circle(player->x, player->y, player->side / 2 + SHIELD_RADIUS_OFFSET, SHIELD_COLOR, 3);
    }
}

void shield_destroy(Shield *shield) {
    free(shield);
}
