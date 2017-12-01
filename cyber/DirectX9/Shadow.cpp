#include"Shadow.h"
#include"Map.h"

Shadow::Shadow()
{
	lightmove = false;
	lightWay = 0;
	X = -1, Y = -1;
}
Shadow::~Shadow()
{
	Release();
}

void Shadow::Release()
{
	delete[] stageShadow;
	stageShadow = nullptr;

	delete[] stageShadow2;
	stageShadow2 = nullptr;
}

void Shadow::SetShadow(Map &map)
{
	stageShadow = new bool[map.GetMapWidth()*map.GetMapHeight()];
	stageShadow2 = new bool[map.GetMapWidth()*map.GetMapHeight()];
	for (int x = 0; x < map.GetMapWidth(); x++)
	{
		for (int y = 0; y < map.GetMapHeight(); y++)
		{
			//影の配列の初期化
			stageShadow[x + y*map.GetMapWidth()] = false;
			stageShadow2[x + y*map.GetMapWidth()] = false;
		}
	}
	lightWay = map.GetLightW();
}

void Shadow::MoveLight(int l)
{
	lightWay += l;

	//範囲外に値が行ったとき最初または最後に戻す
	if (lightWay > 3)
	{
		lightWay = 0;
	}
	else if (lightWay < 0)
	{
		lightWay = 3;
	}
	lightmove = true;
}

void Shadow::MoveShadow(int x, int y, int m, Map &map)
{
	//なにも受け付けない外壁はマイナスなので正の数に変更
	for (int shadow = 1; shadow <= m; shadow++)
	{
		X = -1, Y = -1;
		
		switch (lightWay)
		{
			case 0://北
				X = x;
				Y = y + shadow;
				break;
			case 1://東
				Y = y;
				X = x - shadow;
				break;
			case 2://南
				X = x;
				Y = y - shadow;
				break;
			case 3://西
				X = x + shadow;
				Y = y;
				break;
		}
		if (X >= 0 && X < map.GetMapWidth() && Y >= 0 && Y < map.GetMapHeight())
		{
			stageShadow[X + Y*map.GetMapWidth()] = true;
			if (lightmove == false)
			{
				stageShadow2[X + Y*map.GetMapWidth()] = true;
			}
			else
			{
				//falseにする条件を持たせることで切り替え描画可能
				//タイムの取得が必要
				lightmove = false;
			}
		}
	}
}


void Shadow::DrawShadow(int x , int y ,int Pixel,int px,int py, Sprite *sprite, Texture &tex,int sw,int sh, Map &map)
{
	if (stageShadow[x + y*map.GetMapWidth()] == true)
	{
		if (lightmove == false || stageShadow2[x + y*map.GetMapWidth()] == true)
		{
			sprite->SetPos(Pixel * x - Pixel * px + sw / 2,
				Pixel * y - Pixel * py + sh / 2);
			sprite->Draw(tex);
		}
	}
}
