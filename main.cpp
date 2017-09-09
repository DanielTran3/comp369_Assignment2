#include <stdio.h>
#include <allegro.h>
#include "assignment2.h"
#include "sound.h"
#include "sprite.h"
#include "spritehandler.h"

volatile int ticks;
volatile int framerate;
bool gameOver;

//calculate framerate every second
void timer1(void)
{
    framerate = ticks;
    ticks=0;
}
END_OF_FUNCTION(timer1)

void displayGameOverScreen() {
	rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
	textprintf_centre_ex(screen, font, WIDTH / 2, HEIGHT / 2, WHITE, -1, "Game Over");
	textprintf_centre_ex(screen, font, WIDTH / 2, HEIGHT / 2 + 30, WHITE, -1, "Press Space To Retry or ESC to Exit");
}

bool chooseToContinue() {
	while(1) {
		if (key[KEY_ESC]) {
			return false;
		}
		if (key[KEY_SPACE]) {
			return true;
		}
	}
}

void level1(Sprite *cursor) {
	cursor->setXDelay(10);
	cursor->setYDelay(0);
	cursor->setVelX(5.0);
	cursor->setVelY(0);
}

void checkCursorOnBoundary(Sprite *cursor) {
	if (cursor->getX() > WIDTH / 2 + LINELENGTH - cursor->getWidth() / 2) {
			cursor->setX(WIDTH / 2 + LINELENGTH - cursor->getWidth() / 2);
			cursor->setDirection(-1);
		}
	if (cursor->getX() < WIDTH / 2 - LINELENGTH - cursor->getWidth() / 2) {
		cursor->setX(WIDTH / 2 - LINELENGTH - cursor->getWidth() / 2);
		cursor->setDirection(1);
	}
}

void hitTheTarget(Sprite *cursor, Sprite *target) {
	if (target->PointInside(cursor->CenterX(), cursor->CenterY())) {
		printf("COLLIDED");
		int randomLocation = (WIDTH - LINELENGTH) / 2 + rand() % (LINELENGTH + 1);
		target->setX(randomLocation);
		cursor->ChangeDirection();
	}
	else {
		printf("GAMEOVER");
		gameOver = true;
	}
}

// Depending on the randomLocation placement of the target in respect to the current location of the cursor,
// this can be used to determine which side of the target the cursor has to pass in order to determine that
// the player has lost.
// If the target spawns to the right of the cursor, check for the cursor passing the right side of the target
// If the target spawns to the left of the cursor, check for the cursor passing the left side of the target

int main(void) {
	allegro_init();
	set_color_depth(16);
	install_keyboard();
	install_int(timer1, 1000);
	srand(time(NULL));
	
	if (install_sound(DIGI_AUTODETECT, MIDI_NONE, "") != 0) {
		allegro_message("Error Initializing Sound System");
		return 1;
	}
	
	int ret = set_gfx_mode(MODE_W, WIDTH, HEIGHT, 0, 0);
	if (ret != 0) {
		allegro_message("Error setting up screen!");
		return 1;
	}
	
	// Incorrect
	ticks++;
	rest(15);
	
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
	level1(cursor);

	while (!key[KEY_ESC]) {
		cursor->UpdatePosition();
		checkCursorOnBoundary(cursor);
		if (key[KEY_SPACE]) {
			hitTheTarget(cursor, cursor2);
		}
		//printf("x: %f, y: %f\n", cursor->getX(), cursor->getY());
		rectfill(buffer, 0, 0, WIDTH, HEIGHT, BLACK);
		hline(buffer, WIDTH / 2 - LINELENGTH, HEIGHT / 2, WIDTH / 2 + LINELENGTH, WHITE);
		draw_sprite(buffer, cursor2->getImage(), cursor2->getX(), cursor2->getY());
		
		//hline(screen, WIDTH / 2 - lineLength, HEIGHT / 2, WIDTH / 2 + lineLength, WHITE);
		draw_sprite(buffer, cursor->getImage(), cursor->getX(), cursor->getY());
		
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, WIDTH - 1, HEIGHT - 1);
		release_screen();
//        cursor->Collided()
		hardlineSounds->PollTurnOnOrOffMusic();
		if (gameOver) {
			displayGameOverScreen();
			if (chooseToContinue()) {
				// Restore game to default (bring user to starting screen)
				gameOver = false;
				continue;
			}
			else {
				break;
			}
		}
	}
	
	delete hardlineSounds;
	allegro_exit();
	return 0;
}
END_OF_MAIN();
