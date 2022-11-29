//=======================================
//
//�p�[�e�B�N���̃��C������[Particle.cpp]
//Author ��������
//
//=======================================
#include"main.h"
#include"Particle.h"
#include"Effect.h"
#include <time.h>				//�����_�������ɕK�v

//�}�N����`
#define MAX_PARTICLE			(128)		//�p�[�e�B�N���̍ő吔

//�p�[�e�B�N���\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;					//�ʒu(�o���ʒu)
	D3DXVECTOR3 move;					//�ړ���
	float fRadius;						//���a
	int nLife;							//����(��������)
	int nType;							//���(���R)
	int nCount;							//�J�E���g
	bool bUse;							//�g�p���Ă��邩�ǂ���
}Particle;

//�O���[�o���ϐ�
Particle g_aParticle[MAX_PARTICLE];		//�p�[�e�B�N���̏��

//==========================================
//�p�[�e�B�N���̏���������
//==========================================
void InitParticle(void)
{
	//�����̎��ݒu
	srand((unsigned int)time(NULL));

	int nCntParticle;

	//�����̏��̏�����
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ʒu������������

		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ړ��ʂ�����������

		g_aParticle[nCntParticle].fRadius = 0.0f;								//���a������������

		g_aParticle[nCntParticle].nLife = 0;									//����������������

		g_aParticle[nCntParticle].nCount = 0;									//�J�E���g������������

		g_aParticle[nCntParticle].bUse = false;									//�g�p���Ă��Ȃ���Ԃɂ���
	}
}

//========================================
//�p�[�e�B�N���̏I������
//========================================
void UninitParticle(void)
{

}

//========================================
//�p�[�e�B�N���̍X�V����
//========================================
void UpdateParticle(void)
{
	int nCntParticle;					//�񐔂̕ϐ���錾����
	D3DXVECTOR3 fAngle;					//�p�x�̕ϐ�
	float fSpeed;						//�ړ��ʂ̕ϐ�

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{//�p�[�e�B�N�����g�p����Ă���
			//�p�[�e�B�N���̎��������Z����
			g_aParticle[nCntParticle].nLife--;

			//�J�E���^�[�����Z����
			g_aParticle[nCntParticle].nCount++;

			switch (g_aParticle[nCntParticle].nType)
			{
			case PTCL_EXPL:					//�����p�[�e�B�N��

				for (int nCntSet = 0; nCntSet < 10; nCntSet++)
				{
					//�p�x��ݒ肷��
					fAngle.x = (float)(rand() % 629 - 314) / 100.0f;

					//�p�x��ݒ肷��
					fAngle.y = (float)(rand() % 629 - 314) / 100.0f;

					//�p�x��ݒ肷��
					fAngle.z = (float)(rand() % 629 - 314) / 100.0f;

					//�ړ��ʂ�ݒ肷��
					fSpeed = (float)(rand() % 500) / 100.0f + 4.0f;

					//�ړ��ʂ�ݒ肷��
					g_aParticle[nCntParticle].move.x = sinf(fAngle.x) * fSpeed;
					g_aParticle[nCntParticle].move.y = cosf(fAngle.y) * fSpeed;
					g_aParticle[nCntParticle].move.z = cosf(fAngle.z) * fSpeed;

					//���a��ݒ肷��
					g_aParticle[nCntParticle].fRadius = (float)(rand() % 400) / 100.0f + 10.0f;

					//�G�t�F�N�g�̐ݒ菈��
					SetEffect(g_aParticle[nCntParticle].pos, g_aParticle[nCntParticle].move, g_aParticle[nCntParticle].fRadius, 20, EFFECT_EXPL);
				}

				break;						//�����o��
			}

			if (g_aParticle[nCntParticle].nLife == 0)
			{//�������s�����ꍇ
			 //�g�p���Ă��Ȃ���Ԃɂ���
				g_aParticle[nCntParticle].bUse = false;
			}
		}
	}
}

//=====================================
//�p�[�e�B�N���̕`�揈��
//=====================================
void DrawParticle(void)
{

}

//========================================
//�p�[�e�B�N���̐ݒ菈��
//========================================
void SetParticle(D3DXVECTOR3 pos, int nType)
{
	int nCntParticle;

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{//�p�[�e�B�N�����g�p����Ă��Ȃ�
		 //��ނ�ݒ�
			g_aParticle[nCntParticle].nType = nType;

			switch (g_aParticle[nCntParticle].nType)
			{
			case PTCL_EXPL:				//�v���C���[�����񂾂Ƃ�
				//�ʒu��ݒ�
				g_aParticle[nCntParticle].pos = pos;

				//������ݒ�
				g_aParticle[nCntParticle].nLife = 5;

				break;					//�����o��
			}

			//�J�E���^�[��0�ɐݒ肷��
			g_aParticle[nCntParticle].nCount = 0;

			//�g�p���Ă����Ԃɂ���
			g_aParticle[nCntParticle].bUse = true;				//�g�p���Ă����Ԃɂ���

			break;
		}
	}
}