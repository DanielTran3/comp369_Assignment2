#ifndef _ASSIGNMENT2_H
#define _ASSIGNMENT2_H 1

#include <allegro.h>
#include "assignment2.h"
#include "sprite.h"
#include "spritehandler.h"
#include "playerinfo.h"

#define MODE_W GFX_AUTODETECT_WINDOWED
#define MODE_F GFX_AUTODETECT_FULLSCREEN

#define WIDTH 640
#define HEIGHT 480

#define CHARACTER_WIDTH 8
#define LINE_SPACING 10
#define MASCOTFRAMES 4

#define HELP_SCREEN "images/help.bmp"
#define TITLE_BACKGROUND "images/title.pcx"
#define INSTRUCTIONS_BACKGROUND "images/instructions.pcx"
#define LEVELS_BACKGROUND "images/levels.pcx"
#define BACKGROUND "images/background_1.pcx"
#define GAMEOVER_BACKGROUND "images/ending.pcx"

#define SONIC "sprites/Sonic.bmp"
#define TARGET "sprites/target.bmp"
#define CURSOR "sprites/cursor.bmp"

#define WHITE makecol(255, 255, 255)
#define BLACK makecol(0, 0, 0)
#define LIGHT_GREEN makecol(100, 255, 0)
#define RED makecol(255, 0, 0)


void timer1(void);
void drawImage(const char * filename);
void displayInstructions();
void displayTitleScreen();
void displayGameOverScreen(PlayerInfo *player);
void displayUserInformation(PlayerInfo *player);
int getHorizontalSpacing(const char *text);
void draw_pretty_box(const char * textToMeasure, int x, int y, int offset_x, int offset_y, int numLines);
bool chooseToContinue(Sound *music);
void easy(Sprite *cursor);
void medium(Sprite *cursor);
void hard(Sprite *cursor);
void checkCursorOnBoundary(Sprite *cursor);
bool hitTheTarget(Sprite *cursor, Sprite *target);
void relocateTarget(Sprite *cursor, Sprite *target, PlayerInfo *player);
void setTargetSides(Sprite *cursor, Sprite *target);
void checkIfCursorPassesTarget(Sprite *cursor, Sprite * target);
BITMAP *grabframe(BITMAP *source, int width, int height, int startx, int starty, int columns, int frame);
Sprite *createMascotAnimSprite();
void animateMascot(PlayerInfo *player);
#endif
