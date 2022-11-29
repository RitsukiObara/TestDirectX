//===========================================
//
//���f���̃��C������[model.cpp]
//Author ��������
//
//===========================================
#include "main.h"
#include "model.h"
#include "input.h"
#include "Shadow.h"
#include "camera.h"
#include "Bullet.h"
#include "Effect.h"
#include "Tower.h"
#include "Enemy.h"

//�}�N����`
#define MAX_TEXTURE					(10)			//�e�N�X�`���̍ő吔
#define PLAYER_SPEED				(7.0f)			//���f���̈ړ�����

//�v���g�^�C�v�錾
void CollisionPlayer(Model *pModel);				//�v���C���[�̓����蔻��
void CollisionPlayerAttack(Model *pModel);			//�v���C���[�̍U�����菈��

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureModel[MAX_TEXTURE] = {};		//�e�N�X�`���ւ̃|�C���^
LPD3DXMESH g_pMeshModel = NULL;						//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatModel = NULL;				//�}�e���A���ւ̃|�C���^
DWORD g_dwNumMatModel = 0;							//�}�e���A���̐�
Model g_Model;										//���f���̏��
D3DXVECTOR3 Camerarot;								//�J�����̌���

//==========================================
//���f���̏���������
//==========================================
void InitModel(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nNumVtx;							//���_��
	DWORD dwSizeFVF;						//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;							//���_�o�b�t�@�ւ̃|�C���^

	////���n�̃e�N�X�`���̓ǂݍ���
	//D3DXCreateTextureFromFile(pDevice,
	//	"data/TEXTURE/�W���W�� D4C.png",
	//	&g_pTexturePolygon);

	//���f���̈ʒu������������
	g_Model.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���f���̑O��̈ʒu������������
	g_Model.posOld = g_Model.pos;

	//���f���̈ړ��ʂ�����������
	g_Model.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���f���̌���������������
	g_Model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�g�p���Ă���
	g_Model.bUse = true;

	//�e�̃C���f�b�N�X(�e�̔ԍ�)������������
	g_Model.nShadow = -1;

	//�J�����̌���������������
	Camerarot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�ڕW�̌���������������
	g_Model.fRotDest = Camerarot.y;

	//�����̍���������������
	g_Model.fRotDiff = 0.0f;

	//�ړ����Ă��Ȃ�
	g_Model.bMove = false;

	//���x��������������
	g_Model.nPoint = 0;

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/MODEL/Player.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel,
		NULL,
		&g_dwNumMatModel,
		&g_pMeshModel);

	//���_�����擾
	nNumVtx = g_pMeshModel->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshModel->GetFVF());

	//���_�o�b�t�@�����b�N
	g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//���_���W�̑��
	g_Model.VtxMaxModel = *(D3DXVECTOR3*)pVtxBuff;
	g_Model.VtxMinModel = *(D3DXVECTOR3*)pVtxBuff;

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (vtx.x < g_Model.VtxMinModel.x)
		{//��r�Ώۂ�X���W�����i�K�̍ŏ��l�����������ꍇ
			//�ŏ��l���X�V����
			g_Model.VtxMinModel.x = vtx.x;
		}
		else if (vtx.x > g_Model.VtxMaxModel.x)
		{//��r�Ώۂ�X���W�����i�K�̍ő�l�����傫���ꍇ
			//�ő�l���X�V����
			g_Model.VtxMaxModel.x = vtx.x;
		}

		if (vtx.y < g_Model.VtxMinModel.y)
		{//��r�Ώۂ�Y���W�����i�K�̍ŏ��l�����������ꍇ
			//�ŏ��l���X�V����
			g_Model.VtxMinModel.y = vtx.y;
		}
		else if (vtx.y > g_Model.VtxMaxModel.y)
		{//��r�Ώۂ�Y���W�����i�K�̍ő�l�����傫���ꍇ
			//�ő�l���X�V����
			g_Model.VtxMaxModel.y = vtx.y;
		}

		if (vtx.z < g_Model.VtxMinModel.z)
		{//��r�Ώۂ�Z���W�����i�K�̍ŏ��l�����������ꍇ
			//�ŏ��l���X�V����
			g_Model.VtxMinModel.z = vtx.z;
		}
		else if (vtx.z > g_Model.VtxMaxModel.z)
		{//��r�Ώۂ�Z���W�����i�K�̍ő�l�����傫���ꍇ
			//�ő�l���X�V����
			g_Model.VtxMaxModel.z = vtx.z;
		}

		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += dwSizeFVF;
	}

	D3DXMATERIAL *pMat;					//�}�e���A���ւ̃|�C���^

	//�}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C���������݂���
			//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				"data/TEXTURE/PlayerFace.png",
				&g_apTextureModel[nCntMat]);
		}

	}

	//�e�̐ݒ菈��
	g_Model.nShadow = SetShadow(SHADOW_PLAYER);
}

