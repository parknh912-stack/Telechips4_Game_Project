/* --- header & addon --- */
#include "Core.h"
#include "Keyboard.h"
#include "Sprites.h"
#include "Display.h"

#include "Audio.h"
#include "Fx.h"

#include "Player_Enemy/Player_Enemy.h"
#include "UI/UI.h"
#include "Rank.h"

// 작성자: 신제현
// 레벨 업에 필요한 점수의 양
#define LV_UP           (1000)

/* --- General --- */
long frames = 0;
long score = 0;
int level = 1;

STATE current_state = STATE_MENU;

void must_init(bool test, const char* description)
{
    if (test) return;
    printf("couldn't initialize %s\n", description);
    exit(1);
}

int between(int lo, int hi)
{
    return lo + (rand() % (hi - lo));
}

float between_f(float lo, float hi)
{
    return lo + ((float)rand() / (float)RAND_MAX) * (hi - lo);
}

bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
{
    if (ax1 > bx2) return false;
    if (ax2 < bx1) return false;
    if (ay1 > by2) return false;
    if (ay2 < by1) return false;
    return true;
}

/* --- Rank --- */ //0327 김병헌
RANK ranking[RANK_QUEUE_SIZE]; // Rank.h의 extern 변수 실제 선언
int rank_count = 0;
char player_name[MAX_NAME_LEN] = "";
extern int name_len = 0;

// 작성자 : 박남현
/* --- 원형 충돌 --- */
bool collide_circle(int cx1, int cy1, int r1, int cx2, int cy2, int r2) {
    long dx = cx2 - cx1;
    long dy = cy2 - cy1;

    long radium_Sum = r1 + r2;
    long radium_Square = (radium_Sum * radium_Sum);
    long distance = (dx * dx) + (dy * dy);

    return (distance <= radium_Square);     //반지름의 제곱이 더 크면 충돌
}

/* --- Gameplay --- */
// 작성자: 신제현
void pause_resume_game(STATE* state)
{
    switch (*state)
    {
    case STATE_PLAYING:
        *state = STATE_PAUSE;
        current_menu_selection = 0;
        break;
    case STATE_PAUSE:
        *state = STATE_PLAYING;
        break;
    }
}

// 작성자: 신제현
void game_state_update(STATE* state, bool* done)
{
    bool is_select_pressed = (key[ALLEGRO_KEY_ENTER] & KEY_SEEN) || (key[ALLEGRO_KEY_SPACE] & KEY_SEEN);

    switch (*state)
    {
    case STATE_MENU:
        menu_input_update(3);
        if (is_select_pressed) {
            if (current_menu_selection == 0) {
                *state = STATE_PLAYING;
            }
            else if (current_menu_selection == 1) {
                *state = STATE_RANK;
                current_menu_selection = 0;
            }
            else if (current_menu_selection == 2) {
                *done = true;
            }
        }
        break;

    case STATE_PLAYING:
        fx_update();
        shots_update();
        stars_update();
        ship_update();
        aliens_update();
        hud_update();
        // 작성자: 신제현
        // 레벨 증가하는 점수에 따라 해당 조건 검사
        if (score >= level * LV_UP)
        {
            *state = STATE_LEVEL_UP;
            current_menu_selection = 0;
            ++level;
        }
        //0327 김병헌
        if (ship.lives < 0)
        {
            // 랭킹 진입 가능 여부 체크
            if (rank_count < MAX_RANKING || score > ranking[MAX_RANKING - 1].score)
            {
                *state = STATE_INPUT_NAME; // 랭킹권이면 이름 입력으로
                name_len = 0;
                player_name[0] = '\0';
            }
            else
            {
                *state = STATE_GAMEOVER;   // 아니면 그냥 게임오버로
            }
            current_menu_selection = 0;
            return; // 상태가 바뀌었으므로 즉시 빠져나감
        }

        break;

    case STATE_PAUSE:
        menu_input_update(2);
        if (is_select_pressed) {
            if (current_menu_selection == 0) 
            {
                *state = STATE_PLAYING;
            }
            else if (current_menu_selection == 1) 
            {
                *state = STATE_MENU;
                current_menu_selection = 0;
            }
        }
        break;

    case STATE_GAMEOVER:
        if (is_select_pressed) {
            if (current_menu_selection == 0)
            {
                *state = STATE_NEWGAME;
            }
            else if (current_menu_selection == 1) 
            {
                *state = STATE_RANK;
            }
            else if (current_menu_selection == 2) 
            {
                *state = STATE_MENU;
                current_menu_selection = 0;
            }
        }
        break;

    case STATE_NEWGAME:
        frames = 0;
        score = 0;
        level = 1;
        score_display = 0;
        ship_init();
        hud_init();
        keyboard_init();
        fx_init();
        aliens_init();
        stars_init();
        shots_init();
        current_state = STATE_PLAYING;
        return;
        break;


    case STATE_RANK:
        menu_input_update(1);
        if (is_select_pressed) {
            *state = STATE_MENU;
            current_menu_selection = 0;
        }
        break;

    case STATE_INPUT_NAME:
        menu_input_update(1);
        if (is_select_pressed) {
            if (name_len > 0) { // 이름이 한 글자라도 있을 때만
                rank_add(player_name, score); // 삽입정렬
                rank_save();                  // 파일에 기록
                *state = STATE_RANK;          // 랭킹판으로 이동해서 점수 확인
                current_menu_selection = 0;
            }
        }
        break;

    case STATE_LEVEL_UP:        // 재작성자: 신제현
        menu_input_update(6);
        if (is_select_pressed)
        {
            *state = STATE_LEVEL_UP;
            
            switch (current_menu_selection)
            {
            case 0:
                // 공격력 증가 적용
             
                break;
            case 1:
                // 투사체 발사 수 증가
                break;
            case 2:
                // 공격 속도 증가
                break;
            case 3:
                // 이동 속도 증가
                break;
            case 4:
                // 체력 최대치 증가
                ship.lives *= 1.1;
                break;
            case 5:
                // 체력 지속 회복 
                break;
            }

            *state = STATE_PLAYING;
            current_menu_selection = 0;
        }
        break;
    }
}


