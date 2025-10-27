#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "IntroScreen.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCROLL_SPEED 1.0
#define FAST_SCROLL_SPEED 5.0  // velocidade quando o espaço está pressionado

void show_intro_screen(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *queue) {
    const char *intro_text[] = {
        "Ha muito tempo, em uma galaxia distante,",
        "",
        "Episodio 2",
        "SPACE IMPACT PROG2",
        "",
        "A galaxia enfrenta sua maior ameaça.",
        "Uma frota alienígena de malware esta",
        "varrendo sistemas estelares inteiros,",
        "destruindo tudo em seu caminho.",
        "",
        "Voce, um piloto lendario do C3SL e o ultimo",
        "sobrevivente da Força de Defesa do DINF,",
        "e a unica esperança que resta.",
        "",
        "Com uma nave de combate construida a partir",
        "de sucatas e uma determinacao inabalavel,",
        "sua missao e simples, mas mortal:",
        "",
        "Destrua a frota invasora,",
        "elimine os comandante alienigenas,",
        "e salve a galaxia!",
        "",
        "A batalha final pela sobrevivencia começa agora...",
        "",
        "Prepare-se para o Space Impact Prog2!"
    };

    int line_count = sizeof(intro_text) / sizeof(intro_text[0]);
    float text_y = SCREEN_HEIGHT;
    bool done = false;
    bool space_pressed = false;

    ALLEGRO_EVENT event;

    // É importante instalar o teclado antes de usar
    al_install_keyboard();

    while (!done) {
        while (al_get_next_event(queue, &event)) {
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                exit(0);
            } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    space_pressed = true;
                }
            } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
                if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    space_pressed = false;
                }
            }
        }

        // velocidade depende se o espaço está pressionado
        float current_speed = space_pressed ? FAST_SCROLL_SPEED : SCROLL_SPEED;
        text_y -= current_speed;

        // se passou todo o texto, encerra
        if (text_y + line_count * 30 < 0) {
            done = true;
        }

        al_clear_to_color(al_map_rgb(0, 0, 0));

        for (int i = 0; i < line_count; i++) {
            float line_x = SCREEN_WIDTH / 2;
            float line_y = text_y + i * 30;
            al_draw_text(font, al_map_rgb(255, 255, 0), line_x, line_y, ALLEGRO_ALIGN_CENTER, intro_text[i]);
        }

        al_flip_display();
        al_rest(1.0 / 60.0);
    }
}
