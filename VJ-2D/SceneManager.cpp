#include "SceneManager.h"
#include "Game.h"
#include <string>
using namespace std;

SceneManager::SceneManager() {}

SceneManager::~SceneManager() 
{
	for (Scene* scene : sceneArray)
		delete scene;
}

int SceneManager::currentScene;

void SceneManager::init()
{
	Scene *scene1 = new Scene(), *scene2 = new Scene();
	scene1->init("levels/level01.txt"); scene2->init("levels/level02.txt");
	sceneArray.push_back(scene1); sceneArray.push_back(scene2);
	currentScene = 0;

}

void SceneManager::update(int deltaTime) // ??? (keys)
{
	// En todos los casos, reseteamos el nivel actual, y nos movemos al correspondiente
	if (Game::instance().getKey('1') && !Game::instance().getKeyAlreadyPressing('1')) {
		Game::instance().setKeyAlreadyPressing('1');
		string levelString = "levels/level0";
		levelString += to_string(currentScene+1);
		levelString += ".txt";
		sceneArray[currentScene]->init(levelString);
		currentScene = 0;
	}

	if (Game::instance().getKey('2') && !Game::instance().getKeyAlreadyPressing('2')) {
		Game::instance().setKeyAlreadyPressing('2');
		string levelString = "levels/level0";
		levelString += to_string(currentScene+1);
		levelString += ".txt";
		sceneArray[currentScene]->init(levelString);
		currentScene = 1;
	}
	
	if (sceneArray[currentScene]->getDoorEntered()) {
		// Reseteamos el nivel actual y pasamos al siguiente
		string levelString = "levels/level0";
		levelString += to_string(currentScene + 1);
		levelString += ".txt";
		sceneArray[currentScene]->init(levelString);

		currentScene += 1;
		if (currentScene == sceneArray.size()) {
			Game::instance().setStart(false);
			currentScene = 0;
		}
	}

	sceneArray[currentScene]->update(deltaTime);
}

void SceneManager::render()
{
	sceneArray[currentScene]->render();
}

void SceneManager::resetLevels() 
{
	for (int i = 1; i <= sceneArray.size(); i++) {
		string levelString = "levels/level0";
		levelString += to_string(i);
		levelString += ".txt";
		sceneArray[i - 1]->init(levelString);
	}
	currentScene = 0;
}

int SceneManager::getCurrentScene()
{
	return currentScene;
}