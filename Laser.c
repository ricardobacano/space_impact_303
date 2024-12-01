#include <stdio.h>
#include <stdlib.h>

#include "Laser.h"
#include "Enemy.h"  // Para a estrutura e manipulação dos inimigos
#include "Boss.h"    // Para a estrutura e manipulação do Boss


// Cria o laser
Laser* laser_create(float initial_x, float width, float speed, unsigned int damage) {
    Laser *laser = (Laser *)malloc(sizeof(Laser));
    if (!laser) return NULL;

    laser->x = initial_x;
    laser->width = width;
    laser->speed = speed;
    laser->is_active = false;  // Inicia desativado
    laser->damage = damage;

    return laser;
}

// Atualiza a posição do laser
void laser_update(Laser *laser) {
    if (!laser || !laser->is_active) return;

    // Move o laser para a direita
    laser->x += laser->speed;

    // Desativa o laser se sair da tela
    if (laser->x > X_SCREEN) {
        laser->is_active = false;
        laser->x = 0;  // Reinicia na posição inicial
    }
}

// Desenha o laser
void laser_draw(Laser *laser) {
    if (!laser || !laser->is_active) return;

    al_draw_filled_rectangle(
        laser->x, 0,                // Posição inicial (esquerda)
        laser->x + laser->width, Y_SCREEN,  // Posição final (direita)
        al_map_rgb(255, 0, 0)       // Cor vermelha
    );
}

// Verifica colisão do laser com inimigos
void laser_check_collision_with_enemies(Laser *laser, Enemy **enemies) {
    if (!laser || !laser->is_active || !enemies) return;

    Enemy *current = *enemies;
    Enemy *previous = NULL;

    while (current != NULL) {
        // Verifica se o laser colide com o inimigo
        if (laser->x + laser->width >= current->x - current->hitbox_width / 2 &&
            laser->x <= current->x + current->hitbox_width / 2) {

            // Aplica dano ao inimigo
            current->hp -= laser->damage;
            printf("Inimigo atingido! HP restante: %d\n", current->hp);

            // Remove o inimigo se sua vida acabar
            if (current->hp <= 0) {
                if (previous) {
                    previous->next = current->next;
                } else {
                    *enemies = current->next;
                }
                destroy_enemy(current);
                if (previous) {
                    current = previous->next;
                } else {
                    current = *enemies;
                }
                continue;
            }
        }

        previous = current;
        current = current->next;
    }
}

// Verifica colisão do laser com o Boss
void laser_check_collision_with_boss(Laser *laser, Boss *boss) {
    if (!laser || !laser->is_active || !boss) return;

    // Verifica se o laser colide com o Boss
    if (laser->x + laser->width >= boss->x - BOSS_WIDTH / 2 &&
        laser->x <= boss->x + BOSS_WIDTH / 2) {

        // Aplica dano ao Boss
        boss->hp -= laser->damage;
        printf("Boss atingido! HP restante: %d\n", boss->hp);

        // Atualiza a barra de vida do Boss
        healthbar_update(boss->health_bar, boss->hp);
    }
}

// Ativa o laser
void activate_laser(Laser *laser) {
    if (!laser) return;

    laser->is_active = true;
    laser->x = 0;  // Começa do lado esquerdo da tela
}

void draw_laser_cooldown_bar(float cooldown_timer, float max_cooldown, float screen_width, float screen_height) {
    float bar_width = screen_width * 0.3;  // Largura da barra (80% da tela)
    float bar_height = 20;  // Altura da barra
    float bar_x = (screen_width - bar_width) / 2;  // Centraliza horizontalmente
    float bar_y = screen_height - bar_height - 10;  // Fica no final da tela

    float fill_width = bar_width * ((max_cooldown - cooldown_timer) / max_cooldown);

    // Fundo da barra
    al_draw_filled_rectangle(bar_x, bar_y, bar_x + bar_width, bar_y + bar_height, al_map_rgb(50, 50, 50));

    // Parte preenchida
    al_draw_filled_rectangle(bar_x, bar_y, bar_x + fill_width, bar_y + bar_height, al_map_rgb(255, 0, 0));

    // Contorno da barra
    al_draw_rectangle(bar_x, bar_y, bar_x + bar_width, bar_y + bar_height, al_map_rgb(255, 255, 255), 2);
}