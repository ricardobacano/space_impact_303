#ifndef __SCRAP_H__
#define __SCRAP_H__

#include <allegro5/allegro.h>

typedef struct Scrap {
    float x, y;
    unsigned char collected;
    struct Scrap *next;
} Scrap;

Scrap* create_scrap(float x, float y);
void move_scrap(Scrap *head, float speed);
void draw_scrap(Scrap *head, ALLEGRO_BITMAP* scrap_sprite);
void destroy_scrap(Scrap *scrap);
void destroy_all_scrap(Scrap *head);
void scrap_count_draw(int scrap_count); 

#endif
 