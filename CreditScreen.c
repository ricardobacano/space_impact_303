#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
 #include <stdio.h>

#include "CreditScreen.h"

void display_credits_screen(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font) {
    ALLEGRO_BITMAP *qrcode = al_load_bitmap("imagens/qrcode.png");
    if (!qrcode) {
        fprintf(stderr, "Falha ao carregar a imagem dos créditos!\n");
        return;
    }

    al_clear_to_color(al_map_rgb(0, 0, 0));
    
    al_draw_text(font, al_map_rgb(153, 51, 153), al_get_display_width(display) / 2, al_get_display_height(display) / 2 - 60, ALLEGRO_ALIGN_CENTER, "Créditos");
    al_draw_text(font, al_map_rgb(255, 255, 255), al_get_display_width(display) / 2, al_get_display_height(display) / 2 - 30, ALLEGRO_ALIGN_CENTER, "Desenvolvido por: Ricardo");
    al_draw_text(font, al_map_rgb(255, 255, 255), al_get_display_width(display) / 2, al_get_display_height(display) / 2, ALLEGRO_ALIGN_CENTER, "Jogo feito com Allegro! x(");

    int image_width = al_get_bitmap_width(qrcode);
    float image_x = (al_get_display_width(display) - image_width) / 2;
    float image_y = al_get_display_height(display) / 2 + 60;

    al_draw_bitmap(qrcode, image_x, image_y, 0); 

    al_flip_display();

    // loop de eventos na tela
    ALLEGRO_EVENT event;
    while (1) {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
            al_destroy_bitmap(qrcode); 
            return; 
        }

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            al_destroy_bitmap(qrcode); 
            exit(0);
        }
    }
}