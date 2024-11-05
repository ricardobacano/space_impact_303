#ifndef __SCORE__  
#define __SCORE__ 

typedef struct {
    int points;  
} Score;

Score* score_create();  
void score_increment(Score *score, int value);  
void score_draw(Score *score);  
void score_destroy(Score *score);  

#endif 
