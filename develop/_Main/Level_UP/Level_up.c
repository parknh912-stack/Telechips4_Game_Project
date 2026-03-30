#include "../Core.h"
#include "Level_up.h"

#include "../Player_Enemy/Player_Enemy.h"
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
// 공격력 증가(무한 증가)
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

// 공격 속도 증가(최대 공격 속도: 49)
void fire_rate_up(void)
{
	if (ship.fire_rate < FIRE_RATE_MAX)
		ship.fire_rate += 0.2f;
}

// 이동 속도 증가(최대 이동 속도: 99)
void speed_up(void)
{
	if (ship.speed < SPEED_MAX)
		ship.speed += 0.1f;
}

// 체력 최대치 증가(체력 최대치의 5%씩 증가함)
void max_lifes_up(void)
{
	if (ship.max_lifes < MAX_LIFES_MAX)
		ship.max_lifes *= 1.05;
}

// 체력 즉시 회복(현재 체력의 50% 채워 줌)
void instant_lifes(void)
{
	ship.curr_lifes = fminf(ship.max_lifes, ship.curr_lifes * 1.5f);
}