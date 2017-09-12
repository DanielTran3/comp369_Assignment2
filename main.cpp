#include <stdio.h>
#include "assignment2.h"

volatile int ticks;
volatile int framerate;
bool gameOver;
int lineLength = 200;
BITMAP *mascotFrames[MASCOTFRAMES];
//calculate framerate every second
void timer1(void)
{
    framerate = ticks;
    ticks=0;
}
END_OF_FUNCTION(timer1);

/*
	Draws a fullscreen image given the filename input. Generally used to display a background image
*/
void drawImage(const char * filename) {
	BITMAP *image;
	image = load_bitmap(filename, NULL);
	if (!image) {
		allegro_message("Error Loading %s", filename);
	}
	
	blit(image, screen, 0, 0, 0, 0, WIDTH, HEIGHT);
	destroy_bitmap(image);
}

// Title Screen
void displayTitleScreen(FONT *titleFont, FONT *titleFont_sm) {
	// Replace the title with a blit to the screen with a better title
	//drawImage(TITLE_IMAGE);
	
	BITMAP *buffer;
	buffer = create_bitmap(WIDTH, HEIGHT);
	int counter = 200;
	int pointer = 0;
	char title[9] = "HARDLINE";
	int title_pos[8] = {200, 230, 260, 290, 320, 350, 380, 410};
	for (int i = 0; i < HEIGHT; i++) {
		rectfill(buffer, 0, 0, WIDTH, HEIGHT, BLACK);
		if (i < title_pos[pointer]) {
			for (int k = pointer; k < strlen(title); k++) {
				textprintf_ex(buffer, titleFont, i, 0, WHITE, -1, "%c", title[k]);				
			}	
		}
		else {
			if (pointer < strlen(title)) {
				counter += 30;
				pointer++;					
			}
		}
		for (int j = 0; j < pointer; j++) {
			textprintf_ex(buffer, titleFont, title_pos[j], 0, WHITE, -1, "%c", title[j]);				
		}
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, WIDTH - 1, HEIGHT - 1);
		release_screen();
	}
	for (int i = 210; i < 450; i++) {
		hline(buffer, 200, 75, i, WHITE);
		hline(buffer, 200, 76, i, WHITE);
		hline(buffer, 200, 77, i, WHITE);
		hline(buffer, 200, 78, i, WHITE);
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, WIDTH - 1, HEIGHT - 1);
		release_screen();
	}
	textprintf_centre_ex(screen, titleFont_sm, WIDTH / 2, HEIGHT / 2 + 5 * LINE_SPACING, WHITE, -1, "Press ENTER to Start");
	while (!key[KEY_ENTER]);
	destroy_bitmap(buffer);
	rest(100);
	clear_keybuf();
}
// Instructions/Controls Screen
/*
	Draws the instructions screen
*/
void displayInstructions() {
	int xOffset = WIDTH / 2;
	int yOffset = HEIGHT / 4;
	
	//drawImage(INSTRUCTIONS_IMAGE);
	draw_pretty_box("If you miss or the cursor passes the target, you lose the game!", 70, yOffset, 10, 30, 21);	
	textout_centre_ex(screen, font, "Instructions", xOffset, yOffset, WHITE, 0);
	textout_centre_ex(screen, font, "Select the options on the following screens", xOffset, yOffset + 2 * LINE_SPACING, WHITE, 0);
	textout_centre_ex(screen, font, "by pressing the colored keys on the screen.", xOffset, yOffset + 3 * LINE_SPACING, WHITE, 0);
	
	textout_centre_ex(screen, font, "A cursor moves on the screen and your goal is", xOffset, yOffset + 5 * LINE_SPACING, WHITE, 0);
	textout_centre_ex(screen, font, "to press SPACE when the cursor touches the target!", xOffset, yOffset + 6 * LINE_SPACING, WHITE, 0);
	
	textout_centre_ex(screen, font, "There are 5 levels in total. Everytime you hit the", xOffset, yOffset + 8 * LINE_SPACING, WHITE, 0);
	textout_centre_ex(screen, font, "the target, your score increases and the cursor", xOffset, yOffset + 9 * LINE_SPACING, WHITE, 0);
	textout_centre_ex(screen, font, "will move in the opposite direction.", xOffset, yOffset + 10 * LINE_SPACING, WHITE, 0);
	textout_centre_ex(screen, font, "Every 5 times the target is hit, the cursor will speed up!", xOffset, yOffset + 11 * LINE_SPACING, WHITE, 0);
	textout_centre_ex(screen, font, "If you miss or the cursor passes the target, you lose the game!", xOffset, yOffset + 12 * LINE_SPACING, WHITE, 0);
		
	textout_centre_ex(screen, font, "That's all there is to this game! Enjoy!", xOffset, yOffset + 14 * LINE_SPACING, WHITE, 0);
	
	textout_centre_ex(screen, font, "Press ENTER to Continue", xOffset, yOffset + 20 * LINE_SPACING, WHITE, 0);
	
	while (!key[KEY_ENTER]);
	rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
	rest(100);
	clear_keybuf();
}

