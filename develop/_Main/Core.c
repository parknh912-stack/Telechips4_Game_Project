/*
텔레칩스 임베디드 스쿨 4기
게임프로젝트
팀원 : 박남현, 천원석, 신제현, 김병헌
프로젝트 명 : Space Survivor
*/

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
#include "Item/Item.h"
#include "Level_UP/Level_up.h"			// 레벨 업(스탯 강화) / 0327 신제현
#include "Stage/Stage.h"                // 스테이지 /0329 박남현
#include "Backgrounds/Background.h"     // 배경

/* --- General --- */
long frames = 0;
long score = 0;
int level = 1;
int stage_num = 0;                      //0329 박남현
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



// 작성자 : 박남현
/* --- 원형 충돌 --- */
bool collide_circle(int cx1, int cy1, int r1, int cx2, int cy2, int r2) 
{
    long dx = cx2 - cx1;
    long dy = cy2 - cy1;

    long radium_Sum = r1 + r2;
    long radium_Square = (radium_Sum * radium_Sum);
    long distance = (dx * dx) + (dy * dy);

    return (distance <= radium_Square);     //반지름의 제곱이 더 크면 충돌
}

/* --- Rank --- */ //0327 김병헌
RANK ranking[RANK_QUEUE_SIZE]; // Rank.h의 extern 변수 실제 선언
int rank_count = 0;
char player_name[MAX_NAME_LEN] = "";
extern int name_len = 0;

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

// 0327 신제현
// 게임 초기화 동작을 함수로 모듈화
void game_state_init(void)
{
    frames = 0;
    score = 0;
    level = 1;
    score_display = 0;
    stage_alert_timer = al_get_time();  // 0330 신제현 - 스테이지 1 진입 알림

    stage_init();   //0329 박남현
    ship_init();
    backgound_init();
    hud_init();
    keyboard_init();
    fx_init();
    aliens_init();
    item_init();
    stars_init();
    shots_init();

    current_state = STATE_PLAYING;
}

