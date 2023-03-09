#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE

#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"

class TileMap
{
private:
	TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap* createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

	~TileMap();

	void render(const glm::vec2& minCoords, ShaderProgram& program);
	void free();

	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;

	int getTileSizeX() const;
	int getTileSizeY() const;
	glm::ivec2 getMapSize() const;
	glm::ivec2 getDoorPos();
	int getTileInPos(int x, int y) const;
	void tileStepped(int x, int y);
	bool keyAppeared() const;
	void keyCollected();

private:
	bool loadLevel(const string& levelFile);
	void prepareArrays(const glm::vec2& minCoords, ShaderProgram& program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	bool doorOpened;
	int nTiles, validTiles, steppedTiles;
	glm::ivec2 position, mapSize, tilesheetSize, doorPos;
	int tileWidth, tileHeight;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int* map;	// seguramente poner aqui una matriz con un vector de vectores en vez de simular una matriz 
};

#endif