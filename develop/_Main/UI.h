#ifndef _UI_H_
#define _UI_H_

#include "Display.h"
#include "Core.h"

/* --- stars --- */

typedef struct STAR
{
    float y;
    float speed;
} STAR;

#define STARS_N ((BUFFER_W / 2) - 1)
extern STAR stars[STARS_N];

void stars_init();
void stars_update();
void stars_draw();


/* --- hud --- */

extern ALLEGRO_FONT* font;
extern long score_display;
void hud_init();
void hud_deinit();
void hud_update();
void hud_draw();

/* --- UI --- */

#define MENU_CHOICE_MAX     (2)

typedef struct pause_menu
{
    // 스프라이트(비트맵으로 쓸) 요소
    ALLEGRO_BITMAP* _sheet;                     // 메뉴 스프라이트 시트
    ALLEGRO_BITMAP* bg;                         // 백그라운드 화면
    ALLEGRO_BITMAP* panel;                      // 메뉴 창 틀
    ALLEGRO_BITMAP* button[MENU_CHOICE_MAX];    // 버튼 이미지(항목별)
    ALLEGRO_BITMAP* cursor;                     // 선택 커서

    // 메뉴 항목
    const char* choices[MENU_CHOICE_MAX];       // 항목 문자열
    int choice_count;                           // 항목 개수
    int selected;                               // 현재 커서의 위치

    // 위치 및 스타일
    int x, y;                                   // 메뉴 기준 좌표
    float choice_spacing;                       // 항목 간 간격

    bool activated;                             // 메뉴 활성화 여부
} PAUSE_MENU;

extern PAUSE_MENU pause_menu;

void pause_menu_init(void);
void pause_menu_deinit(void);

#endif