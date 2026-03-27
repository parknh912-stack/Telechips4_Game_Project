#include "../Core.h"
#include "Player_Enemy.h"

#include "../Display.h"
#include "../Keyboard.h"
#include "../Sprites.h"
#include "../Audio.h"
#include "../Fx.h"
#include "../Item/Item.h"

/* --- shot --- */

SHOT shots[SHOTS_N];

void shots_init()
{
    for (int i = 0; i < SHOTS_N; i++)
        shots[i].used = false;
}

//작성자 : 박남현
bool shots_add(bool is_ship, bool straight, float x, float y)
{
    //sound 설정
    al_play_sample(
        sample_shot,
        0.3,
        0,
        is_ship ? 1.0 : between_f(1.5, 1.6),
        ALLEGRO_PLAYMODE_ONCE,
        NULL
    );

	// 공통 세팅
	float speed = is_ship ? 4.0f : 3.0f;
	int count = is_ship ? ship.shot_count : 1;

	float spread_gap = 0.2f;
	float target_x, target_y;
	bool has_target = false;
	bool has_shot_created_success = false;
	//shot이 아군것인지 적군것인지 확인 후 target 좌표 설정
	if (is_ship)
	{
        int count = ship.shot_count;
		int target_idx = get_closet_enemy();    //가장 가까운 적을 조준하는 함수 및 기능
		if (target_idx != -1)
		{
			target_x = aliens[target_idx].cx;
			target_y = aliens[target_idx].cy;
			has_target = true;
		}
		else
			return false;
	}
	else //alien
	{
		if (straight)
		{
			target_x = ship.cx;
			target_y = ship.cy;
			has_target = true;
		}
	}

	if (has_target)
	{
		float base_angle = atan2f(target_y - y, target_x - x);
		float start_angle = base_angle - ((count - 1) / 2.0f) * spread_gap;

		for (int i = 0; i < count; ++i)
		{
			float curr_angle = start_angle + i * spread_gap;
			float dx = cos(curr_angle) * speed;
			float dy = sin(curr_angle) * speed;
			if (shots_create_instance(is_ship, x, y, dx, dy))
				has_shot_created_success = true;
		}
	}
	else if (!is_ship && !straight)
	{
		if (shots_create_instance(false, x, y, between_f(-2.0f, 2.0f), between_f(-2.0f, 2.0f)))
			has_shot_created_success = true;
	}
	return has_shot_created_success;
}

bool shots_create_instance(bool is_ship, float x, float y, float dx, float dy)
{
    if (dx == 0 && dy == 0)
        return true;
    for (int i = 0; i < SHOTS_N; ++i)
    {
        if (shots[i].used) continue;
        shots[i].ship = is_ship;
        shots[i].used = true;
        shots[i].frame = 0;

        shots[i].x = x - (is_ship ? (SHIP_SHOT_W / 2) : (ALIEN_SHOT_W / 2));
        shots[i].y = y - (is_ship ? 0 : (ALIEN_SHOT_H / 2));
        shots[i].dx = dx;
        shots[i].dy = dy;
        
        return true;
    }
    return false;
}


// 작성자 : 박남현
/* --- 가장 가까운 적 식별 --- */
int get_closet_enemy()
{
    int target_idx = -1;
    float min_distance = 999999.0f;

    for (int i = 0; i < ALIENS_N; ++i)
    {
        if (!aliens[i].used) continue;

        float dx = aliens[i].cx - ship.cx;
        float dy = aliens[i].cy - ship.cy;
        float square_distance = (dx * dx) + (dy * dy);
        if (min_distance > square_distance)
        {
            min_distance = square_distance;
            target_idx = i;
        }
    }
    return target_idx;
}

