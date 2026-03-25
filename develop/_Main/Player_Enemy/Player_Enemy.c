#include "../Core.h"
#include "Player_Enemy.h"

#include "../Display.h"
#include "../Keyboard.h"
#include "../Sprites.h"
#include "../Audio.h"
#include "../Fx.h"

/* --- shot --- */


SHOT shots[SHOTS_N];

void shots_init()
{
    for (int i = 0; i < SHOTS_N; i++)
        shots[i].used = false;
}

bool shots_add(bool is_ship, bool straight, int x, int y)
{
    al_play_sample(
        sample_shot,
        0.3,
        0,
        is_ship ? 1.0 : between_f(1.5, 1.6),
        ALLEGRO_PLAYMODE_ONCE,
        NULL
    );

    for (int i = 0; i < SHOTS_N; i++)
    {
        if (shots[i].used)
            continue;

        shots[i].ship = is_ship;

        if (is_ship)
        {
            shots[i].x = x - (SHIP_SHOT_W / 2);
            shots[i].y = y;
        }

        else // alien
        {
            shots[i].x = x - (ALIEN_SHOT_W / 2);
            shots[i].y = y - (ALIEN_SHOT_H / 2);

            /* 플레이어를 향해 쏘도록 수정*/
            if (straight)
            {
                float angle = atan2(ship.cy - y, ship.cx - x);
                float alien_shot_speed = 2.5f;
                shots[i].dx = cos(angle) * alien_shot_speed;
                shots[i].dy = sin(angle) * alien_shot_speed;;
            }
            else 
            {
                //추후에 랜덤 방향이 아닌, 여러 모양의 탄막으로 만들 예정
                shots[i].dx = between(-2, 2);
                shots[i].dy = between(-2, 2);
            }

            // if the shot has no speed, don't bother
            if (!shots[i].dx && !shots[i].dy)
                return true;

            shots[i].frame = 0;
        }

        shots[i].frame = 0;
        shots[i].used = true;

        return true;
    }
    return false;
}

void shots_update()
{
    for (int i = 0; i < SHOTS_N; i++)
    {
        if (!shots[i].used)
            continue;

        if (shots[i].ship)
        {
            shots[i].y -= 5;

            if (shots[i].y < -SHIP_SHOT_H)
            {
                shots[i].used = false;
                continue;
            }
        }
        else // alien
        {
            shots[i].x += shots[i].dx;
            shots[i].y += shots[i].dy;

            if ((shots[i].x < -ALIEN_SHOT_W)
                || (shots[i].x > BUFFER_W)
                || (shots[i].y < -ALIEN_SHOT_H)
                || (shots[i].y > BUFFER_H) ) 
            {
                shots[i].used = false;
                continue;
            }
        }

        shots[i].frame++;
    }
}

bool shots_collide(bool ship, int cx, int cy, int w, int h)
{
    for (int i = 0; i < SHOTS_N; i++)
    {
        if (!shots[i].used)
            continue;

        // don't collide with one's own shots
        if (shots[i].ship == ship)
            continue;

        int sw, sh;
        if (ship)
        {
            sw = ALIEN_SHOT_W;
            sh = ALIEN_SHOT_H;

            if (collide_circle(cx, cy, 15, shots[i].x, shots[i].y, 5))
            {
                fx_add(true, shots[i].x + (sw / 2), shots[i].y + (sh / 2));
                shots[i].used = false;
                return true;
            }

        }
        else
        {
            sw = SHIP_SHOT_W;
            sh = SHIP_SHOT_H;
            if (collide(cx, cy, cx + w, cy + h, shots[i].x, shots[i].y, shots[i].x + sw, shots[i].y + sh))
            {
                fx_add(true, shots[i].x + (sw / 2), shots[i].y + (sh / 2));
                shots[i].used = false;
                return true;
            }
        }
       
    }

    return false;
}

void shots_draw()
{
    for (int i = 0; i < SHOTS_N; i++)
    {
        if (!shots[i].used)
            continue;

        int frame_display = (shots[i].frame / 2) % 2;
        
        /*수정; 총알 크기 수정*/
        ALLEGRO_BITMAP* current_shot = sprites.ship_shot[frame_display];
        int sw = al_get_bitmap_width(current_shot);
        int sh = al_get_bitmap_height(current_shot);

        if (shots[i].ship)
			al_draw_scaled_bitmap(sprites.ship_shot[frame_display],
				0, 0, sw, sh,
				shots[i].x - (SHIP_SHOT_W / 2),
				shots[i].y - (SHIP_SHOT_H / 2),
                SHIP_SHOT_W, SHIP_SHOT_H,
				0);

        else // alien
        {
            ALLEGRO_COLOR tint =
                frame_display
                ? al_map_rgb_f(1, 1, 1)
                : al_map_rgb_f(0.5, 0.5, 0.5)
                ;
            al_draw_tinted_bitmap(sprites.alien_shot, tint, shots[i].x, shots[i].y, 0);
        }
    }
}


