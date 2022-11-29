//===========================================
//
//敵のメイン処理[Enemy.cpp]
//Author 小原立暉
//
//===========================================
#include "main.h"
#include "Enemy.h"
#include "Shadow.h"
#include "model.h"
#include "Tower.h"

//マクロ定義
#define MAX_TEXTURE					(10)			//テクスチャの最大数
#define ENEMY_SPEED					(3.0f)			//敵の移動距離

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureEnemy[MAX_TEXTURE] = {};		//テクスチャへのポインタ
Enemy g_Enemy[MAX_ENEMY];									//モデルの情報
int g_nEnemyCounter;										//敵の数

//テクスチャファイル名
const char *c_apFilenameEnemy[2] = 
{
	"data/TEXTURE/SlimeEye.png",					//目のテクスチャ
	"data/TEXTURE/SlimeMouth.png"					//口のテクスチャ
};

//==========================================
//モデルの初期化処理
//==========================================
void InitEnemy(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		//位置を初期化する
		g_Enemy[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//前回の位置を初期化する
		g_Enemy[nCnt].posOld = g_Enemy[nCnt].pos;

		//移動量を初期化する
		g_Enemy[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//向きを初期化する
		g_Enemy[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//使用している
		g_Enemy[nCnt].bUse = false;

		//影のインデックス(影の番号)を初期化する
		g_Enemy[nCnt].nShadow = -1;

		//メッシュ(頂点情報)へのポインタを初期化する
		g_Enemy[nCnt].g_pMeshEnemy = NULL;

		//マテリアルへのポインタを初期化する
		g_Enemy[nCnt].g_pBuffMatEnemy = NULL;

		//マテリアルの数を初期化する
		g_Enemy[nCnt].g_dwNumMatEnemy = 0;

		//種類を種類を初期化する
		g_Enemy[nCnt].nType = 0;
	}

	//敵の数を初期化する
	g_nEnemyCounter = 0;

	//敵の設定処理
	SetEnemy(ENEMY_SLIME);
}

//========================================
//モデルの終了処理
//========================================
void UninitEnemy(void)
{
	for (int nCntMat = 0; nCntMat < MAX_TEXTURE; nCntMat++)
	{
		//テクスチャの破棄
		if (g_apTextureEnemy[nCntMat] != NULL)
		{
			g_apTextureEnemy[nCntMat]->Release();
			g_apTextureEnemy[nCntMat] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		//メッシュの破棄
		if (g_Enemy[nCnt].g_pMeshEnemy != NULL)
		{
			g_Enemy[nCnt].g_pMeshEnemy->Release();
			g_Enemy[nCnt].g_pMeshEnemy = NULL;
		}

		//マテリアルの破棄
		if (g_Enemy[nCnt].g_pBuffMatEnemy != NULL)
		{
			g_Enemy[nCnt].g_pBuffMatEnemy->Release();
			g_Enemy[nCnt].g_pBuffMatEnemy = NULL;
		}
	}
}

//========================================
//モデルの更新処理
//========================================
void UpdateEnemy(void)
{
	Tower *pTower = GetTower();					//塔の情報を取得する
	int nEnmCount = 0;							//敵の総数

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (g_Enemy[nCnt].bUse == true)
		{//使用されている場合
			//敵の前回の位置を記録する
			g_Enemy[nCnt].posOld = g_Enemy[nCnt].pos;

			//敵の総数を加算する
			nEnmCount++;

			float fRotDest,			//目標の向き
				  fRotDiff;			//目標までの差分

			//目的の向きを設定する
			fRotDest = atan2f(pTower->pos.x - g_Enemy[nCnt].pos.x, pTower->pos.z - g_Enemy[nCnt].pos.z);

			//向きの差分を求める
			fRotDiff = fRotDest - g_Enemy[nCnt].rot.y * 0.8f;

			if (fRotDiff > D3DX_PI)
			{//角度が3.14fより大きかった場合
				//角度から1周分減らす
				fRotDiff = -D3DX_PI;
			}
			else if (fRotDiff < -D3DX_PI)
			{//角度が-3.14fより小さかった場合
				//角度に1周分加える
				fRotDiff = D3DX_PI;
			}

			//角度を補正する
			g_Enemy[nCnt].rot.y += fRotDiff;

			if (g_Enemy[nCnt].rot.y > D3DX_PI)
			{//角度が3.14fより大きかった場合
				//角度から1周分減らす
				g_Enemy[nCnt].rot.y = -D3DX_PI;
			}
			else if (g_Enemy[nCnt].rot.y < -D3DX_PI)
			{//角度が-3.14fより小さかった場合
				//角度に1周分加える
				g_Enemy[nCnt].rot.y = D3DX_PI;
			}

			//上に加速する
			g_Enemy[nCnt].move.x = sinf(g_Enemy[nCnt].rot.y) * ENEMY_SPEED;
			g_Enemy[nCnt].move.z = cosf(g_Enemy[nCnt].rot.y) * ENEMY_SPEED;

			//位置を更新する
			g_Enemy[nCnt].pos.x += g_Enemy[nCnt].move.x;
			g_Enemy[nCnt].pos.z += g_Enemy[nCnt].move.z;

			//影の位置設定処理
			SetPositionShadow(g_Enemy[nCnt].nShadow, g_Enemy[nCnt].pos, g_Enemy[nCnt].rot);

			//塔の当たり判定
			CollisionTower(g_Enemy[nCnt].VtxMaxEnemy, g_Enemy[nCnt].VtxMinEnemy, &g_Enemy[nCnt].pos, &g_Enemy[nCnt].posOld, &g_Enemy[nCnt].move, COLL_ENEMY);
		}
	}

	//カウントをコピーする
	g_nEnemyCounter = nEnmCount;
}

//=====================================
//モデルの描画処理
//=====================================
void DrawEnemy(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (g_Enemy[nCnt].bUse == true)
		{//使用している場合
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Enemy[nCnt].mtx);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy[nCnt].rot.y, g_Enemy[nCnt].rot.x, g_Enemy[nCnt].rot.z);
			D3DXMatrixMultiply(&g_Enemy[nCnt].mtx, &g_Enemy[nCnt].mtx, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Enemy[nCnt].pos.x, g_Enemy[nCnt].pos.y, g_Enemy[nCnt].pos.z);
			D3DXMatrixMultiply(&g_Enemy[nCnt].mtx, &g_Enemy[nCnt].mtx, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[nCnt].mtx);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_Enemy[nCnt].g_pBuffMatEnemy->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_Enemy[nCnt].g_dwNumMatEnemy; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_apTextureEnemy[nCntMat]);

				//モデル(パーツ)の描画
				g_Enemy[nCnt].g_pMeshEnemy->DrawSubset(nCntMat);
			}
			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================
