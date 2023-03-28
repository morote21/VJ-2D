#include "SceneManager.h"
#include "Game.h"
#include <string>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
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
	initShaders();
	Scene *scene1 = new Scene(), *scene2 = new Scene();
	scene1->init("levels/level01.txt"); scene2->init("levels/level02.txt");
	sceneArray.push_back(scene1); sceneArray.push_back(scene2);
	currentScene = 0;

	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, 40) };
	glm::vec2 texCoords[2] = { glm::vec2(0.0f, 0.0f), glm::vec2(1.f, 1.f) };
	hudBackgroundTexture.loadFromFile("images/hudbackground.png", TEXTURE_PIXEL_FORMAT_RGBA);
	hudBackground = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	
	numbersTexture.loadFromFile("images/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);

	geom[0] = glm::vec2(0.f, 0.f); geom[1] = glm::vec2(20.f, 30.f);
	for (int i = 0; i < 10; i++) {
		texCoords[0] = glm::vec2(i/10.f, 0.0f); texCoords[1] = glm::vec2((i+1)/10.f, 1.f);
		numbers.push_back(TexturedQuad::createTexturedQuad(geom, texCoords, texProgram));
	}
	
	
	
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);

	lives = 3;
	score = 0;

}

void SceneManager::update(int deltaTime) // ??? (keys)
{
	// En todos los casos, reseteamos el nivel actual, y nos movemos al correspondiente
	//(¿resetear vidas y score si hacemos lo de las teclas? porque estamos forzando cambio de nivel sin completarlo...)
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
	
	sceneArray[currentScene]->update(deltaTime, lives, score);
	
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
			lives = 3;
			score = 0;
		}
	}
}

void SceneManager::render()
{
	int timer = sceneArray[currentScene]->render();

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	glm::mat4 modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	hudBackground->render(hudBackgroundTexture);

	// render de HUD aquí

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(70, 5, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	numbers[lives]->render(numbersTexture);

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(300, 5, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	numbers[int((score / 10000) % 10)]->render(numbersTexture);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(322, 5, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	numbers[int((score / 1000) % 10)]->render(numbersTexture);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(344, 5, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	numbers[int((score / 100) % 10)]->render(numbersTexture);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(366, 5, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	numbers[int((score / 10) % 10)]->render(numbersTexture);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(388, 5, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	numbers[int(score % 10)]->render(numbersTexture);


	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(580, 5, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	numbers[int((timer / 10) % 10)]->render(numbersTexture);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(602, 5, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	numbers[int(timer % 10)]->render(numbersTexture);


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
	lives = 3;
	score = 0;
}

void SceneManager::resetLevel(int level)
{
	string levelString = "levels/level0";
	levelString += to_string(level);
	levelString += ".txt";
	sceneArray[level - 1]->init(levelString);
	currentScene = level;
}

int SceneManager::getCurrentScene()
{
	return currentScene;
}

void SceneManager::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}