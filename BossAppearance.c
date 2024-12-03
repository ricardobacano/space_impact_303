#include "BossAppearance.h"
#include <stdlib.h>

BossAppearance* create_boss_appearance(float x, float y, float duration) {
    BossAppearance *appearance = (BossAppearance*) malloc(sizeof(BossAppearance));
    if (!appearance) return NULL;

    appearance->x = x;
    appearance->y = y;
    appearance->scale = 0.1;  // Começa pequeno
    appearance->opacity = 0.0;  // Começa invisível
    appearance->duration = duration;
    appearance->elapsed_time = 0.0;
    appearance->is_active = true;

    return appearance;
}

void update_boss_appearance(BossAppearance *appearance, float delta_time) {
    if (!appearance || !appearance->is_active) return;

    appearance->elapsed_time += delta_time;

    if (appearance->elapsed_time < appearance->duration) {
        // Calcula a escala e opacidade progressivamente
        appearance->scale = 0.1 + (0.9 * (appearance->elapsed_time / appearance->duration));  // Cresce até 1.0
        appearance->opacity = appearance->elapsed_time / appearance->duration;  // Fica totalmente visível ao final
    } else {
        appearance->scale = 1.0;
        appearance->opacity = 1.0;
        appearance->is_active = false;  // Animação concluída
    }
}

void draw_boss_appearance(ALLEGRO_BITMAP *boss_sprite, float progress, int screen_width, int screen_height) {
    if (progress > 1.0f) progress = 1.0f; // Limita o progresso a no máximo 100%

    // Define escala e posição baseadas no progresso
    float scale = progress; // Escala crescente, de 0.0 (invisível) a 1.0 (tamanho normal)

    // Ajusta a posição para o canto direito, mas centralizado verticalmente
    float x = screen_width - (al_get_bitmap_width(boss_sprite) * scale) - 50; // 50px de margem do lado direito
    float y = screen_height / 2 - (al_get_bitmap_height(boss_sprite) * scale) / 2; // Centralizado verticalmente

    // Desenha o boss com a escala proporcional
    al_draw_scaled_bitmap(
        boss_sprite, // Sprite do Boss
        0, 0, // Coordenadas do sprite na fonte
        al_get_bitmap_width(boss_sprite), al_get_bitmap_height(boss_sprite), // Tamanho total do sprite
        x, y, // Coordenadas para desenhar na tela
        al_get_bitmap_width(boss_sprite) * scale, al_get_bitmap_height(boss_sprite) * scale, // Dimensões escaladas
        0 // Flags (sem rotação ou espelhamento)
    );
}



void destroy_boss_appearance(BossAppearance *appearance) {
    if (appearance) free(appearance);
}