//========================================
//���f���̏I������
//========================================
void UninitModel(void)
{
	for (int nCntMat = 0; nCntMat < MAX_TEXTURE; nCntMat++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureModel[nCntMat] != NULL)
		{
			g_apTextureModel[nCntMat]->Release();
			g_apTextureModel[nCntMat] = NULL;
		}
	}

	//���b�V���̔j��
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//========================================
//���f���̍X�V����
//========================================
void UpdateModel(void)
{
	//�J�����̌��������
	D3DXVECTOR3 Camerarot = GetCamera().rot;

	//���f���̑O��̈ʒu���L�^����
	g_Model.posOld = g_Model.pos;

	if (GetKeyboardPress(DIK_LEFT) == true)
	{//���L�[�������ꂽ
		if (GetKeyboardPress(DIK_UP) == true)
		{//��L�[�������ꂽ
			//����ɉ�������
			g_Model.move.x = sinf(-D3DX_PI * 0.75f - Camerarot.y) * PLAYER_SPEED;
			g_Model.move.z = -cosf(-D3DX_PI * 0.75f - Camerarot.y) * PLAYER_SPEED;

			g_Model.fRotDest = (-D3DX_PI * 0.25f + Camerarot.y);
		}
		else if (GetKeyboardPress(DIK_DOWN) == true)
		{//���L�[�������ꂽ
			//�����ɉ�������
			g_Model.move.x = sinf(-D3DX_PI * 0.25f - Camerarot.y) * PLAYER_SPEED;
			g_Model.move.z = -cosf(-D3DX_PI * 0.25f - Camerarot.y) * PLAYER_SPEED;

			g_Model.fRotDest = (-D3DX_PI * 0.75f + Camerarot.y);
		}
		else
		{//���L�[�����������ꂽ
			//���ɉ�������
			g_Model.move.x = sinf(-D3DX_PI * 0.5f - Camerarot.y) * PLAYER_SPEED;
			g_Model.move.z = -cosf(-D3DX_PI * 0.5f - Camerarot.y) * PLAYER_SPEED;

			g_Model.fRotDest = (-D3DX_PI * 0.5f + Camerarot.y);
		}

		//�ړ����Ă���
		g_Model.bMove = true;
	}
	else if (GetKeyboardPress(DIK_RIGHT) == true)
	{//�E�L�[�������ꂽ
		if (GetKeyboardPress(DIK_UP) == true)
		{//��L�[�������ꂽ
			//�E��ɉ�������
			g_Model.move.x = sinf(D3DX_PI * 0.75f- Camerarot.y) * PLAYER_SPEED;
			g_Model.move.z = -cosf(D3DX_PI * 0.75f- Camerarot.y) * PLAYER_SPEED;

			g_Model.fRotDest = (D3DX_PI * 0.25f + Camerarot.y);
		}
		else if (GetKeyboardPress(DIK_DOWN) == true)
		{//���L�[�������ꂽ
			//�E���ɉ�������
			g_Model.move.x = sinf(D3DX_PI * 0.25f- Camerarot.y) * PLAYER_SPEED;
			g_Model.move.z = -cosf(D3DX_PI * 0.25f- Camerarot.y) * PLAYER_SPEED;

			g_Model.fRotDest = (D3DX_PI * 0.75f + Camerarot.y);
		}
		else
		{//�E�L�[�����������ꂽ
			//�E�ɉ������Ă���
			g_Model.move.x = sinf(D3DX_PI * 0.5f - Camerarot.y) * PLAYER_SPEED;
			g_Model.move.z = -cosf(D3DX_PI * 0.5f - Camerarot.y) * PLAYER_SPEED;

			g_Model.fRotDest = (D3DX_PI * 0.5f + Camerarot.y);
		}

		//�ړ����Ă���
		g_Model.bMove = true;
	}
	else if (GetKeyboardPress(DIK_UP) == true)
	{//��L�[�������ꂽ
		//��ɉ�������
		g_Model.move.x = sinf(Camerarot.y) * PLAYER_SPEED;
		g_Model.move.z = cosf(Camerarot.y) * PLAYER_SPEED;

		g_Model.fRotDest = (Camerarot.y);

		//�ړ����Ă���
		g_Model.bMove = true;
	}
	else if (GetKeyboardPress(DIK_DOWN) == true)
	{//���L�[�������ꂽ
		//���ɉ�������
		g_Model.move.x = -sinf(Camerarot.y) * PLAYER_SPEED;
		g_Model.move.z = -cosf(Camerarot.y) * PLAYER_SPEED;

		g_Model.fRotDest = (D3DX_PI + Camerarot.y);

		//�ړ����Ă���
		g_Model.bMove = true;
	}

	//�ڕW�̊p�x�ւ̍��������߂�
	g_Model.fRotDiff = g_Model.fRotDest - g_Model.rot.y;

	if (g_Model.fRotDiff > D3DX_PI)
	{//�p�x��3.14f���傫�������ꍇ
		//�p�x����1�������炷
		g_Model.fRotDiff -= (2 * D3DX_PI);
	}
	else if (g_Model.fRotDiff < -D3DX_PI)
	{//�p�x��-3.14f��菬���������ꍇ
		//�p�x��1����������
		g_Model.fRotDiff += (2 * D3DX_PI);
	}

	//�p�x��␳����
	g_Model.rot.y += g_Model.fRotDiff * 0.08f;

	if (g_Model.rot.y > D3DX_PI)
	{//�p�x��3.14f���傫�������ꍇ
		//�p�x����1�������炷
		g_Model.rot.y = -D3DX_PI;
	}
	else if (g_Model.rot.y < -D3DX_PI)
	{//�p�x��-3.14f��菬���������ꍇ
		//�p�x��1����������
		g_Model.rot.y = D3DX_PI;
	}

	//if (g_Model.rot.y > D3DX_PI)
	//{//3.14f���傫���Ȃ����ꍇ
	//	//-3.14f�ɕ␳����
	//	g_Model.rot.y = -D3DX_PI;
	//}
	//else if (g_Model.rot.y < -D3DX_PI)
	//{//-3.14f��菬�����Ȃ����ꍇ
	//	//3.14f�ɕ␳����
	//	g_Model.rot.y = D3DX_PI;
	//}

	if (GetKeyboardTrigger(DIK_0) == true)
	{//0���������ꍇ
		//�e�̐ݒ菈��
		SetBullet(g_Model.rot, D3DXVECTOR3(g_Model.pos.x, g_Model.pos.y + 10.0f, g_Model.pos.z));
	}

	if (g_Model.bMove == true)
	{//�ړ����Ă���ꍇ
		//�����Ă���G�t�F�N�g���o��
		SetEffect(g_Model.pos, D3DXVECTOR3((-g_Model.move.x * CENTER_MAGNI), 2.0f, (-g_Model.move.z * CENTER_MAGNI)), 15.0f, 10, EFFECT_WALK);
	}

	//�ړ�������
	g_Model.pos += g_Model.move;

	//�ړ��ʂ��X�V(��������)
	g_Model.move.x += (0.0f - g_Model.move.x) * 0.07f;
	g_Model.move.z += (0.0f - g_Model.move.z) * 0.07f;

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//ENTER�L�[���������ꍇ
		//�|���S���̈ʒu������������
		g_Model.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//���f���̈ړ��ʂ�����������
		g_Model.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�|���S���̌���������������
		g_Model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�ڕW�̌���������������
		g_Model.fRotDest = Camerarot.y;

		//�����̍���������������
		g_Model.fRotDiff = 0.0f;
	}

	//�ړ����Ă��Ȃ��悤�ɏ���������
	g_Model.bMove = false;

	//�v���C���[�̓����蔻��
	CollisionPlayer(&g_Model);

	//�v���C���[�Ɠ��̓����蔻��
	CollisionTower(g_Model.VtxMaxModel, g_Model.VtxMinModel, &g_Model.pos, &g_Model.posOld, &g_Model.move, COLL_PLAYER);

	//�v���C���[�̍U�����菈��
	CollisionPlayerAttack(&g_Model);			

	//�e�̈ʒu�ݒ菈��
	SetPositionShadow(g_Model.nShadow, g_Model.pos, g_Model.rot);
}

