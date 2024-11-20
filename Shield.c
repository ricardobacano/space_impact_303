#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>  
#include <stdlib.h>
#include <stdio.h>
#include "Shield.h"

#define SHIELD_RADIUS_OFFSET 15  // Aumenta o tamanho do escudo em relação ao jogador
#define SHIELD_COLOR al_map_rgba(135, 206, 235, 100) // Cor azul translúcida
#define SHIELD_DURATION 5.0f

Shield* shield_create() {
    Shield *new_shield = (Shield*) malloc(sizeof(Shield));
    if (!new_shield) return NULL;

    new_shield->is_active = false;
    new_shield->start_time = 0;
    new_shield->last_used_time = -SHIELD_COOLDOWN; // Permite ativação inicial
    new_shield->hp = 100.0f;       // HP inicial do escudo
    new_shield->max_hp = 100.0f;   // HP máximo do escudo

    return new_shield;
}

void shield_take_damage(Shield *shield, float damage) {
    if (shield->is_active) {
        shield->hp -= damage;
        if (shield->hp <= 0) {
            shield->hp = 0;
            shield->is_active = false; // Desativa o escudo se o HP chegar a 0
            printf("Escudo destruído!\n");
        }
    }
}


void shield_activate(Shield *shield) {
    float current_time = al_get_time();

    // Verifica se o cooldown já passou
    if (!shield->is_active && (current_time - shield->last_used_time >= SHIELD_COOLDOWN)) {
        shield->is_active = true;
        shield->start_time = current_time;      // Marca o início do escudo
        shield->last_used_time = current_time;  // Atualiza o tempo de uso
        printf("Escudo ativado!\n");
    } else if (shield->is_active) {
        printf("Escudo já está ativo!\n");
    } else {
        printf("Escudo em cooldown! %.2f segundos restantes.\n", 
               SHIELD_COOLDOWN - (current_time - shield->last_used_time));
    }
}


void shield_update(Shield *shield, float current_time) {
    // Verifica se o escudo está ativo e se o tempo de duração já passou
    if (shield->is_active && (current_time - shield->start_time >= SHIELD_DURATION)) {
        shield->is_active = false;  // Desativa o escudo
        printf("Escudo desativado! Agora em cooldown.\n");
    }
}

void shield_draw_bar(Shield *shield, ALLEGRO_FONT *font, float x, float y, float width, float height) {
    if (shield == NULL || font == NULL) return;

    // Calcula o preenchimento da barra baseado no HP do escudo
    float shield_fill = (shield->hp > 0) ? (shield->hp / shield->max_hp) * width : 0;

    // Desenha a borda da barra
    al_draw_rectangle(x, y, x + width, y + height, al_map_rgb(255, 255, 255), 2);

    // Desenha o preenchimento da barra se o escudo tiver HP
    if (shield_fill > 0) {
        al_draw_filled_rectangle(x, y, x + shield_fill, y + height, al_map_rgb(135, 206, 250)); // Azul
    }

    // Exibe o tempo restante do escudo ao lado da barra
    if (shield->is_active) {
        float shield_time_remaining = shield->duration - (al_get_time() - shield->start_time);

        if (shield_time_remaining > 0) {
            char shield_timer_text[50];
            snprintf(shield_timer_text, 50, "%.1fs", shield_time_remaining);

            // Desenha o contador ao lado direito da barra
            al_draw_text(font, al_map_rgb(255, 255, 255), x + width + 10, y, 0, shield_timer_text);
        }
    }
}


void draw_shield_timer(Shield *shield, ALLEGRO_FONT *font, float x, float y) {
    if (shield->is_active) {
        float shield_time_remaining = shield->duration - (al_get_time() - shield->start_time);

        if (shield_time_remaining > 0) {
            char shield_timer_text[50];
            snprintf(shield_timer_text, 50, "Escudo ativo: %.1fs", shield_time_remaining);
            al_draw_text(font, al_map_rgb(255, 255, 255), x, y, 0, shield_timer_text);
            printf("Tempo restante do escudo: %.1fs\n", shield_time_remaining); // Debug
        }
    }
}



void shield_draw(Shield *shield, square *player) {
    if (shield->is_active) {
        float shield_radius = (player->side / 2) + SHIELD_RADIUS_OFFSET; // Calcula o raio do escudo
        al_draw_circle(player->x, player->y, shield_radius, SHIELD_COLOR, 3); // Desenha o círculo
    }
}



void shield_destroy(Shield *shield) {
    free(shield);
}
