//===========================================
//
//���b�V���h�[���̃��C������[meshdome.cpp]
//Author ��������
//
//===========================================
#include "main.h"
#include "meshdome.h"

//�}�N����`
#define MAX_DOME_TOP			(36)
#define MAX_DOME_INDEX			(58)

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturemeshDome = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffmeshDome = NULL;		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshDome = NULL;		//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posmeshDome;								//�ʒu
D3DXVECTOR3 g_rotmeshDome;								//����
D3DXMATRIX g_mtxWorldmeshDome;							//�|���S���̃��[���h�}�g���b�N�X

//==========================================
//���b�V���t�B�[���h�̏���������
//==========================================
void InitmeshDome(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	////���n�̃e�N�X�`���̓ǂݍ���
	//D3DXCreateTextureFromFile(pDevice,
	//	"data/TEXTURE/Block.png",
	//	&g_pTexturemeshDome);

	//�ʒu�̏�����
	g_posmeshDome = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//������������
	g_rotmeshDome = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_DOME_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffmeshDome,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffmeshDome->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-sinf(0.0f) * SKY_RADIUS, SKY_HEIGHT * 0.0f, -cosf(0.0f) * SKY_RADIUS);
	pVtx[1].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * SKY_RADIUS, SKY_HEIGHT * 0.0f, -cosf(D3DX_PI * 0.25f) * SKY_RADIUS);
	pVtx[2].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * SKY_RADIUS, SKY_HEIGHT * 0.0f, -cosf(D3DX_PI * 0.5f) * SKY_RADIUS);
	pVtx[3].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * SKY_RADIUS, SKY_HEIGHT * 0.0f, -cosf(D3DX_PI * 0.75f) * SKY_RADIUS);
	pVtx[4].pos = D3DXVECTOR3(sinf(0.0f) * SKY_RADIUS, SKY_HEIGHT * 0.0f, cosf(0.0f) * SKY_RADIUS);
	pVtx[5].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * SKY_RADIUS, SKY_HEIGHT * 0.0f, -cosf(-D3DX_PI * 0.75f) * SKY_RADIUS);
	pVtx[6].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * SKY_RADIUS, SKY_HEIGHT * 0.0f, -cosf(-D3DX_PI * 0.5f) * SKY_RADIUS);
	pVtx[7].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * SKY_RADIUS, SKY_HEIGHT * 0.0f, -cosf(-D3DX_PI * 0.25f) * SKY_RADIUS);
	pVtx[8].pos = D3DXVECTOR3(-sinf(0.0f) * SKY_RADIUS, SKY_HEIGHT * 0.0f, -cosf(0.0f) * SKY_RADIUS);
	pVtx[9].pos = D3DXVECTOR3(-sinf(0.0f) * (SKY_RADIUS -200.0f), SKY_HEIGHT * 0.2f, -cosf(0.0f) * (SKY_RADIUS - 200.0f));
	pVtx[10].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * (SKY_RADIUS - 200.0f), SKY_HEIGHT * 0.2f, -cosf(D3DX_PI * 0.25f) * (SKY_RADIUS - 200.0f));
	pVtx[11].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * (SKY_RADIUS - 200.0f), SKY_HEIGHT * 0.2f, -cosf(D3DX_PI * 0.5f) * (SKY_RADIUS - 200.0f));
	pVtx[12].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * (SKY_RADIUS - 200.0f), SKY_HEIGHT * 0.2f, -cosf(D3DX_PI * 0.75f) * (SKY_RADIUS - 200.0f));
	pVtx[13].pos = D3DXVECTOR3(sinf(0.0f) * (SKY_RADIUS - 200.0f), SKY_HEIGHT * 0.2f, cosf(0.0f) * (SKY_RADIUS - 200.0f));
	pVtx[14].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * (SKY_RADIUS - 200.0f), SKY_HEIGHT * 0.2f, -cosf(-D3DX_PI * 0.75f) * (SKY_RADIUS - 200.0f));
	pVtx[15].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * (SKY_RADIUS - 200.0f), SKY_HEIGHT * 0.2f, -cosf(-D3DX_PI * 0.5f) * (SKY_RADIUS - 200.0f));
	pVtx[16].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * (SKY_RADIUS - 200.0f), SKY_HEIGHT * 0.2f, -cosf(-D3DX_PI * 0.25f) * (SKY_RADIUS - 200.0f));
	pVtx[17].pos = D3DXVECTOR3(-sinf(0.0f) * (SKY_RADIUS - 200.0f), SKY_HEIGHT * 0.2f, -cosf(0.0f) * (SKY_RADIUS - 200.0f));
	pVtx[18].pos = D3DXVECTOR3(-sinf(0.0f) * (SKY_RADIUS - 400.0f), SKY_HEIGHT * 0.4f, -cosf(0.0f) * (SKY_RADIUS - 400.0f));
	pVtx[19].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * (SKY_RADIUS - 400.0f), SKY_HEIGHT * 0.4f, -cosf(D3DX_PI * 0.25f) * (SKY_RADIUS - 400.0f));
	pVtx[20].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * (SKY_RADIUS - 400.0f), SKY_HEIGHT * 0.4f, -cosf(D3DX_PI * 0.5f) * (SKY_RADIUS - 400.0f));
	pVtx[21].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * (SKY_RADIUS - 400.0f), SKY_HEIGHT * 0.4f, -cosf(D3DX_PI * 0.75f) * (SKY_RADIUS - 400.0f));
	pVtx[22].pos = D3DXVECTOR3(sinf(0.0f) * (SKY_RADIUS - 400.0f), SKY_HEIGHT * 0.4f, cosf(0.0f) * (SKY_RADIUS - 400.0f));
	pVtx[23].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * (SKY_RADIUS - 400.0f), SKY_HEIGHT * 0.4f, -cosf(-D3DX_PI * 0.75f) * (SKY_RADIUS - 400.0f));
	pVtx[24].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * (SKY_RADIUS - 400.0f), SKY_HEIGHT * 0.4f, -cosf(-D3DX_PI * 0.5f) * (SKY_RADIUS - 400.0f));
	pVtx[25].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * (SKY_RADIUS - 400.0f), SKY_HEIGHT * 0.4f, -cosf(-D3DX_PI * 0.25f) * (SKY_RADIUS - 400.0f));
	pVtx[26].pos = D3DXVECTOR3(-sinf(0.0f) * (SKY_RADIUS - 400.0f), SKY_HEIGHT * 0.4f, -cosf(0.0f) * (SKY_RADIUS - 400.0f));
	pVtx[27].pos = D3DXVECTOR3(-sinf(0.0f) * (SKY_RADIUS - 600.0f), SKY_HEIGHT * 0.6f, -cosf(0.0f) * (SKY_RADIUS - 600.0f));
	pVtx[28].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * (SKY_RADIUS - 600.0f), SKY_HEIGHT * 0.6f, -cosf(D3DX_PI * 0.25f) * (SKY_RADIUS - 600.0f));
	pVtx[29].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * (SKY_RADIUS - 600.0f), SKY_HEIGHT * 0.6f, -cosf(D3DX_PI * 0.5f) * (SKY_RADIUS - 600.0f));
	pVtx[30].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * (SKY_RADIUS - 600.0f), SKY_HEIGHT * 0.6f, -cosf(D3DX_PI * 0.75f) * (SKY_RADIUS - 600.0f));
	pVtx[31].pos = D3DXVECTOR3(sinf(0.0f) * (SKY_RADIUS - 600.0f), SKY_HEIGHT * 0.6f, cosf(0.0f) * (SKY_RADIUS - 600.0f));
	pVtx[32].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * (SKY_RADIUS - 600.0f), SKY_HEIGHT * 0.6f, -cosf(-D3DX_PI * 0.75f) * (SKY_RADIUS - 600.0f));
	pVtx[33].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * (SKY_RADIUS - 600.0f), SKY_HEIGHT * 0.6f, -cosf(-D3DX_PI * 0.5f) * (SKY_RADIUS - 600.0f));
	pVtx[34].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * (SKY_RADIUS - 600.0f), SKY_HEIGHT * 0.6f, -cosf(-D3DX_PI * 0.25f) * (SKY_RADIUS - 600.0f));
	pVtx[35].pos = D3DXVECTOR3(-sinf(0.0f) * (SKY_RADIUS - 600.0f), SKY_HEIGHT * 0.6f, -cosf(0.0f) * (SKY_RADIUS - 600.0f));

	for (int nCntSet = 0; nCntSet < MAX_DOME_TOP; nCntSet++)
	{
		//�@���x�N�g���̐ݒ�
		pVtx[nCntSet].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[nCntSet].col = D3DXCOLOR(0.3f, 0.3f, 1.0f, 1.0f);
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
	pVtx[9].tex = D3DXVECTOR2(0.0f, 0.25f);
	pVtx[10].tex = D3DXVECTOR2(0.125f, 0.25f);
	pVtx[11].tex = D3DXVECTOR2(0.25f, 0.25f);
	pVtx[12].tex = D3DXVECTOR2(0.375f, 0.25f);
	pVtx[13].tex = D3DXVECTOR2(0.5f, 0.25f);
	pVtx[14].tex = D3DXVECTOR2(0.625f, 0.25f);
	pVtx[15].tex = D3DXVECTOR2(0.75f, 0.25f);
	pVtx[16].tex = D3DXVECTOR2(0.875f, 0.25f);
	pVtx[17].tex = D3DXVECTOR2(1.0f, 0.25f);
	pVtx[18].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[19].tex = D3DXVECTOR2(0.125f, 0.5f);
	pVtx[20].tex = D3DXVECTOR2(0.25f, 0.5f);
	pVtx[21].tex = D3DXVECTOR2(0.375f, 0.5f);
	pVtx[22].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[23].tex = D3DXVECTOR2(0.625f, 0.5f);
	pVtx[24].tex = D3DXVECTOR2(0.75f, 0.5f);
	pVtx[25].tex = D3DXVECTOR2(0.875f, 0.5f);
	pVtx[26].tex = D3DXVECTOR2(1.0f, 0.5f);
	pVtx[27].tex = D3DXVECTOR2(0.0f, 0.75f);
	pVtx[28].tex = D3DXVECTOR2(0.125f, 0.75f);
	pVtx[29].tex = D3DXVECTOR2(0.25f, 0.75f);
	pVtx[30].tex = D3DXVECTOR2(0.375f, 0.75f);
	pVtx[31].tex = D3DXVECTOR2(0.5f, 0.75f);
	pVtx[32].tex = D3DXVECTOR2(0.625f, 0.75f);
	pVtx[33].tex = D3DXVECTOR2(0.75f, 0.75f);
	pVtx[34].tex = D3DXVECTOR2(0.875f, 0.75f);
	pVtx[35].tex = D3DXVECTOR2(1.0f, 0.75f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffmeshDome->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_DOME_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshDome,
		NULL);

	WORD *pIdx;						//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 9;
	pIdx[1] = 0;
	pIdx[2] = 10;
	pIdx[3] = 1;
	pIdx[4] = 11;
	pIdx[5] = 2;
	pIdx[6] = 12;
	pIdx[7] = 3;
	pIdx[8] = 13;
	pIdx[9] = 4;
	pIdx[10] = 14;
	pIdx[11] = 5;
	pIdx[12] = 15;
	pIdx[13] = 6;
	pIdx[14] = 16;
	pIdx[15] = 7;
	pIdx[16] = 17;
	pIdx[17] = 8;
	pIdx[18] = 8;
	pIdx[19] = 18;
	pIdx[20] = 18;
	pIdx[21] = 9;
	pIdx[22] = 19;
	pIdx[23] = 10;
	pIdx[24] = 20;
	pIdx[25] = 11;
	pIdx[26] = 21;
	pIdx[27] = 12;
	pIdx[28] = 22;
	pIdx[29] = 13;
	pIdx[30] = 23;
	pIdx[31] = 14;
	pIdx[32] = 24;
	pIdx[33] = 15;
	pIdx[34] = 25;
	pIdx[35] = 16;
	pIdx[36] = 26;
	pIdx[37] = 17;
	pIdx[38] = 17;
	pIdx[39] = 27;
	pIdx[40] = 27;
	pIdx[41] = 18;
	pIdx[42] = 28;
	pIdx[43] = 19;
	pIdx[44] = 29;
	pIdx[45] = 20;
	pIdx[46] = 30;
	pIdx[47] = 21;
	pIdx[48] = 31;
	pIdx[49] = 22;
	pIdx[50] = 32;
	pIdx[51] = 23;
	pIdx[52] = 33;
	pIdx[53] = 24;
	pIdx[54] = 34;
	pIdx[55] = 25;
	pIdx[56] = 35;
	pIdx[57] = 26;

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshDome->Unlock();
}

