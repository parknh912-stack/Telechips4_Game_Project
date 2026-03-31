#include "../Core.h"
#include "../Display.h"
#include "../UI/UI.h"
#include "../Keyboard.h"
#include "../Sprites.h"
#include "../Player_Enemy/Player_Enemy.h"
#include "../Rank.h"
#include "../Level_UP/Level_UP.h"//ÃÖÁ¾ Ã¼·Â¹Ù ±¸Çö

// --- stars ---
STAR stars[STARS_N];

void stars_init()//0328 ±èº´Çå
{
    for (int i = 0; i < STARS_N; i++)
    {
        stars[i].y = between_f(0, BUFFER_H);
        stars[i].speed = between_f(STAR_SPEED_MIN, STAR_SPEED_MAX);//0328 ±èº´Çå
    }
}
void stars_update()//0328 ±èº´Çå
{
    for (int i = 0; i < STARS_N; i++)//0328 ±èº´Çå
    {
        stars[i].y += stars[i].speed;//0328 ±èº´Çå
        if (stars[i].y >= BUFFER_H)//0328 ±èº´Çå
        {
            stars[i].y = 0;//0328 ±èº´Çå
            stars[i].speed = between_f(STAR_SPEED_MIN, STAR_SPEED_MAX);//0328 ±èº´Çå
        }
    }
}

void stars_draw()
{
    float star_x = STAR_START;//0328 ±èº´Çå
    for (int i = 0; i < STARS_N; i++)//0328 ±èº´Çå
    {
        float l = stars[i].speed;//0328 ±èº´Çå
        al_draw_pixel(star_x, stars[i].y, al_map_rgb_f(rand()%255, rand()%255, rand() % 255));//0328 ±èº´Çå
        star_x += STAR_SPACING;//0328 ±èº´Çå
    }
}



// --- hud ---
ALLEGRO_FONT* font;
ALLEGRO_FONT* bold_font;//0327 ±èº´Çå Á¦¸ñ¿ë Å« ÆùÆ®
ALLEGRO_FONT* compcolor_font;//±èº´Çå º¸»ö¿ë ÆùÆ®
long score_display;

double stage_alert_timer = -1.0;        // 0330 ½ÅÁ¦Çö
double boss_alert_timer = -1.0;         // 0330 ½ÅÁ¦Çö
double survive_timer = -1.0;            // 0330 ½ÅÁ¦Çö

