#include "Direct3D.h"

#include "Sprite.h"
#include "Texture.h"

//�X�^�e�B�b�N�ȃ����o�ϐ��̏�����
Direct3D* Direct3D::pInstance = nullptr;


//�R���X�g���N�^
Direct3D::Direct3D()
{
	//�|�C���^�ɒl�������邾��
	//�܂��ǂ̃I�u�W�F�N�g���w���Ȃ�
	pD3D9 = NULL;
	pDevice3D = NULL;
}

//�f�X�g���N�^
Direct3D::~Direct3D()
{
	Release();
}

//����̊֐�
void Direct3D::Release()
{
	if (pDevice3D != NULL)
	{
		pDevice3D->Release();
		pD3D9->Release();

		pDevice3D = NULL;
		pD3D9 = NULL;
	}
}

//�f�o�C�X�̍쐬�����݂�
bool Direct3D::TryCreate(HWND hWnd)
{
	if (pDevice3D != nullptr)
	{
		//���łɍ쐬����Ă���̂Ŏ��s
		return false;
	}
	
	//�쐬�̊֐����Ă�
	if (!Create(hWnd))
	{
		return false;
	}

	//�쐬����
	return true;
}

//�f�o�C�X�̍쐬
//TryCreate�֐�����Ă�
bool Direct3D::Create(HWND hWnd)
{
	//�o�b�N�o�b�t�@(�����)�̂����������v�Z
	//�E�B���h�E�̃N���C�A���g�̈�̂��������ɍ��킹��

	//�N���C�A���g�̈�̂��������𒲂ׂ�
	RECT rect;
	GetClientRect(hWnd, &rect);

	//�o�b�N�o�b�t�@�̕��ƍ���
	int width  = rect.right  - rect.left;
	int height = rect.bottom - rect.top;

	//Direct3D9�I�u�W�F�N�g�̍쐬
	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);

	//�f�B�X�v���C�����擾
	D3DDISPLAYMODE Display;
	pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display);

	//�X���b�v�`�F�C���ׂ̈̐ݒ�
	//�X���b�v�`�F�C���@
	//�X�N���[���̃o�b�N�o�b�t�@��
	//�t�����g�o�b�t�@�ɒu�������鏈���̂���

	D3DPRESENT_PARAMETERS D3DParam;
	D3DParam.BackBufferWidth = width;				//�o�b�N�o�b�t�@�̕��@�N���C�A���g�̈�Ɠ���
	D3DParam.BackBufferHeight = height;				//�o�b�N�o�b�t�@�̍���
	D3DParam.BackBufferFormat = Display.Format;		//�o�b�N�o�b�t�@�̃t�H�[�}�b�g
													//�f�B�X�v���C�����
	D3DParam.BackBufferCount = 1;					//�o�b�N�o�b�t�@�̐�
	D3DParam.MultiSampleType = D3DMULTISAMPLE_NONE;	//�}���`�T���v�����O�̐ݒ�
													//�Ȃ�
	D3DParam.MultiSampleQuality = 0;				//�}���`�T���v�����O�̃N�I���e�B
	D3DParam.SwapEffect = D3DSWAPEFFECT_DISCARD;	//�X���b�v�`�F�C���̕��@
													//Direct3D�C��
	D3DParam.hDeviceWindow = hWnd;					//�`�悷��E�B���h�E�̎��ʃn���h��
	D3DParam.Windowed = TRUE;						//�E�B���h�E���[�h
	D3DParam.EnableAutoDepthStencil = TRUE;			//�[�x�X�e���V���o�b�t�@�@Z�o�b�t�@
													//�`��̍ہ@���łɎ�O�̂��̂�������Ă����
													//���̂��̂̕`����Ȃ����߂̏��@
													//�X�N���[���ɐF�ł͂Ȃ����̃s�N�Z���ɏ����ꂽ���̂�
													//�J��������̋����������Ă���ƃC���[�W����Ɨǂ�
	D3DParam.AutoDepthStencilFormat = D3DFMT_D24S8;	//�[�x�X�e���V���̃t�H�[�}�b�g
	D3DParam.Flags = 0;								//�ݒ�̃t���O
	D3DParam.FullScreen_RefreshRateInHz = 0;		//�E�B���h�E���[�h�͎g�p���Ȃ��̂�0

	D3DParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	//�A�_�v�^�̃��t���b�V�����[�g��present���������s���郌�[�g�̊֌W

	//�������̐ݒ�Ńf�o�C�X�쐬�����݂�
	//HAL���[�h�ō쐬
	if (FAILED(pD3D9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		D3DParam.hDeviceWindow,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&D3DParam, &pDevice3D)
	))
	{
		//1�ڂ̐ݒ�Ŏ��s������
		if (FAILED(pD3D9->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			D3DParam.hDeviceWindow,
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
			//��ڂ̐ݒ肩�� HARDWARE -> MIXED
			&D3DParam, &pDevice3D)
		))
		{
			//2�ڂ̐ݒ�Ŏ��s������
			if (FAILED(pD3D9->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,		//��ڂ̐ݒ肩�� HAL -> REF
				D3DParam.hDeviceWindow,
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
				&D3DParam, &pDevice3D)
			))
			{
				//3�ڂ̐ݒ�Ŏ��s������
				if (FAILED(pD3D9->CreateDevice(
					D3DADAPTER_DEFAULT,
					D3DDEVTYPE_REF,
					D3DParam.hDeviceWindow,
					D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
					//3�ڂ̐ݒ肩�� HARDWARE -> MIXED
					&D3DParam, &pDevice3D)
				))
				{
					//4�S�Ă̐ݒ�ō쐬�����s������

					//�������������
					//���s�����Ŋ֐����I����
					pD3D9->Release();
					pD3D9 = NULL;
					pDevice3D = NULL;

					return false;
				}
			}
		}
	}

	if (FAILED(InitFont(hWnd))) { return false; }

	this->hWnd = hWnd;

	//�����ɗ����Ƃ������Ƃ͂ǂꂩ�̐ݒ�Ńf�o�C�X�̍쐬����������
	return true;

}

