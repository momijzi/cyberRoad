#include "Texture.h"

//�R���X�g���N�^
Texture::Texture()
{
	pTexture = nullptr;

	//�������͍Œ�1
	divU = divV = 1;

	//�������̉��ڂ���\�����l��
	//0���番����-1�܂łƂ���
	numU = numV = 0;
}

//�f�X�g���N�^
Texture::~Texture()
{
	Release();
}

//�������
void Texture::Release()
{
	if (pTexture != nullptr)
	{
		//IDirect3DTexture9��
		//��p�̕ϐ��Ń��������m�ۂ���
		//Direct3D���ŊǗ����Ă���
		
		//�J������ۂ���p�̊֐������s����
		pTexture->Release();

		pTexture = nullptr;	
				
		divU = divV = 1;		
		numU = numV = 0;
	}
}

bool Texture::Load(TCHAR* FilePath)
{
	Direct3D &d3d = Direct3D::GetInstance();

	//Load�̏�����Direct3D�̃f�o�C�X��
	//�K�v�Ȃ̂� Direct3D���Ƀ��[�h�����̖{�̂�
	//�L�q���Ă���

	//���[�h�̎��s�ƒl�̃��^�[��
	return d3d.LoadTexture((*this), FilePath);
	
}

//�����̐ݒ�(���E�㉺�����ɓ������邩)
void Texture::SetDivide(unsigned int DivU,
	unsigned int DivV)
{
	//��������1�ȏ�
	divU =(DivU != 0)? DivU : 1;
	divV =(DivV != 0)? DivV : 1;

	//���ݐݒ肳��Ă���
	//numU�� �V�����ݒ肵��divU�𒴂��Ă��܂���
	//�ꍇ�̑Ώ�
	numU = (numU >= divU) ? divU - 1 : numU;
	numV = (numV >= divV) ? divV - 1 : numV;
}

//�������̉��ڂ�I�����Ă��邩
void Texture::SetNum(unsigned int NumU,
	unsigned int NumV)
{
	numU = (numU >= divU) ? divU-1 : NumU;
	numV = (numV >= divV) ? divV-1 : NumV;
}