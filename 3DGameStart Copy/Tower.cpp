//===========================================
//
//モデルのメイン処理[model.cpp]
//Author 小原立暉
//
//===========================================
#include "main.h"
#include "Tower.h"
#include "Shadow.h"
#include "input.h"
#include "Enemy.h"

//マクロ定義
#define PLAYER_SPEED				(10.0f)			//モデルの移動距離
#define MAX_MATERIAL				(32)			//マテリアルの種類

//プロトタイプ宣言
void HitTower(void);								//タワーのダメージ判定

//グローバル変数
LPD3DXMESH g_pMeshTower = NULL;						//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatTower = NULL;				//マテリアルへのポインタ
DWORD g_dwNumMatTower = 0;							//マテリアルの数
Tower g_Tower;										//モデルの情報
D3DXMATERIAL MatCopy[MAX_MATERIAL] = {};			//マテリアルのコピー

//==========================================
//塔の初期化処理
//==========================================
void InitTower(void)
{
	int nNumVtx;							//頂点数
	DWORD dwSizeFVF;						//頂点フォーマットのサイズ
	BYTE *pVtxBuff;							//頂点バッファへのポインタ

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//モデルの位置を初期化する
	g_Tower.pos = D3DXVECTOR3(450.0f, 0.0f, 450.0f);

	//モデルの向きを初期化する
	g_Tower.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//使用している
	g_Tower.bUse = true;

	//影のインデックス(影の番号)を初期化する
	g_Tower.nShadow = -1;

	//通常状態
	g_Tower.TowerState = TOWER_NORMAL;

	//塔の体力を初期化する
	g_Tower.nLife = 60;

	//ダメージカウントを初期化する
	g_Tower.nDamageCount = 0;

	//無敵カウントを初期化する
	g_Tower.nInvincibleCount = 0;

	//点滅しない
	g_Tower.bFlash = false;

	//Xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/Tower.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatTower,
		NULL,
		&g_dwNumMatTower,
		&g_pMeshTower);

	//頂点数を取得
	nNumVtx = g_pMeshTower->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshTower->GetFVF());

	//頂点バッファをロック
	g_pMeshTower->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//頂点座標の代入
	g_Tower.VtxMaxTower = *(D3DXVECTOR3*)pVtxBuff;
	g_Tower.VtxMinTower = *(D3DXVECTOR3*)pVtxBuff;

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (vtx.x < g_Tower.VtxMinTower.x)
		{//比較対象のX座標が現段階の最小値よりも小さい場合
			//最小値を更新する
			g_Tower.VtxMinTower.x = vtx.x;
		}
		else if (vtx.x > g_Tower.VtxMaxTower.x)
		{//比較対象のX座標が現段階の最大値よりも大きい場合
			//最大値を更新する
			g_Tower.VtxMaxTower.x = vtx.x;
		}

		if (vtx.y < g_Tower.VtxMinTower.y)
		{//比較対象のY座標が現段階の最小値よりも小さい場合
			//最小値を更新する
			g_Tower.VtxMinTower.y = vtx.y;
		}
		else if (vtx.y > g_Tower.VtxMaxTower.y)
		{//比較対象のY座標が現段階の最大値よりも大きい場合
			//最大値を更新する
			g_Tower.VtxMaxTower.y = vtx.y;
		}

		if (vtx.z < g_Tower.VtxMinTower.z)
		{//比較対象のZ座標が現段階の最小値よりも小さい場合
			//最小値を更新する
			g_Tower.VtxMinTower.z = vtx.z;
		}
		else if (vtx.z > g_Tower.VtxMaxTower.z)
		{//比較対象のZ座標が現段階の最大値よりも大きい場合
			//最大値を更新する
			g_Tower.VtxMaxTower.z = vtx.z;
		}

		//頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += dwSizeFVF;
	}

	D3DXMATERIAL *pMat;					//マテリアルへのポインタ

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatTower->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatTower; nCntMat++)
	{
		//コピーを取る
		MatCopy[nCntMat] = pMat[nCntMat];
	}

	//影の設定処理
	g_Tower.nShadow = SetShadow(SHADOW_TOWER);
}

