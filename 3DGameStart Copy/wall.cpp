//===========================================
//
//�ǂ̃��C������[wall.cpp]
//Author ��������
//
//===========================================
#include "main.h"
#include "wall.h"
#include "input.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Wall g_aWall[NUM_WALL];							//�ǂ̏��

//==========================================
//�|���S���̏���������
//==========================================
void InitWall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	////���n�̃e�N�X�`���̓ǂݍ���
	//D3DXCreateTextureFromFile(pDevice,
	//	"data/TEXTURE/�W���W�� D4C.png",
	//	&g_pTextureWall);

	for (int nCntSet = 0; nCntSet < NUM_WALL; nCntSet++)
	{//���̏�����
		switch (nCntSet)
		{
		case 0:					//�E�̕�1

			//�ʒu�̏�����
			g_aWall[nCntSet].pos = D3DXVECTOR3(600.0f, 0.0f, 0.0f);

			//������������
			g_aWall[nCntSet].rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);

			break;				//�����o��

		case 1:					//���̕�1

			//�ʒu�̏�����
			g_aWall[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, 600.0f);

			//������������
			g_aWall[nCntSet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			break;				//�����o��

		case 2:					//���̕�1

			//�ʒu�̏�����
			g_aWall[nCntSet].pos = D3DXVECTOR3(-600.0f, 0.0f, 0.0f);

			//������������
			g_aWall[nCntSet].rot = D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f);

			break;				//�����o��

		case 3:					//��O�̕�1

			//�ʒu�̏�����
			g_aWall[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, -600.0f);

			//������������
			g_aWall[nCntSet].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

			break;				//�����o��

		case 4:					//�E�̕�2

			//�ʒu�̏�����
			g_aWall[nCntSet].pos = D3DXVECTOR3(600.0f, 0.0f, 0.0f);

			//������������
			g_aWall[nCntSet].rot = D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f);

			break;				//�����o��

		case 5:					//���̕�2

			//�ʒu�̏�����
			g_aWall[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, 600.0f);

			//������������
			g_aWall[nCntSet].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

			break;				//�����o��

		case 6:					//���̕�2

			//�ʒu�̏�����
			g_aWall[nCntSet].pos = D3DXVECTOR3(-600.0f, 0.0f, 0.0f);

			//������������
			g_aWall[nCntSet].rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);

			break;				//�����o��

		case 7:					//��O�̕�2

			//�ʒu�̏�����
			g_aWall[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, -600.0f);

			//������������
			g_aWall[nCntSet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			break;				//�����o��
		}
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * NUM_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < NUM_WALL; nCntWall++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-600.0f, +300.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+600.0f, +300.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-600.0f, +0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+600.0f, +0.0f, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].hor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].hor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].hor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].hor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		if (nCntWall > 3)
		{//���̕ǂ̏ꍇ
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
		}
		else
		{//�\�̕�
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;						//���_�f�[�^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();
}

//========================================
//�|���S���̏I������
//========================================
void UninitWall(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//========================================
//�|���S���̍X�V����
//========================================
void UpdateWall(void)
{

}

//=====================================
//�|���S���̕`�揈��
//=====================================
void DrawWall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntWall = 0; nCntWall < NUM_WALL; nCntWall++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aWall[nCntWall].mtx);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
		D3DXMatrixMultiply(&g_aWall[nCntWall].mtx, &g_aWall[nCntWall].mtx, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
		D3DXMatrixMultiply(&g_aWall[nCntWall].mtx, &g_aWall[nCntWall].mtx, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].mtx);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0,
			g_pVtxBuffWall,										//���_�o�b�t�@�ւ̃|�C���^
			0,
			sizeof(VERTEX_3D));									//���_���\���̂̃T�C�Y

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureWall);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//�v���~�e�B�u�̎��
			4 * nCntWall,										//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);													//�`�悷��v���~�e�B�u��
	}
}

//=============================
//�ǂ̎擾����
//=============================
Wall *GetWall(void)
{
	//�ǂ̏���Ԃ�
	return &g_aWall[0];
}