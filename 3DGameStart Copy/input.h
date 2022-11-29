//=======================================
//
//���̓w�b�_�[[input.h]
//Author ��������
//
//=======================================
#ifndef _IMPUT_H_//���̃}�N����`������Ă��Ȃ�������
#define _INPUT_H_//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include<Xinput.h>

#pragma comment(lib,"xinput.lib")

//�Q�[���p�b�h�̃{�^��
typedef enum
{
	BUTTON_UP = 0,			//��{�^��
	BUTTON_DOWN,			//���{�^��
	BUTTON_LEFT,			//���{�^��
	BUTTON_RIGHT,			//�E�{�^��
	BUTTON_START,			//START�{�^��
	BUTTON_BACK,			//BACK�{�^��
	BUTTON_LEFTSTICK,		//���̃X�e�B�b�N��������
	BUTTON_RIGHTSTICK,		//�E�̃X�e�B�b�N��������
	BUTTON_LB,				//LB�{�^��
	BUTTON_RB,				//RB�{�^��
	BUTTON_PPP1,			//�s��
	BUTTON_PPP2,			//�s��
	BUTTON_A,				//A�{�^��
	BUTTON_B,				//B�{�^��
	BUTTON_X,				//X�{�^��
	BUTTON_Y,				//Y�{�^��
	BUTTON_STICKLX,			//L�X�e�B�b�N(X��)(�E�ɍs���قǒl�͑傫��)
	BUTTON_STICKLY,			//L�X�e�B�b�N(Y��)(��ɍs���قǒl�͑傫��)
	BUTTON_PPP3,			//�s��
	BUTTON_PPP4,			//�s��
	BUTTON_MAX
}JOYKEY;

//�v���g�^�C�v�錾
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);			//���͂̏���������
void UninitKeyboard(void);										//���͂̏I������
void UpdateKeyboard(void);										//���͂̍X�V����
bool GetKeyboardPress(int nKey);								//���͂̕`�揈��

//�L�[�{�[�h����
bool GetKeyboardTrigger(int nKey);								//���͂̃g���K�[��񏈗�
bool GetKeyboardRelease(int nKey);								//���͂̃����[�X��񏈗�
bool GetKeyboardRepeat(int nKey);								//���͂̃��s�[�g��񏈗�

//�Q�[���p�b�h�p
//�v���g�^�C�v�錾
HRESULT InitGamePad(void);										//�Q�[���p�b�h���͂̏���������
void UninitGamePad(void);										//�Q�[���p�b�h���͂̏I������
void UpdateGamePad(void);										//�Q�[���p�b�h���͂̍X�V����
bool GetGamePadPress(JOYKEY nKey, int nPlayer);					//�Q�[���p�b�h���͂̃v���X��񏈗�
bool GetGamePadTrigger(JOYKEY nKey, int nPlayer);				//�Q�[���p�b�h���͂̃g���K�[��񏈗�
SHORT GetGameStickLXPress(JOYKEY nKey, int nPlayer);			//L�X�e�B�b�N(X��)���͂̃v���X��񏈗�
SHORT GetGameStickLYPress(JOYKEY nKey, int nPlayer);			//L�X�e�B�b�N(Y��)���͂̃v���X��񏈗�
SHORT GetGameStickLXTrigger(JOYKEY nKey, int nPlayer);			//L�X�e�B�b�N(X��)���͂̃v���X��񏈗�
SHORT GetGameStickLYTrigger(JOYKEY nKey, int nPlayer);			//L�X�e�B�b�N(Y��)���͂̃v���X��񏈗�


#endif