#ifndef _SOUNDMANAGER_INCLUDE
#define _SOUNDMANAGER_INCLUDE

#include <irrKlang.h>
using namespace irrklang;

class SoundManager
{
public:
	SoundManager();
	~SoundManager();
	
	static SoundManager& instance() {
		static SoundManager SM;
		return SM;
	}

	void init();
	void playMenu();
	void playItem();
	void playStage(int nStage);
	void playDeath();
	void playDoor();
	void playThree();
	void playTwo();
	void playOne();
	void playStart();
	void playMissionComplete();
	ISoundEngine* getSoundEngine();

private:
	ISoundEngine* engine;
	ISound* menu, * stage1, * stage2, * stage3, * item, * death, * door, * three, * two, * one, * start;
	ISound* missioncomplete;
};

#endif	