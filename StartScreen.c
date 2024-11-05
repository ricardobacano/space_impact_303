#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>  
#include "StartScreen.h"

void display_start_screen(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_text(font, al_map_rgb(255, 255, 255), al_get_display_width(display) / 2,
                 al_get_display_height(display) / 2 - 50, ALLEGRO_ALIGN_CENTER, "SPACE IMPACT 303");

    al_draw_text(font, al_map_rgb(255, 255, 255), al_get_display_width(display) / 2,
                 al_get_display_height(display) / 2, ALLEGRO_ALIGN_CENTER, "Pressione ENTER para iniciar");

    al_flip_display();

    ALLEGRO_EVENT event;
    while (1) {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
            break;
        }
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            exit(0);  
        }
    }
}
