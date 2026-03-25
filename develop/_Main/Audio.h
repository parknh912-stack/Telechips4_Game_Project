#ifndef _AUDIO_H_
#define _AUDIO_H_

/* --- Audio --- */

extern ALLEGRO_SAMPLE* sample_shot;
extern ALLEGRO_SAMPLE* sample_explode[2];

void audio_init();
void audio_deinit();

#endif // !_DISPLAY_H_