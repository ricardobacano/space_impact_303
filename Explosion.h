#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

typedef struct Explosion {
    float x, y;                 
    float duration;             
    struct Explosion *next;     
} Explosion;

Explosion* create_explosion(float x, float y, float duration);

// função pra atualizar a animação da explosão
void update_explosions(Explosion **explosions, float delta_time);

// função realizar a animação da explosão
void draw_explosions(Explosion *explosions, ALLEGRO_BITMAP *explosion_sprite);

// destruir toda fila de explosão
void destroy_all_explosions(Explosion *explosions);

#endif
