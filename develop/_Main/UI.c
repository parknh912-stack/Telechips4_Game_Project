#include "Core.h"
#include "UI.h"

#include "Display.h"
#include "Keyboard.h"
#include "Sprites.h"
#include "Player_Enemy/Player_Enemy.h"

// --- stars ---


STAR stars[STARS_N];

void stars_init()
{
    for (int i = 0; i < STARS_N; i++)
    {
        stars[i].y = between_f(0, BUFFER_H);
        stars[i].speed = between_f(0.1, 1);
    }
}

void stars_update()
{
    for (int i = 0; i < STARS_N; i++)
    {
        stars[i].y += stars[i].speed;
        if (stars[i].y >= BUFFER_H)
        {
            stars[i].y = 0;
            stars[i].speed = between_f(0.1, 1);
        }
    }
}

void stars_draw()
{
    float star_x = 1.5;
    for (int i = 0; i < STARS_N; i++)
    {
        float l = stars[i].speed * 0.8;
        al_draw_pixel(star_x, stars[i].y, al_map_rgb_f(l, l, l));
        star_x += 2;
    }
}


/* --- hud --- */

ALLEGRO_FONT* font;
long score_display;

void hud_init()
{
    font = al_create_builtin_font();
    must_init(font, "font");

    score_display = 0;
}

void hud_deinit()
{
    al_destroy_font(font);
}

void hud_update()
{
    if (frames % 2)
        return;

    for (long i = 5; i > 0; i--)
    {
        long diff = 1 << i;
        if (score_display <= (score - diff))
            score_display += diff;
    }
}

void hud_draw()
{
    al_draw_textf(
        font,
        al_map_rgb_f(1, 1, 1),
        1, 1,
        0,
        "%06ld",
        score_display
    );

    int spacing = LIFE_W + 1;
    for (int i = 0; i < ship.lives; i++)
        al_draw_bitmap(sprites.life, 1 + (i * spacing), 10, 0);

    if (ship.lives < 0)
        al_draw_text(
            font,
            al_map_rgb_f(1, 1, 1),
            BUFFER_W / 2, BUFFER_H / 2,
            ALLEGRO_ALIGN_CENTER,
            "G A M E  O V E R"
        );
    

}

/* --- pause menu --- */

PAUSE_MENU pause_menu;

// =====================
void pause_menu_init(void)
{
    pause_menu._sheet = al_load_bitmap("./uipack_rpg_sheet.png");
    must_init(pause_menu._sheet, "pause menu sheet");

    pause_menu.bg = al_create_sub_bitmap(pause_menu._sheet, 0, 0, 0, 0);
    pause_menu.panel = al_create_sub_bitmap(pause_menu._sheet, 0, 0, 0, 0);
    pause_menu.cursor = al_create_sub_bitmap(pause_menu._sheet, 0, 0, 0, 0);

    pause_menu.button[0] = al_create_sub_bitmap(pause_menu._sheet, 0, 100, 100, 0);
    pause_menu.button[1]= al_create_sub_bitmap(pause_menu._sheet, 0, 0, 0, 0);

    pause_menu.choices[0] = "게임 재개";
    pause_menu.choices[1] = "게임 나가기";
    pause_menu.choice_count = 2;

    pause_menu.selected = 0;
    pause_menu.activated = false;

    pause_menu.x = BUFFER_W / 2;
    pause_menu.y = BUFFER_H / 2;

    score_display = 0;
}

void pause_menu_deinit(void)
{
    al_destroy_bitmap(pause_menu.bg);
    al_destroy_bitmap(pause_menu.panel);
    al_destroy_bitmap(pause_menu.cursor);

    for (int i = 0; i < pause_menu.choice_count; ++i)
    {
        al_destroy_bitmap(pause_menu.button[i]);
    }

    al_destroy_bitmap(pause_menu._sheet);
}

// =====================