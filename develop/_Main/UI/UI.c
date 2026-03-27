#include "../Core.h"
#include "../UI/UI.h"
#include "../Display.h"
#include "../Keyboard.h"
#include "../Sprites.h"
#include "../Player_Enemy/Player_Enemy.h"
#include "../Rank.h"    //추가

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
            stars[i].speed = between_f(0.1,1);
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

// 작성: 0327 김병헌
void hud_init()
{
    al_init_font_addon();       // 0327 김병헌
    al_init_ttf_addon();        // 0327 김병헌
    font = al_load_ttf_font("PressStart2P.ttf", 15, 0);       // 두 번째 인수가 글꼴 크기입니다.
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

    // 1. 점수 출력
    al_draw_textf(font, al_map_rgb_f(1, 1, 1), 5, 15, 0, "%012ld", score_display);

    // 2. 체력바 출력 
    // 0327 김병헌
    float hp_ratio = (float)ship.curr_lifes / 5.0f;

    if (hp_ratio < 0)
        hp_ratio = 0;

    // 2. 레벨 출력
    al_draw_textf(font, al_map_rgb_f(1, 1, 1), 1, 80, 0, "LEVEL: %03d", level);
    
    int spacing = LIFE_W + 1;
    al_draw_bitmap(sprites.life_bar, spacing, 35, 0);
    for (int i = 0; i < ship.curr_lifes; i++)
        al_draw_bitmap(sprites.life, 9 + (i * spacing), 35, 0);

}

// --- UI ---

// 작성자: 김병헌
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

void draw_menu_ui(MENU* m, const char* title, int button_y, float wanted_width, float wanted_height)
{
      
    draw_ui_element(UI_PANEL_BLUE_X, UI_PANEL_BLUE_Y, UI_PANEL_W, UI_PANEL_H, m->x - wanted_width / 2, m->y - wanted_height / 2, wanted_width, wanted_height);

    if (title) 
    {
        al_draw_text(font, al_map_rgb(255, 255, 0), m->x, m->y - (m->height / 2) + 20,
            ALLEGRO_ALIGN_CENTER, title);
    }

    for (int i = 0; i < m->item_count; i++)
    {
        float btn_w = m->width * 0.8f;
        float btn_h = 40.0f;
        float btn_x = m->x - (btn_w / 2);       // 여기는 고정! yes/no가 만들어지는지 고민을 해봐야 하는 지점
        float btn_y = m->y - button_y + (i * 50);

        int sx, sy, sh;

        if (m->selected == i) 
        {
            sx = UI_BTN_BLUE_P_X; sy = UI_BTN_BLUE_P_Y; sh = UI_BTN_P_H;
            btn_y += 4;
        }
        else
        {
            sx = UI_BTN_BLUE_X; sy = UI_BTN_BLUE_Y; sh = UI_BTN_H;
        }

        draw_ui_element(sx, sy, UI_BTN_W, sh, btn_x, btn_y, btn_w, btn_h);
    
        al_draw_text(font, al_map_rgb(255, 255, 255), m->x, btn_y + 12,
            ALLEGRO_ALIGN_CENTER, m->items[i]);
    }
}

void menu_input_update(int item_count) {
    if (key[ALLEGRO_KEY_UP] & KEY_SEEN) 
    {
        current_menu_selection--;

        if (current_menu_selection < 0) 
        {
            current_menu_selection = item_count - 1;
        }
    }
    if (key[ALLEGRO_KEY_DOWN] & KEY_SEEN) 
    {
        current_menu_selection++;

        if (current_menu_selection >= item_count)
        {
            current_menu_selection = 0;
        }
    }
}

void ui_draw_main_menu() {
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 200, 250, {"Start Game", "Ranking", "Exit"}, 3, current_menu_selection };
    draw_menu_ui(&m, "- SPACE SURVIVOR -", UI_BTN_POS_Y_MID, UI_PANEL_SIZE_W, UI_PANEL_SIZE_H_M);
}

void ui_draw_pause_menu() {
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 180, 200, {"Resume", "Main Menu"}, 2, current_menu_selection };
    draw_menu_ui(&m, "PAUSED", UI_BTN_POS_Y_MID, UI_PANEL_SIZE_W, UI_PANEL_SIZE_H_M);
}

void ui_draw_gameover_menu() {
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 200, 200, {"Restart", "Main Menu"}, 2, current_menu_selection };
    draw_menu_ui(&m, "GAME OVER", UI_BTN_POS_Y_MID, UI_PANEL_SIZE_W, UI_PANEL_SIZE_H_M);
}

void ui_draw_rank_menu() {
    // 1. 기본 메뉴 판 그리기 (Back 버튼 포함)
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 220, 300, {"Back"}, 1, current_menu_selection };
    draw_menu_ui(&m, "LEADERBOARD", UI_BTN_POS_Y_LOW, UI_PANEL_SIZE_W_L, UI_PANEL_SIZE_H_L);


    // 2. 랭킹 데이터 출력 (상위 5개)
    float start_y = m.y - (m.height / 2) + 60; // 타이틀 아래 지점
    for (int i = 0; i < MAX_RANKING; i++) {
        // 순위 및 이름 (왼쪽 정렬 느낌)
        al_draw_textf(font, al_map_rgb(255, 255, 255), m.x - 80, start_y + (i * 25),
            ALLEGRO_ALIGN_LEFT, "%d. %-10s", i + 1, ranking[i].username);

        // 점수 (오른쪽 정렬 느낌)
        al_draw_textf(font, al_map_rgb(255, 255, 0), m.x + 80, start_y + (i * 25),
            ALLEGRO_ALIGN_RIGHT, "%ld", ranking[i].score);
    }
}

void ui_draw_input_name_menu() 
{
    // 1. 배경 판 그리기
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 220, 160, {"Save (Enter)"}, 1, current_menu_selection };
    draw_menu_ui(&m, "NEW HIGH SCORE!", UI_BTN_POS_Y_LOW, UI_PANEL_SIZE_W, UI_PANEL_SIZE_H_M);

    // 2. 입력 박스 영역 (버튼 위 빈 공간)
    draw_ui_element(0, 0, 190, 49, 480, 350, 320, 50);
    float input_y = m.y;

    // 현재 입력 중인 이름 출력
    al_draw_text(font, al_map_rgb(0, 0, 0), m.x, input_y + 10, ALLEGRO_ALIGN_CENTER, player_name);
}

// 작성자: 신제현
void ui_draw_level_up_menu()
{
    MENU m = {
        BUFFER_W / 2,
        BUFFER_H / 2,
        200,
        400,
        { "Damage Up", "Shot Count Up", "Fire Rate Up", "Speed Up", "Max Lifes Up", "Instant Lifes"},
        6,
        current_menu_selection
    };

    draw_menu_ui(&m, "LEVEL UP!!!", UI_BTN_POS_Y_HI, UI_PANEL_SIZE_W, UI_PANEL_SIZE_H_L);
}