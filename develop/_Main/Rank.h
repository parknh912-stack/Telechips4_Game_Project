#ifndef _RANK_H_
#define _RANK_H_

#define MAX_RANKING     (10)
#define RANK_QUEUE_SIZE 20
#define MAX_NAME_LEN 15

typedef struct _RANK {
    char username[MAX_NAME_LEN];
    long score;
}RANK;


extern RANK ranking[RANK_QUEUE_SIZE];//하루종일 켜놓는 오락실 오락기도 아니고 게임 한번에 20판 돌릴 일이 없음.
extern int rank_count; // 현재 큐에 저장된 데이터 개수
extern char player_name[MAX_NAME_LEN]; // 현재 입력 중인 이름

//랭킹 관련 함수입니다.
void rank_init();
void rank_add(const char* name, long new_score);
void rank_save();
void rank_load();

#endif
