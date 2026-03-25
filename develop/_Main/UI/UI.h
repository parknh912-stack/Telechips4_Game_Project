#ifndef _UI_H_
#define _UI_H_
#include "../Display.h"

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

#endif