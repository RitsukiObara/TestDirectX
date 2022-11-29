//===================================
//
//�e�w�b�_�[[Shadow.h]
//Author ��������
//
//===================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

//�e�̍\����
typedef struct
{
	D3DXVECTOR3 pos;					//�ʒu
	D3DXVECTOR3 rot;					//����
	bool bUse;							//�g�p���Ă��邩�ǂ���
}Shadow;

//�e�̎��
typedef enum
{
	SHADOW_PLAYER = 0,					//�v���C���[�̉e
	SHADOW_ENEMY,						//�G�̉e
	SHADOW_TOWER,						//���̉e
	SHADOW_BULLET,						//�e�̉e
	SHADOW_MAX
}ShadowType;

//�v���g�^�C�v�錾
void InitShadow(void);								//�e�̏���������
void UninitShadow(void);							//�e�̏I������
void UpdateShadow(void);							//�e�̍X�V����
void DrawShadow(void);								//�e�̕`�揈��
int SetShadow(int nType);							//�e�̐ݒ菈��
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//�e�̈ʒu�ݒ菈��
void ResetShadow(int *nIdxShadow);					//�e�̃��Z�b�g����

#endif