//=====================================
//���f���̕`�揈��
//=====================================
void DrawModel(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Model.mtxModel);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Model.rot.y, g_Model.rot.x, g_Model.rot.z);
	D3DXMatrixMultiply(&g_Model.mtxModel, &g_Model.mtxModel, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_Model.pos.x, g_Model.pos.y, g_Model.pos.z);
	D3DXMatrixMultiply(&g_Model.mtxModel, &g_Model.mtxModel, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Model.mtxModel);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureModel[nCntMat]);

		//���f��(�p�[�c)�̕`��
		g_pMeshModel->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=======================================
//���f���̍��W�̎擾����
//=======================================
Model *GetModel(void)
{
	//���f���̏���Ԃ�
	return &g_Model;
}

//=======================================
//�v���C���[�̓����蔻��
//=======================================
void CollisionPlayer(Model *pModel)
{
	if (pModel->pos.x >= 570.0f)
	{//�E�̕ǂɂԂ������ꍇ
		//�E����o�Ȃ��悤�ɂ���
		pModel->pos.x = 570.0f;
	}

	if (pModel->pos.x <= -570.0f)
	{//���̕ǂɂԂ������ꍇ
		//������o�Ȃ��悤�ɂ���
		pModel->pos.x = -570.0f;
	}

	if (pModel->pos.z >= 570.0f)
	{//���̕ǂɂԂ������ꍇ
		//������o�Ȃ��悤�ɂ���
		pModel->pos.z = 570.0f;
	}

	if (pModel->pos.z <= -570.0f)
	{//��O�̕ǂɂԂ������ꍇ
		//��O����o�Ȃ��悤�ɂ���
		pModel->pos.z = -570.0f;
	}
}