//========================================
//塔の終了処理
//========================================
void UninitTower(void)
{
	//メッシュの破棄
	if (g_pMeshTower != NULL)
	{
		g_pMeshTower->Release();
		g_pMeshTower = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatTower != NULL)
	{
		g_pBuffMatTower->Release();
		g_pBuffMatTower = NULL;
	}
}

//========================================
//塔の更新処理
//========================================
void UpdateTower(void)
{
	if (g_Tower.bUse == true)
	{//使用されている場合
		//影の位置設定処理
		SetPositionShadow(g_Tower.nShadow, g_Tower.pos, g_Tower.rot);

		switch (g_Tower.TowerState)
		{
		case TOWER_NORMAL:			//通常状態

			break;					//抜け出す

		case TOWER_DAMAGE:			//ダメージ状態
			//ダメージカウントを減算する
			g_Tower.nDamageCount--;

			if (g_Tower.nDamageCount <= 0)
			{//ダメージカウントが0以下になった場合
				//無敵状態にする
				g_Tower.TowerState = TOWER_INVINCIBLE;

				//無敵カウントを設定する
				g_Tower.nInvincibleCount = 120;
			}

			break;					//抜け出す

		case TOWER_INVINCIBLE:		//無敵状態
			//無敵カウントを減算する
			g_Tower.nInvincibleCount--;

			if (g_Tower.nInvincibleCount % 10 == 0)
			{//一定時間経過したら
				//点滅を取る
				g_Tower.bFlash = g_Tower.bFlash ? false : true;
			}

			if (g_Tower.nInvincibleCount <= 0)
			{//無敵カウントが0以下になった場合
				//通常状態にする
				g_Tower.TowerState = TOWER_NORMAL;

				//点滅しない
				g_Tower.bFlash = false;
			}

			break;					//抜け出す

		case TOWER_PINCH:			//ピンチ状態

			break;					//抜け出す

		case TOWER_DEATH:			//死亡状態
			//使用していない
			g_Tower.bUse = false;

			break;					//抜け出す
		}
	}
	else
	{//使用されていない場合
		//影のリセット
		ResetShadow(&g_Tower.nShadow);
	}
}

//=====================================
//塔の描画処理
//=====================================
void DrawTower(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	if (g_Tower.bUse == true)
	{//使用されている場合
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Tower.mtx);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Tower.rot.y, g_Tower.rot.x, g_Tower.rot.z);
		D3DXMatrixMultiply(&g_Tower.mtx, &g_Tower.mtx, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Tower.pos.x, g_Tower.pos.y, g_Tower.pos.z);
		D3DXMatrixMultiply(&g_Tower.mtx, &g_Tower.mtx, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Tower.mtx);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatTower->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatTower; nCntMat++)
		{
			switch (g_Tower.TowerState)
			{
			case TOWER_DAMAGE:			//ダメージ状態

				//赤色に変化させる
				pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				break;					//抜け出す

			case TOWER_INVINCIBLE:		//無敵状態

				if (g_Tower.bFlash == false)
				{//点滅状態でなかった場合
					
					//元に戻す
					pMat[nCntMat].MatD3D.Diffuse = MatCopy[nCntMat].MatD3D.Diffuse;
					pMat[nCntMat].MatD3D.Ambient = MatCopy[nCntMat].MatD3D.Ambient;
				}
				else
				{//点滅状態だった場合
					//透明に変化させる
					pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
					pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
				}

				break;					//抜け出す
			}

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, NULL);

			//モデル(パーツ)の描画
			g_pMeshTower->DrawSubset(nCntMat);
		}

		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=======================================
//モデルの座標の取得処理
//=======================================
Tower *GetTower(void)
{
	//モデルの情報を返す
	return &g_Tower;
}

