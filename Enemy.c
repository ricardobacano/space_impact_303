#include <stdlib.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <math.h>

#include "Enemy.h"
#include "ShooterEnemy.h"

Enemy* create_enemy(float x, float y, float hitbox_width, float hitbox_height, unsigned char hp) {
    Enemy *new_enemy = (Enemy*) malloc(sizeof(Enemy));
    if (!new_enemy) return NULL;
    new_enemy->x = x;
    new_enemy->y = y;
    new_enemy->hitbox_width = hitbox_width;  
    new_enemy->hitbox_height = hitbox_height; 
    new_enemy->hp = hp;
    new_enemy->next = NULL;
    new_enemy->explosions = NULL;  
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
        current->x -= speed;

        if (current->x < 0) {
            if (previous) {
                previous->next = current->next;
            } else {
                *enemies = current->next;
            }

            Enemy *temp = current;
            current = current->next;
            destroy_enemy(temp);
        } else {
            previous = current;
            current = current->next;
        }
    }
}


void draw_enemies(Enemy *head, ALLEGRO_BITMAP* enemy_sprite, bool debug_mode) {
    for (Enemy *current = head; current != NULL; current = current->next) {
        al_draw_bitmap(enemy_sprite, 
                       current->x - al_get_bitmap_width(enemy_sprite) / 2,
                       current->y - al_get_bitmap_height(enemy_sprite) / 2, 
                       0);

        if (debug_mode) {
            al_draw_rectangle(
                current->x - current->hitbox_width / 2, 
                current->y - current->hitbox_height / 2, 
                current->x + current->hitbox_width / 2, 
                current->y + current->hitbox_height / 2, 
                al_map_rgb(255, 0, 0), 1
            );
        }
    }
}

void destroy_enemy(Enemy *enemy) {
    if (enemy != NULL) {
        free(enemy);
        enemy = NULL;
    }
}


void destroy_all_enemies(Enemy **head) {
    Enemy *current = *head;
    Enemy *next = NULL;
    
    while (current != NULL) {
        next = current->next;  
        destroy_enemy(current); 
        current = next;         
    }
    
    *head = NULL;
}

void check_collision_with_player(square *player, Enemy **enemies) {
    Enemy *previous = NULL;
    Enemy *current = *enemies;

    while (current != NULL) {
        float enemy_left = current->x - current->hitbox_width / 2;
        float enemy_right = current->x + current->hitbox_width / 2;
        float enemy_top = current->y - current->hitbox_height / 2;
        float enemy_bottom = current->y + current->hitbox_height / 2;

        float player_left = player->x - player->side / 2;
        float player_right = player->x + player->side / 2;
        float player_top = player->y - player->side / 2;
        float player_bottom = player->y + player->side / 2;

        if (player_right >= enemy_left && player_left <= enemy_right &&
            player_bottom >= enemy_top && player_top <= enemy_bottom) {
            
            int damage = 10; 

            if (player->shield && player->shield->is_active) { 

                if (player->shield->hp > damage) {
                    player->shield->hp -= damage;
                } else {
                    int excess_damage = damage - player->shield->hp;
                    player->shield->hp = 0;
                    player->shield->is_active = false;

                    // caso esteja ativo ainda porem caiu, jogador recebe dano
                    player->hp -= excess_damage;
                }

            } else {
                player->hp -= damage;
            }

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

unsigned char check_kill(square *player, Enemy **enemies, Score *score, Explosion **explosion, int *enemies_destroyed) {
    Enemy *previous = NULL, *current = *enemies;

    while (current != NULL) {
        bullet *prev_bullet = NULL;
        for (bullet *index = player->gun->shots; index != NULL; index = (bullet*)index->next) {
            if ((index->x >= current->x - current->hitbox_width / 2) && 
                (index->x <= current->x + current->hitbox_width / 2) &&
                (index->y >= current->y - current->hitbox_height / 2) &&
                (index->y <= current->y + current->hitbox_height / 2)) {
                
                current->hp -= index->damage;

                if (current->hp <= 0) {
                    
                    Explosion *new_explosion = create_explosion(current->x, current->y, 0.5); 
                    if (new_explosion) {
                        new_explosion->next = *explosion;
                        *explosion = new_explosion;
                    }

                    score_increment(score, 10);
                    if (enemies_destroyed) {   
                        (*enemies_destroyed)++;
                    }

                    if (previous) {
                        previous->next = current->next;
                    } else {
                        *enemies = current->next;
                    }

                    Enemy *temp = current;
                    current = current->next;
                    destroy_enemy(temp);
                    return 1;
                }

                if (prev_bullet) {
                    prev_bullet->next = index->next;
                } else {
                    player->gun->shots = (bullet*)index->next;
                }
                bullet_destroy(index);
                break;  
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

unsigned char check_collision_between_all_enemies(float x, float y, Enemy *enemies, ShooterEnemy *shooter_enemies) {
    if (check_collision_with_enemies(x, y, enemies)) {
        return 1; 
    }
    if (check_collision_with_shooter_enemies(x, y, shooter_enemies)) {
        return 1; 
    }
    return 0; 
}