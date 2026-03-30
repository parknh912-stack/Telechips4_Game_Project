#include "Core.h"
#include "Audio.h"

/* --- Audio --- */

ALLEGRO_SAMPLE* sample_shot;
ALLEGRO_SAMPLE* sample_explode[2];
ALLEGRO_SAMPLE* sample_bgm[3];
ALLEGRO_SAMPLE* sample_level_up;
ALLEGRO_SAMPLE_ID bgm_id;

void audio_init()
{
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(128);

    sample_shot = al_load_sample("laserSmall_000.ogg");
    must_init(sample_shot, "shot sample");

    sample_explode[0] = al_load_sample("explode1.flac");
    must_init(sample_explode[0], "explode[0] sample");
    sample_explode[1] = al_load_sample("explode2.flac");
    must_init(sample_explode[1], "explode[1] sample");

    sample_bgm[0] = al_load_sample("Full_Throttle_Sector.mp3");
    must_init(sample_bgm[0], "bgm[0] sample");
    sample_bgm[1] = al_load_sample("Escape_Velocity.mp3");
    must_init(sample_bgm[1], "bgm[1] sample");
    sample_bgm[2] = al_load_sample("Beyond_The_Final_Screen.mp3");
    must_init(sample_bgm[2], "bgm[2] sample");

    sample_level_up = al_load_sample("maplestory-lvl-up.mp3");
    must_init(sample_level_up, "sample_level_up sample");

}
void audio_play_bgm(int stage_num)
{
    // 이미 재생 중인 BGM이 있다면 정지
    al_stop_sample(&bgm_id);

    // 해당 스테이지 음악 반복 재생 (ALLEGRO_PLAYMODE_LOOP)
    al_play_sample(sample_bgm[stage_num], 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &bgm_id);
}

void audio_stop_bgm()
{
    al_stop_sample(&bgm_id);
}

void audio_deinit()
{
    al_destroy_sample(sample_shot);
    al_destroy_sample(sample_explode[0]);
    al_destroy_sample(sample_explode[1]);
    for (int i = 0; i < 3; i++) al_destroy_sample(sample_bgm[i]);
    al_destroy_sample(sample_level_up);
}