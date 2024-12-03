#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <allegro5/allegro5.h>

void background_init(const char* filepath);

void background_update(float speed);

void background_draw();

void background_destroy();

#endif 
