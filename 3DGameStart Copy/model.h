//===================================
//
//���f���w�b�_�[[model.h]
//Author ��������
//
//===================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//���f���̍\����
typedef struct
{
	D3DXVECTOR3 pos;								//���݂̈ʒu
	D3DXVECTOR3 posOld;								//�O��̈ʒu
	D3DXVECTOR3 move;								//�ړ���
	D3DXVECTOR3 rot;								//����
	float fRotDest;									//�ڕW�̌���
	float fRotDiff;									//�����̍���
	bool bUse;										//�g�p���Ă��邩�ǂ���
	bool bMove;										//�ړ����Ă��邩�ǂ���
	D3DXMATRIX mtxModel;							//���f���̃��[���h�̃}�g���b�N�X
	int nShadow;									//�e�̃C���f�b�N�X(�e�̔ԍ�)
	D3DXVECTOR3 VtxMinModel;						//���f���̍ŏ��l
	D3DXVECTOR3 VtxMaxModel;						//���f���̍ő�l
	int nPoint;										//�|�C���g
}Model;

//�v���g�^�C�v�錾
void InitModel(void);							//���f���̏���������
void UninitModel(void);							//���f���̏I������
void UpdateModel(void);							//���f���̍X�V����
void DrawModel(void);							//���f���̕`�揈��
Model *GetModel(void);							//���f���̎擾����

#endif