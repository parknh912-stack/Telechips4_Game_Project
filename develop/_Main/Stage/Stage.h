#ifndef _STAGE_H_
#define _STAGE_H_

//0329 박남현
/* --- Stage --- */
#define CURR_STAGE (&stage_info[stage_num])

static bool boss_spawned = false;

// 스테이지 정보
// 1. 최대 적 숫자
// 2. 적의 스폰 가중치
// 3. 다음 스테이지를 위한 기준 스코어
// 4. 스폰 간격
// 5. 점수 배수
// 6. 적 체력 배수
typedef struct STAGE {
	int max_enemies;
	int spawn_weight[4];
	long target_score;
	float spawn_interval;
	float score_multiplier;
	float life_multiplier;
}STAGE;

extern STAGE stage_info[];

void stage_init();
void stage_update();

#endif