/* --- Main --- */
int main()
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    disp_init();
    audio_init();

    must_init(al_init_image_addon(), "image");
    sprites_init();
    ui_init(); // UI 시트 로드
    hud_init();

    must_init(al_init_primitives_addon(), "primitives");
    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");
    must_init(al_reserve_samples(16), "reserve samples");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    keyboard_init();
    fx_init();
    shots_init();
    ship_init();
    aliens_init();
    stars_init();
    rank_init();

    frames = 0;
    score = 0;

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);

    while (1)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            game_state_update(&current_state, &done);

            // 작성자: 신제현
            if (key[ALLEGRO_KEY_ESCAPE] & KEY_SEEN)
            {
                if (current_state == STATE_PLAYING || current_state == STATE_PAUSE) 
                {
                    pause_resume_game(&current_state);
                }
            }

            redraw = true;
            ++frames;
            break;

        case ALLEGRO_EVENT_KEY_CHAR: // 실시간 문자 입력 처리 : 김병헌
            if (current_state == STATE_INPUT_NAME) 
            {
                if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && name_len > 0) 
                {
                    player_name[--name_len] = '\0';
                }
                else if (event.keyboard.unichar >= 32 && event.keyboard.unichar <= 126 && name_len < MAX_NAME_LEN - 1) 
                {
                    player_name[name_len++] = (char)event.keyboard.unichar;
                    player_name[name_len] = '\0';
                }
            }
            break;


        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        if (done)
            break;

        keyboard_update(&event);

        // 작성자: 김병헌
        if (redraw && al_is_event_queue_empty(queue))
        {
            disp_pre_draw();
            al_clear_to_color(al_map_rgb(0, 0, 0));

            stars_draw();

            switch (current_state)
            {
            case STATE_PLAYING:
                aliens_draw();
                shots_draw();
                fx_draw();
                ship_draw();
                hud_draw();
                break;

            case STATE_PAUSE:
                aliens_draw();
                shots_draw();
                fx_draw();
                ship_draw();
                hud_draw();
                al_draw_filled_rectangle(0, 0, BUFFER_W, BUFFER_H, al_map_rgba_f(0, 0, 0, 0.5));
                ui_draw_pause_menu();
                break;

            case STATE_MENU:
                ui_draw_main_menu();
                break;

            case STATE_GAMEOVER:
                ui_draw_gameover_menu();
                break;

            case STATE_RANK:
                ui_draw_rank_menu();
                break;

            case STATE_INPUT_NAME:
                ui_draw_input_name_menu();
                break;

            case STATE_LEVEL_UP:
                aliens_draw();
                shots_draw();
                fx_draw();
                ship_draw();
                hud_draw();
                al_draw_filled_rectangle(0, 0, BUFFER_W, BUFFER_H, al_map_rgba_f(0, 0, 0, 0.5));
                ui_draw_level_up_menu();
                break;
            }

            disp_post_draw();
            redraw = false;
        }
    }

    ui_deinit();
    sprites_deinit();
    hud_deinit();
    audio_deinit();
    disp_deinit();
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}