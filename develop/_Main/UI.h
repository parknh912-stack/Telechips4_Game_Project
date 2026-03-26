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

#define WINDOW_W    (100)
#define WINDOW_H    (100)

typedef struct ui
{
    // 체력 바 계열
    ALLEGRO_BITMAP* bar_left;
    ALLEGRO_BITMAP* bar_mid;
    ALLEGRO_BITMAP* bar_right;
    ALLEGRO_BITMAP* bar_max;

    // 버튼 계열
    ALLEGRO_BITMAP* rect_button;

    // 커서 계열
    ALLEGRO_BITMAP* cursor;

    // 윈도우 계열
    ALLEGRO_BITMAP* window;
} UI;

typedef struct _window
{
    UI ui;
    int x, y;
    int cx, cy;
} WINDOW;

void ui_draw(void);
void ui_init(void);
void ui_deinit(void);

#endif