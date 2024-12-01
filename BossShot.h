#ifndef __BOSS_SHOT__
#define __BOSS_SHOT__

#include <allegro5/allegro.h>

#include "Square.h"

// Definições de constantes para o Boss
#define BOSS_SHOT_TYPE_NORMAL 0
#define BOSS_SHOT_TYPE_SPLIT 1
#define BOSS_SHOT_TYPE_SLOW 4
#define BOSS_SHOT_TYPE_FAST 3
#define BOSS_FAST_BULLET_SPEED 8.0f   // Velocidade do tiro rápido
#define BOSS_SLOW_BULLET_SPEED 10.0f   // Velocidade do tiro bumerangue


// Estrutura para as balas do Boss
typedef struct boss_shot {
    unsigned short x;   // Posição x
    unsigned short y;   // Posição y
    unsigned char damage; // Dano da bala
    unsigned char type;  // Tipo da bala (normal ou divisão)
    int direction;       // Direção do tiro (para o tipo dividido)
    ALLEGRO_BITMAP *sprite;    // Sprite do tiro (adicionamos isso)
    struct boss_shot* next; // Próxima bala
} boss_shot;

// Funções para o controle das balas do Boss
boss_shot* boss_shot_create(unsigned short x, unsigned short y, unsigned char type, unsigned char damage);
void boss_shot_move(boss_shot *shot);
void boss_shot_destroy(boss_shot *shot);
void boss_shot_draw(boss_shot *shot, ALLEGRO_BITMAP *bullet_sprite);
void boss_shot_handle_collision(boss_shot *shot, square *player);
ALLEGRO_BITMAP *resize_sprite(ALLEGRO_BITMAP *original, int new_width, int new_height);

#endif
