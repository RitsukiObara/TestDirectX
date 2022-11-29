//===========================================
//
//モデルのメイン処理[model.cpp]
//Author 小原立暉
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

//マクロ定義
#define MAX_TEXTURE					(10)			//テクスチャの最大数
#define PLAYER_SPEED				(7.0f)			//モデルの移動距離

//プロトタイプ宣言
void CollisionPlayer(Model *pModel);				//プレイヤーの当たり判定
void CollisionPlayerAttack(Model *pModel);			//プレイヤーの攻撃判定処理

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureModel[MAX_TEXTURE] = {};		//テクスチャへのポインタ
LPD3DXMESH g_pMeshModel = NULL;						//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatModel = NULL;				//マテリアルへのポインタ
DWORD g_dwNumMatModel = 0;							//マテリアルの数
Model g_Model;										//モデルの情報
D3DXVECTOR3 Camerarot;								//カメラの向き

//==========================================
//モデルの初期化処理
//==========================================
void InitModel(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nNumVtx;							//頂点数
	DWORD dwSizeFVF;						//頂点フォーマットのサイズ
	BYTE *pVtxBuff;							//頂点バッファへのポインタ

	////下地のテクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice,
	//	"data/TEXTURE/ジョジョ D4C.png",
	//	&g_pTexturePolygon);

	//モデルの位置を初期化する
	g_Model.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//モデルの前回の位置を初期化する
	g_Model.posOld = g_Model.pos;

	//モデルの移動量を初期化する
	g_Model.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//モデルの向きを初期化する
	g_Model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//使用している
	g_Model.bUse = true;

	//影のインデックス(影の番号)を初期化する
	g_Model.nShadow = -1;

	//カメラの向きを初期化する
	Camerarot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//目標の向きを初期化する
	g_Model.fRotDest = Camerarot.y;

	//向きの差分を初期化する
	g_Model.fRotDiff = 0.0f;

	//移動していない
	g_Model.bMove = false;

	//レベルを初期化する
	g_Model.nPoint = 0;

	//Xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/Player.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel,
		NULL,
		&g_dwNumMatModel,
		&g_pMeshModel);

	//頂点数を取得
	nNumVtx = g_pMeshModel->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshModel->GetFVF());

	//頂点バッファをロック
	g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//頂点座標の代入
	g_Model.VtxMaxModel = *(D3DXVECTOR3*)pVtxBuff;
	g_Model.VtxMinModel = *(D3DXVECTOR3*)pVtxBuff;

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (vtx.x < g_Model.VtxMinModel.x)
		{//比較対象のX座標が現段階の最小値よりも小さい場合
			//最小値を更新する
			g_Model.VtxMinModel.x = vtx.x;
		}
		else if (vtx.x > g_Model.VtxMaxModel.x)
		{//比較対象のX座標が現段階の最大値よりも大きい場合
			//最大値を更新する
			g_Model.VtxMaxModel.x = vtx.x;
		}

		if (vtx.y < g_Model.VtxMinModel.y)
		{//比較対象のY座標が現段階の最小値よりも小さい場合
			//最小値を更新する
			g_Model.VtxMinModel.y = vtx.y;
		}
		else if (vtx.y > g_Model.VtxMaxModel.y)
		{//比較対象のY座標が現段階の最大値よりも大きい場合
			//最大値を更新する
			g_Model.VtxMaxModel.y = vtx.y;
		}

		if (vtx.z < g_Model.VtxMinModel.z)
		{//比較対象のZ座標が現段階の最小値よりも小さい場合
			//最小値を更新する
			g_Model.VtxMinModel.z = vtx.z;
		}
		else if (vtx.z > g_Model.VtxMaxModel.z)
		{//比較対象のZ座標が現段階の最大値よりも大きい場合
			//最大値を更新する
			g_Model.VtxMaxModel.z = vtx.z;
		}

		//頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += dwSizeFVF;
	}

	D3DXMATERIAL *pMat;					//マテリアルへのポインタ

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイル名が存在する
			//このファイル名を使用してテクスチャを読み込む
			D3DXCreateTextureFromFile(pDevice,
				"data/TEXTURE/PlayerFace.png",
				&g_apTextureModel[nCntMat]);
		}

	}

	//影の設定処理
	g_Model.nShadow = SetShadow(SHADOW_PLAYER);
}

