//===========================================
//
//���f���̃��C������[model.cpp]
//Author ��������
//
//===========================================
#include "main.h"
#include "Tower.h"
#include "Shadow.h"
#include "input.h"
#include "Enemy.h"

//�}�N����`
#define PLAYER_SPEED				(10.0f)			//���f���̈ړ�����
#define MAX_MATERIAL				(32)			//�}�e���A���̎��

//�v���g�^�C�v�錾
void HitTower(void);								//�^���[�̃_���[�W����

//�O���[�o���ϐ�
LPD3DXMESH g_pMeshTower = NULL;						//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatTower = NULL;				//�}�e���A���ւ̃|�C���^
DWORD g_dwNumMatTower = 0;							//�}�e���A���̐�
Tower g_Tower;										//���f���̏��
D3DXMATERIAL MatCopy[MAX_MATERIAL] = {};			//�}�e���A���̃R�s�[

//==========================================
//���̏���������
//==========================================
void InitTower(void)
{
	int nNumVtx;							//���_��
	DWORD dwSizeFVF;						//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;							//���_�o�b�t�@�ւ̃|�C���^

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���f���̈ʒu������������
	g_Tower.pos = D3DXVECTOR3(450.0f, 0.0f, 450.0f);

	//���f���̌���������������
	g_Tower.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�g�p���Ă���
	g_Tower.bUse = true;

	//�e�̃C���f�b�N�X(�e�̔ԍ�)������������
	g_Tower.nShadow = -1;

	//�ʏ���
	g_Tower.TowerState = TOWER_NORMAL;

	//���̗̑͂�����������
	g_Tower.nLife = 60;

	//�_���[�W�J�E���g������������
	g_Tower.nDamageCount = 0;

	//���G�J�E���g������������
	g_Tower.nInvincibleCount = 0;

	//�_�ł��Ȃ�
	g_Tower.bFlash = false;

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/MODEL/Tower.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatTower,
		NULL,
		&g_dwNumMatTower,
		&g_pMeshTower);

	//���_�����擾
	nNumVtx = g_pMeshTower->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshTower->GetFVF());

	//���_�o�b�t�@�����b�N
	g_pMeshTower->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//���_���W�̑��
	g_Tower.VtxMaxTower = *(D3DXVECTOR3*)pVtxBuff;
	g_Tower.VtxMinTower = *(D3DXVECTOR3*)pVtxBuff;

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (vtx.x < g_Tower.VtxMinTower.x)
		{//��r�Ώۂ�X���W�����i�K�̍ŏ��l�����������ꍇ
			//�ŏ��l���X�V����
			g_Tower.VtxMinTower.x = vtx.x;
		}
		else if (vtx.x > g_Tower.VtxMaxTower.x)
		{//��r�Ώۂ�X���W�����i�K�̍ő�l�����傫���ꍇ
			//�ő�l���X�V����
			g_Tower.VtxMaxTower.x = vtx.x;
		}

		if (vtx.y < g_Tower.VtxMinTower.y)
		{//��r�Ώۂ�Y���W�����i�K�̍ŏ��l�����������ꍇ
			//�ŏ��l���X�V����
			g_Tower.VtxMinTower.y = vtx.y;
		}
		else if (vtx.y > g_Tower.VtxMaxTower.y)
		{//��r�Ώۂ�Y���W�����i�K�̍ő�l�����傫���ꍇ
			//�ő�l���X�V����
			g_Tower.VtxMaxTower.y = vtx.y;
		}

		if (vtx.z < g_Tower.VtxMinTower.z)
		{//��r�Ώۂ�Z���W�����i�K�̍ŏ��l�����������ꍇ
			//�ŏ��l���X�V����
			g_Tower.VtxMinTower.z = vtx.z;
		}
		else if (vtx.z > g_Tower.VtxMaxTower.z)
		{//��r�Ώۂ�Z���W�����i�K�̍ő�l�����傫���ꍇ
			//�ő�l���X�V����
			g_Tower.VtxMaxTower.z = vtx.z;
		}

		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += dwSizeFVF;
	}

	D3DXMATERIAL *pMat;					//�}�e���A���ւ̃|�C���^

	//�}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatTower->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatTower; nCntMat++)
	{
		//�R�s�[�����
		MatCopy[nCntMat] = pMat[nCntMat];
	}

	//�e�̐ݒ菈��
	g_Tower.nShadow = SetShadow(SHADOW_TOWER);
}

