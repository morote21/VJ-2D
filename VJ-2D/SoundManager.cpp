#include "SoundManager.h"
#include <iostream>
using namespace std;

SoundManager::SoundManager()
{
	engine = NULL;
	menu = stage1 = stage2 = stage3 = NULL;
}

SoundManager::~SoundManager()
{
	if (engine != NULL)
		engine->drop();
}

void SoundManager::init()
{
	engine = irrklang::createIrrKlangDevice();
	menu = engine->play2D("sounds/menu.mp3", true, true);
	cout << "musica: " << menu << endl;
	menu->setVolume(0.4f);

	stage1 = engine->play2D("sounds/stage1.mp3", true, true);
	stage1->setVolume(0.2f);

	stage2 = engine->play2D("sounds/stage2.mp3", true, true);
	stage2->setVolume(0.2f);

	stage3 = engine->play2D("sounds/stage3.mp3", true, true);
	stage3->setVolume(0.2f);
}


void SoundManager::playMenu()
{
	// pausamos todos menos la musica del menu
	stage1->setIsPaused(true);
	stage2->setIsPaused(true);
	stage3->setIsPaused(true);
	
	menu->setPlayPosition(0);	// iniciamos la cancion desde el principio
	menu->setIsPaused(false);
}

void SoundManager::playStage(int nStage)
{
	if (nStage == 0) {
		menu->setIsPaused(true);
		stage2->setIsPaused(true);
		stage3->setIsPaused(true);

		stage1->setPlayPosition(0);
		stage1->setIsPaused(false);
	}
	else if (nStage == 1) {
		menu->setIsPaused(true);
		stage1->setIsPaused(true);
		stage3->setIsPaused(true);

		stage2->setPlayPosition(0);
		stage2->setIsPaused(false);
	}
	else if (nStage == 2) {
		menu->setIsPaused(true);
		stage1->setIsPaused(true);
		stage2->setIsPaused(true);

		stage3->setPlayPosition(0);
		stage3->setIsPaused(false);
	}
}
