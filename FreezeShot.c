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
    new_shot->width = width;
    new_shot->height = height;
    new_shot->is_active = true;
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

        if (current->y > screen_height || !current->is_active) {
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
void draw_freeze_shots(FreezeShot *shots, bool debug_mode) {
    FreezeShot *current = shots;

    while (current != NULL) {
        if (current->is_active) {
            if (current->sprite) {
                al_draw_bitmap(current->sprite, current->x - current->width / 2, current->y - current->height / 2, 0);
            } else {
                // Desenha um retângulo genérico se não houver sprite
                al_draw_filled_rectangle(
                    current->x - current->width / 2,
                    current->y - current->height / 2,
                    current->x + current->width / 2,
                    current->y + current->height / 2,
                    al_map_rgb(0, 255, 255)
                );
            }

            if (debug_mode) {
                al_draw_rectangle(
                    current->x - current->width / 2,
                    current->y - current->height / 2,
                    current->x + current->width / 2,
                    current->y + current->height / 2,
                    al_map_rgb(255, 0, 0),
                    1
                );
            }
        }
        current = current->next;
    }
}

// Verifica colisão com o jogador
bool check_freeze_shot_collision(FreezeShot *shot, square *player) {
    if (!shot || !player) return false;

    if (shot->x + shot->width / 2 >= player->x - player->side / 2 &&
        shot->x - shot->width / 2 <= player->x + player->side / 2 &&
        shot->y + shot->height / 2 >= player->y - player->side / 2 &&
        shot->y - shot->height / 2 <= player->y + player->side / 2) {
        shot->is_active = false; // Desativa o tiro após a colisão
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
