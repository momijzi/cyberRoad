#include "Map.h"

Map::Map()
{
	mapHeight = 0;
	mapWidth = 0;
	mapNum = 0;
	lightw = 0;
	startX = 0;
	startY = 0;
}

Map::~Map()
{
	Release();
}

void Map::Release()
{
	if (mapBlock)
	{
		delete[] mapBlock;
		mapBlock = nullptr;
	}

	if (mapArrow)
	{
		delete[] mapArrow;
		mapArrow = nullptr;
	}
}

void Map::SetMapData(int num)
{
	mapNum = num;
	std::string textName1("Text/Map");
	std::string textName2(".txt");
	std::string textName = textName1 + std::to_string(num) + textName2;
	std::ifstream mapText;

	Reset(mapText, textName);
}

void Map::Reset(std::ifstream& mapText, std::string textName)
{
	mapText.open(textName);
	mapText >> mapHeight;
	mapText >> mapWidth;
	mapText >> lightw;
	mapText >> startX;
	mapText >> startY;
	mapBlock = new int[mapHeight*mapWidth];
	mapArrow = new int[mapHeight*mapWidth];
	SetMapArray(mapHeight, mapWidth, mapText, mapBlock);
	SetMapArray(mapHeight, mapWidth, mapText, mapArrow);
	mapText.close();
}

void Map::SetMapArray(int Height, int Width, std::ifstream& data, int *array)
{
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			array[i*Width + j] = 0;
		}
	}
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			data >> array[i*Width + j];
		}
	}
}

void Map::SetMapBlock(int x, int y, int num)
{
	mapBlock[x + y*mapWidth] = num;
}

void Map::SetMapArrow(int x, int y, int num)
{
	mapArrow[x + y*mapWidth] = num;
}