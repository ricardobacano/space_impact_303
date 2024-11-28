#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro.h>

#include "Square.h"

square* square_create(unsigned char side, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y) {
    if ((x - side / 2 < 0) || (x + side / 2 > max_x) || (y - side / 2 < 0) || (y + side / 2 > max_y)) return NULL;
    if (face > 3) return NULL;

    square *new_square = (square*) malloc(sizeof(square));
    if (!new_square) return NULL;

    new_square->side = 55;
    new_square->face = face;
    new_square->hp = 5;
    new_square->x = x;
    new_square->y = y;
    new_square->control = joystick_create();
    new_square->gun = pistol_create();

    // Aloca e inicializa o escudo
    new_square->shield = (Shield*) malloc(sizeof(Shield));
    if (!new_square->shield) {
        free(new_square->control);
        free(new_square->gun);
        free(new_square);
        return NULL;
    }

    new_square->shield->is_active = false;
    new_square->shield->hp = 100.0f;  
    new_square->shield->max_hp = 100.0f;
    new_square->shield->cooldown = SHIELD_COOLDOWN;
    new_square->shield->duration = 5.0f;
    new_square->shield->last_used_time = -SHIELD_COOLDOWN;

    return new_square;
}


void square_move(square *element, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y) {
    if (!trajectory) {
        if ((element->x - steps * SQUARE_STEP) - element->side / 2 >= 0)
            element->x = element->x - steps * SQUARE_STEP;
    } else if (trajectory == 1) {
        if ((element->x + steps * SQUARE_STEP) + element->side / 2 <= max_x)
            element->x = element->x + steps * SQUARE_STEP;
    } else if (trajectory == 2) {
        if ((element->y - steps * SQUARE_STEP) - element->side / 2 >= 0)
            element->y = element->y - steps * SQUARE_STEP;
    } else if (trajectory == 3) {
        if ((element->y + steps * SQUARE_STEP) + element->side / 2 <= max_y)
            element->y = element->y + steps * SQUARE_STEP;
    }
}

void square_shot(square *element) {
    bullet *shot;
    if (!element->face) shot = pistol_shot(element->x - element->side / 2, element->y, element->face, element->gun);
    else if (element->face == 1) shot = pistol_shot(element->x + element->side / 2, element->y, element->face, element->gun);
    if (shot) element->gun->shots = shot;
}

void square_draw(square *player, ALLEGRO_BITMAP* spaceship_image) {
    if (!player || !spaceship_image) return;

    float scale_y = 1.0;  // Escala padrão
    if (player->control->up) {
        scale_y = 0.9;  // Diminuir altura ao subir
    } else if (player->control->down) {
        scale_y = 1.1;  // Aumentar altura ao descer
    }

    // Desenho com escala
    al_draw_scaled_bitmap(
        spaceship_image,
        0, 0,                                     
        al_get_bitmap_width(spaceship_image),     // Largura do bitmap
        al_get_bitmap_height(spaceship_image),    // Altura do bitmap
        player->x - (player->side / 2),           // Posição X (centralizada)
        player->y - (player->side / 2 * scale_y), // Posição Y (ajustada pela escala)
        player->side,                             // Nova largura
        player->side * scale_y,                   // Nova altura (ajustada pela escala)
        0                                         // Sem flags adicionais
    );
}

void square_destroy(square *element) {
    if (element) {
        if (element->control) joystick_destroy(element->control);
        if (element->gun) pistol_destroy(element->gun);
        if (element->shield) free(element->shield);
        free(element);
    }
}

void update_position(square *player) {
    if (player->control->left) {
        square_move(player, 1, 0, X_SCREEN, Y_SCREEN);
    }
    if (player->control->right) {
        square_move(player, 1, 1, X_SCREEN, Y_SCREEN);
    }
    if (player->control->up) {
        square_move(player, 1, 2, X_SCREEN, Y_SCREEN);
    }
    if (player->control->down) {
        square_move(player, 1, 3, X_SCREEN, Y_SCREEN);
    }

    if (player->control->fire) {
        if (!player->gun->timer) {
            square_shot(player);
            player->gun->timer = PISTOL_COOLDOWN;
        }
    }
}

void update_bullets(square *player) {
    bullet *previous = NULL;
    for (bullet *index = player->gun->shots; index != NULL;) {
        if (!index->trajectory) index->x -= BULLET_MOVE;
        else if (index->trajectory == 1) index->x += BULLET_MOVE;

        if ((index->x < 0) || (index->x > X_SCREEN)) {
            if (previous) {
                previous->next = index->next;
                bullet_destroy(index);
                index = (bullet*) previous->next;
            } else {
                player->gun->shots = (bullet*) index->next;
                bullet_destroy(index);
                index = player->gun->shots;
            }
        } else {
            previous = index;
            index = (bullet*) index->next;
        }
    }
}

void update_player_hp(square *player, int delta_hp) {
    player->hp += delta_hp;
    if (player->hp > 100) player->hp = 100;  // Limita ao máximo
    if (player->hp < 0) player->hp = 0;      // Evita valores negativos
    printf("HP do jogador atualizado para: %d\n", player->hp);
}