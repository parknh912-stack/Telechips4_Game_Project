#ifndef _DISPLAY_H_
#define _DISPLAY_H_

// --- display ---

#define BUFFER_W 1280
#define BUFFER_H 720

#define DISP_SCALE 1
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

extern ALLEGRO_DISPLAY* disp;
extern ALLEGRO_BITMAP* buffer;

void disp_init();
void disp_deinit();
void disp_pre_draw();
void disp_post_draw();

#endif // !_DISPLAY_H_
