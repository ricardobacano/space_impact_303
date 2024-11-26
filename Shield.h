#ifndef SHIELD_H
#define SHIELD_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

typedef struct {
    bool is_active;
    float duration;     // Duração do escudo em segundos
    float start_time;   // Tempo em que o escudo foi ativado
    float cooldown;     // Tempo de espera entre ativações
    float last_used_time;   // Última vez em que o escudo foi ativado
    float hp;               // HP atual do escudo
    float max_hp;           // HP máximo do escudo
} Shield;

#define SHIELD_COOLDOWN 10.0f
#define SHIELD_DURATION 15
#define SHIELD_RADIUS_OFFSET 15.0f // Aumenta o tamanho do escudo em relação ao jogador

// Funções do escudo
Shield* shield_create(float max_hp);
void shield_activate(Shield *shield);
void shield_take_damage(Shield *shield, float damage);
void shield_update(Shield *shield, float current_time);
void shield_draw(Shield *shield, float player_x, float player_y, float player_radius);
void shield_draw_bar(Shield *shield, ALLEGRO_FONT *font, float x, float y, float width, float height);
void shield_destroy(Shield *shield);

#endif // SHIELD_H
