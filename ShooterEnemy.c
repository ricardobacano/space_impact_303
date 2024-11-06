#include <stdlib.h>
#include "ShooterEnemy.h"
#include "BulletEnemy.h"
#include <stdio.h>

ShooterEnemy* create_shooter_enemy(float x, float y, int hp) {
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
    ShooterEnemy *previous = NULL;
    ShooterEnemy *current = *shooter_enemies;

    while (current != NULL) {
        bullet *prev_bullet = NULL;
        bullet *index = player->gun->shots;

        // Verificar cada tiro do jogador
        while (index != NULL) {
            if ((index->x >= current->x - 10) && (index->x <= current->x + 10) &&
                (index->y >= current->y - 10) && (index->y <= current->y + 10)) {


                // Reduz o HP do inimigo
                current->hp -= index->damage;


                // Se o HP do inimigo for zero ou menor
                if (current->hp <= 0) {
                    score_increment(score, 20);  // Aumenta a pontuação


                    // Remover o projétil do jogador que atingiu o inimigo
                    if (prev_bullet != NULL) {
                        prev_bullet->next = index->next;
                    } else {
                        player->gun->shots = index->next;
                    }

                    bullet *to_destroy_bullet = index;
                    index = index->next;  // Avança o ponteiro antes de destruir o projétil
                    bullet_destroy(to_destroy_bullet);

                    // Remove o inimigo da lista
                    if (previous != NULL) {
                        previous->next = current->next;
                    } else {
                        *shooter_enemies = current->next;
                    }

                    // Armazene o próximo inimigo antes de destruir o atual
                    ShooterEnemy *to_destroy_enemy = current;
                    current = current->next;

                    // Destruir o inimigo atual
                    destroy_shooter_enemy(to_destroy_enemy);

                    // Continue com o próximo inimigo
                    break;  // Saia do loop de tiros e vá para o próximo inimigo
                } else {
                    // Se o inimigo não foi destruído, apenas remove o projétil
                    if (prev_bullet != NULL) {
                        prev_bullet->next = index->next;
                    } else {
                        player->gun->shots = index->next;
                    }

                    bullet *to_destroy_bullet = index;
                    index = index->next;  // Avança o ponteiro antes de destruir o projétil
                    bullet_destroy(to_destroy_bullet);

                    // Continue verificando outros tiros no mesmo inimigo
                    continue;
                }
            }

            // Avançar para o próximo tiro
            prev_bullet = index;
            index = index->next;
        }

        // Atualizar o ponteiro previous e avançar para o próximo inimigo
        if (current != NULL) {
            previous = current;
            current = current->next;
        }
    }

    return 0;
}

void update_shooter_enemy(ShooterEnemy **head) {
    ShooterEnemy *previous = NULL;
    ShooterEnemy *current = *head;

    while (current != NULL) {
        current->x -= ENEMY_SPEED;  // Move os inimigos da direita para a esquerda

        if (current->x < 0) {  // Se o inimigo sair da tela, removê-lo
            if (previous) {
                previous->next = current->next;
            } else {
                *head = current->next;
            }
            ShooterEnemy *to_destroy = current;
            current = current->next;
            destroy_shooter_enemy(to_destroy);
        } else {
            previous = current;
            current = current->next;
        }
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

unsigned char check_collision_with_player_shooter_enemy(square *player, ShooterEnemy **shooter_enemies) {
    ShooterEnemy *previous = NULL;
    ShooterEnemy *current = *shooter_enemies;

    while (current != NULL) {
        // Cálculo da hitbox do jogador e do inimigo para verificar a colisão
        float player_left = player->x - player->side / 2;
        float player_right = player->x + player->side / 2;
        float player_top = player->y - player->side / 2;
        float player_bottom = player->y + player->side / 2;

        float enemy_left = current->x - 10;  // Considerando o inimigo como 20 pixels de largura
        float enemy_right = current->x + 10;
        float enemy_top = current->y - 10;   // Considerando o inimigo como 20 pixels de altura
        float enemy_bottom = current->y + 10;

        // Adicione os prints aqui para ver a posição do jogador e do inimigo antes da verificação de colisão

        // Verifica se as hitboxes se sobrepõem
        if (player_right >= enemy_left && player_left <= enemy_right &&
            player_bottom >= enemy_top && player_top <= enemy_bottom) {
            
            // Colisão detectada: reduz HP do jogador e destrói o inimigo
            player->hp -= 20;  // Você pode ajustar esse valor conforme necessário

            // Imprimir a nova quantidade de HP do player

            // Armazene o próximo inimigo antes de destruir o atual
            ShooterEnemy *next_enemy = current->next;

            // Remover o inimigo da lista
            if (previous != NULL) {
                previous->next = next_enemy;
            } else {
                *shooter_enemies = next_enemy;
            }

            // Destruir o inimigo atual
            destroy_shooter_enemy(current);

            // Atualizar `current` para o próximo inimigo antes de continuar
            current = next_enemy;

            // Retornar 1 para indicar que uma colisão foi tratada e o inimigo foi removido
            return 1;
        } else {
            // Se não houver colisão, apenas avançamos para o próximo inimigo
            previous = current;
            current = current->next;
        }
    }

    return 0;
}

void destroy_shooter_enemy(ShooterEnemy *enemy) {
    if (enemy == NULL) return;

    // Libere todos os projéteis disparados por esse inimigo
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