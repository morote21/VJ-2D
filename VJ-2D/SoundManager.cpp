#include "SoundManager.h"
#include <iostream>
using namespace std;

SoundManager::SoundManager()
{
	engine = NULL;
	menu = stage1 = stage2 = stage3 = item = death = NULL;
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
	menu->setVolume(0.4f);

	stage1 = engine->play2D("sounds/stage1.mp3", true, true);
	stage1->setVolume(0.2f);

	stage2 = engine->play2D("sounds/stage2.mp3", true, true);
	stage2->setVolume(0.2f);

	stage3 = engine->play2D("sounds/stage3.mp3", true, true);
	stage3->setVolume(0.2f);

	//item = engine->play2D("sounds/coin.mp3", true, true);
	//item->setVolume(0.5f);
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

void SoundManager::playItem()
{
	item = engine->play2D("sounds/coin.mp3", false, true);
	item->setIsPaused(false);
	item->setVolume(0.5f);
	item->setPlayPosition(200);
}

void SoundManager::playDeath()
{
	item = engine->play2D("sounds/death.mp3", false, true);
	item->setIsPaused(false);
	item->setVolume(0.4f);
	item->setPlayPosition(320);
}

void SoundManager::playDoor()
{
	door = engine->play2D("sounds/door.mp3", false, true);
	door->setIsPaused(false);
	door->setVolume(0.4f);
	door->setPlayPosition(0);
}

void SoundManager::playThree()
{
	three = engine->play2D("sounds/three.mp3", false, true);
	three->setIsPaused(false);
	three->setVolume(0.4f);
	three->setPlayPosition(100);
}

void SoundManager::playTwo()
{
	two = engine->play2D("sounds/two.mp3", false, true);
	two->setIsPaused(false);
	two->setVolume(0.4f);
	two->setPlayPosition(100);
}

void SoundManager::playOne()
{
	one = engine->play2D("sounds/one.mp3", false, true);
	one->setIsPaused(false);
	one->setVolume(0.4f);
	one->setPlayPosition(100);
}

void SoundManager::playStart()
{
	start = engine->play2D("sounds/start.mp3", false, true);
	start->setIsPaused(false);
	start->setVolume(0.4f);
	start->setPlayPosition(100);
}

void SoundManager::playMissionComplete()
{
	missioncomplete = engine->play2D("sounds/missioncomplete.mp3", false, true);
	missioncomplete->setIsPaused(false);
	missioncomplete->setVolume(0.4f);
	missioncomplete->setPlayPosition(100);
}

void SoundManager::playPoison()
{
	poison = engine->play2D("sounds/poison.mp3", false, true);
	poison->setIsPaused(false);
	poison->setVolume(0.4f);
	poison->setPlayPosition(0);
}

void SoundManager::playGameover()
{
	gameover = engine->play2D("sounds/gameover.wav", false, true);
	gameover->setIsPaused(false);
	gameover->setVolume(0.4f);
	gameover->setPlayPosition(0);
}

void SoundManager::playAllOver()
{
	allover = engine->play2D("sounds/allover.wav", false, true);
	allover->setIsPaused(false);
	allover->setVolume(0.4f);
	allover->setPlayPosition(700);
}