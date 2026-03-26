#include "../Core.h"
#include "Player_Enemy.h"

#include "../Display.h"
#include "../Keyboard.h"
#include "../Sprites.h"
#include "../Audio.h"
#include "../Fx.h"


//bool collide_x1(int ax1, int bx2)
//{
//    if (ax1 < bx2) return true;
//}
//bool collide_x2(int ax2, int bx1)
//{
//    if (ax2 > bx1) return true;
//}
//bool collide_y1(int ay1, int by2)
//{
//    if (ay1 < by2) return true;
//}
//bool collide_y2(int ay2, int by1)
//{
//    if (ay2 > by1) return true;
//}




/* --- shot --- */


SHOT shots[SHOTS_N];

void shots_init()
{
    for (int i = 0; i < SHOTS_N; i++)
        shots[i].used = false;
}

bool shots_add(bool ship, bool straight, int x, int y)
{
    al_play_sample(
        sample_shot,
        0.3,
        0,
        ship ? 1.0 : between_f(1.5, 1.6),
        ALLEGRO_PLAYMODE_ONCE,
        NULL
    );

    for (int i = 0; i < SHOTS_N; i++)
    {
        if (shots[i].used)
            continue;

        shots[i].ship = ship;

        if (ship)
        {
            shots[i].x = x - (SHIP_SHOT_W / 2);
            shots[i].y = y;
        }
        else // alien
        {
            shots[i].x = x - (ALIEN_SHOT_W / 2);
            shots[i].y = y - (ALIEN_SHOT_H / 2);

            if (straight)
            {
                shots[i].dx = 0;
                shots[i].dy = 2;
            }
            else
            {

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
                || (shots[i].y > BUFFER_H)
                ) {
                shots[i].used = false;
                continue;
            }
        }

        shots[i].frame++;
    }
}

