#include <stdlib.h>
#include "Explosion.h"

Explosion* create_explosion(float x, float y, float duration) {
    Explosion *new_explosion = (Explosion*) malloc(sizeof(Explosion));
    if (!new_explosion) return NULL;

    new_explosion->x = x;
    new_explosion->y = y;
    new_explosion->duration = duration;
    new_explosion->next = NULL;

    return new_explosion;
}

void update_explosions(Explosion **explosions, float delta_time) {
    Explosion *current = *explosions;
    Explosion *previous = NULL;

    while (current != NULL) {
        current->duration -= delta_time;
        if (current->duration <= 0) {
            Explosion *to_destroy = current;
            if (previous) {
                previous->next = current->next;
            } else {
                *explosions = current->next;
            }
            current = current->next;
            free(to_destroy);  // Libera antes de avançar
            continue;
        }
        previous = current;
        current = current->next;
    }
}


void draw_explosions(Explosion *explosions, ALLEGRO_BITMAP *explosion_sprite) {
    for (Explosion *current = explosions; current != NULL; current = current->next) {
        al_draw_bitmap(
            explosion_sprite,
            current->x - al_get_bitmap_width(explosion_sprite) / 2,
            current->y - al_get_bitmap_height(explosion_sprite) / 2,
            0
        );
    }
}

void destroy_all_explosions(Explosion *explosions) {
    Explosion *current = explosions;
    while (current != NULL) {
        Explosion *next = current->next;
        free(current);
        current = next;
    }
}
