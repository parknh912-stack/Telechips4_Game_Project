#include "../Core.h"
#include "Background.h"
#include "../Display.h"
#include "../Player_Enemy/Player_Enemy.h"

ALLEGRO_BITMAP* BG_sheet[4] = { NULL };


void backgound_init()
{
	BG_sheet[0] = al_load_bitmap("black.png");
	must_init(BG_sheet[0], "BG_sheet[0]");

	BG_sheet[1] = al_load_bitmap("blue.png");
	must_init(BG_sheet[1], "BG_sheet[1]");

	BG_sheet[2] = al_load_bitmap("darkPurple.png");
	must_init(BG_sheet[2], "BG_sheet[2]");

	BG_sheet[3] = al_load_bitmap("purple.png");
	must_init(BG_sheet[3], "BG_sheet[3]");
}

void background_draw()
{
	float camera_x = ship.cx - 640;
	float camera_y = ship.cy - 360;
	float scroll_x = fmod(camera_x, 256);
	float scroll_y = fmod(camera_y, 256);

	for (int i = -1; i < 7; ++i)
	{
		for (int j = -1; j < 4; ++j)
		{
			float draw_x = camera_x + (i * 256) - scroll_x;
			float draw_y = camera_y + (j * 256) - scroll_y;
			al_draw_bitmap(BG_sheet[2], draw_x, draw_y, 0);
		}
	}
}

void background_deinit()
{
	al_destroy_bitmap(BG_sheet[0]);
	al_destroy_bitmap(BG_sheet[1]);
	al_destroy_bitmap(BG_sheet[2]);
	al_destroy_bitmap(BG_sheet[3]);
}