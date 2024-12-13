#ifndef TRANSITION_SCREEN_H
#define TRANSITION_SCREEN_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

typedef struct {
    float countdown_timer;
    bool is_transition_active;
    ALLEGRO_FONT *transition_font;
} TransitionScreen;

#define X_SCREEN 800
#define Y_SCREEN 600

void transition_init(TransitionScreen *screen, ALLEGRO_FONT *font, float initial_time);

bool transition_update(TransitionScreen *screen, float delta_time);

void transition_draw(const TransitionScreen *screen);

bool transition_finished(const TransitionScreen *screen);

#endif 
