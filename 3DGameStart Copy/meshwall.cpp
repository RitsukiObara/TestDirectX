//===========================================
//
//メッシュウォールのメイン処理[meshwall.cpp]
//Author 小原立暉
//
//===========================================
#include "main.h"
#include "meshwall.h"
#include "meshfield.h"

//マクロ定義
#define MAX_WALL					(4)					//壁の数
#define MAX_WALL_TOP				(27)				//頂点数
#define MAX_WALL_INDEX				(38)				//インデックス数

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturemeshwall = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffmeshwall = NULL;		//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshwall = NULL;		//インデックスバッファへのポインタ
D3DXVECTOR3 g_posmeshwall[MAX_WALL];					//位置
D3DXVECTOR3 g_rotmeshwall[MAX_WALL];					//向き
D3DXMATRIX g_mtxWorldmeshwall[MAX_WALL];				//ポリゴンのワールドマトリックス

//==========================================
//メッシュフィールドの初期化処理
//==========================================
void Initmeshwall(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//下地のテクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Barrack.png",
		&g_pTexturemeshwall);

	//位置の初期化
	g_posmeshwall[0] = D3DXVECTOR3(600.0f, 0.0f, 0.0f);

	//向きを初期化
	g_rotmeshwall[0] = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);

	//位置の初期化
	g_posmeshwall[1] = D3DXVECTOR3(0.0f, 0.0f, 600.0f);

	//向きを初期化
	g_rotmeshwall[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//位置の初期化
	g_posmeshwall[2] = D3DXVECTOR3(-600.0f, 0.0f, 0.0f);

	//向きを初期化
	g_rotmeshwall[2] = D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f);

	//位置の初期化
	g_posmeshwall[3] = D3DXVECTOR3(0.0f, 0.0f, -600.0f);

	//向きを初期化
	g_rotmeshwall[3] = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_WALL_TOP * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffmeshwall,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffmeshwall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSet = 0; nCntSet < MAX_WALL; nCntSet++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-MESH_RADIUS, +300.0f, +0.0f);
		pVtx[1].pos = D3DXVECTOR3(-450.0f, +300.0f, +0.0f);
		pVtx[2].pos = D3DXVECTOR3(-300.0f, +300.0f, +0.0f);
		pVtx[3].pos = D3DXVECTOR3(-150.0f, +300.0f, +0.0f);
		pVtx[4].pos = D3DXVECTOR3(+0.0f, +300.0f, +0.0f);
		pVtx[5].pos = D3DXVECTOR3(+150.0f, +300.0f, +0.0f);
		pVtx[6].pos = D3DXVECTOR3(+300.0f, +300.0f, +0.0f);
		pVtx[7].pos = D3DXVECTOR3(+450.0f, +300.0f, +0.0f);
		pVtx[8].pos = D3DXVECTOR3(+MESH_RADIUS, +300.0f, +0.0f);
		pVtx[9].pos = D3DXVECTOR3(-MESH_RADIUS, +150.0f, +0.0f);
		pVtx[10].pos = D3DXVECTOR3(-450.0f, +150.0f, +0.0f);
		pVtx[11].pos = D3DXVECTOR3(-300.0f, +150.0f, +0.0f);
		pVtx[12].pos = D3DXVECTOR3(-150.0f, +150.0f, +0.0f);
		pVtx[13].pos = D3DXVECTOR3(+0.0f, +150.0f, +0.0f);
		pVtx[14].pos = D3DXVECTOR3(+150.0f, +150.0f, +0.0f);
		pVtx[15].pos = D3DXVECTOR3(+300.0f, +150.0f, +0.0f);
		pVtx[16].pos = D3DXVECTOR3(+450.0f, +150.0f, +0.0f);
		pVtx[17].pos = D3DXVECTOR3(+MESH_RADIUS, +150.0f, +0.0f);
		pVtx[18].pos = D3DXVECTOR3(-MESH_RADIUS, +0.0f, +0.0f);
		pVtx[19].pos = D3DXVECTOR3(-450.0f, +0.0f, +0.0f);
		pVtx[20].pos = D3DXVECTOR3(-300.0f, 0.0f, +0.0f);
		pVtx[21].pos = D3DXVECTOR3(-150.0f, +0.0f, +0.0f);
		pVtx[22].pos = D3DXVECTOR3(+0.0f, +0.0f, +0.0f);
		pVtx[23].pos = D3DXVECTOR3(+150.0f, +0.0f, +0.0f);
		pVtx[24].pos = D3DXVECTOR3(+300.0f, +0.0f, +0.0f);
		pVtx[25].pos = D3DXVECTOR3(+450.0f, +0.0f, +0.0f);
		pVtx[26].pos = D3DXVECTOR3(+MESH_RADIUS, +0.0f, +0.0f);

		for (int nCnt = 0; nCnt < MAX_WALL_TOP; nCnt++)
		{
			//法線ベクトルの設定
			pVtx[nCnt].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
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
		pVtx[9].tex = D3DXVECTOR2(0.0f, 0.5f);
		pVtx[10].tex = D3DXVECTOR2(0.125f, 0.5f);
		pVtx[11].tex = D3DXVECTOR2(0.25f, 0.5f);
		pVtx[12].tex = D3DXVECTOR2(0.375f, 0.5f);
		pVtx[13].tex = D3DXVECTOR2(0.5f, 0.5f);
		pVtx[14].tex = D3DXVECTOR2(0.625f, 0.5f);
		pVtx[15].tex = D3DXVECTOR2(0.75f, 0.5f);
		pVtx[16].tex = D3DXVECTOR2(0.875f, 0.5f);
		pVtx[17].tex = D3DXVECTOR2(1.0f, 0.5f);
		pVtx[18].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[19].tex = D3DXVECTOR2(0.125f, 1.0f);
		pVtx[20].tex = D3DXVECTOR2(0.25f, 1.0f);
		pVtx[21].tex = D3DXVECTOR2(0.375f, 1.0f);
		pVtx[22].tex = D3DXVECTOR2(0.5f, 1.0f);
		pVtx[23].tex = D3DXVECTOR2(0.625f, 1.0f);
		pVtx[24].tex = D3DXVECTOR2(0.75f, 1.0f);
		pVtx[25].tex = D3DXVECTOR2(0.875f, 1.0f);
		pVtx[26].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += MAX_WALL_TOP;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffmeshwall->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_WALL_INDEX * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshwall,
		NULL);

	WORD *pIdx;						//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点データへのポインタを取得
	g_pIdxBuffMeshwall->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIndex = 0; nCntIndex < MAX_WALL; nCntIndex++)
	{
		pIdx[0] = 9;
		pIdx[1] = 0;
		pIdx[2] = 10;
		pIdx[3] = 1;
		pIdx[4] = 11;
		pIdx[5] = 2;
		pIdx[6] = 12;
		pIdx[7] = 3;
		pIdx[8] = 13;
		pIdx[9] = 4;
		pIdx[10] = 14;
		pIdx[11] = 5;
		pIdx[12] = 15;
		pIdx[13] = 6;
		pIdx[14] = 16;
		pIdx[15] = 7;
		pIdx[16] = 17;
		pIdx[17] = 8;
		pIdx[18] = 8;
		pIdx[19] = 18;
		pIdx[20] = 18;
		pIdx[21] = 9;
		pIdx[22] = 19;
		pIdx[23] = 10;
		pIdx[24] = 20;
		pIdx[25] = 11;
		pIdx[26] = 21;
		pIdx[27] = 12;
		pIdx[28] = 22;
		pIdx[29] = 13;
		pIdx[30] = 23;
		pIdx[31] = 14;
		pIdx[32] = 24;
		pIdx[33] = 15;
		pIdx[34] = 25;
		pIdx[35] = 16;
		pIdx[36] = 26;
		pIdx[37] = 17;

		pIdx += MAX_WALL_INDEX;
	}

	//インデックスバッファをアンロックする
	g_pIdxBuffMeshwall->Unlock();
}

