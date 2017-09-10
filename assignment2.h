#ifndef _ASSIGNMENT2_H
#define _ASSIGNMENT2_H 1

#include <allegro.h>
#include "assignment2.h"
#include "sound.h"
#include "sprite.h"
#include "spritehandler.h"
#include "playerinfo.h"

#define MODE_W GFX_AUTODETECT_WINDOWED
#define MODE_F GFX_AUTODETECT_FULLSCREEN

#define WIDTH 640
#define HEIGHT 480

#define CHARACTER_WIDTH 8
#define LINE_SPACING 10

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
void hitTheTarget(Sprite *cursor, Sprite *target, PlayerInfo *player);
void setTargetSides(Sprite *cursor, Sprite *target);
void checkIfCursorPassesTarget(Sprite *cursor, Sprite * target);
#endif