HRESULT Direct3D::InitFont(HWND hWnd)
{
	//�t�H���g�ݒ�(�f�o�C�X�A�����A���A�t�H���g�X�^�C���A�~�j�}�b�v�̃��x���A�ΆA�����Z�b�g�A�o�͐��x�A�o�͕i���A�t�H���g�s�b�`�ƃt�@�~���A�t�H���g���A�t�H���g�|�C���^)
	HRESULT hr = D3DXCreateFont(pDevice3D, 20, 0, FW_NORMAL, 1, false, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "�l�r �S�V�b�N", &pFont);
	return hr;
}

void Direct3D::DrawString(std::string *str, RECT rect, D3DXCOLOR col)
{
	if (pDevice3D == NULL) { return; }

	pFont->DrawText(NULL, _T(str->c_str()), -1, &rect, DT_LEFT | DT_NOCLIP, col);

}

//�`��̐ݒ�
void Direct3D::SetRenderState(RENDERSTATE state)
{
	//�f�o�C�X���쐬����Ă��Ȃ��Ȃ璆�f
	if (pDevice3D == nullptr)return ;

	switch (state)
	{
		case RENDER_DEFAULT:
			pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, false);//���e�X�g����
			pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, false);//���u�����h����

			break;
		case RENDER_ALPHATEST:
			//���������̐؂蔲��
			//���l�ł��̃s�N�Z����`�悷�邩�𔻒f����
			//���f��̃��l�͂��炩���ߌ��߂Ă���
			pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, true);//���e�X�g�L��
			pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, false);//���u�����h����
			pDevice3D->SetRenderState(D3DRS_ALPHAREF, 0x80);//���f��l
			pDevice3D->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);//��l�ȏ�ŕ`��

			break;
		case RENDER_ALPHABLEND:
			//�������`��

			pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);//���e�X�g����
			pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//���u�����h�L��

			//�u�����h�W���@������`�悷��s�N�Z���F�ɑ΂������
			pDevice3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		
			//�u�����h�W��	���Ƀo�b�N�o�b�t�@�ɏ�����Ă���s�N�Z���F�ɑ΂������
			pDevice3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	
			
			pDevice3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			pDevice3D->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			pDevice3D->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			pDevice3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			pDevice3D->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			pDevice3D->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			//�ŏI�I�ȕ`��F�@���@
			//	SRC�F�~�u�����f�B���O�W���iSRC)�@�{�@DEST�F�~�u�����f�B���O�W���iDEST�j

			break;	
	}


}

HRESULT Direct3D::BeginScene()
{	
	if (pDevice3D != nullptr)
	{
		//Direct3D�f�o�C�X��BegineScene�����s
		return pDevice3D->BeginScene();
	}
	else
	{
		return S_FALSE;
	}
}

HRESULT Direct3D::EndScene()
{
	if (pDevice3D != nullptr)
	{
		//Direct3D�f�o�C�X��EndScene�����s
		return pDevice3D->EndScene();
	}
	else
	{
		return S_FALSE;
	}
}