/* --- Player --- */

SHIP ship;

void ship_init()
{
    ship.x = (BUFFER_W / 2) - (SHIP_W / 2);
    ship.y = (BUFFER_H / 2) - (SHIP_H / 2);
    ship.cx = ship.x + (SHIP_W / 2);
    ship.cy = ship.y + (SHIP_H / 2);
    ship.shot_timer = 0;
    ship.lives = 100;
    ship.respawn_timer = 0;
    ship.invincible_timer = 3;
}

void ship_update()
{
    if (ship.lives < 0)
        return;

    if (ship.respawn_timer)
    {
        ship.respawn_timer--;
        return;
    }

    if (key[ALLEGRO_KEY_LEFT])
        ship.x -= SHIP_SPEED;
    if (key[ALLEGRO_KEY_RIGHT])
        ship.x += SHIP_SPEED;
    if (key[ALLEGRO_KEY_UP])
        ship.y -= SHIP_SPEED;
    if (key[ALLEGRO_KEY_DOWN])
        ship.y += SHIP_SPEED;

    if (ship.x < 0)
        ship.x = 0;
    if (ship.y < 0)
        ship.y = 0;

    if (ship.x > SHIP_MAX_X)
        ship.x = SHIP_MAX_X;
    if (ship.y > SHIP_MAX_Y)
        ship.y = SHIP_MAX_Y;

    ship.cx = ship.x + (SHIP_W / 2);
    ship.cy = ship.y + (SHIP_H / 2);

    if (ship.invincible_timer)
        ship.invincible_timer--;
    else
    {
        //if (shots_collide(true, ship.x, ship.y, SHIP_W, SHIP_H))
		if (shots_collide(true, ship.cx, ship.cy, SHIP_W, SHIP_H))
        {
            fx_add(false, ship.cx, ship.cy);
            fx_add(false, ship.cx + 4, ship.cy + 2);
            fx_add(false, ship.cx - 2, ship.cy - 4);
            fx_add(false, ship.cx + 1, ship.cy - 5);

            ship.lives--;
            ship.respawn_timer = 90;
            ship.invincible_timer = 180;
        }
    }

    if (ship.shot_timer)
        ship.shot_timer--;
    else if (key[ALLEGRO_KEY_X])
    {
        if (shots_add(true, false, ship.cx, ship.cy))
            ship.shot_timer = 5;
    }
}

void ship_draw()
{
    if (ship.lives < 0)
        return;
    if (ship.respawn_timer)
        return;
    if (((ship.invincible_timer / 2) % 3) == 1)
        return;
    al_draw_scaled_bitmap(sprites.ship,
        0, 0,
        91, 91,
		ship.x, ship.y,
        SHIP_W, SHIP_H,
        0);
        
    //al_draw_bitmap(sprites.ship, ship.x, ship.y, 0);
}


/* --- Enemy --- */

ALIEN aliens[ALIENS_N];

void aliens_init()
{
    for (int i = 0; i < ALIENS_N; i++)
        aliens[i].used = false;
}

