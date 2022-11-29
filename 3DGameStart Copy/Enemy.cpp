//===========================================
//
//�G�̃��C������[Enemy.cpp]
//Author ��������
//
//===========================================
#include "main.h"
#include "Enemy.h"
#include "Shadow.h"
#include "model.h"
#include "Tower.h"

//�}�N����`
#define MAX_TEXTURE					(10)			//�e�N�X�`���̍ő吔
#define ENEMY_SPEED					(3.0f)			//�G�̈ړ�����

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureEnemy[MAX_TEXTURE] = {};		//�e�N�X�`���ւ̃|�C���^
Enemy g_Enemy[MAX_ENEMY];									//���f���̏��
int g_nEnemyCounter;										//�G�̐�

//�e�N�X�`���t�@�C����
const char *c_apFilenameEnemy[2] = 
{
	"data/TEXTURE/SlimeEye.png",					//�ڂ̃e�N�X�`��
	"data/TEXTURE/SlimeMouth.png"					//���̃e�N�X�`��
};

//==========================================
//���f���̏���������
//==========================================
void InitEnemy(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		//�ʒu������������
		g_Enemy[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�O��̈ʒu������������
		g_Enemy[nCnt].posOld = g_Enemy[nCnt].pos;

		//�ړ��ʂ�����������
		g_Enemy[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//����������������
		g_Enemy[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�g�p���Ă���
		g_Enemy[nCnt].bUse = false;

		//�e�̃C���f�b�N�X(�e�̔ԍ�)������������
		g_Enemy[nCnt].nShadow = -1;

		//���b�V��(���_���)�ւ̃|�C���^������������
		g_Enemy[nCnt].g_pMeshEnemy = NULL;

		//�}�e���A���ւ̃|�C���^������������
		g_Enemy[nCnt].g_pBuffMatEnemy = NULL;

		//�}�e���A���̐�������������
		g_Enemy[nCnt].g_dwNumMatEnemy = 0;

		//��ނ���ނ�����������
		g_Enemy[nCnt].nType = 0;
	}

	//�G�̐�������������
	g_nEnemyCounter = 0;

	//�G�̐ݒ菈��
	SetEnemy(ENEMY_SLIME);
}

//========================================
//���f���̏I������
//========================================
void UninitEnemy(void)
{
	for (int nCntMat = 0; nCntMat < MAX_TEXTURE; nCntMat++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureEnemy[nCntMat] != NULL)
		{
			g_apTextureEnemy[nCntMat]->Release();
			g_apTextureEnemy[nCntMat] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		//���b�V���̔j��
		if (g_Enemy[nCnt].g_pMeshEnemy != NULL)
		{
			g_Enemy[nCnt].g_pMeshEnemy->Release();
			g_Enemy[nCnt].g_pMeshEnemy = NULL;
		}

		//�}�e���A���̔j��
		if (g_Enemy[nCnt].g_pBuffMatEnemy != NULL)
		{
			g_Enemy[nCnt].g_pBuffMatEnemy->Release();
			g_Enemy[nCnt].g_pBuffMatEnemy = NULL;
		}
	}
}

//========================================
//���f���̍X�V����
//========================================
void UpdateEnemy(void)
{
	Tower *pTower = GetTower();					//���̏����擾����
	int nEnmCount = 0;							//�G�̑���

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (g_Enemy[nCnt].bUse == true)
		{//�g�p����Ă���ꍇ
			//�G�̑O��̈ʒu���L�^����
			g_Enemy[nCnt].posOld = g_Enemy[nCnt].pos;

			//�G�̑��������Z����
			nEnmCount++;

			float fRotDest,			//�ڕW�̌���
				  fRotDiff;			//�ڕW�܂ł̍���

			//�ړI�̌�����ݒ肷��
			fRotDest = atan2f(pTower->pos.x - g_Enemy[nCnt].pos.x, pTower->pos.z - g_Enemy[nCnt].pos.z);

			//�����̍��������߂�
			fRotDiff = fRotDest - g_Enemy[nCnt].rot.y * 0.8f;

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
			g_Enemy[nCnt].rot.y += fRotDiff;

			if (g_Enemy[nCnt].rot.y > D3DX_PI)
			{//�p�x��3.14f���傫�������ꍇ
				//�p�x����1�������炷
				g_Enemy[nCnt].rot.y = -D3DX_PI;
			}
			else if (g_Enemy[nCnt].rot.y < -D3DX_PI)
			{//�p�x��-3.14f��菬���������ꍇ
				//�p�x��1����������
				g_Enemy[nCnt].rot.y = D3DX_PI;
			}

			//��ɉ�������
			g_Enemy[nCnt].move.x = sinf(g_Enemy[nCnt].rot.y) * ENEMY_SPEED;
			g_Enemy[nCnt].move.z = cosf(g_Enemy[nCnt].rot.y) * ENEMY_SPEED;

			//�ʒu���X�V����
			g_Enemy[nCnt].pos.x += g_Enemy[nCnt].move.x;
			g_Enemy[nCnt].pos.z += g_Enemy[nCnt].move.z;

			//�e�̈ʒu�ݒ菈��
			SetPositionShadow(g_Enemy[nCnt].nShadow, g_Enemy[nCnt].pos, g_Enemy[nCnt].rot);

			//���̓����蔻��
			CollisionTower(g_Enemy[nCnt].VtxMaxEnemy, g_Enemy[nCnt].VtxMinEnemy, &g_Enemy[nCnt].pos, &g_Enemy[nCnt].posOld, &g_Enemy[nCnt].move, COLL_ENEMY);
		}
	}

	//�J�E���g���R�s�[����
	g_nEnemyCounter = nEnmCount;
}

//=====================================
//���f���̕`�揈��
//=====================================
void DrawEnemy(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (g_Enemy[nCnt].bUse == true)
		{//�g�p���Ă���ꍇ
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Enemy[nCnt].mtx);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy[nCnt].rot.y, g_Enemy[nCnt].rot.x, g_Enemy[nCnt].rot.z);
			D3DXMatrixMultiply(&g_Enemy[nCnt].mtx, &g_Enemy[nCnt].mtx, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_Enemy[nCnt].pos.x, g_Enemy[nCnt].pos.y, g_Enemy[nCnt].pos.z);
			D3DXMatrixMultiply(&g_Enemy[nCnt].mtx, &g_Enemy[nCnt].mtx, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[nCnt].mtx);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_Enemy[nCnt].g_pBuffMatEnemy->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_Enemy[nCnt].g_dwNumMatEnemy; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureEnemy[nCntMat]);

				//���f��(�p�[�c)�̕`��
				g_Enemy[nCnt].g_pMeshEnemy->DrawSubset(nCntMat);
			}
			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================
