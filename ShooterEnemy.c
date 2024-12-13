#include <stdlib.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

#include "ShooterEnemy.h"
#include "BulletEnemy.h"

ShooterEnemy* create_shooter_enemy(float x, float y, int hp) {
    ShooterEnemy *new_enemy = (ShooterEnemy*) malloc(sizeof(ShooterEnemy));
    if (!new_enemy) return NULL;
    new_enemy->x = x;
    new_enemy->y = y;
     new_enemy->hitbox_width = 30.0f; 
    new_enemy->hitbox_height = 30.0f;
    new_enemy->hp = hp;
    new_enemy->shots = NULL;
    new_enemy->next = NULL;
    new_enemy->explosion = NULL;
    return new_enemy;
}

void shooter_enemy_shoot(ShooterEnemy *enemy) {
    if (enemy == NULL) return;

    bullet_enemy *new_bullet = bullet_enemy_create(enemy->x - 10, enemy->y, 0, 10, 0);  
    if (new_bullet) {
        new_bullet->next = enemy->shots;
        enemy->shots = new_bullet;
    }
}

void draw_shooter_enemies(ShooterEnemy *head, ALLEGRO_BITMAP *shooter_enemy_sprite, bool debug_mode) {
    if (head == NULL) {
        return; 
    }

    for (ShooterEnemy *current = head; current != NULL; current = current->next) {
        al_draw_bitmap(shooter_enemy_sprite, 
                       current->x - al_get_bitmap_width(shooter_enemy_sprite) / 2,
                       current->y - al_get_bitmap_height(shooter_enemy_sprite) / 2, 
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

void draw_shooter_bullets(ShooterEnemy *head, ALLEGRO_BITMAP *bullet_sprite) {
    for (ShooterEnemy *current = head; current != NULL; current = current->next) {
        if (current->shots != NULL) { 
            for (bullet_enemy *bullet = current->shots; bullet != NULL; bullet = bullet->next) {
                if (bullet != NULL) {
                    al_draw_bitmap(bullet_sprite,
                                   bullet->x - al_get_bitmap_width(bullet_sprite) / 2,
                                   bullet->y - al_get_bitmap_height(bullet_sprite) / 2,
                                   0);
                }
            }
        }
    }
}

unsigned char check_kill_shooter_enemies(square *player, ShooterEnemy **shooter_enemies, Score *score, Explosion **explosions, int *enemies_destroyed) {
    ShooterEnemy *previous = NULL;
    ShooterEnemy *current = *shooter_enemies;

    while (current != NULL) {
        bullet *prev_bullet = NULL;
        bullet *index = player->gun->shots;

        while (index != NULL) {
            if ((index->x >= current->x - current->hitbox_width / 2) &&
                (index->x <= current->x + current->hitbox_width / 2) &&
                (index->y >= current->y - current->hitbox_height / 2) &&
                (index->y <= current->y + current->hitbox_height / 2)) {

                current->hp -= index->damage;

                if (prev_bullet) {
                    prev_bullet->next = index->next;
                } else {
                    player->gun->shots = (bullet*)index->next;
                }
                bullet *to_destroy_bullet = index;
                index = (bullet*)index->next;
                bullet_destroy(to_destroy_bullet);

                if (current->hp <= 0) {

                    Explosion *new_explosion = create_explosion(current->x, current->y, 0.5);  
                    if (new_explosion) {
                        new_explosion->next = *explosions; 
                        *explosions = new_explosion;
                    }

                    score_increment(score, 20);
                    if (enemies_destroyed) {   
                        (*enemies_destroyed)++;
                    }  

                    if (previous) {
                        previous->next = current->next;
                    } else {
                        *shooter_enemies = current->next;
                    }

                    ShooterEnemy *to_destroy_enemy = current;
                    current = current->next;
                    destroy_shooter_enemy(to_destroy_enemy);

                    return 1;  
                }

                break;  
            }

            prev_bullet = index;
            index = (bullet*)index->next;
        }

        if (current != NULL) {
            previous = current;
            current = current->next;
        }
    }

    return 0; 
}

unsigned char check_collision_with_player_shooter_enemy(square *player, ShooterEnemy **shooter_enemies) {
    ShooterEnemy *previous = NULL;
    ShooterEnemy *current = *shooter_enemies;

    while (current != NULL) {
        float player_left = player->x - player->side / 2;
        float player_right = player->x + player->side / 2;
        float player_top = player->y - player->side / 2;
        float player_bottom = player->y + player->side / 2;

        float enemy_left = current->x - current->hitbox_width / 2;
        float enemy_right = current->x + current->hitbox_width / 2;
        float enemy_top = current->y - current->hitbox_height / 2;
        float enemy_bottom = current->y + current->hitbox_height / 2;

        if (player_right >= enemy_left && player_left <= enemy_right &&
            player_bottom >= enemy_top && player_top <= enemy_bottom) {
            
            if (player->shield && player->shield->is_active) {
                int damage_to_shield = 10;

                if (player->shield->hp > damage_to_shield) {
                    player->shield->hp -= damage_to_shield;
                } else {
                    int excess_damage = damage_to_shield - player->shield->hp;
                    player->shield->hp = 0;
                    player->shield->is_active = false;  

                    player->hp -= excess_damage;
                }
            } else {
                int damage = 20; 
                player->hp -= damage;
            }

            ShooterEnemy *next_enemy = current->next;
            if (previous != NULL) {
                previous->next = next_enemy;
            } else {
                *shooter_enemies = next_enemy;
            }

            destroy_shooter_enemy(current);

            return 1;  
        }

        previous = current;
        current = current->next;
    }

    return 0; 
}

void update_shooter_enemy(ShooterEnemy **head) {
    ShooterEnemy *previous = NULL;
    ShooterEnemy *current = *head;

    while (current != NULL) {
        current->x -= ENEMY_SPEED;  
        if (current->x < 0) {  
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
            if (bullet_ptr->type == 0) {  
                bullet_enemy_move(bullet_ptr);
            } else if (bullet_ptr->type == 1) { 
                bullet_ptr->x -= bullet_ptr->direction * BULLET_ENEMY_MOVE;
                if (bullet_ptr->x <= 0 || bullet_ptr->x >= 800) { 
                    bullet_ptr->direction *= -1;
                }
            }

            if ((bullet_ptr->x >= player->x - player->side / 2) && 
                (bullet_ptr->x <= player->x + player->side / 2) && 
                (bullet_ptr->y >= player->y - player->side / 2) &&
                (bullet_ptr->y <= player->y + player->side / 2)) {
                
                if (player->shield->is_active) {
                    shield_take_damage(player->shield, bullet_ptr->damage);
                } else {
                    player->hp -= bullet_ptr->damage;
                }

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

            if (bullet_ptr->x < 0 || bullet_ptr->y < 0 || bullet_ptr->y > 600) {
                bullet_enemy *next_bullet = bullet_ptr->next;  
                if (prev_bullet) {
                    prev_bullet->next = next_bullet;
                } else {
                    current->shots = next_bullet; 
                }
                bullet_enemy_destroy(bullet_ptr); 
                bullet_ptr = next_bullet; 
            }


            prev_bullet = bullet_ptr;
            bullet_ptr = bullet_ptr->next;
        }
    }
}

void destroy_shooter_enemy(ShooterEnemy *enemy) {
    if (enemy != NULL) {
        bullet_enemy *current_bullet = enemy->shots;
        while (current_bullet != NULL) {
            bullet_enemy *next_bullet = current_bullet->next;
            bullet_enemy_destroy(current_bullet);
            current_bullet = next_bullet;
        }
        free(enemy);
        enemy = NULL; 
    }
}
void destroy_all_shooter_enemies(ShooterEnemy *head) {
    while (head != NULL) {
        ShooterEnemy *next_enemy = head->next;
        destroy_shooter_enemy(head);
        head = next_enemy;
    }
}
