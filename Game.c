#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <time.h>

#include "Square.h"
#include "HealthBar.h"
#include "StartScreen.h"
#include "Enemy.h"  
#include "Score.h"  
#include "ShooterEnemy.h"
#include "Scrap.h"

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

    srand(time(NULL));  // Semente para valores aleatórios baseada no tempo

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    display_start_screen(disp, queue, font);

    square* player_1 = square_create(20, 1, 10, Y_SCREEN / 2, X_SCREEN, Y_SCREEN);
    if (!player_1) return 1;
    player_1->hp = 100;

    HealthBar *player_1_healthbar = healthbar_create(10, 10, player_1->hp);

    // Criação da pontuação
    Score *score = score_create();
    if (!score) return 2;

    // Lista de inimigos comuns e inimigos que atiram
    Enemy *enemies = NULL;
    ShooterEnemy* shooter_enemies = NULL;

    Scrap *scrap_list = NULL;
    int scrap_count = 0; // Contador de sucata

    ALLEGRO_EVENT event;
    al_start_timer(timer);
    unsigned char p1k = 0;

    while (1) {
        al_wait_for_event(queue, &event);

        if (p1k) {
            // Tela de game over
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, "GAME OVER");
            al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 + 20, ALLEGRO_ALIGN_CENTER, "Pressione ESPAÇO para sair");
            al_flip_display();
            if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_SPACE) break;
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
        } else {
            if (event.type == ALLEGRO_EVENT_TIMER) {
                frame_count++;

                // Aumenta a velocidade dos inimigos a cada 600 frames (~20 segundos se 30 FPS)
                if (frame_count % 600 == 0) {
                    enemy_speed += SPEED_INCREMENT;
                    if (enemy_speed > MAX_ENEMY_SPEED) {
                        enemy_speed = MAX_ENEMY_SPEED;
                    }
                }

                // Gera novos inimigos comuns aleatoriamente
                if (rand() % 50 == 0) {
                    float new_x = X_SCREEN;
                    float new_y = rand() % Y_SCREEN;

                    // Verifica colisão com outros inimigos antes de criar um novo
                    if (!check_collision_with_enemies(new_x, new_y, enemies) &&
                        !check_collision_with_shooter_enemies(new_x, new_y, shooter_enemies)) {
                        Enemy *new_enemy = create_enemy(new_x, new_y, 10);
                        if (new_enemy) {
                            new_enemy->next = enemies;
                            enemies = new_enemy;
                        }
                    }
                }

                // Gera novos inimigos que atiram aleatoriamente
                if (rand() % 200 == 0) {
                    float new_x = X_SCREEN;
                    float new_y = rand() % Y_SCREEN;

                    // Verifica colisão com outros inimigos antes de criar um novo
                    if (!check_collision_with_enemies(new_x, new_y, enemies) &&
                        !check_collision_with_shooter_enemies(new_x, new_y, shooter_enemies)) {
                        ShooterEnemy *new_shooter = create_shooter_enemy(new_x, new_y, 15);
                        if (new_shooter) {
                            new_shooter->next = shooter_enemies;
                            shooter_enemies = new_shooter;
                        }
                    }
                }

                // Gera novas sucatas aleatoriamente
                if (rand() % 100 == 0) {
                    float new_x = X_SCREEN;
                    float new_y = rand() % Y_SCREEN;

                    Scrap *new_scrap = create_scrap(new_x, new_y);
                    if (new_scrap) {
                        new_scrap->next = scrap_list;
                        scrap_list = new_scrap;
                    }
                }

                // Atualiza atiradores para atirar a cada intervalo de tempo
                ShooterEnemy *current_shooter = shooter_enemies;
                while (current_shooter != NULL) {
                    if (frame_count % 100 == 0) {
                        shooter_enemy_shoot(current_shooter);
                    }
                    current_shooter = current_shooter->next;
                }

                // Atualiza os elementos do jogo
                update_position(player_1);
                update_bullets(player_1);
                update_enemies(&enemies, enemy_speed);
                update_shooter_enemy(&shooter_enemies);
                move_shooter_bullets(shooter_enemies, player_1);
                move_scrap(scrap_list, enemy_speed);

                // Verifica colisões
                check_collision_with_player(player_1, &enemies);
                check_kill(player_1, &enemies, score);
                check_kill_shooter_enemies(player_1, &shooter_enemies, score);

                // Verifica colisão com sucata
                Scrap *prev_scrap = NULL;
                Scrap *current_scrap = scrap_list;

                while (current_scrap != NULL) {
                    if ((player_1->x + player_1->side / 2 >= current_scrap->x - 5) &&
                        (player_1->x - player_1->side / 2 <= current_scrap->x + 5) &&
                        (player_1->y + player_1->side / 2 >= current_scrap->y - 5) &&
                        (player_1->y - player_1->side / 2 <= current_scrap->y + 5)) {
                        
                        // Incrementa o contador de sucata
                        scrap_count++;

                        // Remove a sucata coletada da lista
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

                // Desenha todos os elementos
                al_clear_to_color(al_map_rgb(0, 0, 0));
                
                // Desenha a barra de vida
                healthbar_draw(player_1_healthbar);

                // Desenha o contador de sucata ao lado da barra de vida
                char scrap_text[50];
                al_draw_text(font, al_map_rgb(255, 255, 0), 10, 40, 0, scrap_text);  // Texto abaixo da barra de vida

                // Desenha o jogador
                al_draw_filled_rectangle(player_1->x - player_1->side / 2, player_1->y - player_1->side / 2,
                                         player_1->x + player_1->side / 2, player_1->y + player_1->side / 2,
                                         al_map_rgb(255, 0, 0));

                // Desenha os inimigos comuns
                draw_enemies(enemies);

                // Desenha as sucatas
                draw_scrap(scrap_list);

                // Desenha os tiros do jogador
                for (bullet *index = player_1->gun->shots; index != NULL; index = index->next) {
                    al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(255, 0, 0));
                }

                // Desenha os inimigos que atiram e seus tiros
                for (ShooterEnemy *shooter = shooter_enemies; shooter != NULL; shooter = shooter->next) {
                    al_draw_filled_rectangle(shooter->x - 10, shooter->y - 10, shooter->x + 10, shooter->y + 10, al_map_rgb(0, 0, 255));
                    for (bullet_enemy *index = shooter->shots; index != NULL; index = index->next) {
                        al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(0, 0, 255));
                    }
                }

                // Desenha a pontuação
                score_draw(score);

                if (player_1->gun->timer) player_1->gun->timer--;

                al_flip_display();

            } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.keyboard.keycode == ALLEGRO_KEY_A) { player_1->control->left = 1;  
                } else if (event.keyboard.keycode == ALLEGRO_KEY_D) { player_1->control->right = 1;  
                } else if (event.keyboard.keycode == ALLEGRO_KEY_W) { player_1->control->up = 1;  
                } else if (event.keyboard.keycode == ALLEGRO_KEY_S) { player_1->control->down = 1;  
                } else if (event.keyboard.keycode == ALLEGRO_KEY_C) { player_1->control->fire = 1;  
                }
            } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
                if (event.keyboard.keycode == ALLEGRO_KEY_A) { player_1->control->left = 0;  
                } else if (event.keyboard.keycode == ALLEGRO_KEY_D) { player_1->control->right = 0; 
                } else if (event.keyboard.keycode == ALLEGRO_KEY_W) { player_1->control->up = 0;  
                } else if (event.keyboard.keycode == ALLEGRO_KEY_S) { player_1->control->down = 0;  
                } else if (event.keyboard.keycode == ALLEGRO_KEY_C) { player_1->control->fire = 0;  
                }
            } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                break;
            }
        }
    }

    // Limpeza dos recursos alocados
    destroy_all_enemies(enemies);
    destroy_all_shooter_enemies(shooter_enemies);
    destroy_all_scrap(scrap_list);
    healthbar_destroy(player_1_healthbar);
    score_destroy(score);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    square_destroy(player_1);

    return 0;
}