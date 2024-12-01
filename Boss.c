#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include "Boss.h"

Boss* create_boss(float x, float y, ALLEGRO_BITMAP *sprite) {
    Boss *new_boss = (Boss*) malloc(sizeof(Boss));
    if (!new_boss) return NULL;

    new_boss->x = x;
    new_boss->y = y;
    new_boss->hp = BOSS_MAX_HP;
    new_boss->cooldown = BOSS_SHOT_COOLDOWN;
    new_boss->shots = NULL;
    new_boss->direction = 1;  // Inicialmente, o boss vai se mover para baixo

    // Cria a barra de vida fixa no topo da tela
    new_boss->health_bar = healthbar_create(X_SCREEN - 130, 20, new_boss->hp);  // Coloca a barra no topo e define o valor máximo de HP
    new_boss->sprite = sprite; 

    return new_boss;
}

// Função para o Boss atirar
void boss_shoot(Boss *boss, ALLEGRO_BITMAP *fast_bullet_sprite, ALLEGRO_BITMAP *slow_bullet_sprite) {
    if (boss == NULL) return;

    static int alternate_shot = 0; // Variável estática para alternar entre os tiros

    if (boss->cooldown == 0) {
        if (alternate_shot == 0) {
            // Tiro rápido
            boss_shot *new_fast_bullet = boss_shot_create(boss->x - BOSS_WIDTH / 2, boss->y, BOSS_SHOT_TYPE_FAST, 20);
            if (new_fast_bullet) {
                new_fast_bullet->next = boss->shots;
                boss->shots = new_fast_bullet;
                new_fast_bullet->sprite = fast_bullet_sprite;
            }
        } else {
            // Tiro bumerangue
            boss_shot *new_boomerang_bullet = boss_shot_create(boss->x - BOSS_WIDTH / 2, boss->y, BOSS_SHOT_TYPE_SPLIT, 20);
            if (new_boomerang_bullet) {
                new_boomerang_bullet->next = boss->shots;
                boss->shots = new_boomerang_bullet;
                new_boomerang_bullet->sprite = slow_bullet_sprite;
            }
        }

        // Alterna para o próximo tipo de tiro
        alternate_shot = 1 - alternate_shot;

        boss->cooldown = BOSS_SHOT_COOLDOWN;  // Reinicia o cooldown após atirar
    }
}


int update_boss_shooting(Boss *boss, int frame_count, int shoot_pattern, ALLEGRO_BITMAP *fast_bullet_sprite, ALLEGRO_BITMAP *slow_bullet_sprite) {
    if (!boss) return shoot_pattern;

    // Alterna padrão de disparo a cada 150 frames
    if (frame_count % 150 == 0) {
        shoot_pattern = (shoot_pattern == 1) ? 2 : 1;
    }

    // Realiza disparos conforme o padrão
    if (shoot_pattern == 1 && boss->cooldown == 0) {
        boss_shoot(boss, slow_bullet_sprite, fast_bullet_sprite);
    } else if (shoot_pattern == 2 && boss->cooldown == 0) {
        boss_shoot(boss, fast_bullet_sprite, slow_bullet_sprite);
    }

    // Reduz cooldown do Boss
    if (boss->cooldown > 0) boss->cooldown--;

    return shoot_pattern;
}



void update_boss(Boss *boss, square *player) {
    if (boss == NULL || player == NULL) return;

    // Calcular a diferença entre a posição do boss e a do jogador
    float delta_x = player->x - boss->x;
    float delta_y = player->y - boss->y;

    // Calcular a distância entre o boss e o jogador
    float distance = sqrt(delta_x * delta_x + delta_y * delta_y);

    // Evitar que o boss se mova a uma velocidade superior à distância
    if (distance != 0) {
        // Normalizar a direção
        float direction_x = delta_x / distance;
        float direction_y = delta_y / distance;

        // Mover o boss na direção do jogador
        boss->x += direction_x * BOSS_MOVE_SPEED;
        boss->y += direction_y * BOSS_MOVE_SPEED;
    }

    // Verifica se o boss está dentro dos limites da tela
    if (boss->x < 0) boss->x = 0;
    if (boss->x > X_SCREEN) boss->x = X_SCREEN;
    if (boss->y < 0) boss->y = 0;
    if (boss->y > Y_SCREEN) boss->y = Y_SCREEN;
}

