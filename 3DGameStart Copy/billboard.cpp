//===========================================
//
//ビルボードのメイン処理[Billboard.cpp]
//Author 小原立暉
//
//===========================================
#include"main.h"
#include "billboard.h"
#include "input.h"
#include "Shadow.h"
#include "Effect.h"
#include "Particle.h"
#include "wall.h"

//マクロ定義
#define MAX_BILLBOARD			(256)				//ビルボードの最大数

//プロトタイプ宣言
void CollisionWall(Billboard *Billboard);		//壁の当たり判定

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBillboard[BILLBOARD_MAX] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;	//頂点バッファへのポインタ
Billboard g_aBillboard[MAX_BILLBOARD];				//ビルボードの情報

//テクスチャファイル名
const char *c_apFilenameBillboard[BILLBOARD_MAX] = 
{
	"data/TEXTURE/explosion000.png",				//爆発のテクスチャ
	"data/TEXTURE/bullet000.png"					//弾のテクスチャ
};

//==========================================
//ビルボードの初期化処理
//==========================================
void InitBillboard(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntTexture = 0; nCntTexture < BILLBOARD_MAX; nCntTexture++)
	{//テクスチャの設定
		//下地のテクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameBillboard[nCntTexture],
			&g_pTextureBillboard[nCntTexture]);
	}
	
	for (int nCntSet = 0; nCntSet < MAX_BILLBOARD; nCntSet++)
	{
		//位置の初期化
		g_aBillboard[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//向きの初期化
		g_aBillboard[nCntSet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//影の番号を初期化する
		g_aBillboard[nCntSet].nShadow = -1;

		//アニメカウンターを初期化する
		g_aBillboard[nCntSet].nAnimCounter = 0;

		//アニメパターンNo.を初期化する
		g_aBillboard[nCntSet].nAnimPattern = 0;

		//使用していない
		g_aBillboard[nCntSet].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-10.0f, +10.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+10.0f, +10.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-10.0f, -10.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+10.0f, -10.0f, 0.0f);

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

		pVtx += 4;						//頂点データを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}

//========================================
//ビルボードの終了処理
//========================================
void UninitBillboard(void)
{
	for (int nCntBill = 0; nCntBill < BILLBOARD_MAX; nCntBill++)
	{
		//テクスチャの破棄
		if (g_pTextureBillboard[nCntBill] != NULL)
		{
			g_pTextureBillboard[nCntBill]->Release();
			g_pTextureBillboard[nCntBill] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//========================================
//ビルボードの更新処理
//========================================
void UpdateBillboard(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		if (g_aBillboard[nCntBill].bUse == true)
		{//使用されていた場合
			//影の位置の設定処理
			SetPositionShadow(g_aBillboard[nCntBill].nShadow, g_aBillboard[nCntBill].pos, g_aBillboard[nCntBill].rot);

			//寿命を減らす
			g_aBillboard[nCntBill].nLife--;

			switch (g_aBillboard[nCntBill].Type)
			{
			case BILLBOARD_BULLET:				//弾の場合

				//上に加速する
				g_aBillboard[nCntBill].pos.x += sinf(g_aBillboard[nCntBill].rot.y) * 4.0f;
				g_aBillboard[nCntBill].pos.z += cosf(g_aBillboard[nCntBill].rot.y) * 4.0f;

				//エフェクトの設定処理
				SetEffect(g_aBillboard[nCntBill].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 8.0f, 10, EFFECT_BULLET);

				//壁の当たり判定
				CollisionWall(&g_aBillboard[nCntBill]);

				break;							//抜け出す

			case BILLBOARD_EXPL:				//爆発の場合

				//アニメカウンターを加算する
				g_aBillboard[nCntBill].nAnimCounter++;

				if (g_aBillboard[nCntBill].nAnimCounter == 6)
				{//アニメーションカウンターが6になった場合
					//アニメカウンターを0にする
					g_aBillboard[nCntBill].nAnimCounter = 0;

					//アニメーションパターンNo.を加算する
					g_aBillboard[nCntBill].nAnimPattern = (g_aBillboard[nCntBill].nAnimPattern + 1) % 8;
				}

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f + (g_aBillboard[nCntBill].nAnimPattern * 0.125f), 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.125f + (g_aBillboard[nCntBill].nAnimPattern * 0.125f), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + (g_aBillboard[nCntBill].nAnimPattern * 0.125f), 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.125f + (g_aBillboard[nCntBill].nAnimPattern * 0.125f), 1.0f);

				break;							//抜け出す
			}

			if (g_aBillboard[nCntBill].nLife <= 0 && g_aBillboard[nCntBill].bUse == true)
			{//寿命が0以下になった場合
				if (g_aBillboard[nCntBill].Type != BILLBOARD_EXPL)
				{//爆発のビルボード以外の場合
					//爆発のビルボードを設定
					SetBillboard(g_aBillboard[nCntBill].rot, g_aBillboard[nCntBill].pos, BILLBOARD_EXPL);

					//使用していない
					g_aBillboard[nCntBill].bUse = false;

					//影のリセット処理
					ResetShadow(&g_aBillboard[nCntBill].nShadow);
				}
				else
				{//爆発のビルボードの場合
					//使用していない
					g_aBillboard[nCntBill].bUse = false;
				}
			}
		}
		pVtx += 4;					//頂点データを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}

//=====================================
//ビルボードの描画処理
//=====================================
void DrawBillboard(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	D3DXMATRIX mtxTrans;			//計算用マトリックス
	D3DXMATRIX mtxView;				//ビューマトリックス取得用

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Zテストの有効/無効設定

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);					//アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);				//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 75);							//アルファテストの参照値設定

	for (int nCntDraw = 0; nCntDraw < MAX_BILLBOARD; nCntDraw++)
	{
		if (g_aBillboard[nCntDraw].bUse == true)
		{//使用していた場合
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBillboard[nCntDraw].mtx);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに向けて正面に向ける
			D3DXMatrixInverse(&g_aBillboard[nCntDraw].mtx, NULL, &mtxView);

			//逆行列を求める
			g_aBillboard[nCntDraw].mtx._41 = 0.0f;
			g_aBillboard[nCntDraw].mtx._42 = 0.0f;
			g_aBillboard[nCntDraw].mtx._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCntDraw].pos.x, g_aBillboard[nCntDraw].pos.y, g_aBillboard[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_aBillboard[nCntDraw].mtx, &g_aBillboard[nCntDraw].mtx, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCntDraw].mtx);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0,
				g_pVtxBuffBillboard,								//頂点バッファへのポインタ
				0,
				sizeof(VERTEX_3D));									//頂点情報構造体のサイズ

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBillboard[g_aBillboard[nCntDraw].Type]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
				4 * nCntDraw,										//描画する最初の頂点インデックス
				2);													//描画するプリミティブ数
		}
	}

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			//アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						//アルファテストの参照値設定

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				//Zテストの有効/無効設定
}