//���f���̐ݒ菈��
//======================================
void SetEnemy(int nType)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntTexture = 0;					//�e�N�X�`���̐�
	int nNumVtx;							//���_��
	DWORD dwSizeFVF;						//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;							//���_�o�b�t�@�ւ̃|�C���^

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_Enemy[nCntEnemy].bUse == false)
		{//�G���g�p����Ă��Ȃ��ꍇ

			//���f���̈ʒu������������
			g_Enemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//���f���̈ړ��ʂ�����������
			g_Enemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//���f���̌���������������
			g_Enemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			switch (nType)
			{
			case ENEMY_SLIME:			//�X���C��

				//X�t�@�C���̓ǂݍ���
				D3DXLoadMeshFromX("data/MODEL/Slime.x",
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_Enemy[nCntEnemy].g_pBuffMatEnemy,
					NULL,
					&g_Enemy[nCntEnemy].g_dwNumMatEnemy,
					&g_Enemy[nCntEnemy].g_pMeshEnemy);

				break;					//�����o��

			case ENEMY_INSECT:			//��

				//X�t�@�C���̓ǂݍ���
				D3DXLoadMeshFromX("data/MODEL/Insect.x",
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_Enemy[nCntEnemy].g_pBuffMatEnemy,
					NULL,
					&g_Enemy[nCntEnemy].g_dwNumMatEnemy,
					&g_Enemy[nCntEnemy].g_pMeshEnemy);

				break;					//�����o��
			}

			//���_�����擾
			nNumVtx = g_Enemy[nCntEnemy].g_pMeshEnemy->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y���擾
			dwSizeFVF = D3DXGetFVFVertexSize(g_Enemy[nCntEnemy].g_pMeshEnemy->GetFVF());

			//���_�o�b�t�@�����b�N
			g_Enemy[nCntEnemy].g_pMeshEnemy->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			//���_���W�̑��
			g_Enemy[nCntEnemy].VtxMaxEnemy = *(D3DXVECTOR3*)pVtxBuff;
			g_Enemy[nCntEnemy].VtxMinEnemy = *(D3DXVECTOR3*)pVtxBuff;

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//���_���W�̑��
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				if (vtx.x < g_Enemy[nCntEnemy].VtxMinEnemy.x)
				{//��r�Ώۂ�X���W�����i�K�̍ŏ��l�����������ꍇ
					//�ŏ��l���X�V����
					g_Enemy[nCntEnemy].VtxMinEnemy.x = vtx.x;
				}
				else if (vtx.x > g_Enemy[nCntEnemy].VtxMaxEnemy.x)
				{//��r�Ώۂ�X���W�����i�K�̍ő�l�����傫���ꍇ
					//�ő�l���X�V����
					g_Enemy[nCntEnemy].VtxMaxEnemy.x = vtx.x;
				}

				if (vtx.y < g_Enemy[nCntEnemy].VtxMinEnemy.y)
				{//��r�Ώۂ�Y���W�����i�K�̍ŏ��l�����������ꍇ
					//�ŏ��l���X�V����
					g_Enemy[nCntEnemy].VtxMinEnemy.y = vtx.y;
				}
				else if (vtx.y > g_Enemy[nCntEnemy].VtxMaxEnemy.y)
				{//��r�Ώۂ�Y���W�����i�K�̍ő�l�����傫���ꍇ
					//�ő�l���X�V����
					g_Enemy[nCntEnemy].VtxMaxEnemy.y = vtx.y;
				}

				if (vtx.z < g_Enemy[nCntEnemy].VtxMinEnemy.z)
				{//��r�Ώۂ�Z���W�����i�K�̍ŏ��l�����������ꍇ
					//�ŏ��l���X�V����
					g_Enemy[nCntEnemy].VtxMinEnemy.z = vtx.z;
				}
				else if (vtx.z > g_Enemy[nCntEnemy].VtxMaxEnemy.z)
				{//��r�Ώۂ�Z���W�����i�K�̍ő�l�����傫���ꍇ
					//�ő�l���X�V����
					g_Enemy[nCntEnemy].VtxMaxEnemy.z = vtx.z;
				}

				//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
				pVtxBuff += dwSizeFVF;
			}

			D3DXMATERIAL *pMat;					//�}�e���A���ւ̃|�C���^

			//��ނ�����������
			g_Enemy[nCntEnemy].nType = 0;

			//�}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_Enemy[nCntEnemy].g_pBuffMatEnemy->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_Enemy[nCntEnemy].g_dwNumMatEnemy; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{//�e�N�X�`���t�@�C���������݂���
					//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
					D3DXCreateTextureFromFile(pDevice,
						c_apFilenameEnemy[nCntTexture],
						&g_apTextureEnemy[nCntMat]);

					//�e�N�X�`���̐������Z����
					nCntTexture++;
				}
			}
			//�e�̐ݒ菈��
			g_Enemy[nCntEnemy].nShadow = SetShadow(SHADOW_ENEMY);

			//�g�p���Ă���
			g_Enemy[nCntEnemy].bUse = true;

			break;					//�����o��
		}
	}
}

//================================
//�G�̑����擾����
//================================
int GetEnemyCount(void)
{
	//�G�̑�����Ԃ�
	return g_nEnemyCounter;
}

//================================
//�G�̎擾����
//================================
Enemy *GetEnemy(void)
{
	//�G�̏���Ԃ�
	return &g_Enemy[0];
}