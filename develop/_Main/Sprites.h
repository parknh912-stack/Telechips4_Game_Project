#ifndef _SPRITES_H_
#define _SPRITES_H_
#include <stdio.h>
/* --- Sprites --- */

#define SHIP_W 32
#define SHIP_H 32
#define SHIP_R 20

#define SHIP_SHOT_W 9
#define SHIP_SHOT_H 9
#define SHIP_SHOT_R 5

#define LIFE_W 6
#define LIFE_H 6

extern const int ALIEN_W[];
extern const int ALIEN_H[];
extern const int ALIEN_R[];

#define ALIEN_BUG_W      ALIEN_W[0]
#define ALIEN_BUG_H      ALIEN_H[0]
#define ALIEN_ARROW_W    ALIEN_W[1]
#define ALIEN_ARROW_H    ALIEN_H[1]
#define ALIEN_THICCBOI_W ALIEN_W[2]
#define ALIEN_THICCBOI_H ALIEN_H[2]
#define ALIEN_BOSS_W     ALIEN_W[3]
#define ALIEN_BOSS_H     ALIEN_H[3]

#define ALIEN_SHOT_W 9
#define ALIEN_SHOT_H 9
#define ALIEN_SHOT_R 5

#define EXPLOSION_FRAMES 4
#define SPARKS_FRAMES    3

typedef struct SPRITES
{
    ALLEGRO_BITMAP* _sheet;

    ALLEGRO_BITMAP* ship;
    ALLEGRO_BITMAP* ship_shot[2];
    ALLEGRO_BITMAP* life;
    ALLEGRO_BITMAP* life_bar;       //UI

    ALLEGRO_BITMAP* alien[6];
    ALLEGRO_BITMAP* alien_shot;

    ALLEGRO_BITMAP* explosion[EXPLOSION_FRAMES];
    ALLEGRO_BITMAP* sparks[SPARKS_FRAMES];

    ALLEGRO_BITMAP* powerup[4];
} SPRITES;

extern SPRITES sprites;

ALLEGRO_BITMAP* sprite_grab(int x, int y, int w, int h);
void sprites_init();
void sprites_deinit();

#endif