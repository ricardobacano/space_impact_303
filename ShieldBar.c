#include <stdlib.h>
#include <stdio.h>
#include "ShieldBar.h"

#define HEALTHBAR_HEIGHT 10.0

ShieldBar* shieldbar_create(float x, float y, unsigned char max_shield) {
    ShieldBar *new_bar = (ShieldBar*) malloc(sizeof(ShieldBar));
    if (!new_bar) return NULL;

    new_bar->x = x;
    new_bar->y = y + HEALTHBAR_HEIGHT + 5; 
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

    // contorno da barra 
    al_draw_rectangle(bar->x, bar->y, bar->x + SHIELDBAR_WIDTH, bar->y + SHIELDBAR_HEIGHT, al_map_rgb(255, 255, 255), 1);

    // barra preenchida
    al_draw_filled_rectangle(bar->x, bar->y, bar->x + current_width, bar->y + SHIELDBAR_HEIGHT, SHIELDBAR_COLOR);
}

void shieldbar_destroy(ShieldBar *bar) {
    free(bar);
}

void draw_shield_timer(Shield *shield, ALLEGRO_FONT *font, float x, float y, bool is_paused) {
    if (!shield || !font) return;

    if (is_paused) return;

    if (shield->is_active) {
        float shield_time_remaining = shield->duration - (al_get_time() - shield->start_time);

        if (shield_time_remaining > 0) {
            char timer_text[50];
            snprintf(timer_text, 50, "Escudo ativo: %.1fs", shield_time_remaining);
            al_draw_text(font, al_map_rgb(135, 206, 235), x, y, ALLEGRO_ALIGN_LEFT, timer_text);
        }
    } else {
        float cooldown_time_remaining = shield->cooldown - (al_get_time() - shield->last_used_time);

        if (cooldown_time_remaining > 0) {
            char cooldown_text[50];
            snprintf(cooldown_text, 50, "Cooldown: %.1fs", cooldown_time_remaining);
            al_draw_text(font, al_map_rgb(255, 0, 0), x, y, ALLEGRO_ALIGN_LEFT, cooldown_text);
        }
    }
}


