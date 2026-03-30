#include "../Core.h"
#include "Stage.h"
#include "../Player_Enemy/Player_Enemy.h"
#include "../Display.h"
#include "../Audio.h"
#include "../Sprites.h"
#include "../Item/Item.h"
#include "../UI/UI.h"

//0329 박남현
/* --- Stage ---*/

STAGE stage_info[3] = {
	// 적 숫자, {스폰 가중치 METEOR, FAST, SHOOTER, BOSS}, 스폰간격, 점수배수, 체력배수
	{
		.max_enemies = 20,
		.spawn_weight = {80, 20, 0 ,0},
		.target_time = 60 * 10,
		.spawn_interval = 3.0f, 
		.score_multiplier = 1.0f, 
		.life_multiplier = 1.0f
	},
		{
		.max_enemies = 30,
		.spawn_weight = {20, 40, 40 ,0},
		.spawn_interval = 2.0f,
		.target_time = 3600,
		.score_multiplier = 1.5f,
		.life_multiplier = 2.0f
	},
		{
		.max_enemies = 40,
		.spawn_weight = {20, 40, 40 ,0},
		.target_time = 3600,
		.spawn_interval = 1.0f,
		.score_multiplier = 2.0f,
		.life_multiplier = 4.0f
	}
};

int stage_frames;

void stage_init()
{
	stage_num = 0;
	boss_spawned = false;
	stage_frames = 0;
	audio_play_bgm(stage_num);
}

void stage_update()
{

	if (!boss_spawned && stage_frames >= (CURR_STAGE->target_time))
	{
		item_use(ITEM_TYPE_BOMB);
		spawn_boss();
		boss_spawned = true;
		boss_alert_timer = al_get_time();
	}

	if (boss_spawned && !is_boss_alive())
	{
		if (stage_num < 2)	//3스테이지 이전
		{
			stage_num++;
			boss_spawned = false;
			stage_frames = 0;

			// 0330 신제현
			stage_alert_timer = al_get_time();

			audio_play_bgm(stage_num);
			for (int i = 0; i < ALIENS_N; ++i)
			{
				if (!aliens[i].used) continue;
				aliens[i].used = false;
			}
			shots_init();
		}
		else
		{
			audio_stop_bgm();
			score += 18000 - frames;
			current_state = STATE_ENDING;
		}
	}
	stage_frames++;
}