//========================================
//モデルの終了処理
//========================================
void UninitModel(void)
{
	for (int nCntMat = 0; nCntMat < MAX_TEXTURE; nCntMat++)
	{
		//テクスチャの破棄
		if (g_apTextureModel[nCntMat] != NULL)
		{
			g_apTextureModel[nCntMat]->Release();
			g_apTextureModel[nCntMat] = NULL;
		}
	}

	//メッシュの破棄
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//========================================
//モデルの更新処理
//========================================
void UpdateModel(void)
{
	//カメラの向きを取る
	D3DXVECTOR3 Camerarot = GetCamera().rot;

	//モデルの前回の位置を記録する
	g_Model.posOld = g_Model.pos;

	if (GetKeyboardPress(DIK_LEFT) == true)
	{//左キーが押された
		if (GetKeyboardPress(DIK_UP) == true)
		{//上キーが押された
			//左上に加速する
			g_Model.move.x = sinf(-D3DX_PI * 0.75f - Camerarot.y) * PLAYER_SPEED;
			g_Model.move.z = -cosf(-D3DX_PI * 0.75f - Camerarot.y) * PLAYER_SPEED;

			g_Model.fRotDest = (-D3DX_PI * 0.25f + Camerarot.y);
		}
		else if (GetKeyboardPress(DIK_DOWN) == true)
		{//下キーが押された
			//左下に加速する
			g_Model.move.x = sinf(-D3DX_PI * 0.25f - Camerarot.y) * PLAYER_SPEED;
			g_Model.move.z = -cosf(-D3DX_PI * 0.25f - Camerarot.y) * PLAYER_SPEED;

			g_Model.fRotDest = (-D3DX_PI * 0.75f + Camerarot.y);
		}
		else
		{//左キーだけが押された
			//左に加速する
			g_Model.move.x = sinf(-D3DX_PI * 0.5f - Camerarot.y) * PLAYER_SPEED;
			g_Model.move.z = -cosf(-D3DX_PI * 0.5f - Camerarot.y) * PLAYER_SPEED;

			g_Model.fRotDest = (-D3DX_PI * 0.5f + Camerarot.y);
		}

		//移動している
		g_Model.bMove = true;
	}
	else if (GetKeyboardPress(DIK_RIGHT) == true)
	{//右キーが押された
		if (GetKeyboardPress(DIK_UP) == true)
		{//上キーが押された
			//右上に加速する
			g_Model.move.x = sinf(D3DX_PI * 0.75f- Camerarot.y) * PLAYER_SPEED;
			g_Model.move.z = -cosf(D3DX_PI * 0.75f- Camerarot.y) * PLAYER_SPEED;

			g_Model.fRotDest = (D3DX_PI * 0.25f + Camerarot.y);
		}
		else if (GetKeyboardPress(DIK_DOWN) == true)
		{//下キーが押された
			//右下に加速する
			g_Model.move.x = sinf(D3DX_PI * 0.25f- Camerarot.y) * PLAYER_SPEED;
			g_Model.move.z = -cosf(D3DX_PI * 0.25f- Camerarot.y) * PLAYER_SPEED;

			g_Model.fRotDest = (D3DX_PI * 0.75f + Camerarot.y);
		}
		else
		{//右キーだけが押された
			//右に加速していく
			g_Model.move.x = sinf(D3DX_PI * 0.5f - Camerarot.y) * PLAYER_SPEED;
			g_Model.move.z = -cosf(D3DX_PI * 0.5f - Camerarot.y) * PLAYER_SPEED;

			g_Model.fRotDest = (D3DX_PI * 0.5f + Camerarot.y);
		}

		//移動している
		g_Model.bMove = true;
	}
	else if (GetKeyboardPress(DIK_UP) == true)
	{//上キーが押された
		//上に加速する
		g_Model.move.x = sinf(Camerarot.y) * PLAYER_SPEED;
		g_Model.move.z = cosf(Camerarot.y) * PLAYER_SPEED;

		g_Model.fRotDest = (Camerarot.y);

		//移動している
		g_Model.bMove = true;
	}
	else if (GetKeyboardPress(DIK_DOWN) == true)
	{//下キーが押された
		//下に加速する
		g_Model.move.x = -sinf(Camerarot.y) * PLAYER_SPEED;
		g_Model.move.z = -cosf(Camerarot.y) * PLAYER_SPEED;

		g_Model.fRotDest = (D3DX_PI + Camerarot.y);

		//移動している
		g_Model.bMove = true;
	}

	//目標の角度への差分を求める
	g_Model.fRotDiff = g_Model.fRotDest - g_Model.rot.y;

	if (g_Model.fRotDiff > D3DX_PI)
	{//角度が3.14fより大きかった場合
		//角度から1周分減らす
		g_Model.fRotDiff -= (2 * D3DX_PI);
	}
	else if (g_Model.fRotDiff < -D3DX_PI)
	{//角度が-3.14fより小さかった場合
		//角度に1周分加える
		g_Model.fRotDiff += (2 * D3DX_PI);
	}

	//角度を補正する
	g_Model.rot.y += g_Model.fRotDiff * 0.08f;

	if (g_Model.rot.y > D3DX_PI)
	{//角度が3.14fより大きかった場合
		//角度から1周分減らす
		g_Model.rot.y = -D3DX_PI;
	}
	else if (g_Model.rot.y < -D3DX_PI)
	{//角度が-3.14fより小さかった場合
		//角度に1周分加える
		g_Model.rot.y = D3DX_PI;
	}

	//if (g_Model.rot.y > D3DX_PI)
	//{//3.14fより大きくなった場合
	//	//-3.14fに補正する
	//	g_Model.rot.y = -D3DX_PI;
	//}
	//else if (g_Model.rot.y < -D3DX_PI)
	//{//-3.14fより小さくなった場合
	//	//3.14fに補正する
	//	g_Model.rot.y = D3DX_PI;
	//}

	if (GetKeyboardTrigger(DIK_0) == true)
	{//0を押した場合
		//弾の設定処理
		SetBullet(g_Model.rot, D3DXVECTOR3(g_Model.pos.x, g_Model.pos.y + 10.0f, g_Model.pos.z));
	}

	if (g_Model.bMove == true)
	{//移動している場合
		//歩いているエフェクトを出す
		SetEffect(g_Model.pos, D3DXVECTOR3((-g_Model.move.x * CENTER_MAGNI), 2.0f, (-g_Model.move.z * CENTER_MAGNI)), 15.0f, 10, EFFECT_WALK);
	}

	//移動させる
	g_Model.pos += g_Model.move;

	//移動量を更新(減衰する)
	g_Model.move.x += (0.0f - g_Model.move.x) * 0.07f;
	g_Model.move.z += (0.0f - g_Model.move.z) * 0.07f;

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//ENTERキーを押した場合
		//ポリゴンの位置を初期化する
		g_Model.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//モデルの移動量を初期化する
		g_Model.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//ポリゴンの向きを初期化する
		g_Model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//目標の向きを初期化する
		g_Model.fRotDest = Camerarot.y;

		//向きの差分を初期化する
		g_Model.fRotDiff = 0.0f;
	}

	//移動していないように初期化する
	g_Model.bMove = false;

	//プレイヤーの当たり判定
	CollisionPlayer(&g_Model);

	//プレイヤーと塔の当たり判定
	CollisionTower(g_Model.VtxMaxModel, g_Model.VtxMinModel, &g_Model.pos, &g_Model.posOld, &g_Model.move, COLL_PLAYER);

	//プレイヤーの攻撃判定処理
	CollisionPlayerAttack(&g_Model);			

	//影の位置設定処理
	SetPositionShadow(g_Model.nShadow, g_Model.pos, g_Model.rot);
}

