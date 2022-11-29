//===================================
//
//�o���l�w�b�_�[[exp.h]
//Author ��������
//
//===================================
#ifndef _EXP_H_
#define _EXP_H_

#include"main.h"

//�e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;					//�ʒu
	D3DXVECTOR3 rot;					//����
	D3DXVECTOR3 move;					//�ړ���
	D3DXMATRIX mtx;						//���[���h�}�g���b�N�X
	float fRadius;						//���a
	int nLife;							//����
	bool bUse;							//�g�p���Ă��邩�ǂ���
}Exp;

//�v���g�^�C�v�錾
void InitExp(void);						//�o���l�̏���������
void UninitExp(void);					//�o���l�̏I������
void UpdateExp(void);					//�o���l�̍X�V����
void DrawExp(void);						//�o���l�̕`�揈��
void SetExp(D3DXVECTOR3 pos);			//�o���l�̐ݒ菈��

#endif