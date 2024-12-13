#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
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
#include "BackGround.h"
#include "Explosion.h"
#include "PowerUp.h"
#include "BossShot.h"
#include "Laser.h"
#include "IntroScreen.h"
#include "BossAppearance.h"
#include "TransitionScreen.h"
#include "ControlsScreen.h"
#include "CreditScreen.h"

#define X_SCREEN 800
#define Y_SCREEN 600
#define INITIAL_ENEMY_SPEED 2.0f  
#define MAX_ENEMY_SPEED 10.0f     
#define SPEED_INCREMENT 0.8f    
#define SPAWN_MARGIN 50 

int main() {
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_ttf_addon();
    al_init_font_addon();
    al_init_acodec_addon(); 

    srand(time(NULL));  

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    
    al_init_image_addon();

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
    ALLEGRO_BITMAP* shooter_bullet_sprite_second = NULL;

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

    shooter_bullet_sprite_second = al_load_bitmap("./imagens/tiro_diva2.png");
    if (!shooter_bullet_sprite_second) {
        fprintf(stderr, "Erro ao carregar o sprite do projétil dos inimigos atiradores.\n");
        return -1;
    }

    ALLEGRO_BITMAP* enemy_sprite = al_load_bitmap("./imagens/inimigo_1.png");
    if (!enemy_sprite) {
        fprintf(stderr, "Erro ao carregar o sprite do inimigo.\n");
        return -1;
    }

    ALLEGRO_BITMAP* shooter_enemy_sprite = al_load_bitmap("./imagens/inimigo_f1_tiro.png");
    if (!shooter_enemy_sprite) {
        fprintf(stderr, "Erro ao carregar o sprite do inimigo atirador.\n");
        return -1;
    }

    ALLEGRO_BITMAP* enemy_sprite2 = al_load_bitmap("./imagens/inimigo1_fase2.png");
    if (!enemy_sprite) {
        fprintf(stderr, "Erro ao carregar o sprite do inimigo.\n");
        return -1;
    }

    ALLEGRO_BITMAP* shooter_enemy_sprite2 = al_load_bitmap("./imagens/inimigo_fase2.png");
    if (!shooter_enemy_sprite2) {
        fprintf(stderr, "Erro ao carregar o sprite do inimigo atirador.\n");
        return -1;
    }

    if (!shooter_enemy_sprite) {
        fprintf(stderr, "Erro ao carregar o sprite do inimigo atirador: %d\n", al_get_errno());
        return -1;
    } else {
        printf("Sprite carregado com sucesso!\n");
    }

    ALLEGRO_BITMAP* scrap_sprite = al_load_bitmap("./imagens/scrap.webp");
    if (!scrap_sprite) {
        fprintf(stderr, "Erro ao carregar o sprite de sucata.\n");
        return -1;
    }

    ALLEGRO_BITMAP *explosion_sprite = al_load_bitmap("./imagens/explosion.png");
    if (!explosion_sprite) {
        fprintf(stderr, "Erro ao carregar o sprite da explosão.\n");
        return -1;
    }

    ALLEGRO_BITMAP *boss_sprite = al_load_bitmap("./imagens/boss_limpo.png");
    if (!boss_sprite) {
        fprintf(stderr, "Erro ao carregar o sprite do boss.\n");
        return -1; 
    }

    ALLEGRO_BITMAP *shot1_boss = al_load_bitmap("./imagens/tiroboss2_limpo.png");
    if (!shot1_boss) {
        fprintf(stderr, "Erro ao carregar o sprite do boss.\n");
        return -1;
    }

    ALLEGRO_BITMAP *shot2_boss = al_load_bitmap("./imagens/roundaboud.png");
    if (!shot2_boss) {
        fprintf(stderr, "Erro ao carregar o sprite do boss.\n");
        return -1;
    }

    ALLEGRO_BITMAP *freeze_sprite = al_load_bitmap("./imagens/tiro_gelo.png");
    if (!freeze_sprite) {
        fprintf(stderr, "Erro ao carregar o tiro congelante\n");
        return -1;
    }

    ALLEGRO_BITMAP *boss2_sprite = al_load_bitmap("./imagens/boss2.png");
    if (!boss2_sprite) {
        fprintf(stderr, "Erro ao carregar o sprite do boss 2.\n");
        return -1;
    }

    ALLEGRO_BITMAP *boss2_shot = al_load_bitmap("./imagens/tiro_boss2.png");
    if (!boss2_shot) {
        fprintf(stderr, "Erro ao carregar o tiro do boss 2.\n");
        return -1;
    }

    ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    show_intro_screen(disp, font, queue);

    background_init("./imagens/fundo_space.png");
    display_start_screen(disp, queue, font);

    square* player_1 = square_create(20, 1, 10, Y_SCREEN / 2, X_SCREEN, Y_SCREEN);
    if (!player_1) 
        return 1;
    player_1->hp = 200;

    HealthBar *player_1_healthbar = healthbar_create(10, 10, player_1->hp);
    ShieldBar *player_1_shieldbar = shieldbar_create(10 + HEALTHBAR_WIDTH + 10, 10, player_1->shield->duration);

    Score *score = score_create();
    if (!score) 
        return 2;

    Enemy *enemies = NULL;
    ShooterEnemy* shooter_enemies = NULL;

    float enemy_speed = INITIAL_ENEMY_SPEED;
    int frame_count = 0;

    Laser *laser = laser_create(0, LASER_WIDTH, LASER_SPEED, LASER_DAMAGE);
    bool laser_ready = true;  
    float laser_cooldown_timer = 0; 

    Scrap *scrap_list = NULL;
    int scrap_count = 0; 
    int power_up_stage = 0; 
    int shoot_pattern = 1;
 
    Boss *boss = NULL;
    float boss_appearance_timer = 0.0f; 
    float boss_appearance_duration = 2.0f; 
    bool is_boss_surfacing = false; 

    Explosion *explosions = NULL;
    float delta_time = 0.0;

    ALLEGRO_EVENT event;
    al_start_timer(timer);
    unsigned char p1k = 0;
    bool is_paused = false; 
    bool is_double_speed = false;  
    bool debug_mode = false;

    int enemies_destroyed = 0;

    int game_phase = 1;
    while (1) {
        al_wait_for_event(queue, &event);
        if (p1k)
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 - 40, ALLEGRO_ALIGN_CENTER, "GAME OVER OTARIO");
            al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 - 20, ALLEGRO_ALIGN_CENTER, "Pressione ESPAÇO para sair");
            char score_text[50];
            snprintf(score_text, sizeof(score_text), "Pontuacao Final: %d", score->points);
            al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 + 20, ALLEGRO_ALIGN_CENTER, score_text);
            char enemies_text[50];
            snprintf(enemies_text, sizeof(enemies_text), "Inimigos Destruidos: %d", enemies_destroyed);
            al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 + 40, ALLEGRO_ALIGN_CENTER, enemies_text);
            al_flip_display();
            if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_SPACE) break;
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
        }
        else
        {
            if (game_phase == 1) {
                if (event.type == ALLEGRO_EVENT_KEY_DOWN) {

                    if (event.keyboard.keycode == ALLEGRO_KEY_2) {
                        toggle_game_speed(timer, &is_double_speed);
                        background_update(2.0);
                    }
                    if (event.keyboard.keycode == ALLEGRO_KEY_N) {
                        game_phase = 2;
                    }
                    if (event.keyboard.keycode == ALLEGRO_KEY_P) {
                        is_paused = !is_paused;
                    }
                    if (event.keyboard.keycode == ALLEGRO_KEY_B && boss == NULL) {
                        boss = create_boss(X_SCREEN - 100, Y_SCREEN / 2, boss_sprite);
                        is_boss_surfacing = true;
                        boss_appearance_timer = 0.0f;
                    }
                    if (event.keyboard.keycode == ALLEGRO_KEY_L) { 
                        if (power_up_stage >= 2 && laser_ready) {  
                            if (!laser->is_active) {
                                activate_laser(laser);
                                laser_ready = false;  
                                laser_cooldown_timer = LASER_COOLDOWN;
                            }
                        } 
                    }
                    if (event.keyboard.keycode == ALLEGRO_KEY_E) {
                        if (power_up_stage >= 1) {  
                            shield_activate(player_1->shield);
                        }
                    }
                    if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_H) {
                        debug_mode = !debug_mode;  
                    }
                    if (!is_paused) {
                        if (event.keyboard.keycode == ALLEGRO_KEY_A) { 
                            player_1->control->left = 1;  
                        } else if (event.keyboard.keycode == ALLEGRO_KEY_D) { 
                            player_1->control->right = 1;  
                        } else if (event.keyboard.keycode == ALLEGRO_KEY_W) { 
                            player_1->control->up = 1;  
                        } else if (event.keyboard.keycode == ALLEGRO_KEY_S) { 
                            player_1->control->down = 1;  
                        } else if (event.keyboard.keycode == ALLEGRO_KEY_C) { 
                            player_1->control->fire = 1;
                 
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

                    if (frame_count % 200 == 0) {
                        enemy_speed += SPEED_INCREMENT;
                        if (enemy_speed > MAX_ENEMY_SPEED) {
                            enemy_speed = MAX_ENEMY_SPEED;
                        }
                    }

                    if (enemy_speed == MAX_ENEMY_SPEED && boss == NULL) {
                        boss = create_boss(X_SCREEN - 100, Y_SCREEN / 2, boss_sprite);
                    }

                    if (boss == NULL && rand() % 50 == 0) {
                        float new_x = X_SCREEN;
                        float new_y = SPAWN_MARGIN + rand() % (Y_SCREEN - 2 * SPAWN_MARGIN); 

                        if (!check_collision_between_all_enemies(new_x, new_y, enemies, shooter_enemies)) {
                            Enemy *new_enemy = create_enemy(new_x, new_y, 35.0f, 35.0f, 20); 
                            if (new_enemy) {
                                new_enemy->next = enemies;
                                enemies = new_enemy;
                            }
                        }
                    }

                    if (boss == NULL && rand() % 200 == 0) {  
                    float new_x = X_SCREEN;
                    float new_y = SPAWN_MARGIN + rand() % (Y_SCREEN - 2 * SPAWN_MARGIN);  

                    if (!check_collision_between_all_enemies(new_x, new_y, enemies, shooter_enemies)) {
                        ShooterEnemy *new_enemy = create_shooter_enemy(new_x, new_y, 30);  
                        if (new_enemy) {
                            new_enemy->next = shooter_enemies;
                            shooter_enemies = new_enemy;
                            }
                        }
                    }

                    if (rand() % 150 == 0) {
                        float new_x = X_SCREEN;
                        float new_y = SPAWN_MARGIN + rand() % (Y_SCREEN - 2 * SPAWN_MARGIN);

                        if (!check_collision_with_scrap(new_x, new_y, scrap_list)) {
                            Scrap *new_scrap = create_scrap(new_x, new_y);
                                if (new_scrap) {
                                    new_scrap->next = scrap_list;
                                    scrap_list = new_scrap;
                            }
                        }  
                    }

                    char power_up_message[50] = "";  
                    float power_up_message_timer = 0;

                    if (scrap_count >= 5) {
                        scrap_count = 0; 

                        if (power_up_stage < 3) {
                            power_up_stage++; 

                            switch (power_up_stage) {
                                case 1:
                                    snprintf(power_up_message, sizeof(power_up_message), "ESCUDO CONSTRUIDO! Use 'E' para ativar!");
                                    break;

                                case 2:
                                    snprintf(power_up_message, sizeof(power_up_message), "LASER DISPONIVEL! Use 'L'");
                                    break;

                                case 3:
                                    snprintf(power_up_message, sizeof(power_up_message), "REPARO AUTOMATICO!");
                                    break;
                            }
                        } else { 
                            player_1->hp += 20;  
                            if (player_1->hp > 100) player_1->hp = 100;  
                            snprintf(power_up_message, sizeof(power_up_message), "Reparando Vida!");
                        }

                        power_up_message_timer = 2.0;  
                    }


                    if (power_up_message_timer > 0) {
                        al_draw_text(font, al_map_rgb(255, 255, 0), X_SCREEN / 2, 50, ALLEGRO_ALIGN_CENTER, power_up_message);
                        power_up_message_timer -= 1.0 / 30.0; 
                    }

                    if (!laser_ready) {
                        laser_cooldown_timer -= 1.0 / 30.0;  
                        if (laser_cooldown_timer <= 0) {
                            laser_ready = true;  
                            laser_cooldown_timer = 0;  
                        }
                    }

                    ShooterEnemy *current_shooter = shooter_enemies;
                    while (current_shooter != NULL) {
                        if (frame_count % 100 == 0) {
                            shooter_enemy_shoot(current_shooter);
                        }
                        current_shooter = current_shooter->next;
                    }

                    if (boss != NULL) {
                        update_boss(boss, player_1, is_paused);
                    }

                    delta_time = 5.0 / 30.0;

                    update_position(player_1);
                    update_bullets(player_1);
                    update_enemies(&enemies, enemy_speed);
                    update_shooter_enemy(&shooter_enemies);
                    update_explosions(&explosions, delta_time);
                    shield_update(player_1->shield, al_get_time());
                    shieldbar_update(player_1_shieldbar, player_1->shield->is_active ? player_1->shield->duration - (al_get_time() - player_1->shield->start_time) : 0);
                    update_boss(boss,player_1, is_paused);
                    shoot_pattern = update_boss_shooting(boss, frame_count, shoot_pattern, shot1_boss, shot2_boss);
                    laser_update(laser);

                    move_shooter_bullets(shooter_enemies, player_1);
                    move_scrap(scrap_list, enemy_speed);

                    check_collision_with_player(player_1, &enemies);
                    check_kill(player_1, &enemies, score, &explosions, &enemies_destroyed);
                    check_kill_shooter_enemies(player_1, &shooter_enemies, score, &explosions,&enemies_destroyed);
                    check_collision_with_player_shooter_enemy(player_1, &shooter_enemies);
                    check_player_bullets_with_boss(player_1, boss);
                    laser_check_collision_with_enemies(laser, &enemies, score);
                    laser_check_collision_with_boss(laser, boss);
                    check_boss_bullets_with_player(player_1, boss);
                    laser_check_collision_with_shooter_enemies(laser, &shooter_enemies, score);

                    if (boss != NULL) {
                        check_boss_collision_with_player(player_1, boss);
                    }

                    healthbar_update(player_1_healthbar, player_1->hp);
                    p1k = (player_1->hp <= 0) ? 1 : 0;
                }

                if (!is_paused) {
                    if (boss != NULL) {
                    if (frame_count % 150 == 0) {
                        shoot_pattern = (shoot_pattern == 1) ? 2 : 1;
                    }

                    if (shoot_pattern == 1 && boss->cooldown == 0) {
                        boss_shoot(boss, shot1_boss, shot2_boss);
                    } else if (shoot_pattern == 2 && boss->cooldown == 0) {
                        boss_shoot(boss, shot2_boss, shot1_boss);
                    }

                    update_boss_shots(boss, is_paused);

                    boss_shot *current_shot = boss->shots;
                    while (current_shot != NULL) {
                        boss_shot_draw(current_shot, current_shot->sprite);
                        current_shot = current_shot->next;
                    }

                    check_player_bullets_with_boss_shots(player_1, boss);

                    }
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
                            if (current_scrap != NULL) {
                                destroy_scrap(current_scrap);
                                current_scrap = NULL;  
                            }
                            current_scrap = prev_scrap->next;
                        } else {
                            scrap_list = current_scrap->next;
                            if (current_scrap != NULL) {
                                destroy_scrap(current_scrap);
                                current_scrap = NULL;  
                            }
                            current_scrap = scrap_list;
                        }
                    } else {
                        prev_scrap = current_scrap;
                        current_scrap = current_scrap->next;
                    }
                }

                al_clear_to_color(al_map_rgb(0, 0, 0));

                background_draw();
                healthbar_draw(player_1_healthbar);
                shield_draw(player_1->shield, player_1->x, player_1->y, player_1->side / 2);
                shield_draw_bar(player_1->shield, font, 120, 10, 200, 20);

                char scrap_text[50];
                snprintf(scrap_text, 50, "Sucata: %d", scrap_count);
                al_draw_text(font, al_map_rgb(255, 255, 0), 10, 40, 0, scrap_text);

                draw_shield_timer(player_1->shield, font, 330, 15, is_paused);
                square_draw(player_1, spaceship_image, debug_mode);
                
                draw_enemies(enemies, enemy_sprite, debug_mode);

                draw_scrap(scrap_list, scrap_sprite);

                draw_explosions(explosions, explosion_sprite);

                laser_draw(laser);


                for (bullet *index = player_1->gun->shots; index != NULL; index = index->next) {
                    al_draw_bitmap(bullet_sprite, index->x - al_get_bitmap_width(bullet_sprite) / 2, 
                                index->y - al_get_bitmap_height(bullet_sprite) / 2, 0);
                }

                draw_shooter_enemies(shooter_enemies, shooter_enemy_sprite, debug_mode);

                for (ShooterEnemy *current = shooter_enemies; current != NULL; current = current->next) {
                    for (bullet_enemy *bullet = current->shots; bullet != NULL; bullet = bullet->next) {
                        al_draw_bitmap(shooter_bullet_sprite,
                                    bullet->x - al_get_bitmap_width(shooter_bullet_sprite) / 2,
                                    bullet->y - al_get_bitmap_height(shooter_bullet_sprite) / 2,
                                    0);
                    }
                }

                if (is_boss_surfacing) {
                    boss_appearance_timer += delta_time;

                    draw_boss_appearance(boss_sprite, boss_appearance_timer / boss_appearance_duration, X_SCREEN, Y_SCREEN);

                    if (boss_appearance_timer >= boss_appearance_duration) {
                        is_boss_surfacing = false; 
                    }
                } else if (boss != NULL) {
                
                    draw_boss(boss, debug_mode);
                    update_boss(boss, player_1, is_paused);
                }


                score_draw(score);

                if (player_1->gun->timer && !is_paused) player_1->gun->timer--;

                if (is_paused) {
                     al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, "JOGO PAUSADO");
                    al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 + 20, ALLEGRO_ALIGN_CENTER, "Pressione 'P' para continuar");
                }

                draw_laser_cooldown_bar(laser_cooldown_timer, LASER_COOLDOWN, X_SCREEN, Y_SCREEN);

               if (boss != NULL && boss->hp <= 0) {

                if (boss->shots != NULL) {
                    boss_shot *current_shot = boss->shots;
                    while (current_shot != NULL) {
                        boss_shot *next_shot = current_shot->next;
                        boss_shot_destroy(current_shot); 
                        current_shot = next_shot;
                    }
                    boss->shots = NULL; 
                }

                destroy_boss(boss); 
                boss = NULL; 

                game_phase = 2;
               }

                al_flip_display();
            } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                break;
            } 
        } else if (game_phase == 2) {


            static TransitionScreen transition_screen;
            static bool transition_initialized = false;
            static bool phase_2_initialized = false;  
            static bool is_reset_timer_active = false; // modificação
            static float reset_timer = 0.0f;  // modificação

            if (!transition_initialized) {
                transition_init(&transition_screen, font, 5.0f); 
                transition_initialized = true;
            }

            bool is_transition_active = transition_update(&transition_screen, 1.0f / 30.0f);

            if (is_transition_active) {
                transition_draw(&transition_screen);
                al_flip_display();
            } else {
                // linha da modificação
                if (!phase_2_initialized || (is_reset_timer_active && reset_timer <= 0.0f)) {    

                while (enemies != NULL) {
                    Enemy *temp = enemies;
                    enemies = enemies->next;
                    destroy_enemy(temp); 
                }

                while (shooter_enemies != NULL) {
                    ShooterEnemy *temp = shooter_enemies;
                    shooter_enemies = shooter_enemies->next;
                    destroy_shooter_enemy(temp); 
                }

                while (scrap_list != NULL) {
                    Scrap *temp = scrap_list;
                    scrap_list = scrap_list->next;
                    destroy_scrap(temp); 
                }
                player_1->x = 50;                
                player_1->y = Y_SCREEN / 2;     
                player_1->hp = 200;

                player_1->control->right = 0;
                player_1->control->left = 0;
                player_1->control->up = 0;
                player_1->control->down = 0;
                player_1->control->fire = 0;

                enemy_speed = INITIAL_ENEMY_SPEED + 1.5f;

                background_init("./imagens/fundo2temp.png");

                phase_2_initialized = true;
                is_reset_timer_active = false;
                reset_timer = 0.0f;
                }

            // nova modificação
            if (player_1->hp <= 0 && !is_reset_timer_active) {
                is_reset_timer_active = true; 
                reset_timer = 5.0f;  
            }

            if (is_reset_timer_active) {
                reset_timer -= 1.0f / 30.0f;  
                al_clear_to_color(al_map_rgb(0, 0, 0)); 

                al_draw_textf(
                    font, al_map_rgb(255, 0, 0), X_SCREEN / 2, Y_SCREEN / 2,
                    ALLEGRO_ALIGN_CENTER,
                    "Fase sera reiniciada em %.1f segundos", reset_timer
                );

                al_flip_display();  
                continue;  
            }
            // acaba a modificação

            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {

                    if (event.keyboard.keycode == ALLEGRO_KEY_2) {
                        toggle_game_speed(timer, &is_double_speed);
                        background_update(2.0);
                    }
                    if (event.keyboard.keycode == ALLEGRO_KEY_P) {
                        is_paused = !is_paused;
                    }
                    if (event.keyboard.keycode == ALLEGRO_KEY_B && boss == NULL) {
                        boss = create_boss(X_SCREEN - 100, Y_SCREEN / 2, boss2_sprite);
                        is_boss_surfacing = true;
                        boss_appearance_timer = 0.0f;
                    }
                    if (event.keyboard.keycode == ALLEGRO_KEY_L) { 
                        if (power_up_stage >= 2 && laser_ready) {  
                            if (!laser->is_active) {
                                activate_laser(laser);
                                laser_ready = false;  
                                laser_cooldown_timer = LASER_COOLDOWN;
                            }
                        } 
                    }
                    if (event.keyboard.keycode == ALLEGRO_KEY_E) {
                        if (power_up_stage >= 1) {  
                            shield_activate(player_1->shield);
                        } 
                    }
                    if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_H) {
                        debug_mode = !debug_mode;  
                    }
                    if (!is_paused) {
                        if (event.keyboard.keycode == ALLEGRO_KEY_A) { 
                            player_1->control->left = 1;  
                        } else if (event.keyboard.keycode == ALLEGRO_KEY_D) { 
                            player_1->control->right = 1;  
                        } else if (event.keyboard.keycode == ALLEGRO_KEY_W) { 
                            player_1->control->up = 1;  
                        } else if (event.keyboard.keycode == ALLEGRO_KEY_S) { 
                            player_1->control->down = 1;  
                        } else if (event.keyboard.keycode == ALLEGRO_KEY_C) { 
                            player_1->control->fire = 1;
                 
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

                    if (frame_count % 200 == 0) {
                        enemy_speed += SPEED_INCREMENT;
                        if (enemy_speed > MAX_ENEMY_SPEED) {
                            enemy_speed = MAX_ENEMY_SPEED;
                        }
                    }

                    if (enemy_speed == MAX_ENEMY_SPEED && boss == NULL) {
                        boss = create_boss(X_SCREEN - 100, Y_SCREEN / 2, boss2_sprite);
                    }

                    if (player_1->is_frozen) {
                        player_1->control->up = 0;
                        player_1->control->down = 0;
                        player_1->control->left = 0;
                        player_1->control->right = 0;
                    }


                    if (boss == NULL && rand() % 30 == 0) {
                        float new_x = X_SCREEN;
                        float new_y = SPAWN_MARGIN + rand() % (Y_SCREEN - 2 * SPAWN_MARGIN); 

                        if (!check_collision_between_all_enemies(new_x, new_y, enemies, shooter_enemies)) {
                            Enemy *new_enemy = create_enemy(new_x, new_y, 35.0f, 35.0f, 20); 
                            if (new_enemy) {
                                new_enemy->next = enemies;
                                enemies = new_enemy;
                            }
                        }
                    }

                    if (boss == NULL && rand() % 90 == 0) {  
                    float new_x = X_SCREEN;
                    float new_y = SPAWN_MARGIN + rand() % (Y_SCREEN - 2 * SPAWN_MARGIN);  

                    if (!check_collision_between_all_enemies(new_x, new_y, enemies, shooter_enemies)) {
                        ShooterEnemy *new_enemy = create_shooter_enemy(new_x, new_y, 30);  
                        if (new_enemy) {
                            new_enemy->next = shooter_enemies;
                            shooter_enemies = new_enemy;
                            }
                        }
                    }

                    if (rand() % 100 == 0) {
                        float new_x = X_SCREEN;
                        float new_y = SPAWN_MARGIN + rand() % (Y_SCREEN - 2 * SPAWN_MARGIN);

                        if (!check_collision_with_scrap(new_x, new_y, scrap_list)) {
                            Scrap *new_scrap = create_scrap(new_x, new_y);
                                if (new_scrap) {
                                    new_scrap->next = scrap_list;
                                    scrap_list = new_scrap;
                            }
                        }  
                    }

                    char power_up_message[50] = "";  
                    float power_up_message_timer = 0;

                    if (scrap_count >= 5) {
                        scrap_count = 0; 

                        if (power_up_stage < 3) {
                            power_up_stage++; 

                            switch (power_up_stage) {
                                case 1:
                                    snprintf(power_up_message, sizeof(power_up_message), "ESCUDO CONSTRUIDO! Use 'E' para ativar!");
                                    break;

                                case 2:
                                    snprintf(power_up_message, sizeof(power_up_message), "LASER DISPONIVEL! Use 'L'");
                                    break;

                                case 3:
                                    snprintf(power_up_message, sizeof(power_up_message), "REPARO AUTOMATICO!");
                                    break;
                            }
                        } else { 
                            player_1->hp += 20;  
                            if (player_1->hp > 100) player_1->hp = 100;  
                            snprintf(power_up_message, sizeof(power_up_message), "Reparando Vida!");
                        }

                        power_up_message_timer = 2.0;  
                    }

                    if (power_up_message_timer > 0) {
                        al_draw_text(font, al_map_rgb(255, 255, 0), X_SCREEN / 2, 50, ALLEGRO_ALIGN_CENTER, power_up_message);
                        power_up_message_timer -= 1.0 / 30.0; 
                    }

                    if (!laser_ready) {
                        laser_cooldown_timer -= 1.0 / 30.0;  
                        if (laser_cooldown_timer <= 0) {
                            laser_ready = true;  
                            laser_cooldown_timer = 0;  
                        }
                    }

                    ShooterEnemy *current_shooter = shooter_enemies;
                    while (current_shooter != NULL) {
                        if (frame_count % 100 == 0) {
                            shooter_enemy_shoot(current_shooter);
                        }
                        current_shooter = current_shooter->next;
                    }


                    delta_time = 5.0 / 30.0;

                    update_position(player_1);
                    update_bullets(player_1);
                    update_enemies(&enemies, enemy_speed);
                    update_shooter_enemy(&shooter_enemies);
                    update_explosions(&explosions, delta_time);
                    shield_update(player_1->shield, al_get_time());
                    shieldbar_update(player_1_shieldbar, player_1->shield->is_active ? player_1->shield->duration - (al_get_time() - player_1->shield->start_time) : 0);
                    update_boss(boss,player_1, is_paused);
                    shoot_pattern = update_boss_shooting(boss, frame_count, shoot_pattern, shot1_boss, shot2_boss);
                    laser_update(laser);
                    move_shooter_bullets(shooter_enemies, player_1);
                    move_scrap(scrap_list, enemy_speed);

                    check_collision_with_player(player_1, &enemies);
                    check_kill(player_1, &enemies, score, &explosions, &enemies_destroyed);
                    check_kill_shooter_enemies(player_1, &shooter_enemies, score, &explosions,&enemies_destroyed);
                    check_player_bullets_with_boss(player_1, boss);
                    check_collision_with_player_shooter_enemy(player_1, &shooter_enemies);
                    laser_check_collision_with_boss(laser, boss);
                    check_boss_bullets_with_player(player_1, boss);
                    laser_check_collision_with_enemies(laser, &enemies,score);
                    laser_check_collision_with_shooter_enemies(laser, &shooter_enemies,score);

                    if (boss != NULL) {
                        check_boss_collision_with_player(player_1, boss);
                    };

                    healthbar_update(player_1_healthbar, player_1->hp);
                    
                }

                 if (boss != NULL) {
                if (frame_count % 150 == 0) {
                    shoot_pattern = (shoot_pattern == 1) ? 2 : 1;
                }

                if (shoot_pattern == 1 && boss->cooldown == 0) {
                    boss_shoot(boss, boss2_shot, freeze_sprite);
                } else if (shoot_pattern == 2 && boss->cooldown == 0) {
                    boss_shoot(boss, boss2_shot, freeze_sprite);
                }

                update_boss_shots(boss, is_paused);

                boss_shot *current_shot = boss->shots;
                boss_shot *prev_shot = NULL;

                        while (current_shot != NULL) {
                            if (check_collision(
                                    current_shot->x, current_shot->y, 10, 10, 
                                    player_1->x - player_1->side / 2, player_1->y - player_1->side / 2, player_1->side, player_1->side)) {

                                if (current_shot->sprite == freeze_sprite) {
                                    player_1->is_frozen = true;
                                    player_1->freeze_timer = 120; 
                                } else { 
                                player_1->hp -= current_shot->damage;
                            }

                            if (prev_shot == NULL) {
                                boss->shots = current_shot->next;
                            } else {
                                prev_shot->next = current_shot->next;
                            }

                            boss_shot *to_remove = current_shot;
                            current_shot = current_shot->next;
                            boss_shot_destroy(to_remove);
                        } else {
                            prev_shot = current_shot;
                            current_shot = current_shot->next;
                        }
                    }

                    boss_shot *draw_shot = boss->shots;
                    while (draw_shot != NULL) {
                        boss_shot_draw(draw_shot, draw_shot->sprite);
                        draw_shot = draw_shot->next;
                    }

                    if (player_1->is_frozen) {
                        player_1->freeze_timer--;
                        if (player_1->freeze_timer <= 0) {
                            player_1->is_frozen = false; 
                        }
                    } else {
                        update_position(player_1);
                    }
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
                            if (current_scrap != NULL) {
                                destroy_scrap(current_scrap);
                                current_scrap = NULL;  
                            }
                            current_scrap = prev_scrap->next;
                        } else {
                            scrap_list = current_scrap->next;
                            if (current_scrap != NULL) {
                                destroy_scrap(current_scrap);
                                current_scrap = NULL;  
                            }
                            current_scrap = scrap_list;
                        }
                    } else {
                        prev_scrap = current_scrap;
                        current_scrap = current_scrap->next;
                    }
                }

                al_clear_to_color(al_map_rgb(0, 0, 0));


                background_draw();
                healthbar_draw(player_1_healthbar);
                shield_draw(player_1->shield, player_1->x, player_1->y, player_1->side / 2);
                shield_draw_bar(player_1->shield, font, 120, 10, 200, 20);

                char scrap_text[50];
                snprintf(scrap_text, 50, "Sucata: %d", scrap_count);
                al_draw_text(font, al_map_rgb(255, 255, 0), 10, 40, 0, scrap_text);

                draw_shield_timer(player_1->shield, font, 330, 15, is_paused);

                square_draw(player_1, spaceship_image, debug_mode);
                
                draw_enemies(enemies, enemy_sprite2, debug_mode);

                draw_scrap(scrap_list, scrap_sprite);

                draw_explosions(explosions, explosion_sprite);

                laser_draw(laser);

                for (bullet *index = player_1->gun->shots; index != NULL; index = index->next) {
                    al_draw_bitmap(bullet_sprite, index->x - al_get_bitmap_width(bullet_sprite) / 2, 
                                index->y - al_get_bitmap_height(bullet_sprite) / 2, 0);
                }

                draw_shooter_enemies(shooter_enemies, shooter_enemy_sprite2, debug_mode);

                for (ShooterEnemy *current = shooter_enemies; current != NULL; current = current->next) {
                    for (bullet_enemy *bullet = current->shots; bullet != NULL; bullet = bullet->next) {
                        al_draw_bitmap(shooter_bullet_sprite_second,
                                    bullet->x - al_get_bitmap_width(shooter_bullet_sprite_second) / 2,
                                    bullet->y - al_get_bitmap_height(shooter_bullet_sprite_second) / 2,
                                    0);
                    }
                }
                    if (player_1->is_frozen) {
                    ALLEGRO_COLOR freeze_color = al_map_rgba_f(0.5, 0.8, 1.0, 0.5);

                    al_draw_filled_circle(
                        player_1->x, 
                        player_1->y, 
                        player_1->side,
                        freeze_color
                    );

                    
                    al_draw_circle(
                        player_1->x,
                        player_1->y,
                        player_1->side + 5, 
                        al_map_rgba_f(0.3, 0.7, 1.0, 0.8), 
                        2.0 
                    );
                }

                if (is_boss_surfacing) {
                    boss_appearance_timer += delta_time;

                    draw_boss_appearance(boss2_sprite, boss_appearance_timer / boss_appearance_duration, X_SCREEN, Y_SCREEN);

                    if (boss_appearance_timer >= boss_appearance_duration) {
                        is_boss_surfacing = false; 
                    }
                } else if (boss != NULL) {
                
                    draw_boss(boss, debug_mode);
                    update_boss(boss, player_1, is_paused);
                }

                score_draw(score);

                if (player_1->gun->timer && !is_paused) player_1->gun->timer--;

                if (is_paused) {
                     al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, "JOGO PAUSADO");
                    al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 + 20, ALLEGRO_ALIGN_CENTER, "Pressione 'P' para continuar");
                }

                draw_laser_cooldown_bar(laser_cooldown_timer, LASER_COOLDOWN, X_SCREEN, Y_SCREEN);


                if (boss != NULL && boss->hp <= 0) {

                    if (boss->shots != NULL) {
                        boss_shot *current_shot = boss->shots;
                        while (current_shot != NULL) {
                            boss_shot *next_shot = current_shot->next;
                            boss_shot_destroy(current_shot); 
                            current_shot = next_shot;
                        }
                        boss->shots = NULL;
                    }

                    destroy_boss(boss); 
                    boss = NULL; 

                    while (1) {
                        al_clear_to_color(al_map_rgb(0, 0, 0));

                        al_draw_text(font, al_map_rgb(0, 255, 0), X_SCREEN / 2, Y_SCREEN / 2 - 80, ALLEGRO_ALIGN_CENTER, "PARABeNS, DESTRUIU A TROPA DO MALWARE!");

                        al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 - 40, ALLEGRO_ALIGN_CENTER, "Fim de Jogo");
                        al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN / 2, Y_SCREEN / 2 - 20, ALLEGRO_ALIGN_CENTER, "Pressione ESQ para sair");

                        char score_text[50];
                        snprintf(score_text, sizeof(score_text), "Pontuacao Final: %d", score->points);
                        al_draw_text(font, al_map_rgb(255, 255, 0), X_SCREEN / 2, Y_SCREEN / 2 + 40, ALLEGRO_ALIGN_CENTER, score_text);

                        char enemies_text[50];
                        snprintf(enemies_text, sizeof(enemies_text), "Inimigos Derrotados: %d", enemies_destroyed);
                        al_draw_text(font, al_map_rgb(255, 255, 0), X_SCREEN / 2, Y_SCREEN / 2 + 60, ALLEGRO_ALIGN_CENTER, enemies_text);

                        al_flip_display();

                        ALLEGRO_EVENT end_event;
                        al_wait_for_event(queue, &end_event);

                        if (end_event.type == ALLEGRO_EVENT_KEY_DOWN) {
                            if (end_event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                                exit(0); 
                            }
                        }

                        if (end_event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                            exit(0); 
                        }
                    }
                }
                    al_flip_display();
                    } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                            break;
                    } 
                }  
            }
        }
    }

    background_destroy();
    destroy_all_enemies(&enemies);
    destroy_all_shooter_enemies(shooter_enemies);
    if (shooter_enemy_sprite) {
        al_destroy_bitmap(shooter_enemy_sprite);
        shooter_enemy_sprite = NULL;  
    }
    if (shooter_bullet_sprite) 
        al_destroy_bitmap(shooter_bullet_sprite);

    if (shooter_bullet_sprite_second) 
        al_destroy_bitmap(shooter_bullet_sprite_second);

    destroy_all_scrap(scrap_list);
    healthbar_destroy(player_1_healthbar);
    score_destroy(score);
    al_destroy_font(font);
    al_destroy_display(disp);
    if (scrap_sprite != NULL) {
        al_destroy_bitmap(scrap_sprite);
    }
    al_destroy_bitmap(enemy_sprite);
    al_destroy_timer(timer);
    al_destroy_bitmap(spaceship_image);
    al_destroy_event_queue(queue);
    al_destroy_bitmap(boss_sprite);
    al_destroy_bitmap(boss2_sprite);
    joystick_destroy(player_1->control);
    shieldbar_destroy(player_1_shieldbar);


    if (player_1->shield) {
        shield_destroy(player_1->shield);
        player_1->shield = NULL; 
    }

    square_destroy(player_1);

    return 0;
}