//작성자 : 박남현
void shots_update()
{
    for (int i = 0; i < SHOTS_N; i++)
    {
        if (!shots[i].used)
            continue;

        if (shots[i].ship)
        {
            shots[i].x += shots[i].dx;
            shots[i].y += shots[i].dy;

            // 작성자 : 박남현
            // 외곽 범위 수정
            if (shots[i].x < -SHIP_SHOT_H || shots[i].y < -SHIP_SHOT_H ||
                shots[i].x > BUFFER_W + SHIP_SHOT_W || shots[i].y > BUFFER_W + SHIP_SHOT_W)
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
                || (shots[i].y > BUFFER_H))
            {
                shots[i].used = false;
                continue;
            }
        }

        shots[i].frame++;
    }
}

//작성자 : 박남현
bool shots_collide(bool ship, float cx, float cy, float w, float h)
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
            // 작성자 : 박남현
            /* 적 -> 아군인 경우는 원형 충돌 */
            if (collide_circle(cx, cy, SHIP_R, shots[i].x, shots[i].y, ALIEN_SHOT_R))
            {
                fx_add(true, shots[i].x + (sw / 2.0f), shots[i].y + (sh / 2.0f));
                shots[i].used = false;
                return true;
            }

        }
        else
        {
            sw = SHIP_SHOT_W;
            sh = SHIP_SHOT_H;
            /* 아군 -> 적인 경우는 AABB 충돌 (차후 원형 충돌로 바꿀 예정) */
            if (collide(cx, cy, cx + w, cy + h, shots[i].x, shots[i].y, shots[i].x + sw, shots[i].y + sh))
            {
                fx_add(true, shots[i].x + (sw / 2.0f), shots[i].y + (sh / 2.0f));
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
        // 작성자 : 박남현
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
                ? al_map_rgb_f(1, 0, 0)
                : al_map_rgb_f(0.9, 0, 0)
                ;
            // 작성자 : 박남현
            // 작게 바꿈
            al_draw_tinted_scaled_bitmap(sprites.alien_shot, tint,
                0, 0, 41, 41,
                shots[i].x, shots[i].y,
                ALIEN_SHOT_W, ALIEN_SHOT_H,
                0);
            //al_draw_tinted_bitmap(sprites.alien_shot, tint, shots[i].x, shots[i].y, 0);
        }
    }
}


/* --- Player --- */

SHIP ship;

// 작성자 : 박남현
// 거의 전체적으로 변경되었음.
void ship_init()
{
    ship.x = (BUFFER_W / 2) - (SHIP_W / 2);
    ship.y = (BUFFER_H / 2) - (SHIP_H / 2);
    ship.cx = ship.x + (SHIP_W / 2);
    ship.cy = ship.y + (SHIP_H / 2);

    ship.speed = 3.0f;          //이동속도, 수정가능
    ship.fire_rate = 1.0f;      //초당 공격 횟수 (수정가능)
    ship.shot_timer = 60;       //shot_timer
    ship.damage = 10;           //데미지, int      (수정가능)
    ship.shot_count = 1;        //투사체 수 (홀수개만)
    ship.max_lifes = 100;       //최대 체력
    ship.curr_lifes = 100;      //현재 체력
    ship.barrier = false;

    ship.invincible_timer = 3;  //무적시간
}

