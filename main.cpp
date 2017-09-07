#include <allegro.h>
#include "assignment2.h"
#include "sound.h"

int main(void) {
	Sound *popTheLockSounds = new Sound();
	
	allegro_init();
	install_keyboard();
	
	if (install_sound(DIGI_AUTODETECT, MIDI_NONE, "") != 0) {
		allegro_message("Error Initializing Sound System");
		return 1;
	}
	
	int ret = set_gfx_mode(MODE_W, WIDTH, HEIGHT, 0, 0);
	if (ret != 0) {
		allegro_message("Error setting up screen!");
		return 1;
	}
	
	rect(screen, 10, 10, 20, 20, WHITE);
	while (1) {
		if (key[KEY_ESC]) {
			break;
		}
		popTheLockSounds->PollTurnOnOrOffMusic();
	}
	
	allegro_exit(); 
}
END_OF_MAIN();
