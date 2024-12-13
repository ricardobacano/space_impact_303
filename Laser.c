#include <stdio.h>
#include <stdlib.h>

#include "Laser.h"
#include "Enemy.h"  
#include "Boss.h"    

// Cria o laser
Laser* laser_create(float initial_x, float width, float speed, unsigned int damage) {
    Laser *laser = (Laser *)malloc(sizeof(Laser));
    if (!laser) return NULL;

    laser->x = initial_x;
    laser->width = width;
    laser->speed = speed;
    laser->is_active = false;  
    laser->damage = damage;

    return laser;
}

void laser_update(Laser *laser) {
    if (!laser || !laser->is_active) return;

    laser->x += laser->speed;

    // desativa se o laser sai da terra
    if (laser->x > X_SCREEN) {
        laser->is_active = false;
        laser->x = 0; 
    }
}

// Desenha o laser
void laser_draw(Laser *laser) {
    if (!laser || !laser->is_active) return;

    al_draw_filled_rectangle(
        laser->x, 0,                
        laser->x + laser->width, Y_SCREEN,  
        al_map_rgb(255, 0, 0)       
    );
}

void laser_check_collision_with_enemies(Laser *laser, Enemy **enemies,  Score *score) {
    if (!laser || !laser->is_active || !enemies) return;

    Enemy *current = *enemies;
    Enemy *previous = NULL;

    while (current != NULL) {
        if (laser->x + laser->width >= current->x - current->hitbox_width / 2 &&
            laser->x <= current->x + current->hitbox_width / 2) {

            current->hp -= laser->damage;

            // remove o inimigo se sua vida acabar
            if (current->hp <= 0) {
                score_increment(score, 10);
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

void laser_check_collision_with_boss(Laser *laser, Boss *boss) {
    if (!laser || !laser->is_active || !boss) return;

    if (laser->x + laser->width >= boss->x - BOSS_WIDTH / 2 &&
        laser->x <= boss->x + BOSS_WIDTH / 2) {

        boss->hp -= laser->damage;
        printf("Boss atingido! HP restante: %d\n", boss->hp);

        healthbar_update(boss->health_bar, boss->hp);
    }
}

void activate_laser(Laser *laser) {
    if (!laser) return;

    laser->is_active = true;
    laser->x = 0; 
}

void laser_check_collision_with_shooter_enemies(Laser *laser, ShooterEnemy **shooter_enemies, Score *score) {
    if (!laser || !shooter_enemies) return;

    ShooterEnemy *current = *shooter_enemies;
    ShooterEnemy *prev = NULL;

    while (current != NULL) {

        float shooter_left = current->x - current->hitbox_width / 2;
        float shooter_right = current->x + current->hitbox_width / 2;

        if (laser->x <= shooter_right && (laser->x + laser->width) >= shooter_left) {
            
            score_increment(score, 10);
            if (prev == NULL) {
                *shooter_enemies = current->next;
            } else {
                prev->next = current->next;
            }

            ShooterEnemy *to_remove = current;
            current = current->next;

            destroy_shooter_enemy(to_remove); 
        } else {
            prev = current;
            current = current->next;
        }
    }
}
void draw_laser_cooldown_bar(float cooldown_timer, float max_cooldown, float screen_width, float screen_height) {
    float bar_width = screen_width * 0.3;  // largura, 30% da tela
    float bar_height = 20; 
    float bar_x = (screen_width - bar_width) / 2;  
    float bar_y = screen_height - bar_height - 10; 

    float fill_width = bar_width * ((max_cooldown - cooldown_timer) / max_cooldown);

    // fundo da barra
    al_draw_filled_rectangle(bar_x, bar_y, bar_x + bar_width, bar_y + bar_height, al_map_rgb(50, 50, 50));

    // preenchimento
    al_draw_filled_rectangle(bar_x, bar_y, bar_x + fill_width, bar_y + bar_height, al_map_rgb(255, 0, 0));

    // contorno
    al_draw_rectangle(bar_x, bar_y, bar_x + bar_width, bar_y + bar_height, al_map_rgb(255, 255, 255), 2);
}