void hud_init()//0328 ±èº´Çå
{
    al_init_font_addon();//0327 ±èº´Çå ¼öÁ¤»çÇ× : ±Û¾¾ Å©±â¸¦ Å°¿ü½À´Ï´Ù.
    al_init_ttf_addon();//0328 ±èº´Çå
    font = al_load_ttf_font("PressStart2P.ttf", FONT_SIZE_NORMAL, 0);//0328 ±èº´Çå
    bold_font = al_load_ttf_font("PressStart2P.ttf", FONT_SIZE_TITLE, 0);//0328 ±èº´Çå
    compcolor_font = al_load_ttf_font("PressStart2P.ttf", FONT_SIZE_TITLE, 0);//0328 ±èº´Çå
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
    al_draw_textf(font, al_map_rgb_f(1, 1, 1), HUD_SCORE_X, HUD_SCORE_Y, 0, "%06ld", score_display);//0328 ±èº´Çå

    al_draw_textf(font,al_map_rgb_f(1, 1, 1), HUD_LEVEL_X, HUD_LEVEL_Y, 0,"Level: %02d",level);//0328 ±èº´Çå

    //ÇöÀç ½ºÅ×ÀÌÁö Ãâ·Â
    al_draw_textf(font, al_map_rgb_f(1, 1, 1), 5, 120, 0, "stage: %02d", stage_num + 1);
    al_draw_textf(font, al_map_rgb_f(1, 1, 1), 5, 150, 0, "x : %d", (int)ship.cx);
    al_draw_textf(font, al_map_rgb_f(1, 1, 1), 5, 180, 00, "y : %d", (int)ship.cy);
    al_draw_textf(font, al_map_rgb_f(1, 1, 1), 5, 210, 00, "sec : %d", (frames / 60));

    al_draw_scaled_bitmap(sprites.life_bar, 0, 0, LIFE_BAR_SRC_W, LIFE_BAR_SRC_H, HUD_LIFE_BAR_X, HUD_LIFE_BAR_Y, 4*(HUD_LIFE_BAR_X + ship.max_lifes - 6), HUD_LIFE_BAR_Y, 0);
    al_draw_filled_rectangle(HUD_LIFE_BAR_X, HUD_LIFE_BAR_Y + 2, 4*(ship.curr_lifes), HUD_LIFE_BAR_Y + HUD_LIFE_WIDTH + 1, al_map_rgb(255, 0, 0));
    al_draw_textf(font, COLOR_YELLOW, 4 * (HUD_LIFE_BAR_X + ship.max_lifes - 2), HUD_LIFE_BAR_Y*1.5, 0, "%.1f", (float)100* ship.curr_lifes / ship.max_lifes);
    
    // 0330 ½ÅÁ¦Çö - º¸½º ³ªÅ¸³­´Ù°í È­¸é¿¡ ¸Þ½ÃÁö 2ÃÊ°£ Ç¥½Ã
    if (is_boss_alive() && boss_alert_timer >= 0.0 && (al_get_time() - boss_alert_timer < 2.0))
        al_draw_text(
            bold_font,
            al_map_rgb_f(1.0, 0.0, 0.0),
            BUFFER_W / 2, BUFFER_H / 2 - 120,
            ALLEGRO_ALIGN_CENTER,
            "!!! A L E R T !!!"
        );
	// 0330 ½ÅÁ¦Çö - ¸î ¹øÂ° ½ºÅ×ÀÌÁö¿¡ ÁøÀÔÇß´Ù°í 2ÃÊ°£ È­¸é¿¡ ¸Þ½ÃÁö Ç¥½Ã
    if (stage_alert_timer >= 0.0 && (al_get_time() - stage_alert_timer < 2.0))
    {
        al_draw_textf(
            bold_font,
            al_map_rgb_f(1.0, 1.0, 1.0),
            BUFFER_W / 2, BUFFER_H / 2 - 120,
            ALLEGRO_ALIGN_CENTER,
            "=== S T A G E   %02d ===",
            stage_num + 1
		);
	}
	// 0330 ½ÅÁ¦Çö - Áß°£ »ó´Ü¿¡ »ýÁ¸ ½Ã°£ º¸¿©ÁÖ±â
	int total_second = frames / 60;
	int min = total_second / 60;
	int sec = total_second % 60;
	int milli_sec = (frames % 60) * 1000 / 60;

	al_draw_textf(
		bold_font,
		COLOR_WHITE,
		BUFFER_W / 2, 15,
		ALLEGRO_ALIGN_CENTER,
		"TIME   %02d:%02d:%03d", min, sec, milli_sec);
}


// --- UI ---

// ÀÛ¼ºÀÚ: ±èº´Çå
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

void draw_bold_text(ALLEGRO_FONT* font, ALLEGRO_COLOR main_color, ALLEGRO_COLOR outline_color, float x, float y, int flags, int thickness, const char* text)//0328 ±èº´Çå º¼µåÃ¼ ¸¸µé±â µû·Î ÇÔ¼ö·Î ¸¸µê
{
    //Å×µÎ¸®
    al_draw_text(font, outline_color, x + thickness, y, flags, text);//0328 ±èº´Çå
    al_draw_text(font, outline_color, x - thickness, y, flags, text);//0328 ±èº´Çå
    al_draw_text(font, outline_color, x, y + thickness, flags, text);//0328 ±èº´Çå
    al_draw_text(font, outline_color, x, y - thickness, flags, text);//0328 ±èº´Çå
    //º»¹®
    al_draw_text(font, main_color, x, y, flags, text);//0328 ±èº´Çå
}

