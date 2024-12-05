#include <stdio.h>
#include "FPS.h"

void toggle_game_speed(ALLEGRO_TIMER* timer, bool* is_double_speed) {
    *is_double_speed = !(*is_double_speed); 

    if (*is_double_speed) {
        // ajusta o timer para ser 2x mais rapido
        al_set_timer_speed(timer, 1.0 / 60.0); 
        printf("Jogo rodando 2x mais rápido!\n");
    } else {
        // ajusta o timer para a velocidade normal (declarada 30 fps)
        al_set_timer_speed(timer, 1.0 / 30.0);  
        printf("Jogo rodando na velocidade normal.\n");
    }
}