bool shots_collide(bool ship, int x, int y, int w, int h)
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
        }
        else
        {
            sw = SHIP_SHOT_W;
            sh = SHIP_SHOT_H;
        }

        if (collide(x, y, x + w, y + h, shots[i].x, shots[i].y, shots[i].x + sw, shots[i].y + sh))
        {
            fx_add(true, shots[i].x + (sw / 2), shots[i].y + (sh / 2));
            shots[i].used = false;
            return true;
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

        if (shots[i].ship)
            al_draw_bitmap(sprites.ship_shot[frame_display], shots[i].x, shots[i].y, 0);
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
    ship.x = (BUFFER_W / 2) - (SHIP_W / 2); //왼쪽 위를 가르킴
    ship.y = (BUFFER_H / 2) - (SHIP_H / 2);
    ship.cx = ship.x + (SHIP_W / 2);        // 배의 가운데 x좌표
    ship.cy = ship.y + (SHIP_H / 2);
    ship.shot_timer = 0;
    ship.lives = 10;
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

    if (ship.invincible_timer)
        ship.invincible_timer--;
    else
    {
        if (shots_collide(true, ship.x, ship.y, SHIP_W, SHIP_H))
        {
            int x = ship.x + (SHIP_W / 2);
            int y = ship.y + (SHIP_H / 2);
            fx_add(false, x, y);
            fx_add(false, x + 4, y + 2);
            fx_add(false, x - 2, y - 4);
            fx_add(false, x + 1, y - 5);

            ship.lives--;
            ship.respawn_timer = 90;
            ship.invincible_timer = 180;
        }
    }

    if (ship.shot_timer)
        ship.shot_timer--;
    else if (key[ALLEGRO_KEY_X])
    {
        int x = ship.x + (SHIP_W / 2);
        if (shots_add(true, false, x, ship.y))
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
  //  al_draw_scaled_bitmap(sprites.ship,
  //      0, 0,
  //      91, 91,
		//ship.x, ship.y,
  //      SHIP_W, SHIP_H,
  //      0);
        
    al_draw_bitmap(sprites.ship, ship.x, ship.y, 0);
}


/* --- Enemy --- */

ALIEN aliens[ALIENS_N];

void aliens_init()
{
    for (int i = 0; i < ALIENS_N; i++)
        aliens[i].used = false;
}

void aliens_collide()
{
    for (int i = 0 ; i < ALIENS_N; i++)
    {
        for (int j = i + 1; j < ALIENS_N; j++)
        {
            aliens[i].ax2 = aliens[i].x + ALIEN_W[aliens[i].type];
            aliens[i].ay2 = aliens[i].y + ALIEN_H[aliens[i].type];

            aliens[j].bx2 = aliens[j].x + ALIEN_W[aliens[j].type];
            aliens[j].by2 = aliens[j].y + ALIEN_H[aliens[j].type];

            if (collide(aliens[i].x, aliens[i].y, aliens[i].ax2, aliens[i].ay2, aliens[j].x, aliens[j].y, aliens[j].bx2, aliens[j].by2))
            {
                if (aliens[i].ax2 > aliens[j].x)
                {
                    aliens[i].x -= 1;
                    aliens[j].x += 1;
                }
                if (aliens[i].x >= aliens[j].bx2)
                {
                    aliens[i].x += 1;
                    aliens[j].x -= 1;
                }
                if (aliens[i].ay2 < aliens[j].y)
                {
                    aliens[j].y -= 1;
                    aliens[i].y += 1;
                }
                if (aliens[i].y >= aliens[j].by2)
                {
                    aliens[i].y += 1;
                    aliens[j].y += 1;
                }
            }
        }
    }
}

void aliens_update()
{
    int aliens_spawn_location = between(0, 4);
    int new_quota = //2초마다 랜덤 생성
        (frames % 120)
        ? 0
        : between(2, 4) //종류도 랜덤
        ;
    int new_x = between(10, BUFFER_W - 50);
    int new_y = between(10, BUFFER_H - 50);

    for (int i = 0; i < ALIENS_N; i++)
    {
        if (!aliens[i].used)
        {
            // if this alien is unused, should it spawn?
            if (new_quota > 0)
            {
                // 상 위치에서 스폰
                if (aliens_spawn_location == 0)
                {
                    // 50 ~ 349
                    new_x += between(40, 80);
                    // 만약 260을 넘는다면 new_x = 1 ~ 88, 260 이하라면 50 ~ 259 
                    if (new_x > (BUFFER_W - 60))
                        new_x -= (BUFFER_W - 60);

                    aliens[i].x = new_x;
                    aliens[i].y = between(-40, -30);
                }

                // 하 위치에서 스폰 
                if (aliens_spawn_location == 1)
                {
                    new_x += between(40, 80);
                    if (new_x > (BUFFER_W - 60))
                        new_x -= (BUFFER_W - 60);

                    aliens[i].x = new_x;
                    aliens[i].y = 240;
                }
                // 좌 위치에서 스폰
                if (aliens_spawn_location == 2)
                {
                    new_y += between(40, 80);
                    if (new_y > (BUFFER_H - 40))
                        new_y -= BUFFER_H - 40;
                    aliens[i].x = between(-40, -30);
                    aliens[i].y = new_y;
                }
                // 우 위치에서 스폰
                if (aliens_spawn_location == 3)
                {
                    new_y += between(40, 80);
                    if (new_y > (BUFFER_H - 40))
                        new_y -= BUFFER_H - 40;
                    aliens[i].x = between(360, 390);
                    aliens[i].y = new_y;
                }

                //aliens[i].y = between(-40, -30);
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
        aliens_collide();

        switch (aliens[i].type)
        {
        case ALIEN_TYPE_BUG:
            if (frames % 2) {
                
                if (aliens[i].x > ship.x)           //추격 기능 추가 , 차후에 이속에 따라 이동하는거 넣어야함
                {
                    aliens[i].x--;
                }
                if (aliens[i].x < ship.x)
                {
                    aliens[i].x++;
                }
                if (aliens[i].y > ship.y)
                {
                    aliens[i].y--;
                }
                if (aliens[i].y < ship.y)
                {
                    aliens[i].y++;
                }
                /*aliens[i].y++;*/
            }

            break;

        case ALIEN_TYPE_ARROW:
        {
            if (aliens[i].x > ship.x)
            {
                aliens[i].x--;
            }
            if (aliens[i].x < ship.x)
            {
                aliens[i].x++;
            }
            if (aliens[i].y > ship.y)
            {
                aliens[i].y--;
            }
            if (aliens[i].y < ship.y)
            {

                aliens[i].y++;
            }
        }
            //aliens[i].y++;
            break;

        case ALIEN_TYPE_THICCBOI:
            if (!(frames % 4)) {
                if (aliens[i].x > ship.x)
                {
                    aliens[i].x--;
                }
                if (aliens[i].x < ship.x)
                {

                    aliens[i].x++;
                }
                if (aliens[i].y > ship.y)
                {
                    aliens[i].y--;
                }
                if (aliens[i].y < ship.y)
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

        int cx = aliens[i].x + (ALIEN_W[aliens[i].type] / 2);
        int cy = aliens[i].y + (ALIEN_H[aliens[i].type] / 2);

        if (aliens[i].life <= 0)
        {
            fx_add(false, cx, cy);

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
                fx_add(false, cx - 10, cy - 4);
                fx_add(false, cx + 4, cy + 10);
                fx_add(false, cx + 8, cy + 8);
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
                shots_add(false, true, cx, aliens[i].y);
                aliens[i].shot_timer = 80;
                break;
            case ALIEN_TYPE_THICCBOI:
                shots_add(false, true, cx - 5, cy);
                shots_add(false, true, cx + 5, cy);
                shots_add(false, true, cx - 5, cy + 8);
                shots_add(false, true, cx + 5, cy + 8);
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

        //al_draw_scaled_bitmap(sprites.alien[aliens[i].type],
        //    0, 0,
        //    101, 84,
        //    aliens[i].x, aliens[i].y,
        //    ALIEN_W[aliens[i].type], ALIEN_W[aliens[i].type],
        //    0);
        al_draw_bitmap(sprites.alien[aliens[i].type], aliens[i].x, aliens[i].y, 0);
    }
}