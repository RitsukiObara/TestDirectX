//===========================================
//
//爆発のメイン処理[Explosion.cpp]
//Author 小原立暉
//
//===========================================
#include"main.h"
#include "Explosion.h"
#include "Particle.h"

//マクロ定義
#define MAX_EXPLOSION			(128)				//ビルボードの最大数
#define EXPL_RADIUS				(30.0f)				//爆発の半径
#define EXPL_ANIM_COUNT			(6)					//爆発アニメーションのカウント
#define EXPL_ANIM_PATTERN		(0.125f)			//爆発アニメーションのパターン数
#define EXPL_LIFE				(60)				//爆発の寿命

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	//頂点バッファへのポインタ
Explosion g_aExplosion[MAX_EXPLOSION];				//ビルボードの情報

//==========================================
//爆発の初期化処理
//==========================================
void InitExplosion(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//下地のテクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/explosion000.png",
		&g_pTextureExplosion);

	for (int nCntSet = 0; nCntSet < MAX_EXPLOSION; nCntSet++)
	{
		//位置の初期化
		g_aExplosion[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//アニメカウンターを初期化する
		g_aExplosion[nCntSet].nAnimCounter = 0;

		//アニメパターンNo.を初期化する
		g_aExplosion[nCntSet].nAnimPattern = 0;

		//使用していない
		g_aExplosion[nCntSet].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-EXPL_RADIUS, +EXPL_RADIUS, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+EXPL_RADIUS, +EXPL_RADIUS, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-EXPL_RADIUS, -EXPL_RADIUS, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+EXPL_RADIUS, -EXPL_RADIUS, 0.0f);

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
	g_pVtxBuffExplosion->Unlock();
}

//========================================
//ビルボードの終了処理
//========================================
void UninitExplosion(void)
{
	//テクスチャの破棄
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//========================================
//爆発の更新処理
//========================================
void UpdateExplosion(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_EXPLOSION; nCntBill++)
	{
		if (g_aExplosion[nCntBill].bUse == true)
		{//使用されていた場合
			//寿命を減らす
			g_aExplosion[nCntBill].nLife--;

			//アニメカウンターを加算する
			g_aExplosion[nCntBill].nAnimCounter++;

			if (g_aExplosion[nCntBill].nAnimCounter == EXPL_ANIM_COUNT)
			{//アニメーションカウンターが6になった場合
				//アニメカウンターを0にする
				g_aExplosion[nCntBill].nAnimCounter = 0;

				//アニメーションパターンNo.を加算する
				g_aExplosion[nCntBill].nAnimPattern = (g_aExplosion[nCntBill].nAnimPattern + 1) % 8;
			}

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (g_aExplosion[nCntBill].nAnimPattern * EXPL_ANIM_PATTERN), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(EXPL_ANIM_PATTERN + (g_aExplosion[nCntBill].nAnimPattern * EXPL_ANIM_PATTERN), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (g_aExplosion[nCntBill].nAnimPattern * EXPL_ANIM_PATTERN), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(EXPL_ANIM_PATTERN + (g_aExplosion[nCntBill].nAnimPattern * EXPL_ANIM_PATTERN), 1.0f);

			if (g_aExplosion[nCntBill].nLife <= 0 && g_aExplosion[nCntBill].bUse == true)
			{//寿命が0以下になった場合
				 //使用していない
				g_aExplosion[nCntBill].bUse = false;
			}
		}
		pVtx += 4;					//頂点データを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//=====================================
//爆発の描画処理
//=====================================
void DrawExplosion(void)
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
	pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);				//アルファテストの参照値設定

	for (int nCntDraw = 0; nCntDraw < MAX_EXPLOSION; nCntDraw++)
	{
		if (g_aExplosion[nCntDraw].bUse == true)
		{//使用していた場合
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aExplosion[nCntDraw].mtx);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに向けて正面に向ける
			D3DXMatrixInverse(&g_aExplosion[nCntDraw].mtx, NULL, &mtxView);

			//逆行列を求める
			g_aExplosion[nCntDraw].mtx._41 = 0.0f;
			g_aExplosion[nCntDraw].mtx._42 = 0.0f;
			g_aExplosion[nCntDraw].mtx._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aExplosion[nCntDraw].pos.x, g_aExplosion[nCntDraw].pos.y, g_aExplosion[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_aExplosion[nCntDraw].mtx, &g_aExplosion[nCntDraw].mtx, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aExplosion[nCntDraw].mtx);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0,
				g_pVtxBuffExplosion,								//頂点バッファへのポインタ
				0,
				sizeof(VERTEX_3D));									//頂点情報構造体のサイズ

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureExplosion);

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
//爆発の設定処理
//======================================
void SetExplosion(D3DXVECTOR3 pos)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_EXPLOSION; nCntBill++)
	{
		if (g_aExplosion[nCntBill].bUse == false)
		{//使用されていなかった場合
			//位置を設定する
			g_aExplosion[nCntBill].pos = pos;

			//アニメカウンターを設定する
			g_aExplosion[nCntBill].nAnimCounter = 0;

			//アニメパターンNo.を設定する
			g_aExplosion[nCntBill].nAnimPattern = 0;

			//寿命を設定する
			g_aExplosion[nCntBill].nLife = EXPL_LIFE;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-EXPL_RADIUS, +EXPL_RADIUS, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+EXPL_RADIUS, +EXPL_RADIUS, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-EXPL_RADIUS, -EXPL_RADIUS, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+EXPL_RADIUS, -EXPL_RADIUS, 0.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (g_aExplosion[nCntBill].nAnimPattern * 0.125f), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f + (g_aExplosion[nCntBill].nAnimPattern * 0.125f), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (g_aExplosion[nCntBill].nAnimPattern * 0.125f), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f + (g_aExplosion[nCntBill].nAnimPattern * 0.125f), 1.0f);

			//パーティクルの設定処理
			SetParticle(g_aExplosion[nCntBill].pos, PTCL_EXPL);

			//使用する
			g_aExplosion[nCntBill].bUse = true;

			break;									//抜け出す
		}
		pVtx += 4;									//頂点データを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}