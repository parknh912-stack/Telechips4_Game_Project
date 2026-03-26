#ifndef _RANK_H_
#define _RANK_H_

typedef struct _RANK {
    char username[15];
    long score;
}RANK;

extern RANK ranking[10];

//랭킹 관련 함수입니다.
void rank_init();
void rank_add();
void rank_save();
void rank_load();

#endif
