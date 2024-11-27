#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h> 

#include "PowerUp.h"

void activate_power_up(square *player, int *scrap_count, int *power_up_stage, char *power_up_message, float *power_up_message_timer) {
    *scrap_count = 0;  // Reseta o contador de sucatas

    // Verifica se ainda há power-ups disponíveis
    if (*power_up_stage >= 3) {
        snprintf(power_up_message, 50, "Todos os power-ups já foram ativados!");
        *power_up_message_timer = 2.0;  // Exibe a mensagem por 2 segundos
        return;  // Não faz mais nada se todos os power-ups foram usados
    }

    // Ativa o próximo power-up
    switch (*power_up_stage) {
        case 0:  // Power-up: Extra Shield
            player->shield->hp = player->shield->max_hp;  // Restaura o HP do escudo
            player->shield->is_active = true;  // Ativa o escudo
            player->shield->start_time = al_get_time();  // Reinicia a duração
            snprintf(power_up_message, 50, "Power-up ativado: Extra Shield!");
            break;

        case 1:  // Power-up: Rocket Missile
            // Aqui você pode implementar o aumento do dano ou outro efeito
            // player->gun->damage += 5;
            snprintf(power_up_message, 50, "Power-up ativado: Rocket Missile!");
            break;

        case 2:  // Power-up: Repair Health
            player->hp += 20;  // Cura o jogador
            if (player->hp > 100) player->hp = 100;  // Limita o HP a 100
            snprintf(power_up_message, 50, "Power-up ativado: Repair Health!");
            break;
    }

    *power_up_message_timer = 2.0;  // Exibe a mensagem por 2 segundos
    (*power_up_stage)++;  // Incrementa para o próximo estágio do power-up
}

