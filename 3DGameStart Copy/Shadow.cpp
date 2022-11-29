//===========================================
//
//�|���S���̃��C������[Shadow.cpp]
//Author ��������
//
//===========================================
#include "main.h"
#include "Shadow.h"

//�}�N����`
#define MAX_SHADOW					(256)			//�e�̍ő吔

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Shadow g_aShadow[MAX_SHADOW];						//�e�̏��
D3DXMATRIX g_mtxWorldShadow;						//�|���S���̃��[���h�}�g���b�N�X

//==========================================
//�e�̏���������
//==========================================
void InitShadow(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���n�̃e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/shadow000.jpg",
		&g_pTextureShadow);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{//���̏�����
		//�ʒu��������
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//������������
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�g�p���Ă��Ȃ�
		g_aShadow[nCntShadow].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBuff = 0; nCntBuff < MAX_SHADOW; nCntBuff++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-20.0f, 0.0f, +10.0f);
		pVtx[1].pos = D3DXVECTOR3(+20.0f, 0.0f, +10.0f);
		pVtx[2].pos = D3DXVECTOR3(-20.0f, 0.0f, -10.0f);
		pVtx[3].pos = D3DXVECTOR3(+20.0f, 0.0f, -10.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;									//���_�f�[�^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShadow->Unlock();
}

//========================================
//�e�̏I������
//========================================
void UninitShadow(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//========================================
//�e�̍X�V����
//========================================
void UpdateShadow(void)
{

}

//=====================================
//�e�̕`�揈��
//=====================================
void DrawShadow(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	//���Z����
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{//�e���g�p����Ă����ꍇ
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldShadow);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldShadow);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0,
				g_pVtxBuffShadow,									//���_�o�b�t�@�ւ̃|�C���^
				0,
				sizeof(VERTEX_3D));									//���_���\���̂̃T�C�Y

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureShadow);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//�v���~�e�B�u�̎��
				nCntShadow * 4,										//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);													//�`�悷��v���~�e�B�u��
		}
	}

	//�ʏ�̍����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===============================
//�e�̐ݒ菈��
//===============================
int SetShadow(int nType)
{
	//�e�̔ԍ�������������
	int nIdxShadow = -1;

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == false)
		{//�g�p���Ă��Ȃ������ꍇ

			switch (nType)
			{
			case SHADOW_PLAYER:				//�v���C���[�̉e

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(-20.0f, 0.0f, +10.0f);
				pVtx[1].pos = D3DXVECTOR3(+20.0f, 0.0f, +10.0f);
				pVtx[2].pos = D3DXVECTOR3(-20.0f, 0.0f, -10.0f);
				pVtx[3].pos = D3DXVECTOR3(+20.0f, 0.0f, -10.0f);

				break;						//�����o��

			case SHADOW_ENEMY:				//�G�̉e

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(-20.0f, 0.0f, +20.0f);
				pVtx[1].pos = D3DXVECTOR3(+20.0f, 0.0f, +20.0f);
				pVtx[2].pos = D3DXVECTOR3(-20.0f, 0.0f, -20.0f);
				pVtx[3].pos = D3DXVECTOR3(+20.0f, 0.0f, -20.0f);

				break;						//�����o��

			case SHADOW_TOWER:				//���̉e

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(-230.0f, 0.0f, +230.0f);
				pVtx[1].pos = D3DXVECTOR3(+230.0f, 0.0f, +230.0f);
				pVtx[2].pos = D3DXVECTOR3(-230.0f, 0.0f, -230.0f);
				pVtx[3].pos = D3DXVECTOR3(+230.0f, 0.0f, -230.0f);

				break;						//�����o��

			case SHADOW_BULLET:				//�e�̉e

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(-10.0f, 0.0f, +10.0f);
				pVtx[1].pos = D3DXVECTOR3(+10.0f, 0.0f, +10.0f);
				pVtx[2].pos = D3DXVECTOR3(-10.0f, 0.0f, -10.0f);
				pVtx[3].pos = D3DXVECTOR3(+10.0f, 0.0f, -10.0f);

				break;						//�����o��
			}

			//�ʒu��ݒ肷��
			g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//������ݒ肷��
			g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//�g�p����
			g_aShadow[nCntShadow].bUse = true;

			//�e�̔ԍ����R�s�[����
			nIdxShadow = nCntShadow;

			break;				//�����o��
		}
		pVtx += 4;				//���_�f�[�^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShadow->Unlock();

	//�e�̔ԍ���Ԃ�
	return nIdxShadow;
}

//===============================
//�e�̈ʒu�ݒ菈��
//===============================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (g_aShadow[nIdxShadow].bUse == true)
	{//�g�p����Ă����ꍇ
		//�ʒu��ݒ肷��
		g_aShadow[nIdxShadow].pos.x = pos.x;
		g_aShadow[nIdxShadow].pos.z = pos.z;

		//������ݒ肷��
		g_aShadow[nIdxShadow].rot.y = rot.y;
	}
}

//================================
//�e�̃��Z�b�g����
//================================
void ResetShadow(int *nIdxShadow)
{
	if (g_aShadow[*nIdxShadow].bUse == true)
	{//�e���g�p����Ă����ꍇ
		//�g�p����߂�
		g_aShadow[*nIdxShadow].bUse = false;

		//�e�̔ԍ���-1�ɖ߂�
		*nIdxShadow = -1;
	}
}