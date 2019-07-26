#pragma once
#include "GameObject.h"
#include "Global.h" 
#include "GridCell.h"
#include "../GameObject/Captain/Captain.h"

#include <vector>

#include <fstream>
#include <string>

using namespace std;

class Cell;

typedef vector<GridCell *> CellRow;
typedef vector<CellRow> CellMatrix;
class Grid {
private:
	static Grid * __instance;
	int width, height;

	CellMatrix cells;

	vector<Tile *> curTiles;

	vector<Tile *> CollisionTiles;

	Camera *Camera;

	Captain * captain;

	Grid();

public:
	static Grid * GetInstance();
	void LoadCells();

	void GetCameraPosOnGrid(int &l, int &r, int &t, int &b);
	void GetCaptainPosOnGrid(int &l, int &r, int &t, int &b);

	vector<Tile *> GetCurTiles() { return this->curTiles; }
	vector<Tile *> GetCollisionTiles() { return this->CollisionTiles; }
 
	void Update(DWORD dt);
	void Render();

	CellMatrix GetTile() { return this->cells; }

	static void SetNewGrid()
	{
		__instance = NULL;
		__instance = new Grid();
	}
	~Grid() {
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				if (cells[i][j] != NULL)
					delete cells[i][j];
			}
	}
};