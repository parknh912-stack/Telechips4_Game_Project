#include "../Core.h"
#include "../UI/UI.h"
#include "../Display.h"
#include "../Keyboard.h"
#include "../Sprites.h"
#include "../Player_Enemy/Player_Enemy.h"
#include "../Rank.h"


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
ALLEGRO_FONT* bold_font;//0327 김병헌 제목용 큰 폰트
ALLEGRO_FONT* compcolor_font;//김병헌 보색용 폰트
long score_display;

void hud_init()//0327 김병헌
{
    al_init_font_addon();//0327 김병헌 수정사항 : 글씨 크기를 키웠습니다.
    al_init_ttf_addon();//0327김
    font = al_load_ttf_font("PressStart2P.ttf", 15, 0);//해당 항목 두 번째가 size입니다.
    bold_font = al_load_ttf_font("PressStart2P.ttf", 22, 0);//제목용입니다.
    compcolor_font = al_load_ttf_font("PressStart2P.ttf", 22, 0);//제목용입니다. compcolor = 보색 줄임말
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
    al_draw_textf(font, al_map_rgb_f(1, 1, 1), 5, 15, 0, "%06ld", score_display);

    float hp_ratio = (float)ship.lives / 5.0f;
    if (hp_ratio < 0) hp_ratio = 0;

    al_draw_textf(font,al_map_rgb_f(1, 1, 1),5, 90, 0,"Level: %02d",level);

    int spacing = LIFE_W + 1;
    al_draw_bitmap(sprites.life_bar, spacing, 35, 0);
    for (int i = 0; i < ship.lives; i++)
        al_draw_bitmap(sprites.life, 9 + (i * spacing), 35, 0);
}

// --- UI ---

// 작성자: 김병헌
ALLEGRO_BITMAP* ui_sheet = NULL;
int current_menu_selection = 0;

void ui_init()
{
    ui_sheet = al_load_bitmap("ui_sheet.png");
    must_init(ui_sheet, "ui_sheet");
}

void ui_deinit()
{
    if (ui_sheet)
    {
        al_destroy_bitmap(ui_sheet);
        ui_sheet = NULL;
    }
}

void draw_ui_element(int sx, int sy, int sw, int sh, float dx, float dy, float real_width, float real_height)
{
    al_draw_scaled_bitmap(ui_sheet, sx, sy, sw, sh, dx, dy, real_width, real_height, 0);
}

void draw_menu_ui(MENU* m, const char* title, int button_y, float wanted_width, float wanted_height, ALLEGRO_FONT* fonto)
{
    draw_ui_element(UI_PANEL_BLUE_X, UI_PANEL_BLUE_Y, UI_PANEL_W, UI_PANEL_H, m->x - wanted_width/2, m->y - wanted_height/2, wanted_width, wanted_height);

    if (title)
    {
        al_draw_text(compcolor_font, al_map_rgb(0, 0, 0), m->x+2, m->y - (m->height / 2) + 20, ALLEGRO_ALIGN_CENTER, title);//0327 김병헌 보색 구현
        al_draw_text(compcolor_font, al_map_rgb(0, 0, 0), m->x-2, m->y - (m->height / 2) + 20, ALLEGRO_ALIGN_CENTER, title);//0327 김병헌 보색 구현
        al_draw_text(compcolor_font, al_map_rgb(0, 0, 0), m->x, m->y+2 - (m->height / 2) + 20, ALLEGRO_ALIGN_CENTER, title);//0327 김병헌 보색 구현
        al_draw_text(compcolor_font, al_map_rgb(0, 0, 0), m->x, m->y-2 - (m->height / 2) + 20, ALLEGRO_ALIGN_CENTER, title);//0327 김병헌 보색 구현
        al_draw_text(fonto, al_map_rgb(180, 180, 0), m->x, m->y - (m->height / 2) + 20, ALLEGRO_ALIGN_CENTER, title);//0327 김병헌 볼드체 구현
    }

    for (int i = 0; i < m->item_count; i++)
    {
        float btn_w = m->width * 0.9f;
        float btn_h = 40.0f;
        float btn_x = m->x - (btn_w / 2);//여긴 고정입니다. 저희가 yes no를 만들지는 좀 고민을 해봐야겟어요.
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
        //해당 4줄 추가도 보색
        al_draw_text(font, al_map_rgb(0, 0, 0), m->x+1, btn_y + 12, ALLEGRO_ALIGN_CENTER, m->items[i]);
        al_draw_text(font, al_map_rgb(0, 0, 0), m->x-1, btn_y + 12, ALLEGRO_ALIGN_CENTER, m->items[i]);
        al_draw_text(font, al_map_rgb(0, 0, 0), m->x, btn_y + 13, ALLEGRO_ALIGN_CENTER, m->items[i]);
        al_draw_text(font, al_map_rgb(0, 0, 0), m->x, btn_y + 11, ALLEGRO_ALIGN_CENTER, m->items[i]);
        al_draw_text(font, al_map_rgb(255, 255, 255), m->x, btn_y + 12, ALLEGRO_ALIGN_CENTER, m->items[i]);
        //해당 4줄 추가도 보색22
    }
}

