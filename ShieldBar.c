#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include "ShieldBar.h"

#define SHIELDBAR_WIDTH 100
#define SHIELDBAR_HEIGHT 10
#define SHIELDBAR_COLOR al_map_rgb(0, 0, 255) // Cor azul para a barra de escudo

ShieldBar* shieldbar_create(float x, float y, unsigned char max_shield) {
    ShieldBar *new_bar = (ShieldBar*) malloc(sizeof(ShieldBar));
    if (!new_bar) return NULL;

    new_bar->x = x;
    new_bar->y = y;
    new_bar->max_shield = max_shield;
    new_bar->current_shield = max_shield;

    return new_bar;
}

void shieldbar_update(ShieldBar *bar, unsigned char current_shield) {
    if (current_shield <= bar->max_shield) {
        bar->current_shield = current_shield;
    }
}

void shieldbar_draw(ShieldBar *bar) {
    float shield_ratio = (float) bar->current_shield / bar->max_shield;
    float current_width = SHIELDBAR_WIDTH * shield_ratio;

    // Desenha o contorno da barra de escudo
    al_draw_rectangle(bar->x, bar->y, bar->x + SHIELDBAR_WIDTH, bar->y + SHIELDBAR_HEIGHT, al_map_rgb(255, 255, 255), 1);

    // Desenha a parte preenchida da barra de escudo
    al_draw_filled_rectangle(bar->x, bar->y, bar->x + current_width, bar->y + SHIELDBAR_HEIGHT, SHIELDBAR_COLOR);
}

void shieldbar_destroy(ShieldBar *bar) {
    free(bar);
}
