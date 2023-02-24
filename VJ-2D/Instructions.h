#ifndef _INSTRUCTIONS_INCLUDE
#define _INSTRUCTIONS_INCLUDE

#include <glm/glm.hpp>
#include "TexturedQuad.h"
#include "Sprite.h"
#include <vector>


class Instructions
{
public:
	Instructions();
	~Instructions();

	void init(ShaderProgram& texProgram);
	void render(ShaderProgram& texProgram);
	void update(int deltaTime);


private:
	int selectedWindow;
	Texture instructionsWindowSheet;
	vector<TexturedQuad*> instructionsWindows;
	glm::mat4 projection;
	int currentTime;
};

#endif