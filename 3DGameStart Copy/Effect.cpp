//=======================================
//
//エフェクトのメイン処理[Effect.cpp]
//Author 小原立暉
//
//=======================================
#include"main.h"
#include"Effect.h"

//マクロ定義
#define MAX_EFFECT			(16384)		//エフェクトの最大数

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 move;					//移動量
	D3DXMATRIX mtx;						//ワールドマトリックス
	D3DXCOLOR col;						//色
	float fRadius;						//半径
	int nType;							//エフェクトの種類
	int nLife;							//寿命
	bool bUse;							//使用しているかどうか
}Effect;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	//頂点バッファへのポインタ
Effect g_aEffect[MAX_EFFECT];			//エフェクトの情報

//==========================================
//エフェクトの初期化処理
//==========================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data//TEXTURE//shadow000.jpg",
		&g_pTextureEffect);

	//爆発の情報の初期化
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//位置を初期化する

		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//色を初期化する

		g_aEffect[nCntEffect].nType = EFFECT_BULLET;						//弾の種類を初期化する

		g_aEffect[nCntEffect].fRadius = 0.0f;								//半径を初期化する

		g_aEffect[nCntEffect].nLife = 100;									//寿命を初期化する

		g_aEffect[nCntEffect].bUse = false;									//使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

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
	g_pVtxBuffEffect->Unlock();
}

//========================================
//エフェクトの終了処理
//========================================
void UninitEffect(void)
{
	//テクスチャの破棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//========================================
//エフェクトの更新処理
//========================================
void UpdateEffect(void)
{
	int nCntEffect;					//回数の変数を宣言する

	VERTEX_3D * pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//爆発が使用されている
			//寿命を減らしていく
			g_aEffect[nCntEffect].nLife--;

			//移動する
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

			switch (g_aEffect[nCntEffect].nType)
			{
			case EFFECT_BULLET:			//弾の場合

				//透明度(α値)を薄くしていく
				g_aEffect[nCntEffect].col.a -= 0.1f;

				//半径を減らしていく(縮めていく)
				g_aEffect[nCntEffect].fRadius -= (g_aEffect[nCntEffect].fRadius / 10.0f);

				break;					//抜け出す

			case EFFECT_EXPL:			//爆発の場合

				if (g_aEffect[nCntEffect].nLife % 2 == 0)
				{//寿命が一定数減った場合
					//透明度(α値)を薄くしていく
					g_aEffect[nCntEffect].col.a -= 0.1f;

					//半径を減らしていく(縮めていく)
					g_aEffect[nCntEffect].fRadius -= (g_aEffect[nCntEffect].fRadius / 10.0f);
				}

				break;					//抜け出す

			case EFFECT_WALK:			//歩いていた時のエフェクト

				//半径を減らしていく(縮めていく)
				g_aEffect[nCntEffect].fRadius -= (g_aEffect[nCntEffect].fRadius / 10.0f);

				break;					//抜けだす
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			if (g_aEffect[nCntEffect].nLife == 0)
			{//エフェクトの寿命が尽きた時
				//使用していないことにする
				g_aEffect[nCntEffect].bUse = false;
			}
		}
		pVtx += 4;//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//=====================================
//エフェクトの描画処理
//=====================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;

	D3DXMATRIX mtxTrans;			//計算用マトリックス
	D3DXMATRIX mtxView;				//ビューマトリックス取得用

	//デバイスの取得
	pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Zテストの有効/無効設定

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);					//アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);				//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);				//アルファテストの参照値設定

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている
			if (g_aEffect[nCntEffect].nType == EFFECT_WALK)
			{//黒系の飛沫
				//減算合成に設定
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}
			else
			{//白系の飛沫
				//αブレンディングを加算処理に設定
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtx);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに向けて正面に向ける
			D3DXMatrixInverse(&g_aEffect[nCntEffect].mtx, NULL, &mtxView);

			//逆行列を求める
			g_aEffect[nCntEffect].mtx._41 = 0.0f;
			g_aEffect[nCntEffect].mtx._42 = 0.0f;
			g_aEffect[nCntEffect].mtx._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtx, &g_aEffect[nCntEffect].mtx, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtx);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0,
				g_pVtxBuffEffect,										//頂点バッファへのポインタ
				0,
				sizeof(VERTEX_3D));										//頂点情報構造体のサイズ

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
				4 * nCntEffect,									//描画する最初の頂点インデックス
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
//エフェクトの設定処理
//========================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRadius, int nLife, int nType)
{
	int nCntEffect;

	VERTEX_3D * pVtx;					//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//弾が使用されていない
			//位置を設定
			g_aEffect[nCntEffect].pos = pos;

			//移動量を設定
			g_aEffect[nCntEffect].move = move;

			//種類を設定
			g_aEffect[nCntEffect].nType = nType;

			switch (g_aEffect[nCntEffect].nType)
			{
			case EFFECT_BULLET:				//弾の場合
				//シアン掛かった白色に設定
				g_aEffect[nCntEffect].col = D3DXCOLOR(0.7f, 1.0f, 1.0f, 1.0f);

				break;						//抜け出す

			case EFFECT_EXPL:				//爆発の場合
				//赤色に設定
				g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f);

				break;						//抜け出す

			case EFFECT_WALK:				//歩いた時の煙
				//灰色に設定
				g_aEffect[nCntEffect].col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

				break;						//抜け出す
			}

			//頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			//半径を設定
			g_aEffect[nCntEffect].fRadius = fRadius;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			//寿命を設定
			g_aEffect[nCntEffect].nLife = nLife;

			//使用している状態にする
			g_aEffect[nCntEffect].bUse = true;				//使用している状態にする

			break;
		}
		pVtx += 4;//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}