//=====================================
//モデルの描画処理
//=====================================
void DrawModel(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Model.mtxModel);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Model.rot.y, g_Model.rot.x, g_Model.rot.z);
	D3DXMatrixMultiply(&g_Model.mtxModel, &g_Model.mtxModel, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Model.pos.x, g_Model.pos.y, g_Model.pos.z);
	D3DXMatrixMultiply(&g_Model.mtxModel, &g_Model.mtxModel, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Model.mtxModel);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureModel[nCntMat]);

		//モデル(パーツ)の描画
		g_pMeshModel->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=======================================
//モデルの座標の取得処理
//=======================================
Model *GetModel(void)
{
	//モデルの情報を返す
	return &g_Model;
}

//=======================================
//プレイヤーの当たり判定
//=======================================
void CollisionPlayer(Model *pModel)
{
	if (pModel->pos.x >= 570.0f)
	{//右の壁にぶつかった場合
		//右から出ないようにする
		pModel->pos.x = 570.0f;
	}

	if (pModel->pos.x <= -570.0f)
	{//左の壁にぶつかった場合
		//左から出ないようにする
		pModel->pos.x = -570.0f;
	}

	if (pModel->pos.z >= 570.0f)
	{//奥の壁にぶつかった場合
		//奥から出ないようにする
		pModel->pos.z = 570.0f;
	}

	if (pModel->pos.z <= -570.0f)
	{//手前の壁にぶつかった場合
		//手前から出ないようにする
		pModel->pos.z = -570.0f;
	}
}

