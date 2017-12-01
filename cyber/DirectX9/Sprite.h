#pragma once

#include "Direct3D.h"

class Texture;

class Sprite
{
	//Direct3D����̓����o�ɃA�N�Z�X�\�ɂ���
	friend class Direct3D;

private:
	D3DXVECTOR2 pos;//�X�v���C�g�̒��S���W
	float width;//�X�v���C�g�̕�
	float height;//�X�v���C�g�̍���

	float angle;//��]�̒l�@�ʓx�@�i���W�A���j

	float alpha;//���l

public:
	Sprite();
	virtual ~Sprite();

	void SetPos(float x, float y);//���W�̐ݒ�

	void SetSize(float width, float height);//���ƍ����̐ݒ�

	void SetAngle(float rad);//��]�̒l��ݒ�

	void SetAlpha(float alpha);//�����x�̐ݒ�

	void Draw(Texture& tex);//�`��

	//�X�v���C�g�̒��S���W�̎擾
	D3DXVECTOR2 GetCenterPos() 
	{ return pos; }

	//���̎擾
	float GetWidth()
	{ return width; }

	//�����̎擾
	float GetHeight()
	{ return height;}

	//��]�̎擾
	float GetAngle_Rad()
	{ return angle;}

	//�����x
	float GetAlpha()
	{ return alpha; }

};