// Level Selection Screen
void displayLevelSelectionScreen(FONT *headerFont, FONT *selectionFont, Sprite *cursor) {
	int xOffset = WIDTH / 4;
    int yOffset = HEIGHT / 4 + 50;
	textprintf_ex(screen, headerFont, xOffset / 2, yOffset, WHITE, -1, "Select A Difficulty");
	textprintf_ex(screen, selectionFont, xOffset, yOffset + 6 * LINE_SPACING, WHITE, -1, "1 - Easy");
	textprintf_ex(screen, selectionFont, xOffset, yOffset + 10 * LINE_SPACING, WHITE, -1, "2 - Medium");
	textprintf_ex(screen, selectionFont, xOffset, yOffset + 14 * LINE_SPACING, WHITE, -1, "3 - Hard");
	while (1) {
		if (key[KEY_1]) {
			easy(cursor);
			break;
		}
		if (key[KEY_2]) {
			medium(cursor);
			break;
		}
		if (key[KEY_3]) {
			hard(cursor);
			break;
		}
	}
}

void displayGameOverScreen(PlayerInfo *player, FONT *gameOverFont) {
	FONT *lucida_calligraphy_36 = load_font("fonts/lucida_calligraphy_36.pcx", NULL, NULL);
	if (!lucida_calligraphy_36) {
		allegro_message("Cannot find Lucida Calligraphy Font\n");
	}
	else {
	    int xOffset = WIDTH / 2;
	    int yOffset = HEIGHT / 4 + 50;
	    rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
	    draw_pretty_box("Press Enter To Retry or ESC to Exit", xOffset / 2 + 20, yOffset, 30, 30, 12);
	    textprintf_centre_ex(screen, lucida_calligraphy_36, xOffset, yOffset - 20, WHITE, -1, "Game Over");
	    textprintf_centre_ex(screen, font, xOffset, yOffset + 5 * LINE_SPACING, WHITE, -1, "Your Score: %i", player->getScore());
	    textprintf_centre_ex(screen, font, xOffset, yOffset + 6.5 * LINE_SPACING, WHITE, -1, "Highest Score: %i", player->getHighestScore());
	    textprintf_centre_ex(screen, font, xOffset, yOffset + 11 * LINE_SPACING, WHITE, -1, "Press Enter To Retry or ESC to Exit");
	}
}

/*
	Function to display the title of the game, the type of quiz the user selected, and the user's score.
*/
void displayUserInformation(PlayerInfo *player, BITMAP *buffer) {
	int xOffset = WIDTH / 2;
	int yOffset = 5;
	rectfill(buffer, 0, 0, WIDTH, yOffset + 10, BLACK);
	textprintf_centre_ex(buffer, font, xOffset, yOffset, WHITE, 0, "Level: %i | Score: %i", player->getLevel(), player->getScore());
	hline(buffer, 0, yOffset + 10, WIDTH, WHITE);
}

/*
	Get the number of characters in the inputted text
*/
int getHorizontalSpacing(const char *text) {
	return text_length(font, text) / CHARACTER_WIDTH;
}

/*
	Draw a box based on the text length and number of lines
*/
void draw_pretty_box(const char * textToMeasure, int x, int y, int offset_x, int offset_y, int numLines) {
	int textLength = getHorizontalSpacing(textToMeasure) * CHARACTER_WIDTH;
	printf("textLength: %i\n", textLength);
	rectfill(screen, x - offset_x, y - offset_y, x + textLength + offset_x, y + (numLines * LINE_SPACING) + offset_y, BLACK);
	rect(screen, x - offset_x, y - offset_y, x + textLength + offset_x, y + (numLines * LINE_SPACING) + offset_y, WHITE);
}

bool chooseToContinue(Sound *music) {
	while(1) {
		music->PollTurnOnOrOffMusic();
		if (key[KEY_ESC]) {
			return false;
		}
		if (key[KEY_ENTER]) {
			return true;
		}
	}
}

void easy(Sprite *cursor) {
	lineLength = 500;
	cursor->setXDelay(10);
	cursor->setYDelay(0);
	cursor->setVelX(3.0);
	cursor->setVelY(0);
}

void medium(Sprite *cursor) {
	lineLength = 350;
	cursor->setXDelay(8);
	cursor->setYDelay(0);
	cursor->setVelX(5.0);
	cursor->setVelY(0);
}

