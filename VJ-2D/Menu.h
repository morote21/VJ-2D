#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
#include "TexturedQuad.h"
//#include "Sprite.h"

class Menu 
{
public:
	Menu();
	~Menu();

	void init();	
	void initShaders();
	void update(int deltaTime);
	void render();
	
private:
	int id;
	float currentTime;
	TexturedQuad* background;
	Texture menuWindow;
	ShaderProgram texProgram;
	glm::mat4 projection;

};


#endif // _MENU_INCLUDE
