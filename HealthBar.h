#ifndef __HEALTHBAR__  
#define __HEALTHBAR__  

#define HEALTHBAR_WIDTH 100
#define HEALTHBAR_HEIGHT 10
#define HEALTHBAR_COLOR al_map_rgb(0, 255, 0) // Cor verde para a barra de vida

typedef struct {
    float x;  
    float y;  
    unsigned char max_hp;  
    unsigned char current_hp;  
} HealthBar;

HealthBar* healthbar_create(float x, float y, unsigned char max_hp);
void healthbar_update(HealthBar *bar, unsigned char current_hp);
void healthbar_draw(HealthBar *bar);
void healthbar_destroy(HealthBar *bar);

#endif 
