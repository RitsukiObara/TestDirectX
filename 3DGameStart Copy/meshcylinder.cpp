//===========================================
//
//メッシュシリンダーのメイン処理[meshcylinder.cpp]
//Author 小原立暉
//
//===========================================
#include "main.h"
#include "meshcylinder.h"

//マクロ定義
#define MAX_CYLINDER_TOP			(18)				//頂点数
#define MAX_CYLINDER_INDEX			(18)				//インデックス数
#define MAX_CYLINDERLENGTH			(800.0f)			//中心からの背景の距離

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturemeshCylinder = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffmeshCylinder = NULL;		//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL;		//インデックスバッファへのポインタ
D3DXVECTOR3 g_posmeshCylinder;								//位置
D3DXMATRIX g_mtxWorldmeshCylinder;							//ポリゴンのワールドマトリックス

//==========================================
//メッシュフィールドの初期化処理
//==========================================
void InitmeshCylinder(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//下地のテクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Block.png",
		&g_pTexturemeshCylinder);

	//位置の初期化
	g_posmeshCylinder = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_CYLINDER_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffmeshCylinder,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffmeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-sinf(0.0f) * MAX_CYLINDERLENGTH, BG_HEIGHT, -cosf(0.0f) * MAX_CYLINDERLENGTH);
	pVtx[1].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * MAX_CYLINDERLENGTH, BG_HEIGHT, -cosf(D3DX_PI * 0.25f) * MAX_CYLINDERLENGTH);
	pVtx[2].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * MAX_CYLINDERLENGTH, BG_HEIGHT, -cosf(D3DX_PI * 0.5f) * MAX_CYLINDERLENGTH);
	pVtx[3].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * MAX_CYLINDERLENGTH, BG_HEIGHT, -cosf(D3DX_PI * 0.75f) * MAX_CYLINDERLENGTH);
	pVtx[4].pos = D3DXVECTOR3(sinf(0.0f) * MAX_CYLINDERLENGTH, BG_HEIGHT, cosf(0.0f) * MAX_CYLINDERLENGTH);
	pVtx[5].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * MAX_CYLINDERLENGTH, BG_HEIGHT, -cosf(-D3DX_PI * 0.75f) * MAX_CYLINDERLENGTH);
	pVtx[6].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * MAX_CYLINDERLENGTH, BG_HEIGHT, -cosf(-D3DX_PI * 0.5f) * MAX_CYLINDERLENGTH);
	pVtx[7].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * MAX_CYLINDERLENGTH, BG_HEIGHT, -cosf(-D3DX_PI * 0.25f) * MAX_CYLINDERLENGTH);
	pVtx[8].pos = D3DXVECTOR3(-sinf(0.0f) * MAX_CYLINDERLENGTH, BG_HEIGHT, -cosf(0.0f) * MAX_CYLINDERLENGTH);
	pVtx[9].pos = D3DXVECTOR3(-sinf(0.0f) * MAX_CYLINDERLENGTH, 0.0f, -cosf(0.0f) * MAX_CYLINDERLENGTH);
	pVtx[10].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * MAX_CYLINDERLENGTH, 0.0f, -cosf(D3DX_PI * 0.25f) * MAX_CYLINDERLENGTH);
	pVtx[11].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * MAX_CYLINDERLENGTH, 0.0f, -cosf(D3DX_PI * 0.5f) * MAX_CYLINDERLENGTH);
	pVtx[12].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * MAX_CYLINDERLENGTH, 0.0f, -cosf(D3DX_PI * 0.75f) * MAX_CYLINDERLENGTH);
	pVtx[13].pos = D3DXVECTOR3(sinf(0.0f) * MAX_CYLINDERLENGTH, 0.0f, cosf(0.0f) * MAX_CYLINDERLENGTH);
	pVtx[14].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * MAX_CYLINDERLENGTH, 0.0f, -cosf(-D3DX_PI * 0.75f) * MAX_CYLINDERLENGTH);
	pVtx[15].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * MAX_CYLINDERLENGTH, 0.0f, -cosf(-D3DX_PI * 0.5f) * MAX_CYLINDERLENGTH);
	pVtx[16].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * MAX_CYLINDERLENGTH, 0.0f, -cosf(-D3DX_PI * 0.25f) * MAX_CYLINDERLENGTH);
	pVtx[17].pos = D3DXVECTOR3(-sinf(0.0f) * MAX_CYLINDERLENGTH, 0.0f, -cosf(0.0f) * MAX_CYLINDERLENGTH);

	for (int nCntSet = 0; nCntSet < MAX_CYLINDER_TOP; nCntSet++)
	{
		//法線ベクトルの設定
		pVtx[nCntSet].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[nCntSet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.375f, 0.0f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(0.625f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.75f, 0.0f);
	pVtx[7].tex = D3DXVECTOR2(0.875f, 0.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[10].tex = D3DXVECTOR2(0.125f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(0.25f, 1.0f);
	pVtx[12].tex = D3DXVECTOR2(0.375f, 1.0f);
	pVtx[13].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[14].tex = D3DXVECTOR2(0.625f, 1.0f);
	pVtx[15].tex = D3DXVECTOR2(0.75f, 1.0f);
	pVtx[16].tex = D3DXVECTOR2(0.875f, 1.0f);
	pVtx[17].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffmeshCylinder->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_CYLINDER_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCylinder,
		NULL);

	WORD *pIdx;						//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点データへのポインタを取得
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 0;
	pIdx[1] = 9;
	pIdx[2] = 1;
	pIdx[3] = 10;
	pIdx[4] = 2;
	pIdx[5] = 11;
	pIdx[6] = 3;
	pIdx[7] = 12;
	pIdx[8] = 4;
	pIdx[9] = 13;
	pIdx[10] = 5;
	pIdx[11] = 14;
	pIdx[12] = 6;
	pIdx[13] = 15;
	pIdx[14] = 7;
	pIdx[15] = 16;
	pIdx[16] = 8;
	pIdx[17] = 17;

	//インデックスバッファをアンロックする
	g_pIdxBuffMeshCylinder->Unlock();
}

//========================================
//ポリゴンの終了処理
//========================================
void UninitmeshCylinder(void)
{
	//テクスチャの破棄
	if (g_pTexturemeshCylinder != NULL)
	{
		g_pTexturemeshCylinder->Release();
		g_pTexturemeshCylinder = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffmeshCylinder != NULL)
	{
		g_pVtxBuffmeshCylinder->Release();
		g_pVtxBuffmeshCylinder = NULL;
	}

	//インデックスバッファの破棄
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}
}

//========================================
//ポリゴンの更新処理
//========================================
void UpdatemeshCylinder(void)
{

}

//=====================================
//ポリゴンの描画処理
//=====================================
void DrawmeshCylinder(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldmeshCylinder);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posmeshCylinder.x, g_posmeshCylinder.y, g_posmeshCylinder.z);
	D3DXMatrixMultiply(&g_mtxWorldmeshCylinder, &g_mtxWorldmeshCylinder, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldmeshCylinder);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffmeshCylinder,								//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_3D));									//頂点情報構造体のサイズ

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshCylinder);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturemeshCylinder);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,													//描画する最初の頂点インデックス
		0,
		MAX_CYLINDER_TOP,									//用意した頂点の数
		0,
		16);												//描画するプリミティブ数
}