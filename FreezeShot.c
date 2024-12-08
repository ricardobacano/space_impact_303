#include "FreezeShot.h"
#include <stdlib.h>
#include <allegro5/allegro_primitives.h>

// Cria um novo tiro congelante
FreezeShot *create_freeze_shot(float x, float y, float speed, float width, float height, ALLEGRO_BITMAP *sprite) {
    FreezeShot *new_shot = (FreezeShot *)malloc(sizeof(FreezeShot));
    if (!new_shot) return NULL;

    new_shot->x = x;
    new_shot->y = y;
    new_shot->speed = speed;
    new_shot->width = width;   // Agora com o campo width
    new_shot->height = height; // Agora com o campo height
    new_shot->active = true;   // Renomeado de is_active para active
    new_shot->sprite = sprite;
    new_shot->next = NULL;

    return new_shot;
}

// Atualiza a posição dos tiros congelantes
void update_freeze_shots(FreezeShot **shots, float delta_time, float screen_width, float screen_height) {
    FreezeShot *current = *shots;
    FreezeShot *prev = NULL;

    while (current != NULL) {
        current->y += current->speed * delta_time;

        if (current->y > screen_height || !current->active) {  // Renomeado is_active para active
            // Remove o tiro se ele sair da tela ou não estiver mais ativo
            FreezeShot *to_destroy = current;
            if (prev) {
                prev->next = current->next;
            } else {
                *shots = current->next;
            }
            current = current->next;
            free(to_destroy);
        } else {
            prev = current;
            current = current->next;
        }
    }
}


// Desenha os tiros congelantes
void draw_freeze_shots(FreezeShot *shots) {
    FreezeShot *current = shots;

    while (current != NULL) {
        if (current->active && current->sprite) { // Verifica se o tiro está ativo e tem sprite
            al_draw_bitmap(current->sprite, 
                           current->x - al_get_bitmap_width(current->sprite) / 2, 
                           current->y - al_get_bitmap_height(current->sprite) / 2, 
                           0); // Desenha o sprite centralizado
        }
        current = current->next; // Avança para o próximo tiro
    }
}


// Verifica colisão com o jogador
bool check_freeze_shot_collision(FreezeShot *shot, square *player) {
    if (!shot || !player) return false;

    if (shot->x + shot->width / 2 >= player->x - player->side / 2 &&
        shot->x - shot->width / 2 <= player->x + player->side / 2 &&
        shot->y + shot->height / 2 >= player->y - player->side / 2 &&
        shot->y - shot->height / 2 <= player->y + player->side / 2) {  // Usando width e height corretamente
        shot->active = false;  // Renomeado de is_active para active
        return true;
    }

    return false;
}


// Destroi a lista de tiros congelantes
void destroy_freeze_shots(FreezeShot **shots) {
    FreezeShot *current = *shots;
    while (current != NULL) {
        FreezeShot *to_destroy = current;
        current = current->next;
        free(to_destroy);
    }
    *shots = NULL;
}
