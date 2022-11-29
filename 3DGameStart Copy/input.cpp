//=========================================
//
//���͂̃��C������[input.cpp]
//Author ��������
//
//=========================================
#include "input.h"

//�}�N����`
#define NUM_KEY_MAX			(256)			//�L�[�̍ő吔
#define MAX_PLAYER			(4)				//�v���C�o����l��

//�O���[�o���ϐ��錾
LPDIRECTINPUT8 g_pInput = NULL;				//DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;	//���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^

											//�L�[�{�[�h�p
BYTE g_aKeyState[NUM_KEY_MAX];				//�L�[�{�[�h�̃v���X���
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];		//�L�[�{�[�h�̃g���K�[���
BYTE g_aKeyStateRelease[NUM_KEY_MAX];		//�L�[�{�[�h�̃����[�X���
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];		//�L�[�{�[�h�̃��s�[�g���

											//�Q�[���p�b�h�p
XINPUT_STATE g_aGamePadState[NUM_KEY_MAX];			//�Q�[���p�b�h�̃v���X���
XINPUT_STATE g_aGamePadStateTrigger[NUM_KEY_MAX];	//�Q�[���p�b�h�̃g���K�[���

//==================================
//�L�[�{�[�h�̏���������
//==================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	g_pDevKeyboard->Acquire();
	return S_OK;
}

//==================================
//�L�[�{�[�h�̏I������
//==================================
void UninitKeyboard(void)
{
	//���̓f�o�C�X(�L�[�{�[�h)�̔j��
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();					//�L�[�{�[�h�ւ̃A�N�Z�X�������

		g_pDevKeyboard->Release();

		g_pDevKeyboard = NULL;
	}

	//DirectInput�I�u�W�F�N�g�̕���
	if (g_pInput != NULL)
	{
		g_pInput->Release();

		g_pInput = NULL;
	}
}

//=====================================
//�L�[�{�[�h�̍X�V����
//=====================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];						//�L�[�{�[�h�̓��͏��
	int nCntKey;

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		//�L�[�{�[�h�̃g���K�[����ۑ�

			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];	//�L�[�{�[�h�̃����[�X����ۑ�

			g_aKeyState[nCntKey] = aKeyState[nCntKey];	//�L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();						//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//======================================
//�L�[�{�[�h�̃v���X�����l��
//======================================
bool GetKeyboardPress(int nKey)
{
	return(g_aKeyState[nKey] & 0x80) ? true : false;
}

//======================================
//�L�[�{�[�h�̃g���K�[�����l��
//======================================
bool GetKeyboardTrigger(int nKey)
{
	return(g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//======================================
//�L�[�{�[�h�̃����[�X�����l��
//======================================
bool GetKeyboardRelease(int nKey)
{
	return(g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//======================================
//�L�[�{�[�h�̃��s�[�g�����l��
//======================================
bool GetKeyboardRepeat(int nKey)
{
	return(g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//======================================
//�Q�[���p�b�h�̏���������
//======================================
HRESULT InitGamePad(void)
{
	int nGPCount;					//�񐔂̕ϐ���錾����

									//�Q�[���p�b�h�ݒ��L���ɂ���
	XInputEnable(true);

	for (nGPCount = 0; nGPCount < MAX_PLAYER; nGPCount++)
	{//�������̏�����
	 //�Q�[���p�b�h�̃v���X����p�ӂ���
		memset(&g_aGamePadState[nGPCount], 0, sizeof(XINPUT_STATE));

		//�Q�[���p�b�h�̃g���K�[����p�ӂ���
		memset(&g_aGamePadStateTrigger[nGPCount], 0, sizeof(XINPUT_STATE));
	}

	return S_OK;
}

//======================================
//�Q�[���p�b�h�̏I������
//======================================
void UninitGamePad(void)
{
	//�Q�[���p�b�h�ݒ�𖳌��ɂ���
	XInputEnable(false);
}

//======================================
//�Q�[���p�b�h�̍X�V����
//======================================
void UpdateGamePad(void)
{
	XINPUT_STATE aGamePadState[NUM_KEY_MAX];						//�Q�[���p�b�h�̓��͏��
	int nCntGP;

	for (nCntGP = 0; nCntGP < MAX_PLAYER; nCntGP++)
	{//���ꂼ��̃Q�[���p�b�h���ݒ肷��
	 //���̓f�o�C�X����f�[�^���擾
		if (XInputGetState(nCntGP, &aGamePadState[nCntGP]) == ERROR_SUCCESS)
		{
			g_aGamePadStateTrigger[nCntGP].Gamepad.wButtons =
				~g_aGamePadState[nCntGP].Gamepad.wButtons & aGamePadState[nCntGP].Gamepad.wButtons
				;		//�Q�[���p�b�h�̃g���K�[����ۑ�

						//g_aKeyStateRelease[nCntKey] = (g_aGamePadState[nCntKey] ^ aGamePadState[nCntKey]) & ~aGamePadState[nCntKey];		//�L�[�{�[�h�̃����[�X����ۑ�

			g_aGamePadState[nCntGP] = aGamePadState[nCntGP];	//�Q�[���p�b�h�̃v���X����ۑ�
		}
	}
}

//======================================
//�Q�[���p�b�h�̃v���X���͏���
//======================================
bool GetGamePadPress(JOYKEY nKey, int nPlayer)
{
	return(g_aGamePadState[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//======================================
//�Q�[���p�b�h�̃g���K�[���͏���
//======================================
bool GetGamePadTrigger(JOYKEY nKey, int nPlayer)
{
	return(g_aGamePadStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//======================================
//L�X�e�B�b�N(X��)���͂̃v���X��񏈗�
//======================================
SHORT GetGameStickLXPress(JOYKEY nKey, int nPlayer)
{
	return g_aGamePadState[nPlayer].Gamepad.sThumbLX;
}

//======================================
//L�X�e�B�b�N(Y��)���͂̃v���X��񏈗�
//======================================
SHORT GetGameStickLYPress(JOYKEY nKey, int nPlayer)
{
	return g_aGamePadState[nPlayer].Gamepad.sThumbLY;
}

//======================================
//L�X�e�B�b�N(X��)���͂̃g���K�[��񏈗�
//======================================
SHORT GetGameStickLXTrigger(JOYKEY nKey, int nPlayer)
{
	return g_aGamePadStateTrigger[nPlayer].Gamepad.sThumbLX;
}

//======================================
//L�X�e�B�b�N(Y��)���͂̃g���K�[��񏈗�
//======================================
SHORT GetGameStickLYTrigger(JOYKEY nKey, int nPlayer)
{
	return g_aGamePadStateTrigger[nPlayer].Gamepad.sThumbLY;
}
