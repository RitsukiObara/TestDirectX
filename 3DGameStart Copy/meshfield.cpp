//===========================================
//
//メッシュフィールドのメイン処理[meshfield.cpp]
//Author 小原立暉
//
//===========================================
#include "main.h"
#include "meshfield.h"

//マクロ定義
#define MAX_FIELD_TOP				(9)					//頂点数
#define MAX_FIELD_INDEX				(14)				//インデックス数

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturemeshfield = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffmeshfield = NULL;		//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;		//インデックスバッファへのポインタ
D3DXVECTOR3 g_posmeshfield;								//位置
D3DXVECTOR3 g_rotmeshfield;								//向き
D3DXMATRIX g_mtxWorldmeshfield;							//ポリゴンのワールドマトリックス

//==========================================
//メッシュフィールドの初期化処理
//==========================================
void Initmeshfield(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//下地のテクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Block.png",
		&g_pTexturemeshfield);

	//位置の初期化
	g_posmeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//向きを初期化
	g_rotmeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_FIELD_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffmeshfield,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffmeshfield->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-MESH_RADIUS, +0.0f, +MESH_RADIUS);
	pVtx[1].pos = D3DXVECTOR3(+0.0f, +0.0f, +MESH_RADIUS);
	pVtx[2].pos = D3DXVECTOR3(+MESH_RADIUS, +0.0f, +MESH_RADIUS);
	pVtx[3].pos = D3DXVECTOR3(-MESH_RADIUS, +0.0f, +0.0f);
	pVtx[4].pos = D3DXVECTOR3(+0.0f, +0.0f, +0.0f);
	pVtx[5].pos = D3DXVECTOR3(+MESH_RADIUS, +0.0f, +0.0f);
	pVtx[6].pos = D3DXVECTOR3(-MESH_RADIUS, +0.0f, -MESH_RADIUS);
	pVtx[7].pos = D3DXVECTOR3(+0.0f, +0.0f, -MESH_RADIUS);
	pVtx[8].pos = D3DXVECTOR3(+MESH_RADIUS, +0.0f, -MESH_RADIUS);

	for (int nCntSet = 0; nCntSet < MAX_FIELD_TOP; nCntSet++)
	{
		//法線ベクトルの設定
		pVtx[nCntSet].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[nCntSet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffmeshfield->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_FIELD_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);

	WORD *pIdx;						//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点データへのポインタを取得
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 3;
	pIdx[1] = 0;
	pIdx[2] = 4;
	pIdx[3] = 1;
	pIdx[4] = 5;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 6;
	pIdx[8] = 6;
	pIdx[9] = 3;
	pIdx[10] = 7;
	pIdx[11] = 4;
	pIdx[12] = 8;
	pIdx[13] = 5;

	//インデックスバッファをアンロックする
	g_pIdxBuffMeshField->Unlock();
}

//========================================
//ポリゴンの終了処理
//========================================
void Uninitmeshfield(void)
{
	//テクスチャの破棄
	if (g_pTexturemeshfield != NULL)
	{
		g_pTexturemeshfield->Release();
		g_pTexturemeshfield = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffmeshfield != NULL)
	{
		g_pVtxBuffmeshfield->Release();
		g_pVtxBuffmeshfield = NULL;
	}

	//インデックスバッファの破棄
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//========================================
//ポリゴンの更新処理
//========================================
void Updatemeshfield(void)
{

}

//=====================================
//ポリゴンの描画処理
//=====================================
void Drawmeshfield(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

		//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldmeshfield);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotmeshfield.y, g_rotmeshfield.x, g_rotmeshfield.z);
	D3DXMatrixMultiply(&g_mtxWorldmeshfield, &g_mtxWorldmeshfield, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posmeshfield.x, g_posmeshfield.y, g_posmeshfield.z);
	D3DXMatrixMultiply(&g_mtxWorldmeshfield, &g_mtxWorldmeshfield, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldmeshfield);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffmeshfield,								//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_3D));									//頂点情報構造体のサイズ

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshField);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturemeshfield);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,													//描画する最初の頂点インデックス
		0,
		MAX_FIELD_TOP,										//用意した頂点の数
		0,
		12);												//描画するプリミティブ数
}