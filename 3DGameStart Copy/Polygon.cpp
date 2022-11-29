//===========================================
//
//�|���S���̃��C������[Polygon.cpp]
//Author ��������
//
//===========================================
#include"main.h"
#include "Polygon.h"
#include "input.h"

//�}�N����`

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturePolygon = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;	//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posPolygon;							//�ʒu
D3DXVECTOR3 g_rotPolygon;							//����
D3DXMATRIX g_mtxWorldPolygon;						//�|���S���̃��[���h�}�g���b�N�X

//==========================================
//�|���S���̏���������
//==========================================
void InitPolygon(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���n�̃e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Block.png",
		&g_pTexturePolygon);

	//�|���S���̈ʒu������������
	g_posPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�|���S���̌���������������
	g_rotPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-600.0f, 0.0f, +600.0f);
	pVtx[1].pos = D3DXVECTOR3(+600.0f, 0.0f, +600.0f);
	pVtx[2].pos = D3DXVECTOR3(-600.0f, 0.0f, -600.0f);
	pVtx[3].pos = D3DXVECTOR3(+600.0f, 0.0f, -600.0f);

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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPolygon->Unlock();
}

//========================================
//�|���S���̏I������
//========================================
void UninitPolygon(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//========================================
//�|���S���̍X�V����
//========================================
void UpdatePolygon(void)
{
	
}

//=====================================
//�|���S���̕`�揈��
//=====================================
void DrawPolygon(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldPolygon);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotPolygon.y, g_rotPolygon.x, g_rotPolygon.z);
	D3DXMatrixMultiply(&g_mtxWorldPolygon, &g_mtxWorldPolygon, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posPolygon.x, g_posPolygon.y, g_posPolygon.z);
	D3DXMatrixMultiply(&g_mtxWorldPolygon, &g_mtxWorldPolygon, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPolygon);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffPolygon,									//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_3D));									//���_���\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePolygon);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//�v���~�e�B�u�̎��
		0,													//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);													//�`�悷��v���~�e�B�u��
}