//========================================
//ポリゴンの終了処理
//========================================
void Uninitmeshwall(void)
{
	//テクスチャの破棄
	if (g_pTexturemeshwall != NULL)
	{
		g_pTexturemeshwall->Release();
		g_pTexturemeshwall = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffmeshwall != NULL)
	{
		g_pVtxBuffmeshwall->Release();
		g_pVtxBuffmeshwall = NULL;
	}

	//インデックスバッファの破棄
	if (g_pIdxBuffMeshwall != NULL)
	{
		g_pIdxBuffMeshwall->Release();
		g_pIdxBuffMeshwall = NULL;
	}
}

//========================================
//ポリゴンの更新処理
//========================================
void Updatemeshwall(void)
{
	
}

//=====================================
//ポリゴンの描画処理
//=====================================
void Drawmeshwall(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	for (int nCntMesh = 0; nCntMesh < MAX_WALL; nCntMesh++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldmeshwall[nCntMesh]);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotmeshwall[nCntMesh].y, g_rotmeshwall[nCntMesh].x, g_rotmeshwall[nCntMesh].z);
		D3DXMatrixMultiply(&g_mtxWorldmeshwall[nCntMesh], &g_mtxWorldmeshwall[nCntMesh], &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_posmeshwall[nCntMesh].x, g_posmeshwall[nCntMesh].y, g_posmeshwall[nCntMesh].z);
		D3DXMatrixMultiply(&g_mtxWorldmeshwall[nCntMesh], &g_mtxWorldmeshwall[nCntMesh], &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldmeshwall[nCntMesh]);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0,
			g_pVtxBuffmeshwall,									//頂点バッファへのポインタ
			0,
			sizeof(VERTEX_3D));									//頂点情報構造体のサイズ

		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_pIdxBuffMeshwall);

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturemeshwall);

		//ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			nCntMesh * MAX_WALL_TOP,							//描画する最初の頂点インデックス
			0,
			nCntMesh * MAX_WALL_TOP,							//用意した頂点の数
			0,
			36);												//描画するプリミティブ数
	}
}