void draw_menu_ui(MENU* m, const char* title, int button_y, float wanted_width, float wanted_height, ALLEGRO_FONT* fonto)
{
    draw_ui_element(UI_PANEL_BLUE_X, UI_PANEL_BLUE_Y, UI_PANEL_W, UI_PANEL_H, m->x - wanted_width / 2, m->y - wanted_height / 2, wanted_width, wanted_height);

    if (title)
    {
        draw_bold_text(fonto, COLOR_TITLE, COLOR_BLACK, m->x, (m->y)/1.7, ALLEGRO_ALIGN_CENTER,2, title);//0328 ±èº´Çå
    }

    for (int i = 0; i < m->item_count; i++)
    {
        float btn_w = m->width * 0.9f;
        float btn_h = 40.0f;
        float btn_x = m->x - (btn_w / 2);//¿©±ä °íÁ¤ÀÔ´Ï´Ù. ÀúÈñ°¡ yes no¸¦ ¸¸µéÁö´Â Á» °í¹ÎÀ» ÇØºÁ¾ß°Ù¾î¿ä.
        float btn_y = m->y - button_y + (i * 50);

        int sx, sy, sh;
        if (m->selected == i)
        {
            sx = UI_BTN_BLUE_P_X; sy = UI_BTN_BLUE_P_Y; sh = UI_BTN_P_H;
            btn_y += MENU_BTN_PRESSED_OFFSET;

            float cursor_w = MENU_CUR_SIZE;//0328 ±èº´Çå - ¸Þ´º¼±ÅÃ Ä¿¼­
            float cursor_h = MENU_CUR_SIZE;//0328 ±èº´Çå - ¸Þ´º¼±ÅÃ Ä¿¼­
            float cursor_x = btn_x - cursor_w - 15.0f;//0328 ±èº´Çå - ¸Þ´º¼±ÅÃ Ä¿¼­
            float cursor_y = btn_y + (btn_h / 2.0f) - (cursor_h / 2.0f);//0328 ±èº´Çå - ¸Þ´º¼±ÅÃ Ä¿¼­
            draw_ui_element(MENU_SEL_POS_X, MENU_SEL_POS_Y, MENU_SEL_WIDTH, MENU_SEL_HEIGHT, cursor_x, cursor_y, cursor_w, cursor_h);//0328 ±èº´Çå - ¸Þ´º¼±ÅÃ Ä¿¼­
        }
        else
        {
            sx = UI_BTN_BLUE_X; sy = UI_BTN_BLUE_Y; sh = UI_BTN_H;
        }

        draw_ui_element(sx, sy, UI_BTN_W, sh, btn_x, btn_y, btn_w, btn_h);
        
        
        draw_bold_text(font, COLOR_WHITE, COLOR_BLACK, m->x, btn_y + 12, ALLEGRO_ALIGN_CENTER, 1, m->items[i]);//0328 ±èº´Çå

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
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 350, 300, {"Start Game", "How to play", "Ranking", "Exit"}, 4, current_menu_selection };//0328 ±èº´Çå howtoplay Ãß°¡
    draw_menu_ui(&m, "- SPACE SURVIVOR -", UI_BTN_POS_Y_MID, UI_PANEL_SIZE_W, UI_PANEL_SIZE_H_L, bold_font);
}

