#include "BossAppearance.h"
#include <stdlib.h>

BossAppearance* create_boss_appearance(float x, float y, float duration) {
    BossAppearance *appearance = (BossAppearance*) malloc(sizeof(BossAppearance));
    if (!appearance) return NULL;

    appearance->x = x;
    appearance->y = y;
    appearance->scale = 0.1;  // começa pequenho
    appearance->opacity = 0.0; // começo invisivel
    appearance->duration = duration;
    appearance->elapsed_time = 0.0;
    appearance->is_active = true;

    return appearance;
}

void update_boss_appearance(BossAppearance *appearance, float delta_time) {
    if (!appearance || !appearance->is_active) return;

    appearance->elapsed_time += delta_time;

    if (appearance->elapsed_time < appearance->duration) {
        // calculo de escala de opacidade
        appearance->scale = 0.1 + (0.9 * (appearance->elapsed_time / appearance->duration));  
        appearance->opacity = appearance->elapsed_time / appearance->duration;  
    } else {
        appearance->scale = 1.0;
        appearance->opacity = 1.0;
        appearance->is_active = false; 
    }
}

void draw_boss_appearance(ALLEGRO_BITMAP *boss_sprite, float progress, int screen_width, int screen_height) {
    if (progress > 1.0f) progress = 1.0f; // Limita o progresso a no máximo 100%

    float scale = progress; // escala crescente, 0.0 (invisivel), 1.0 (tamanho normal)

    float x = screen_width - (al_get_bitmap_width(boss_sprite) * scale) - 50;
    float y = screen_height / 2 - (al_get_bitmap_height(boss_sprite) * scale) / 2;

    // desenha a escala do boss proporcional
    al_draw_scaled_bitmap(
        boss_sprite, 
        0, 0, 
        al_get_bitmap_width(boss_sprite), al_get_bitmap_height(boss_sprite), 
        x, y, 
        al_get_bitmap_width(boss_sprite) * scale, al_get_bitmap_height(boss_sprite) * scale, 
        0 
    );
}

void destroy_boss_appearance(BossAppearance *appearance) {
    if (appearance) free(appearance);
}
