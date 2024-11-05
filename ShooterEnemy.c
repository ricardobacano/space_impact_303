#include <stdlib.h>
#include "ShooterEnemy.h"
#include "BulletEnemy.h"

ShooterEnemy* create_shooter_enemy(float x, float y, unsigned char hp) {
    ShooterEnemy *new_enemy = (ShooterEnemy*) malloc(sizeof(ShooterEnemy));
    if (!new_enemy) return NULL;
    new_enemy->x = x;
    new_enemy->y = y;
    new_enemy->hp = hp;
    new_enemy->shots = NULL;
    new_enemy->next = NULL;
    return new_enemy;
}

void shooter_enemy_shoot(ShooterEnemy *enemy) {
    if (enemy == NULL) return;

    bullet_enemy *new_bullet = bullet_enemy_create(enemy->x - 10, enemy->y);  // Cria uma nova bala
    if (new_bullet) {
        new_bullet->next = enemy->shots;  // Adiciona à lista de tiros
        enemy->shots = new_bullet;
    }
}

unsigned char check_kill_shooter_enemies(square *player, ShooterEnemy **shooter_enemies, Score *score) {
    ShooterEnemy *previous = NULL, *current = *shooter_enemies;

    while (current != NULL) {
        bullet *prev_bullet = NULL;
        for (bullet *index = player->gun->shots; index != NULL; index = (bullet*)index->next) {
            if ((index->x >= current->x - 10) && (index->x <= current->x + 10) &&
                (index->y >= current->y - 10) && (index->y <= current->y + 10)) {
                current->hp -= index->damage;
                if (current->hp <= 0) {
                    score_increment(score, 20);  // Aumenta a pontuação por matar um ShooterEnemy

                    // Remove o ShooterEnemy da lista
                    if (previous) {
                        previous->next = current->next;
                    } else {
                        *shooter_enemies = current->next;
                    }
                    destroy_shooter_enemy(current);
                    return 1;
                }

                // Remove o projétil do jogador após o impacto
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

void update_shooter_enemy(ShooterEnemy **head) {
    ShooterEnemy *current = *head;

    while (current != NULL) {
        current = current->next;
    }
}

unsigned char check_collision_with_shooter_enemies(float x, float y, ShooterEnemy *head) {
    for (ShooterEnemy *current = head; current != NULL; current = current->next) {
        float distance_x = current->x - x;
        float distance_y = current->y - y;
        float distance_squared = distance_x * distance_x + distance_y * distance_y;

        if (distance_squared < 400) {
            return 1; 
        }
    }
    return 0;  
}

void move_shooter_bullets(ShooterEnemy *head, square *player) {
    for (ShooterEnemy *current = head; current != NULL; current = current->next) {
        bullet_enemy *bullet_ptr = current->shots;
        bullet_enemy *prev_bullet = NULL;

        while (bullet_ptr != NULL) {
            bullet_enemy_move(bullet_ptr);

            if ((bullet_ptr->x >= player->x - player->side / 2) && 
                (bullet_ptr->x <= player->x + player->side / 2) && 
                (bullet_ptr->y >= player->y - player->side / 2) &&
                (bullet_ptr->y <= player->y + player->side / 2)) {
                
                player->hp -= 10;  

                if (prev_bullet) {
                    prev_bullet->next = bullet_ptr->next;
                } else {
                    current->shots = bullet_ptr->next;
                }
                bullet_enemy *temp = bullet_ptr;
                bullet_ptr = bullet_ptr->next;
                bullet_enemy_destroy(temp);
                continue;
            }

            if (bullet_ptr->x < 0) {
                if (prev_bullet) {
                    prev_bullet->next = bullet_ptr->next;
                } else {
                    current->shots = bullet_ptr->next;
                }
                bullet_enemy *temp = bullet_ptr;
                bullet_ptr = bullet_ptr->next;
                bullet_enemy_destroy(temp);
                continue;
            }

            prev_bullet = bullet_ptr;
            bullet_ptr = bullet_ptr->next;
        }
    }
}

void destroy_shooter_enemy(ShooterEnemy *enemy) {
    if (enemy == NULL) return;

    bullet_enemy *current_bullet = enemy->shots;
    while (current_bullet != NULL) {
        bullet_enemy *next_bullet = current_bullet->next;
        bullet_enemy_destroy(current_bullet);
        current_bullet = next_bullet;
    }
    free(enemy);
}

void destroy_all_shooter_enemies(ShooterEnemy *head) {
    while (head != NULL) {
        ShooterEnemy *next_enemy = head->next;
        destroy_shooter_enemy(head);
        head = next_enemy;
    }
}