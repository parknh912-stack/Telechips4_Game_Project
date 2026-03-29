#include "../Core.h"
#include "Background.h"
#include "../Display.h"

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
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			al_draw_bitmap(BG_sheet[2], 256*j, 256 * i, 0);
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