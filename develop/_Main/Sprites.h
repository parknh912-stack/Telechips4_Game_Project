#ifndef _SPRITES_H_
#define _SPRITES_H_
#include <stdio.h>
#include "Item/Item.h"
/* --- Sprites --- */

#define SHIP_W 32
#define SHIP_H 32
#define SHIP_R 15

#define SHIP_SHOT_W 9
#define SHIP_SHOT_H 9
#define SHIP_SHOT_R 5

#define LIFE_W 9//0327 UI 체력바 변경
#define LIFE_H 9//0327 UI 체력바 변경

extern const int ALIEN_W[];
extern const int ALIEN_H[];
extern const int ALIEN_R[];

#define ALIEN_METEOR_W   ALIEN_W[0]
#define ALIEN_METEOR_H   ALIEN_H[0]
#define ALIEN_FAST_W    ALIEN_W[1]
#define ALIEN_FAST_H    ALIEN_H[1]
#define ALIEN_SHOOTER_W ALIEN_W[2]
#define ALIEN_SHOOTER_H ALIEN_H[2]
#define ALIEN_BOSS_W     ALIEN_W[3]
#define ALIEN_BOSS_H     ALIEN_H[3]

#define ALIEN_SHOT_W 9
#define ALIEN_SHOT_H 9
#define ALIEN_SHOT_R 5

#define EXPLOSION_FRAMES 4
#define SPARKS_FRAMES    3

//0327 박남현
#define ITEMS_W 20
#define ITEMS_H 20
#define ITEMS_R 10
#define ITEMS_TYPE_N 6

#define ITEMS_BARRIER_W 50
#define ITEMS_BARRIER_H 50

typedef struct SPRITES
{
    ALLEGRO_BITMAP* _sheet;

    ALLEGRO_BITMAP* ship;
    ALLEGRO_BITMAP* ship_shot[2];
    ALLEGRO_BITMAP* life;
    ALLEGRO_BITMAP* life_bar;

    ALLEGRO_BITMAP* alien[4];
    ALLEGRO_BITMAP* alien_shot;

    ALLEGRO_BITMAP* explosion[EXPLOSION_FRAMES];
    ALLEGRO_BITMAP* sparks[SPARKS_FRAMES];

    ALLEGRO_BITMAP* item[ITEMS_TYPE_N];
    ALLEGRO_BITMAP* barrier;
} SPRITES;

extern SPRITES sprites;

ALLEGRO_BITMAP* sprite_grab(int x, int y, int w, int h);
void sprites_init();
void sprites_deinit();

#endif