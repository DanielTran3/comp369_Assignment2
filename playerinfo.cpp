#include "playerinfo.h"

PlayerInfo::PlayerInfo() {
	_level = 1;
	_score = 0;
	_highestScore = 0;
	_highestLevel = 0;
	_difficulty = 0;
	_numHits = 0;
	_hasLeveled = false;
}

PlayerInfo::~PlayerInfo() {
	
}

void PlayerInfo::IncreaseLevel(Sprite *cursor) {
	_level++;
	if (_level > _highestLevel) {
		_highestLevel = _level;
		cursor->IncreaseSpeed();
	}
	_hasLeveled = true;
}
void PlayerInfo::IncreaseScore(Sprite *cursor) {
	_score++;
	_numHits++;
	if (_score > _highestScore) {
		_highestScore = _score;
	}
	if (_numHits >= HITSBEFORELEVELUP) {
		IncreaseLevel(cursor);
		_numHits = 0;
	}
}

void setDifficulty(int difficulty, Sprite *playerCursor) {
	
}

int PlayerInfo::getLevel() {
	return _level;
}
void PlayerInfo::setLevel(int level) {
	_level = level;
}
int PlayerInfo::getScore() {
	return _score;
}
void PlayerInfo::setScore(int score) {
	_score = score;
}
int PlayerInfo::getHighestScore() {
	return _highestScore;
}
void PlayerInfo::setHighestScore(int highestScore) {
	_highestScore = highestScore;
}
int PlayerInfo::getHighestLevel() {
	return _highestLevel;
}
void PlayerInfo::setHighestLevel(int highestLevel) {
	_highestLevel = highestLevel;
}
int PlayerInfo::getDifficulty() {
	return _difficulty;
}
void PlayerInfo::setDifficulty(int difficulty) {
	_difficulty = difficulty;
}
int PlayerInfo::getNumHits() {
	return _numHits;
}
void PlayerInfo::setNumHits(int numHits) {
	_numHits = numHits;
}
bool PlayerInfo::HasLeveled() {
	return _hasLeveled;
}
void PlayerInfo::ResetLeveled() {
	_hasLeveled = false;
}
