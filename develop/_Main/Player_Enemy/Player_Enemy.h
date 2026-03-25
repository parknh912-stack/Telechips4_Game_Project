#ifndef _PLAYER_ENEMY_H_
#define _PLAYER_ENEMY_H_

/* --- shot --- */

typedef struct SHOT
{
    int x, y, dx, dy;
    int frame;
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




/* --- Player --- */

#define SHIP_SPEED 3
#define SHIP_MAX_X (BUFFER_W - SHIP_W)
#define SHIP_MAX_Y (BUFFER_H - SHIP_H)

typedef struct SHIP
{
    int x, y;
    int cx, cy;         //Áß¾Ó ÁÂÇ¥ Ãß°¡
    int shot_timer;
    int lives;
    int respawn_timer;
    int invincible_timer;
} SHIP;
extern SHIP ship;

void ship_init();
void ship_update();
void ship_draw();





/* --- Enemy --- */

typedef enum ALIEN_TYPE
{
    ALIEN_TYPE_BUG = 0,
    ALIEN_TYPE_ARROW,
    ALIEN_TYPE_THICCBOI,
    ALIEN_TYPE_N
} ALIEN_TYPE;

typedef struct ALIEN
{
    int x, y;
    int cx, cy;      //Áß¾Ó ÁÂÇ¥ Ãß°¡
    ALIEN_TYPE type;
    int shot_timer;
    int blink;
    int life;
    bool used;
} ALIEN;

#define ALIENS_N 16
extern ALIEN aliens[ALIENS_N];

void aliens_init();
void aliens_update();   //»ý¼º
void aliens_draw();

#endif // 
