#include "Core.h"
#include "Sprites.h"

/* --- Sprites --- */

// 0: 접근만 하는 적 ( 운석 ), 1: 0번과 같지만 더 빠르고 체력이 높다,  2: 0번보다 느리지만 투사체 발사 3: 보스   
// 1번적은 enemyred2로
const int ALIEN_W[] = { 101, 104, 103, 112 };
const int ALIEN_H[] = { 84, 84, 84, 75 };
const int ALIEN_R[] = { 42, 42, 42, 32 };
SPRITES sprites;

ALLEGRO_BITMAP* sprite_grab(int x, int y, int w, int h)
{
    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(sprites._sheet, x, y, w, h);
    must_init(sprite, "sprite grab");
    return sprite;
}

//작성자 : 박남현
// 수정 : 0327 천원석
void sprites_init()
{
    sprites._sheet = al_load_bitmap("sheet.png");
    must_init(sprites._sheet, "spritesheet");

    sprites.ship = sprite_grab(444, 91, 91, 91);    //"ufoBlue.png" x="444" y="91" width="91" height="91"/>

    sprites.ship_shot[0] = sprite_grab(434, 325, 48, 46); //"laserBlue09.png" x="434" y="325" width="48" height="46"
    sprites.ship_shot[1] = sprite_grab(698, 795, 38, 37); //"laserBlue11.png" x="698" y="795" width="38" height="37"

    sprites.life = sprite_grab(777, 443, 32, 26); //"playerLife3_red.png" x = "777" y = "443" width = "32" height = "26" / >
    sprites.life_bar = sprite_grab(0, 0, 222, 39); //UI

    sprites.alien[0] = sprite_grab(224, 748, ALIEN_METEOR_W, ALIEN_METEOR_H); // "meteorGrey_big1.png" x="224" y="748" width="101" height="84"/>
    sprites.alien[1] = sprite_grab(120, 520, ALIEN_FAST_W, ALIEN_FAST_H); // "enemyRed2.png" x="120" y="520" width="104" height="84"/>
    sprites.alien[2] = sprite_grab(224, 496, ALIEN_SHOOTER_W, ALIEN_SHOOTER_H); // "enemyGreen3.png" x="224" y="496" width="103" height="84"/>
    sprites.alien[3] = sprite_grab(0, 941, ALIEN_BOSS_W, ALIEN_BOSS_H); // "playerShip2_red.png" x="0" y="941" width="112" height="75"/>
    /*sprites.alien[4] = sprite_grab(0, 21, ALIEN_THICCBOI_W, ALIEN_THICCBOI_H);
    sprites.alien[5] = sprite_grab(0, 21, ALIEN_THICCBOI_W, ALIEN_THICCBOI_H);*/




    sprites.alien_shot = sprite_grab(310, 982, 41, 41); //"turretBase_big.png" x="310" y="982" width="41" height="41"/>

    sprites.explosion[0] = sprite_grab(33, 10, 9, 9);
    sprites.explosion[1] = sprite_grab(43, 9, 11, 11);
    sprites.explosion[2] = sprite_grab(46, 21, 17, 18);
    sprites.explosion[3] = sprite_grab(46, 40, 17, 17);

    sprites.sparks[0] = sprite_grab(34, 0, 10, 8);
    sprites.sparks[1] = sprite_grab(45, 0, 7, 8);
    sprites.sparks[2] = sprite_grab(54, 0, 9, 8);

    sprites.powerup[0] = sprite_grab(0, 49, 9, 12);
    sprites.powerup[1] = sprite_grab(10, 49, 9, 12);
    sprites.powerup[2] = sprite_grab(20, 49, 9, 12);
    sprites.powerup[3] = sprite_grab(30, 49, 9, 12);
}

void sprites_deinit()
{
    al_destroy_bitmap(sprites.ship);

    al_destroy_bitmap(sprites.ship_shot[0]);
    al_destroy_bitmap(sprites.ship_shot[1]);

    al_destroy_bitmap(sprites.life);
    al_destroy_bitmap(sprites.life_bar);    //UI

    al_destroy_bitmap(sprites.alien[0]);
    al_destroy_bitmap(sprites.alien[1]);
    al_destroy_bitmap(sprites.alien[2]);

    al_destroy_bitmap(sprites.alien_shot);

    al_destroy_bitmap(sprites.explosion[0]);
    al_destroy_bitmap(sprites.explosion[1]);
    al_destroy_bitmap(sprites.explosion[2]);
    al_destroy_bitmap(sprites.explosion[3]);

    al_destroy_bitmap(sprites.sparks[0]);
    al_destroy_bitmap(sprites.sparks[1]);
    al_destroy_bitmap(sprites.sparks[2]);

    al_destroy_bitmap(sprites.powerup[0]);
    al_destroy_bitmap(sprites.powerup[1]);
    al_destroy_bitmap(sprites.powerup[2]);
    al_destroy_bitmap(sprites.powerup[3]);

    al_destroy_bitmap(sprites._sheet);
}