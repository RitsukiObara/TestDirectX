//===========================================
//
//壁のメイン処理[wall.cpp]
//Author 小原立暉
//
//===========================================
#include "main.h"
#include "wall.h"
#include "input.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;	//頂点バッファへのポインタ
Wall g_aWall[NUM_WALL];							//壁の情報

//==========================================
//ポリゴンの初期化処理
//==========================================
void InitWall(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	////下地のテクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice,
	//	"data/TEXTURE/ジョジョ D4C.png",
	//	&g_pTextureWall);

	for (int nCntSet = 0; nCntSet < NUM_WALL; nCntSet++)
	{//情報の初期化
		switch (nCntSet)
		{
		case 0:					//右の壁1

			//位置の初期化
			g_aWall[nCntSet].pos = D3DXVECTOR3(600.0f, 0.0f, 0.0f);

			//向きを初期化
			g_aWall[nCntSet].rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);

			break;				//抜け出す

		case 1:					//奥の壁1

			//位置の初期化
			g_aWall[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, 600.0f);

			//向きを初期化
			g_aWall[nCntSet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			break;				//抜け出す

		case 2:					//左の壁1

			//位置の初期化
			g_aWall[nCntSet].pos = D3DXVECTOR3(-600.0f, 0.0f, 0.0f);

			//向きを初期化
			g_aWall[nCntSet].rot = D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f);

			break;				//抜け出す

		case 3:					//手前の壁1

			//位置の初期化
			g_aWall[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, -600.0f);

			//向きを初期化
			g_aWall[nCntSet].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

			break;				//抜け出す

		case 4:					//右の壁2

			//位置の初期化
			g_aWall[nCntSet].pos = D3DXVECTOR3(600.0f, 0.0f, 0.0f);

			//向きを初期化
			g_aWall[nCntSet].rot = D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f);

			break;				//抜け出す

		case 5:					//奥の壁2

			//位置の初期化
			g_aWall[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, 600.0f);

			//向きを初期化
			g_aWall[nCntSet].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

			break;				//抜け出す

		case 6:					//左の壁2

			//位置の初期化
			g_aWall[nCntSet].pos = D3DXVECTOR3(-600.0f, 0.0f, 0.0f);

			//向きを初期化
			g_aWall[nCntSet].rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);

			break;				//抜け出す

		case 7:					//手前の壁2

			//位置の初期化
			g_aWall[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, -600.0f);

			//向きを初期化
			g_aWall[nCntSet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			break;				//抜け出す
		}
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * NUM_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < NUM_WALL; nCntWall++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-600.0f, +300.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+600.0f, +300.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-600.0f, +0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+600.0f, +0.0f, 0.0f);

		//法線ベクトルの設定
		pVtx[0].hor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].hor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].hor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].hor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		if (nCntWall > 3)
		{//裏の壁の場合
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
		}
		else
		{//表の壁
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;						//頂点データを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}

//========================================
//ポリゴンの終了処理
//========================================
void UninitWall(void)
{
	//テクスチャの破棄
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//========================================
//ポリゴンの更新処理
//========================================
void UpdateWall(void)
{

}

//=====================================
//ポリゴンの描画処理
//=====================================
void DrawWall(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntWall = 0; nCntWall < NUM_WALL; nCntWall++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aWall[nCntWall].mtx);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
		D3DXMatrixMultiply(&g_aWall[nCntWall].mtx, &g_aWall[nCntWall].mtx, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
		D3DXMatrixMultiply(&g_aWall[nCntWall].mtx, &g_aWall[nCntWall].mtx, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].mtx);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0,
			g_pVtxBuffWall,										//頂点バッファへのポインタ
			0,
			sizeof(VERTEX_3D));									//頂点情報構造体のサイズ

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureWall);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
			4 * nCntWall,										//描画する最初の頂点インデックス
			2);													//描画するプリミティブ数
	}
}

//=============================
//壁の取得処理
//=============================
Wall *GetWall(void)
{
	//壁の情報を返す
	return &g_aWall[0];
}