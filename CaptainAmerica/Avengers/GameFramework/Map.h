#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include <unordered_map> 
#include <iostream>
#include <fstream>
#include <string>

#include "Sprite.h"
#include "Debug.h"
#include "Global.h"
#include "GameObject.h"

using namespace std;

struct Tile {
	int tileId;
	ObjectType type;
	int x, y;
	int width = 16, height = 16;
	Collider *colider;
};

typedef vector<Tile> Row;
typedef vector<Row> Matrix;

const vector<int> _BrickStage_1{ 11, 12, 13, 34, 42, 65, 92, 93, 96, 97 };
const vector<int> _BrickStage_2{ 5, 11, 12, 13, 14, 15, 21, 22, 54, 55, 56, 57, 58, 76, 77, 78, 79, 80, 94, 98, 119, 126, 127, 128, 239, 240, 241, 242, 243, 253, 254 };
const vector<int> _BrickStage_BOSS_1{ 1, 2, 7, 8, 9, 11 };
const vector<int> _BrickStage_BOSS_2{  };

class CMap
{
private:
	static CMap *__instance;

	string LoadMatrix(LPCWSTR filePath);
	void LoadTileset(LPCWSTR tilesLocation);
	Matrix matrix;

	void AddObjects(Stage stage) { };
	int mapWidth, mapHeight;
	int tilesetWidth, tilesetHeight;

	Row GetMatrixRow(int lineNum, string line, string delimiter);

	LPCWSTR infoLocation;
	unordered_map<int, Sprite*> tiles;

public:
	static CMap *GetInstance(LPCWSTR filePath = NULL);
	CMap(LPCWSTR filePath);

	int GetWidth() { return this->mapWidth; }
	int GetHeight() { return this->mapHeight; }
	int GetTilesetWidth() { return this->tilesetWidth; }
	int GetTilesetHeight() { return this->tilesetHeight; }

	Matrix& GetMatrix() { return this->matrix; }

	static void ReleaseMap()	{	__instance = NULL;	}
	void RenderTile(Tile *curTile);
	void Render();

	~CMap() { };
};