void ui_draw_pause_menu()
{
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 180, 200, {"Resume", "Main Menu"}, 2, current_menu_selection };
    draw_menu_ui(&m, "PAUSED", UI_BTN_POS_Y_MID, UI_PANEL_SIZE_W, UI_PANEL_SIZE_H_M, bold_font);

    draw_bold_text(bold_font, COLOR_WHITE, COLOR_GOLD, STATUS_POS_X, STATUS_POS_Y, 0, 2, "STATUS");
    al_draw_textf(font, COLOR_WHITE, STATUS_POS_X, STATUS_POS_Y + RANK_LINE_SPACING * 1, 0,
        "HP : %d / %d", ship.curr_lifes, ship.max_lifes);

    al_draw_textf(font, COLOR_WHITE, STATUS_POS_X, STATUS_POS_Y + RANK_LINE_SPACING * 2, 0,
        "DAMAGE : %d", ship.damage);

    al_draw_textf(font, COLOR_WHITE, STATUS_POS_X, STATUS_POS_Y + RANK_LINE_SPACING * 3, 0,
        "SPEED : %.1f", ship.speed);

    al_draw_textf(font, COLOR_WHITE, STATUS_POS_X, STATUS_POS_Y + RANK_LINE_SPACING * 4, 0,
        "FIRE RATE : %.1f", ship.fire_rate);

    al_draw_textf(font, COLOR_WHITE, STATUS_POS_X, STATUS_POS_Y + RANK_LINE_SPACING * 5, 0,
        "SHOTS : %d", ship.shot_count);
}

void ui_draw_gameover_menu()
{
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 200, 250, {"Restart", "Ranking","Main Menu"}, 3, current_menu_selection };
    draw_menu_ui(&m, "GAME OVER", UI_BTN_POS_Y_MID, UI_PANEL_SIZE_W, UI_PANEL_SIZE_H_M, bold_font);
}

void ui_draw_rank_menu()
{
    // 1. ±âº» ¸Þ´º ÆÇ ±×¸®±â (Back ¹öÆ° Æ÷ÇÔ)
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 220, 300, {"Back"}, 1, current_menu_selection };
    draw_menu_ui(&m, "LEADERBOARD", UI_BTN_POS_Y_LOW, UI_PANEL_SIZE_W_L, UI_PANEL_SIZE_H_L, bold_font);

    // 2. ·©Å· µ¥ÀÌÅÍ Ãâ·Â (»óÀ§ 5°³)
    float start_y = m.y - (m.height / 2) + RANK_TITLE_OFFSET_Y; // 0328 ±èº´Çå
    for (int i = 0; i < MAX_RANKING; i++)
    {
        // ¼øÀ§ ¹× ÀÌ¸§ (¿ÞÂÊ Á¤·Ä)
        //0328 ±èº´Çå ±Ý Àº µ¿ ¸Þ´Þ ¹× º¯¼ö Á¤¸®
        float left_x = m.x - RANK_POS_X_OFFSET; //0328 ±èº´Çå
        float right_x = m.x + RANK_POS_X_OFFSET; //0328 ±èº´Çå
        float line_y = start_y + (i * RANK_LINE_SPACING); //0328 ±èº´Çå

        if (i == 0)
        {
            al_draw_textf(font, COLOR_GOLD, left_x, line_y, ALLEGRO_ALIGN_LEFT, "%d. %-10s", i + 1, ranking[i].username);//0328 ±èº´Çå
        }
        else if (i == 1)
        {
            al_draw_textf(font, COLOR_SILVER, left_x, line_y, ALLEGRO_ALIGN_LEFT, "%d. %-10s", i + 1, ranking[i].username);//0328 ±èº´Çå
        }
        else if (i == 2)
        {
            al_draw_textf(font, COLOR_BRONZE, left_x, line_y, ALLEGRO_ALIGN_LEFT, "%d. %-10s", i + 1, ranking[i].username);//0328 ±èº´Çå
        }
        else al_draw_textf(font, COLOR_BLACK, left_x, line_y, ALLEGRO_ALIGN_LEFT, "%d. %-10s", i + 1, ranking[i].username);//0328 ±èº´Çå

        al_draw_textf(font, COLOR_YELLOW, right_x, line_y,ALLEGRO_ALIGN_RIGHT, "%ld", ranking[i].score);//0328 ±èº´Çå
    }
}

