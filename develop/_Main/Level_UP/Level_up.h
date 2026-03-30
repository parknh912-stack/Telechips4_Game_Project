#ifndef _LEVEL_UP_H_
#define _LEVEL_UP_H_

// 작성자: 신제현
// 레벨 업에 필요한 점수의 양
#define LV_UP           (10000)			////0329 박남현 - 수치 조정 (테스트용)
// 투사체 발사 최대 수(5개가 디폴트)
#define SHOT_COUNT_MAX	(5)	
// 이동 속도 상한선
#define SPEED_MAX		(10)
// 공격 속도 상한선
#define FIRE_RATE_MAX	(20)
// 체력 최대치 상한선
#define MAX_LIFES_MAX	(250)
// 레벨업 요구치 증가 상수
#define LV_UP_REQ		(2)

// 작성자: 신제현
void damage_up(void);
void shot_count_up(void);
void fire_rate_up(void);
void speed_up(void);
void max_lifes_up(void);
void instant_lifes(void);
STATE check_level_up(long score);

extern long score_at_last_level;

#endif // !_ITEM_H_
