#ifndef SHIELDBAR_H
#define SHIELDBAR_H

#include <allegro5/allegro.h>

#define SHIELDBAR_WIDTH 100
#define SHIELDBAR_HEIGHT 10
#define SHIELDBAR_COLOR al_map_rgb(0, 0, 255) 

typedef struct {
    float x;
    float y;
    unsigned char max_shield;
    unsigned char current_shield;
} ShieldBar;

// Funções para a ShieldBar
ShieldBar* shieldbar_create(float x, float y, unsigned char max_shield);
void shieldbar_update(ShieldBar *bar, unsigned char current_shield);
void shieldbar_draw(ShieldBar *bar);
void shieldbar_destroy(ShieldBar *bar);

#endif // SHIELDBAR_H