void ui_draw_input_name_menu()
{
    // 1. ¹è°æ ÆÇ ±×¸®±â
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 220, 160, {"Save (Enter)"}, 1, current_menu_selection };
    draw_menu_ui(&m, "NEW HIGH SCORE!", UI_BTN_POS_Y_LOW, UI_PANEL_SIZE_W, UI_PANEL_SIZE_H_L, bold_font);

    float input_w = 320.0f; //0328 ±èº´Çå ÀÔ·ÂÃ¢ ³Êºñ
    float input_h = 50.0f; //0328 ±èº´Çå ÀÔ·ÂÃ¢ ³ôÀÌ
    float input_x = m.x - (input_w / 2); //0328 ±èº´Çå ÀÔ·ÂÃ¢ ³Êºñ¿¡ µû¸¥ À§Ä¡
    float input_y = m.y - (input_h / 2); //0328 ±èº´Çå ÀÔ·ÂÃ¢ ³ôÀÌ¿¡ µû¸¥ À§Ä¡

    // 2. ÀÔ·Â ¹Ú½º ¿µ¿ª (¹öÆ° À§ ºó °ø°£)
    draw_ui_element(0,0,INPUT_BOX_SRC_W, INPUT_BOX_SRC_H, input_x, input_y, input_w, input_h);//0328 ±èº´Çå

    // ÇöÀç ÀÔ·Â ÁßÀÎ ÀÌ¸§ Ãâ·Â
    al_draw_text(font, COLOR_BLACK, m.x, m.y-6,ALLEGRO_ALIGN_CENTER, player_name);//0328 ±èº´Çå


}

// ÀÛ¼ºÀÚ: ½ÅÁ¦Çö
void ui_draw_level_up_menu()
{
    MENU m =  { BUFFER_W / 2,BUFFER_H / 2,300,400,{ "ATK + 2 ", "BULLET + 1", "ATK SPD + 20%", "SPD + 10%", "MAX LIFE + 10%", "HEAL"}, 6,current_menu_selection};
    
    draw_menu_ui(&m, "LEVEL UP!!!", UI_BTN_POS_Y_HI, UI_PANEL_SIZE_W, UI_PANEL_SIZE_H_L, bold_font);
    
}


void ui_draw_h2p_menu()//0328 ±èº´Çå
{
    int extra_y = 50;
    MENU m = { BUFFER_W / 2, BUFFER_H / 2, 220, 300, {"Back"}, 1, current_menu_selection };
    draw_menu_ui(&m, "ABOUT", UI_BTN_POS_Y_LOW - extra_y, UI_PANEL_SIZE_W_VL, UI_PANEL_SIZE_H_L+100, bold_font);
    for (int i = 1; i <= 5; i++)
    {
        al_draw_scaled_bitmap
        (
            sprites.item[i],           // 1. ºñÆ®¸Ê
            0, 0,                      // 2, 3. ¼Ò½º ½ÃÀÛ (sx, sy)
            al_get_bitmap_width(sprites.item[i]),  // 4. ¼Ò½º °¡·Î (sw)
            al_get_bitmap_height(sprites.item[i]), // 5. ¼Ò½º ¼¼·Î (sh)
            m.x / 2 + 120, m.y - 6 + (i-1)*25,          // 6, 7. ´ë»ó À§Ä¡ (dx, dy)
            20,  // 8. ´ë»ó °¡·Î (dw) - ´©¶ôµÇ¾ú´ø ºÎºÐ
            20, // 9. ´ë»ó ¼¼·Î (dh) - ´©¶ôµÇ¾ú´ø ºÎºÐ
            0                          // 10. ÇÃ·¡±×
        );
    }
    
    
    draw_bold_text(font, COLOR_WHITE, COLOR_BLACK, m.x, m.y - 6, ALLEGRO_ALIGN_CENTER, 1, "This is health potion");//0330 ±èº´Çå
    draw_bold_text(font, COLOR_WHITE, COLOR_BLACK, m.x, m.y - 6 + 25, ALLEGRO_ALIGN_CENTER, 1, "attack speed bonus");//0330 ±èº´Çå
    draw_bold_text(font, COLOR_WHITE, COLOR_BLACK, m.x, m.y - 6 + 2*25, ALLEGRO_ALIGN_CENTER, 1, "random box");//0330 ±èº´Çå
    draw_bold_text(font, COLOR_WHITE, COLOR_BLACK, m.x, m.y - 6 + 3 * 25, ALLEGRO_ALIGN_CENTER, 1, "shield");//0330 ±èº´Çå
    draw_bold_text(font, COLOR_WHITE, COLOR_BLACK, m.x, m.y - 6 + 4 * 25, ALLEGRO_ALIGN_CENTER, 1, "ufo");//0330 ±èº´Çå
    
    
}