void menu_input_update(int item_count)
{
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

void ui_draw_main_menu()
{
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 200, 250, {"Start Game", "Ranking", "Exit"}, 3, current_menu_selection };
    draw_menu_ui(&m, "- SPACE SURVIVOR -", UI_BTN_POS_Y_MID, UI_PANEL_SIZE_W, UI_PANEL_SIZE_H_M, bold_font);
}


void ui_draw_pause_menu()
{
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 180, 200, {"Resume", "Main Menu"}, 2, current_menu_selection };
    draw_menu_ui(&m, "PAUSED", UI_BTN_POS_Y_MID, UI_PANEL_SIZE_W, UI_PANEL_SIZE_H_M, bold_font);
}

void ui_draw_gameover_menu()
{
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 200, 250, {"Restart", "Ranking","Main Menu"}, 3, current_menu_selection };
    draw_menu_ui(&m, "GAME OVER", UI_BTN_POS_Y_MID, UI_PANEL_SIZE_W, UI_PANEL_SIZE_H_M, bold_font);
}

void ui_draw_rank_menu()
{
    // 1. 기본 메뉴 판 그리기 (Back 버튼 포함)
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 220, 300, {"Back"}, 1, current_menu_selection };
    draw_menu_ui(&m, "LEADERBOARD", UI_BTN_POS_Y_LOW, UI_PANEL_SIZE_W_L, UI_PANEL_SIZE_H_L, bold_font);

    // 2. 랭킹 데이터 출력 (상위 5개)
    float start_y = m.y - (m.height / 2) + 60; // 타이틀 아래 지점
    for (int i = 0; i < MAX_RANKING; i++)
    {
        // 순위 및 이름 (왼쪽 정렬 느낌)
        //0327 김병헌 추가 금 은 동 메달 느낌 구현
        if (i == 0)
        {
            al_draw_textf(font, al_map_rgb(255, 215, 0), m.x - 160, start_y + (i * 25), ALLEGRO_ALIGN_LEFT, "%d. %-10s", i + 1, ranking[i].username);
        }
        else if (i == 1)
        {
            al_draw_textf(font, al_map_rgb(192, 192, 192), m.x - 160, start_y + (i * 25), ALLEGRO_ALIGN_LEFT, "%d. %-10s", i + 1, ranking[i].username);
        }
        else if (i == 2)
        {
            al_draw_textf(font, al_map_rgb(205, 127, 50), m.x - 160, start_y + (i * 25), ALLEGRO_ALIGN_LEFT, "%d. %-10s", i + 1, ranking[i].username);
        }
        else al_draw_textf(font, al_map_rgb(1, 1, 1), m.x - 160, start_y + (i * 25), ALLEGRO_ALIGN_LEFT, "%d. %-10s", i + 1, ranking[i].username);

        // 점수 (오른쪽 정렬 느낌)
        al_draw_textf(font, al_map_rgb(255, 255, 0), m.x + 160, start_y + (i * 25),ALLEGRO_ALIGN_RIGHT, "%ld", ranking[i].score);
    }
}

void ui_draw_input_name_menu()
{
    // 1. 배경 판 그리기
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 220, 160, {"Save (Enter)"}, 1, current_menu_selection };
    draw_menu_ui(&m, "NEW HIGH SCORE!", UI_BTN_POS_Y_LOW, UI_PANEL_SIZE_W, UI_PANEL_SIZE_H_M, bold_font);

    // 2. 입력 박스 영역 (버튼 위 빈 공간)
    draw_ui_element(0,0,190,49,480,350,320,50);
    float input_y = m.y;

    // 현재 입력 중인 이름 출력
    al_draw_text(font, al_map_rgb(0, 0, 0), m.x, input_y + 10,ALLEGRO_ALIGN_CENTER, player_name);


}

// 작성자: 신제현
void ui_draw_level_up_menu()
{
    MENU m =  { BUFFER_W / 2,BUFFER_H / 2,200,400,{ "option 1", "option 2", "option 3", "option 4", "option 5", "option 6"}, 6,current_menu_selection};
    
    draw_menu_ui(&m, "LEVEL UP!!!", UI_BTN_POS_Y_HI, UI_PANEL_SIZE_W, UI_PANEL_SIZE_H_L, bold_font);
    
}