//========================================
//���̏I������
//========================================
void UninitTower(void)
{
	//���b�V���̔j��
	if (g_pMeshTower != NULL)
	{
		g_pMeshTower->Release();
		g_pMeshTower = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatTower != NULL)
	{
		g_pBuffMatTower->Release();
		g_pBuffMatTower = NULL;
	}
}

//========================================
//���̍X�V����
//========================================
void UpdateTower(void)
{
	if (g_Tower.bUse == true)
	{//�g�p����Ă���ꍇ
		//�e�̈ʒu�ݒ菈��
		SetPositionShadow(g_Tower.nShadow, g_Tower.pos, g_Tower.rot);

		switch (g_Tower.TowerState)
		{
		case TOWER_NORMAL:			//�ʏ���

			break;					//�����o��

		case TOWER_DAMAGE:			//�_���[�W���
			//�_���[�W�J�E���g�����Z����
			g_Tower.nDamageCount--;

			if (g_Tower.nDamageCount <= 0)
			{//�_���[�W�J�E���g��0�ȉ��ɂȂ����ꍇ
				//���G��Ԃɂ���
				g_Tower.TowerState = TOWER_INVINCIBLE;

				//���G�J�E���g��ݒ肷��
				g_Tower.nInvincibleCount = 120;
			}

			break;					//�����o��

		case TOWER_INVINCIBLE:		//���G���
			//���G�J�E���g�����Z����
			g_Tower.nInvincibleCount--;

			if (g_Tower.nInvincibleCount % 10 == 0)
			{//��莞�Ԍo�߂�����
				//�_�ł����
				g_Tower.bFlash = g_Tower.bFlash ? false : true;
			}

			if (g_Tower.nInvincibleCount <= 0)
			{//���G�J�E���g��0�ȉ��ɂȂ����ꍇ
				//�ʏ��Ԃɂ���
				g_Tower.TowerState = TOWER_NORMAL;

				//�_�ł��Ȃ�
				g_Tower.bFlash = false;
			}

			break;					//�����o��

		case TOWER_PINCH:			//�s���`���

			break;					//�����o��

		case TOWER_DEATH:			//���S���
			//�g�p���Ă��Ȃ�
			g_Tower.bUse = false;

			break;					//�����o��
		}
	}
	else
	{//�g�p����Ă��Ȃ��ꍇ
		//�e�̃��Z�b�g
		ResetShadow(&g_Tower.nShadow);
	}
}

//=====================================
//���̕`�揈��
//=====================================
void DrawTower(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^

	if (g_Tower.bUse == true)
	{//�g�p����Ă���ꍇ
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Tower.mtx);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Tower.rot.y, g_Tower.rot.x, g_Tower.rot.z);
		D3DXMatrixMultiply(&g_Tower.mtx, &g_Tower.mtx, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Tower.pos.x, g_Tower.pos.y, g_Tower.pos.z);
		D3DXMatrixMultiply(&g_Tower.mtx, &g_Tower.mtx, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Tower.mtx);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatTower->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatTower; nCntMat++)
		{
			switch (g_Tower.TowerState)
			{
			case TOWER_DAMAGE:			//�_���[�W���

				//�ԐF�ɕω�������
				pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				break;					//�����o��

			case TOWER_INVINCIBLE:		//���G���

				if (g_Tower.bFlash == false)
				{//�_�ŏ�ԂłȂ������ꍇ
					
					//���ɖ߂�
					pMat[nCntMat].MatD3D.Diffuse = MatCopy[nCntMat].MatD3D.Diffuse;
					pMat[nCntMat].MatD3D.Ambient = MatCopy[nCntMat].MatD3D.Ambient;
				}
				else
				{//�_�ŏ�Ԃ������ꍇ
					//�����ɕω�������
					pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
					pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
				}

				break;					//�����o��
			}

			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);

			//���f��(�p�[�c)�̕`��
			g_pMeshTower->DrawSubset(nCntMat);
		}

		//�ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//=======================================
//���f���̍��W�̎擾����
//=======================================
Tower *GetTower(void)
{
	//���f���̏���Ԃ�
	return &g_Tower;
}

//========================================
//�^���[�̃_���[�W����
//========================================
void HitTower(void)
{
	if (g_Tower.TowerState == TOWER_NORMAL || g_Tower.TowerState == TOWER_PINCH)
	{//�ʏ��Ԃ������̓s���`��Ԃ������ꍇ
		//�̗͂�1���炷
		g_Tower.nLife--;

		if (g_Tower.nLife <= 0)
		{//�^���[�̎�����0�ȉ��ɂȂ����ꍇ
			//���S��Ԃɂ���
			g_Tower.TowerState = TOWER_DEATH;
		}

		//�_���[�W��Ԃɂ���
		g_Tower.TowerState = TOWER_DAMAGE;

		//�_���[�W�J�E���g��ݒ肷��
		g_Tower.nDamageCount = 10;
	}
}

