#ifndef __SELECTION_SCREEN_H__
#define __SELECTION_SCREEN_H__

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>

#define NUM_OPTIONS 3

// Estrutura para representar a tela de seleção
typedef struct {
    int selected_option;
    bool is_visible;
} SelectionScreen;

// Funções para a tela de seleção
SelectionScreen* selection_screen_create();
void selection_screen_show(SelectionScreen *screen);
void selection_screen_draw(SelectionScreen *screen, ALLEGRO_FONT *font);
void selection_screen_handle_input(SelectionScreen *screen, ALLEGRO_EVENT *event);
void selection_screen_hide(SelectionScreen *screen);
void selection_screen_destroy(SelectionScreen *screen);

#endif // SELECTION_SCREEN_H