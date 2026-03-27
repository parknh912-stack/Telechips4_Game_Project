#ifndef _UI_H_
#define _UI_H_

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "../Display.h"
#include "../Core.h"

// --- UI 이미지 좌표 (uipack_rpg_sheet.xml 기준) ---
#define UI_PANEL_BLUE_X 190
#define UI_PANEL_BLUE_Y 0
#define UI_PANEL_W      100
#define UI_PANEL_H      100

#define UI_BTN_BLUE_X   0
#define UI_BTN_BLUE_Y   188
#define UI_BTN_W        190
#define UI_BTN_H        49

#define UI_BTN_BLUE_P_X 0
#define UI_BTN_BLUE_P_Y 143
#define UI_BTN_P_H      45

//0327 김병헌 : 상수 사용을 줄이기 위함
#define UI_BTN_POS_Y_LOW    -80 
#define UI_BTN_POS_Y_MID    50
#define UI_BTN_POS_Y_HI     120

#define UI_PANEL_SIZE_W     350
#define UI_PANEL_SIZE_W_L   550
#define UI_PANEL_SIZE_H_M   270
#define UI_PANEL_SIZE_H_L   450
//0327 김병헌 : 상수 사용을 줄이기 위함

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
extern ALLEGRO_BITMAP* ui_sheet;
extern int current_menu_selection;

// 작성자: 신제현
typedef struct MENU {
    float x;
    float y;
    float width;
    float height;
    const char* items[10];
    int item_count;
    int selected;
} MENU;

// 작성자: 김병헌
void ui_init();
void ui_deinit();
void draw_ui_element(int sx, int sy, int sw, int sh, float dx, float dy, float dw, float dh);
void draw_menu_ui(MENU* m, const char* title, int button_y, float wanted_with, float wanted_height);
void menu_input_update(int item_count);

void ui_draw_main_menu();
void ui_draw_pause_menu();
void ui_draw_gameover_menu();
void ui_draw_rank_menu();
void ui_draw_input_name_menu();

// 작성자: 신제현
void ui_draw_level_up_menu(void);
#endif