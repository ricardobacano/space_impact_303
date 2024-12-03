#ifndef __BOSS_APPEARANCE_H__
#define __BOSS_APPEARANCE_H__

#include <allegro5/allegro.h>

typedef struct {
    float x, y;             // Posição do boss
    float scale;            // Escala do boss (para efeito de crescimento)
    float opacity;          // Opacidade (para efeito de fade-in)
    float duration;         // Duração total da animação
    float elapsed_time;     // Tempo decorrido desde o início da animação
    bool is_active;         // Indica se a animação está em andamento
} BossAppearance;

BossAppearance* create_boss_appearance(float x, float y, float duration);
void update_boss_appearance(BossAppearance *appearance, float delta_time);
void draw_boss_appearance(ALLEGRO_BITMAP *boss_sprite, float progress, int screen_width, int screen_height);
void destroy_boss_appearance(BossAppearance *appearance);

#endif
