#include <allegro.h>
#include "assignment2.h"

int main(void) {
	allegro_init();
	install_keyboard();
	int ret = set_gfx_mode(MODE_W, WIDTH, HEIGHT, 0, 0);
	if (ret != 0) {
		allegro_message("Error setting up screen!");
		return 1;
	}
	
	while (1) {
		if (key[KEY_ESC]) {
			break;
		}
	}
	
	allegro_exit(); 
}
END_OF_MAIN()