// Desenhar o Boss na tela
void draw_boss(Boss *boss, bool debug_mode) {
    if (!boss || !boss->sprite) return;

    // Desenha o sprite do Boss
    al_draw_bitmap(
        boss->sprite,
        boss->x - al_get_bitmap_width(boss->sprite) / 2,  // Centraliza em X
        boss->y - al_get_bitmap_height(boss->sprite) / 2, // Centraliza em Y
        0
    );

    // Desenha a barra de vida do Boss
    healthbar_draw(boss->health_bar);

    // Desenha os tiros do Boss
    boss_shot *current_shot = boss->shots;
    while (current_shot != NULL) {

        boss_shot_draw(current_shot, current_shot->sprite);  // Usa o sprite correto para cada tipo de tiro
        current_shot = current_shot->next;  // Avança para o próximo tiro
    }

    if (debug_mode) {
        al_draw_rectangle(
            boss->x - al_get_bitmap_width(boss->sprite) / 2,  // X inicial da hitbox
            boss->y - al_get_bitmap_height(boss->sprite) / 2, // Y inicial da hitbox
            boss->x + al_get_bitmap_width(boss->sprite) / 2,  // X final da hitbox
            boss->y + al_get_bitmap_height(boss->sprite) / 2, // Y final da hitbox
            al_map_rgb(255, 0, 0),                            // Cor da hitbox (vermelha)
            1                                                 // Espessura da linha
        );
    }
}

void update_boss_shots(Boss *boss) {
    boss_shot *current = boss->shots;
    boss_shot *prev = NULL;

    while (current != NULL) {
        // Atualiza a posição do tiro
        boss_shot_move(current);

        // Remove o tiro se sair da tela ou concluir o movimento de bumerangue
        if (current->x < 0 || current->x > X_SCREEN) {
            if (prev == NULL) { // Removendo o primeiro tiro
                boss->shots = current->next;
            } else { // Removendo um tiro no meio ou no final
                prev->next = current->next;
            }

            boss_shot *to_remove = current;
            current = current->next;
            boss_shot_destroy(to_remove); // Libera a memória
        } else {
            // Avança para o próximo tiro
            prev = current;
            current = current->next;
        }
    }
}



// Função para destruir o Boss e liberar a memória alocada
void destroy_boss(Boss *boss) {
    if (boss == NULL) return;

    // Destruir os tiros do Boss
    boss_shot *current_shot = boss->shots;
    while (current_shot != NULL) {
        boss_shot *next_bullet = current_shot->next;
        boss_shot_destroy(current_shot);
        current_shot = next_bullet;
    }

    // Destruir a barra de vida
    healthbar_destroy(boss->health_bar);

    free(boss);
}

// Função para verificar colisão do Boss com o jogador
void check_boss_collision_with_player(square *player, Boss *boss) {
    if (boss == NULL || player == NULL) return;

    // Verifica se há colisão do jogador com o boss
    if ((player->x + player->side / 2 >= boss->x - BOSS_WIDTH / 2) &&
        (player->x - player->side / 2 <= boss->x + BOSS_WIDTH / 2) &&
        (player->y + player->side / 2 >= boss->y - BOSS_HEIGHT / 2) &&
        (player->y - player->side / 2 <= boss->y + BOSS_HEIGHT / 2)) {

        int damage = 30;  // Dano causado pela colisão com o boss

        if (player->shield && player->shield->is_active) {
            printf("Escudo ativo! Dano recebido: %d\n", damage);

            // Aplica dano ao escudo
            if (player->shield->hp > damage) {
                player->shield->hp -= damage;
            } else {
                int excess_damage = damage - player->shield->hp;
                player->shield->hp = 0;
                player->shield->is_active = false;
                printf("Escudo destruído! Excesso de dano: %d\n", excess_damage);

                // Aplica o excesso de dano ao HP do jogador
                player->hp -= excess_damage;
            }
        } else {
            // Se o escudo não estiver ativo
            player->hp -= damage;
            printf("Jogador tomou dano! HP restante: %d\n", player->hp);
        }
    }
}
int check_collision(float x1, float y1, float width1, float height1,
                    float x2, float y2, float width2, float height2) {
    return !(x1 > x2 + width2 || x1 + width1 < x2 ||
             y1 > y2 + height2 || y1 + height1 < y2);
}