//======================================
//ビルボードの設定処理
//======================================
void SetBillboard(D3DXVECTOR3 rot, D3DXVECTOR3 pos, int nType)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		if (g_aBillboard[nCntBill].bUse == false)
		{//使用されていなかった場合
			//位置を設定する
			g_aBillboard[nCntBill].pos = pos;

			//向きを設定する
			g_aBillboard[nCntBill].rot = rot;

			//種類を設定する
			g_aBillboard[nCntBill].Type = nType;

			//アニメカウンターを設定する
			g_aBillboard[nCntBill].nAnimCounter = 0;

			//アニメパターンNo.を設定する
			g_aBillboard[nCntBill].nAnimPattern = 0;

			if (g_aBillboard[nCntBill].Type != BILLBOARD_EXPL)
			{//爆発のビルボード以外だった場合
				//影を設定する
				g_aBillboard[nCntBill].nShadow = SetShadow(SHADOW_BULLET);
			}

			switch (g_aBillboard[nCntBill].Type)
			{
			case BILLBOARD_BULLET:					//弾の場合

				//寿命を設定する
				g_aBillboard[nCntBill].nLife = 100;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-10.0f, 10.0f, -0.0f);
				pVtx[1].pos = D3DXVECTOR3(10.0f, 10.0f, -0.0f);
				pVtx[2].pos = D3DXVECTOR3(-10.0f, -10.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(10.0f, -10.0f, 0.0f);

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				break;								//抜け出す

			case BILLBOARD_EXPL:					//爆発だった場合

				//寿命を設定する
				g_aBillboard[nCntBill].nLife = 60;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-30.0f, 30.0f, -0.0f);
				pVtx[1].pos = D3DXVECTOR3(30.0f, 30.0f, -0.0f);
				pVtx[2].pos = D3DXVECTOR3(-30.0f, -30.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(30.0f, -30.0f, 0.0f);

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f + (g_aBillboard[nCntBill].nAnimPattern * 0.125f), 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.125f + (g_aBillboard[nCntBill].nAnimPattern * 0.125f), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + (g_aBillboard[nCntBill].nAnimPattern * 0.125f), 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.125f + (g_aBillboard[nCntBill].nAnimPattern * 0.125f), 1.0f);

				//パーティクルの設定処理
				SetParticle(g_aBillboard[nCntBill].pos, PTCL_EXPL);

				break;								//抜け出す
			}

			//使用する
			g_aBillboard[nCntBill].bUse = true;

			break;									//抜け出す
		}
		pVtx += 4;									//頂点データを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}

//================================
//壁の当たり判定
//================================
void CollisionWall(Billboard *Billboard)
{
	if (Billboard->pos.x >= 300.0f ||
		Billboard->pos.x <= -300.0f ||
		Billboard->pos.z >= 300.0f ||
		Billboard->pos.z <= -300.0f)
	{//弾がステージの外に出た場合
		//爆発のビルボードを設定
		SetBillboard(Billboard->rot, Billboard->pos, BILLBOARD_EXPL);

		//使用していない
		Billboard->bUse = false;

		//影のリセット処理
		ResetShadow(&Billboard->nShadow);
	}
}