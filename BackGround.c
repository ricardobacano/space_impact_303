#include "BackGround.h"
#include <allegro5/allegro_image.h>
#include <stdio.h>

static ALLEGRO_BITMAP* background = NULL;
static float background_x = 0;

void background_init(const char* filepath) {
    background = al_load_bitmap(filepath);
    if (!background) {
        fprintf(stderr, "Falha ao carregar a imagem de fundo: %s\n", filepath);
    }
    background_x = 0; 
}

void background_update(float speed) {
    background_x -= speed;  
    if (background_x <= -al_get_bitmap_width(background)) {
        background_x = 0; 
    }
}

void background_draw() {
    if (background) {
        al_draw_bitmap(background, background_x, 0, 0);
        al_draw_bitmap(background, background_x + al_get_bitmap_width(background), 0, 0);
    }
}

void background_destroy() {
    if (background) {
        al_destroy_bitmap(background);
        background = NULL;
    }
}