void ship_update()
{
    if (ship.curr_lifes < 0)
    {
        current_state = STATE_GAMEOVER;
        ship_init();
        return;
    }

    if (key[ALLEGRO_KEY_LEFT])
        ship.x -= ship.speed;
    if (key[ALLEGRO_KEY_RIGHT])
        ship.x += ship.speed;
    if (key[ALLEGRO_KEY_UP])
        ship.y -= ship.speed;
    if (key[ALLEGRO_KEY_DOWN])
        ship.y += ship.speed;

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
        if (shots_collide(true, ship.cx, ship.cy, SHIP_W, SHIP_H))
        {
            fx_add(false, ship.cx, ship.cy);
            fx_add(false, ship.cx + 4, ship.cy + 2);
            fx_add(false, ship.cx - 2, ship.cy - 4);
            fx_add(false, ship.cx + 1, ship.cy - 5);
            if (!ship.barrier) 
            {
                ship.curr_lifes--;
                ship.invincible_timer = 1;
            }
            else
            {
                ship.invincible_timer = 180;
                ship.barrier = false;
            }
        }

        /* 적과 직접 충돌 시에도 */
        if (ship_collide(ship.cx, ship.cy))
        {
            //fx_add(false, ship.cx, ship.cy);
			//fx_add(false, ship.cx + 4, ship.cy + 2);
			//fx_add(false, ship.cx - 2, ship.cy - 4);
			//fx_add(false, ship.cx + 1, ship.cy - 5);
			if (!ship.barrier)
			{
				ship.curr_lifes--;
				ship.invincible_timer = 1;
			}
			else
			{
				ship.invincible_timer = 180;
                ship.barrier = false;
			}
        }
    }

 

    // 작성자 : 박남현
    // 일정 시간마다 자동 공격
    if (ship.shot_timer)
        ship.shot_timer--;
    if (ship.shot_timer <= 0) {

		shots_add(true, false, ship.cx, ship.cy);
		ship.shot_timer = 60.0 / ship.fire_rate;
	}
    item_collide(ship.cx, ship.cy);
}

void ship_draw()
{
    if (ship.curr_lifes < 0)
        return;
    if (((ship.invincible_timer / 2) % 3) == 1)
        return;
    // 작성자 : 박남현
    // 크기 다르게 바꿈
    if (ship.barrier) {
        al_draw_scaled_bitmap(sprites.barrier,
            0, 0,
            al_get_bitmap_width(sprites.barrier), 
            al_get_bitmap_height(sprites.barrier),
			ship.cx - (ITEMS_BARRIER_W / 2), ship.cy - (ITEMS_BARRIER_H / 2),
            ITEMS_BARRIER_W, ITEMS_BARRIER_H,
            0);
    }
    al_draw_scaled_bitmap(sprites.ship,
        0, 0,
        91, 91,
        ship.x, ship.y,
        SHIP_W, SHIP_H,
        0);

    //al_draw_bitmap(sprites.ship, ship.x, ship.y, 0);
}

bool ship_collide(int cx, int cy)
{
    for (int i = 0; i < ALIENS_N; ++i)
    {
        if (!aliens[i].used) continue;
        if (collide_circle(cx, cy, SHIP_R, aliens[i].cx, aliens[i].cy, ALIEN_R[aliens[i].type]))
            return true;
    }
    return false;
}

/* --- Enemy --- */

ALIEN aliens[ALIENS_N];
// 작성자 : 박남현
/* 타입별 체력 및 속도 고정 */
const int ALIEN_LIFE[] = { 50, 36, 45, 101 };
const float ALIEN_SPEED[] = { 42, 20, 27, 84 };

void aliens_init()
{
    for (int i = 0; i < ALIENS_N; i++)
        aliens[i].used = false;
}

