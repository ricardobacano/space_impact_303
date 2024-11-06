#include <stdio.h>
#include "FPS.h"

// Função para alternar a velocidade do jogo
void toggle_game_speed(ALLEGRO_TIMER* timer, bool* is_double_speed) {
    *is_double_speed = !(*is_double_speed);  // Alterna o estado de velocidade

    if (*is_double_speed) {
        // Ajusta o timer para ser 2x mais rápido
        al_set_timer_speed(timer, 1.0 / 60.0);  // Aumenta a taxa de FPS para 60
        printf("Jogo rodando 2x mais rápido!\n");
    } else {
        // Ajusta o timer de volta para a velocidade normal
        al_set_timer_speed(timer, 1.0 / 30.0);  // Volta a taxa de FPS para 30
        printf("Jogo rodando na velocidade normal.\n");
    }
}
