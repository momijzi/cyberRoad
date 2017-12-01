#pragma once

#include"Texture.h"
#include"Sprite.h"

class Map;

class Shadow
{
private:
	
public:
	
	Shadow();
	~Shadow();

	void Release();

	//影が出るかどうか
	bool *stageShadow;
	bool *stageShadow2;
	bool lightmove;
	int lightWay;//光のある方向　東、南、西、北
	int X, Y;

	void SetShadow(Map &map);
	void MoveLight(int l);
	void SetLight(int l) { lightWay = l; }
	void MoveShadow(int x, int y , int m, Map &map);
	void DrawShadow(int x, int y,int Pixel,int px,int py,Sprite *sprite,Texture &tex, int sw, int sh, Map &map);
	void FalseLightMove() { lightmove = false; }
};
