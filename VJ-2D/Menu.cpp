#include "Menu.h"
#include "Game.h"
#include "SoundManager.h"
#include <GL/glut.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Menu::Menu() 
{
	
}

Menu::~Menu()
{
	for (int i = 0; i < backgroundArray.size(); i++) {
		if (backgroundArray[i] != NULL)
			delete backgroundArray[i];
	}
}

void Menu::init()
{
	initShaders();	// carga la configuracion del texProgram
	credits.init(texProgram);
	instructions.init(texProgram);

	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) };
	glm::vec2 texCoords[2] = { glm::vec2(0.0f, 0.0f), glm::vec2(0.5f, 0.5f) };
	backgroundArray.push_back(TexturedQuad::createTexturedQuad(geom, texCoords, texProgram));

	texCoords[0] = glm::vec2(0.5f, 0.0f); texCoords[1] = glm::vec2(1.f, 0.5f);
	backgroundArray.push_back(TexturedQuad::createTexturedQuad(geom, texCoords, texProgram));

	texCoords[0] = glm::vec2(0.0f, 0.5f); texCoords[1] = glm::vec2(0.5f, 1.f);
	backgroundArray.push_back(TexturedQuad::createTexturedQuad(geom, texCoords, texProgram));

	menuWindow.loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGB);
	//instructionsWindow.loadFromFile("images/instructions.png", TEXTURE_PIXEL_FORMAT_RGB);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	selectorIndex = 0;
}

void Menu::update(int deltaTime)
{
	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && !Game::instance().getSpecialKeyAlreadyPressing(GLUT_KEY_DOWN)) {
		if (!Game::instance().getStart()) {
			if (currentWindow == 0) {
				if (selectorIndex == 2)
					selectorIndex = 0;
				else
					selectorIndex += 1;
				Game::instance().setSpecialKeyAlreadyPressing(GLUT_KEY_DOWN);
				cout << selectorIndex << endl;
			}
		}
	}

	if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !Game::instance().getSpecialKeyAlreadyPressing(GLUT_KEY_UP)) {
		if (!Game::instance().getStart()) {
			if (currentWindow == 0) {
				if (selectorIndex == 0)
					selectorIndex = 2;
				else
					selectorIndex -= 1;
				Game::instance().setSpecialKeyAlreadyPressing(GLUT_KEY_UP);
				cout << selectorIndex << endl;
			}
		}
	}

	if (Game::instance().getKey(13) && !Game::instance().getKeyAlreadyPressing(13)) {
		if (!Game::instance().getStart()) {
			if (selectorIndex == 0) {
				Game::instance().setStart(true);
				SoundManager::instance().playStage(0);
			}
			else
				currentWindow = selectorIndex;
			Game::instance().setKeyAlreadyPressing(13);
		}
	}

	if (Game::instance().getKey(27) && !Game::instance().getKeyAlreadyPressing(27)) {
		if (!Game::instance().getStart()) {
			if (currentWindow == 0)
				Game::instance().stopGame();
			else if (currentWindow == 1 || currentWindow == 2) {
				if (currentWindow == 1)
					instructions.resetSelectedWindow(); // para cuando vuelvas al menú de instrucciones

				currentWindow = 0;	
			}
			Game::instance().setKeyAlreadyPressing(27);
		}
	}

	
	if (currentWindow == 1)
		instructions.update(deltaTime);
	else if (currentWindow == 2)
		credits.update(deltaTime);
}

void Menu::render()
{
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	glm::mat4 modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	
	if (currentWindow == 0) {
		backgroundArray[selectorIndex]->render(menuWindow);
	}
	else if (currentWindow == 1)
		instructions.render(texProgram);
	else if (currentWindow == 2)
		credits.render(texProgram);
}

void Menu::setSelectorIndex(int index)
{
	selectorIndex = index;
}

int Menu::getSelectorIndex()
{
	return selectorIndex;
}

void Menu::initShaders()
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