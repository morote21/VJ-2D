#ifndef _PAUSE_INCLUDE
#define _PAUSE_INCLUDE

#include <glm/glm.hpp>
#include "TexturedQuad.h"
#include "Sprite.h"

class Pause
{

public:
	Pause();
	~Pause();

	void init();
	void initShaders();
	void update(int deltaTime);
	void render();
	bool isPaused();
	void setPaused(bool paused);


private:
	
	int selectorIndex;
	float currentTime;
	bool state;	// juego pausado o no
	vector<TexturedQuad*> backgroundArray;
	Texture spritesheet, pauseWindow;
	ShaderProgram texProgram;
	glm::mat4 projection;

};


#endif