//========================================
//�v���C���[�Ɠ��̓����蔻��
//========================================
void CollisionTower(D3DXVECTOR3 MaxPos, D3DXVECTOR3 MinPos, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, int nType)
{
	if (g_Tower.bUse == true)
	{//�����g���Ă����ꍇ
		if (pPos->x + MinPos.x <= g_Tower.pos.x + g_Tower.VtxMaxTower.x && pPos->x + MaxPos.x >= g_Tower.pos.x + g_Tower.VtxMinTower.x)
		{//����X���̒��ɂ����ꍇ
			if (pPosOld->z + MaxPos.z <= g_Tower.pos.z + g_Tower.VtxMinTower.z && pPos->z + MaxPos.z >= g_Tower.pos.z + g_Tower.VtxMinTower.z)
			{//�O��̈ʒu���u���b�N����O���A���݂̈ʒu���u���b�N���������������ꍇ(��O�Ŏ~�߂��鏈��)
				//�u���b�N�̏�ɗ�������
				pPos->z = g_Tower.pos.z + g_Tower.VtxMinTower.z - MaxPos.z;

				//�ړ��ʂ�0�ɂ���
				pMove->z = 0.0f;

				if (nType == COLL_ENEMY)
				{//�G���Ԃ����Ă����ꍇ
					//���̃_���[�W����
					HitTower();
				}
			}							//��O�Ŏ~�߂��鏈��
			else if (pPosOld->z + MinPos.z >= g_Tower.pos.z + g_Tower.VtxMaxTower.z && pPos->z + MinPos.z <= g_Tower.pos.z + g_Tower.VtxMaxTower.z)
			{//�O��̈ʒu�����̈ʒu���������A���݂̈ʒu�����̈ʒu������O�������ꍇ(���Ŏ~�߂��鏈��)
				//�u���b�N�̏�ɗ�������
				pPos->z = g_Tower.pos.z + g_Tower.VtxMaxTower.z - MinPos.z;

				//�ړ��ʂ�0�ɂ���
				pMove->z = 0.0f;

				if (nType == COLL_ENEMY)
				{//�G���Ԃ����Ă����ꍇ
					//���̃_���[�W����
					HitTower();
				}
			}							//���Ŏ~�߂��鏈��
		}

		if (pPos->z + MinPos.z <= g_Tower.pos.z + g_Tower.VtxMaxTower.z && pPos->z + MaxPos.z >= g_Tower.pos.z + g_Tower.VtxMinTower.z)
		{//����Z���̒��ɂ����ꍇ
			if (pPosOld->x + MaxPos.x <= g_Tower.pos.x + g_Tower.VtxMinTower.x && pPos->x + MaxPos.x >= g_Tower.pos.x + g_Tower.VtxMinTower.x)
			{//�O��̈ʒu���u���b�N�̍��[��荶���A���݂̈ʒu���u���b�N�̍������E�������ꍇ(���̏���)
				//�u���b�N�̍��[�ɒ���t������
				pPos->x = g_Tower.pos.x + g_Tower.VtxMinTower.x - MaxPos.x;

				//�ړ��ʂ�0�ɂ���
				pMove->x = 0.0f;

				if (nType == COLL_ENEMY)
				{//�G���Ԃ����Ă����ꍇ
					//���̃_���[�W����
					HitTower();
				}
			}							//���[�̏���
			else if (pPosOld->x + MinPos.x >= g_Tower.pos.x + g_Tower.VtxMaxTower.x && pPos->x + MinPos.x <= g_Tower.pos.x + g_Tower.VtxMaxTower.x)
			{//�O��̈ʒu���u���b�N�̉E�[���E���A���݂̈ʒu���u���b�N�̍������E�������ꍇ(�E�̏���)
				//�u���b�N�̉E�[�ɒ���t������
				pPos->x = g_Tower.pos.x + g_Tower.VtxMaxTower.x - MinPos.x;

				//�ړ��ʂ�0�ɂ���
				pMove->x = 0.0f;

				if (nType == COLL_ENEMY)
				{//�G���Ԃ����Ă����ꍇ
					//���̃_���[�W����
					HitTower();
				}
			}							//�E�[�̏���								//�W�����v���Ă��Ȃ�������
		}
	}
}