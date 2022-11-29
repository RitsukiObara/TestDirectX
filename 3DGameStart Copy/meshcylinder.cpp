//===========================================
//
//���b�V���V�����_�[�̃��C������[meshcylinder.cpp]
//Author ��������
//
//===========================================
#include "main.h"
#include "meshcylinder.h"

//�}�N����`
#define MAX_CYLINDER_TOP			(18)				//���_��
#define MAX_CYLINDER_INDEX			(18)				//�C���f�b�N�X��
#define MAX_CYLINDERLENGTH			(800.0f)			//���S����̔w�i�̋���

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturemeshCylinder = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffmeshCylinder = NULL;		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL;		//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posmeshCylinder;								//�ʒu
D3DXMATRIX g_mtxWorldmeshCylinder;							//�|���S���̃��[���h�}�g���b�N�X

//==========================================
//���b�V���t�B�[���h�̏���������
//==========================================
void InitmeshCylinder(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���n�̃e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Block.png",
		&g_pTexturemeshCylinder);

	//�ʒu�̏�����
	g_posmeshCylinder = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_CYLINDER_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffmeshCylinder,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffmeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-sinf(0.0f) * MAX_CYLINDERLENGTH, BG_HEIGHT, -cosf(0.0f) * MAX_CYLINDERLENGTH);
	pVtx[1].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * MAX_CYLINDERLENGTH, BG_HEIGHT, -cosf(D3DX_PI * 0.25f) * MAX_CYLINDERLENGTH);
	pVtx[2].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * MAX_CYLINDERLENGTH, BG_HEIGHT, -cosf(D3DX_PI * 0.5f) * MAX_CYLINDERLENGTH);
	pVtx[3].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * MAX_CYLINDERLENGTH, BG_HEIGHT, -cosf(D3DX_PI * 0.75f) * MAX_CYLINDERLENGTH);
	pVtx[4].pos = D3DXVECTOR3(sinf(0.0f) * MAX_CYLINDERLENGTH, BG_HEIGHT, cosf(0.0f) * MAX_CYLINDERLENGTH);
	pVtx[5].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * MAX_CYLINDERLENGTH, BG_HEIGHT, -cosf(-D3DX_PI * 0.75f) * MAX_CYLINDERLENGTH);
	pVtx[6].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * MAX_CYLINDERLENGTH, BG_HEIGHT, -cosf(-D3DX_PI * 0.5f) * MAX_CYLINDERLENGTH);
	pVtx[7].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * MAX_CYLINDERLENGTH, BG_HEIGHT, -cosf(-D3DX_PI * 0.25f) * MAX_CYLINDERLENGTH);
	pVtx[8].pos = D3DXVECTOR3(-sinf(0.0f) * MAX_CYLINDERLENGTH, BG_HEIGHT, -cosf(0.0f) * MAX_CYLINDERLENGTH);
	pVtx[9].pos = D3DXVECTOR3(-sinf(0.0f) * MAX_CYLINDERLENGTH, 0.0f, -cosf(0.0f) * MAX_CYLINDERLENGTH);
	pVtx[10].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * MAX_CYLINDERLENGTH, 0.0f, -cosf(D3DX_PI * 0.25f) * MAX_CYLINDERLENGTH);
	pVtx[11].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * MAX_CYLINDERLENGTH, 0.0f, -cosf(D3DX_PI * 0.5f) * MAX_CYLINDERLENGTH);
	pVtx[12].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * MAX_CYLINDERLENGTH, 0.0f, -cosf(D3DX_PI * 0.75f) * MAX_CYLINDERLENGTH);
	pVtx[13].pos = D3DXVECTOR3(sinf(0.0f) * MAX_CYLINDERLENGTH, 0.0f, cosf(0.0f) * MAX_CYLINDERLENGTH);
	pVtx[14].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * MAX_CYLINDERLENGTH, 0.0f, -cosf(-D3DX_PI * 0.75f) * MAX_CYLINDERLENGTH);
	pVtx[15].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * MAX_CYLINDERLENGTH, 0.0f, -cosf(-D3DX_PI * 0.5f) * MAX_CYLINDERLENGTH);
	pVtx[16].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * MAX_CYLINDERLENGTH, 0.0f, -cosf(-D3DX_PI * 0.25f) * MAX_CYLINDERLENGTH);
	pVtx[17].pos = D3DXVECTOR3(-sinf(0.0f) * MAX_CYLINDERLENGTH, 0.0f, -cosf(0.0f) * MAX_CYLINDERLENGTH);

	for (int nCntSet = 0; nCntSet < MAX_CYLINDER_TOP; nCntSet++)
	{
		//�@���x�N�g���̐ݒ�
		pVtx[nCntSet].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[nCntSet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.375f, 0.0f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(0.625f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.75f, 0.0f);
	pVtx[7].tex = D3DXVECTOR2(0.875f, 0.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[10].tex = D3DXVECTOR2(0.125f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(0.25f, 1.0f);
	pVtx[12].tex = D3DXVECTOR2(0.375f, 1.0f);
	pVtx[13].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[14].tex = D3DXVECTOR2(0.625f, 1.0f);
	pVtx[15].tex = D3DXVECTOR2(0.75f, 1.0f);
	pVtx[16].tex = D3DXVECTOR2(0.875f, 1.0f);
	pVtx[17].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffmeshCylinder->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_CYLINDER_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCylinder,
		NULL);

	WORD *pIdx;						//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 0;
	pIdx[1] = 9;
	pIdx[2] = 1;
	pIdx[3] = 10;
	pIdx[4] = 2;
	pIdx[5] = 11;
	pIdx[6] = 3;
	pIdx[7] = 12;
	pIdx[8] = 4;
	pIdx[9] = 13;
	pIdx[10] = 5;
	pIdx[11] = 14;
	pIdx[12] = 6;
	pIdx[13] = 15;
	pIdx[14] = 7;
	pIdx[15] = 16;
	pIdx[16] = 8;
	pIdx[17] = 17;

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshCylinder->Unlock();
}

//========================================
//�|���S���̏I������
//========================================
void UninitmeshCylinder(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturemeshCylinder != NULL)
	{
		g_pTexturemeshCylinder->Release();
		g_pTexturemeshCylinder = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffmeshCylinder != NULL)
	{
		g_pVtxBuffmeshCylinder->Release();
		g_pVtxBuffmeshCylinder = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}
}

//========================================
//�|���S���̍X�V����
//========================================
void UpdatemeshCylinder(void)
{

}

//=====================================
//�|���S���̕`�揈��
//=====================================
void DrawmeshCylinder(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldmeshCylinder);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posmeshCylinder.x, g_posmeshCylinder.y, g_posmeshCylinder.z);
	D3DXMatrixMultiply(&g_mtxWorldmeshCylinder, &g_mtxWorldmeshCylinder, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldmeshCylinder);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffmeshCylinder,								//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_3D));									//���_���\���̂̃T�C�Y

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshCylinder);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturemeshCylinder);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,													//�`�悷��ŏ��̒��_�C���f�b�N�X
		0,
		MAX_CYLINDER_TOP,									//�p�ӂ������_�̐�
		0,
		16);												//�`�悷��v���~�e�B�u��
}