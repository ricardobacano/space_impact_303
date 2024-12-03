#ifndef __LASER_H__
#define __LASER_H__

#include <stdbool.h>
#include "Square.h"  // Para usar a estrutura square
#include "Boss.h"
#include "Enemy.h"
#include "ShooterEnemy.h"
#include <allegro5/allegro_primitives.h>

// Definições para o laser
#define LASER_SPEED 10.0         // Velocidade de movimento do laser
#define LASER_WIDTH 20          // Largura do laser
#define LASER_DAMAGE 20         // Dano causado pelo laser
#define LASER_COOLDOWN 15.0

// Estrutura do Laser
typedef struct Laser {
    float x;               // Posição X do laser
    float width;           // Largura do laser
    float speed;           // Velocidade de movimento do laser
    bool is_active;        // Indica se o laser está ativo
    unsigned int damage;   // Dano do laser
} Laser;

// Funções do Laser
Laser* laser_create(float initial_x, float width, float speed, unsigned int damage); // Inicializa o laser
void laser_update(Laser *laser);                                                    // Atualiza a posição do laser
void laser_draw(Laser *laser);                                                      // Desenha o laser
void laser_check_collision_with_player(square *player, Laser *laser);               // Verifica colisão com o jogador
void activate_laser(Laser *laser);                                                  // Ativa o laser
void laser_check_collision_with_boss(Laser *laser, Boss *boss);
void laser_check_collision_with_enemies(Laser *laser, Enemy **enemies);
void draw_laser_cooldown_bar(float cooldown_timer, float max_cooldown, float screen_width, float screen_height);
void laser_check_collision_with_shooter_enemies(Laser *laser, ShooterEnemy **shooter_enemies);

#endif
