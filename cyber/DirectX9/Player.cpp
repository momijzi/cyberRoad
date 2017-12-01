#include"Player.h"
#include "Map.h"


//�R���X�g���N�^�Ƃ������̏�����
Player::Player()
{
	playerX = 0 ;//�z��Ȃ̂Ł@-1�̐�����͂���
	playerY = 0 ;
	mPlayerX = 0;
	mPlayerY = 0;
	//pWay = UP;
}
//�f�X�g���N�^�Ƃ������̉��
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
	//���ł���Ȃ�i�߂�
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

