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
/* --- General --- */

long frames;
long score;

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
bool collide_circle(int cx1, int cy1, int r1, int cx2, int cy2, int r2) {
    long dx = cx2 - cx1;
    long dy = cy2 - cy1;

    long radium_Sum = r1 + r2;
    long radium_Square = (radium_Sum * radium_Sum);
    long distance = (dx * dx) + (dy * dy);

    return (distance <= radium_Square);     //반지름의 제곱이 더 크면 충돌
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
            fx_update();
            shots_update();
            stars_update();
            ship_update();
            aliens_update();
            hud_update();

            if (key[ALLEGRO_KEY_ESCAPE])
                done = true;

            redraw = true;
            frames++;
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        if (done)
            break;

        keyboard_update(&event);

        if (redraw && al_is_event_queue_empty(queue))
        {
            disp_pre_draw();
            al_clear_to_color(al_map_rgb(0, 0, 0));

            stars_draw();
            aliens_draw();
            shots_draw();
            fx_draw();
            ship_draw();

            hud_draw();

            disp_post_draw();
            redraw = false;
        }
    }

    sprites_deinit();
    hud_deinit();
    audio_deinit();
    disp_deinit();
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}