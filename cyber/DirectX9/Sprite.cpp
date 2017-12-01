#include "Sprite.h"

Sprite::Sprite()
{
	//�C���X�^���X�����ꂽ���̏����l
	pos.x = pos.y = 0.0f;
	width = height = 0.0f;

	angle = 0.0f;

	alpha = 1.0f;
}

Sprite::~Sprite()
{
	//���ɕK�v�ȏ����Ȃ�
}

//���S���W�̕ύX
void Sprite::SetPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
}

//���ƍ����̐ݒ�
void Sprite::SetSize(float width, float height)
{
	this->width  = width;
	this->height = height;
}

//��]�̐ݒ�
void Sprite::SetAngle(float rad)
{
	angle = rad;
}

//�����x�̐ݒ�
void Sprite::SetAlpha(float a)
{
	if (a <= 0.0f) 
	{
		alpha = 0.0f;
	}
	else if (a >= 1.0f)
	{
		alpha = 1;
	}
	else
	{
		alpha = a;
	}
}

//�`��
void Sprite::Draw(Texture& tex)
{
	//Direct3D�̃C���X�^���X���擾
	Direct3D& d3d =
		Direct3D::GetInstance();

	//this =�@�����ւ̃|�C���^
	
	//�@(*�|�C���^)�Ȃ̂ŕϐ��{�̂�����
	//�Q�ƂƂ��Ċ֐��ɓn��
	d3d.DrawSprite(*this,tex);
}

