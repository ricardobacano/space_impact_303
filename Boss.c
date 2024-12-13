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
    new_boss->direction = 1;  
    new_boss->health_bar = healthbar_create(X_SCREEN - 130, 20, new_boss->hp);  
    new_boss->sprite = sprite; 

    return new_boss;
}

void boss_shoot(Boss *boss, ALLEGRO_BITMAP *fast_bullet_sprite, ALLEGRO_BITMAP *slow_bullet_sprite) {
    if (boss == NULL) return;

    static int alternate_shot = 0; 
    if (boss->cooldown == 0) {
        if (alternate_shot == 0) {
            boss_shot *new_fast_bullet = boss_shot_create(boss->x - BOSS_WIDTH / 2, boss->y, BOSS_SHOT_TYPE_FAST, 20);
            if (new_fast_bullet) {
                new_fast_bullet->next = boss->shots;
                boss->shots = new_fast_bullet;
                new_fast_bullet->sprite = fast_bullet_sprite;
            }
        } else {
            boss_shot *new_boomerang_bullet = boss_shot_create(boss->x - BOSS_WIDTH / 2, boss->y, BOSS_SHOT_TYPE_SPLIT, 20);
            if (new_boomerang_bullet) {
                new_boomerang_bullet->next = boss->shots;
                boss->shots = new_boomerang_bullet;
                new_boomerang_bullet->sprite = slow_bullet_sprite;
            }
        }

        alternate_shot = 1 - alternate_shot;

        boss->cooldown = BOSS_SHOT_COOLDOWN;  
    }
}


int update_boss_shooting(Boss *boss, int frame_count, int shoot_pattern, ALLEGRO_BITMAP *fast_bullet_sprite, ALLEGRO_BITMAP *slow_bullet_sprite) {
    if (!boss) return shoot_pattern;

    if (frame_count % 150 == 0) {
        shoot_pattern = (shoot_pattern == 1) ? 2 : 1;
    }

    if (shoot_pattern == 1 && boss->cooldown == 0) {
        boss_shoot(boss, slow_bullet_sprite, fast_bullet_sprite);
    } else if (shoot_pattern == 2 && boss->cooldown == 0) {
        boss_shoot(boss, fast_bullet_sprite, slow_bullet_sprite);
    }

    if (boss->cooldown > 0) boss->cooldown--;

    return shoot_pattern;
}

void update_boss(Boss *boss, square *player, bool is_paused) {
    if (boss == NULL || player == NULL) return;

    if (is_paused) return;

    float delta_x = player->x - boss->x;
    float delta_y = player->y - boss->y;

    float distance = sqrt(delta_x * delta_x + delta_y * delta_y);

    if (distance != 0) {
        float direction_x = delta_x / distance;
        float direction_y = delta_y / distance;

        boss->x += direction_x * BOSS_MOVE_SPEED;
        boss->y += direction_y * BOSS_MOVE_SPEED;
    }

    if (boss->x < 0) boss->x = 0;
    if (boss->x > X_SCREEN) boss->x = X_SCREEN;
    if (boss->y < 0) boss->y = 0;
    if (boss->y > Y_SCREEN) boss->y = Y_SCREEN;
}

void draw_boss(Boss *boss, bool debug_mode) {
    if (!boss || !boss->sprite) return;

    al_draw_bitmap(
        boss->sprite,
        boss->x - al_get_bitmap_width(boss->sprite) / 2,  
        boss->y - al_get_bitmap_height(boss->sprite) / 2, 
        0
    );

    healthbar_draw(boss->health_bar);

    boss_shot *current_shot = boss->shots;
    while (current_shot != NULL) {

        boss_shot_draw(current_shot, current_shot->sprite);  
        current_shot = current_shot->next;  
    }

    char hp_text[200];
    snprintf(hp_text, sizeof(hp_text), "%d/%d", boss->hp, 1000);
    al_draw_text(
        al_create_builtin_font(),                     
        al_map_rgb(0, 128, 0),                    
        boss->health_bar->x + HEALTHBAR_WIDTH - 170 ,  
        boss->health_bar->y - (HEALTHBAR_HEIGHT / 4), 
        0,                                           
        hp_text                                       
    );

    if (debug_mode) {
        al_draw_rectangle(
            boss->x - al_get_bitmap_width(boss->sprite) / 2,  
            boss->y - al_get_bitmap_height(boss->sprite) / 2, 
            boss->x + al_get_bitmap_width(boss->sprite) / 2,  
            boss->y + al_get_bitmap_height(boss->sprite) / 2, 
            al_map_rgb(255, 0, 0),                            
            1                                                 
        );
    }
}

