#pragma once
#include <fstream>
#include <string>

class Map
{
private:
	//マップの高さと幅
	int mapHeight;
	int mapWidth;
	int startX;
	int startY;
	//マップを格納する配列
	int *mapBlock;
	int *mapArrow;
	//マップ番号
	int mapNum;
	int lightw;

public:
	Map();
	~Map();
	int GetMapBlock(int x, int y) { return mapBlock[x + y*mapWidth]; }
	int GetMapArrow(int x, int y) { return mapArrow[x + y*mapWidth]; }
	int GetMapHeight() { return mapHeight; }
	int GetMapWidth() { return mapWidth; }
	int GetLightW() { return lightw; }
	int GetStartX() { return startX; }
	int GetStartY() { return startY; }
	void SetMapData(int num);
	void SetMapArray(int Height, int Width, std::ifstream& data, int *array);
	void SetMapBlock(int x, int y, int num);
	void SetMapArrow(int x, int y, int num);
	void Release();
	void Reset(std::ifstream& mapText, std::string textName);
};