#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

typedef struct Explosion {
    float x, y;                 // Posição da explosão
    float duration;             // Duração restante para exibição
    struct Explosion *next;     // Próxima explosão na lista
} Explosion;

Explosion* create_explosion(float x, float y, float duration);
void update_explosions(Explosion **explosions, float delta_time);
void draw_explosions(Explosion *explosions, ALLEGRO_BITMAP *explosion_sprite);
void destroy_all_explosions(Explosion *explosions);

#endif
