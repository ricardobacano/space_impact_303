#include <stdlib.h>
#include "ShooterEnemy.h"
#include "Square.h"

#define SHOOTER_COOLDOWN 60  // Inimigo atira a cada 60 frames (aproximadamente 2 segundos se o jogo for a 30 FPS)
#define BULLET_SPEED 4       // Velocidade dos projéteis disparados pelo inimigo

ShooterEnemy* create_shooter_enemy(float x, float y, unsigned char hp) {
    ShooterEnemy *new_enemy = (ShooterEnemy*) malloc(sizeof(ShooterEnemy));
    if (!new_enemy) return NULL;

    new_enemy->x = x;
    new_enemy->y = y;
    new_enemy->hp = hp;
    new_enemy->timer = 0; // Timer para controle de tiros
    new_enemy->shots = NULL;
    new_enemy->next = NULL;

    return new_enemy;
}

void update_shooter_enemy(ShooterEnemy **head) {
    ShooterEnemy *current = *head;
    while (current != NULL) {
        if (current->timer == 0) {
            // Atira
            bullet *new_bullet = bullet_create(current->x - 10, current->y, 0, 5, current->shots);
            if (new_bullet) {
                new_bullet->next = current->shots;
                current->shots = new_bullet;
            }
            current->timer = SHOOTER_COOLDOWN;  // Reinicia o cooldown do tiro
        } else {
            current->timer--;
        }
        current = current->next;
    }
}

void move_shooter_bullets(ShooterEnemy *head, square *player) {
    ShooterEnemy *current = head;
    while (current != NULL) {
        bullet *prev_bullet = NULL;
        bullet *bullet_ptr = current->shots;

        while (bullet_ptr != NULL) {
            bullet_ptr->x -= BULLET_SPEED;

            // Verifica se a bala colidiu com o jogador
            if ((bullet_ptr->x >= player->x - player->side / 2) && 
                (bullet_ptr->x <= player->x + player->side / 2) && 
                (bullet_ptr->y >= player->y - player->side / 2) && 
                (bullet_ptr->y <= player->y + player->side / 2)) {

                // Causa dano ao jogador
                player->hp -= bullet_ptr->damage;

                // Destrói a bala
                if (prev_bullet) {
                    prev_bullet->next = bullet_ptr->next;
                    bullet_destroy(bullet_ptr);
                    bullet_ptr = prev_bullet->next;
                } else {
                    current->shots = bullet_ptr->next;
                    bullet_destroy(bullet_ptr);
                    bullet_ptr = current->shots;
                }

            } else if (bullet_ptr->x < 0) {  
                if (prev_bullet) {
                    prev_bullet->next = bullet_ptr->next;
                    bullet_destroy(bullet_ptr);
                    bullet_ptr = prev_bullet->next;
                } else {
                    current->shots = bullet_ptr->next;
                    bullet_destroy(bullet_ptr);
                    bullet_ptr = current->shots;
                }
            } else {
                prev_bullet = bullet_ptr;
                bullet_ptr = bullet_ptr->next;
            }
        }

        current = current->next;
    }
}

void destroy_shooter_enemy(ShooterEnemy *enemy) {
    bullet *current_bullet = enemy->shots;
    while (current_bullet != NULL) {
        bullet *next_bullet = current_bullet->next;
        bullet_destroy(current_bullet);
        current_bullet = next_bullet;
    }
    free(enemy);
}

void destroy_all_shooter_enemies(ShooterEnemy *head) {
    ShooterEnemy *current = head;
    while (current != NULL) {
        ShooterEnemy *next = current->next;
        destroy_shooter_enemy(current);
        current = next;
    }
}

unsigned char check_kill_shooter_enemies(square *player, ShooterEnemy **shooter_enemies, Score *score) {
    ShooterEnemy *previous = NULL, *current = *shooter_enemies;

    while (current != NULL) {
        bullet *prev_bullet = NULL;
        bullet *index = player->gun->shots;

        while (index != NULL) {
            // Verificar se houve colisão entre o projétil do jogador e o inimigo atirador
            if ((index->x >= current->x - 10) && (index->x <= current->x + 10) &&
                (index->y >= current->y - 10) && (index->y <= current->y + 10)) {

                // Reduzir a vida do inimigo
                current->hp -= index->damage;

                // Se a vida do inimigo acabar, remover da lista
                if (current->hp <= 0) {
                    score_increment(score, 20); // Mais pontos para os inimigos que atiram

                    // Remover o inimigo da lista
                    if (previous != NULL) {
                        previous->next = current->next;
                    } else {
                        *shooter_enemies = current->next;
                    }

                    // Destruir o inimigo
                    ShooterEnemy *to_delete = current;
                    current = current->next;

                    destroy_shooter_enemy(to_delete);

                    // Também remover o projétil do jogador que atingiu o inimigo
                    if (prev_bullet != NULL) {
                        prev_bullet->next = index->next;
                    } else {
                        player->gun->shots = (bullet*)index->next;
                    }
                    bullet_destroy(index);

                    // Retornar indicando que o inimigo foi destruído
                    return 1;
                }

                // Se o inimigo não for destruído, remover o projétil que o atingiu
                if (prev_bullet != NULL) {
                    prev_bullet->next = index->next;
                } else {
                    player->gun->shots = (bullet*)index->next;
                }
                bullet_destroy(index);

                // Como o inimigo não foi destruído, saímos do loop de projéteis para evitar múltiplos impactos no mesmo ciclo
                return 0;
            }

            // Continuar iterando pelos projéteis
            prev_bullet = index;
            index = (bullet*)index->next;
        }

        // Continuar iterando pelos inimigos
        previous = current;
        current = current->next;
    }

    return 0; // Nenhum inimigo foi destruído
}