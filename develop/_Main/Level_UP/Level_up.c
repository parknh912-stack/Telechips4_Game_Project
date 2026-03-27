#include "../Core.h"
#include "Level_up.h"

// 작성자: 신제현
// 레벨 증가하는 점수에 따라 해당 조건 검사하는 함수
STATE check_level_up(long score)
{
	// 너무나 빠른 레벨업을 막고자
	// 레벨업 요구치를 기하급수적으로 늘리기
	int level_up_score = (level * level * LV_UP) / 2;
	
	if (score >= level_up_score)
	{
		++level;
		return STATE_LEVEL_UP;
	}

	return STATE_PLAYING;
}

// 작성자: 신제현
// 공격력 증가
void damage_up(void)
{
	ship.damage += 2;
}

// 투사체 발사 수 증가
void shot_count_up(void)
{
	ship.shot_count += 1;
}

// 공격 속도 증가
void fire_rate_up(void)
{
	ship.fire_rate += 1;
}

// 이동 속도 증가
void speed_up(void)
{
	ship.speed += 1;
}

// 체력 최대치 증가
void max_lifes_up(void)
{
	ship.max_lifes *= 1.01;
}

// 체력 즉시 회복
void instant_lifes(void)
{
	ship.curr_lifes = ship.max_lifes;
}