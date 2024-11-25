#include "BackGround.h"
#include <allegro5/allegro_image.h>
#include <stdio.h>

static ALLEGRO_BITMAP* background = NULL;
static float background_x = 0;

// Função para inicializar o fundo
void background_init(const char* filepath) {
    background = al_load_bitmap(filepath);
    if (!background) {
        fprintf(stderr, "Falha ao carregar a imagem de fundo: %s\n", filepath);
    }
    background_x = 0;  // Inicializa a posição do fundo
}

// Função para atualizar a posição do fundo para dar a sensação de movimento
void background_update(float speed) {
    background_x -= speed;  // Decrementa para dar a sensação de movimento para frente
    if (background_x <= -al_get_bitmap_width(background)) {
        background_x = 0;  // Reinicia quando a imagem sai da tela
    }
}

// Função para desenhar o fundo na tela
void background_draw() {
    if (background) {
        // Desenha o fundo duas vezes para criar a ilusão de movimento contínuo
        al_draw_bitmap(background, background_x, 0, 0);
        al_draw_bitmap(background, background_x + al_get_bitmap_width(background), 0, 0);
    }
}

// Função para destruir os recursos do fundo
void background_destroy() {
    if (background) {
        al_destroy_bitmap(background);
        background = NULL;
    }
}
