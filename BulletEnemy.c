#include <stdlib.h>
#include "BulletEnemy.h"

bullet_enemy* bullet_enemy_create(unsigned short x, unsigned short y) {
    bullet_enemy *new_bullet = (bullet_enemy*) malloc(sizeof(bullet_enemy));
    if (!new_bullet) return NULL;  // Verifica o sucesso da alocação de memória
    new_bullet->x = x;
    new_bullet->y = y;
    new_bullet->next = NULL;
    return new_bullet;  // Retorna o novo projétil criado
}

void bullet_enemy_move(bullet_enemy *elements) {
    for (bullet_enemy *index = elements; index != NULL; index = index->next) {
        index->x -= BULLET_ENEMY_MOVE;  // Move a bala para a esquerda
    }
}

void bullet_enemy_destroy(bullet_enemy *element) {
    free(element);  // Libera a memória do projétil
}
