#include <stdlib.h>
#include "BulletEnemy.h"

bullet_enemy* bullet_enemy_create(unsigned short x, unsigned short y, unsigned char type, unsigned char damage, unsigned char owner) {
    bullet_enemy *new_bullet = (bullet_enemy*) malloc(sizeof(bullet_enemy));
    if (!new_bullet) return NULL;

    new_bullet->x = x;
    new_bullet->y = y;
    new_bullet->damage = damage;
    new_bullet->type = type;
    new_bullet->owner = owner;  
    new_bullet->direction = (type == 1) ? 1 : 0;  
    new_bullet->next = NULL;

    return new_bullet;
}

void draw_bullets(bullet_enemy *bullets, ALLEGRO_BITMAP *enemy_bullet_sprite, ALLEGRO_BITMAP *boss_bullet_sprite) {
    for (bullet_enemy *index = bullets; index != NULL; index = index->next) {
        ALLEGRO_BITMAP *sprite = (index->owner == 1) ? boss_bullet_sprite : enemy_bullet_sprite;

        al_draw_bitmap(sprite,
                       index->x - al_get_bitmap_width(sprite) / 2,
                       index->y - al_get_bitmap_height(sprite) / 2,
                       0);
    }
}

void bullet_enemy_move(bullet_enemy *elements) {
    for (bullet_enemy *index = elements; index != NULL; index = index->next) {
        index->x -= BULLET_ENEMY_MOVE; 
    }
}

void bullet_enemy_destroy(bullet_enemy *element) {
    if (element != NULL) {
        free(element);
        element = NULL; 
    }
}
