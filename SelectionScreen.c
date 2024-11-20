#include "SelectionScreen.h"
#include <stdlib.h>

SelectionScreen* selection_screen_create() {
    SelectionScreen *new_screen = (SelectionScreen*) malloc(sizeof(SelectionScreen));
    if (!new_screen) return NULL;

    new_screen->selected_option = 0;  // Inicialmente, a primeira opção é selecionada
    new_screen->is_visible = false;   // Tela começa invisível
    new_screen->background_bitmap = al_create_bitmap(800, 600); // Cria um bitmap preto
    if (!new_screen->background_bitmap) {
        free(new_screen);
        return NULL;
    }

    al_set_target_bitmap(new_screen->background_bitmap);
    al_clear_to_color(al_map_rgb(0, 0, 0));  // Preenche o bitmap com preto
    al_set_target_backbuffer(al_get_current_display()); // Volta ao backbuffer

    return new_screen;
}


void selection_screen_show(SelectionScreen *screen) {
    screen->is_visible = true;
}

void selection_screen_draw(SelectionScreen *screen, ALLEGRO_FONT *font) {
    if (!screen->is_visible) return;

    // Preenche todo o fundo com preto
    al_draw_filled_rectangle(0, 0, X_SCREEN, Y_SCREEN, al_map_rgb(0, 0, 0));

    // (Opcional) Desenha uma sobreposição translúcida para destacar o menu
    al_draw_filled_rectangle(100, 100, 700, 500, al_map_rgba(0, 0, 0, 150));

    // Opções do menu
    const char *options[NUM_OPTIONS] = {"Option 1: Rocket Missil", "Option 2: Extra Shield", "Option 3: Repair Health"};

    // Desenha cada opção, destacando a selecionada
    for (int i = 0; i < NUM_OPTIONS; i++) {
        ALLEGRO_COLOR color = (i == screen->selected_option) ? al_map_rgb(255, 255, 0) : al_map_rgb(255, 255, 255);
        al_draw_text(font, color, 150, 200 + i * 50, 0, options[i]);
    }
}

void selection_screen_handle_input(SelectionScreen *screen, ALLEGRO_EVENT *event) {
    if (!screen->is_visible) return;

    if (event->type == ALLEGRO_EVENT_KEY_DOWN) {
        if (event->keyboard.keycode == ALLEGRO_KEY_UP) {
            screen->selected_option = (screen->selected_option - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        } else if (event->keyboard.keycode == ALLEGRO_KEY_DOWN) {
            screen->selected_option = (screen->selected_option + 1) % NUM_OPTIONS;
        } else if (event->keyboard.keycode == ALLEGRO_KEY_ENTER) {
            printf("Option %d selected\n", screen->selected_option + 1);
            selection_screen_hide(screen);  // Fecha a tela após a seleção
        }
    }
}

void selection_screen_hide(SelectionScreen *screen) {
    screen->is_visible = false;
}

void selection_screen_destroy(SelectionScreen *screen) {
    if (screen->background_bitmap) {
        al_destroy_bitmap(screen->background_bitmap);
    }
    free(screen);
}