void ui_draw_clear_menu() // 0330 ±èº´Çå
{

    static float scroll_y = 200;
    static int frame_count = 0;
    frame_count++;

    float intensity = 240 * (1.0f / (1.0f + expf(-(frame_count - 500) * 0.005f)));//ÃµÃµÈ÷ ¹à¾ÆÁö´Â

    static ALLEGRO_BITMAP* bg_image = NULL;
    if (!bg_image) 
    {
        bg_image = al_load_bitmap("Ending_Scene.png");
    }


    // 2. ¹è°æ ±×¸®±â
    if (bg_image) 
    {
        al_draw_tinted_scaled_bitmap
        (
            bg_image,            // ºñÆ®¸Ê Æ÷ÀÎÅÍ
            al_map_rgb(intensity, intensity, intensity), // Æ¾Æ® ÄÃ·¯
            0, 0,                // ¿øº» ÀÌ¹ÌÁöÀÇ ½ÃÀÛÁ¡ (sx, sy)
            al_get_bitmap_width(bg_image),  // ¿øº» ³Êºñ (sw)
            al_get_bitmap_height(bg_image), // ¿øº» ³ôÀÌ (sh)
            0, 0,                // È­¸é¿¡ ±×·ÁÁú ½ÃÀÛÁ¡ (dx, dy)
            BUFFER_W, BUFFER_H,  // È­¸é¿¡ ±×·ÁÁú Å©±â (dw, dh)
            0                    // ÇÃ·¡±×
        );
    }
    draw_bold_text(bold_font, COLOR_TITLE, COLOR_WHITE, BUFFER_W / 2, scroll_y - 40, ALLEGRO_ALIGN_CENTER, 2, "Congrats!");
    scroll_y -= 0.5;
    al_draw_multiline_text(
        font,
        COLOR_WHITE,
        BUFFER_W / 2,
        scroll_y,
        500,
        20,
        ALLEGRO_ALIGN_CENTER,
        "You saved our land\nThank you for playing our game\nVisit www.TeleChips2026.com\n\n\ncredit\n\nPM : PARK NAMHYEON\n\nPlayer Logic : CHEON WONSEOK\n\nGame Logic : SHIN JEHYEON\n\nUI : KIM BYEONGHEON"
    );
    if (scroll_y < -200)
    {
        if ((frame_count / 25) % 2 == 0)
        {
            draw_bold_text(bold_font, COLOR_BLACK, COLOR_WHITE, BUFFER_W / 2, BUFFER_H / 2, ALLEGRO_ALIGN_CENTER, 2, "Press ENTER to Continue");
        }

    }

}

// ÀÛ¼ºÀÚ: 0330 ½ÅÁ¦Çö - ¿£µù ¸Þ´º
void ui_draw_ending_menu(void)
{
    MENU m = 
    {
        BUFFER_W / 2, BUFFER_H / 2,
        300, 400,
        { "Record Your Score", "Return To Menu" },
        2,
        current_menu_selection
    };

    draw_menu_ui(&m, "GAME CLEAR!!!", UI_BTN_POS_Y_MID, UI_PANEL_SIZE_W, UI_PANEL_SIZE_H_M, bold_font);
}