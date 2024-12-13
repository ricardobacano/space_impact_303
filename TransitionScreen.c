#include <stdio.h>

#include "TransitionScreen.h"

void transition_init(TransitionScreen *screen, ALLEGRO_FONT *font, float initial_time) {
    screen->countdown_timer = initial_time;
    screen->is_transition_active = true;
    screen->transition_font = font;
}

bool transition_update(TransitionScreen *screen, float delta_time) {
    if (screen->is_transition_active) {
        screen->countdown_timer -= delta_time;
        if (screen->countdown_timer <= 0.0f) {
            screen->countdown_timer = 0.0f;
            screen->is_transition_active = false;
        }
    }
    return screen->is_transition_active;
}

void transition_draw(const TransitionScreen *screen) {
    if (screen->is_transition_active) {
        al_clear_to_color(al_map_rgb(0, 0, 0)); // Tela preta de transição
        char message[50];
        snprintf(message, sizeof(message), "Segunda Fase comecando em: %.0f", screen->countdown_timer);

        int text_x = X_SCREEN / 2;  // Centraliza horizontalmente na largura da tela
        int text_y = Y_SCREEN / 2;  // Centraliza verticalmente na altura da tela

        // Desenha o texto centralizado
        al_draw_text(screen->transition_font, al_map_rgb(255, 255, 255), text_x, text_y, ALLEGRO_ALIGN_CENTER, message);
    }
}


bool transition_finished(const TransitionScreen *screen) {
    return !screen->is_transition_active;
}
