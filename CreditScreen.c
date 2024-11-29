#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <stdio.h>

void display_credits_screen(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    
    al_draw_text(font, al_map_rgb(255, 255, 255), al_get_display_width(display) / 2, al_get_display_height(display) / 2 - 30, ALLEGRO_ALIGN_CENTER, "Créditos");
    al_draw_text(font, al_map_rgb(255, 255, 255), al_get_display_width(display) / 2, al_get_display_height(display) / 2, ALLEGRO_ALIGN_CENTER, "Desenvolvido por: [Seu Nome]");
    al_draw_text(font, al_map_rgb(255, 255, 255), al_get_display_width(display) / 2, al_get_display_height(display) / 2 + 30, ALLEGRO_ALIGN_CENTER, "Jogo feito com Allegro!");

    al_flip_display();

    ALLEGRO_EVENT event;
    while (1) {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
            return; // Retorna ao menu
        }

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            exit(0);
        }
    }
}
