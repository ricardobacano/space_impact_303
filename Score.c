#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "Score.h"

Score* score_create() {
    Score *new_score = (Score*) malloc(sizeof(Score));
    if (!new_score) return NULL;

    new_score->points = 0;  
    return new_score;
}

void score_increment(Score *score, int value) {
    if (score) {
        score->points += value;  
    }
}


void score_draw(Score *score) {
    if (score) {
        ALLEGRO_FONT* font = al_create_builtin_font();
        if (font) {
            al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 30, 0, "Score: %d", score->points);
            al_destroy_font(font);
        }
    }
}

void score_destroy(Score *score) {
    if (score) {
        free(score);  
    }
}

