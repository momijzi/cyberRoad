#pragma once

#include"Texture.h"
#include"Sprite.h"

class Map;

class Player
{
private:

public:
	
	Player();
	~Player();

	//�v���C���[�̌��ݍ��W
	int playerX, playerY;

	//�v���C���[�̌��݌����Ă������
	//enum PlayerWay{UP,DOWN,LEFT,RIGHT};
	//PlayerWay pWay;

	//�v���C���[�̈ړ�������������߂邽�߂̕ϐ�
	int mPlayerX, mPlayerY;

	//�v���C���[�ړ�
	void movePlayer(int map);

	void SetPlayer(Map &map);
	//�v���C���[�`��
	void Drawplayer(int screenWidth, int screenHeight,Sprite *sprite,Texture &tex);

};