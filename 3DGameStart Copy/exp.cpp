//=======================================
//
//�o���l�̃��C������[Exp.cpp]
//Author ��������
//
//=======================================
#include "main.h"
#include "exp.h"
#include "model.h"

//�}�N����`
#define MAX_EXP			(1024)		//�G�t�F�N�g�̍ő吔

//�v���g�^�C�v�錾
void CollisionExperience(Model *pModel, Exp *pExp);		//�o���l�̓����蔻��
void GetExperience(Model * pModel, Exp *pExp);			//�o���l�̎擾����

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureExp = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExp = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Exp g_aExp[MAX_EXP];							//�o���l�̏��

//==========================================
//�o���l�̏���������
//==========================================
void InitExp(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExp;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data//TEXTURE//shadow000.jpg",
		&g_pTextureExp);

	//�����̏��̏�����
	for (nCntExp = 0; nCntExp < MAX_EXP; nCntExp++)
	{
		g_aExp[nCntExp].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ʒu������������

		g_aExp[nCntExp].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����������������

		g_aExp[nCntExp].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ړ��ʂ�����������

		g_aExp[nCntExp].fRadius = 0.0f;									//���a������������

		g_aExp[nCntExp].nLife = 100;									//����������������

		g_aExp[nCntExp].bUse = false;									//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EXP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExp,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffExp->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExp = 0; nCntExp < MAX_EXP; nCntExp++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_aExp[nCntExp].fRadius, +g_aExp[nCntExp].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+g_aExp[nCntExp].fRadius, +g_aExp[nCntExp].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aExp[nCntExp].fRadius, -g_aExp[nCntExp].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+g_aExp[nCntExp].fRadius, -g_aExp[nCntExp].fRadius, 0.0f);

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

		pVtx += 4;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExp->Unlock();
}

//========================================
//�o���l�̏I������
//========================================
void UninitExp(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureExp != NULL)
	{
		g_pTextureExp->Release();
		g_pTextureExp = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffExp != NULL)
	{
		g_pVtxBuffExp->Release();
		g_pVtxBuffExp = NULL;
	}
}

//========================================
//�o���l�̍X�V����
//========================================
void UpdateExp(void)
{
	int nCntExp;					//�񐔂̕ϐ���錾����
	Model *pModel = GetModel();		//�v���C���[�̏����擾����

	VERTEX_3D * pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExp->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExp = 0; nCntExp < MAX_EXP; nCntExp++)
	{
		if (g_aExp[nCntExp].bUse == true)
		{//�o���l���g�p����Ă���
			//���������炵�Ă���
			g_aExp[nCntExp].nLife--;

			float fRotDest,			//�ڕW�̌���
				  fRotDiff;			//�ڕW�܂ł̍���

			//�ړI�̌�����ݒ肷��
			fRotDest = atan2f(pModel->pos.x - g_aExp[nCntExp].pos.x, pModel->pos.z - g_aExp[nCntExp].pos.z);

			//�����̍��������߂�
			fRotDiff = fRotDest - g_aExp[nCntExp].rot.y * 0.8f;

			if (fRotDiff > D3DX_PI)
			{//�p�x��3.14f���傫�������ꍇ
				//�p�x����1�������炷
				fRotDiff = -D3DX_PI;
			}
			else if (fRotDiff < -D3DX_PI)
			{//�p�x��-3.14f��菬���������ꍇ
				//�p�x��1����������
				fRotDiff = D3DX_PI;
			}

			//�p�x��␳����
			g_aExp[nCntExp].rot.y += fRotDiff;

			if (g_aExp[nCntExp].rot.y > D3DX_PI)
			{//�p�x��3.14f���傫�������ꍇ
				//�p�x����1�������炷
				g_aExp[nCntExp].rot.y = -D3DX_PI;
			}
			else if (g_aExp[nCntExp].rot.y < -D3DX_PI)
			{//�p�x��-3.14f��菬���������ꍇ
				//�p�x��1����������
				g_aExp[nCntExp].rot.y = D3DX_PI;
			}

			//��ɉ�������
			g_aExp[nCntExp].move.x = sinf(g_aExp[nCntExp].rot.y) * 10.0f;
			g_aExp[nCntExp].move.z = cosf(g_aExp[nCntExp].rot.y) * 10.0f;

			//�ʒu���X�V����
			g_aExp[nCntExp].pos.x += g_aExp[nCntExp].move.x;
			g_aExp[nCntExp].pos.z += g_aExp[nCntExp].move.z;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aExp[nCntExp].fRadius, +g_aExp[nCntExp].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aExp[nCntExp].fRadius, +g_aExp[nCntExp].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aExp[nCntExp].fRadius, -g_aExp[nCntExp].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aExp[nCntExp].fRadius, -g_aExp[nCntExp].fRadius, 0.0f);

			if (g_aExp[nCntExp].nLife == 0)
			{//�G�t�F�N�g�̎������s������
				//�g�p���Ă��Ȃ����Ƃɂ���
				g_aExp[nCntExp].bUse = false;
			}

			//�o���l�̓����蔻��
			CollisionExperience(pModel, &g_aExp[nCntExp]);
		}
		pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExp->Unlock();
}

