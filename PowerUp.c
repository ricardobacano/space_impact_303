#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h> 

#include "PowerUp.h"

void activate_power_up(square *player, int *scrap_count, int *power_up_stage, char *power_up_message, float *power_up_message_timer) {
    *scrap_count = 0; 

    if (*power_up_stage >= 3) {
        snprintf(power_up_message, 50, "Todos os power-ups já foram ativados!");
        *power_up_message_timer = 2.0;  
        return; 
    }

    // ativa o proximo power-up
    switch (*power_up_stage) {
        case 0:  
            player->shield->hp = player->shield->max_hp;  
            player->shield->is_active = true;  
            snprintf(power_up_message, 50, "Power-up ativado: Extra Shield!");
            break;

        case 1:  
            snprintf(power_up_message, 50, "Power-up ativado: LASER! Use 'L'");
            break;

        // corrigir a cura do player
        case 2:  
            player->hp += 20;  
            if (player->hp > 100) player->hp = 100;  
            snprintf(power_up_message, 50, "Power-up ativado: Reparar Vida!");
            break;
    }

    *power_up_message_timer = 2.0;  
    (*power_up_stage)++;  
}

