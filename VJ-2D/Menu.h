#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
//#include "TextureQuad.h"
//#include "Sprite.h"

class Menu 
{
public:
	Menu();
	~Menu();

	void init(const int& sceneId);	
	
private:
	int id;
	float currentTime;
	

	glm::mat4 projection;

};


#endif // _MENU_INCLUDE
