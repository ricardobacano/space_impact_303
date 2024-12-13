#ifndef __BOSS_APPEARANCE_H__
#define __BOSS_APPEARANCE_H__

#include <allegro5/allegro.h>

typedef struct {
    float x, y;             
    float scale;           
    float opacity;          
    float duration;        
    float elapsed_time;     
    bool is_active;         
} BossAppearance;

BossAppearance* create_boss_appearance(float x, float y, float duration);

// atualização da animação
void update_boss_appearance(BossAppearance *appearance, float delta_time);

// desenha do boss (para utilizar na animação)
void draw_boss_appearance(ALLEGRO_BITMAP *boss_sprite, float progress, int screen_width, int screen_height);

// libera a memoria de animação (salva ponteiros)
void destroy_boss_appearance(BossAppearance *appearance);

#endif