//========================================
//タワーのダメージ判定
//========================================
void HitTower(void)
{
	if (g_Tower.TowerState == TOWER_NORMAL || g_Tower.TowerState == TOWER_PINCH)
	{//通常状態もしくはピンチ状態だった場合
		//体力を1個減らす
		g_Tower.nLife--;

		if (g_Tower.nLife <= 0)
		{//タワーの寿命が0以下になった場合
			//死亡状態にする
			g_Tower.TowerState = TOWER_DEATH;
		}

		//ダメージ状態にする
		g_Tower.TowerState = TOWER_DAMAGE;

		//ダメージカウントを設定する
		g_Tower.nDamageCount = 10;
	}
}

//========================================
//プレイヤーと塔の当たり判定
//========================================
void CollisionTower(D3DXVECTOR3 MaxPos, D3DXVECTOR3 MinPos, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, int nType)
{
	if (g_Tower.bUse == true)
	{//塔が使われていた場合
		if (pPos->x + MinPos.x <= g_Tower.pos.x + g_Tower.VtxMaxTower.x && pPos->x + MaxPos.x >= g_Tower.pos.x + g_Tower.VtxMinTower.x)
		{//塔のX幅の中にいた場合
			if (pPosOld->z + MaxPos.z <= g_Tower.pos.z + g_Tower.VtxMinTower.z && pPos->z + MaxPos.z >= g_Tower.pos.z + g_Tower.VtxMinTower.z)
			{//前回の位置がブロックより手前かつ、現在の位置がブロックよりも奥かつだった場合(手前で止められる処理)
				//ブロックの上に立たせる
				pPos->z = g_Tower.pos.z + g_Tower.VtxMinTower.z - MaxPos.z;

				//移動量を0にする
				pMove->z = 0.0f;

				if (nType == COLL_ENEMY)
				{//敵がぶつかってきた場合
					//塔のダメージ判定
					HitTower();
				}
			}							//手前で止められる処理
			else if (pPosOld->z + MinPos.z >= g_Tower.pos.z + g_Tower.VtxMaxTower.z && pPos->z + MinPos.z <= g_Tower.pos.z + g_Tower.VtxMaxTower.z)
			{//前回の位置が塔の位置よりも奥かつ、現在の位置が塔の位置よりも手前だった場合(奥で止められる処理)
				//ブロックの上に立たせる
				pPos->z = g_Tower.pos.z + g_Tower.VtxMaxTower.z - MinPos.z;

				//移動量を0にする
				pMove->z = 0.0f;

				if (nType == COLL_ENEMY)
				{//敵がぶつかってきた場合
					//塔のダメージ判定
					HitTower();
				}
			}							//奥で止められる処理
		}

		if (pPos->z + MinPos.z <= g_Tower.pos.z + g_Tower.VtxMaxTower.z && pPos->z + MaxPos.z >= g_Tower.pos.z + g_Tower.VtxMinTower.z)
		{//塔のZ幅の中にいた場合
			if (pPosOld->x + MaxPos.x <= g_Tower.pos.x + g_Tower.VtxMinTower.x && pPos->x + MaxPos.x >= g_Tower.pos.x + g_Tower.VtxMinTower.x)
			{//前回の位置がブロックの左端より左かつ、現在の位置がブロックの左側より右だった場合(左の処理)
				//ブロックの左端に張り付かせる
				pPos->x = g_Tower.pos.x + g_Tower.VtxMinTower.x - MaxPos.x;

				//移動量を0にする
				pMove->x = 0.0f;

				if (nType == COLL_ENEMY)
				{//敵がぶつかってきた場合
					//塔のダメージ判定
					HitTower();
				}
			}							//左端の処理
			else if (pPosOld->x + MinPos.x >= g_Tower.pos.x + g_Tower.VtxMaxTower.x && pPos->x + MinPos.x <= g_Tower.pos.x + g_Tower.VtxMaxTower.x)
			{//前回の位置がブロックの右端より右かつ、現在の位置がブロックの左側より右だった場合(右の処理)
				//ブロックの右端に張り付かせる
				pPos->x = g_Tower.pos.x + g_Tower.VtxMaxTower.x - MinPos.x;

				//移動量を0にする
				pMove->x = 0.0f;

				if (nType == COLL_ENEMY)
				{//敵がぶつかってきた場合
					//塔のダメージ判定
					HitTower();
				}
			}							//右端の処理								//ジャンプしていなかった時
		}
	}
}