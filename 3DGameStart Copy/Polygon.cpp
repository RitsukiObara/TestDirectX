//===========================================
//
//ポリゴンのメイン処理[Polygon.cpp]
//Author 小原立暉
//
//===========================================
#include"main.h"
#include "Polygon.h"
#include "input.h"

//マクロ定義

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePolygon = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;	//頂点バッファへのポインタ
D3DXVECTOR3 g_posPolygon;							//位置
D3DXVECTOR3 g_rotPolygon;							//向き
D3DXMATRIX g_mtxWorldPolygon;						//ポリゴンのワールドマトリックス

//==========================================
//ポリゴンの初期化処理
//==========================================
void InitPolygon(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//下地のテクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Block.png",
		&g_pTexturePolygon);

	//ポリゴンの位置を初期化する
	g_posPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//ポリゴンの向きを初期化する
	g_rotPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-600.0f, 0.0f, +600.0f);
	pVtx[1].pos = D3DXVECTOR3(+600.0f, 0.0f, +600.0f);
	pVtx[2].pos = D3DXVECTOR3(-600.0f, 0.0f, -600.0f);
	pVtx[3].pos = D3DXVECTOR3(+600.0f, 0.0f, -600.0f);

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

	//頂点バッファをアンロックする
	g_pVtxBuffPolygon->Unlock();
}

//========================================
//ポリゴンの終了処理
//========================================
void UninitPolygon(void)
{
	//テクスチャの破棄
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//========================================
//ポリゴンの更新処理
//========================================
void UpdatePolygon(void)
{
	
}

//=====================================
//ポリゴンの描画処理
//=====================================
void DrawPolygon(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldPolygon);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotPolygon.y, g_rotPolygon.x, g_rotPolygon.z);
	D3DXMatrixMultiply(&g_mtxWorldPolygon, &g_mtxWorldPolygon, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posPolygon.x, g_posPolygon.y, g_posPolygon.z);
	D3DXMatrixMultiply(&g_mtxWorldPolygon, &g_mtxWorldPolygon, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPolygon);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffPolygon,									//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_3D));									//頂点情報構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePolygon);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
		0,													//描画する最初の頂点インデックス
		2);													//描画するプリミティブ数
}