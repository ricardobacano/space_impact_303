#ifndef SHIELDBAR_H
#define SHIELDBAR_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

#include "Shield.h"

#define SHIELDBAR_WIDTH 100
#define SHIELDBAR_HEIGHT 10
#define SHIELDBAR_COLOR al_map_rgb(0, 0, 255) 

typedef struct {
    float x;
    float y;
    unsigned char max_shield;
    unsigned char current_shield;
} ShieldBar;

ShieldBar* shieldbar_create(float x, float y, unsigned char max_shield);

// atualização da barra de escudo (destruido, ativada e etc)
void shieldbar_update(ShieldBar *bar, unsigned char current_shield);

// desenho da barra de escudo
void shieldbar_draw(ShieldBar *bar);

// libera a memoria da barra de escudo
void shieldbar_destroy(ShieldBar *bar);

// timer de atualização da barra de escudo (cooldown e ativado)
void draw_shield_timer(Shield *shield, ALLEGRO_FONT *font, float x, float y, bool is_paused);

#endif 