void update_boss_shots(Boss *boss,bool is_paused) {
    boss_shot *current = boss->shots;
    boss_shot *prev = NULL;

    if (is_paused) return;

    while (current != NULL) {
        boss_shot_move(current);

        if (current->x < 0 || current->x > X_SCREEN) {
            if (prev == NULL) { 
                boss->shots = current->next;
            } else { 
                prev->next = current->next;
            }

            boss_shot *to_remove = current;
            current = current->next;
            boss_shot_destroy(to_remove); 
        } else {
            prev = current;
            current = current->next;
        }
    }
}

void destroy_boss(Boss *boss) {
    if (boss == NULL) return;

    boss_shot *current_shot = boss->shots;
    while (current_shot != NULL) {
        boss_shot *next_bullet = current_shot->next;
        boss_shot_destroy(current_shot);
        current_shot = next_bullet;
    }

    healthbar_destroy(boss->health_bar);


    free(boss);
}

void check_boss_collision_with_player(square *player, Boss *boss) {
    if (boss == NULL || player == NULL) return;

    if ((player->x + player->side / 2 >= boss->x - BOSS_WIDTH / 2) &&
        (player->x - player->side / 2 <= boss->x + BOSS_WIDTH / 2) &&
        (player->y + player->side / 2 >= boss->y - BOSS_HEIGHT / 2) &&
        (player->y - player->side / 2 <= boss->y + BOSS_HEIGHT / 2)) {

        int damage = 10;  

        if (player->shield && player->shield->is_active) {

            if (player->shield->hp > damage) {
                player->shield->hp -= damage;
            } else {
                int excess_damage = damage - player->shield->hp;
                player->shield->hp = 0;
                player->shield->is_active = false;

                player->hp -= excess_damage;
            }
        } else {
            player->hp -= damage;
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
            if (check_collision(player_bullet->x, player_bullet->y, 10, 10,  
                                boss_bullet->x, boss_bullet->y, 20, 20)) {  
                if (boss_bullet->type == BOSS_SHOT_TYPE_SPLIT) {  
                    if (prev_boss_bullet == NULL) {
                        boss->shots = boss_bullet->next;
                    } else {
                        prev_boss_bullet->next = boss_bullet->next;
                    }
                    boss_shot_destroy(boss_bullet);

                    if (prev_player_bullet == NULL) {
                        player->gun->shots = player_bullet->next;
                    } else {
                        prev_player_bullet->next = player_bullet->next;
                    }
                    bullet_destroy(player_bullet);

                    return; 
                }
            }
            prev_boss_bullet = boss_bullet;
            boss_bullet = boss_bullet->next;
        }
        prev_player_bullet = player_bullet;
        player_bullet = player_bullet->next;
    }
}

void check_player_bullets_with_boss(square *player, Boss *boss) {
    if (!player || !boss) return;

    bullet *current_bullet = player->gun->shots;
    bullet *prev_bullet = NULL;

    float boss_x = boss->x - BOSS_WIDTH / 2;
    float boss_y = boss->y - BOSS_HEIGHT / 2;
    float boss_width = BOSS_WIDTH;
    float boss_height = BOSS_HEIGHT;

    while (current_bullet != NULL) {
        if (check_collision(current_bullet->x, current_bullet->y, 10, 10, 
                            boss_x, boss_y, boss_width, boss_height)) {

            boss->hp -= current_bullet->damage;

            healthbar_update(boss->health_bar, boss->hp);

            if (prev_bullet == NULL) {
                player->gun->shots = current_bullet->next;
            } else {
                prev_bullet->next = current_bullet->next;
            }

            bullet *to_remove = current_bullet;
            current_bullet = current_bullet->next;
            bullet_destroy(to_remove); 
        } else {
            prev_bullet = current_bullet;
            current_bullet = current_bullet->next;
        }
    }
}

void check_boss_bullets_with_player(square *player, Boss *boss) {
    if (!player || !boss) return;

    boss_shot *current_shot = boss->shots;
    boss_shot *prev_shot = NULL;

    // achar as dimensões do player
    float player_x = player->x - player->side / 2;
    float player_y = player->y - player->side / 2;
    float player_width = player->side;
    float player_height = player->side;

    while (current_shot != NULL) {
        if (check_collision(current_shot->x, current_shot->y, 10, 10, 
                            player_x, player_y, player_width, player_height)) {

            int damage = current_shot->damage; 

            if (player->shield && player->shield->is_active) {

                if (player->shield->hp > damage) {
                    player->shield->hp -= damage;
                } else {
                    int excess_damage = damage - player->shield->hp;
                    player->shield->hp = 0;
                    player->shield->is_active = false;

                    player->hp -= excess_damage;
                }
            } else {
                player->hp -= damage;
            }

            if (prev_shot == NULL) {
                boss->shots = current_shot->next;
            } else {
                prev_shot->next = current_shot->next;
            }

            boss_shot *to_remove = current_shot;
            current_shot = current_shot->next;
            boss_shot_destroy(to_remove); 
        } else {
            prev_shot = current_shot;
            current_shot = current_shot->next;
        }
    }
}
