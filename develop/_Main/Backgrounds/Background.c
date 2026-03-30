#include "../Core.h"
#include "Background.h"
#include "../Display.h"
#include "../Player_Enemy/Player_Enemy.h"

ALLEGRO_BITMAP* BG_sheet[4] = { NULL };
ALLEGRO_BITMAP* BG_PLANET_sheet[10] = { NULL };

void backgound_init()
{
	BG_sheet[0] = al_load_bitmap("blue.png");
	must_init(BG_sheet[0], "BG_sheet[0]");

	BG_sheet[1] = al_load_bitmap("purple.png");
	must_init(BG_sheet[1], "BG_sheet[1]");

	BG_sheet[2] = al_load_bitmap("darkPurple.png");
	must_init(BG_sheet[2], "BG_sheet[2]");

	BG_sheet[3] = al_load_bitmap("black.png");
	must_init(BG_sheet[3], "BG_sheet[3]");

	/* --- 快林 操固扁 侩 青己 持扁*/
	BG_PLANET_sheet[0] = al_load_bitmap("planet00.png");
	must_init(BG_PLANET_sheet[0], "BG_PLANET_sheet[0]");

	BG_PLANET_sheet[1] = al_load_bitmap("planet01.png");
	must_init(BG_PLANET_sheet[1], "BG_PLANET_sheet[1]");

	BG_PLANET_sheet[2] = al_load_bitmap("planet02.png");
	must_init(BG_PLANET_sheet[2], "BG_PLANET_sheet[2]");

	BG_PLANET_sheet[3] = al_load_bitmap("planet03.png");
	must_init(BG_PLANET_sheet[3], "BG_PLANET_sheet[3]");

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
			al_draw_bitmap(BG_sheet[stage_num], draw_x, draw_y, 0);
		}
	}

	al_draw_scaled_bitmap(BG_PLANET_sheet[0], 0, 0, 1280, 1280,
		420, 860, 320, 320, 0);
	al_draw_scaled_bitmap(BG_PLANET_sheet[1], 0, 0, 1280, 1280,
		580, 3120, 320, 320, 0);
	al_draw_scaled_bitmap(BG_PLANET_sheet[2], 0, 0, 1280, 1280,
		1900, 1900, 320, 320, 0);
	al_draw_scaled_bitmap(BG_PLANET_sheet[3], 0, 0, 1280, 1280,
		4000, 3890, 320, 320, 0);

}

void background_deinit()
{
	al_destroy_bitmap(BG_sheet[0]);
	al_destroy_bitmap(BG_sheet[1]);
	al_destroy_bitmap(BG_sheet[2]);
	al_destroy_bitmap(BG_sheet[3]);

	al_destroy_bitmap(BG_PLANET_sheet[0]);
	al_destroy_bitmap(BG_PLANET_sheet[1]);
	al_destroy_bitmap(BG_PLANET_sheet[2]);
	al_destroy_bitmap(BG_PLANET_sheet[3]);
}