// 작성자: 신제현
void game_state_update(STATE* state, bool* done)
{
    bool is_select_pressed = (key[ALLEGRO_KEY_ENTER] & KEY_SEEN) || (key[ALLEGRO_KEY_SPACE] & KEY_SEEN);

    switch (*state)
    {
    case STATE_MENU:
        menu_input_update(5);
        if (is_select_pressed)
        {
            if (current_menu_selection == 0)
            {
                *state = STATE_NEWGAME;
            }

            else if (current_menu_selection == 1)
            {
                *state = STATE_ABOUT;//0328 김병헌 겜설명
            }
            else if (current_menu_selection == 2)
            {
                *state = STATE_RANK;
                current_menu_selection = 0;
            }
            else if (current_menu_selection == 3)
            {
                *done = true;
            }
            else if (current_menu_selection == 4)//0330 김병헌 잘 나오는지 테스트
            {
                *state = STATE_ENDING_SCENE;
            }
        }
        break;

    case STATE_PLAYING:
        stage_update(); //0329 박남현

        // 0330 신제현 - 버그 수정
        // 스테이지 클리어로 상태가 바뀌었으면 나머지를 스킵할 것
        if (*state != STATE_PLAYING)
            break;

        fx_update();
        shots_update();
        stars_update();
        ship_update();
        aliens_update();
        item_update();  //0327 박남현
        hud_update();
        // 작성자: 신제현
        // 레벨 증가하는 점수에 따라 해당 조건 검사
        *state = check_level_up(score);
        current_menu_selection = 0;

        //0327 김병헌
        if (ship.curr_lifes < 0)
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
        if (is_select_pressed)
        {
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
        if (is_select_pressed)
        {
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
        game_state_init();		// 수정, 0327 신제현
        return;

    case STATE_RANK:
        menu_input_update(1);
        if (is_select_pressed)
        {
            *state = STATE_MENU;
            current_menu_selection = 0;
        }
        break;

    case STATE_INPUT_NAME:
        menu_input_update(1);
        if (is_select_pressed)
        {
            if (name_len > 0)
            { // 이름이 한 글자라도 있을 때만
                rank_add(player_name, score); // 삽입정렬
                rank_save();                  // 파일에 기록
                *state = STATE_RANK;          // 랭킹판으로 이동해서 점수 확인
                current_menu_selection = 0;
            }
        }
        break;

    case STATE_LEVEL_UP:        // 0326 신제현
        menu_input_update(6);
        if (is_select_pressed)
        {
            *state = STATE_LEVEL_UP;

            // 재작성자: 신제현
            // 캐릭터 강화 단순화하여 구현(0326 신제현)
            // 강화 함수를 따로 구현하여 적용(0327 신제현)
            switch (current_menu_selection)
            {
            case 0:
                // 공격력 증가 적용
                printf("Before: %d\n", ship.damage);
                damage_up();
                printf("After: %d\n", ship.damage);
                break;
            case 1:
                // 투사체 발사 수 증가
                printf("Before: %d\n", ship.shot_count);
                shot_count_up();
                printf("After: %d\n", ship.shot_count);
                break;
            case 2:
                // 공격 속도 증가
                printf("Before: %f\n", ship.fire_rate);
                fire_rate_up();
                printf("After: %f\n", ship.fire_rate);
                break;
            case 3:
                // 이동 속도 증가
                printf("Before: %f\n", ship.speed);
                speed_up();
                printf("After: %f\n", ship.speed);
                break;
            case 4:
                // 체력 최대치 증가
                printf("Before: %d\n", ship.max_lifes);
                max_lifes_up();
                printf("After: %d\n", ship.max_lifes);
                break;
            case 5:
                // 체력 즉시 회복
                printf("Before: %d\n", ship.curr_lifes);
                instant_lifes();
                printf("After: %d\n", ship.curr_lifes);
                break;
            }
            *state = STATE_PLAYING;
            current_menu_selection = 0;
        }
        break;

    case STATE_ABOUT: //0328 김병헌 겜설명
        menu_input_update(1);//0328 김병헌 겜설명
        if (is_select_pressed)
        {

            *state = STATE_MENU;//0328 김병헌 겜설명
            current_menu_selection = 0;//0328 김병헌 겜설명
        }
        break;

    case STATE_ENDING_SCENE: // 0330 김병헌 엔딩 씬
    {
        static int ending_frame_counter = 0;
        if (ending_frame_counter < 800)ending_frame_counter++;

        menu_input_update(1);
        if (ending_frame_counter >= 800)
        {
            if (is_select_pressed)
            {
                *state = STATE_ENDING;
                current_menu_selection = 0;
                ending_frame_counter = 0;
            }
        }
    }
    break;

    case STATE_ENDING:          // 0330 신제현 - 엔딩 화면
        menu_input_update(2);

        if (is_select_pressed)
        {
            if (current_menu_selection == 0)
            {
                // 랭킹 진입 체크 후 이름 입력으로
                if (rank_count < MAX_RANKING || score > ranking[MAX_RANKING - 1].score)
                {
                    *state = STATE_INPUT_NAME;
                    name_len = 0;
                    player_name[0] = '\0';
                }
                else
                {
                    *state = STATE_RANK;  // 랭킹권 아니면 보기만
                }
            }
            else if (current_menu_selection == 1)
            {
                *state = STATE_MENU;
                current_menu_selection = 0;
            }
        }
        break;

    }
}

void camera_apply(ALLEGRO_TRANSFORM* trans)
{
    float camera_x = ship.cx - 640;
    float camera_y = ship.cy - 360;

    al_identity_transform(trans);
    al_translate_transform(trans, -camera_x, -camera_y);
    al_use_transform(trans);
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
    backgound_init();
    hud_init();

    must_init(al_init_primitives_addon(), "primitives");
    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");
    must_init(al_reserve_samples(16), "reserve samples");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    keyboard_init();
    stage_init();               //0329
    fx_init();
    shots_init();
    ship_init();
    aliens_init();
    item_init();
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

            // 0330 신제현 - STATE_PLAYING 상태에 있을 때에만 프레임 수를 증가
            // 그 외의 경우는 증가시키지 않음
            if (current_state == STATE_PLAYING)
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
        ALLEGRO_TRANSFORM trans;

        int time = al_get_time();

        // 작성자: 김병헌
        if (redraw && al_is_event_queue_empty(queue))
        {
            disp_pre_draw();
            al_clear_to_color(al_map_rgb(0, 0, 0));

            stars_draw();

            switch (current_state)
            {
            case STATE_MENU:
                ui_draw_main_menu();
                audio_stop_bgm();
                break;

            case STATE_PLAYING:
                camera_apply(&trans);

                background_draw();  //0329
                aliens_draw();
                item_draw(); //0327
                shots_draw();
                fx_draw();

                al_identity_transform(&trans);
                al_use_transform(&trans);

                ship_draw();
                hud_draw();
                break;

            case STATE_PAUSE:
                camera_apply(&trans);

                background_draw();  //0329
                aliens_draw();
                item_draw();
                shots_draw();
                fx_draw();

                al_identity_transform(&trans);
                al_use_transform(&trans);

                ship_draw();
                hud_draw();
                al_draw_filled_rectangle(0, 0, BUFFER_W, BUFFER_H, al_map_rgba_f(0, 0, 0, 0.5));
                ui_draw_pause_menu();
                break;

            case STATE_INPUT_NAME:
                ui_draw_input_name_menu();
                break;

            case STATE_GAMEOVER:
                ui_draw_gameover_menu();
                break;

            case STATE_RANK:
                ui_draw_rank_menu();
                break;

            case STATE_LEVEL_UP:
                camera_apply(&trans);

                background_draw();  //0329
                aliens_draw();
                item_draw();    //0327
                shots_draw();
                fx_draw();

                al_identity_transform(&trans);
                al_use_transform(&trans);

                ship_draw();
                hud_draw();
                al_draw_filled_rectangle(0, 0, BUFFER_W, BUFFER_H, al_map_rgba_f(0, 0, 0, 0.5));
                ui_draw_level_up_menu();
                break;

            case STATE_ABOUT:
                ui_draw_h2p_menu();
                break;

            case STATE_ENDING_SCENE:
                ui_draw_clear_menu();
                break;

            case STATE_ENDING:
                hud_draw();
                al_draw_filled_rectangle(0, 0, BUFFER_W, BUFFER_H, al_map_rgba_f(0, 0, 0, 0.5));
                ui_draw_ending_menu();
                break;
            }
           

            disp_post_draw();
            redraw = false;
        }
    }

    background_deinit();
    ui_deinit();
    sprites_deinit();
    hud_deinit();
    audio_deinit();
    disp_deinit();
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}