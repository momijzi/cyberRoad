#include  "DirectInput.h"

DirectInput* DirectInput::pInstance =nullptr;

DirectInput::DirectInput()
{
	pDInput = nullptr;

	pKeyDevice = nullptr;
	pMouseDevice = nullptr;

	mousePos.Set(0, 0);
	mousePosDelta.Set(0, 0);
	mousePosPreview.Set(0, 0);


	//�z��ϐ��̏�����//�S�ă[��
	
	ZeroMemory(KeyInput, sizeof(int)*INPUT_BUFFER_SIZE);
	ZeroMemory(KeyInputPreview, sizeof(int)*INPUT_BUFFER_SIZE);

}

//�f�X�g���N�^
DirectInput::~DirectInput()
{
	Release();
}

HRESULT DirectInput::Init(HWND hWnd)
{
	this->hWnd = hWnd;

	if (initialized == false)
	{
		//DirectInput�I�u�W�F�N�g�̍쐬
		HRESULT hresult
			= DirectInput8Create
			(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
				IID_IDirectInput8, (VOID**)&pDInput, NULL);

		if (FAILED(hresult))
		{
			return hresult;
		}

		//�I�u�W�F�N�g���o�����̂�
		//�L�[�{�[�h�p�̃f�o�C�X��������
		hresult = InitKey();

		if (FAILED(hresult))
		{
			return hresult;
		}

		//�}�E�X�g�p�̏���
		hresult = InitMouse();
		if (FAILED(hresult))
		{
			return hresult;
		}
		//�S�Ă̏��������I��
		//�������ς݂Ƃ���
		initialized = true;
	}

	//����������
	return S_OK;
}

HRESULT DirectInput::InitKey()
{
	HRESULT hresult;

	//CreateDevice�̑������ɍ�肽���f�o�C�X�̃^�C�v������
	//��������������f�o�C�X�̕ۑ��悪����|�C���^
	//�^�C�v�ɂ�����炸�@�������̌^��
	//LPDIRECTINPUTDEVICE8*�@�ł��邱�Ƃɒ���
	hresult = pDInput
		->CreateDevice(GUID_SysKeyboard, &pKeyDevice, NULL);

	if (FAILED(hresult))
	{
		return hresult;
	}

	//�f�[�^�t�H�[�}�b�g�̐ݒ� �L�[�{�[�h��I��
	hresult = pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hresult))
	{
		return hresult;
	}

	//�������[�h�̐ݒ�@
	//OS�⑼�̃A�v���P�[�V�����Ƃ̃f�o�C�X���L�ׂ̈̐ݒ�
	//�f�t�H���g�ݒ�@
	hresult = pKeyDevice->SetCooperativeLevel
	(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

	if (FAILED(hresult))
	{
		return hresult;
	}
	
	//���̓f�o�C�X�ւ̃A�N�Z�X�����擾
	pKeyDevice->Acquire();

	return S_OK;

}

