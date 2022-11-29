//===========================================
//
//�����̃��C������[Explosion.cpp]
//Author ��������
//
//===========================================
#include"main.h"
#include "Explosion.h"
#include "Particle.h"

//�}�N����`
#define MAX_EXPLOSION			(128)				//�r���{�[�h�̍ő吔
#define EXPL_RADIUS				(30.0f)				//�����̔��a
#define EXPL_ANIM_COUNT			(6)					//�����A�j���[�V�����̃J�E���g
#define EXPL_ANIM_PATTERN		(0.125f)			//�����A�j���[�V�����̃p�^�[����
#define EXPL_LIFE				(60)				//�����̎���

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Explosion g_aExplosion[MAX_EXPLOSION];				//�r���{�[�h�̏��

//==========================================
//�����̏���������
//==========================================
void InitExplosion(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//���n�̃e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/explosion000.png",
		&g_pTextureExplosion);

	for (int nCntSet = 0; nCntSet < MAX_EXPLOSION; nCntSet++)
	{
		//�ʒu�̏�����
		g_aExplosion[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�A�j���J�E���^�[������������
		g_aExplosion[nCntSet].nAnimCounter = 0;

		//�A�j���p�^�[��No.������������
		g_aExplosion[nCntSet].nAnimPattern = 0;

		//�g�p���Ă��Ȃ�
		g_aExplosion[nCntSet].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-EXPL_RADIUS, +EXPL_RADIUS, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+EXPL_RADIUS, +EXPL_RADIUS, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-EXPL_RADIUS, -EXPL_RADIUS, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+EXPL_RADIUS, -EXPL_RADIUS, 0.0f);

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
	g_pVtxBuffExplosion->Unlock();
}

//========================================
//�r���{�[�h�̏I������
//========================================
void UninitExplosion(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//========================================
//�����̍X�V����
//========================================
void UpdateExplosion(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_EXPLOSION; nCntBill++)
	{
		if (g_aExplosion[nCntBill].bUse == true)
		{//�g�p����Ă����ꍇ
			//���������炷
			g_aExplosion[nCntBill].nLife--;

			//�A�j���J�E���^�[�����Z����
			g_aExplosion[nCntBill].nAnimCounter++;

			if (g_aExplosion[nCntBill].nAnimCounter == EXPL_ANIM_COUNT)
			{//�A�j���[�V�����J�E���^�[��6�ɂȂ����ꍇ
				//�A�j���J�E���^�[��0�ɂ���
				g_aExplosion[nCntBill].nAnimCounter = 0;

				//�A�j���[�V�����p�^�[��No.�����Z����
				g_aExplosion[nCntBill].nAnimPattern = (g_aExplosion[nCntBill].nAnimPattern + 1) % 8;
			}

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f + (g_aExplosion[nCntBill].nAnimPattern * EXPL_ANIM_PATTERN), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(EXPL_ANIM_PATTERN + (g_aExplosion[nCntBill].nAnimPattern * EXPL_ANIM_PATTERN), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (g_aExplosion[nCntBill].nAnimPattern * EXPL_ANIM_PATTERN), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(EXPL_ANIM_PATTERN + (g_aExplosion[nCntBill].nAnimPattern * EXPL_ANIM_PATTERN), 1.0f);

			if (g_aExplosion[nCntBill].nLife <= 0 && g_aExplosion[nCntBill].bUse == true)
			{//������0�ȉ��ɂȂ����ꍇ
				 //�g�p���Ă��Ȃ�
				g_aExplosion[nCntBill].bUse = false;
			}
		}
		pVtx += 4;					//���_�f�[�^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//=====================================
//�����̕`�揈��
//=====================================
void DrawExplosion(void)
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
	pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);				//�A���t�@�e�X�g�̎Q�ƒl�ݒ�

	for (int nCntDraw = 0; nCntDraw < MAX_EXPLOSION; nCntDraw++)
	{
		if (g_aExplosion[nCntDraw].bUse == true)
		{//�g�p���Ă����ꍇ
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aExplosion[nCntDraw].mtx);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����Ɍ����Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aExplosion[nCntDraw].mtx, NULL, &mtxView);

			//�t�s������߂�
			g_aExplosion[nCntDraw].mtx._41 = 0.0f;
			g_aExplosion[nCntDraw].mtx._42 = 0.0f;
			g_aExplosion[nCntDraw].mtx._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aExplosion[nCntDraw].pos.x, g_aExplosion[nCntDraw].pos.y, g_aExplosion[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_aExplosion[nCntDraw].mtx, &g_aExplosion[nCntDraw].mtx, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aExplosion[nCntDraw].mtx);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0,
				g_pVtxBuffExplosion,								//���_�o�b�t�@�ւ̃|�C���^
				0,
				sizeof(VERTEX_3D));									//���_���\���̂̃T�C�Y

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureExplosion);

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
//�����̐ݒ菈��
//======================================
void SetExplosion(D3DXVECTOR3 pos)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_EXPLOSION; nCntBill++)
	{
		if (g_aExplosion[nCntBill].bUse == false)
		{//�g�p����Ă��Ȃ������ꍇ
			//�ʒu��ݒ肷��
			g_aExplosion[nCntBill].pos = pos;

			//�A�j���J�E���^�[��ݒ肷��
			g_aExplosion[nCntBill].nAnimCounter = 0;

			//�A�j���p�^�[��No.��ݒ肷��
			g_aExplosion[nCntBill].nAnimPattern = 0;

			//������ݒ肷��
			g_aExplosion[nCntBill].nLife = EXPL_LIFE;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-EXPL_RADIUS, +EXPL_RADIUS, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+EXPL_RADIUS, +EXPL_RADIUS, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-EXPL_RADIUS, -EXPL_RADIUS, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+EXPL_RADIUS, -EXPL_RADIUS, 0.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f + (g_aExplosion[nCntBill].nAnimPattern * 0.125f), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f + (g_aExplosion[nCntBill].nAnimPattern * 0.125f), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (g_aExplosion[nCntBill].nAnimPattern * 0.125f), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f + (g_aExplosion[nCntBill].nAnimPattern * 0.125f), 1.0f);

			//�p�[�e�B�N���̐ݒ菈��
			SetParticle(g_aExplosion[nCntBill].pos, PTCL_EXPL);

			//�g�p����
			g_aExplosion[nCntBill].bUse = true;

			break;									//�����o��
		}
		pVtx += 4;									//���_�f�[�^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}