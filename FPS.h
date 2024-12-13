#ifndef __FPS__
#define __FPS

#include <allegro5/allegro5.h>
#include <stdbool.h>

// função para alternar a velocidade do jogo
void toggle_game_speed(ALLEGRO_TIMER* timer, bool* is_double_speed);

#endif 