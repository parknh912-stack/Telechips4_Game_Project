#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "Rank.h"

RANK ranking[RANK_QUEUE_SIZE];

void rank_init() {
    rank_count = 0;
    for (int i = 0; i < RANK_QUEUE_SIZE; i++) {
        strcpy(ranking[i].username, "---");
        ranking[i].score = 0;
    }
}

void rank_add(const char* name, long new_score) {
    // 1. 큐가 가득 찼고, 마지막 점수보다 낮으면 무시
    if (rank_count >= RANK_QUEUE_SIZE && new_score <= ranking[RANK_QUEUE_SIZE - 1].score) {
        return;
    }

    int i;
    // 2. 적절한 삽입 위치 찾기 (우선순위 큐의 핵심)
    // 뒤에서부터 확인하며 새 점수보다 낮은 데이터들을 한 칸씩 뒤로 밀어냅니다.
    for (i = rank_count; i > 0; i--) {
        if (new_score > ranking[i - 1].score) {
            // 큐 사이즈를 넘지 않도록 안전하게 한 칸씩 밀기
            if (i < RANK_QUEUE_SIZE) {
                ranking[i] = ranking[i - 1];
            }
        }
        else {
            break;
        }
    }

    // 3. 찾은 위치 i에 데이터 삽입
    if (i < RANK_QUEUE_SIZE) {
        strncpy(ranking[i].username, name, MAX_NAME_LEN - 1);
        ranking[i].username[MAX_NAME_LEN - 1] = '\0';
        ranking[i].score = new_score;

        // 4. 데이터 개수 갱신
        if (rank_count < RANK_QUEUE_SIZE) {
            rank_count++;
        }
    }
}

void rank_save() {
    FILE* f = fopen("ranking.dat", "wb");
    if (f) {
        fwrite(&rank_count, sizeof(int), 1, f); // 현재 개수 먼저 저장
        fwrite(ranking, sizeof(RANK), RANK_QUEUE_SIZE, f);
        fclose(f);
    }
}

void rank_load() {
    FILE* f = fopen("ranking.dat", "rb");
    if (f) {
        fread(&rank_count, sizeof(int), 1, f);
        fread(ranking, sizeof(RANK), RANK_QUEUE_SIZE, f);
        fclose(f);
    }
    else {
        rank_init(); // 파일이 없으면 초기화
    }
}