//�}�E�X�֘A�̏�����
//�}�E�X���͂��g�����߂̏���
HRESULT DirectInput::InitMouse()
{
	HRESULT hresult;

	//�}�E�X�f�o�C�X�̍쐬
	//�L�[�{�[�h�̃f�o�C�X�쐬�Ɠ����v��
	//���f�o�C�X�̃^�C�v���Ⴄ����
	hresult = pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice, NULL);

	//���s�����璆�f
	if (FAILED(hresult))return hresult;

	//�f�[�^�t�H�[�}�b�g�̐ݒ�
	hresult = pMouseDevice->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hresult))return hresult;

	//�������[�h�̐ݒ�
	hresult = pMouseDevice->SetCooperativeLevel
	(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if (FAILED(hresult))return hresult;

	//�����[�h�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;//����
											//Axis ��
											//Rel Relative ����
											//ABS absolute ���
	//�����[�h�ݒ�
	hresult = pMouseDevice->SetProperty
	(DIPROP_AXISMODE, &diprop.diph);

	if (FAILED(hresult))return hresult;

	//���̓f�o�C�X�ւ̃A�N�Z�X�����擾
	hresult = pMouseDevice->Acquire();

	if (FAILED(hresult))return hresult;

	//�}�E�X�̍��W���E�B���h�E�̒��S�ɐݒ�
	//�N���C�A���g�̈�̏㉺���E�̒[�̍��W���擾
	RECT rect;
	GetClientRect(hWnd, &rect);

	//���ƍ��������߂�
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	POINT localPos;
	//local�Ƃ����̂̓N���C�A���g�̈�̍���p��
	//��ɂ������W������

	localPos.x = width / 2;
	localPos.y = height / 2;

	//�N���C�A���g���W����X�N���[�����W�ւ̕ϊ�
	POINT worldPos = localPos;
	ClientToScreen(hWnd, &worldPos);
	//�}�E�X�̃X�^�[�g�ʒu�̕ύX
	//�X�N���[�����W�ł̎w��
	SetCursorPos(worldPos.x, worldPos.y);

	//�}�E�X�̍��W�̓N���C�A���g���W�ŊǗ�������
	//mousePos��mousePosPreview�������l�ŃX�^�[�g��������
	//��������͍D�݂̖��
	mousePos.Set(localPos.x, localPos.y);
	mousePosPreview.Set(localPos.x, localPos.y);
	mousePosDelta.Set(0,0);

	return S_OK;
}
HRESULT DirectInput::Update()
{
	//���������ς�ł��Ȃ���������s����̂�
	//��肪���낢��Əo��̂Ł@�֐��𒆒f
	if (initialized == false)
	{
		return S_FALSE;
	}

	//�L�[�{�[�h�̏�Ԃ��擾����
	//�����o�ϐ��̍X�V

	//�A�N�Z�X���擾
	HRESULT hresult = pKeyDevice->Acquire();

	//S_FALSE�ȊO�̎��s�͒ʂ��Ȃ�
	if (hresult == DI_OK || hresult == S_FALSE)
	{
		//�O��̃t���[���̃L�[����Preview�̕��ɃR�s�[
		memcpy(KeyInputPreview, 
			KeyInput, 
			sizeof(int)*INPUT_BUFFER_SIZE);

		//��ԁ@�܂��f�o�C�X���猻�݂�ON OFF�����擾
		//t temporaly �ꎞ��
		//work�@��Ɨp��
		BYTE tKeyInputWork[INPUT_BUFFER_SIZE];

		//�f�o�C�X����L�[���(on off)�̎擾
		pKeyDevice->
			GetDeviceState(sizeof(BYTE)*INPUT_BUFFER_SIZE, 
				&tKeyInputWork);

		//�L�[��OnOff����
		//���t���[�������ꑱ���Ă��邩�Ƃ������ɉ��H
		for (int i = 0; i < INPUT_BUFFER_SIZE; i++)
		{
			if (tKeyInputWork[i])//�L�[��������Ă���
			{
				if (KeyInput[i] < 2147483647)
				{
					//�����ꑱ���Ă���t���[���������Z
					KeyInput[i]++;
				}
			}
			else
			{
				//������Ă��Ȃ��ꍇ�́@
				//�A���t���[���� 0 �ɂ���
				KeyInput[i]=0;
			}
		}
	}
	//�}�E�X����
	//�A�N�Z�X���擾
	hresult = pMouseDevice->Acquire();

	if (hresult == DI_OK || hresult == S_FALSE)
	{
		//�O��̃}�E�X��Ԃ��܂���ƃR�s�[
		memcpy(&mouseInputPreview, &mouseInput, sizeof(DIMOUSESTATE2));

		pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseInput);

		for (int i = 0; i < MOUSE_BUTTON_MAX; i++)
		{
			//�}�E�X�̃{�^����������Ă��邩���肵�Ă���
			if (mouseInput.rgbButtons[i])
			{
				if (mouseButtonCounter[i] < 2147483647)
				{
					//������Ă���t���[�����̃J�E���g�A�b�v
					mouseButtonCounter[i]++;
				}
				else
				{
					//������Ă��Ȃ��΂����J�E���g���Z�b�g
					mouseButtonCounter[i] = 0;
				}
			}
		}
		//�}�E�X�̍��W�̎擾�֐���
		//Directinput�̃R�[�h�����悤���Ă��Ȃ�
		//���W�̎�舵����������̕����킩��₷������


		//�X�N���[�����̃}�E�X�̍��W
		POINT screenPos;
		//�J�[�\���ʒu���X�N���[�����W�Ŏ擾
		GetCursorPos(&screenPos);

		//�N���C�A���g���W�ɕϊ�
		POINT localPos = screenPos;
		ScreenToClient(hWnd, &localPos);

		//1�t���[���O�̍��W��ޔ�
		mousePosPreview = mousePos;
		//���݂̍��W���X�V
		mousePos.Set(localPos.x, localPos.y);

		//�ړ������������v�Z
		mousePosDelta = mousePos - mousePosPreview;
	}

	return S_OK;
}

