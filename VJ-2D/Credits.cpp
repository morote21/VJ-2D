#include "Credits.h"
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>

Credits::Credits()
{
}

Credits::~Credits()
{
	if (background != NULL) delete background;
}

void Credits::init(ShaderProgram& texProgram)
{
	glm::vec2 geom[2] = { glm::vec2(0.0f, 0.0f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) };
	glm::vec2 texCoords[2] = { glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f) };
	creditsWindow.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGB);
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
}

void Credits::render(ShaderProgram& texProgram)
{
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	glm::mat4 modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	background->render(creditsWindow);
}

void Credits::update(int deltaTime)
{
	currentTime += deltaTime;	// lo mas seguro es que sobre porque no se usa para nada
}