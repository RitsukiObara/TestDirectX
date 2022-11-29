//===================================
//
//�G�t�F�N�g�w�b�_�[[Effect.h]
//Author ��������
//
//===================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include"main.h"

//�G�t�F�N�g�̎��
typedef enum
{
	EFFECT_BULLET = 0,					//�e�̃G�t�F�N�g
	EFFECT_EXPL,						//�����̃G�t�F�N�g
	EFFECT_WALK,						//�����Ă��鎞�̏���
	EFFECT_MAX
}EffectType;

//�v���g�^�C�v�錾
void InitEffect(void);										//�G�t�F�N�g�̏���������
void UninitEffect(void);									//�G�t�F�N�g�̏I������
void UpdateEffect(void);									//�G�t�F�N�g�̍X�V����
void DrawEffect(void);										//�G�t�F�N�g�̕`�揈��
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRadius, int nLife, int nType);					//�G�t�F�N�g�̐ݒ菈��

#endif