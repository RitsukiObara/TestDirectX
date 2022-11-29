//=============================================================================
//
// �T�E���h���� [sound.h]
// Author ��������
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�ꗗ
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_GAME,		//�Q�[��BGM
	SOUND_LABEL_SE_CLEAR,		//�N���A��
	SOUND_LABEL_BGM_TITLE,		//�I�[�v�j���OBGM
	SOUND_LABEL_SE_SELECT,		//���艹
	SOUND_LABEL_BGM_TUTORIAL,	//�`���[�g���A��BGM
	SOUND_LABEL_BGM_RESULT,		//���U���gBGM
	SOUND_LABEL_SE_CHOICE,		//�I����
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
HRESULT ReplaySound(SOUND_LABEL label);

#endif
