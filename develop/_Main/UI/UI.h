#ifndef _UI_H_
#define _UI_H_

#include <allegro5/allegro5.h>
#include "../Display.h"
#include "../Core.h"

//0328 김병헌 : 매직넘버 수정을 위한 작업이 있었습니다.
/* -- Asset Location -- */
#define UI_PANEL_BLUE_X            190 
#define UI_PANEL_BLUE_Y            0
#define UI_PANEL_W                 100
#define UI_PANEL_H                 100

#define UI_BTN_BLUE_X              0
#define UI_BTN_BLUE_Y              188
#define UI_BTN_W                   190
#define UI_BTN_H                   49

#define UI_BTN_BLUE_P_X            0
#define UI_BTN_BLUE_P_Y            143
#define UI_BTN_P_H                 45

/* -- UI Button Position -- */
#define UI_BTN_POS_Y_LOW           -80 
#define UI_BTN_POS_Y_MID           50
#define UI_BTN_POS_Y_HI            120

/* -- UI Panel size -- */
#define UI_PANEL_SIZE_W            400
#define UI_PANEL_SIZE_W_L          550
#define UI_PANEL_SIZE_W_VL         1000
#define UI_PANEL_SIZE_H_M          270
#define UI_PANEL_SIZE_H_L          450

/* -- FONT -- */
#define FONT_SIZE_NORMAL           15
#define FONT_SIZE_TITLE            22

/* -- STAR(Game Background) -- */
#define STAR_SPEED_MIN             0.1f
#define STAR_SPEED_MAX             1.0f
#define STAR_START                 1.5f
#define STAR_SPACING               2.0f

/* -- HUD -- */
#define HUD_SCORE_X                5
#define HUD_SCORE_Y                15
#define HUD_LEVEL_X                5
#define HUD_LEVEL_Y                90
#define HUD_LIFE_BAR_Y             35
#define HUD_LIFE_ICON_OFFSET_X     9
#define LIFE_BAR_SRC_W             222
#define LIFE_BAR_SRC_H             38
#define PLAYER_MAX_HP_BASE         5.0f

/* -- MENU UI -- */
#define MENU_TITLE_OFFSET_Y        20
#define MENU_BTN_WIDTH_RATIO       0.9f
#define MENU_BTN_HEIGHT            40.0f
#define MENU_BTN_SPACING           50
#define MENU_BTN_PRESSED_OFFSET    4
#define MENU_BTN_SIZE_X 220
#define MENU_BTN_SIZE_Y 300

#define MENU_SEL_POS_X             171
#define MENU_SEL_POS_Y             486
#define MENU_SEL_WIDTH             22
#define MENU_SEL_HEIGHT            21
#define MENU_CUR_SIZE              40.0f

/* -- RANKING -- */
#define RANK_TITLE_OFFSET_Y        60
#define RANK_LINE_SPACING          25
#define RANK_POS_X_OFFSET          160
#define INPUT_BOX_SRC_W            190
#define INPUT_BOX_SRC_H            49

/* -- COLOR -- */
#define COLOR_GOLD                 al_map_rgb(255, 215, 0)
#define COLOR_SILVER               al_map_rgb(192, 192, 192)
#define COLOR_BRONZE               al_map_rgb(205, 127, 50)
#define COLOR_WHITE                al_map_rgb(255, 255, 255)
#define COLOR_BLACK                al_map_rgb(0, 0, 0)
#define COLOR_YELLOW               al_map_rgb(255, 255, 0)
#define COLOR_TITLE                al_map_rgb(230, 230, 0)

/* -- stars -- */
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

/* -- hud -- */
extern ALLEGRO_FONT* font;
extern ALLEGRO_FONT* bold_font;//제목용
extern ALLEGRO_FONT* compcolor_font;//보색용
extern long score_display;
void hud_init();
void hud_deinit();
void hud_update();
void hud_draw();

/* -- UI -- */
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
void draw_menu_ui(MENU* m, const char* title, int button_y, float wanted_width, float wanted_height, ALLEGRO_FONT* fonto);
void menu_input_update(int item_count);

void draw_bold_text(ALLEGRO_FONT* font, ALLEGRO_COLOR main_color, ALLEGRO_COLOR outline_color, float x, float y, int flags,int thickness, const char* text); //0328 김병헌 볼드체 작성 함수
void ui_draw_main_menu();
void ui_draw_pause_menu();
void ui_draw_gameover_menu();
void ui_draw_rank_menu();
void ui_draw_input_name_menu();
void ui_draw_h2p_menu();//0328 김병헌

// 작성자: 신제현
void ui_draw_level_up_menu(void);

#endif