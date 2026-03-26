#ifndef _CORE_H_
#define _CORE_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

/* --- General --- */
extern long frames;
extern long score;
extern int level;

void must_init(bool test, const char* description);
int between(int lo, int hi);
float between_f(float lo, float hi);
bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);
//작성자 : 박남현
bool collide_circle(int cx1, int cy1, int r1, int cx2, int cy2, int r2);



/* --- Gameplay --- */
// 작성자: 김병헌
typedef enum STATE {
    STATE_MENU,             // 0: 메인 메뉴
    STATE_PLAYING,          // 1: 게임 진행 중
    STATE_PAUSE,            // 2: 일시정지
    STATE_INPUT_NAME,       // 3: 신기록 이름 입력
    STATE_GAMEOVER,         // 4: 게임 오버
    STATE_RANK,             // 5: 랭킹 확인
    STATE_LEVEL_UP,          // 6. 레벨 업햇을때 뜨는 창
    STATE_NEWGAME
} STATE;

typedef struct _RANK
{
    char
}RANK;

// 작성자: 신제현
extern STATE current_state;

void game_state_update(STATE* state, bool* done);
void pause_resume_game(STATE* state);

#endif // !_CORE_H_