//=====================================
//�o���l�̕`�揈��
//=====================================
void DrawExp(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExp;

	D3DXMATRIX mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;				//�r���[�}�g���b�N�X�擾�p

									//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Z�e�X�g�̗L��/�����ݒ�

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);					//�A���t�@�e�X�g�̗L��/�����ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);				//�A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);				//�A���t�@�e�X�g�̎Q�ƒl�ݒ�

	for (nCntExp = 0; nCntExp < MAX_EXP; nCntExp++)
	{
		if (g_aExp[nCntExp].bUse == true)
		{//�G�t�F�N�g���g�p����Ă���
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aExp[nCntExp].mtx);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����Ɍ����Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aExp[nCntExp].mtx, NULL, &mtxView);

			//�t�s������߂�
			g_aExp[nCntExp].mtx._41 = 0.0f;
			g_aExp[nCntExp].mtx._42 = 0.0f;
			g_aExp[nCntExp].mtx._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aExp[nCntExp].pos.x, g_aExp[nCntExp].pos.y, g_aExp[nCntExp].pos.z);
			D3DXMatrixMultiply(&g_aExp[nCntExp].mtx, &g_aExp[nCntExp].mtx, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aExp[nCntExp].mtx);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0,
				g_pVtxBuffExp,									//���_�o�b�t�@�ւ̃|�C���^
				0,
				sizeof(VERTEX_3D));								//���_���\���̂̃T�C�Y

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureExp);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
				4 * nCntExp,									//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);												//�`�悷��v���~�e�B�u��
		}
	}

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			//�A���t�@�e�X�g�̗L��/�����ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		//�A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						//�A���t�@�e�X�g�̎Q�ƒl�ݒ�

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				//Z�e�X�g�̗L��/�����ݒ�

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//========================================
//�o���l�̐ݒ菈��
//========================================
void SetExp(D3DXVECTOR3 pos)
{
	int nCntExp;

	VERTEX_3D * pVtx;					//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExp->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExp = 0; nCntExp < MAX_EXP; nCntExp++)
	{
		if (g_aExp[nCntExp].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			//�ʒu��ݒ�
			g_aExp[nCntExp].pos = pos;

			//�ړ��ʂ�ݒ�
			g_aExp[nCntExp].move = D3DXVECTOR3();

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(0.2f,1.0f,0.1f,1.0f);
			pVtx[1].col = D3DXCOLOR(0.2f,1.0f,0.1f,1.0f);
			pVtx[2].col = D3DXCOLOR(0.2f,1.0f,0.1f,1.0f);
			pVtx[3].col = D3DXCOLOR(0.2f,1.0f,0.1f,1.0f);

			//���a��ݒ�
			g_aExp[nCntExp].fRadius = 5.0f;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aExp[nCntExp].fRadius, +g_aExp[nCntExp].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aExp[nCntExp].fRadius, +g_aExp[nCntExp].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aExp[nCntExp].fRadius, -g_aExp[nCntExp].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aExp[nCntExp].fRadius, -g_aExp[nCntExp].fRadius, 0.0f);

			//������ݒ�
			g_aExp[nCntExp].nLife = 500;

			//�g�p���Ă����Ԃɂ���
			g_aExp[nCntExp].bUse = true;				//�g�p���Ă����Ԃɂ���

			break;
		}
		pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExp->Unlock();
}

//========================================
//�o���l�̓����蔻��
//========================================
void CollisionExperience(Model *pModel, Exp *pExp)
{
	if (pExp->bUse == true)
	{//�o���l���g�p����Ă����ꍇ
		if (pExp->pos.x + pExp->fRadius <= pModel->pos.x + pModel->VtxMaxModel.x&&
			pExp->pos.x + pExp->fRadius >= pModel->pos.x + pModel->VtxMinModel.x&&
			pExp->pos.z + pExp->fRadius <= pModel->pos.z + pModel->VtxMaxModel.z&&
			pExp->pos.z + pExp->fRadius >= pModel->pos.z + pModel->VtxMinModel.z)
		{//�G�����̒��ɓ������ꍇ
			//�o���l�̎擾����
			GetExperience(pModel, pExp);
		}
	}
}

//=========================================
//�o���l�̎擾����
//=========================================
void GetExperience(Model * pModel, Exp *pExp)
{
	//�o���l���擾����
	pExp->bUse = false;

	//���x�����グ��
	pModel->nPoint++;
}