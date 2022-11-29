//===========================================
//
//ライトのメイン処理[light.cpp]
//Author 小原立暉
//
//===========================================
#include "main.h"
#include "light.h"

//マクロ定義
#define NUM_LIGHT			(3)			//ライトの数

//グローバル変数
D3DLIGHT9 g_light[NUM_LIGHT];			//ライトの情報

//==========================================
//ライトの初期化処理
//==========================================
void InitLight(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXVECTOR3 vecDir;				//設定用方向ベクトル

	for (int nCntSet = 0; nCntSet < NUM_LIGHT; nCntSet++)
	{
		//ライトの情報をクリアする
		ZeroMemory(&g_light[nCntSet], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		g_light[nCntSet].Type = D3DLIGHT_DIRECTIONAL;

		switch (nCntSet)
		{
		case 0:

			//ライトの拡散光を設定
			g_light[nCntSet].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//ライトの方向を設定
			vecDir = D3DXVECTOR3(0.22f,-0.87f, 0.44f);

			break;				//抜け出す

		case 1:

			//ライトの拡散光を設定
			g_light[nCntSet].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

			//ライトの方向を設定
			vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);

			break;				//抜け出す

		case 2:

			//ライトの拡散光を設定
			g_light[nCntSet].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

			//ライトの方向を設定
			vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);

			break;				//抜け出す
		}

		D3DXVec3Normalize(&vecDir, &vecDir);				//ベクトルを正規化する

		g_light[nCntSet].Direction = vecDir;

		//ライトを設定する
		pDevice->SetLight(nCntSet, &g_light[nCntSet]);

		//ライトを有効にする
		pDevice->LightEnable(nCntSet, TRUE);
	}
}

//========================================
//ライトの終了処理
//========================================
void UninitLight(void)
{
	
}

//========================================
//ライトの更新処理
//========================================
void UpdateLight(void)
{

}