//���݉�����Ă��邩�@�P����On�@Off
bool DirectInput::KeyState(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		//������Ă����Ԃ�0�Ȃ̂�
		//0���傫���Ɖ�����Ă��邱�Ƃ��킩��
		return KeyInput[code] > 0;
	}
	
	//�͈͊O��false
	return false;
}

//���̃t���[�������̃L�[�̉����ꂽ�u�Ԃł��邩
bool DirectInput::KeyJustPressed(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		//0����1�ɐ؂�ւ�����u�Ԃ�
		//�����ꂽ�u�Ԃ������Ă���
		return KeyInput[code] ==1;
	}

	//�͈͊O��false
	return false;
}

//���̃t���[�����L�[�̗����ꂽ�u�Ԃ�
bool DirectInput::KeyJustReleased(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		//���݂�0������
		//1�t���[���O�ł͉�����Ă���
		//���ꂪ�����ꂽ�u��
		
		return 
			(KeyInput[code] == 0 &&
			KeyInputPreview[code]>0);
	}

	//�͈͊O��false
	return false;
}

//�����ꑱ���Ă���t���[������Ԃ�
int DirectInput::KeyCount(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		return KeyInput[code] ;
	}

	//�͈͊O��0
	return 0;
}

bool DirectInput::MouseButton(int num)
{
	if (num >= 0 && num < MOUSE_BUTTON_MAX)
	{
		return mouseInput.rgbButtons[num] >= 1;
	}
	else
	{
		return false;//�z��͈͊O
	}

}
bool DirectInput::MouseButtonJustPressed(int num)
{
	if (num >= 0 && num < MOUSE_BUTTON_MAX)
	{
		return (mouseInput.rgbButtons[num] >= 1) && (mouseInputPreview.rgbButtons[num] == 0);
	}
	else
	{
		return false;//�z��͈͊O
	}

}
bool DirectInput::MouseButtonJustReleased(int num)
{
	if (num >= 0 && num < MOUSE_BUTTON_MAX)
	{
		return (mouseInput.rgbButtons[num] ==0) && (mouseInputPreview.rgbButtons[num] >=1);
	}
	else
	{
		return false;//�z��͈͊O
	}

}

int DirectInput::MouseButtonCount(int num)
{
	if (num >= 0 && num < MOUSE_BUTTON_MAX)
	{
		return mouseButtonCounter[num];
	}
	else
	{
		return 0;//�͈͊O����
	}
}

HRESULT DirectInput::Release()
{
	if (pMouseDevice)
	{
		pMouseDevice->Unacquire();
		pMouseDevice->Release();
		pMouseDevice = NULL;
	}

	if (pKeyDevice)
	{
		pKeyDevice->Unacquire();
		pKeyDevice->Release();
		pKeyDevice = NULL;
	}
	if (pDInput)
	{
			pDInput->Release();
			pDInput = NULL;
	}
	return S_OK;
}