#ifndef _SOUND_H
#include <allegro.h>

#define BGM "sounds/pocketTrivia_BGM.wav"
#define CORRECT_SOUND "sounds/pocketTrivia_Correct.wav"
#define INCORRECT_SOUND "sounds/pocketTrivia_Incorrect.wav"

class Sound {
	private:
		int _panning;
		int _pitch;
		int _volume;
		SAMPLE *_bgm;
		SAMPLE *_soundEffect;
	public:
		Sound();
		~Sound();
		
		void setPanning(int panning);
		int getPanning();
		
		void setPitch(int pitch);
		int getPitch();
		
		void setVolume(int volume);
		int getVolume();
		
		void setBGM(const char *bgmName);
		SAMPLE *getBGM();
		
		void setSoundEffect(const char *soundEffectName);
		SAMPLE *getSoundEffect();

		void playMusic();
		void stopMusic();
		
		void playSoundEffect();
		void stopSoundEffect();

		void updateSound(SAMPLE *sound);
		void updateAllSounds();
		
		void PollTurnOnOrOffMusic();
};

#endif

