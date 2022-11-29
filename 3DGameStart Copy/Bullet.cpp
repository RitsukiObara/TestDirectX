//===========================================
//
//弾のメイン処理[Bullet.cpp]
//Author 小原立暉
//
//===========================================
#include "main.h"
#include "Bullet.h"
#include "Shadow.h"
#include "Effect.h"
#include "wall.h"
#include "Explosion.h"
#include "Polygon.h"

//マクロ定義
#define MAX_BULLET				(128)				//弾の最大数
#define BULLET_RADIUS			(10.0f)				//弾の半径
#define BULLET_SPEED			(4.0f)				//弾の速度
#define BULLET_EFFECT_RADIUS	(8.0f)				//弾のエフェクトの半径
#define BULLET_EFFECT_LIFE		(10)				//弾のエフェクトの寿命
#define BULLET_LIFE				(100)				//弾の寿命

//プロトタイプ宣言
void CollisionWall(Bullet *Bullet);				//壁の当たり判定
void DeathBullet(Bullet *Bullet);				//弾の死亡処理

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];					//弾の情報

//==========================================
//弾の初期化処理
//==========================================
void InitBullet(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//下地のテクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bullet000.png",
		&g_pTextureBullet);

	for (int nCntSet = 0; nCntSet < MAX_BULLET; nCntSet++)
	{
		//位置の初期化
		g_aBullet[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//向きの初期化
		g_aBullet[nCntSet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//影の番号を初期化する
		g_aBullet[nCntSet].nShadow = INIT_SHADOW;

		//使用していない
		g_aBullet[nCntSet].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-BULLET_RADIUS, +BULLET_RADIUS, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+BULLET_RADIUS, +BULLET_RADIUS, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BULLET_RADIUS, -BULLET_RADIUS, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+BULLET_RADIUS, -BULLET_RADIUS, 0.0f);

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
	g_pVtxBuffBullet->Unlock();
}

//========================================
//弾の終了処理
//========================================
void UninitBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//========================================
//弾の更新処理
//========================================
void UpdateBullet(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_BULLET; nCntBill++)
	{
		if (g_aBullet[nCntBill].bUse == true)
		{//使用されていた場合
			//影の位置の設定処理
			SetPositionShadow(g_aBullet[nCntBill].nShadow, g_aBullet[nCntBill].pos, g_aBullet[nCntBill].rot);

			//寿命を減らす
			g_aBullet[nCntBill].nLife--;

			//加速する
			g_aBullet[nCntBill].pos.x += sinf(g_aBullet[nCntBill].rot.y) * BULLET_SPEED;
			g_aBullet[nCntBill].pos.z += cosf(g_aBullet[nCntBill].rot.y) * BULLET_SPEED;

			//エフェクトの設定処理
			SetEffect(g_aBullet[nCntBill].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), BULLET_EFFECT_RADIUS, BULLET_EFFECT_LIFE, EFFECT_BULLET);

			//壁の当たり判定
			CollisionWall(&g_aBullet[nCntBill]);

			
		}
		pVtx += 4;					//頂点データを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//=====================================
//弾の描画処理
//=====================================
void DrawBullet(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	D3DXMATRIX mtxTrans;			//計算用マトリックス
	D3DXMATRIX mtxView;				//ビューマトリックス取得用

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);					//アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);				//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);				//アルファテストの参照値設定

	for (int nCntDraw = 0; nCntDraw < MAX_BULLET; nCntDraw++)
	{
		if (g_aBullet[nCntDraw].bUse == true)
		{//使用していた場合
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBullet[nCntDraw].mtx);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに向けて正面に向ける
			D3DXMatrixInverse(&g_aBullet[nCntDraw].mtx, NULL, &mtxView);

			//逆行列を求める
			g_aBullet[nCntDraw].mtx._41 = 0.0f;
			g_aBullet[nCntDraw].mtx._42 = 0.0f;
			g_aBullet[nCntDraw].mtx._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntDraw].pos.x, g_aBullet[nCntDraw].pos.y, g_aBullet[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCntDraw].mtx, &g_aBullet[nCntDraw].mtx, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntDraw].mtx);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0,
				g_pVtxBuffBullet,								//頂点バッファへのポインタ
				0,
				sizeof(VERTEX_3D));									//頂点情報構造体のサイズ

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBullet);

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
}

//======================================
//弾の設定処理
//======================================
void SetBullet(D3DXVECTOR3 rot, D3DXVECTOR3 pos)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_BULLET; nCntBill++)
	{
		if (g_aBullet[nCntBill].bUse == false)
		{//使用されていなかった場合
			//位置を設定する
			g_aBullet[nCntBill].pos = pos;

			//向きを設定する
			g_aBullet[nCntBill].rot = rot;

			//影を設定する
			g_aBullet[nCntBill].nShadow = SetShadow(SHADOW_BULLET);

			//寿命を設定する
			g_aBullet[nCntBill].nLife = BULLET_LIFE;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-BULLET_RADIUS, +BULLET_RADIUS, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+BULLET_RADIUS, +BULLET_RADIUS, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-BULLET_RADIUS, -BULLET_RADIUS, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+BULLET_RADIUS, -BULLET_RADIUS, 0.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//使用する
			g_aBullet[nCntBill].bUse = true;

			break;									//抜け出す
		}
		pVtx += 4;									//頂点データを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//================================
//壁の当たり判定
//================================
void CollisionWall(Bullet *Bullet)
{
	if (Bullet->pos.x >= POLYGON_RADIUS ||
		Bullet->pos.x <= -POLYGON_RADIUS ||
		Bullet->pos.z >= POLYGON_RADIUS ||
		Bullet->pos.z <= -POLYGON_RADIUS)
	{//弾がステージの外に出た場合
		//爆発のビルボードを設定
		SetExplosion(Bullet->pos);

		//使用していない
		Bullet->bUse = false;

		//影のリセット処理
		ResetShadow(&Bullet->nShadow);
	}
}

//==================================
//弾の死亡処理
//==================================
void DeathBullet(Bullet *Bullet)
{
	if (Bullet->nLife <= 0 && Bullet->bUse == true)
	{//寿命が0以下になった場合
		//爆発のビルボードを設定
		SetExplosion(Bullet->pos);

		//使用していない
		Bullet->bUse = false;

		//影のリセット処理
		ResetShadow(&Bullet->nShadow);
	}
}