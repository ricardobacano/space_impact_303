#ifndef __START_SCREEN_H__
#define __START_SCREEN_H__

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#define X_SCREEN 800
#define Y_SCREEN 600

// Função que exibe a tela de início (menu principal)
void display_start_screen(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font);

#endif // __START_SCREEN_H__