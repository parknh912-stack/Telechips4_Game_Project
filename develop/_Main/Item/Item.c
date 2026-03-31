#include "../Core.h"
#include "Item.h"

#include "../Display.h"
#include "../Keyboard.h"
#include "../Sprites.h"
#include "../Audio.h"
#include "../Fx.h"
#include "../Player_Enemy/Player_Enemy.h"
// 작성자 : 박남현
/* --- ITEM --- */

ITEM items[ITEMS_N];

float drop_table[ALIEN_TYPE_N][ITEM_TYPE_N] = {
	// NONE, HEAL, BOMB, EXP, BARRIER, RANDOM_STAT
	[ALIEN_TYPE_METEOR] = { 88.0f, 4.0f, 4.0f, 4.0f, 4.0f, 0.0f },
	[ALIEN_TYPE_FAST] = { 75.0f, 5.0f, 5.0f, 10.0f, 5.0f, 0.0f },
	[ALIEN_TYPE_SHOOTER] = { 40.0f, 20.0f, 10.0f, 20.0f, 10.0f, 0.0f },
	[ALIEN_TYPE_BOSS] = { 0.0f,  20.0f, 20.0f, 20.0f, 20.0f, 20.0f },
};

void item_init()
{
	for (int i = 0; i < ITEMS_N; ++i)
		items[i].used = false;
}

//타입과 위치를 받아서, 드롭할 아이템을 정함
void item_add(float cx, float cy, int type)
{
	float* curr_drop_table = drop_table[type];
	float curr_weight = 0.0f;
	float rand = between_f(0.0f, 100.0f);

	for (int i = 0; i < ITEM_TYPE_N; ++i)
	{
		curr_weight += curr_drop_table[i];
		if (rand <= curr_weight)
		{
			if (i != ITEM_TYPE_NONE) {	//ITEM_TYPE_NONE 이 아니라면
				item_create_instance(i, cx, cy);
			}
			return;
		}
	}
}

//아이템이 정해진다면, 해당 위치에 아이템을 만들고, 성공, 실패 여부를 반환
bool item_create_instance(int type, float cx, float cy)
{
	for (int i = 0; i < ITEMS_N; ++i)
	{
		if (items[i].used) continue;
		items[i].type = type;
		items[i].timer = 900;	// 15초
		items[i].blink = 0;
		items[i].used = true;

		items[i].x = cx - (ITEMS_W / 2);
		items[i].y = cy - (ITEMS_H / 2);
		items[i].cx = cx;
		items[i].cy = cy;

		return true;
	}
	return false;
}

//캐릭터와 아이템이 충돌할시, 아이템을 사라지게하고, 사용 효과를 부여
bool item_collide(float cx, float cy)
{
	for (int i = 0; i < ITEMS_N; ++i)
	{
		if (!items[i].used) continue;
		if (collide_circle(cx, cy, SHIP_R, items[i].cx, items[i].cy, ITEMS_R))
		{
			item_use(items[i].type);
			items[i].used = false;
			return true;
		}
	}
	return false;
}

//아이템 상태 업데이트
void item_update()
{
	for (int i = 0; i < ITEMS_N; ++i)
	{
		if (items[i].used)
		{
			items[i].timer--;
			if (items[i].blink)
				items[i].blink--;
			else
				items[i].blink = 60;
		}
	}
}

//아이템의 sprite를 그리는 함수
//0330
void item_draw()
{
	for (int i = 0; i < ITEMS_N; ++i)
	{
		if (!items[i].used)
			continue;
		if (!(items[i].blink % 60))
			continue;

		item_single_draw(i, items[i].x, items[i].y);

		if (items[i].cx > MAP_WIDTH - 640)
			item_single_draw(i, items[i].x - MAP_WIDTH, items[i].y);
		if (items[i].cx < 640)
			item_single_draw(i, items[i].x + MAP_WIDTH, items[i].y);
		if (items[i].cy > MAP_HEIGHT - 640)
			item_single_draw(i, items[i].x, items[i].y - MAP_HEIGHT);
		if (items[i].cy < 640)
			item_single_draw(i, items[i].x, items[i].y + MAP_HEIGHT);
	}
}

//0330
void item_single_draw(int i, float x, float y)
{
	al_draw_scaled_bitmap(
		sprites.item[items[i].type],
		0, 0,
		al_get_bitmap_width(sprites.item[items[i].type]),
		al_get_bitmap_height(sprites.item[items[i].type]),
		x, y,
		ITEMS_W, ITEMS_H,
		0);
}


void item_use(ITEM_TYPE type)
{
	switch (type)
	{
	case ITEM_TYPE_HEAL_POTION:
		ship.curr_lifes = fminf(ship.max_lifes, ship.curr_lifes + 20);
		break;
	case ITEM_TYPE_BOMB:
		for (int i = 0; i < 50; ++i)
		{
			if (!aliens[i].used) continue;
			if (aliens[i].type == ALIEN_TYPE_BOSS) continue;

			fx_add(false, aliens[i].cx, aliens[i].cy);
			fx_add(false, aliens[i].cx - 10, aliens[i].cy - 4);
			fx_add(false, aliens[i].cx + 4, aliens[i].cy + 10);
			fx_add(false, aliens[i].cx + 8, aliens[i].cy + 8);
			aliens[i].used = false;
		}
		for (int i = 0; i < SHOTS_N; i++) {
			if (shots[i].ship) continue;	//0329 박남현 - 아군 탄은 남아있게 변경
			shots[i].used = false;
		}
		break;
	case ITEM_TYPE_EXP:
		score += 3000;
		break;
	case ITEM_TYPE_BARRIER:
		ship.barrier = true;
		break;
	case ITEM_TYPE_RANDOM_STAT_CHANGE:
		ship.damage = between(1, 100);
		ship.shot_count = between(1, 10) / 2 + 1;
		ship.fire_rate = between_f(0.1f, 10.0f);
		ship.speed = between_f(0.2f, 10.0f);
		ship.curr_lifes = between(1, ship.max_lifes);
		break;
	}
}