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
	void playStage(int nStage);
	ISoundEngine* getSoundEngine();

private:
	ISoundEngine* engine;
	ISound* menu, * stage1, * stage2, * stage3;

};

#endif	