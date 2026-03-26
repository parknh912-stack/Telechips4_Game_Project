#ifndef _PLAYER_ENEMY_H_
#define _PLAYER_ENEMY_H_

/* --- shot --- */

typedef struct SHOT
{
    float x, y, dx, dy;
    int frame;
    float speed;
    bool ship;
    bool used;
} SHOT;

#define SHOTS_N 128

extern SHOT shots[SHOTS_N];
void shots_init();
bool shots_add(bool ship, bool straight, int x, int y);
void shots_update();
bool shots_collide(bool ship, int x, int y, int w, int h);
void shots_draw();

int get_closet_enemy();



/* --- Player --- */

#define SHIP_SPEED 3
#define SHIP_MAX_X (BUFFER_W - SHIP_W)
#define SHIP_MAX_Y (BUFFER_H - SHIP_H)

typedef struct SHIP
{
    /* 좌표 */
    float x, y;
    float cx, cy;       //중앙 좌표

    /* 스텟*/
    float speed;        //이동 속도
    float fire_rate;    //초당 공격 속도
    int shot_timer;     //공격 타이머

    int damage;         //데미지
    int shot_count;     //투사체 갯수
    int max_lives;      //최대 체력
    int lives;          //현재 체력

    int respawn_timer;  //나중에 제거 예정
    int invincible_timer;   //무적시간 (배리어 용)
} SHIP;
extern SHIP ship;

void ship_init();
void ship_update();
void ship_draw();





/* --- Enemy --- */

typedef enum ALIEN_TYPE
{
    ALIEN_TYPE_BUG = 0,
    ALIEN_TYPE_ARROW,       //1
    ALIEN_TYPE_THICCBOI,    //2
    ALIEN_TYPE_BOSS,        //3
    ALIEN_TYPE_N
} ALIEN_TYPE;

typedef struct ALIEN
{
    /* 좌표 */
    float x, y;
    float cx, cy;       //중앙 좌표

    /* 스텟*/
    float speed;        //이동 속도
    float fire_rate;    //초당 공격 속도
    int shot_timer;     //공격 타이머

    ALIEN_TYPE type;
    int blink;
    int life;
    bool used;
} ALIEN;

#define ALIENS_N 5  //나오는 적 숫자

#define ALIEN_LIFE_BUG  ALIEN_LIFE[0]
#define ALIEN_LIFE_BUG  ALIEN_LIFE[0]
#define ALIEN_LIFE_BUG  ALIEN_LIFE[0]
#define ALIEN_LIFE_BUG  ALIEN_LIFE[0]

extern ALIEN aliens[ALIENS_N];
extern const int ALIEN_LIFE[];
extern const float ALIEN_SPEED[];

void aliens_init();
void aliens_update();   //생성
void aliens_draw();
void aliens_move(int i, float speed);


#endif // 
