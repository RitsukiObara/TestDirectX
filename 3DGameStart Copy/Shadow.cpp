//===========================================
//
//ポリゴンのメイン処理[Shadow.cpp]
//Author 小原立暉
//
//===========================================
#include "main.h"
#include "Shadow.h"

//マクロ定義
#define MAX_SHADOW					(256)			//影の最大数

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//頂点バッファへのポインタ
Shadow g_aShadow[MAX_SHADOW];						//影の情報
D3DXMATRIX g_mtxWorldShadow;						//ポリゴンのワールドマトリックス

//==========================================
//影の初期化処理
//==========================================
void InitShadow(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//下地のテクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/shadow000.jpg",
		&g_pTextureShadow);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{//情報の初期化
		//位置を初期化
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//向きを初期化
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//使用していない
		g_aShadow[nCntShadow].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBuff = 0; nCntBuff < MAX_SHADOW; nCntBuff++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-20.0f, 0.0f, +10.0f);
		pVtx[1].pos = D3DXVECTOR3(+20.0f, 0.0f, +10.0f);
		pVtx[2].pos = D3DXVECTOR3(-20.0f, 0.0f, -10.0f);
		pVtx[3].pos = D3DXVECTOR3(+20.0f, 0.0f, -10.0f);

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

		pVtx += 4;									//頂点データを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
}

//========================================
//影の終了処理
//========================================
void UninitShadow(void)
{
	//テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//========================================
//影の更新処理
//========================================
void UpdateShadow(void)
{

}

//=====================================
//影の描画処理
//=====================================
void DrawShadow(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	//減算合成
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{//影が使用されていた場合
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldShadow);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldShadow);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0,
				g_pVtxBuffShadow,									//頂点バッファへのポインタ
				0,
				sizeof(VERTEX_3D));									//頂点情報構造体のサイズ

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
				nCntShadow * 4,										//描画する最初の頂点インデックス
				2);													//描画するプリミティブ数
		}
	}

	//通常の合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===============================
//影の設定処理
//===============================
int SetShadow(int nType)
{
	//影の番号を初期化する
	int nIdxShadow = -1;

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == false)
		{//使用していなかった場合

			switch (nType)
			{
			case SHADOW_PLAYER:				//プレイヤーの影

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-20.0f, 0.0f, +10.0f);
				pVtx[1].pos = D3DXVECTOR3(+20.0f, 0.0f, +10.0f);
				pVtx[2].pos = D3DXVECTOR3(-20.0f, 0.0f, -10.0f);
				pVtx[3].pos = D3DXVECTOR3(+20.0f, 0.0f, -10.0f);

				break;						//抜け出す

			case SHADOW_ENEMY:				//敵の影

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-20.0f, 0.0f, +20.0f);
				pVtx[1].pos = D3DXVECTOR3(+20.0f, 0.0f, +20.0f);
				pVtx[2].pos = D3DXVECTOR3(-20.0f, 0.0f, -20.0f);
				pVtx[3].pos = D3DXVECTOR3(+20.0f, 0.0f, -20.0f);

				break;						//抜け出す

			case SHADOW_TOWER:				//塔の影

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-230.0f, 0.0f, +230.0f);
				pVtx[1].pos = D3DXVECTOR3(+230.0f, 0.0f, +230.0f);
				pVtx[2].pos = D3DXVECTOR3(-230.0f, 0.0f, -230.0f);
				pVtx[3].pos = D3DXVECTOR3(+230.0f, 0.0f, -230.0f);

				break;						//抜け出す

			case SHADOW_BULLET:				//弾の影

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-10.0f, 0.0f, +10.0f);
				pVtx[1].pos = D3DXVECTOR3(+10.0f, 0.0f, +10.0f);
				pVtx[2].pos = D3DXVECTOR3(-10.0f, 0.0f, -10.0f);
				pVtx[3].pos = D3DXVECTOR3(+10.0f, 0.0f, -10.0f);

				break;						//抜け出す
			}

			//位置を設定する
			g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//向きを設定する
			g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//使用する
			g_aShadow[nCntShadow].bUse = true;

			//影の番号をコピーする
			nIdxShadow = nCntShadow;

			break;				//抜け出す
		}
		pVtx += 4;				//頂点データを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();

	//影の番号を返す
	return nIdxShadow;
}

//===============================
//影の位置設定処理
//===============================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (g_aShadow[nIdxShadow].bUse == true)
	{//使用されていた場合
		//位置を設定する
		g_aShadow[nIdxShadow].pos.x = pos.x;
		g_aShadow[nIdxShadow].pos.z = pos.z;

		//向きを設定する
		g_aShadow[nIdxShadow].rot.y = rot.y;
	}
}

//================================
//影のリセット処理
//================================
void ResetShadow(int *nIdxShadow)
{
	if (g_aShadow[*nIdxShadow].bUse == true)
	{//影が使用されていた場合
		//使用をやめる
		g_aShadow[*nIdxShadow].bUse = false;

		//影の番号を-1に戻す
		*nIdxShadow = -1;
	}
}