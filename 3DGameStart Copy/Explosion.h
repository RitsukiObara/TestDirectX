//===================================
//
//�����w�b�_�[[Explosion.h]
//Author ��������
//
//===================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"

//�r���{�[�h�̍\����
typedef struct
{
	D3DXVECTOR3 pos;								//�ʒu
	D3DXMATRIX mtx;									//���[���h�}�g���b�N�X
	int nLife;										//����
	int nAnimCounter;								//�A�j���J�E���^�[
	int nAnimPattern;								//�A�j���p�^�[��No.
	bool bUse;										//�g�p���Ă��邩�ǂ���
}Explosion;

//�v���g�^�C�v�錾
void InitExplosion(void);							//�����̏���������
void UninitExplosion(void);							//�����̏I������
void UpdateExplosion(void);							//�����̍X�V����
void DrawExplosion(void);							//�����̕`�揈��
void SetExplosion(D3DXVECTOR3 pos);					//�����̐ݒ菈��

#endif