//�o�b�N�o�b�t�@�̃N���A
HRESULT  Direct3D::ClearScreen()
{
	if (pDevice3D != nullptr)
	{
		//�w�i�̃N���A�F
		//�����`�悳��Ă��Ȃ���Ԃ��Ƃ��̐F����ʂɏo��
		D3DXCOLOR Color = D3DCOLOR_XRGB(255, 255, 255);

		//Clear�̎��s���ʂ�Ԃ�
		return pDevice3D->Clear(0, NULL,
			//�N���A�̃^�[�Q�b�g�ƂȂ�o�b�t�@���w��
			D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER
			, Color, 1.0f, 0);
	}
	else
	{
		return S_FALSE;
	}
}

//�o�b�N�o�b�t�@���t�����g�ɔ��f
HRESULT Direct3D::Present()
{
	if (pDevice3D != nullptr)
	{
		return pDevice3D->Present(NULL, NULL, NULL, NULL);
	}
	else
	{
		return S_FALSE;
	}
}

void Direct3D::DrawSprite(Sprite& sprite,Texture & tex)
{
	//�f�o�C�X���쐬����Ă��Ȃ����
	//�`��̏����ɓ��炸return����
	if (pDevice3D == nullptr)return;

	//�`��Ɏg�p����ϐ���sprite������o��
	D3DXVECTOR2 pos = sprite.GetCenterPos();
	float width = sprite.GetWidth();
	float height = sprite.GetHeight();
	float alpha = sprite.GetAlpha();
	float angle = sprite.GetAngle_Rad();
	
	char alphaC = static_cast<char>(255 * alpha);

	float u = tex.numU;
	float v = tex.numV;
	float du = tex.divU;
	float dv = tex.divV;

	//�l�p�`�Ȃ̂Œ��_4��
	//�g���C�A���O���X�g���b�v���g�p����̂�
	//vertex�Ɋi�[���鏇�Ԃ� �E��@�E���@����@����
	SpriteVertex vertex[4]=
	{
		//			�@x   y   z	 rhw	���_�F		�ԁ@�΁@�@�����@u	  v
		{ D3DXVECTOR3( width / 2,-height/2,0),1.0f,D3DCOLOR_RGBA(255,255,255,alphaC),(u+1)/du,		v/dv },
		{ D3DXVECTOR3( width / 2, height/2,0),1.0f,D3DCOLOR_RGBA(255,255,255,alphaC),(u+1)/du,	(v+1)/dv },
		{ D3DXVECTOR3(-width / 2,-height/2,0),1.0f,D3DCOLOR_RGBA(255,255,255,alphaC),	 u/du,		v/dv },
		{ D3DXVECTOR3(-width / 2, height/2,0),1.0f,D3DCOLOR_RGBA(255,255,255,alphaC),	 u/du,	(v+1)/dv }
	};


	
	
	for (int i = 0; i < 4; i++)
	{
		//�O�p�֐��@���@�藝���g�p����
		//4�̒��_�̍��W��ϊ�
		//���_�𒆐S�ɂ��� angle�̕�������
		float x = vertex[i].pos.x*cosf(angle) -
			vertex[i].pos.y*sinf(angle);

		float y = vertex[i].pos.x*sinf(angle) +
			vertex[i].pos.y*cosf(angle);

		//���S���W���������s�ړ�
		D3DXVECTOR3 vPos;
		vPos.x =pos.x+ x;
		vPos.y =pos.y+ y;
		vPos.z = 0;
		vertex[i].pos = vPos;
	}

	//�e�N�X�`���̎w��
	pDevice3D->SetTexture(0,tex.pTexture);

	//���_�\���̐錾
	pDevice3D->SetFVF(SPRITE_FVF);

	//�|���S���̕`��
	//�g���C�A���O���X�g���b�v�ŕ`��

	//���� (�`��^�C�v,
		//�O�p�`�̐�,
		//���_�\���̔z��̐擪�A�h���X,
		//�ЂƂ̒��_�\���̂̃T�C�Y);
	pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, 
		vertex, sizeof(SpriteVertex));

}

//�e�N�X�`���̃��[�h
bool Direct3D::LoadTexture(Texture& tex, TCHAR* FilePath)
{
	//�f�o�C�X���쐬����Ă��邩�ǂ����̊m�F
	if (pDevice3D != nullptr)
	{
		if (SUCCEEDED(
			D3DXCreateTextureFromFile(
				pDevice3D, FilePath, &(tex.pTexture)
			)//createFromFile
			)//succeeded
			)//if
		{
			//���[�h�ɐ�������
			return true;
		}
	}

	//�f�o�C�X�����݂��Ȃ�����
	//���݂������쐬�Ɏ��s����
	return false;//���s
}