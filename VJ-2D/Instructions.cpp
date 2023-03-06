#include "Instructions.h"
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glut.h>

Instructions::Instructions()
{
}

Instructions::~Instructions()
{
	for (int i = 0; i < instructionsWindows.size(); i++) {
		if (instructionsWindows[i] != NULL)
			delete instructionsWindows[i];
	}
}

void Instructions::init(ShaderProgram& texProgram)
{
	selectedWindow = 0;
	instructionsWindowSheet.loadFromFile("images/instructions.png", TEXTURE_PIXEL_FORMAT_RGB);

	glm::vec2 geom[2] = { glm::vec2(0.0f, 0.0f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) };
	glm::vec2 texCoords[2] = { glm::vec2(0.0f, 0.0f), glm::vec2(0.5f, 0.5f) };
	instructionsWindows.push_back(TexturedQuad::createTexturedQuad(geom, texCoords, texProgram));

	texCoords[0] = glm::vec2(0.5f, 0.0f); texCoords[1] = glm::vec2(1.0f, 0.5f);
	instructionsWindows.push_back(TexturedQuad::createTexturedQuad(geom, texCoords, texProgram));

	texCoords[0] = glm::vec2(0.0f, 0.5f); texCoords[1] = glm::vec2(0.5f, 1.0f);
	instructionsWindows.push_back(TexturedQuad::createTexturedQuad(geom, texCoords, texProgram));

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
}

void Instructions::render(ShaderProgram& texProgram)
{
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	glm::mat4 modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	instructionsWindows[selectedWindow]->render(instructionsWindowSheet);
}

void Instructions::update(int deltaTime)
{
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && !Game::instance().getSpecialKeyAlreadyPressing(GLUT_KEY_LEFT)) {
		if (!Game::instance().getStart()) {
			if (selectedWindow != 0)
				selectedWindow -= 1;
			Game::instance().setSpecialKeyAlreadyPressing(GLUT_KEY_LEFT);			
		}
	}

	if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && !Game::instance().getSpecialKeyAlreadyPressing(GLUT_KEY_RIGHT)) {
		if (!Game::instance().getStart()) {
			if (selectedWindow != instructionsWindows.size() - 1)
				selectedWindow += 1;
			Game::instance().setSpecialKeyAlreadyPressing(GLUT_KEY_RIGHT);
		}
	}

	currentTime += deltaTime;	// posiblemente sobre
}