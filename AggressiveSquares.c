#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "Square.h"
#include "HealthBar.h"
#include "StartScreen.h"
#include "Enemy.h"  
#include "Score.h"  
#include "ShooterEnemy.h"

#define X_SCREEN 800
#define Y_SCREEN 600
#define INITIAL_ENEMY_SPEED 2.0f  // Velocidade inicial dos inimigos
#define MAX_ENEMY_SPEED 10.0f     // Velocidade máxima dos inimigos
#define SPEED_INCREMENT 0.5f      // Aumento de velocidade a cada período

float enemy_speed = INITIAL_ENEMY_SPEED;
int frame_count = 0;

int main() {
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();

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
    player_1->hp = 200;

    HealthBar *player_1_healthbar = healthbar_create(10, 10, player_1->hp);

    Score *score = score_create();
    if (!score) return 2;

    Enemy *enemies = NULL;
    ShooterEnemy *shooter_enemies = NULL;

    ALLEGRO_EVENT event;
    al_start_timer(timer);
    unsigned char p1k = 0;

    while (1) {
    al_wait_for_event(queue, &event);

    if (p1k) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, "GAME OVER");
        al_flip_display();
        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_SPACE) break;
    } else {
        if (event.type == ALLEGRO_EVENT_TIMER) {
            frame_count++;

            if (frame_count % 600 == 0) {
                enemy_speed += SPEED_INCREMENT;
                if (enemy_speed > MAX_ENEMY_SPEED) {
                    enemy_speed = MAX_ENEMY_SPEED;
                }
            }

            if (rand() % 50 == 0) {
                Enemy *new_enemy = create_enemy(X_SCREEN, rand() % Y_SCREEN, 10);
                if (new_enemy) {
                    new_enemy->next = enemies;
                    enemies = new_enemy;
                }
            }

            if (rand() % 150 == 0) {
                ShooterEnemy *new_shooter = create_shooter_enemy(X_SCREEN, rand() % Y_SCREEN, 15);
                if (new_shooter) {
                    new_shooter->next = shooter_enemies;
                    shooter_enemies = new_shooter;
                }
            }

            update_position(player_1);
            update_bullets(player_1);
            update_enemies(&enemies, enemy_speed);
            update_shooter_enemy(&shooter_enemies);
            move_shooter_bullets(shooter_enemies, player_1); 
            check_collision_with_player(player_1, &enemies);

            check_kill(player_1, &enemies, score);
            check_kill_shooter_enemies(player_1, &shooter_enemies, score);

            p1k = (player_1->hp <= 0) ? 1 : 0;

            healthbar_update(player_1_healthbar, player_1->hp);

            al_clear_to_color(al_map_rgb(0, 0, 0));
            healthbar_draw(player_1_healthbar);

            al_draw_filled_rectangle(player_1->x - player_1->side / 2, player_1->y - player_1->side / 2,
                                     player_1->x + player_1->side / 2, player_1->y + player_1->side / 2,
                                     al_map_rgb(255, 0, 0));

            draw_enemies(enemies);

            for (bullet *index = player_1->gun->shots; index != NULL; index = index->next) {
                al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(255, 0, 0));
            }

                for (ShooterEnemy *shooter = shooter_enemies; shooter != NULL; shooter = shooter->next) {
                    al_draw_filled_rectangle(shooter->x - 10, shooter->y - 10, shooter->x + 10, shooter->y + 10, al_map_rgb(0, 0, 255));
                    for (bullet *index = shooter->shots; index != NULL; index = index->next) {
                        al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(0, 0, 255));
                    }
                }

                score_draw(score);

                if (player_1->gun->timer) player_1->gun->timer--;

                al_flip_display();
            } else if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP) {
                if (event.keyboard.keycode == 1) joystick_left(player_1->control);
                else if (event.keyboard.keycode == 4) joystick_right(player_1->control);
                else if (event.keyboard.keycode == 23) joystick_up(player_1->control);
                else if (event.keyboard.keycode == 19) joystick_down(player_1->control);
                else if (event.keyboard.keycode == 3) joystick_fire(player_1->control);
            } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
        }
    }

    destroy_all_enemies(enemies);
    destroy_all_shooter_enemies(shooter_enemies);
    healthbar_destroy(player_1_healthbar);
    score_destroy(score);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    square_destroy(player_1);

    return 0;
}