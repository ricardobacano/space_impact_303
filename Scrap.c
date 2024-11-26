#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "Scrap.h"

Scrap* create_scrap(float x, float y) {
    Scrap *new_scrap = (Scrap*) malloc(sizeof(Scrap));
    if (!new_scrap) return NULL;
    new_scrap->x = x;
    new_scrap->y = y;
    new_scrap->collected = 0;
    new_scrap->next = NULL;
    return new_scrap;
}

void move_scrap(Scrap *head, float speed) {
    for (Scrap *current = head; current != NULL; current = current->next) {
        current->x -= speed; 
    }
}

void draw_scrap(Scrap *head, ALLEGRO_BITMAP* scrap_sprite) {
    for (Scrap *current = head; current != NULL; current = current->next) {
        // Desenha o sprite redimensionado na posição da sucata
        al_draw_scaled_bitmap(scrap_sprite, 
                              0, 0, 
                              al_get_bitmap_width(scrap_sprite), al_get_bitmap_height(scrap_sprite), 
                              current->x - 10, current->y - 10,  // Posição centralizada
                              25, 25,  // Tamanho desejado da sucata
                              0);
    }
}



void scrap_count_draw(int scrap_count) {
    // Desenha o contador de sucata na tela, ao lado da barra de vida
    ALLEGRO_FONT *font = al_create_builtin_font();
    char scrap_text[20];
    al_draw_text(font, al_map_rgb(200, 200, 200), 10, 50, 0, scrap_text);  // Exibe ao lado da barra de vida
    al_destroy_font(font);
}

void destroy_scrap(Scrap *scrap) {
    if (scrap) free(scrap);
}

void destroy_all_scrap(Scrap *head) {
    Scrap *current = head;
    while (current != NULL) {
        Scrap *next = current->next;
        destroy_scrap(current);
        current = next;
    }
}
