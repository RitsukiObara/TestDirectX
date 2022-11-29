//===========================================
//
//���C�g�̃��C������[light.cpp]
//Author ��������
//
//===========================================
#include "main.h"
#include "light.h"

//�}�N����`
#define NUM_LIGHT			(3)			//���C�g�̐�

//�O���[�o���ϐ�
D3DLIGHT9 g_light[NUM_LIGHT];			//���C�g�̏��

//==========================================
//���C�g�̏���������
//==========================================
void InitLight(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXVECTOR3 vecDir;				//�ݒ�p�����x�N�g��

	for (int nCntSet = 0; nCntSet < NUM_LIGHT; nCntSet++)
	{
		//���C�g�̏����N���A����
		ZeroMemory(&g_light[nCntSet], sizeof(D3DLIGHT9));

		//���C�g�̎�ނ�ݒ�
		g_light[nCntSet].Type = D3DLIGHT_DIRECTIONAL;

		switch (nCntSet)
		{
		case 0:

			//���C�g�̊g�U����ݒ�
			g_light[nCntSet].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//���C�g�̕�����ݒ�
			vecDir = D3DXVECTOR3(0.22f,-0.87f, 0.44f);

			break;				//�����o��

		case 1:

			//���C�g�̊g�U����ݒ�
			g_light[nCntSet].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

			//���C�g�̕�����ݒ�
			vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);

			break;				//�����o��

		case 2:

			//���C�g�̊g�U����ݒ�
			g_light[nCntSet].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

			//���C�g�̕�����ݒ�
			vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);

			break;				//�����o��
		}

		D3DXVec3Normalize(&vecDir, &vecDir);				//�x�N�g���𐳋K������

		g_light[nCntSet].Direction = vecDir;

		//���C�g��ݒ肷��
		pDevice->SetLight(nCntSet, &g_light[nCntSet]);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCntSet, TRUE);
	}
}

//========================================
//���C�g�̏I������
//========================================
void UninitLight(void)
{
	
}

//========================================
//���C�g�̍X�V����
//========================================
void UpdateLight(void)
{

}