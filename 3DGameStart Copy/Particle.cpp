//=======================================
//
//パーティクルのメイン処理[Particle.cpp]
//Author 小原立暉
//
//=======================================
#include"main.h"
#include"Particle.h"
#include"Effect.h"
#include <time.h>				//ランダム処理に必要

//マクロ定義
#define MAX_PARTICLE			(128)		//パーティクルの最大数

//パーティクル構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;					//位置(出現位置)
	D3DXVECTOR3 move;					//移動量
	float fRadius;						//半径
	int nLife;							//寿命(発生時間)
	int nType;							//種類(理由)
	int nCount;							//カウント
	bool bUse;							//使用しているかどうか
}Particle;

//グローバル変数
Particle g_aParticle[MAX_PARTICLE];		//パーティクルの情報

//==========================================
//パーティクルの初期化処理
//==========================================
void InitParticle(void)
{
	//乱数の種を設置
	srand((unsigned int)time(NULL));

	int nCntParticle;

	//爆発の情報の初期化
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//位置を初期化する

		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量を初期化する

		g_aParticle[nCntParticle].fRadius = 0.0f;								//半径を初期化する

		g_aParticle[nCntParticle].nLife = 0;									//寿命を初期化する

		g_aParticle[nCntParticle].nCount = 0;									//カウントを初期化する

		g_aParticle[nCntParticle].bUse = false;									//使用していない状態にする
	}
}

//========================================
//パーティクルの終了処理
//========================================
void UninitParticle(void)
{

}

//========================================
//パーティクルの更新処理
//========================================
void UpdateParticle(void)
{
	int nCntParticle;					//回数の変数を宣言する
	D3DXVECTOR3 fAngle;					//角度の変数
	float fSpeed;						//移動量の変数

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{//パーティクルが使用されている
			//パーティクルの寿命を減算する
			g_aParticle[nCntParticle].nLife--;

			//カウンターを加算する
			g_aParticle[nCntParticle].nCount++;

			switch (g_aParticle[nCntParticle].nType)
			{
			case PTCL_EXPL:					//爆発パーティクル

				for (int nCntSet = 0; nCntSet < 10; nCntSet++)
				{
					//角度を設定する
					fAngle.x = (float)(rand() % 629 - 314) / 100.0f;

					//角度を設定する
					fAngle.y = (float)(rand() % 629 - 314) / 100.0f;

					//角度を設定する
					fAngle.z = (float)(rand() % 629 - 314) / 100.0f;

					//移動量を設定する
					fSpeed = (float)(rand() % 500) / 100.0f + 4.0f;

					//移動量を設定する
					g_aParticle[nCntParticle].move.x = sinf(fAngle.x) * fSpeed;
					g_aParticle[nCntParticle].move.y = cosf(fAngle.y) * fSpeed;
					g_aParticle[nCntParticle].move.z = cosf(fAngle.z) * fSpeed;

					//半径を設定する
					g_aParticle[nCntParticle].fRadius = (float)(rand() % 400) / 100.0f + 10.0f;

					//エフェクトの設定処理
					SetEffect(g_aParticle[nCntParticle].pos, g_aParticle[nCntParticle].move, g_aParticle[nCntParticle].fRadius, 20, EFFECT_EXPL);
				}

				break;						//抜け出す
			}

			if (g_aParticle[nCntParticle].nLife == 0)
			{//寿命が尽きた場合
			 //使用していない状態にする
				g_aParticle[nCntParticle].bUse = false;
			}
		}
	}
}

//=====================================
//パーティクルの描画処理
//=====================================
void DrawParticle(void)
{

}

//========================================
//パーティクルの設定処理
//========================================
void SetParticle(D3DXVECTOR3 pos, int nType)
{
	int nCntParticle;

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{//パーティクルが使用されていない
		 //種類を設定
			g_aParticle[nCntParticle].nType = nType;

			switch (g_aParticle[nCntParticle].nType)
			{
			case PTCL_EXPL:				//プレイヤーが死んだとき
				//位置を設定
				g_aParticle[nCntParticle].pos = pos;

				//寿命を設定
				g_aParticle[nCntParticle].nLife = 5;

				break;					//抜け出す
			}

			//カウンターを0に設定する
			g_aParticle[nCntParticle].nCount = 0;

			//使用している状態にする
			g_aParticle[nCntParticle].bUse = true;				//使用している状態にする

			break;
		}
	}
}