void aliens_update()
{
    int new_quota = //2초마다 랜덤 생성
        (frames % 60)
        ? 0
        : between(2, 4) //종류도 랜덤
        ;

    // 작성자 : 천원석 & 박남현
    /* --- 적 생성 및 초기화 --- */
    for (int i = 0; i < ALIENS_N; i++)
    {
        if (!aliens[i].used)
        {
            // if this alien is unused, should it spawn?
            if (new_quota > 0)
            {
                int new_x = between(10, BUFFER_W - 50);
                int new_y = between(10, BUFFER_H - 50);

                int spawn_dup_check = 0;
                while (!spawn_dup_check)
                {
                    int side = between(0, 4);
                    switch (side)
                    {
                    case 0: //상
                        new_x = between(10, BUFFER_W - 10);
                        new_y = between(-40, -30);
                        break;
                    case 1: //하
                        new_x = between(10, BUFFER_W - 10);
                        new_y = between(BUFFER_H + 30, BUFFER_H + 60);
                        break;
                    case 2: //좌
                        new_x = between(-40, -30);
                        new_y = between(10, BUFFER_H - 10);
                        break;
                    case 3: //우
                        new_x = between(BUFFER_W + 30, BUFFER_W + 60);
                        new_y = between(10, BUFFER_H - 10);
                        break;
                    }
                    spawn_dup_check = 1;
                    for (int j = 0; j < ALIENS_N; j++)
                    {
                        if (aliens[j].used && i != j)
                        {
                            int dx = aliens[j].x - new_x;
                            int dy = aliens[j].y - new_y;
                            if (dx * dx + dy * dy < 1000) {
                                spawn_dup_check = 0;
                                break;
                            }
                        }
                    }
                }

                aliens[i].x = new_x;
                aliens[i].y = new_y;

                aliens[i].type = between(0, ALIEN_TYPE_N);
                //aliens[i].type = 3;
                aliens[i].shot_timer = between(1, 99);
                aliens[i].blink = 0;
                aliens[i].used = true;

                switch (aliens[i].type)
                {
                case ALIEN_TYPE_METEOR:
                    aliens[i].life = 10;
                    aliens[i].speed = 1.5f;
                    break;
                case ALIEN_TYPE_FAST:
                    aliens[i].life = 10;
                    aliens[i].speed = 2.0f;
                    break;
                case ALIEN_TYPE_SHOOTER:
                    aliens[i].life = 20;
                    aliens[i].speed = 0.9f;
                    break;
                case ALIEN_TYPE_BOSS:
                    aliens[i].life = 30;
                    aliens[i].speed = 0.5f;
                    break;
                }

                new_quota--;
            }
            continue;
        }

        aliens[i].cx = aliens[i].x + (ALIEN_W[aliens[i].type] / 2.0f);
        aliens[i].cy = aliens[i].y + (ALIEN_H[aliens[i].type] / 2.0f);

        // 작성자 : 천원석
        // 적-> 캐릭터 방향으로 움직임
        switch (aliens[i].type)
        {
        case ALIEN_TYPE_METEOR:
            aliens_move(i, aliens[i].speed);
            break;

        case ALIEN_TYPE_FAST:
            aliens_move(i, aliens[i].speed);
            break;

        case ALIEN_TYPE_SHOOTER:
            aliens_move(i, aliens[i].speed);
            break;

        case ALIEN_TYPE_BOSS:
            aliens_move(i, aliens[i].speed);
        }

        /* 화면 범위 밖으로 나갈시, 제거*/
        if (aliens[i].x > BUFFER_W + 100 ||
            aliens[i].x < -100 ||
            aliens[i].y > BUFFER_H + 100 ||
            aliens[i].y < -100)
        {
            aliens[i].used = false;
            continue;
        }

        if (aliens[i].blink)
            aliens[i].blink--;

        if (shots_collide(false, aliens[i].x, aliens[i].y, ALIEN_W[aliens[i].type], ALIEN_H[aliens[i].type]))
        {
            aliens[i].life -= ship.damage;
            aliens[i].blink = 4;
        }

        if (aliens[i].life <= 0)
        {
            fx_add(false, aliens[i].cx, aliens[i].cy);

            switch (aliens[i].type)
            {
            case ALIEN_TYPE_METEOR:
                score += 200;
                break;

            case ALIEN_TYPE_FAST:
                score += 150;
                break;

            case ALIEN_TYPE_SHOOTER:
                score += 800;
                fx_add(false, aliens[i].cx - 10, aliens[i].cy - 4);
                fx_add(false, aliens[i].cx + 4, aliens[i].cy + 10);
                fx_add(false, aliens[i].cx + 8, aliens[i].cy + 8);
                break;
            case ALIEN_TYPE_BOSS:
                score += 2000;
                fx_add(false, aliens[i].cx - 10, aliens[i].cy - 4);
                fx_add(false, aliens[i].cx + 4, aliens[i].cy + 10);
                fx_add(false, aliens[i].cx + 8, aliens[i].cy + 8);
                break;
            }

            aliens[i].used = false;
            //0327 박남현 - 아이템 드랍
            item_add(aliens[i].cx, aliens[i].cy, aliens[i].type);
            continue;
        }

        aliens[i].shot_timer--;
        if (aliens[i].shot_timer == 0)
        {
            switch (aliens[i].type)
            {
            case ALIEN_TYPE_METEOR:    //메테오는 총을 쏘지 않는다.
                /*shots_add(false, false, cx, cy);
                aliens[i].shot_timer = 150;*/
                break;
            case ALIEN_TYPE_FAST:
                shots_add(false, true, aliens[i].cx, aliens[i].cy);
                aliens[i].shot_timer = 80;
                break;
            case ALIEN_TYPE_SHOOTER:
                shots_add(false, true, aliens[i].cx - 5, aliens[i].cy);
                shots_add(false, true, aliens[i].cx + 5, aliens[i].cy);
                shots_add(false, true, aliens[i].cx - 5, aliens[i].cy + 8);
                shots_add(false, true, aliens[i].cx + 5, aliens[i].cy + 8);
                aliens[i].shot_timer = 200;
                break;
            case ALIEN_TYPE_BOSS:
                shots_add(false, true, aliens[i].cx, aliens[i].cy);
                aliens[i].shot_timer = 1000;
                aliens[i].shot_count = 5;
                break;
            }
        }
    }
    aliens_collide();
}

