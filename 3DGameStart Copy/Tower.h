//===================================
//
//���w�b�_�[[model.h]
//Author ��������
//
//===================================
#ifndef _TOWER_H_
#define _TOWER_H_

#include "main.h"

//���f���̍\����
typedef struct
{
	D3DXVECTOR3 pos;								//�ʒu
	D3DXVECTOR3 rot;								//����
	bool bUse;										//�g�p���Ă��邩�ǂ���
	D3DXMATRIX mtx;									//���[���h�}�g���b�N�X
	int TowerState;									//�^���[�̏��
	int nLife;										//�̗�
	int nDamageCount;								//�_���[�W�J�E���g
	int nInvincibleCount;							//���G�J�E���g
	int nShadow;									//�e�̃C���f�b�N�X(�e�̔ԍ�)
	bool bFlash;									//�_�ł��邩�ǂ���
	D3DXVECTOR3 VtxMinTower;						//���̍ŏ��l
	D3DXVECTOR3 VtxMaxTower;						//���̍ő�l
}Tower;

//���̏��
typedef enum
{
	TOWER_NORMAL = 0,								//�ʏ���
	TOWER_DAMAGE,									//�_���[�W���
	TOWER_INVINCIBLE,								//���G���
	TOWER_PINCH,									//�s���`���
	TOWER_DEATH,									//���S���
	TOWER_MAX
}TOWERSTATE;

//���ɂԂ��镨�̏���
typedef enum
{
	COLL_PLAYER = 0,
	COLL_ENEMY,
	COLL_MAX
}COLLISIONTYPE;

//�v���g�^�C�v�錾
void InitTower(void);							//���̏���������
void UninitTower(void);							//���̏I������
void UpdateTower(void);							//���̍X�V����
void DrawTower(void);							//���̕`�揈��
Tower *GetTower(void);							//���̎擾����
void CollisionTower(D3DXVECTOR3 MaxPos, D3DXVECTOR3 MinPos, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, int nType);			//���̓����蔻��

#endif