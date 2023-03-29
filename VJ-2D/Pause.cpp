#include "Pause.h"
#include "Scene.h"
#include "Game.h"
#include "SceneManager.h"
#include <GL/glut.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Pause::Pause()
{

}

Pause::~Pause()
{
	
	for (int i = 0; i < backgroundArray.size(); i++) {
		if (backgroundArray[i] != NULL)
			delete backgroundArray[i];
	}
}

void Pause::init()
{
	initShaders();
	
	glm::vec2 geom[2] = { glm::vec2(80.0f, 0.f), glm::vec2(SCREEN_WIDTH - 80, SCREEN_HEIGHT - 80) };
	glm::vec2 texCoords[2] = { glm::vec2(0.0f, 0.0f), glm::vec2(0.5f, 0.5f) };
	backgroundArray.push_back(TexturedQuad::createTexturedQuad(geom, texCoords, texProgram));

	texCoords[0] = glm::vec2(0.5f, 0.0f); texCoords[1] = glm::vec2(1.f, 0.5f);
	backgroundArray.push_back(TexturedQuad::createTexturedQuad(geom, texCoords, texProgram));

	texCoords[0] = glm::vec2(0.0f, 0.5f); texCoords[1] = glm::vec2(0.5f, 1.f);
	backgroundArray.push_back(TexturedQuad::createTexturedQuad(geom, texCoords, texProgram));
	
	texCoords[0] = glm::vec2(0.5f, 0.5f); texCoords[1] = glm::vec2(1.f, 1.f);
	backgroundArray.push_back(TexturedQuad::createTexturedQuad(geom, texCoords, texProgram));

	pauseWindow.loadFromFile("images/pause.png", TEXTURE_PIXEL_FORMAT_RGBA);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	selectorIndex = 0;
	state = false;
}


void Pause::update(int deltaTime)
{
	currentTime += deltaTime;
	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && !Game::instance().getSpecialKeyAlreadyPressing(GLUT_KEY_DOWN)) {
		if (Game::instance().getStart()) {
			if (selectorIndex == 3) 
				selectorIndex = 0;
			else
				selectorIndex += 1;
			Game::instance().setSpecialKeyAlreadyPressing(GLUT_KEY_DOWN);
			cout << selectorIndex << endl;
		}
	}

	if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !Game::instance().getSpecialKeyAlreadyPressing(GLUT_KEY_UP)) {
		if (Game::instance().getStart()) {
			if (selectorIndex == 0)
				selectorIndex = 3;
			else
				selectorIndex -= 1;
			Game::instance().setSpecialKeyAlreadyPressing(GLUT_KEY_UP);
			cout << selectorIndex << endl;
		}
	}

	if (Game::instance().getKey(13) && !Game::instance().getKeyAlreadyPressing(13)) {
		if (Game::instance().getStart()) {
			if (selectorIndex == 0)
				state = false;
			
			else if (selectorIndex == 1) {
				Game::instance().resetMaps();
				SoundManager::instance().playStage(0);
				state = false;
			}

			else if (selectorIndex == 3) {
				Game::instance().setStart(false);
				Game::instance().resetMaps();
				SoundManager::instance().playMenu();
			}
			Game::instance().setKeyAlreadyPressing(13);
		}
	}

	if (Game::instance().getKey(27) && !Game::instance().getKeyAlreadyPressing(27)) {
		if (Game::instance().getStart()) {
				state = false;
			Game::instance().setKeyAlreadyPressing(27);
		}
	}

}

void Pause::render()
{
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	glm::mat4 modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	backgroundArray[selectorIndex]->render(pauseWindow);
}


void Pause::initShaders()
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

void Pause::setPaused(bool paused) {
	state = paused;
}

bool Pause::isPaused()
{
	return state;
}