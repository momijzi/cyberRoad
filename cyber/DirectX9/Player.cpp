#include"Player.h"
#include "Map.h"


//コンストラクタという名の初期化
Player::Player()
{
	playerX = 0 ;//配列なので　-1の数を入力せよ
	playerY = 0 ;
	mPlayerX = 0;
	mPlayerY = 0;
	//pWay = UP;
}
//デストラクタという名の解放
Player::~Player()
{

}

void Player::SetPlayer(Map &map)
{
	playerX = map.GetStartX();
	playerY = map.GetStartY();
}

void Player::movePlayer(int map)
{
	//床であるなら進める
	if (map == 0 || map == -1)
	{
		playerX += mPlayerX;
		playerY += mPlayerY;
	}
}


void Player::Drawplayer(int screenWidth, int screenHeight,Sprite *sprite,Texture &tex)
{
	sprite->SetPos(screenWidth / 2, screenHeight / 2);
	sprite->Draw(tex);
}

