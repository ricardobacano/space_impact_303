#ifndef __LASER_H__
#define __LASER_H__

#include <allegro5/allegro_primitives.h>

#include <stdbool.h>
#include "Square.h"  
#include "Boss.h"
#include "Enemy.h"
#include "ShooterEnemy.h"
#include "Score.h"

#define LASER_SPEED 10.0         
#define LASER_WIDTH 20         
#define LASER_DAMAGE 20        
#define LASER_COOLDOWN 15.0

typedef struct Laser {
    float x;              
    float width;          
    float speed;          
    bool is_active;       
    unsigned int damage;   
} Laser;

Laser* laser_create(float initial_x, float width, float speed, unsigned int damage); 

// função pro laser andar 
void laser_update(Laser *laser);                                                    

// desenha o lasr
void laser_draw(Laser *laser);                                                      

// função pra ativar o laser
void activate_laser(Laser *laser);                                                  

// função colisão com boss
void laser_check_collision_with_boss(Laser *laser, Boss *boss);

// função colisão com inimigos convencionais 
void laser_check_collision_with_enemies(Laser *laser, Enemy **enemies, Score *score);

// função pra colisão com inimigo que atira 
void laser_check_collision_with_shooter_enemies(Laser *laser, ShooterEnemy **shooter_enemies, Score *score);

//função pra desenhar a barra de cooldown
void draw_laser_cooldown_bar(float cooldown_timer, float max_cooldown, float screen_width, float screen_height);


#endif
