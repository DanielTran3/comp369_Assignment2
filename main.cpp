#include <stdio.h>
#include "assignment2.h"

volatile int ticks;
volatile int framerate;
bool gameOver;
int lineLength = 200;
BITMAP *mascotFrames[MASCOTFRAMES];
Sound *hardlineSounds;
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

/*
	Draws a fullscreen image from an input file onto a bitmap.
*/
void drawImage(const char * filename, BITMAP *bitmap) {
	BITMAP *image;
	image = load_bitmap(filename, NULL);
	if (!image) {
		allegro_message("Error Loading %s", filename);
	}
	
	blit(image, bitmap, 0, 0, 0, 0, WIDTH, HEIGHT);
	destroy_bitmap(image);
}

/*
	Animate and displays the title screen
*/
void displayTitleScreen(FONT *titleFont, FONT *titleFont_sm) {
	// Load image here instead of using the displayImage function because we don't want
	// to load and destroy the image bitmap ever loop, as that takes too long
	BITMAP *image;
	image = load_bitmap(TITLE_BACKGROUND, NULL);
	if (!image) {
		allegro_message("Error Loading %s", TITLE_BACKGROUND);
	}
	
	BITMAP *buffer;
	buffer = create_bitmap(WIDTH, HEIGHT);
	// Keeps track of the left-most character until the animation passes that characters'
	// position defined in title_pos
	int pointer = 0;
	char title[9] = "HARDLINE"; // Char array containing individual letters of the title
	int title_pos[8] = {200, 230, 260, 290, 320, 350, 380, 410};
	
	// Loops to animate the title transition from left to right.
	for (int i = 0; i < HEIGHT; i++) {
		blit(image, buffer, 0, 0, 0, 0, WIDTH, HEIGHT);
		// Moves all of the characters who's positions are greater than the current character's final position
		if (i < title_pos[pointer]) {
			for (int k = pointer; k < strlen(title); k++) {
				textprintf_ex(buffer, titleFont, i, 110, BLACK, -1, "%c", title[k]);				
			}	
		}
		else { // If the current character's final position is passed, move onto the next character
			if (pointer < strlen(title)) {
				pointer++;					
			}
		}
		// Display all of the characters where i has passed their final position (not animated anymore)
		for (int j = 0; j < pointer; j++) {
			textprintf_ex(buffer, titleFont, title_pos[j], 110, BLACK, -1, "%c", title[j]);				
		}
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, WIDTH - 1, HEIGHT - 1);
		release_screen();
	}
	// Draws the animations for the line under the title. Multiple lines are drawn to create the effect of a thicker line
	for (int i = 210; i < 450; i++) {
		hline(buffer, 200, 185, i, BLACK);
		hline(buffer, 200, 186, i, BLACK);
		hline(buffer, 200, 187, i, BLACK);
		hline(buffer, 200, 188, i, BLACK);
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, WIDTH - 1, HEIGHT - 1);
		release_screen();
	}
	
	textprintf_centre_ex(screen, titleFont_sm, WIDTH / 2, HEIGHT / 2 + 9 * LINE_SPACING, BLACK, -1, "Press ENTER to Start");
	
	// Allows for toggling of sound and music on this screen
	while (!key[KEY_ENTER]) {
		hardlineSounds->PollTurnOnOrOffMusic();
	}
	destroy_bitmap(buffer);
	// Rest and clear any characters in the keyboard buffer so key presses don't transfer to the next screen
	rest(100);
	clear_keybuf();
}

