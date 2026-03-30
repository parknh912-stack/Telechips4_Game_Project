#ifndef _AUDIO_H_
#define _AUDIO_H_

/* --- Audio --- */

extern ALLEGRO_SAMPLE* sample_shot;
extern ALLEGRO_SAMPLE* sample_explode[2];
extern ALLEGRO_SAMPLE* sample_bgm[3];
extern ALLEGRO_SAMPLE* sample_level_up;
extern ALLEGRO_SAMPLE_ID bgm_id;

void audio_init();
void audio_play_bgm(int stage_num);
void audio_stop_bgm();
void audio_deinit();

#endif // !_DISPLAY_H_