void check_player_bullets_with_boss_shots(square *player, Boss *boss) {
    if (!player || !boss || !player->gun || !player->gun->shots) return;

    bullet *player_bullet = player->gun->shots;
    bullet *prev_player_bullet = NULL;

    boss_shot *boss_bullet = boss->shots;
    boss_shot *prev_boss_bullet = NULL;

    while (player_bullet != NULL) {
        boss_bullet = boss->shots;
        prev_boss_bullet = NULL;

        while (boss_bullet != NULL) {
            // Verifica colisão
            if (check_collision(player_bullet->x, player_bullet->y, 10, 10,  // Supondo que o tamanho das balas do jogador seja 10x10
                                boss_bullet->x, boss_bullet->y, 20, 20)) {  // Tamanho das balas do boss
                if (boss_bullet->type == BOSS_SHOT_TYPE_SPLIT) {  // Apenas para tiros de bumerangue
                    // Remove o tiro do boss
                    if (prev_boss_bullet == NULL) {
                        boss->shots = boss_bullet->next;
                    } else {
                        prev_boss_bullet->next = boss_bullet->next;
                    }
                    boss_shot_destroy(boss_bullet);

                    // Remove o tiro do jogador
                    if (prev_player_bullet == NULL) {
                        player->gun->shots = player_bullet->next;
                    } else {
                        prev_player_bullet->next = player_bullet->next;
                    }
                    bullet_destroy(player_bullet);

                    return; // Sai da verificação após destruir o par
                }
            }
            prev_boss_bullet = boss_bullet;
            boss_bullet = boss_bullet->next;
        }
        prev_player_bullet = player_bullet;
        player_bullet = player_bullet->next;
    }
}

// Função para lidar com colisões do jogador com o Boss
void check_player_bullets_with_boss(square *player, Boss *boss) {
    if (!player || !boss) return;

    bullet *current_bullet = player->gun->shots;
    bullet *prev_bullet = NULL;

    // Pegue as dimensões do Boss
    float boss_x = boss->x - BOSS_WIDTH / 2;
    float boss_y = boss->y - BOSS_HEIGHT / 2;
    float boss_width = BOSS_WIDTH;
    float boss_height = BOSS_HEIGHT;

    while (current_bullet != NULL) {
        // Verifica se a bala do jogador colide com o Boss
        if (check_collision(current_bullet->x, current_bullet->y, 10, 10, // Dimensões da bala do jogador
                            boss_x, boss_y, boss_width, boss_height)) {

            // Diminua a vida do Boss
            boss->hp -= current_bullet->damage;

            // Atualize a barra de vida do Boss
            healthbar_update(boss->health_bar, boss->hp);

            // Remove a bala do jogador
            if (prev_bullet == NULL) {
                player->gun->shots = current_bullet->next;
            } else {
                prev_bullet->next = current_bullet->next;
            }

            bullet *to_remove = current_bullet;
            current_bullet = current_bullet->next;
            bullet_destroy(to_remove); // Libera a memória da bala
        } else {
            // Avança para o próximo tiro
            prev_bullet = current_bullet;
            current_bullet = current_bullet->next;
        }
    }
}

void check_boss_bullets_with_player(square *player, Boss *boss) {
    if (!player || !boss) return;

    boss_shot *current_shot = boss->shots;
    boss_shot *prev_shot = NULL;

    // Pegue as dimensões do jogador
    float player_x = player->x - player->side / 2;
    float player_y = player->y - player->side / 2;
    float player_width = player->side;
    float player_height = player->side;

    while (current_shot != NULL) {
        // Verifica se o tiro do Boss colide com o jogador
        if (check_collision(current_shot->x, current_shot->y, 10, 10, // Dimensões do tiro do Boss
                            player_x, player_y, player_width, player_height)) {

            int damage = current_shot->damage;  // Dano do tiro do Boss

            if (player->shield && player->shield->is_active) {
                printf("Escudo ativo! Dano recebido: %d\n", damage);

                // Aplica dano ao escudo
                if (player->shield->hp > damage) {
                    player->shield->hp -= damage;
                } else {
                    int excess_damage = damage - player->shield->hp;
                    player->shield->hp = 0;
                    player->shield->is_active = false;
                    printf("Escudo destruído! Excesso de dano: %d\n", excess_damage);

                    // Aplica o excesso de dano ao HP do jogador
                    player->hp -= excess_damage;
                }
            } else {
                // Se o escudo não estiver ativo
                player->hp -= damage;
                printf("Jogador tomou dano! HP restante: %d\n", player->hp);
            }

            // Remove o tiro do Boss
            if (prev_shot == NULL) {
                boss->shots = current_shot->next;
            } else {
                prev_shot->next = current_shot->next;
            }

            boss_shot *to_remove = current_shot;
            current_shot = current_shot->next;
            boss_shot_destroy(to_remove); // Libera a memória do tiro
        } else {
            // Avança para o próximo tiro
            prev_shot = current_shot;
            current_shot = current_shot->next;
        }
    }
}