void hard(Sprite *cursor) {
	lineLength = 250;
	cursor->setXDelay(8);
	cursor->setYDelay(0);
	cursor->setVelX(8.0);
	cursor->setVelY(0);
}

void checkCursorOnBoundary(Sprite *cursor) {
	if (cursor->getX() > (WIDTH + lineLength - cursor->getWidth()) / 2) {
			cursor->setX((WIDTH + lineLength - cursor->getWidth()) / 2);
			cursor->setDirection(-1);
		}
	if (cursor->getX() < (WIDTH - lineLength - cursor->getWidth()) / 2) {
		cursor->setX((WIDTH - lineLength - cursor->getWidth()) / 2);
		cursor->setDirection(1);
	}
}

bool hitTheTarget(Sprite *cursor, Sprite *target) {
	//rest(10);
	int edgeOffsetFactor = 5;
	if (target->PointInside(cursor->CenterX(), cursor->CenterY())) {
		printf("COLLIDED");
		return true;
	}
	else {
		printf("GAMEOVER");
		gameOver = true;
		return false;
	}
}

void relocateTarget(Sprite *cursor, Sprite *target, PlayerInfo *player) {
	// Subtract half the target width and a small factor so the target doesn't appear on the end of the line,
	// Since the detection for losing the game is based on the right/left edge of the target
	int randomLocation = (WIDTH - lineLength + target->getWidth()) / 2 + rand() % (lineLength - 2 * target->getWidth());
	target->setX(randomLocation);
	// Ensure the new target position is not overlapping with the cursor
	// Add some buffer space amount to this --------------
	while (cursor->Collided(target, 0)) {
		randomLocation = (WIDTH - lineLength + target->getWidth()) / 2 + rand() % (lineLength  - 2 * target->getWidth());
		target->setX(randomLocation);
	}
	target->setX(randomLocation);
	
	cursor->ChangeDirection();
	player->IncreaseScore(cursor);

	setTargetSides(cursor, target);
}

void setTargetSides(Sprite *cursor, Sprite *target) {
	// Depending on the randomLocation placement of the target in respect to the current location of the cursor,
	// this can be used to determine which side of the target the cursor has to pass in order to determine that
	// the player has lost.
	// If the target spawns to the right of the cursor, check for the cursor passing the right side of the target
	if (cursor->CenterX() < target->CenterX()) {
			target->setLeftOrRightSide(target->getX() + target->getWidth());
			target->setPassLeftSideToLose(false);
	}
	// If the target spawns to the left of the cursor, check for the cursor passing the left side of the target
	else {
		target->setLeftOrRightSide(target->getX());
		target->setPassLeftSideToLose(true);
	}
}

void checkIfCursorPassesTarget(Sprite *cursor, Sprite * target) {
	if (target->getPassLeftSideToLose()) {
		if (cursor->CenterX() < target->getLeftOrRightSide()) {
			gameOver = true;
		}	
	}
	else {
		if (cursor->CenterX() > target->getLeftOrRightSide()) {
			gameOver = true;
		}
	}
	
}

//reuse our friendly tile grabber from chapter 9
BITMAP *grabframe(BITMAP *source, 
                  int width, int height, 
                  int startx, int starty, 
                  int columns, int frame)
{
    BITMAP *temp = create_bitmap(width,height);

    int x = startx + (frame % columns) * width;
    int y = starty + (frame / columns) * height;

    blit(source,temp,x,y,0,0,width,height);

    return temp;
}

Sprite *createMascotAnimSprite() {
	Sprite *tempSprite = new Sprite();
	int ret = tempSprite->Load("sprites/Sonic.bmp");
    if (!ret) {
    	allegro_message("Error loading mascot sprite sheet.");
    	return NULL;
	}
	
	for (int n = 0; n < 4; n++) {
    	mascotFrames[n] = grabframe(tempSprite->getImage(), 37, 46, 0, 0, 4, n);
	}

    tempSprite->setX(647);
    tempSprite->setY(100);
    tempSprite->setWidth(mascotFrames[0]->w);
    tempSprite->setHeight(mascotFrames[0]->h);
    tempSprite->setXDelay(1);
    tempSprite->setYDelay(0);
    tempSprite->setXCount(0);
    tempSprite->setYCount(0);
    tempSprite->setVelX(-5);
    tempSprite->setVelY(0);
    tempSprite->setCurFrame(0);
    tempSprite->setTotalFrames(4);
    tempSprite->setAnimColumns(4);
    tempSprite->setFrameCount(0);
    tempSprite->setFrameDelay(6);
    tempSprite->setAnimDir(1);
    return tempSprite;
}

