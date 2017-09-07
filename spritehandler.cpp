#include "spritehandler.h"

SpriteHandler::SpriteHandler(void)
{
	_count = 0;
}

SpriteHandler::~SpriteHandler(void)
{
    //delete the sprites
	for (int n = 0; n < _count; n++)
		delete _sprites[n];
}

void SpriteHandler::Add(Sprite *spr) 
{
	if (spr != NULL) {
		_sprites[_count] = spr;
		_count++;
	}
}

void SpriteHandler::Create() 
{
	_sprites[_count] = new Sprite();
	_count++;
}

Sprite *SpriteHandler::Get(int index)
{
	return _sprites[index];
}

int SpriteHandler::Size() {
	return _count;
}
