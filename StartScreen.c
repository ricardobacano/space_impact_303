#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h> 
#include <allegro5/allegro_ttf.h>
#include <stdlib.h>
#include <stdio.h>

#include "ControlsScreen.h"
#include "CreditScreen.h"
#include "StartScreen.h"

typedef enum {
    MENU_START,
    MENU_CONTROLS,
    MENU_CREDITS,
    MENU_QUIT
} MenuOption;

void display_start_screen(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font) {
    if (!al_init_image_addon()) {
        fprintf(stderr, "Falha ao inicializar addon de imagem!\n");
        exit(1);
    }

    if (!al_init_font_addon()) {
        fprintf(stderr, "Falha ao inicializar addon de fonte!\n");
        exit(1);
    }
    if (!al_init_ttf_addon()) {
        fprintf(stderr, "Falha ao inicializar addon de fonte TTF!\n");
        exit(1);
    }

    ALLEGRO_BITMAP *background = al_load_bitmap("imagens/fundo.png");
    if (!background) {
        fprintf(stderr, "Falha ao carregar a imagem de fundo! Verifique o caminho e a existência do arquivo.\n");
        exit(1);
    }

    // pegar dimensões na tela 
    int display_width = al_get_display_width(display);
    int display_height = al_get_display_height(display);
    int background_width = al_get_bitmap_width(background);
    int background_height = al_get_bitmap_height(background);

    // centralizar a imagem 
    float x_pos = (display_width - background_width) / 2;
    float y_pos = (display_height - background_height) / 2 - 50;

    al_clear_to_color(al_map_rgb(29, 36, 99));
    al_draw_bitmap(background, x_pos, y_pos, 0);

    MenuOption selected_option = MENU_START;

    // loop para navegação no menu
    ALLEGRO_EVENT event;
    while (1) {
        al_clear_to_color(al_map_rgb(29, 36, 99));  
        al_draw_bitmap(background, x_pos, y_pos, 0); 

        al_draw_text(font, selected_option == MENU_START ? al_map_rgb(255, 0, 0) : al_map_rgb(255, 255, 255), display_width / 2, display_height / 2 + 90, ALLEGRO_ALIGN_CENTER, "Iniciar");
        al_draw_text(font, selected_option == MENU_CONTROLS ? al_map_rgb(255, 0, 0) : al_map_rgb(255, 255, 255), display_width / 2, display_height / 2 + 120, ALLEGRO_ALIGN_CENTER, "Controles");
        al_draw_text(font, selected_option == MENU_CREDITS ? al_map_rgb(255, 0, 0) : al_map_rgb(255, 255, 255), display_width / 2, display_height / 2 + 150, ALLEGRO_ALIGN_CENTER, "Créditos");

        al_flip_display(); // Atualiza o display com as alterações

        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
                selected_option = (selected_option == MENU_START) ? MENU_CREDITS : selected_option - 1;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                selected_option = (selected_option == MENU_CREDITS) ? MENU_START : selected_option + 1;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                if (selected_option == MENU_START) {
                    break; 
                } else if (selected_option == MENU_CONTROLS) {
                    display_controls_screen(display, queue, font);
                } else if (selected_option == MENU_CREDITS) {
                    display_credits_screen(display, queue, font);
                }
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                exit(0); 
            }
        }

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            exit(0); 
        }
    }


    al_destroy_bitmap(background);
}
