#ifndef _PLAYERINFO_H

#include "sprite.h"

#define HITSBEFORELEVELUP 5

class PlayerInfo {
	private:
		int _level;
		int _score;
		int _highestScore;
		int _highestLevel;
		int _difficulty;
		int _numHits;
	public:
		PlayerInfo();
		~PlayerInfo();
		
		void IncreaseLevel(Sprite *playerCursor);
		void IncreaseScore(Sprite *playerCursor);
		void setDifficulty(int difficulty, Sprite *playerCursor);
		
		int getLevel();
		void setLevel(int level);
		int getScore();
		void setScore(int score);
		int getHighestScore();
		void setHighestScore(int highestScore);
		int getHighestLevel();
		void setHighestLevel(int highestLevel);
		int getDifficulty();
		void setDifficulty(int difficulty);
		int getNumHits();
		void setNumHits(int numHits);
};

#endif

