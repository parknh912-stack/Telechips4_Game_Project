#include "Core.h"
#include "UI/UI.h"
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

// --- hud ---
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

// --- UI ---
ALLEGRO_BITMAP* ui_sheet = NULL;
int current_menu_selection = 0;

void ui_init() {
    ui_sheet = al_load_bitmap("ui_sheet.png");
    must_init(ui_sheet, "ui_sheet");
}

void ui_deinit() {
    if (ui_sheet) {
        al_destroy_bitmap(ui_sheet);
        ui_sheet = NULL;
    }
}

void draw_ui_element(int sx, int sy, int sw, int sh, float dx, float dy, float dw, float dh) {
    al_draw_scaled_bitmap(ui_sheet, sx, sy, sw, sh, dx, dy, dw, dh, 0);
}

void draw_menu_ui(MENU* m, const char* title) {
    draw_ui_element(UI_PANEL_BLUE_X, UI_PANEL_BLUE_Y, UI_PANEL_W, UI_PANEL_H,
        m->x - (m->width / 2), m->y - (m->height / 2), m->width, m->height);

    if (title) {
        al_draw_text(font, al_map_rgb(255, 255, 0), m->x, m->y - (m->height / 2) + 20,
            ALLEGRO_ALIGN_CENTER, title);
    }

    for (int i = 0; i < m->item_count; i++) {
        float btn_w = m->width * 0.8f;
        float btn_h = 40.0f;
        float btn_x = m->x - (btn_w / 2);
        float btn_y = m->y - (m->height / 2) + 60 + (i * 50);

        int sx, sy, sh;
        if (m->selected == i) {
            sx = UI_BTN_BLUE_P_X; sy = UI_BTN_BLUE_P_Y; sh = UI_BTN_P_H;
            btn_y += 4;
        }
        else {
            sx = UI_BTN_BLUE_X; sy = UI_BTN_BLUE_Y; sh = UI_BTN_H;
        }

        draw_ui_element(sx, sy, UI_BTN_W, sh, btn_x, btn_y, btn_w, btn_h);

        al_draw_text(font, al_map_rgb(255, 255, 255), m->x, btn_y + 12,
            ALLEGRO_ALIGN_CENTER, m->items[i]);
    }
}

void menu_input_update(int item_count) {
    if (key[ALLEGRO_KEY_UP] & KEY_SEEN) {
        current_menu_selection--;
        if (current_menu_selection < 0) {
            current_menu_selection = item_count - 1;
        }
    }
    if (key[ALLEGRO_KEY_DOWN] & KEY_SEEN) {
        current_menu_selection++;
        if (current_menu_selection >= item_count) {
            current_menu_selection = 0;
        }
    }
}

void ui_draw_main_menu() {
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 200, 250, {"Start Game", "Ranking", "Exit"}, 3, current_menu_selection };
    draw_menu_ui(&m, "- SPACE SURVIVOR -");
}

void ui_draw_pause_menu() {
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 180, 200, {"Resume", "Main Menu"}, 2, current_menu_selection };
    draw_menu_ui(&m, "PAUSED");
}

void ui_draw_gameover_menu() {
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 200, 200, {"Restart", "Main Menu"}, 2, current_menu_selection };
    draw_menu_ui(&m, "GAME OVER");
}

void ui_draw_rank_menu() {
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 220, 280, {"Back"}, 1, current_menu_selection };
    draw_menu_ui(&m, "LEADERBOARD");
}

void ui_draw_input_name_menu() {
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 220, 150, {"Save"}, 1, current_menu_selection };
    draw_menu_ui(&m, "NEW HIGH SCORE!");
}