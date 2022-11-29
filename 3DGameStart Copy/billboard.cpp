//===========================================
//
//�r���{�[�h�̃��C������[Billboard.cpp]
//Author ��������
//
//===========================================
#include"main.h"
#include "billboard.h"
#include "input.h"
#include "Shadow.h"
#include "Effect.h"
#include "Particle.h"
#include "wall.h"

//�}�N����`
#define MAX_BILLBOARD			(256)				//�r���{�[�h�̍ő吔

//�v���g�^�C�v�錾
void CollisionWall(Billboard *Billboard);		//�ǂ̓����蔻��

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBillboard[BILLBOARD_MAX] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Billboard g_aBillboard[MAX_BILLBOARD];				//�r���{�[�h�̏��

//�e�N�X�`���t�@�C����
const char *c_apFilenameBillboard[BILLBOARD_MAX] = 
{
	"data/TEXTURE/explosion000.png",				//�����̃e�N�X�`��
	"data/TEXTURE/bullet000.png"					//�e�̃e�N�X�`��
};

//==========================================
//�r���{�[�h�̏���������
//==========================================
void InitBillboard(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntTexture = 0; nCntTexture < BILLBOARD_MAX; nCntTexture++)
	{//�e�N�X�`���̐ݒ�
		//���n�̃e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameBillboard[nCntTexture],
			&g_pTextureBillboard[nCntTexture]);
	}
	
	for (int nCntSet = 0; nCntSet < MAX_BILLBOARD; nCntSet++)
	{
		//�ʒu�̏�����
		g_aBillboard[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�����̏�����
		g_aBillboard[nCntSet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�e�̔ԍ�������������
		g_aBillboard[nCntSet].nShadow = -1;

		//�A�j���J�E���^�[������������
		g_aBillboard[nCntSet].nAnimCounter = 0;

		//�A�j���p�^�[��No.������������
		g_aBillboard[nCntSet].nAnimPattern = 0;

		//�g�p���Ă��Ȃ�
		g_aBillboard[nCntSet].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-10.0f, +10.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+10.0f, +10.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-10.0f, -10.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+10.0f, -10.0f, 0.0f);

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

		pVtx += 4;						//���_�f�[�^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBillboard->Unlock();
}

//========================================
//�r���{�[�h�̏I������
//========================================
void UninitBillboard(void)
{
	for (int nCntBill = 0; nCntBill < BILLBOARD_MAX; nCntBill++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureBillboard[nCntBill] != NULL)
		{
			g_pTextureBillboard[nCntBill]->Release();
			g_pTextureBillboard[nCntBill] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//========================================
//�r���{�[�h�̍X�V����
//========================================
void UpdateBillboard(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		if (g_aBillboard[nCntBill].bUse == true)
		{//�g�p����Ă����ꍇ
			//�e�̈ʒu�̐ݒ菈��
			SetPositionShadow(g_aBillboard[nCntBill].nShadow, g_aBillboard[nCntBill].pos, g_aBillboard[nCntBill].rot);

			//���������炷
			g_aBillboard[nCntBill].nLife--;

			switch (g_aBillboard[nCntBill].Type)
			{
			case BILLBOARD_BULLET:				//�e�̏ꍇ

				//��ɉ�������
				g_aBillboard[nCntBill].pos.x += sinf(g_aBillboard[nCntBill].rot.y) * 4.0f;
				g_aBillboard[nCntBill].pos.z += cosf(g_aBillboard[nCntBill].rot.y) * 4.0f;

				//�G�t�F�N�g�̐ݒ菈��
				SetEffect(g_aBillboard[nCntBill].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 8.0f, 10, EFFECT_BULLET);

				//�ǂ̓����蔻��
				CollisionWall(&g_aBillboard[nCntBill]);

				break;							//�����o��

			case BILLBOARD_EXPL:				//�����̏ꍇ

				//�A�j���J�E���^�[�����Z����
				g_aBillboard[nCntBill].nAnimCounter++;

				if (g_aBillboard[nCntBill].nAnimCounter == 6)
				{//�A�j���[�V�����J�E���^�[��6�ɂȂ����ꍇ
					//�A�j���J�E���^�[��0�ɂ���
					g_aBillboard[nCntBill].nAnimCounter = 0;

					//�A�j���[�V�����p�^�[��No.�����Z����
					g_aBillboard[nCntBill].nAnimPattern = (g_aBillboard[nCntBill].nAnimPattern + 1) % 8;
				}

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f + (g_aBillboard[nCntBill].nAnimPattern * 0.125f), 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.125f + (g_aBillboard[nCntBill].nAnimPattern * 0.125f), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + (g_aBillboard[nCntBill].nAnimPattern * 0.125f), 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.125f + (g_aBillboard[nCntBill].nAnimPattern * 0.125f), 1.0f);

				break;							//�����o��
			}

			if (g_aBillboard[nCntBill].nLife <= 0 && g_aBillboard[nCntBill].bUse == true)
			{//������0�ȉ��ɂȂ����ꍇ
				if (g_aBillboard[nCntBill].Type != BILLBOARD_EXPL)
				{//�����̃r���{�[�h�ȊO�̏ꍇ
					//�����̃r���{�[�h��ݒ�
					SetBillboard(g_aBillboard[nCntBill].rot, g_aBillboard[nCntBill].pos, BILLBOARD_EXPL);

					//�g�p���Ă��Ȃ�
					g_aBillboard[nCntBill].bUse = false;

					//�e�̃��Z�b�g����
					ResetShadow(&g_aBillboard[nCntBill].nShadow);
				}
				else
				{//�����̃r���{�[�h�̏ꍇ
					//�g�p���Ă��Ȃ�
					g_aBillboard[nCntBill].bUse = false;
				}
			}
		}
		pVtx += 4;					//���_�f�[�^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBillboard->Unlock();
}

//=====================================
//�r���{�[�h�̕`�揈��
//=====================================
void DrawBillboard(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	D3DXMATRIX mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;				//�r���[�}�g���b�N�X�擾�p

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Z�e�X�g�̗L��/�����ݒ�

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);					//�A���t�@�e�X�g�̗L��/�����ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);				//�A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 75);							//�A���t�@�e�X�g�̎Q�ƒl�ݒ�

	for (int nCntDraw = 0; nCntDraw < MAX_BILLBOARD; nCntDraw++)
	{
		if (g_aBillboard[nCntDraw].bUse == true)
		{//�g�p���Ă����ꍇ
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBillboard[nCntDraw].mtx);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����Ɍ����Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aBillboard[nCntDraw].mtx, NULL, &mtxView);

			//�t�s������߂�
			g_aBillboard[nCntDraw].mtx._41 = 0.0f;
			g_aBillboard[nCntDraw].mtx._42 = 0.0f;
			g_aBillboard[nCntDraw].mtx._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCntDraw].pos.x, g_aBillboard[nCntDraw].pos.y, g_aBillboard[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_aBillboard[nCntDraw].mtx, &g_aBillboard[nCntDraw].mtx, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCntDraw].mtx);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0,
				g_pVtxBuffBillboard,								//���_�o�b�t�@�ւ̃|�C���^
				0,
				sizeof(VERTEX_3D));									//���_���\���̂̃T�C�Y

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBillboard[g_aBillboard[nCntDraw].Type]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//�v���~�e�B�u�̎��
				4 * nCntDraw,										//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);													//�`�悷��v���~�e�B�u��
		}
	}

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			//�A���t�@�e�X�g�̗L��/�����ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		//�A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						//�A���t�@�e�X�g�̎Q�ƒl�ݒ�

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				//Z�e�X�g�̗L��/�����ݒ�
}