//========================================
//�|���S���̏I������
//========================================
void UninitmeshDome(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturemeshDome != NULL)
	{
		g_pTexturemeshDome->Release();
		g_pTexturemeshDome = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffmeshDome != NULL)
	{
		g_pVtxBuffmeshDome->Release();
		g_pVtxBuffmeshDome = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMeshDome != NULL)
	{
		g_pIdxBuffMeshDome->Release();
		g_pIdxBuffMeshDome = NULL;
	}
}

//========================================
//�|���S���̍X�V����
//========================================
void UpdatemeshDome(void)
{

}

//=====================================
//�|���S���̕`�揈��
//=====================================
void DrawmeshDome(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldmeshDome);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotmeshDome.y, g_rotmeshDome.x, g_rotmeshDome.z);
	D3DXMatrixMultiply(&g_mtxWorldmeshDome, &g_mtxWorldmeshDome, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posmeshDome.x, g_posmeshDome.y, g_posmeshDome.z);
	D3DXMatrixMultiply(&g_mtxWorldmeshDome, &g_mtxWorldmeshDome, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldmeshDome);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffmeshDome,								//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_3D));									//���_���\���̂̃T�C�Y

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshDome);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturemeshDome);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,													//�`�悷��ŏ��̒��_�C���f�b�N�X
		0,
		MAX_DOME_TOP,										//�p�ӂ������_�̐�
		0,
		56);												//�`�悷��v���~�e�B�u��
}