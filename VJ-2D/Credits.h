#ifndef _CREDITS_INCLUDE
#define _CREDITS_INCLUDE

#include <glm/glm.hpp>
#include "TexturedQuad.h"
#include "Sprite.h"


class Credits
{
public:

	Credits();
	~Credits();

	void init(ShaderProgram& texProgram);
	void render(ShaderProgram& texProgram);
	void update(int deltaTime);


private:
	Texture creditsWindow;
	TexturedQuad* background;
	glm::mat4 projection;
	int currentTime;
};






# endif 
