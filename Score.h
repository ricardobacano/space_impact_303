#ifndef __SCORE__  
#define __SCORE__ 

typedef struct {
    int points;  
} Score;

Score* score_create();  

// incrementa a quantidade de score
void score_increment(Score *score, int value);  

// desenha o score na tela 
void score_draw(Score *score); 

// destroi a lista de score 
void score_destroy(Score *score);  

#endif 
