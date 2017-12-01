#pragma once

//DirectInput �f�o�b�O�t���O
#define D3D_DEBUG_INFO

//DirectInput �o�[�W����
#define DIRECT_INPUT_VERSION 0x0800

//�}�E�X�̃{�^���ɖ��O��t����
#define MOUSE_BUTTON_LEFT	0
#define MOUSE_BUTTON_RIGHT	1
#define MOUSE_BUTTON_MIDDLE 2

//�}�E�X�{�^���̍ő�͂W�{�^��
#define MOUSE_BUTTON_MAX 8

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")

#pragma comment(lib,"dinput8.lib") //directInput

#include <comdef.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>
#include <dinput.h>//directInput

#include"Vector2D.h"

#define INPUT_BUFFER_SIZE 256 //�L�[�̐��@

class DirectInput
{
	//�V���O���g���ȃN���X�ɂ���
private:
	static DirectInput* pInstance;
	//�B��̃C���X�^���X���w���|�C���^
public :

	//�B��̃C���X�^���X���w���|�C���^�̎擾
	static DirectInput*GetInstance()
	{
		if (pInstance == nullptr)
		{
			pInstance =new DirectInput();
		}

		return pInstance;
	}

private :
	//private�ȃ����o�ϐ�

	LPDIRECTINPUT8 pDInput;

	//�L�[�{�[�h�p�̃f�o�C�X
	LPDIRECTINPUTDEVICE8 pKeyDevice;
	//�}�E�X�p�̃f�o�C�X
	LPDIRECTINPUTDEVICE8 pMouseDevice;

	//�L�[�{�[�h�̏��
	//���t���[�������ꑱ���Ă��邩
	int KeyInput[INPUT_BUFFER_SIZE];

	//1�t���[���O�̃L�[�C���v�b�g�̏��
	//���͂̕ω��������� ���ɃL�[�𗣂����u�Ԃ����f�ł���
	int KeyInputPreview[INPUT_BUFFER_SIZE];

	//���݂̃}�E�X���W
	Vector2<int> mousePos;
	//�P�t���[���O�̃}�E�X���W
	Vector2<int> mousePosPreview;
	//1�t���[���O�ƌ��݂̃}�E�X���W�̍���	//���̃t���[���ňړ���������
	Vector2<int> mousePosDelta;

	//�}�E�X�̃{�^���̏��
	DIMOUSESTATE2 mouseInput;
	//�P�t���[���O�̃}�E�X�{�^���̏��
	DIMOUSESTATE2 mouseInputPreview;
	//�}�E�X�̃{�^�������t���[�������ꑱ���Ă��邩
	int mouseButtonCounter[MOUSE_BUTTON_MAX];//�ő�W��������

	//�E�B���h�E�̃n���h��
	HWND hWnd;
	
	bool initialized;


public :
	//public�ȃ����o�֐�

private:
	//private�ȃ����o�֐�

	//�V���O���g���ȃN���X�ɂ���̂�
	//��ȏ�̃C���X�^���X����������悤�ȏ�����
	//private�ɂ��Č��J���Ȃ�
	DirectInput();
	~DirectInput();
	DirectInput(DirectInput& refDi) {};
	DirectInput& operator=(DirectInput& refDi) {};

	
	HRESULT InitKey();//�L�[�{�[�h�̏���
	HRESULT InitMouse();//�}�E�X�̏���

public :
	//public�ȃ����o�֐�
	
	//�������̏����@InitKey��InitMouse���̎��s
	HRESULT Init(HWND hWnd);

	//�������
	HRESULT Release();

	//���͏�Ԃ̍X�V
	HRESULT Update();

	//key�̏�Ԃ̎擾
	bool KeyState(int code);//on off�̏��
	bool KeyJustPressed(int code);//���܉����ꂽ�u�Ԃ�
	bool KeyJustReleased(int code);//���ܗ����ꂽ�u�Ԃ�
	int KeyCount(int code);//�����ꑱ���Ă���t���[����

	//�}�E�X�̏�Ԃ��擾
	bool MouseButton(int num);//������Ă��邩�@on off�@�̏��
	bool MouseButtonJustPressed(int num);//�����ꂽ�u�Ԃ�
	bool MouseButtonJustReleased(int num);//�����ꂽ�u�Ԃ�
	int MouseButtonCount(int num);//�����ꑱ���Ă���t���[����

	//���݂̍��W
	Vector2<int> MousePosition() { return mousePos; }
	//�O�t���[������̈ړ������i�����j
	Vector2<int> MousePositionDelta() { return mousePosDelta; }


};