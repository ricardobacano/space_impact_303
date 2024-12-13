#ifndef __START_SCREEN_H__
#define __START_SCREEN_H__

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "ControlsScreen.h"
#include "CreditScreen.h"

#define X_SCREEN 800
#define Y_SCREEN 600

// função para exibir a tela de início (menu principal)
void display_start_screen(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font);

#endif 