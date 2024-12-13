#include <stdlib.h>
#include <allegro5/allegro_primitives.h> 
#include "HealthBar.h"

HealthBar* healthbar_create(float x, float y, unsigned char max_hp) {
    HealthBar *new_bar = (HealthBar*) malloc(sizeof(HealthBar));
    if (!new_bar) return NULL;

    new_bar->x = x;
    new_bar->y = y;
    new_bar->max_hp = max_hp;
    new_bar->current_hp = max_hp;

    return new_bar;
}

void healthbar_update(HealthBar *bar, unsigned char current_hp) {
    if (current_hp <= bar->max_hp) {
        bar->current_hp = current_hp;
    }
}

void healthbar_draw(HealthBar *bar) {
    float hp_ratio = (float) bar->current_hp / bar->max_hp;
    float current_width = HEALTHBAR_WIDTH * hp_ratio;

    // Desenha o contorno da barra de vida
    al_draw_rectangle(bar->x, bar->y, bar->x + HEALTHBAR_WIDTH, bar->y + HEALTHBAR_HEIGHT, al_map_rgb(255, 255, 255), 1);

    // Desenha a parte preenchida da barra de vida
    al_draw_filled_rectangle(bar->x, bar->y, bar->x + current_width, bar->y + HEALTHBAR_HEIGHT, HEALTHBAR_COLOR);
}

void healthbar_destroy(HealthBar *bar) {
    free(bar);
}