//モデルの設定処理
//======================================
void SetEnemy(int nType)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntTexture = 0;					//テクスチャの数
	int nNumVtx;							//頂点数
	DWORD dwSizeFVF;						//頂点フォーマットのサイズ
	BYTE *pVtxBuff;							//頂点バッファへのポインタ

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_Enemy[nCntEnemy].bUse == false)
		{//敵が使用されていない場合

			//モデルの位置を初期化する
			g_Enemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//モデルの移動量を初期化する
			g_Enemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//モデルの向きを初期化する
			g_Enemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			switch (nType)
			{
			case ENEMY_SLIME:			//スライム

				//Xファイルの読み込み
				D3DXLoadMeshFromX("data/MODEL/Slime.x",
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_Enemy[nCntEnemy].g_pBuffMatEnemy,
					NULL,
					&g_Enemy[nCntEnemy].g_dwNumMatEnemy,
					&g_Enemy[nCntEnemy].g_pMeshEnemy);

				break;					//抜け出す

			case ENEMY_INSECT:			//虫

				//Xファイルの読み込み
				D3DXLoadMeshFromX("data/MODEL/Insect.x",
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_Enemy[nCntEnemy].g_pBuffMatEnemy,
					NULL,
					&g_Enemy[nCntEnemy].g_dwNumMatEnemy,
					&g_Enemy[nCntEnemy].g_pMeshEnemy);

				break;					//抜け出す
			}

			//頂点数を取得
			nNumVtx = g_Enemy[nCntEnemy].g_pMeshEnemy->GetNumVertices();

			//頂点フォーマットのサイズを取得
			dwSizeFVF = D3DXGetFVFVertexSize(g_Enemy[nCntEnemy].g_pMeshEnemy->GetFVF());

			//頂点バッファをロック
			g_Enemy[nCntEnemy].g_pMeshEnemy->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			//頂点座標の代入
			g_Enemy[nCntEnemy].VtxMaxEnemy = *(D3DXVECTOR3*)pVtxBuff;
			g_Enemy[nCntEnemy].VtxMinEnemy = *(D3DXVECTOR3*)pVtxBuff;

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//頂点座標の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				if (vtx.x < g_Enemy[nCntEnemy].VtxMinEnemy.x)
				{//比較対象のX座標が現段階の最小値よりも小さい場合
					//最小値を更新する
					g_Enemy[nCntEnemy].VtxMinEnemy.x = vtx.x;
				}
				else if (vtx.x > g_Enemy[nCntEnemy].VtxMaxEnemy.x)
				{//比較対象のX座標が現段階の最大値よりも大きい場合
					//最大値を更新する
					g_Enemy[nCntEnemy].VtxMaxEnemy.x = vtx.x;
				}

				if (vtx.y < g_Enemy[nCntEnemy].VtxMinEnemy.y)
				{//比較対象のY座標が現段階の最小値よりも小さい場合
					//最小値を更新する
					g_Enemy[nCntEnemy].VtxMinEnemy.y = vtx.y;
				}
				else if (vtx.y > g_Enemy[nCntEnemy].VtxMaxEnemy.y)
				{//比較対象のY座標が現段階の最大値よりも大きい場合
					//最大値を更新する
					g_Enemy[nCntEnemy].VtxMaxEnemy.y = vtx.y;
				}

				if (vtx.z < g_Enemy[nCntEnemy].VtxMinEnemy.z)
				{//比較対象のZ座標が現段階の最小値よりも小さい場合
					//最小値を更新する
					g_Enemy[nCntEnemy].VtxMinEnemy.z = vtx.z;
				}
				else if (vtx.z > g_Enemy[nCntEnemy].VtxMaxEnemy.z)
				{//比較対象のZ座標が現段階の最大値よりも大きい場合
					//最大値を更新する
					g_Enemy[nCntEnemy].VtxMaxEnemy.z = vtx.z;
				}

				//頂点フォーマットのサイズ分ポインタを進める
				pVtxBuff += dwSizeFVF;
			}

			D3DXMATERIAL *pMat;					//マテリアルへのポインタ

			//種類を初期化する
			g_Enemy[nCntEnemy].nType = 0;

			//マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)g_Enemy[nCntEnemy].g_pBuffMatEnemy->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_Enemy[nCntEnemy].g_dwNumMatEnemy; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{//テクスチャファイル名が存在する
					//このファイル名を使用してテクスチャを読み込む
					D3DXCreateTextureFromFile(pDevice,
						c_apFilenameEnemy[nCntTexture],
						&g_apTextureEnemy[nCntMat]);

					//テクスチャの数を加算する
					nCntTexture++;
				}
			}
			//影の設定処理
			g_Enemy[nCntEnemy].nShadow = SetShadow(SHADOW_ENEMY);

			//使用している
			g_Enemy[nCntEnemy].bUse = true;

			break;					//抜け出す
		}
	}
}

//================================
//敵の総数取得処理
//================================
int GetEnemyCount(void)
{
	//敵の総数を返す
	return g_nEnemyCounter;
}

//================================
//敵の取得処理
//================================
Enemy *GetEnemy(void)
{
	//敵の情報を返す
	return &g_Enemy[0];
}