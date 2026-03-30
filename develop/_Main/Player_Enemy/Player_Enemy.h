#ifndef _PLAYER_ENEMY_H_
#define _PLAYER_ENEMY_H_

/* --- shot --- */
// 작성자 : 박남현
typedef struct SHOT
{
    float x, y, dx, dy;
    float speed;        //shot의 이동 속도
    int frame;
    bool ship;
    bool used;
} SHOT;

#define SHOTS_N 512

extern SHOT shots[SHOTS_N];
void shots_init();
bool shots_add(bool is_ship, bool straight, float x, float y, int shot_count);
bool shots_create_instance(bool is_ship, float x, float y, float dx, float dy);
void shots_update();
bool shots_collide(bool ship, float cx, float cy, float w, float h);
void shots_draw();

// 작성자 : 박남현
int get_closet_enemy();     //최단거리 적 idx 반환하는 함수

/* --- Player --- */

//#define SHIP_SPEED 3 삭제
#define MAP_WIDTH 5000
#define MAP_HEIGHT 5000

#define SHIP_MAX_X (BUFFER_W - SHIP_W)
#define SHIP_MAX_Y (BUFFER_H - SHIP_H)

// 작성자 : 박남현
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
    int max_lifes;      //최대 체력
    int curr_lifes;     //현재 체력

    int invincible_timer;   //무적시간 (배리어 용)
    bool barrier;
} SHIP;
extern SHIP ship;

void ship_init();
void ship_update();
void ship_draw();
bool ship_collide(int cx, int cy);




/* --- Enemy --- */
#define ALIENS_N 50 //물리적 한계

typedef enum ALIEN_TYPE
{
    ALIEN_TYPE_METEOR = 0,
    ALIEN_TYPE_FAST,       //1
    ALIEN_TYPE_SHOOTER,    //2
    ALIEN_TYPE_BOSS,        //3
    ALIEN_TYPE_N
} ALIEN_TYPE;


//0329 박남현 - 적 점수
typedef enum ALIEN_SCORE
{
    ALIEN_SCORE_METEOR  = 200,
    ALIEN_SCORE_FAST    = 300,    
    ALIEN_SCORE_SHOOTER = 800,  
    ALIEN_SCORE_BOSS    = 2000,     
    ALIEN_SCORE_N
} ALIEN_SCORE;


//0329 박남현 - 적 체력
typedef enum ALIEN_LIFE
{
    ALIEN_LIFE_METEOR   = 10,
    ALIEN_LIFE_FAST     = 10,
    ALIEN_LIFE_SHOOTER  = 20,
    ALIEN_LIFE_BOSS     = 300,
    ALIEN_LIFE_N
} ALIEN_LIFE;

//0330 박남현 - 적 투사체 수
typedef enum ALIEN_SHOT
{
    ALIEN_SHOT_METEOR   = 0,
    ALIEN_SHOT_FAST     = 0,
    ALIEN_SHOT_SHOOTER  = 1,
    ALIEN_SHOT_BOSS     = 5,
    ALIEN_SHOT_N
} ALIEN_SHOT;

// 작성자 : 박남현
typedef struct ALIEN
{
    /* 좌표 */
    float x, y;
    float cx, cy;       //중앙 좌표

    /* 스텟*/
    float speed;        //이동 속도
    float fire_rate;    //초당 공격 속도
    int shot_timer;     //공격 타이머
    int shot_count;

    ALIEN_TYPE type;
    int blink;
    int life;
    bool used;
} ALIEN;

extern ALIEN aliens[];

void aliens_init();
void aliens_update();   //생성

//0329 박남현 - 스테이지 구현을 위하여 적 생성 관련 함수들 개별 분리
void spawn_enemy(float* new_x, float* new_y, int i);
void spawn_boss();
bool is_boss_alive();
int decide_enemy_type();
void set_aliens_info(int i, float life_mul);

void aliens_move(int i, float speed);
void aliens_draw();
void aliens_single_draw(float between_angle, int i, float cx, float cy);
void aliens_collide();


#endif // 