#include "Core.h"
#include "Keyboard.h"
#include "Sprites.h"
#include "Display.h"
#include "Audio.h"
#include "Fx.h"
#include "Player_Enemy/Player_Enemy.h"
#include "UI/UI.h"

/* --- General --- */
long frames;
long score;
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
    default:
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
        break;

    case STATE_PAUSE:
        menu_input_update(2);
        if (is_select_pressed) {
            if (current_menu_selection == 0) {
                *state = STATE_PLAYING;
            }
            else if (current_menu_selection == 1) {
                *state = STATE_MENU;
                current_menu_selection = 0;
            }
        }
        break;

    case STATE_GAMEOVER:
        menu_input_update(2);
        if (is_select_pressed) {
            if (current_menu_selection == 0) {
                *state = STATE_PLAYING;
            }
            else if (current_menu_selection == 1) {
                *state = STATE_MENU;
                current_menu_selection = 0;
            }
        }
        break;

    case STATE_RANK:
    case STATE_INPUT_NAME:
        menu_input_update(1);
        if (is_select_pressed) {
            *state = STATE_MENU;
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