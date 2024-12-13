#ifndef __SQUARE__  
#define __SQUARE__ 

#include "Joystick.h"
#include "Pistol.h"
#include "Shield.h"
#include "ShieldBar.h"

#define SQUARE_STEP 10
#define X_SCREEN 800
#define Y_SCREEN 600
#define PLAYER_INITIAL_HP 200

typedef struct {
    unsigned char side;
    unsigned char face;
    int hp;
    unsigned short x;
    unsigned short y;
    bool is_frozen;  
    int freeze_timer;
    joystick *control;
    pistol *gun;
    Shield *shield;
} square;

square* square_create(unsigned char side, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);

void square_move(square *element, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y);

void square_shot(square *element);

void square_destroy(square *element);

void square_draw(square *player, ALLEGRO_BITMAP* spaceship_image, bool debug_mode);

void update_position(square *player);

void update_bullets(square *player);

void update_player_hp(square *player, int delta_hp);

#endif  
