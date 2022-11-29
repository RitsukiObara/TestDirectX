//===================================
//
//�e�w�b�_�[[Bullet.h]
//Author ��������
//
//===================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//�r���{�[�h�̍\����
typedef struct
{
	D3DXVECTOR3 pos;								//�ʒu
	D3DXVECTOR3 rot;								//����(���˂̕����ݒ�ɕK�v)
	D3DXMATRIX mtx;									//���[���h�}�g���b�N�X
	int nLife;										//����
	int nShadow;									//�e�̃C���f�b�N�X(�e�̔ԍ�)
	bool bUse;										//�g�p���Ă��邩�ǂ���
}Bullet;

//�v���g�^�C�v�錾
void InitBullet(void);								//�e�̏���������
void UninitBullet(void);							//�e�̏I������
void UpdateBullet(void);							//�e�̍X�V����
void DrawBullet(void);								//�e�̕`�揈��
void SetBullet(D3DXVECTOR3 rot, D3DXVECTOR3 pos);	//�e�̐ݒ菈��

#endif