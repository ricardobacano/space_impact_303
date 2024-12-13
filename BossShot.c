#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include "BossShot.h"

boss_shot* boss_shot_create(unsigned short x, unsigned short y, unsigned char type, unsigned char damage) {
    boss_shot* new_shot = (boss_shot*) malloc(sizeof(boss_shot));
    if (!new_shot) return NULL;

    new_shot->x = x;
    new_shot->y = y;
    new_shot->damage = damage;
    new_shot->type = type;
    new_shot->direction = 1; 
    new_shot->is_freeze = true;  
    new_shot->next = NULL;
    
    return new_shot;
}

void boss_shot_move(boss_shot *shot) {
    if (shot == NULL) return;

    // velocidades do tiro, baseado no tipo
    float speed_x = (shot->type == BOSS_SHOT_TYPE_FAST) ? BOSS_FAST_BULLET_SPEED : 0;
    float speed_y = (shot->type == BOSS_SHOT_TYPE_SPLIT) ? BOSS_SLOW_BULLET_SPEED : BOSS_FAST_BULLET_SPEED;

    if (shot->type == BOSS_SHOT_TYPE_FAST) {
        shot->x -= speed_x; 
    }

    // movimento do tiro bumerangue
    if (shot->type == BOSS_SHOT_TYPE_SPLIT) {
        shot->x -= speed_x;
        shot->y += shot->direction * speed_y; 

        // inverte a direção em y
        if (shot->y <= 0 || shot->y >= Y_SCREEN) {
            shot->direction *= -1; 
        }

        // movimento de retorno x
        if (shot->x <= X_SCREEN / 2) { 
            shot->x += speed_x; 
        }
    }
}

void boss_shot_draw(boss_shot *shot, ALLEGRO_BITMAP *bullet_sprite) {
    if (!shot || !bullet_sprite) {
        printf("Erro: Tiro ou sprite do tiro nulo.\n");
        return;
    }

    al_draw_bitmap(
        bullet_sprite,
        shot->x - al_get_bitmap_width(bullet_sprite) / 2,
        shot->y - al_get_bitmap_height(bullet_sprite) / 2,
        0
    );
}

void boss_shot_destroy(boss_shot *shot) {
    if (shot != NULL) {
        free(shot);
    }
}

void boss_shot_handle_collision(boss_shot *shot, square *player) {
    if (!shot || !player) return;

    if ((shot->x >= player->x - player->side / 2) && 
        (shot->x <= player->x + player->side / 2) && 
        (shot->y >= player->y - player->side / 2) && 
        (shot->y <= player->y + player->side / 2)) {

        player->hp -= shot->damage;

        shot->next = NULL;  
    }
}