//=======================================
//�v���C���[�̍U�����菈��
//=======================================
void CollisionPlayerAttack(Model *pModel)
{
	Enemy *pEnemy = GetEnemy();

	for (int nCntEnm = 0; nCntEnm < MAX_ENEMY; nCntEnm++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//�G���g�p����Ă����ꍇ
			if (pModel->pos.x + pModel->VtxMinModel.x <= pEnemy->pos.x + pEnemy->VtxMaxEnemy.x && pModel->pos.x + pModel->VtxMaxModel.x >= pEnemy->pos.x + pEnemy->VtxMinEnemy.x)
			{//����X���̒��ɂ����ꍇ
				if (pModel->posOld.z + pModel->VtxMaxModel.z <= pEnemy->posOld.z + pEnemy->VtxMinEnemy.z && pModel->pos.z + pModel->VtxMaxModel.z >= pEnemy->pos.z + pEnemy->VtxMinEnemy.z)
				{//�O��̈ʒu���u���b�N����O���A���݂̈ʒu���u���b�N���������������ꍇ(��O�Ŏ~�߂��鏈��)
					//��O�Ŏ~�߂�
					pModel->pos.z = pEnemy->pos.z + pEnemy->VtxMinEnemy.z - pModel->VtxMaxModel.z;
				}							//��O�Ŏ~�߂��鏈��
				else if (pModel->posOld.z + pModel->VtxMinModel.z >= pEnemy->posOld.z + pEnemy->VtxMaxEnemy.z && pModel->pos.z + pModel->VtxMinModel.z <= pEnemy->pos.z + pEnemy->VtxMaxEnemy.z)
				{//�O��̈ʒu�����̈ʒu���������A���݂̈ʒu�����̈ʒu������O�������ꍇ(���Ŏ~�߂��鏈��)
					//���Ŏ~�߂�
					pModel->pos.z = pEnemy->pos.z + pEnemy->VtxMaxEnemy.z - pModel->VtxMinModel.z;
				}							//���Ŏ~�߂��鏈��
			}

			if (pModel->pos.z + pModel->VtxMinModel.z <= pEnemy->pos.z + pEnemy->VtxMaxEnemy.z && pModel->pos.z + pModel->VtxMaxModel.z >= pEnemy->pos.z + pEnemy->VtxMinEnemy.z)
			{//����Z���̒��ɂ����ꍇ
				if (pModel->posOld.x + pModel->VtxMaxModel.x <= pEnemy->posOld.x + pEnemy->VtxMinEnemy.x && pModel->pos.x + pModel->VtxMaxModel.x >= pEnemy->pos.x + pEnemy->VtxMinEnemy.x)
				{//�O��̈ʒu���u���b�N�̍��[��荶���A���݂̈ʒu���u���b�N�̍������E�������ꍇ(���̏���)
					//�u���b�N�̍��[�ɒ���t������
					pModel->pos.x = pEnemy->pos.x + pEnemy->VtxMinEnemy.x - pModel->VtxMaxModel.x;
				}							//���[�̏���
				else if (pModel->posOld.x + pModel->VtxMinModel.x >= pEnemy->posOld.x + pEnemy->VtxMaxEnemy.x && pModel->pos.x + pModel->VtxMinModel.x <= pEnemy->pos.x + pEnemy->VtxMaxEnemy.x)
				{//�O��̈ʒu���u���b�N�̉E�[���E���A���݂̈ʒu���u���b�N�̍������E�������ꍇ(�E�̏���)
					//�u���b�N�̉E�[�ɒ���t������
					pModel->pos.x = pEnemy->pos.x + pEnemy->VtxMaxEnemy.x - pModel->VtxMinModel.x;
				}							//�E�[�̏���								//�W�����v���Ă��Ȃ�������
			}
		}
	}
}