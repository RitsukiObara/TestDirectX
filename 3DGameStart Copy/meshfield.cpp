//===========================================
//
//���b�V���t�B�[���h�̃��C������[meshfield.cpp]
//Author ��������
//
//===========================================
#include "main.h"
#include "meshfield.h"

//�}�N����`
#define MAX_FIELD_TOP				(9)					//���_��
#define MAX_FIELD_INDEX				(14)				//�C���f�b�N�X��

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturemeshfield = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffmeshfield = NULL;		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;		//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posmeshfield;								//�ʒu
D3DXVECTOR3 g_rotmeshfield;								//����
D3DXMATRIX g_mtxWorldmeshfield;							//�|���S���̃��[���h�}�g���b�N�X

//==========================================
//���b�V���t�B�[���h�̏���������
//==========================================
void Initmeshfield(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���n�̃e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Block.png",
		&g_pTexturemeshfield);

	//�ʒu�̏�����
	g_posmeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//������������
	g_rotmeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_FIELD_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffmeshfield,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffmeshfield->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-MESH_RADIUS, +0.0f, +MESH_RADIUS);
	pVtx[1].pos = D3DXVECTOR3(+0.0f, +0.0f, +MESH_RADIUS);
	pVtx[2].pos = D3DXVECTOR3(+MESH_RADIUS, +0.0f, +MESH_RADIUS);
	pVtx[3].pos = D3DXVECTOR3(-MESH_RADIUS, +0.0f, +0.0f);
	pVtx[4].pos = D3DXVECTOR3(+0.0f, +0.0f, +0.0f);
	pVtx[5].pos = D3DXVECTOR3(+MESH_RADIUS, +0.0f, +0.0f);
	pVtx[6].pos = D3DXVECTOR3(-MESH_RADIUS, +0.0f, -MESH_RADIUS);
	pVtx[7].pos = D3DXVECTOR3(+0.0f, +0.0f, -MESH_RADIUS);
	pVtx[8].pos = D3DXVECTOR3(+MESH_RADIUS, +0.0f, -MESH_RADIUS);

	for (int nCntSet = 0; nCntSet < MAX_FIELD_TOP; nCntSet++)
	{
		//�@���x�N�g���̐ݒ�
		pVtx[nCntSet].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[nCntSet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffmeshfield->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_FIELD_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);

	WORD *pIdx;						//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 3;
	pIdx[1] = 0;
	pIdx[2] = 4;
	pIdx[3] = 1;
	pIdx[4] = 5;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 6;
	pIdx[8] = 6;
	pIdx[9] = 3;
	pIdx[10] = 7;
	pIdx[11] = 4;
	pIdx[12] = 8;
	pIdx[13] = 5;

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshField->Unlock();
}

//========================================
//�|���S���̏I������
//========================================
void Uninitmeshfield(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturemeshfield != NULL)
	{
		g_pTexturemeshfield->Release();
		g_pTexturemeshfield = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffmeshfield != NULL)
	{
		g_pVtxBuffmeshfield->Release();
		g_pVtxBuffmeshfield = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//========================================
//�|���S���̍X�V����
//========================================
void Updatemeshfield(void)
{

}

//=====================================
//�|���S���̕`�揈��
//=====================================
void Drawmeshfield(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

		//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldmeshfield);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotmeshfield.y, g_rotmeshfield.x, g_rotmeshfield.z);
	D3DXMatrixMultiply(&g_mtxWorldmeshfield, &g_mtxWorldmeshfield, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posmeshfield.x, g_posmeshfield.y, g_posmeshfield.z);
	D3DXMatrixMultiply(&g_mtxWorldmeshfield, &g_mtxWorldmeshfield, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldmeshfield);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffmeshfield,								//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_3D));									//���_���\���̂̃T�C�Y

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshField);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturemeshfield);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,													//�`�悷��ŏ��̒��_�C���f�b�N�X
		0,
		MAX_FIELD_TOP,										//�p�ӂ������_�̐�
		0,
		12);												//�`�悷��v���~�e�B�u��
}