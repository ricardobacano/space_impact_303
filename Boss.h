#ifndef __BOSS_H__
#define __BOSS_H__

#include "Square.h"
#include "HealthBar.h"
#include "BossShot.h"  
#include "Shield.h"

#define SCREEN_HEIGHT 600

#define BOSS_WIDTH 100
#define BOSS_HEIGHT 60
#define BOSS_MAX_HP 1000
#define BOSS_SHOT_COOLDOWN 100  
#define BOSS_SHOT_RATE 10       
#define BOSS_MOVE_SPEED 0.7       
#define FAST_SHOT_SPEED 10    
#define SPLIT_SHOT_SPEED 5    
#define SPLIT_SHOT_RETURN_DISTANCE 100 

typedef struct Boss {
    int direction;
    float x, y;              
    unsigned int hp;         
    unsigned int cooldown;   
    int next_shot_type;
    boss_shot *shots;       
    HealthBar *health_bar;  
    ALLEGRO_BITMAP *sprite;  
    ALLEGRO_BITMAP *bullet_sprite;  
} Boss;

Boss* create_boss(float x, float y, ALLEGRO_BITMAP *sprite);

// tiro do boss (função para coordenar, não para atirar)
void boss_shoot(Boss *boss, ALLEGRO_BITMAP *fast_bullet_sprite, ALLEGRO_BITMAP *slow_bullet_sprite);

// movimento do boss
void update_boss(Boss *boss, square *player, bool is_paused);

// libera a memoria do boss
void destroy_boss(Boss *boss);         

// desenha o boss
void draw_boss(Boss *boss, bool debug_mode);

// colisão do player com o boss
void check_boss_collision_with_player(square *player, Boss *boss);  

// atualiza o tiro do boss
int update_boss_shooting(Boss *boss, int frame_count, int shoot_pattern, ALLEGRO_BITMAP *fast_bullet_sprite, ALLEGRO_BITMAP *slow_bullet_sprite);

// movimento das balas do boss
void update_boss_shots(Boss *boss,bool is_paused);

// testar colisão (uso geral)
int check_collision(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2); 

// testar a colisão do tiro do player com o tiro bumerangue
void check_player_bullets_with_boss_shots(square *player, Boss *boss);

// testar colisão das balas do player no boss
void check_player_bullets_with_boss(square *player, Boss *boss);

// testar balas do boss no player
void check_boss_bullets_with_player(square *player, Boss *boss);

#endif
