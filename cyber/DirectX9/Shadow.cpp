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
			//�e�̔z��̏�����
			stageShadow[x + y*map.GetMapWidth()] = false;
			stageShadow2[x + y*map.GetMapWidth()] = false;
		}
	}
	lightWay = map.GetLightW();
}

void Shadow::MoveLight(int l)
{
	lightWay += l;

	//�͈͊O�ɒl���s�����Ƃ��ŏ��܂��͍Ō�ɖ߂�
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
	//�Ȃɂ��󂯕t���Ȃ��O�ǂ̓}�C�i�X�Ȃ̂Ő��̐��ɕύX
	for (int shadow = 1; shadow <= m; shadow++)
	{
		X = -1, Y = -1;
		
		switch (lightWay)
		{
			case 0://�k
				X = x;
				Y = y + shadow;
				break;
			case 1://��
				Y = y;
				X = x - shadow;
				break;
			case 2://��
				X = x;
				Y = y - shadow;
				break;
			case 3://��
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
				//false�ɂ���������������邱�ƂŐ؂�ւ��`��\
				//�^�C���̎擾���K�v
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