void aliens_update()
{
    int new_quota = //2초마다 랜덤 생성
        (frames % 120)
        ? 0
        : between(2, 4) //종류도 랜덤
        ;
    int new_x = between(10, BUFFER_W - 50);

    for (int i = 0; i < ALIENS_N; i++)
    {
        if (!aliens[i].used)
        {
            // if this alien is unused, should it spawn?
            if (new_quota > 0)
            {
                new_x += between(40, 80);
                if (new_x > (BUFFER_W - 60))
                    new_x -= (BUFFER_W - 60);

                aliens[i].x = new_x;

                aliens[i].y = between(-40, -30);
                aliens[i].type = between(0, ALIEN_TYPE_N);
                aliens[i].shot_timer = between(1, 99);
                aliens[i].blink = 0;
                aliens[i].used = true;

                switch (aliens[i].type)
                {
                case ALIEN_TYPE_BUG:
                    aliens[i].life = 4;
                    break;
                case ALIEN_TYPE_ARROW:
                    aliens[i].life = 2;
                    break;
                case ALIEN_TYPE_THICCBOI:
                    aliens[i].life = 12;
                    break;
                }

                new_quota--;
            }
            continue;
        }

        aliens[i].cx = aliens[i].x + (ALIEN_W[aliens[i].type] / 2);
        aliens[i].cy = aliens[i].y + (ALIEN_H[aliens[i].type] / 2);

        switch (aliens[i].type)
        {
        case ALIEN_TYPE_BUG:
            if (frames % 2) {
                if (aliens[i].cx > ship.cx)           //추격 기능 추가 , 차후에 이속에 따라 이동하는거 넣어야함
                {
                    aliens[i].x--;
                }
                if (aliens[i].cx < ship.cx)
                {
                    aliens[i].x++;
                }
                if (aliens[i].cy > ship.cy)
                {
                    aliens[i].y--;
                }
                if (aliens[i].cy < ship.cy)
                {
                    aliens[i].y++;
                }
                /*aliens[i].y++;*/
            }

            break;

        case ALIEN_TYPE_ARROW:
        {
            if (aliens[i].cx > ship.cx)
            {
                aliens[i].x--;
            }
            if (aliens[i].cx < ship.cx)
            {
                aliens[i].x++;
            }
            if (aliens[i].cy > ship.cy)
            {
                aliens[i].y--;
            }
            if (aliens[i].cy < ship.cy)
            {
                aliens[i].y++;
            }
        }
        //aliens[i].y++;
        break;

        case ALIEN_TYPE_THICCBOI:
            if (!(frames % 4)) {
                if (aliens[i].cx > ship.cx)
                {
                    aliens[i].x--;
                }
                if (aliens[i].cx < ship.cx)
                {
                    aliens[i].x++;
                }
                if (aliens[i].cy > ship.cy)
                {
                    aliens[i].y--;
                }
                if (aliens[i].cy < ship.cy)
                {
                    aliens[i].y++;
                }
            }
            //aliens[i].y++;
            break;
        }

        if (aliens[i].y >= BUFFER_H)
        {
            aliens[i].used = false;
            continue;
        }

        if (aliens[i].blink)
            aliens[i].blink--;

        if (shots_collide(false, aliens[i].x, aliens[i].y, ALIEN_W[aliens[i].type], ALIEN_H[aliens[i].type]))
        {
            aliens[i].life--;
            aliens[i].blink = 4;
        }



        if (aliens[i].life <= 0)
        {
            fx_add(false, aliens[i].cx, aliens[i].cy);

            switch (aliens[i].type)
            {
            case ALIEN_TYPE_BUG:
                score += 200;
                break;

            case ALIEN_TYPE_ARROW:
                score += 150;
                break;

            case ALIEN_TYPE_THICCBOI:
                score += 800;
                fx_add(false, aliens[i].cx - 10, aliens[i].cy - 4);
                fx_add(false, aliens[i].cx + 4, aliens[i].cy + 10);
                fx_add(false, aliens[i].cx + 8, aliens[i].cy + 8);
                break;
            }

            aliens[i].used = false;
            continue;
        }

        aliens[i].shot_timer--;
        if (aliens[i].shot_timer == 0)
        {
            switch (aliens[i].type)
            {
            case ALIEN_TYPE_BUG:    //메테오는 총을 쏘지 않는다.
                /*shots_add(false, false, cx, cy);
                aliens[i].shot_timer = 150;*/
                break;
            case ALIEN_TYPE_ARROW:
                shots_add(false, true, aliens[i].cx, aliens[i].cy);
                aliens[i].shot_timer = 80;
                break;
            case ALIEN_TYPE_THICCBOI:
                shots_add(false, true, aliens[i].cx - 5, aliens[i].cy);
                shots_add(false, true, aliens[i].cx + 5, aliens[i].cy);
                shots_add(false, true, aliens[i].cx - 5, aliens[i].cy + 8);
                shots_add(false, true, aliens[i].cx + 5, aliens[i].cy + 8);
                aliens[i].shot_timer = 200;
                break;
            }
        }
    }
}

void aliens_draw()
{
    for (int i = 0; i < ALIENS_N; i++)
    {
        if (!aliens[i].used)
            continue;
        if (aliens[i].blink > 2)
            continue;

        al_draw_scaled_bitmap(sprites.alien[aliens[i].type],
            0, 0,
            101, 84,
            aliens[i].x, aliens[i].y,
            ALIEN_W[aliens[i].type], ALIEN_H[aliens[i].type],
            0);
        //al_draw_bitmap(sprites.alien[aliens[i].type], aliens[i].x, aliens[i].y, 0);
    }
}