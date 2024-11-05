#include <stdlib.h>
#include <allegro5/allegro_primitives.h>
#include "Enemy.h"
#include "ShooterEnemy.h"

Enemy* create_enemy(float x, float y, unsigned char hp) {
    Enemy *new_enemy = (Enemy*) malloc(sizeof(Enemy));
    if (!new_enemy) return NULL;
    new_enemy->x = x;
    new_enemy->y = y;
    new_enemy->hp = hp;
    new_enemy->next = NULL;
    return new_enemy;
}

void move_enemies(Enemy *head, float speed) {
    for (Enemy *current = head; current != NULL; current = current->next) {
        current->x -= speed;  
    }
}

void update_enemies(Enemy **enemies, float speed) {
    Enemy *previous = NULL, *current = *enemies;

    while (current != NULL) {
        current->x -= speed;  // Atualiza a posição do inimigo

        // Verifica se o inimigo saiu da tela
        if (current->x < 0) {
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
        } else {
            previous = current;
            current = current->next;
        }
    }
}

void draw_enemies(Enemy *head) {
    for (Enemy *current = head; current != NULL; current = current->next) {
        al_draw_filled_rectangle(current->x - 10, current->y - 10, current->x + 10, current->y + 10, al_map_rgb(0, 255, 0));
    }
}

void destroy_enemy(Enemy *enemy) {
    free(enemy);
}

void destroy_all_enemies(Enemy *head) {
    Enemy *current = head;
    while (current != NULL) {
        Enemy *next = current->next;
        destroy_enemy(current);
        current = next;
    }
}

void check_collision_with_player(square *player, Enemy **enemies) {
    Enemy *previous = NULL;
    Enemy *current = *enemies;

    while (current != NULL) {
        if ((current->x >= player->x - player->side / 2) && (current->x <= player->x + player->side / 2) &&
            (current->y >= player->y - player->side / 2) && (current->y <= player->y + player->side / 2)) {

            player->hp -= 10;  

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
        } else {
            previous = current;
            current = current->next;
        }
    }
}

unsigned char check_kill(square *player, Enemy **enemies, Score *score) {
    Enemy *previous = NULL, *current = *enemies;

    while (current != NULL) {
        bullet *prev_bullet = NULL;
        for (bullet *index = player->gun->shots; index != NULL; index = (bullet*)index->next) {
            if ((index->x >= current->x - 10) && (index->x <= current->x + 10) &&
                (index->y >= current->y - 10) && (index->y <= current->y + 10)) {
                current->hp -= index->damage;
                if (current->hp <= 0) {
                    score_increment(score, 10);
                    
                    if (previous) {
                        previous->next = current->next;
                    } else {
                        *enemies = current->next;
                    }
                    destroy_enemy(current);
                    return 1;
                }

                if (prev_bullet) {
                    prev_bullet->next = index->next;
                } else {
                    player->gun->shots = (bullet*)index->next;
                }
                bullet_destroy(index);
                return 0;
            }
            prev_bullet = index;
        }
        previous = current;
        current = current->next;
    }
    return 0;
}

unsigned char check_collision_with_enemies(float x, float y, Enemy *enemies) {
    Enemy *current_enemy = enemies;
    while (current_enemy != NULL) {
        if ((x >= current_enemy->x - 20 && x <= current_enemy->x + 20) &&
            (y >= current_enemy->y - 20 && y <= current_enemy->y + 20)) {
            return 1; 
        }
        current_enemy = current_enemy->next;
    }
    return 0;  
}