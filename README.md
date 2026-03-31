# 🌌 Space Survivor (Telechips4 Game Project)

![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
![Visual Studio](https://img.shields.io/badge/Visual%20Studio-5C2D91.svg?style=for-the-badge&logo=visual-studio&logoColor=white)
![Allegro 5](https://img.shields.io/badge/Allegro%205-FFB000.svg?style=for-the-badge)

## 프로젝트 소개
**Space Survivor**는 C언어와 Allegro 5 라이브러리를 기반으로 개발된 탄막 슈팅(Bullet Hell) 및 뱀파이어서바이벌라이크 게임입니다. 
플레이어는 우주 공간에서 사방에서 몰려오는 적들을 처치하고 끝까지 살아남아야 합니다!

## API 문서
프로젝트의 구조, 클래스 및 함수 명세는 아래의 API 문서에서 확인할 수 있습니다.

**[Space Survivor API Document 보러가기](https://htmlpreview.github.io/?https://github.com/parknh912-stack/Telechips4_Game_Project/blob/0331_REAL_FINAL/API%20Document.html)**

## 주요 구현 기능
* **충돌 처리 기능 :** 플레이어와 적, 발사체 간의 매끄러운 충돌 판정 (Collision System)
* **스테이지 추가 :** 시간에 따라 난이도가 증가하며 다양한 적들이 생성되는 스테이지 구조
* **자동 조준 :** 아군, 적이 자동으로 조준하여 발사
* **Warp around 구현 :** 무한한 우주의 느낌을 주기 위하여, 맵의 외곽 제한을 해제
  
## 개발 환경 및 협업 툴
* **Language:** C (Low-level memory & pointer implementation)
* **Library:** Allegro 5.2.11.3
* **IDE:** Microsoft Visual Studio (MSVC)
* **Version Control:** Git / GitHub (LFS 적용)
* **Communication & Docs:** Mattermost, Notion

## 팀 구성 (4인 프로젝트)
* **Project Manager : 박남현 (Player & Enemy & Item & 코드 병합)**
* **Team Member 김병헌** (UI, RANK, CORE 개발)
* **Team Member 신제현** (UI, CORE, Staga)
* **Team Member 천원석** (Player & Enemy, Asset, Boss, Spawn, QA)

## 빌드 및 실행 방법
1. 본 저장소를 클론합니다.
   ```bash
   git clone https://github.com/parknh912-stack/Telechips4_Game_Project.git
