//===========================================
//
//メッシュドームのメイン処理[meshdome.cpp]
//Author 小原立暉
//
//===========================================
#include "main.h"
#include "meshdome.h"

//マクロ定義
#define MAX_DOME_TOP			(36)
#define MAX_DOME_INDEX			(58)

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturemeshDome = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffmeshDome = NULL;		//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshDome = NULL;		//インデックスバッファへのポインタ
D3DXVECTOR3 g_posmeshDome;								//位置
D3DXVECTOR3 g_rotmeshDome;								//向き
D3DXMATRIX g_mtxWorldmeshDome;							//ポリゴンのワールドマトリックス

//==========================================
//メッシュフィールドの初期化処理
//==========================================
void InitmeshDome(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	////下地のテクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice,
	//	"data/TEXTURE/Block.png",
	//	&g_pTexturemeshDome);

	//位置の初期化
	g_posmeshDome = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//向きを初期化
	g_rotmeshDome = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_DOME_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffmeshDome,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffmeshDome->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-sinf(0.0f) * SKY_RADIUS, SKY_HEIGHT * 0.0f, -cosf(0.0f) * SKY_RADIUS);
	pVtx[1].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * SKY_RADIUS, SKY_HEIGHT * 0.0f, -cosf(D3DX_PI * 0.25f) * SKY_RADIUS);
	pVtx[2].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * SKY_RADIUS, SKY_HEIGHT * 0.0f, -cosf(D3DX_PI * 0.5f) * SKY_RADIUS);
	pVtx[3].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * SKY_RADIUS, SKY_HEIGHT * 0.0f, -cosf(D3DX_PI * 0.75f) * SKY_RADIUS);
	pVtx[4].pos = D3DXVECTOR3(sinf(0.0f) * SKY_RADIUS, SKY_HEIGHT * 0.0f, cosf(0.0f) * SKY_RADIUS);
	pVtx[5].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * SKY_RADIUS, SKY_HEIGHT * 0.0f, -cosf(-D3DX_PI * 0.75f) * SKY_RADIUS);
	pVtx[6].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * SKY_RADIUS, SKY_HEIGHT * 0.0f, -cosf(-D3DX_PI * 0.5f) * SKY_RADIUS);
	pVtx[7].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * SKY_RADIUS, SKY_HEIGHT * 0.0f, -cosf(-D3DX_PI * 0.25f) * SKY_RADIUS);
	pVtx[8].pos = D3DXVECTOR3(-sinf(0.0f) * SKY_RADIUS, SKY_HEIGHT * 0.0f, -cosf(0.0f) * SKY_RADIUS);
	pVtx[9].pos = D3DXVECTOR3(-sinf(0.0f) * (SKY_RADIUS -200.0f), SKY_HEIGHT * 0.2f, -cosf(0.0f) * (SKY_RADIUS - 200.0f));
	pVtx[10].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * (SKY_RADIUS - 200.0f), SKY_HEIGHT * 0.2f, -cosf(D3DX_PI * 0.25f) * (SKY_RADIUS - 200.0f));
	pVtx[11].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * (SKY_RADIUS - 200.0f), SKY_HEIGHT * 0.2f, -cosf(D3DX_PI * 0.5f) * (SKY_RADIUS - 200.0f));
	pVtx[12].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * (SKY_RADIUS - 200.0f), SKY_HEIGHT * 0.2f, -cosf(D3DX_PI * 0.75f) * (SKY_RADIUS - 200.0f));
	pVtx[13].pos = D3DXVECTOR3(sinf(0.0f) * (SKY_RADIUS - 200.0f), SKY_HEIGHT * 0.2f, cosf(0.0f) * (SKY_RADIUS - 200.0f));
	pVtx[14].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * (SKY_RADIUS - 200.0f), SKY_HEIGHT * 0.2f, -cosf(-D3DX_PI * 0.75f) * (SKY_RADIUS - 200.0f));
	pVtx[15].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * (SKY_RADIUS - 200.0f), SKY_HEIGHT * 0.2f, -cosf(-D3DX_PI * 0.5f) * (SKY_RADIUS - 200.0f));
	pVtx[16].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * (SKY_RADIUS - 200.0f), SKY_HEIGHT * 0.2f, -cosf(-D3DX_PI * 0.25f) * (SKY_RADIUS - 200.0f));
	pVtx[17].pos = D3DXVECTOR3(-sinf(0.0f) * (SKY_RADIUS - 200.0f), SKY_HEIGHT * 0.2f, -cosf(0.0f) * (SKY_RADIUS - 200.0f));
	pVtx[18].pos = D3DXVECTOR3(-sinf(0.0f) * (SKY_RADIUS - 400.0f), SKY_HEIGHT * 0.4f, -cosf(0.0f) * (SKY_RADIUS - 400.0f));
	pVtx[19].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * (SKY_RADIUS - 400.0f), SKY_HEIGHT * 0.4f, -cosf(D3DX_PI * 0.25f) * (SKY_RADIUS - 400.0f));
	pVtx[20].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * (SKY_RADIUS - 400.0f), SKY_HEIGHT * 0.4f, -cosf(D3DX_PI * 0.5f) * (SKY_RADIUS - 400.0f));
	pVtx[21].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * (SKY_RADIUS - 400.0f), SKY_HEIGHT * 0.4f, -cosf(D3DX_PI * 0.75f) * (SKY_RADIUS - 400.0f));
	pVtx[22].pos = D3DXVECTOR3(sinf(0.0f) * (SKY_RADIUS - 400.0f), SKY_HEIGHT * 0.4f, cosf(0.0f) * (SKY_RADIUS - 400.0f));
	pVtx[23].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * (SKY_RADIUS - 400.0f), SKY_HEIGHT * 0.4f, -cosf(-D3DX_PI * 0.75f) * (SKY_RADIUS - 400.0f));
	pVtx[24].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * (SKY_RADIUS - 400.0f), SKY_HEIGHT * 0.4f, -cosf(-D3DX_PI * 0.5f) * (SKY_RADIUS - 400.0f));
	pVtx[25].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * (SKY_RADIUS - 400.0f), SKY_HEIGHT * 0.4f, -cosf(-D3DX_PI * 0.25f) * (SKY_RADIUS - 400.0f));
	pVtx[26].pos = D3DXVECTOR3(-sinf(0.0f) * (SKY_RADIUS - 400.0f), SKY_HEIGHT * 0.4f, -cosf(0.0f) * (SKY_RADIUS - 400.0f));
	pVtx[27].pos = D3DXVECTOR3(-sinf(0.0f) * (SKY_RADIUS - 600.0f), SKY_HEIGHT * 0.6f, -cosf(0.0f) * (SKY_RADIUS - 600.0f));
	pVtx[28].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * (SKY_RADIUS - 600.0f), SKY_HEIGHT * 0.6f, -cosf(D3DX_PI * 0.25f) * (SKY_RADIUS - 600.0f));
	pVtx[29].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * (SKY_RADIUS - 600.0f), SKY_HEIGHT * 0.6f, -cosf(D3DX_PI * 0.5f) * (SKY_RADIUS - 600.0f));
	pVtx[30].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * (SKY_RADIUS - 600.0f), SKY_HEIGHT * 0.6f, -cosf(D3DX_PI * 0.75f) * (SKY_RADIUS - 600.0f));
	pVtx[31].pos = D3DXVECTOR3(sinf(0.0f) * (SKY_RADIUS - 600.0f), SKY_HEIGHT * 0.6f, cosf(0.0f) * (SKY_RADIUS - 600.0f));
	pVtx[32].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * (SKY_RADIUS - 600.0f), SKY_HEIGHT * 0.6f, -cosf(-D3DX_PI * 0.75f) * (SKY_RADIUS - 600.0f));
	pVtx[33].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * (SKY_RADIUS - 600.0f), SKY_HEIGHT * 0.6f, -cosf(-D3DX_PI * 0.5f) * (SKY_RADIUS - 600.0f));
	pVtx[34].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * (SKY_RADIUS - 600.0f), SKY_HEIGHT * 0.6f, -cosf(-D3DX_PI * 0.25f) * (SKY_RADIUS - 600.0f));
	pVtx[35].pos = D3DXVECTOR3(-sinf(0.0f) * (SKY_RADIUS - 600.0f), SKY_HEIGHT * 0.6f, -cosf(0.0f) * (SKY_RADIUS - 600.0f));

	for (int nCntSet = 0; nCntSet < MAX_DOME_TOP; nCntSet++)
	{
		//法線ベクトルの設定
		pVtx[nCntSet].hor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[nCntSet].col = D3DXCOLOR(0.3f, 0.3f, 1.0f, 1.0f);
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
	pVtx[9].tex = D3DXVECTOR2(0.0f, 0.25f);
	pVtx[10].tex = D3DXVECTOR2(0.125f, 0.25f);
	pVtx[11].tex = D3DXVECTOR2(0.25f, 0.25f);
	pVtx[12].tex = D3DXVECTOR2(0.375f, 0.25f);
	pVtx[13].tex = D3DXVECTOR2(0.5f, 0.25f);
	pVtx[14].tex = D3DXVECTOR2(0.625f, 0.25f);
	pVtx[15].tex = D3DXVECTOR2(0.75f, 0.25f);
	pVtx[16].tex = D3DXVECTOR2(0.875f, 0.25f);
	pVtx[17].tex = D3DXVECTOR2(1.0f, 0.25f);
	pVtx[18].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[19].tex = D3DXVECTOR2(0.125f, 0.5f);
	pVtx[20].tex = D3DXVECTOR2(0.25f, 0.5f);
	pVtx[21].tex = D3DXVECTOR2(0.375f, 0.5f);
	pVtx[22].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[23].tex = D3DXVECTOR2(0.625f, 0.5f);
	pVtx[24].tex = D3DXVECTOR2(0.75f, 0.5f);
	pVtx[25].tex = D3DXVECTOR2(0.875f, 0.5f);
	pVtx[26].tex = D3DXVECTOR2(1.0f, 0.5f);
	pVtx[27].tex = D3DXVECTOR2(0.0f, 0.75f);
	pVtx[28].tex = D3DXVECTOR2(0.125f, 0.75f);
	pVtx[29].tex = D3DXVECTOR2(0.25f, 0.75f);
	pVtx[30].tex = D3DXVECTOR2(0.375f, 0.75f);
	pVtx[31].tex = D3DXVECTOR2(0.5f, 0.75f);
	pVtx[32].tex = D3DXVECTOR2(0.625f, 0.75f);
	pVtx[33].tex = D3DXVECTOR2(0.75f, 0.75f);
	pVtx[34].tex = D3DXVECTOR2(0.875f, 0.75f);
	pVtx[35].tex = D3DXVECTOR2(1.0f, 0.75f);

	//頂点バッファをアンロックする
	g_pVtxBuffmeshDome->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_DOME_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshDome,
		NULL);

	WORD *pIdx;						//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点データへのポインタを取得
	g_pIdxBuffMeshDome->Lock(0, 0, (void**)&pIdx, 0);

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
	pIdx[38] = 17;
	pIdx[39] = 27;
	pIdx[40] = 27;
	pIdx[41] = 18;
	pIdx[42] = 28;
	pIdx[43] = 19;
	pIdx[44] = 29;
	pIdx[45] = 20;
	pIdx[46] = 30;
	pIdx[47] = 21;
	pIdx[48] = 31;
	pIdx[49] = 22;
	pIdx[50] = 32;
	pIdx[51] = 23;
	pIdx[52] = 33;
	pIdx[53] = 24;
	pIdx[54] = 34;
	pIdx[55] = 25;
	pIdx[56] = 35;
	pIdx[57] = 26;

	//インデックスバッファをアンロックする
	g_pIdxBuffMeshDome->Unlock();
}

