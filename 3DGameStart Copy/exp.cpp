//=======================================
//
//経験値のメイン処理[Exp.cpp]
//Author 小原立暉
//
//=======================================
#include "main.h"
#include "exp.h"
#include "model.h"

//マクロ定義
#define MAX_EXP			(1024)		//エフェクトの最大数

//プロトタイプ宣言
void CollisionExperience(Model *pModel, Exp *pExp);		//経験値の当たり判定
void GetExperience(Model * pModel, Exp *pExp);			//経験値の取得処理

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureExp = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExp = NULL;	//頂点バッファへのポインタ
Exp g_aExp[MAX_EXP];							//経験値の情報

//==========================================
//経験値の初期化処理
//==========================================
void InitExp(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExp;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data//TEXTURE//shadow000.jpg",
		&g_pTextureExp);

	//爆発の情報の初期化
	for (nCntExp = 0; nCntExp < MAX_EXP; nCntExp++)
	{
		g_aExp[nCntExp].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//位置を初期化する

		g_aExp[nCntExp].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きを初期化する

		g_aExp[nCntExp].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量を初期化する

		g_aExp[nCntExp].fRadius = 0.0f;									//半径を初期化する

		g_aExp[nCntExp].nLife = 100;									//寿命を初期化する

		g_aExp[nCntExp].bUse = false;									//使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EXP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExp,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffExp->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExp = 0; nCntExp < MAX_EXP; nCntExp++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aExp[nCntExp].fRadius, +g_aExp[nCntExp].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+g_aExp[nCntExp].fRadius, +g_aExp[nCntExp].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aExp[nCntExp].fRadius, -g_aExp[nCntExp].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+g_aExp[nCntExp].fRadius, -g_aExp[nCntExp].fRadius, 0.0f);

		//法線ベクトルの設定
		pVtx[0].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;			//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExp->Unlock();
}

//========================================
//経験値の終了処理
//========================================
void UninitExp(void)
{
	//テクスチャの破棄
	if (g_pTextureExp != NULL)
	{
		g_pTextureExp->Release();
		g_pTextureExp = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffExp != NULL)
	{
		g_pVtxBuffExp->Release();
		g_pVtxBuffExp = NULL;
	}
}

//========================================
//経験値の更新処理
//========================================
void UpdateExp(void)
{
	int nCntExp;					//回数の変数を宣言する
	Model *pModel = GetModel();		//プレイヤーの情報を取得する

	VERTEX_3D * pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExp->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExp = 0; nCntExp < MAX_EXP; nCntExp++)
	{
		if (g_aExp[nCntExp].bUse == true)
		{//経験値が使用されている
			//寿命を減らしていく
			g_aExp[nCntExp].nLife--;

			float fRotDest,			//目標の向き
				  fRotDiff;			//目標までの差分

			//目的の向きを設定する
			fRotDest = atan2f(pModel->pos.x - g_aExp[nCntExp].pos.x, pModel->pos.z - g_aExp[nCntExp].pos.z);

			//向きの差分を求める
			fRotDiff = fRotDest - g_aExp[nCntExp].rot.y * 0.8f;

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
			g_aExp[nCntExp].rot.y += fRotDiff;

			if (g_aExp[nCntExp].rot.y > D3DX_PI)
			{//角度が3.14fより大きかった場合
				//角度から1周分減らす
				g_aExp[nCntExp].rot.y = -D3DX_PI;
			}
			else if (g_aExp[nCntExp].rot.y < -D3DX_PI)
			{//角度が-3.14fより小さかった場合
				//角度に1周分加える
				g_aExp[nCntExp].rot.y = D3DX_PI;
			}

			//上に加速する
			g_aExp[nCntExp].move.x = sinf(g_aExp[nCntExp].rot.y) * 10.0f;
			g_aExp[nCntExp].move.z = cosf(g_aExp[nCntExp].rot.y) * 10.0f;

			//位置を更新する
			g_aExp[nCntExp].pos.x += g_aExp[nCntExp].move.x;
			g_aExp[nCntExp].pos.z += g_aExp[nCntExp].move.z;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aExp[nCntExp].fRadius, +g_aExp[nCntExp].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aExp[nCntExp].fRadius, +g_aExp[nCntExp].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aExp[nCntExp].fRadius, -g_aExp[nCntExp].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aExp[nCntExp].fRadius, -g_aExp[nCntExp].fRadius, 0.0f);

			if (g_aExp[nCntExp].nLife == 0)
			{//エフェクトの寿命が尽きた時
				//使用していないことにする
				g_aExp[nCntExp].bUse = false;
			}

			//経験値の当たり判定
			CollisionExperience(pModel, &g_aExp[nCntExp]);
		}
		pVtx += 4;//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExp->Unlock();
}

