#ifndef SHIELD_H
#define SHIELD_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>  
#include "Square.h"

typedef struct {
    bool is_active;
    float duration;     // Duração do escudo em segundos
    float start_time;   // Tempo em que o escudo foi ativado
    float cooldown;     // Tempo de espera entre ativações
    float last_used;    // Última vez em que o escudo foi ativado
    float last_used_time;   // Último momento em que o escudo foi ativado
    float hp;               // HP do escudo
    float max_hp;           // HP máximo do escudo
} Shield;

#define SHIELD_COOLDOWN 10.0f

// Funções de inicialização e manipulação do escudo
Shield* shield_create();
void shield_activate(Shield *shield);
void shield_update(Shield *shield, float current_time);
void shield_draw(Shield *shield, square *player);
void shield_draw_bar(Shield *shield, ALLEGRO_FONT *font, float x, float y, float width, float height);
void shield_destroy(Shield *shield);
void draw_shield_timer(Shield *shield, ALLEGRO_FONT *font, float x, float y);

#endif // SHIELD_H
