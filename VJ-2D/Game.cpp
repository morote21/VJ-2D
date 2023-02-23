#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true; start = false;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	menu.init();	// menu
	//scene.init();	// game

}

bool Game::update(int deltaTime)
{
	//scene.update(deltaTime);
	menu.update(deltaTime);
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//scene.render();
	menu.render(numScreen);
}

void Game::keyPressed(int key)
{
	if (key == 27) // Escape code
		bPlay = false;
	if (key == '0')
		if (!start)		// Ir al menu principal (temporal)
			numScreen = 0;	
	if (key == '2')
		if (!start)		// Ir a los creditos (temporal)
			numScreen = 2;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	if (key == GLUT_KEY_DOWN)
	{
		if (!start) 
		{
			if (numScreen == 0)
			{
				int currentIndex = menu.getSelectorIndex();
				if (currentIndex == 2)
					menu.setSelectorIndex(0);
				else
					menu.setSelectorIndex(currentIndex + 1);
			}
		}
	}
	if (key == GLUT_KEY_UP)
	{
		if (!start)
		{
			if (numScreen == 0)
			{
				int currentIndex = menu.getSelectorIndex();
				if (currentIndex == 0)
					menu.setSelectorIndex(2);
				else
					menu.setSelectorIndex(currentIndex - 1);
			}
		}
	}
	
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
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





