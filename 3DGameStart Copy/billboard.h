//===================================
//
//�r���{�[�h�w�b�_�[[billboard.h]
//Author ��������
//
//===================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"

//�r���{�[�h�̍\����
typedef struct
{
	D3DXVECTOR3 pos;								//�ʒu
	D3DXVECTOR3 rot;								//����(���˂̕����ݒ�ɕK�v)
	D3DXMATRIX mtx;									//���[���h�}�g���b�N�X
	int Type;										//���
	int nLife;										//����
	int nAnimCounter;								//�A�j���J�E���^�[
	int nAnimPattern;								//�A�j���p�^�[��No.
	int nShadow;									//�e�̃C���f�b�N�X(�e�̔ԍ�)
	bool bUse;										//�g�p���Ă��邩�ǂ���
}Billboard;

//�r���{�[�h�̎��
typedef enum
{
	BILLBOARD_EXPL = 0,								//����
	BILLBOARD_BULLET,								//�e
	BILLBOARD_MAX
}Billtype;

//�v���g�^�C�v�錾
void InitBillboard(void);							//�r���{�[�h�̏���������
void UninitBillboard(void);							//�r���{�[�h�̏I������
void UpdateBillboard(void);							//�r���{�[�h�̍X�V����
void DrawBillboard(void);							//�r���{�[�h�̕`�揈��
void SetBillboard(D3DXVECTOR3 rot, D3DXVECTOR3 pos, int nType);	//�r���{�[�h�̐ݒ菈��

#endif