//=======================================
//プレイヤーの攻撃判定処理
//=======================================
void CollisionPlayerAttack(Model *pModel)
{
	Enemy *pEnemy = GetEnemy();

	for (int nCntEnm = 0; nCntEnm < MAX_ENEMY; nCntEnm++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//敵が使用されていた場合
			if (pModel->pos.x + pModel->VtxMinModel.x <= pEnemy->pos.x + pEnemy->VtxMaxEnemy.x && pModel->pos.x + pModel->VtxMaxModel.x >= pEnemy->pos.x + pEnemy->VtxMinEnemy.x)
			{//塔のX幅の中にいた場合
				if (pModel->posOld.z + pModel->VtxMaxModel.z <= pEnemy->posOld.z + pEnemy->VtxMinEnemy.z && pModel->pos.z + pModel->VtxMaxModel.z >= pEnemy->pos.z + pEnemy->VtxMinEnemy.z)
				{//前回の位置がブロックより手前かつ、現在の位置がブロックよりも奥かつだった場合(手前で止められる処理)
					//手前で止める
					pModel->pos.z = pEnemy->pos.z + pEnemy->VtxMinEnemy.z - pModel->VtxMaxModel.z;
				}							//手前で止められる処理
				else if (pModel->posOld.z + pModel->VtxMinModel.z >= pEnemy->posOld.z + pEnemy->VtxMaxEnemy.z && pModel->pos.z + pModel->VtxMinModel.z <= pEnemy->pos.z + pEnemy->VtxMaxEnemy.z)
				{//前回の位置が塔の位置よりも奥かつ、現在の位置が塔の位置よりも手前だった場合(奥で止められる処理)
					//奥で止める
					pModel->pos.z = pEnemy->pos.z + pEnemy->VtxMaxEnemy.z - pModel->VtxMinModel.z;
				}							//奥で止められる処理
			}

			if (pModel->pos.z + pModel->VtxMinModel.z <= pEnemy->pos.z + pEnemy->VtxMaxEnemy.z && pModel->pos.z + pModel->VtxMaxModel.z >= pEnemy->pos.z + pEnemy->VtxMinEnemy.z)
			{//塔のZ幅の中にいた場合
				if (pModel->posOld.x + pModel->VtxMaxModel.x <= pEnemy->posOld.x + pEnemy->VtxMinEnemy.x && pModel->pos.x + pModel->VtxMaxModel.x >= pEnemy->pos.x + pEnemy->VtxMinEnemy.x)
				{//前回の位置がブロックの左端より左かつ、現在の位置がブロックの左側より右だった場合(左の処理)
					//ブロックの左端に張り付かせる
					pModel->pos.x = pEnemy->pos.x + pEnemy->VtxMinEnemy.x - pModel->VtxMaxModel.x;
				}							//左端の処理
				else if (pModel->posOld.x + pModel->VtxMinModel.x >= pEnemy->posOld.x + pEnemy->VtxMaxEnemy.x && pModel->pos.x + pModel->VtxMinModel.x <= pEnemy->pos.x + pEnemy->VtxMaxEnemy.x)
				{//前回の位置がブロックの右端より右かつ、現在の位置がブロックの左側より右だった場合(右の処理)
					//ブロックの右端に張り付かせる
					pModel->pos.x = pEnemy->pos.x + pEnemy->VtxMaxEnemy.x - pModel->VtxMinModel.x;
				}							//右端の処理								//ジャンプしていなかった時
			}
		}
	}
}