/*
	Displays the instructions screen
*/
void displayInstructions() {
	int xOffset = WIDTH / 2;
	int yOffset = HEIGHT / 4;
	
	drawImage(INSTRUCTIONS_BACKGROUND);
	draw_pretty_box("If you miss or the cursor passes the target, you lose the game!", 70, yOffset, 10, 30, 21);	
	textout_centre_ex(screen, font, "Instructions", xOffset, yOffset, WHITE, 0);
	textout_centre_ex(screen, font, "Select the options on the following screens", xOffset, yOffset + 2 * LINE_SPACING, WHITE, 0);
	textout_centre_ex(screen, font, "by pressing the specified keys on the screen.", xOffset, yOffset + 3 * LINE_SPACING, WHITE, 0);
	
	textout_centre_ex(screen, font, "A cursor moves on the screen and your goal is", xOffset, yOffset + 5 * LINE_SPACING, WHITE, 0);
	textout_centre_ex(screen, font, "to press SPACE when the cursor touches the target!", xOffset, yOffset + 6 * LINE_SPACING, WHITE, 0);
	
	textout_centre_ex(screen, font, "Everytime you hit the target, your score increases", xOffset, yOffset + 8 * LINE_SPACING, WHITE, 0);
	textout_centre_ex(screen, font, "and the cursor will move in the opposite direction.", xOffset, yOffset + 9 * LINE_SPACING, WHITE, 0);
	textout_centre_ex(screen, font, "Every 5 target hits will increase the speed of the cursor!", xOffset, yOffset + 10 * LINE_SPACING, WHITE, 0);
	textout_centre_ex(screen, font, "If you miss or the cursor passes the target, you lose the game!", xOffset, yOffset + 11 * LINE_SPACING, WHITE, 0);
		
	textout_centre_ex(screen, font, "---Controls---", xOffset, yOffset + 14 * LINE_SPACING, WHITE, 0);
	textout_ex(screen, font, "Hit the Target: SPACE", xOffset - 75, yOffset + 15 * LINE_SPACING, WHITE, 0);
	textout_ex(screen, font, "Toggle Music  : CTRL + M", xOffset - 75, yOffset + 16 * LINE_SPACING, WHITE, 0);
	textout_ex(screen, font, "Help Menu     : CTRL + H", xOffset - 75, yOffset + 17 * LINE_SPACING, WHITE, 0);
	textout_centre_ex(screen, font, "Press ENTER to Continue", xOffset, yOffset + 20 * LINE_SPACING, WHITE, 0);
	
	// Allows for toggling of sound and music on this screen
	while (!key[KEY_ENTER]) {
		hardlineSounds->PollTurnOnOrOffMusic();
	}
	// Rest and clear any characters in the keyboard buffer so key presses don't transfer to the next screen
	rest(100);
	clear_keybuf();
}

