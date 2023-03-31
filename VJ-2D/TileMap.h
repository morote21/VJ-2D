#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE

#include <glm/glm.hpp>
#include <vector>
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
	bool collisionMoveUpAnything(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;

	int getTileSizeX() const;
	int getTileSizeY() const;
	glm::ivec2 getMapSize() const;
	glm::ivec2 getDoorPos();
	int getTileInPos(int x, int y) const;
	void tileStepped(int x, int y);
	bool keyAppeared() const;
	void keyCollected();
	void setAllSteppedTiles();
	vector<glm::ivec2> getItemsPositions();
	vector<glm::ivec2> getSoldiersPositions();
	vector<glm::ivec2> getAliensPositions();
	glm::ivec2 getPlayerInitPos();

private:
	bool loadLevel(const string& levelFile);
	void prepareArrays(const glm::vec2& minCoords, ShaderProgram& program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	bool doorOpened; // but we have keyCollected on Scene...
	int nTiles, validTiles, steppedTiles;
	glm::ivec2 position, mapSize, tilesheetSize, doorPos, playerInitPos;
	int tileWidth, tileHeight;
	Texture tilesheet;
	glm::vec2 tileTexSize, padding;
	ShaderProgram texProgram;
	int* map;	// seguramente poner aqui una matriz con un vector de vectores en vez de simular una matriz 
	vector<glm::ivec2> itemsPositions, soldiersPositions, aliensPositions;

};

#endif