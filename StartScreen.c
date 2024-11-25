#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h> 
#include <allegro5/allegro_ttf.h> 
#include <stdlib.h>
#include <stdio.h>

#include "StartScreen.h"

void display_start_screen(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font) {
    // Inicializar o addon de imagens
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
    

    // Carregar a imagem de fundo
    ALLEGRO_BITMAP *background = al_load_bitmap("imagens/fundo.png");
    if (!background) {
        fprintf(stderr, "Falha ao carregar a imagem de fundo! Verifique o caminho e a existência do arquivo.\n");
        exit(1);
    }
    // Obter dimensões da tela e da imagem
    int display_width = al_get_display_width(display);
    int display_height = al_get_display_height(display);
    int background_width = al_get_bitmap_width(background);
    int background_height = al_get_bitmap_height(background);

    // Calcular a posição para centralizar a imagem
    float x_pos = (display_width - background_width) / 2;
    float y_pos = (display_height - background_height) / 2 - 50;

    // (r,g,b) value is (45,35,40)
    al_clear_to_color(al_map_rgb(45, 35, 40));
    al_draw_bitmap(background, x_pos, y_pos, 0);

    al_draw_text(font, al_map_rgb(255, 255, 255), display_width / 2,
                 display_height / 2 + 100, ALLEGRO_ALIGN_CENTER, "Pressione ENTER para iniciar");

    // Atualizar a tela
    al_flip_display();

    // Loop para aguardar o pressionamento da tecla ENTER
    ALLEGRO_EVENT event;
    while (1) {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
            break;
        }
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            // Liberar a memória antes de sair
            al_destroy_bitmap(background);
            exit(0);
        }
    }

    // Liberar a memória da imagem de fundo
    al_destroy_bitmap(background);
}
