#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <time.h>
#include <stdio.h>

#include "Square.h"
#include "HealthBar.h"
#include "StartScreen.h"
#include "Enemy.h"  
#include "Score.h"  
#include "ShooterEnemy.h"
#include "Scrap.h"
#include "Boss.h"
#include "FPS.h"
#include "Shield.h"
#include "ShieldBar.h"
#include "SelectionScreen.h"
#include "BackGround.h"

#define X_SCREEN 800
#define Y_SCREEN 600
#define INITIAL_ENEMY_SPEED 2.0f  
#define MAX_ENEMY_SPEED 10.0f     
#define SPEED_INCREMENT 0.5f      

float enemy_speed = INITIAL_ENEMY_SPEED;
int frame_count = 0;

int main() {
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_ttf_addon();
    al_init_font_addon();
    al_init_image_addon(); 

    srand(time(NULL));  // Semente para valores aleatórios baseada no tempo

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();

    ALLEGRO_FONT* font = al_load_font("./imagens/fontes.ttf",15,5);
    if (!font) {
        fprintf(stderr, "erro na fonte.\n");
        return -1;
    }

    ALLEGRO_BITMAP* spaceship_image = al_load_bitmap("./imagens/navezinha_Certa.png");
    if (!spaceship_image) {
        fprintf(stderr, "erro na imagem.\n");
        return -1;
    }

    ALLEGRO_BITMAP* bullet_sprite = NULL;
    ALLEGRO_BITMAP* shooter_bullet_sprite = NULL;

    bullet_sprite = al_load_bitmap("./imagens/tiro_player.png");
    if (!bullet_sprite) {
        fprintf(stderr, "Erro ao carregar o sprite do projétil do jogador.\n");
        return -1;
    }

    shooter_bullet_sprite = al_load_bitmap("./imagens/tiro_dadiva.png");
    if (!shooter_bullet_sprite) {
        fprintf(stderr, "Erro ao carregar o sprite do projétil dos inimigos atiradores.\n");
        return -1;
    }


    ALLEGRO_BITMAP* enemy_sprite = al_load_bitmap("./imagens/inimigo_1.png");
    if (!enemy_sprite) {
        fprintf(stderr, "Erro ao carregar o sprite do inimigo.\n");
        return -1;
    }

    ALLEGRO_BITMAP* shooter_enemy_sprite = al_load_bitmap("./imagens/inimigo_2.png");
    if (!shooter_enemy_sprite) {
        fprintf(stderr, "Erro ao carregar o sprite do inimigo atirador.\n");
        return -1;
    }

    ALLEGRO_BITMAP* scrap_sprite = al_load_bitmap("./imagens/scrap.webp");
    if (!scrap_sprite) {
        fprintf(stderr, "Erro ao carregar o sprite de sucata.\n");
        return -1;
    }


    ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    background_init("./imagens/fundo_space.png");
    display_start_screen(disp, queue, font);

    square* player_1 = square_create(20, 1, 10, Y_SCREEN / 2, X_SCREEN, Y_SCREEN);
    if (!player_1) return 1;
    player_1->hp = 100;

    HealthBar *player_1_healthbar = healthbar_create(10, 10, player_1->hp);
    Shield *player_shield = shield_create(); 
    ShieldBar *player_1_shieldbar = shieldbar_create(10 + HEALTHBAR_WIDTH + 10, 10, player_shield->duration);

    Score *score = score_create();
    if (!score) return 2;

    Enemy *enemies = NULL;
    ShooterEnemy* shooter_enemies = NULL;

    Scrap *scrap_list = NULL;
    int scrap_count = 0; // Contador de sucata

    Boss *boss = NULL;

    SelectionScreen *selection_screen = selection_screen_create();

    ALLEGRO_EVENT event;
    al_start_timer(timer);
    unsigned char p1k = 0;
    bool is_paused = false;  // Variável para controlar se o jogo está em pausa
    bool is_double_speed = false;  

    while (1) {
        al_wait_for_event(queue, &event);
        if (p1k) {
            // Tela de game over
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, "GAME OVER OTÁRIO");
            al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 + 20, ALLEGRO_ALIGN_CENTER, "Pressione ESPAÇO para sair");
            al_flip_display();
            if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_SPACE) break;
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
        } else {
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (selection_screen->is_visible) {
                    selection_screen_handle_input(selection_screen, &event);
                    if (event.keyboard.keycode == ALLEGRO_KEY_Q) {
                        selection_screen_hide(selection_screen);
                        is_paused = false;
                        player_1->control->left = 0;
                        player_1->control->right = 0;
                        player_1->control->up = 0;
                        player_1->control->down = 0;
                    }
                } else {
                    if (event.keyboard.keycode == ALLEGRO_KEY_2) {
                        toggle_game_speed(timer, &is_double_speed);
                        background_update(2.0);
                    }
                    if (event.keyboard.keycode == ALLEGRO_KEY_P) {
                        is_paused = !is_paused;
                        if (is_paused) {
                            printf("Jogo pausado\n");
                        } else {
                            printf("Jogo retomado\n");
                        }
                    }
                    if (event.keyboard.keycode == ALLEGRO_KEY_B && boss == NULL) {
                        boss = create_boss(X_SCREEN - 100, Y_SCREEN / 2);
                        printf("Boss apareceu!\n");
                    }
                    if (event.keyboard.keycode == ALLEGRO_KEY_E) {
                        shield_activate(player_shield);
                    }
                    if (!is_paused) {
                        if (event.keyboard.keycode == ALLEGRO_KEY_A) { player_1->control->left = 1;  
                        } else if (event.keyboard.keycode == ALLEGRO_KEY_D) { player_1->control->right = 1;  
                        } else if (event.keyboard.keycode == ALLEGRO_KEY_W) { player_1->control->up = 1;  
                        } else if (event.keyboard.keycode == ALLEGRO_KEY_S) { player_1->control->down = 1;  
                        } else if (event.keyboard.keycode == ALLEGRO_KEY_C) { player_1->control->fire = 1;  
                        }
                    }
                }
            } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
                if (!is_paused) {
                    if (event.keyboard.keycode == ALLEGRO_KEY_A) { player_1->control->left = 0;  
                    } else if (event.keyboard.keycode == ALLEGRO_KEY_D) { player_1->control->right = 0; 
                    } else if (event.keyboard.keycode == ALLEGRO_KEY_W) { player_1->control->up = 0;  
                    } else if (event.keyboard.keycode == ALLEGRO_KEY_S) { player_1->control->down = 0;  
                    } else if (event.keyboard.keycode == ALLEGRO_KEY_C) { player_1->control->fire = 0;  
                    }
                }
            } else if (event.type == ALLEGRO_EVENT_TIMER) {
                if (!is_paused) {
                    frame_count++;

                    background_update(enemy_speed / 2.0);  

                    if (frame_count % 600 == 0) {
                        enemy_speed += SPEED_INCREMENT;
                        if (enemy_speed > MAX_ENEMY_SPEED) {
                            enemy_speed = MAX_ENEMY_SPEED;
                        }
                    }

                    if (enemy_speed == MAX_ENEMY_SPEED && boss == NULL) {
                        boss = create_boss(X_SCREEN - 100, Y_SCREEN / 2);
                        printf("Boss apareceu automaticamente!\n");
                    }

                    if (boss == NULL && rand() % 50 == 0) {
                        float new_x = X_SCREEN;
                        float new_y = rand() % Y_SCREEN;

                        if (!check_collision_with_enemies(new_x, new_y, enemies) &&
                            !check_collision_with_shooter_enemies(new_x, new_y, shooter_enemies)) {
                            Enemy *new_enemy = create_enemy(new_x, new_y, 10);
                            if (new_enemy) {
                                new_enemy->next = enemies;
                                enemies = new_enemy;
                            }
                        }
                    }

                    if (boss == NULL && rand() % 200 == 0) {
                        float new_x = X_SCREEN;
                        float new_y = rand() % Y_SCREEN;

                        if (!check_collision_with_enemies(new_x, new_y, enemies) &&
                            !check_collision_with_shooter_enemies(new_x, new_y, shooter_enemies)) {
                            ShooterEnemy *new_shooter = create_shooter_enemy(new_x, new_y, 15);
                            if (new_shooter) {
                                new_shooter->next = shooter_enemies;
                                shooter_enemies = new_shooter;
                            }
                        }
                    }

                    if (rand() % 500 == 0) {
                        float new_x = X_SCREEN;
                        float new_y = rand() % Y_SCREEN;

                        Scrap *new_scrap = create_scrap(new_x, new_y);
                        if (new_scrap) {
                            new_scrap->next = scrap_list;
                            scrap_list = new_scrap;
                        }
                    }

                    if (scrap_count >= 10) {
                        scrap_count = 0;
                        selection_screen_show(selection_screen);
                        is_paused = true;
                    }

                    ShooterEnemy *current_shooter = shooter_enemies;
                    while (current_shooter != NULL) {
                        if (frame_count % 100 == 0) {
                            shooter_enemy_shoot(current_shooter);
                        }
                        current_shooter = current_shooter->next;
                    }

                    if (boss != NULL) {
                        update_boss(boss);
                    }

                    update_position(player_1);
                    update_bullets(player_1);
                    update_enemies(&enemies, enemy_speed);
                    update_shooter_enemy(&shooter_enemies);
                    move_shooter_bullets(shooter_enemies, player_1);
                    move_scrap(scrap_list, enemy_speed);
                    shield_update(player_shield, al_get_time()); 
                    shieldbar_update(player_1_shieldbar, player_shield->is_active ? player_shield->duration - (al_get_time() - player_shield->start_time) : 0);

                    check_collision_with_player(player_1, &enemies);
                    check_kill(player_1, &enemies, score);
                    check_kill_shooter_enemies(player_1, &shooter_enemies, score);
                    check_collision_with_player_shooter_enemy(player_1, &shooter_enemies);

                    if (boss != NULL) {
                        check_boss_collision_with_player(player_1, boss);
                    }

                    healthbar_update(player_1_healthbar, player_1->hp);
                    p1k = (player_1->hp <= 0) ? 1 : 0;
                }

                Scrap *prev_scrap = NULL;
                Scrap *current_scrap = scrap_list;

                while (current_scrap != NULL) {
                    if ((player_1->x + player_1->side / 2 >= current_scrap->x - 5) &&
                        (player_1->x - player_1->side / 2 <= current_scrap->x + 5) &&
                        (player_1->y + player_1->side / 2 >= current_scrap->y - 5) &&
                        (player_1->y - player_1->side / 2 <= current_scrap->y + 5)) {
                        
                        scrap_count++;

                        if (prev_scrap) {
                            prev_scrap->next = current_scrap->next;
                            destroy_scrap(current_scrap);
                            current_scrap = prev_scrap->next;
                        } else {
                            scrap_list = current_scrap->next;
                            destroy_scrap(current_scrap);
                            current_scrap = scrap_list;
                        }
                    } else {
                        prev_scrap = current_scrap;
                        current_scrap = current_scrap->next;
                    }
                }

                al_clear_to_color(al_map_rgb(0, 0, 0));

                selection_screen_draw(selection_screen, font);
                background_draw();
                healthbar_draw(player_1_healthbar);
                shield_draw(player_shield, player_1);
                shield_draw_bar(player_shield, font, 120, 10, 200, 20);

                
                char scrap_text[50];
                snprintf(scrap_text, 50, "Sucata: %d", scrap_count);
                al_draw_text(font, al_map_rgb(255, 255, 0), 10, 40, 0, scrap_text);

                draw_shield_timer(player_shield, font, 330, 15);

                square_draw(player_1, spaceship_image);

                draw_enemies(enemies, enemy_sprite);

                draw_scrap(scrap_list, scrap_sprite);

                for (bullet *index = player_1->gun->shots; index != NULL; index = index->next) {
                    // Centraliza o sprite do projétil na posição do projétil
                    al_draw_bitmap(bullet_sprite, index->x - al_get_bitmap_width(bullet_sprite) / 2, 
                                index->y - al_get_bitmap_height(bullet_sprite) / 2, 0);
                }


                for (ShooterEnemy *shooter = shooter_enemies; shooter != NULL; shooter = shooter->next) {
                    // Desenha o sprite do inimigo atirador
                    al_draw_bitmap(shooter_enemy_sprite, shooter->x - al_get_bitmap_width(shooter_enemy_sprite) / 2,
                                shooter->y - al_get_bitmap_height(shooter_enemy_sprite) / 2, 0);

                    // Desenha os projéteis disparados pelo inimigo atirador
                    for (bullet_enemy *index = shooter->shots; index != NULL; index = index->next) {
                        al_draw_bitmap(shooter_bullet_sprite, index->x - al_get_bitmap_width(shooter_bullet_sprite) / 2,
                                    index->y - al_get_bitmap_height(shooter_bullet_sprite) / 2, 0);
                    }
                }


                if (boss != NULL) {
                    draw_boss(boss);
                }

                if (selection_screen->is_visible) {
                    al_clear_to_color(al_map_rgb(0, 0, 0)); // Garante que o fundo inicial seja preto
                    selection_screen_draw(selection_screen, font);
                    selection_screen_handle_input(selection_screen, &event);
                }

                score_draw(score);

                if (player_1->gun->timer && !is_paused) player_1->gun->timer--;

                if (is_paused && scrap_count != 10) {
                     al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, "JOGO PAUSADO");
                    al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 + 20, ALLEGRO_ALIGN_CENTER, "Pressione 'P' para continuar");
                }

                al_flip_display();
            } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                break;
            }
        }
    }

    if (boss != NULL) {
        destroy_boss(boss);
    }

    background_destroy();
    destroy_all_enemies(enemies);
    destroy_all_shooter_enemies(shooter_enemies);
    destroy_all_scrap(scrap_list);
    healthbar_destroy(player_1_healthbar);
    score_destroy(score);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_bitmap(scrap_sprite);
    al_destroy_bitmap(enemy_sprite);
    al_destroy_bitmap(shooter_enemy_sprite);
    al_destroy_bitmap(bullet_sprite);
    al_destroy_bitmap(shooter_bullet_sprite);
    al_destroy_timer(timer);
    al_destroy_bitmap(spaceship_image);
    al_destroy_event_queue(queue);
    square_destroy(player_1);
    selection_screen_destroy(selection_screen);

    return 0;
}
