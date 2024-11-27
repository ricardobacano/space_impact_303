#ifndef __SQUARE__  
#define __SQUARE__ 

#include "Joystick.h"
#include "Pistol.h"
#include "Shield.h"
#include "ShieldBar.h"

#define SQUARE_STEP 10
#define X_SCREEN 800
#define Y_SCREEN 600

typedef struct {
    unsigned char side;
    unsigned char face;
    unsigned char hp;
    unsigned short x;
    unsigned short y;
    joystick *control;
    pistol *gun;
    Shield *shield;
} square;

square* square_create(unsigned char side, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);
void square_move(square *element, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y);
void square_shot(square *element);
void square_destroy(square *element);
void square_draw(square *player, ALLEGRO_BITMAP* spaceship_image);
void update_position(square *player);
void update_bullets(square *player);

#endif  