// 작성자 : 박남현
// 움직이는것 함수화
void aliens_move(int i, float speed)
{
    if (aliens[i].cx > ship.cx) aliens[i].x -= speed;
    if (aliens[i].cx < ship.cx) aliens[i].x += speed;
    if (aliens[i].cy > ship.cy) aliens[i].y -= speed;
    if (aliens[i].cy < ship.cy) aliens[i].y += speed;
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


void aliens_collide()
{
    for (int i = 0; i < ALIENS_N; i++)
    {
        if (!aliens[i].used) continue;

        for (int j = i + 1; j < ALIENS_N; j++)
        {
            if (!aliens[j].used) continue;

            float r_i = ALIEN_W[aliens[i].type] / 2.0f;
            float r_j = ALIEN_W[aliens[j].type] / 2.0f;

            if (collide_circle(aliens[i].cx, aliens[i].cy, r_i, aliens[j].cx, aliens[j].cy, r_j))
            {
                float dx = aliens[i].cx - aliens[j].cx;
                float dy = aliens[i].cy - aliens[j].cy;
                float dist = sqrtf(dx * dx + dy * dy);

                // 두 적이 완전히 겹쳐서 거리가 0인 경우 예외 처리 (나누기 0 방지)
                if (dist == 0) {
                    aliens[i].x += 1; // 강제로 살짝 떨어뜨림
                    continue;
                }
                float overlap = (r_i + r_j) - dist;

                float nx = dx / dist;
                float ny = dy / dist;

                float move_x = nx * overlap * 0.5f;
                float move_y = ny * overlap * 0.5f;

                aliens[i].x += move_x;
                aliens[i].y += move_y;
                aliens[j].x -= move_x;
                aliens[j].y -= move_y;

                aliens[i].cx = aliens[i].x + (ALIEN_W[aliens[i].type] / 2.0f);
                aliens[i].cy = aliens[i].y + (ALIEN_H[aliens[i].type] / 2.0f);
                aliens[j].cx = aliens[j].x + (ALIEN_W[aliens[j].type] / 2.0f);
                aliens[j].cy = aliens[j].y + (ALIEN_H[aliens[j].type] / 2.0f);
            }
        }
    }
}