//========================================
//ポリゴンの終了処理
//========================================
void UninitmeshDome(void)
{
	//テクスチャの破棄
	if (g_pTexturemeshDome != NULL)
	{
		g_pTexturemeshDome->Release();
		g_pTexturemeshDome = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffmeshDome != NULL)
	{
		g_pVtxBuffmeshDome->Release();
		g_pVtxBuffmeshDome = NULL;
	}

	//インデックスバッファの破棄
	if (g_pIdxBuffMeshDome != NULL)
	{
		g_pIdxBuffMeshDome->Release();
		g_pIdxBuffMeshDome = NULL;
	}
}

//========================================
//ポリゴンの更新処理
//========================================
void UpdatemeshDome(void)
{

}

//=====================================
//ポリゴンの描画処理
//=====================================
void DrawmeshDome(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldmeshDome);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotmeshDome.y, g_rotmeshDome.x, g_rotmeshDome.z);
	D3DXMatrixMultiply(&g_mtxWorldmeshDome, &g_mtxWorldmeshDome, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posmeshDome.x, g_posmeshDome.y, g_posmeshDome.z);
	D3DXMatrixMultiply(&g_mtxWorldmeshDome, &g_mtxWorldmeshDome, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldmeshDome);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffmeshDome,								//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_3D));									//頂点情報構造体のサイズ

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshDome);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturemeshDome);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,													//描画する最初の頂点インデックス
		0,
		MAX_DOME_TOP,										//用意した頂点の数
		0,
		56);												//描画するプリミティブ数
}