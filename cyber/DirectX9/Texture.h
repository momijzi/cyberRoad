#pragma once

#include "Direct3D.h"

//�e�N�X�`���Ƃ��ă��[�h�����摜��
//�ێ�����N���X

class Texture
{
	//Direct3D�N���X�̊֐������
	//�A�N�Z�X�w��q�𖳎�����
	//private�ȃ����o�ɃA�N�Z�X�ł���悤�ɂ���
	friend class Direct3D;
	//	pTexture��Direct3D���ŎQ�Ƃ�����

private :

	//���[�h�����摜�f�[�^��ێ�����ϐ�
	//I �C���^�[�t�F�[�X�Ƃ����^�C�v�̃N���X
	//  ���͓��ɋC�ɂ��Ȃ���OK
	IDirect3DTexture9* pTexture;

	unsigned int divU;//x�������̕�����
	unsigned int divV;//y�������̕�����

	unsigned int numU;	//���ݎw�肳��Ă���
						//���������W
						//divU�ȏ�ɂ͂Ȃ�Ȃ�

	unsigned int numV;	//���ݎw�肳��Ă���
						//�c�������W
						//divV�ȏ�ɂ͂Ȃ�Ȃ�

public :
	Texture();
	~Texture();

	//��������@�K�{�@���[�h�����摜���A�����[�h
	void Release();

	//���[�h����֐�
	bool Load(TCHAR* FilePath);

	//�����̐ݒ�(���E�㉺�����ɓ������邩)
	void SetDivide(unsigned int DivU,
					unsigned int DivV);

	//�������̉��ڂ�I�����Ă��邩
	void SetNum(unsigned int NumU,
		unsigned int NumV);

};