/*
	Displays the difficulty selection screen
*/
void displayDifficultySelectionScreen(FONT *headerFont, FONT *selectionFont, Sprite *cursor) {
	drawImage(LEVELS_BACKGROUND);
	int xOffset = WIDTH / 4;
    int yOffset = HEIGHT / 6 + 50;
	textprintf_ex(screen, headerFont, xOffset / 2, yOffset, BLACK, -1, "Select A Difficulty");
	textprintf_ex(screen, selectionFont, xOffset, yOffset + 6 * LINE_SPACING, BLACK, -1, "1 - Easy");
	textprintf_ex(screen, selectionFont, xOffset, yOffset + 10 * LINE_SPACING, BLACK, -1, "2 - Medium");
	textprintf_ex(screen, selectionFont, xOffset, yOffset + 14 * LINE_SPACING, BLACK, -1, "3 - Hard");
	// Changes the difficulty depending on the user's input
	while (1) {
		hardlineSounds->PollTurnOnOrOffMusic();
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

/*
	Displays the help screen
*/
void displayHelpScreen(FONT *helpTitle, FONT *helpFont) {
	int xOffset = WIDTH / 2;
	
	drawImage(HELP_SCREEN);
	textprintf_centre_ex(screen, helpTitle, xOffset, 10, WHITE, -1, "Help");
	textprintf_centre_ex(screen, helpFont, xOffset, HEIGHT / 2 + 12 * LINE_SPACING, WHITE, -1, "Press SPACE at the correct time to increase your score!");
	textprintf_centre_ex(screen, helpFont, xOffset, HEIGHT / 2 + 16 * LINE_SPACING, WHITE, -1, "Press ENTER to exit the help screen");
	textprintf_centre_ex(screen, helpFont, xOffset, HEIGHT / 2 + 18 * LINE_SPACING, WHITE, -1, "OR SPACE to display the instructions");

	// Exit the help screen and continue the game, or display the instructions screen.
	while(1) {
		// Allows for toggling of sound and music on this screen
		hardlineSounds->PollTurnOnOrOffMusic();
		if (key[KEY_ENTER]) {
			break;
		}
		if (key[KEY_SPACE]) {
			displayInstructions();
		}
	}
}

/*
	Displays the game over screen
*/
void displayGameOverScreen(PlayerInfo *player, FONT *gameOverFont) {
	drawImage(GAMEOVER_BACKGROUND);
    int xOffset = WIDTH / 2;
    int yOffset = HEIGHT / 4 + 50;

    draw_pretty_box("Press Enter To Retry or ESC to Exit", xOffset / 2 + 20, yOffset, 30, 30, 12);
    textprintf_centre_ex(screen, gameOverFont, xOffset, yOffset - 20, WHITE, -1, "Game Over");
    textprintf_centre_ex(screen, font, xOffset, yOffset + 5 * LINE_SPACING, WHITE, -1, "Your Score:    %i", player->getScore());
    textprintf_centre_ex(screen, font, xOffset, yOffset + 6.5 * LINE_SPACING, WHITE, -1, "Highest Score: %i", player->getHighestScore());
    textprintf_centre_ex(screen, font, xOffset, yOffset + 11 * LINE_SPACING, WHITE, -1, "Press Enter To Retry or ESC to Exit");
}

/*
	Displays the user's current stats, including the current level, player's score, and the player's highest score
*/
void displayUserInformation(PlayerInfo *player, BITMAP *buffer) {
	int xOffset = WIDTH / 2;
	int yOffset = 5;
	rectfill(buffer, 0, 0, WIDTH, yOffset + 10, BLACK);
	textprintf_centre_ex(buffer, font, xOffset, yOffset, WHITE, 0, "Level: %i | Score: %i | High Score: %i", 
						 player->getLevel(), player->getScore(), player->getHighestScore());
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

/*
	Function that returns true if the player wants to retry the game, or false if they want to quit the game.
*/
bool chooseToContinue() {
	while(1) {
		// Allows for toggling of sound and music on this screen
		hardlineSounds->PollTurnOnOrOffMusic();
		if (key[KEY_ESC]) {
			return false;
		}
		if (key[KEY_ENTER]) {
			return true;
		}
	}
}

/*
	Settings for easy difficulty. A long line and slow moving cursor
*/
void easy(Sprite *cursor) {
	lineLength = 500;
	cursor->setXDelay(4);
	cursor->setYDelay(0);
	cursor->setVelX(1.5);
	cursor->setVelY(0);
}

/*
	Settings for medium difficulty. A medium length line and medium speed moving cursor
*/
void medium(Sprite *cursor) {
	lineLength = 350;
	cursor->setXDelay(4);
	cursor->setYDelay(0);
	cursor->setVelX(2.0);
	cursor->setVelY(0);
}

/*
	Settings for hard difficulty. A short line and fast speed moving cursor
*/
void hard(Sprite *cursor) {
	lineLength = 250;
	cursor->setXDelay(4);
	cursor->setYDelay(0);
	cursor->setVelX(3.0);
	cursor->setVelY(0);
}

/*
	Function that checks if the cursor hit the boundary of the line. If it did, then
	reverse the cursor's direction
*/
void checkCursorOnBoundary(Sprite *cursor) {
	// Cursor hit the right side of the line
	if (cursor->getX() > (WIDTH + lineLength - cursor->getWidth()) / 2) {
			cursor->setX((WIDTH + lineLength - cursor->getWidth()) / 2);
			cursor->setDirection(-1);
		}
	// Cursor hit the left side of the line
	if (cursor->getX() < (WIDTH - lineLength - cursor->getWidth()) / 2) {
		cursor->setX((WIDTH - lineLength - cursor->getWidth()) / 2);
		cursor->setDirection(1);
	}
}

/*
	Determines if the cursor has hit the target
*/
bool hitTheTarget(Sprite *cursor, Sprite *target) {
	// Check if the center of the cursor is anywhere in the space of the target.
	// If so, the target was hit and true is returned
	if (target->PointInside(cursor->CenterX(), cursor->CenterY())) {
		return true;
	}
	// Cursor missed the target, so set gameOver flag to true and return false
	else {
		gameOver = true;
		return false;
	}
}

/*
	Moves the target to a random position after the cursor hit the target.
*/
void relocateTarget(Sprite *cursor, Sprite *target) {
	// Subtract half the target width and a small factor so the target doesn't appear on the end of the line,
	// since the detection for losing the game is based on the right/left edge of the target
	int randomLocation = (WIDTH - lineLength + target->getWidth()) / 2 + rand() % (lineLength - 2 * target->getWidth());
	target->setX(randomLocation);
	
	// Ensure the new target position is not overlapping with the cursor, otherwise generate a new position
	while (cursor->Collided(target, 0)) {
		randomLocation = (WIDTH - lineLength + target->getWidth()) / 2 + rand() % (lineLength  - 2 * target->getWidth());
		target->setX(randomLocation);
	}
	
	// Reverse the direction of the cursor after a target is hit
	cursor->ChangeDirection();

	// Change the side of the target that the cursor must pass depending on the target's
	// position in respect to the cursor
	setTargetSides(cursor, target);
}

/*
	Sets the side of the target that will cause the game to end if the cursor passes it.
*/
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

/*
	Function to check if the cursor has passed the target's side that was set by the setTargetSides() function
*/
void checkIfCursorPassesTarget(Sprite *cursor, Sprite * target) {
	// Check for cursor passing the left side
	if (target->getPassLeftSideToLose()) {
		if (cursor->CenterX() < target->getLeftOrRightSide()) {
			gameOver = true;
		}	
	}
	// Check for cursor passing the right side
	else {
		if (cursor->CenterX() > target->getLeftOrRightSide()) {
			gameOver = true;
		}
	}
	
}

/*
	Frame grabbing function from Chapter 9 of the Textbook
*/
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

/*
	Initializing the mascot (Sonic)'s position, velocity, and animation frames
*/
Sprite *createMascotAnimSprite() {
	// Create and load the sprite 
	Sprite *tempSprite = new Sprite();
	int ret = tempSprite->Load(SONIC);
    if (!ret) {
    	allegro_message("Error loading mascot sprite sheet.");
    	return NULL;
	}
	
	// Get the animation fromes for Sonic
	for (int n = 0; n < 4; n++) {
    	mascotFrames[n] = grabframe(tempSprite->getImage(), 37, 46, 0, 0, 4, n);
	}
	
	// Set sonic at the right side of the screen and his velocity negative so he moves to the left.
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

/*
	Animation for the game's mascot (Sonic) running past the screen on a level up
*/
void animateMascot(BITMAP * buffer, Sprite *mascot, PlayerInfo *player, FONT *levelFont) {
	// check if the user has leveled up (the level up flag is on
	if (player->HasLeveled()) {
		// If the mascot hasn't reached the right side of the screen yet, update his position and animation frame
		if (mascot->getX() > 0) {
			mascot->UpdatePosition();
        	mascot->UpdateAnimation();
			mascot->DrawFrame(buffer);
			textprintf_centre_ex(buffer, levelFont, WIDTH / 2, 100, BLACK, -1, "LEVEL %i", player->getLevel());
		}
		else {
			// Reset the mascot to the right side of the screen and set the leveled up variable false
			mascot->setX(647);
			player->ResetLeveled();	
		}
	}
}

/*
	Reset some of the game parameters
*/
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

void helpMenu(FONT *helpTitle, FONT *helpFont) {
	if ((key[KEY_LCONTROL] && key[KEY_H]) ||
		(key[KEY_RCONTROL] && key[KEY_H])) { 
		hardlineSounds->setSoundEffect(PAUSE_SFX);
		hardlineSounds->playSoundEffect();
		displayHelpScreen(helpTitle, helpFont);
	}
}

int main(void) {
	allegro_init();
	set_color_depth(24);
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
	hardlineSounds = new Sound();
	FONT *letter_gothic_48 = load_font("fonts/Letter_Gothic_Std_48.pcx", NULL, NULL);
	FONT *letter_gothic_28 = load_font("fonts/Letter_Gothic_Std_28.pcx", NULL, NULL);
	FONT *letter_gothic_24 = load_font("fonts/Letter_Gothic_Std_24.pcx", NULL, NULL);
	FONT *letter_gothic_12 = load_font("fonts/Letter_Gothic_Std_12.pcx", NULL, NULL);
	FONT *lucida_calligraphy_36 = load_font("fonts/lucida_calligraphy_36.pcx", NULL, NULL);
	if (!letter_gothic_48 || !letter_gothic_24 || !letter_gothic_12 || !lucida_calligraphy_36) {
		allegro_message("Cannot find one or more font files");
		return 1;
	}
	// Incorrect
	ticks++;
	rest(5);
	
	displayTitleScreen(letter_gothic_48, letter_gothic_24);
	displayInstructions();
	PlayerInfo *player = new PlayerInfo();
	Sprite *cursor = new Sprite();
	Sprite *target = new Sprite();
	cursor->Load(CURSOR);
	target->Load(TARGET);
	displayDifficultySelectionScreen(letter_gothic_28, letter_gothic_24, cursor);
	cursor->setX((WIDTH - lineLength) / 2);
	cursor->setY(HEIGHT / 2);            
	target->setX((WIDTH + lineLength) / 2 - 2 * target->getWidth());
	target->setY(HEIGHT / 2  - target->getHeight() / 2);
	setTargetSides(cursor, target);
	BITMAP *buffer;
	buffer = create_bitmap(WIDTH, HEIGHT);
	
	Sprite *mascot = createMascotAnimSprite();
	
	BITMAP *background_image;
	background_image = load_bitmap(BACKGROUND, NULL);
	if (!background_image) {
		allegro_message("Error Loading %s", BACKGROUND);
	}

	while (!key[KEY_ESC]) {
		helpMenu(lucida_calligraphy_36, letter_gothic_12);
		cursor->UpdatePosition();
		checkCursorOnBoundary(cursor);
		checkIfCursorPassesTarget(cursor, target);
		if (keypressed()) {
			if(key[KEY_SPACE]) {
				// Stop the if hitTheTarget function from triggering multiple times
				// when the space key is pressed.
				clear_keybuf();
				if (hitTheTarget(cursor, target)) {
					player->IncreaseScore(cursor, hardlineSounds);
					relocateTarget(cursor, target);
				}
			}
		}
		blit(background_image, buffer, 0, 0, 0, 0, WIDTH, HEIGHT);
		animateMascot(buffer, mascot, player, letter_gothic_24);
		displayUserInformation(player, buffer);
		hline(buffer, (WIDTH - lineLength) / 2, HEIGHT / 2 - 1, (WIDTH + lineLength) / 2, BLACK);
		hline(buffer, (WIDTH - lineLength) / 2, HEIGHT / 2, (WIDTH + lineLength) / 2, BLACK);
		hline(buffer, (WIDTH - lineLength) / 2, HEIGHT / 2 + 1, (WIDTH + lineLength) / 2, BLACK);
		
		draw_sprite(buffer, target->getImage(), target->getX(), target->getY());
		
		draw_sprite(buffer, cursor->getImage(), cursor->getX(), cursor->getY());
		//textprintf_ex(buffer, font, 0, 0, WHITE, -1, "Frames: %i", framerate);
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, WIDTH - 1, HEIGHT - 1);
		release_screen();
		
		// ADD HELP MENU FUNCTION HERE
		hardlineSounds->PollTurnOnOrOffMusic();
		if (gameOver) {
			hardlineSounds->setSoundEffect(GAMEOVER_SFX);
			hardlineSounds->playSoundEffect();
			displayGameOverScreen(player, lucida_calligraphy_36);
			if (chooseToContinue()) {
				// Restore game to default (bring user to starting screen)
				gameOver = false;
				rest(100);
				displayInstructions();
				displayDifficultySelectionScreen(letter_gothic_28, letter_gothic_24, cursor);
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
        
    destroy_bitmap(background_image);
	delete hardlineSounds;
	allegro_exit();
	return 0;
}
END_OF_MAIN();
