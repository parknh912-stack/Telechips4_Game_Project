#ifndef _LEVEL_UP_H_
#define _LEVEL_UP_H_

// 작성자: 신제현
// 레벨 업에 필요한 점수의 양
#define LV_UP           (1000)

#include "../Core.h"
#include "../Player_Enemy/Player_Enemy.h"

// 작성자: 신제현
void damage_up(void);
void shot_count_up(void);
void fire_rate_up(void);
void speed_up(void);
void max_lifes_up(void);
void instant_lifes(void);
STATE check_level_up(long score);

#endif // !_ITEM_H_
