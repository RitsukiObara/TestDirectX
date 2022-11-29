//===========================================
//
//�e�̃��C������[Bullet.cpp]
//Author ��������
//
//===========================================
#include "main.h"
#include "Bullet.h"
#include "Shadow.h"
#include "Effect.h"
#include "wall.h"
#include "Explosion.h"
#include "Polygon.h"

//�}�N����`
#define MAX_BULLET				(128)				//�e�̍ő吔
#define BULLET_RADIUS			(10.0f)				//�e�̔��a
#define BULLET_SPEED			(4.0f)				//�e�̑��x
#define BULLET_EFFECT_RADIUS	(8.0f)				//�e�̃G�t�F�N�g�̔��a
#define BULLET_EFFECT_LIFE		(10)				//�e�̃G�t�F�N�g�̎���
#define BULLET_LIFE				(100)				//�e�̎���

//�v���g�^�C�v�錾
void CollisionWall(Bullet *Bullet);				//�ǂ̓����蔻��
void DeathBullet(Bullet *Bullet);				//�e�̎��S����

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Bullet g_aBullet[MAX_BULLET];					//�e�̏��

//==========================================
//�e�̏���������
//==========================================
void InitBullet(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���n�̃e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bullet000.png",
		&g_pTextureBullet);

	for (int nCntSet = 0; nCntSet < MAX_BULLET; nCntSet++)
	{
		//�ʒu�̏�����
		g_aBullet[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�����̏�����
		g_aBullet[nCntSet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�e�̔ԍ�������������
		g_aBullet[nCntSet].nShadow = INIT_SHADOW;

		//�g�p���Ă��Ȃ�
		g_aBullet[nCntSet].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-BULLET_RADIUS, +BULLET_RADIUS, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+BULLET_RADIUS, +BULLET_RADIUS, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BULLET_RADIUS, -BULLET_RADIUS, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+BULLET_RADIUS, -BULLET_RADIUS, 0.0f);

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
	g_pVtxBuffBullet->Unlock();
}

//========================================
//�e�̏I������
//========================================
void UninitBullet(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//========================================
//�e�̍X�V����
//========================================
void UpdateBullet(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_BULLET; nCntBill++)
	{
		if (g_aBullet[nCntBill].bUse == true)
		{//�g�p����Ă����ꍇ
			//�e�̈ʒu�̐ݒ菈��
			SetPositionShadow(g_aBullet[nCntBill].nShadow, g_aBullet[nCntBill].pos, g_aBullet[nCntBill].rot);

			//���������炷
			g_aBullet[nCntBill].nLife--;

			//��������
			g_aBullet[nCntBill].pos.x += sinf(g_aBullet[nCntBill].rot.y) * BULLET_SPEED;
			g_aBullet[nCntBill].pos.z += cosf(g_aBullet[nCntBill].rot.y) * BULLET_SPEED;

			//�G�t�F�N�g�̐ݒ菈��
			SetEffect(g_aBullet[nCntBill].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BULLET_EFFECT_RADIUS, BULLET_EFFECT_LIFE, EFFECT_BULLET);

			//�ǂ̓����蔻��
			CollisionWall(&g_aBullet[nCntBill]);

			
		}
		pVtx += 4;					//���_�f�[�^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//=====================================
//�e�̕`�揈��
//=====================================
void DrawBullet(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	D3DXMATRIX mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;				//�r���[�}�g���b�N�X�擾�p

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);					//�A���t�@�e�X�g�̗L��/�����ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);				//�A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);				//�A���t�@�e�X�g�̎Q�ƒl�ݒ�

	for (int nCntDraw = 0; nCntDraw < MAX_BULLET; nCntDraw++)
	{
		if (g_aBullet[nCntDraw].bUse == true)
		{//�g�p���Ă����ꍇ
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBullet[nCntDraw].mtx);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����Ɍ����Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aBullet[nCntDraw].mtx, NULL, &mtxView);

			//�t�s������߂�
			g_aBullet[nCntDraw].mtx._41 = 0.0f;
			g_aBullet[nCntDraw].mtx._42 = 0.0f;
			g_aBullet[nCntDraw].mtx._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntDraw].pos.x, g_aBullet[nCntDraw].pos.y, g_aBullet[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCntDraw].mtx, &g_aBullet[nCntDraw].mtx, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntDraw].mtx);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0,
				g_pVtxBuffBullet,								//���_�o�b�t�@�ւ̃|�C���^
				0,
				sizeof(VERTEX_3D));									//���_���\���̂̃T�C�Y

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBullet);

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
}

//======================================
//�e�̐ݒ菈��
//======================================
void SetBullet(D3DXVECTOR3 rot, D3DXVECTOR3 pos)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_BULLET; nCntBill++)
	{
		if (g_aBullet[nCntBill].bUse == false)
		{//�g�p����Ă��Ȃ������ꍇ
			//�ʒu��ݒ肷��
			g_aBullet[nCntBill].pos = pos;

			//������ݒ肷��
			g_aBullet[nCntBill].rot = rot;

			//�e��ݒ肷��
			g_aBullet[nCntBill].nShadow = SetShadow(SHADOW_BULLET);

			//������ݒ肷��
			g_aBullet[nCntBill].nLife = BULLET_LIFE;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-BULLET_RADIUS, +BULLET_RADIUS, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+BULLET_RADIUS, +BULLET_RADIUS, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-BULLET_RADIUS, -BULLET_RADIUS, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+BULLET_RADIUS, -BULLET_RADIUS, 0.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//�g�p����
			g_aBullet[nCntBill].bUse = true;

			break;									//�����o��
		}
		pVtx += 4;									//���_�f�[�^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//================================
//�ǂ̓����蔻��
//================================
void CollisionWall(Bullet *Bullet)
{
	if (Bullet->pos.x >= POLYGON_RADIUS ||
		Bullet->pos.x <= -POLYGON_RADIUS ||
		Bullet->pos.z >= POLYGON_RADIUS ||
		Bullet->pos.z <= -POLYGON_RADIUS)
	{//�e���X�e�[�W�̊O�ɏo���ꍇ
		//�����̃r���{�[�h��ݒ�
		SetExplosion(Bullet->pos);

		//�g�p���Ă��Ȃ�
		Bullet->bUse = false;

		//�e�̃��Z�b�g����
		ResetShadow(&Bullet->nShadow);
	}
}

//==================================
//�e�̎��S����
//==================================
void DeathBullet(Bullet *Bullet)
{
	if (Bullet->nLife <= 0 && Bullet->bUse == true)
	{//������0�ȉ��ɂȂ����ꍇ
		//�����̃r���{�[�h��ݒ�
		SetExplosion(Bullet->pos);

		//�g�p���Ă��Ȃ�
		Bullet->bUse = false;

		//�e�̃��Z�b�g����
		ResetShadow(&Bullet->nShadow);
	}
}