#include "Shield.h"
#include <stdlib.h>
#include <stdio.h>

#define SHIELD_COLOR al_map_rgba(135, 206, 235, 100) // Cor azul translúcida

Shield* shield_create(float max_hp) {
    Shield *new_shield = (Shield*) malloc(sizeof(Shield));
    if (new_shield == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o escudo.\n");
        return NULL;
    }

    new_shield->is_active = false;
    new_shield->duration = SHIELD_DURATION;
    new_shield->cooldown = SHIELD_COOLDOWN;
    new_shield->start_time = 0;
    new_shield->last_used_time = -SHIELD_COOLDOWN; // primeira ativação 
    new_shield->hp = max_hp;
    new_shield->max_hp = max_hp;

    return new_shield;
}

void shield_activate(Shield *shield) {
    float current_time = al_get_time();

    if (!shield->is_active && (current_time - shield->last_used_time >= shield->cooldown)) {
        shield->is_active = true;
        shield->start_time = current_time;
        shield->last_used_time = current_time;
        shield->hp = shield->max_hp; // reseta o HP do escudo toda vez que ativado pos cooldown
    } 
}


void shield_take_damage(Shield *shield, float damage) {
    if (shield->is_active) {
        shield->hp -= damage;

        if (shield->hp <= 0) {
            shield->is_active = false;
            shield->hp = 0;
        }
    }
}

void shield_update(Shield *shield, float current_time) {
    if (shield->is_active && (current_time - shield->start_time >= shield->duration)) {
        shield->is_active = false;
        shield->last_used_time = current_time; 
    }
}

void shield_draw(Shield *shield, float player_x, float player_y, float player_radius) {
    if (shield->is_active) {
        float shield_radius = player_radius + SHIELD_RADIUS_OFFSET;
        al_draw_circle(player_x, player_y, shield_radius, SHIELD_COLOR, 3); // desenho do circulo
    }
}

void shield_draw_bar(Shield *shield, ALLEGRO_FONT *font, float x, float y, float width, float height) {
    
    float shield_fill = (shield->hp > 0) ? (shield->hp / shield->max_hp) * width : 0;

    al_draw_rectangle(x, y, x + width, y + height, al_map_rgb(255, 255, 255), 2);
    al_draw_filled_rectangle(x, y, x + shield_fill, y + height, al_map_rgb(135, 206, 250));

    if (shield->is_active) {
        float shield_time_remaining = shield->duration - (al_get_time() - shield->start_time);
        if (shield_time_remaining > 0) {
            char shield_timer_text[50];
            al_draw_text(font, al_map_rgb(255, 255, 255), x + width + 10, y, 0, shield_timer_text);
        }
    }
}

void *shield_destroy(Shield *shield) {
    
    if (shield == NULL)
    {
        fprintf(stderr, "Tentativa de destruir um escudo nulo.\n");
        return 0;
    }
    free(shield);
    return 0;
}