//=====================================
//経験値の描画処理
//=====================================
void DrawExp(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExp;

	D3DXMATRIX mtxTrans;			//計算用マトリックス
	D3DXMATRIX mtxView;				//ビューマトリックス取得用

									//デバイスの取得
	pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//αブレンディングを加算処理に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Zテストの有効/無効設定

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);					//アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);				//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);				//アルファテストの参照値設定

	for (nCntExp = 0; nCntExp < MAX_EXP; nCntExp++)
	{
		if (g_aExp[nCntExp].bUse == true)
		{//エフェクトが使用されている
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aExp[nCntExp].mtx);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに向けて正面に向ける
			D3DXMatrixInverse(&g_aExp[nCntExp].mtx, NULL, &mtxView);

			//逆行列を求める
			g_aExp[nCntExp].mtx._41 = 0.0f;
			g_aExp[nCntExp].mtx._42 = 0.0f;
			g_aExp[nCntExp].mtx._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aExp[nCntExp].pos.x, g_aExp[nCntExp].pos.y, g_aExp[nCntExp].pos.z);
			D3DXMatrixMultiply(&g_aExp[nCntExp].mtx, &g_aExp[nCntExp].mtx, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aExp[nCntExp].mtx);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0,
				g_pVtxBuffExp,									//頂点バッファへのポインタ
				0,
				sizeof(VERTEX_3D));								//頂点情報構造体のサイズ

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureExp);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
				4 * nCntExp,									//描画する最初の頂点インデックス
				2);												//描画するプリミティブ数
		}
	}

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			//アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						//アルファテストの参照値設定

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				//Zテストの有効/無効設定

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//========================================
//経験値の設定処理
//========================================
void SetExp(D3DXVECTOR3 pos)
{
	int nCntExp;

	VERTEX_3D * pVtx;					//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExp->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExp = 0; nCntExp < MAX_EXP; nCntExp++)
	{
		if (g_aExp[nCntExp].bUse == false)
		{//弾が使用されていない
			//位置を設定
			g_aExp[nCntExp].pos = pos;

			//移動量を設定
			g_aExp[nCntExp].move = D3DXVECTOR3();

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.2f,1.0f,0.1f,1.0f);
			pVtx[1].col = D3DXCOLOR(0.2f,1.0f,0.1f,1.0f);
			pVtx[2].col = D3DXCOLOR(0.2f,1.0f,0.1f,1.0f);
			pVtx[3].col = D3DXCOLOR(0.2f,1.0f,0.1f,1.0f);

			//半径を設定
			g_aExp[nCntExp].fRadius = 5.0f;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aExp[nCntExp].fRadius, +g_aExp[nCntExp].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aExp[nCntExp].fRadius, +g_aExp[nCntExp].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aExp[nCntExp].fRadius, -g_aExp[nCntExp].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aExp[nCntExp].fRadius, -g_aExp[nCntExp].fRadius, 0.0f);

			//寿命を設定
			g_aExp[nCntExp].nLife = 500;

			//使用している状態にする
			g_aExp[nCntExp].bUse = true;				//使用している状態にする

			break;
		}
		pVtx += 4;//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExp->Unlock();
}

//========================================
//経験値の当たり判定
//========================================
void CollisionExperience(Model *pModel, Exp *pExp)
{
	if (pExp->bUse == true)
	{//経験値が使用されていた場合
		if (pExp->pos.x + pExp->fRadius <= pModel->pos.x + pModel->VtxMaxModel.x&&
			pExp->pos.x + pExp->fRadius >= pModel->pos.x + pModel->VtxMinModel.x&&
			pExp->pos.z + pExp->fRadius <= pModel->pos.z + pModel->VtxMaxModel.z&&
			pExp->pos.z + pExp->fRadius >= pModel->pos.z + pModel->VtxMinModel.z)
		{//敵が塔の中に入った場合
			//経験値の取得処理
			GetExperience(pModel, pExp);
		}
	}
}

//=========================================
//経験値の取得処理
//=========================================
void GetExperience(Model * pModel, Exp *pExp)
{
	//経験値を取得する
	pExp->bUse = false;

	//レベルを上げる
	pModel->nPoint++;
}