//======================================
//�r���{�[�h�̐ݒ菈��
//======================================
void SetBillboard(D3DXVECTOR3 rot, D3DXVECTOR3 pos, int nType)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		if (g_aBillboard[nCntBill].bUse == false)
		{//�g�p����Ă��Ȃ������ꍇ
			//�ʒu��ݒ肷��
			g_aBillboard[nCntBill].pos = pos;

			//������ݒ肷��
			g_aBillboard[nCntBill].rot = rot;

			//��ނ�ݒ肷��
			g_aBillboard[nCntBill].Type = nType;

			//�A�j���J�E���^�[��ݒ肷��
			g_aBillboard[nCntBill].nAnimCounter = 0;

			//�A�j���p�^�[��No.��ݒ肷��
			g_aBillboard[nCntBill].nAnimPattern = 0;

			if (g_aBillboard[nCntBill].Type != BILLBOARD_EXPL)
			{//�����̃r���{�[�h�ȊO�������ꍇ
				//�e��ݒ肷��
				g_aBillboard[nCntBill].nShadow = SetShadow(SHADOW_BULLET);
			}

			switch (g_aBillboard[nCntBill].Type)
			{
			case BILLBOARD_BULLET:					//�e�̏ꍇ

				//������ݒ肷��
				g_aBillboard[nCntBill].nLife = 100;

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(-10.0f, 10.0f, -0.0f);
				pVtx[1].pos = D3DXVECTOR3(10.0f, 10.0f, -0.0f);
				pVtx[2].pos = D3DXVECTOR3(-10.0f, -10.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(10.0f, -10.0f, 0.0f);

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				break;								//�����o��

			case BILLBOARD_EXPL:					//�����������ꍇ

				//������ݒ肷��
				g_aBillboard[nCntBill].nLife = 60;

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(-30.0f, 30.0f, -0.0f);
				pVtx[1].pos = D3DXVECTOR3(30.0f, 30.0f, -0.0f);
				pVtx[2].pos = D3DXVECTOR3(-30.0f, -30.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(30.0f, -30.0f, 0.0f);

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f + (g_aBillboard[nCntBill].nAnimPattern * 0.125f), 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.125f + (g_aBillboard[nCntBill].nAnimPattern * 0.125f), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + (g_aBillboard[nCntBill].nAnimPattern * 0.125f), 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.125f + (g_aBillboard[nCntBill].nAnimPattern * 0.125f), 1.0f);

				//�p�[�e�B�N���̐ݒ菈��
				SetParticle(g_aBillboard[nCntBill].pos, PTCL_EXPL);

				break;								//�����o��
			}

			//�g�p����
			g_aBillboard[nCntBill].bUse = true;

			break;									//�����o��
		}
		pVtx += 4;									//���_�f�[�^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBillboard->Unlock();
}

//================================
//�ǂ̓����蔻��
//================================
void CollisionWall(Billboard *Billboard)
{
	if (Billboard->pos.x >= 300.0f ||
		Billboard->pos.x <= -300.0f ||
		Billboard->pos.z >= 300.0f ||
		Billboard->pos.z <= -300.0f)
	{//�e���X�e�[�W�̊O�ɏo���ꍇ
		//�����̃r���{�[�h��ݒ�
		SetBillboard(Billboard->rot, Billboard->pos, BILLBOARD_EXPL);

		//�g�p���Ă��Ȃ�
		Billboard->bUse = false;

		//�e�̃��Z�b�g����
		ResetShadow(&Billboard->nShadow);
	}
}