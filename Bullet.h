#ifndef __BULLET__
#define __BULLET__

#define BULLET_MOVE 6

typedef struct bullet {
    unsigned short x;                    // Localização no eixo x da bala
    unsigned short y;                    // Localização no eixo y da bala
    unsigned char trajectory;            // Trajetória da bala (0 - esquerda, 1 - direita)
    unsigned char damage;                // Dano que a bala causa ao atingir algo
    struct bullet *next;                 // Próxima bala na lista encadeada
} bullet;                                // Agora você pode usar "bullet" em vez de "struct bullet"

bullet* bullet_create(unsigned short x, unsigned short y, unsigned char trajectory, unsigned char damage, bullet *next);
void bullet_move(bullet *elements);
void bullet_destroy(bullet *element);

#endif
