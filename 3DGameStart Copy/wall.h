//===================================
//
//�ǃw�b�_�[[Wall.h]
//Author ��������
//
//===================================
#ifndef _WALL_H_
#define _WALL_H_

//�}�N����`
#define NUM_WALL			(8)					//�ǂ̐�

//�ǂ̍\����
typedef struct
{
	D3DXVECTOR3 pos;							//�ʒu
	D3DXVECTOR3 rot;							//����
	D3DXMATRIX mtx;								//�ǂ̃��[���h�}�g���b�N�X
}Wall;

//�v���g�^�C�v�錾
void InitWall(void);					//�ǂ̏���������
void UninitWall(void);					//�ǂ̏I������
void UpdateWall(void);					//�ǂ̍X�V����
void DrawWall(void);					//�ǂ̕`�揈��
Wall *GetWall(void);					//�ǂ̎擾����

#endif