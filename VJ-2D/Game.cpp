#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include "Game.h"

using namespace std;


void Game::init()
{
	bPlay = true; start = false; level = 1, menuSoundStarted = true;
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	menu.init();	// menu
	sceneManager.init();	// game

	for (int i = 0; i < 256; i++) {
		keysAlreadyPressing[i] = false;
		specialKeysAlreadyPressing[i] = false;
	}

	SoundManager::instance().playMenu();

}

bool Game::update(int deltaTime)
{
	if (!start) {
		menu.update(deltaTime);
	}
	
	else
		sceneManager.update(deltaTime);
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (!start)
		menu.render();
	else
		sceneManager.render();
}

void Game::keyPressed(int key)
{
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
	keysAlreadyPressing[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
	specialKeysAlreadyPressing[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

void Game::setStart(bool state)
{
	start = state;
}

bool Game::getStart()
{
	return start;
}

void Game::setKeyAlreadyPressing(int key)
{
	keysAlreadyPressing[key] = true;
}

void Game::setSpecialKeyAlreadyPressing(int key)
{
	specialKeysAlreadyPressing[key] = true;
}

bool Game::getKeyAlreadyPressing(int key) const 
{
	return keysAlreadyPressing[key];
}

bool Game::getSpecialKeyAlreadyPressing(int key) const
{
	return specialKeysAlreadyPressing[key];
}

void Game::stopGame()
{
	bPlay = false;
}


void Game::resetMaps()
{
	sceneManager.resetLevels();
}