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
    char* choice[MENU_CHOICE_MAX];        // 메뉴 항목 문자열
    int choice_count;                     // 실제 항목 개수
    int selected;                         // 현재 커서 위치
    int x, y;                             // 메뉴를 그릴 기준 좌표
    bool activated;                       // 메뉴 활성화 여부
    ALLEGRO_COLOR color_selected;        // 선택된 항목 색상
    int choice_spacing;                   // 항목 간 간격
} PAUSE_MENU;

#endif