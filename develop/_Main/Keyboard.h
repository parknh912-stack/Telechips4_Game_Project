#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

/* --- KeyBoard --- */

#define KEY_SEEN     1
#define KEY_DOWN     2

extern unsigned char key[];
void keyboard_init();
void keyboard_update(ALLEGRO_EVENT* event);

#endif // !_KEYBOARD_H_
