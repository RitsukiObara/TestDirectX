//===================================
//
//�G�w�b�_�[[Enemy.h]
//Author ��������
//
//===================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//�}�N����`
#define MAX_ENEMY					(128)			//�G�̍ő吔

//���f���̍\����
typedef struct
{
	D3DXVECTOR3 pos;								//�ʒu
	D3DXVECTOR3 posOld;								//�O��̈ʒu
	D3DXVECTOR3 move;								//�ړ���
	D3DXVECTOR3 rot;								//����
	bool bUse;										//�g�p���Ă��邩�ǂ���
	int nType;										//���
	D3DXMATRIX mtx;									//���[���h�}�g���b�N�X
	LPD3DXMESH g_pMeshEnemy = NULL;					//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER g_pBuffMatEnemy = NULL;			//�}�e���A���ւ̃|�C���^
	DWORD g_dwNumMatEnemy = 0;						//�}�e���A���̐�
	int nShadow = -1;								//�e�̃C���f�b�N�X(�e�̔ԍ�)
	D3DXVECTOR3 VtxMinEnemy;						//�G�̍ŏ��l
	D3DXVECTOR3 VtxMaxEnemy;						//�G�̍ő�l
}Enemy;

//�G�̎��
typedef enum
{
	ENEMY_SLIME = 0,								//�X���C��
	ENEMY_INSECT,									//��
	ENEMY_MAX
}ENEMTTYPE;

//�v���g�^�C�v�錾
void InitEnemy(void);							//�G�̏���������
void UninitEnemy(void);							//�G�̏I������
void UpdateEnemy(void);							//�G�̍X�V����
void DrawEnemy(void);							//�G�̕`�揈��
void SetEnemy(int nType);						//�G�̐ݒ菈��
int GetEnemyCount(void);						//�G�̑����擾����
Enemy *GetEnemy(void);							//�G�̎擾����

#endif