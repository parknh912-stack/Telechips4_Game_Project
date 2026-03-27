#include "../Core.h"
#include "Level_up.h"

// 레벨 업 전 캐릭터의 가장 최근 스코어
long score_at_last_level = 0;	

// 작성자: 신제현
// 레벨 증가하는 점수에 따라 해당 조건 검사하는 함수
STATE check_level_up(long score)
{
	long gap = score - score_at_last_level;
	
	// 레벨마다 레벨업 요구치가 1.7배 증가함
	int required_score = (int)(LV_UP + pow(LV_UP_REQ, level - 1));

	if (gap >= required_score)
	{
		score_at_last_level = score;
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
	// 재작성자: 신제현
	// 투사체는 5개 이상 넘어가지 않음
	if (ship.shot_count < SHOT_COUNT_MAX)
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
	ship.max_lifes *= 1.1;
}

// 체력 즉시 회복
void instant_lifes(void)
{
	ship.curr_lifes = ship.max_lifes;
}