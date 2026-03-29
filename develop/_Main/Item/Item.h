#ifndef _ITEM_H_
#define _ITEM_H_

#define ITEMS_N 20

typedef enum ITEM_TYPE
{
    ITEM_TYPE_NONE = 0,
    ITEM_TYPE_HEAL_POTION,  //1
    ITEM_TYPE_BOMB,         //2
    ITEM_TYPE_EXP,          //3
    ITEM_TYPE_BARRIER,      //4
    ITEM_TYPE_RANDOM_STAT_CHANGE,    //5
    ITEM_TYPE_N
} ITEM_TYPE;

typedef struct ITEM
{
    float x, y, cx, cy;
    float vx, vy;
    ITEM_TYPE type;
    int timer;
    int blink;
    bool used;
}ITEM;

extern ITEM items[];

void item_init();
void item_add(float cx, float cy, int type);
bool item_create_instance(int type, float cx, float cy);
bool item_collide(float cx, float cy);
void item_update();
void item_draw();

void item_use(ITEM_TYPE type);
#endif // !_ITEM_H_