void animateMascot(BITMAP * buffer, Sprite *mascot, PlayerInfo *player, FONT *levelFont) {
	rectfill(buffer, 0, 0, WIDTH - 1, HEIGHT - 1, BLACK);
	if (player->HasLeveled()) {
		if (mascot->getX() > 0) {
			mascot->UpdatePosition();
        	mascot->UpdateAnimation();
			mascot->DrawFrame(buffer);
			textprintf_centre_ex(buffer, levelFont, WIDTH / 2, 100, WHITE, -1, "LEVEL %i", player->getLevel());
		}
		else {
			mascot->setX(647);
			player->ResetLeveled();	
		}
	}
}

void restartGame(PlayerInfo *player, Sprite *cursor, Sprite *target) {
	cursor->setX((WIDTH - lineLength) / 2);
	cursor->setY(HEIGHT / 2);            
	target->setX((WIDTH + lineLength) / 2 - 2 * target->getWidth());
	target->setY(HEIGHT / 2  - target->getHeight() / 2);
	setTargetSides(cursor, target);
	player->setLevel(1);
	player->setNumHits(0);
	player->setScore(0);
	player->ResetLeveled();
}

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
	
	FONT *letter_gothic_48 = load_font("fonts/Letter_Gothic_Std_48.pcx", NULL, NULL);
	FONT *letter_gothic_28 = load_font("fonts/Letter_Gothic_Std_28.pcx", NULL, NULL);
	FONT *letter_gothic_24 = load_font("fonts/Letter_Gothic_Std_24.pcx", NULL, NULL);
	FONT *letter_gothic_12 = load_font("fonts/Letter_Gothic_Std_12.pcx", NULL, NULL);
	if (!letter_gothic_48 || !letter_gothic_24 || !letter_gothic_12) {
		allegro_message("Cannot find fonts/title_font.pcx file\n");
		return 1;
	}
	// Incorrect
	ticks++;
	rest(5);
	
	displayTitleScreen(letter_gothic_48, letter_gothic_24);
	displayInstructions();
	PlayerInfo *player = new PlayerInfo();
	SpriteHandler *spriteContainer = new SpriteHandler();
	Sound *hardlineSounds = new Sound();
	Sprite *cursor = new Sprite();
	Sprite *target = new Sprite();
	cursor->Load("sprites/cursor.bmp");
	target->Load("sprites/target.bmp");
	displayLevelSelectionScreen(letter_gothic_28, letter_gothic_24, cursor);
	cursor->setX((WIDTH - lineLength) / 2);
	cursor->setY(HEIGHT / 2);            
	target->setX((WIDTH + lineLength) / 2 - 2 * target->getWidth());
	target->setY(HEIGHT / 2  - target->getHeight() / 2);
	setTargetSides(cursor, target);
	BITMAP *buffer;
	buffer = create_bitmap(WIDTH, HEIGHT);
	
	spriteContainer->Add(cursor);
	spriteContainer->Add(target);
	spriteContainer->Add(createMascotAnimSprite());

	while (!key[KEY_ESC]) {
		cursor->UpdatePosition();
		checkCursorOnBoundary(cursor);
		checkIfCursorPassesTarget(cursor, target);
		if (keypressed()) {
			if(key[KEY_SPACE]) {
				// Stop the if hitTheTarget function from triggering multiple times
				// when the space key is pressed.
				clear_keybuf();
				if (hitTheTarget(cursor, target)) {
					relocateTarget(cursor, target, player);
				}
			}
		}
		animateMascot(buffer, spriteContainer->Get(2), player, letter_gothic_24);
		displayUserInformation(player, buffer);
		hline(buffer, (WIDTH - lineLength) / 2, HEIGHT / 2, (WIDTH + lineLength) / 2, WHITE);
		draw_sprite(buffer, target->getImage(), target->getX(), target->getY());
		
		draw_sprite(buffer, cursor->getImage(), cursor->getX(), cursor->getY());
		//textprintf_ex(buffer, font, 0, 0, WHITE, -1, "Frames: %i", framerate);
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, WIDTH - 1, HEIGHT - 1);
		release_screen();

		hardlineSounds->PollTurnOnOrOffMusic();
		if (gameOver) {
			displayGameOverScreen(player, letter_gothic_24);
			if (chooseToContinue(hardlineSounds)) {
				// Restore game to default (bring user to starting screen)
				gameOver = false;
				rest(100);
				displayInstructions();
				displayLevelSelectionScreen(letter_gothic_28, letter_gothic_24, cursor);
				restartGame(player, cursor, target);
				continue;
			}
			else {
				break;
			}
		}
	}
	
	for (int f = 0; f < MASCOTFRAMES; f++) {
		destroy_bitmap(mascotFrames[f]);
	}
        
	delete hardlineSounds;
	allegro_exit();
	return 0;
}
END_OF_MAIN();
