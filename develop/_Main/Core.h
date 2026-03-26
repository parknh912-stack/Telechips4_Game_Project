#ifndef _CORE_H_
#define _CORE_H_

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

/* --- General --- */
extern long frames;
extern long score;
extern int level;			// 레벨을 관리하는 변수 추가

typedef enum STATE {
    STATE_MENU,          // 0: 게임 시작 전 메인 메뉴 화면 상태
    STATE_PLAYING,       // 1: 실제 게임이 진행 중인 상태
    STATE_PAUSE,         // 2: 게임 플레이 중 ESC 키를 눌러 일시정지된 상태
    STATE_INPUT_NAME,    // 3: 게임 오버 후 신기록 달성 시 이름을 입력받는 화면 상태
    STATE_GAMEOVER,      // 4: 게임 오버 후 재시작, 랭킹, 메뉴로 갈지 선택하는 화면 상태
    STATE_RANK           // 5: 현재 저장된 랭킹 목록을 보여주는 화면 상태
} STATE;

void must_init(bool test, const char* description);
int between(int lo, int hi);
float between_f(float lo, float hi);
bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);

/* --- Gameplay --- */
void game_state_update(void* paused);
void pause_resume_game(void* paused, void* frames);

#endif // !_CORE_H_

