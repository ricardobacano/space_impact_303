#ifndef SHIELD_H
#define SHIELD_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

typedef struct {
    bool is_active;
    float duration;     
    float start_time;   
    float cooldown;     
    float last_used_time;   
    float hp;               
    float max_hp;          
} Shield;

#define SHIELD_COOLDOWN 5
#define SHIELD_DURATION 5
#define SHIELD_RADIUS_OFFSET 15.0f 

Shield* shield_create(float max_hp);

// ativar o escudo 
void shield_activate(Shield *shield);

// shield tomou dano
void shield_take_damage(Shield *shield, float damage);

// atualiza o shield (se tomou dano ou esta inativo)
void shield_update(Shield *shield, float current_time);

// desenho do escudo (circulo)
void shield_draw(Shield *shield, float player_x, float player_y, float player_radius);

// desenha a barra de escudo na tela
void shield_draw_bar(Shield *shield, ALLEGRO_FONT *font, float x, float y, float width, float height);

void *shield_destroy(Shield *shield);

#endif // SHIELD_H
