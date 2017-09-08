#include <stdio.h>
#include <allegro.h>
#include "assignment2.h"
#include "sound.h"
#include "sprite.h"
#include "spritehandler.h"

int main(void) {
	int lineLength = 200;
	float angle = 0;
	int cursorDirection = 1;
	int cursorSpeed = 1;
	allegro_init();
	set_color_depth(16);
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
	
	Sound *hardlineSounds = new Sound();
	Sprite *cursor = new Sprite();
	cursor->Load("sprites/target.bmp");
	cursor->setX(WIDTH / 2 - cursor->getWidth());
	cursor->setY(HEIGHT / 2);
	Sprite *cursor2 = new Sprite();
	cursor2->Load("sprites/target.bmp");
	cursor2->setX(WIDTH / 2);
	cursor2->setY(HEIGHT / 2  - cursor->getHeight() / 2);
	BITMAP *buffer;
	buffer = create_bitmap(WIDTH, HEIGHT);
	//draw_sprite(screen, cursor2->getImage(), WIDTH / 2, HEIGHT / 2);
	hline(buffer, WIDTH / 2 - lineLength, HEIGHT / 2, WIDTH / 2 + lineLength, WHITE);
	printf("loaded\n");
	
	while (!key[KEY_ESC]) {
		//printf("x: %f, y: %f\n", cursor->getX(), cursor->getY());
		//rectfill(screen, WIDTH / 2 - lineLength, HEIGHT / 2 - 10, WIDTH / 2 + lineLength, HEIGHT / 2 + 10, BLACK);
		cursor->setX(cursor->getX() + cursorDirection * cursorSpeed);
		if (cursorDirection == 1) {
			rectfill(buffer, cursor->getX(), cursor->getY() + 2, cursor->getX() + cursor->getWidth() / 2 + cursorSpeed, cursor->getY() + cursor->getHeight(), BLACK);	
		}
		if (cursorDirection == -1) {
			rectfill(buffer, cursor->getX() + cursor->getWidth() / 2 - cursorSpeed, cursor->getY()  + 2, cursor->getX() + cursor->getWidth() + cursorSpeed, cursor->getY() + cursor->getHeight(), BLACK);	
		}
		if (cursor->getX() > WIDTH / 2 + lineLength - cursor->getWidth() / 2) {
			cursor->setX(WIDTH / 2 + lineLength - cursor->getWidth() / 2);
			cursorDirection = -1;
		}
		if (cursor->getX() < WIDTH / 2 - lineLength - cursor->getWidth() / 2) {
			cursor->setX(WIDTH / 2 - lineLength - cursor->getWidth() / 2);
			cursorDirection = 1;
		}
		//hline(screen, WIDTH / 2 - lineLength, HEIGHT / 2, WIDTH / 2 + lineLength, WHITE);
		draw_sprite(buffer, cursor->getImage(), cursor->getX(), cursor->getY());
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, WIDTH - 1, HEIGHT - 1);
		release_screen();
//        cursor->Collided()
		// Possibly move this into an interrupt handler
		hardlineSounds->PollTurnOnOrOffMusic();
	}
	
	delete hardlineSounds;
	allegro_exit();
	return 0;
}
END_OF_MAIN();
