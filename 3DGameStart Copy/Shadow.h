//===================================
//
//影ヘッダー[Shadow.h]
//Author 小原立暉
//
//===================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

//影の構造体
typedef struct
{
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 rot;					//向き
	bool bUse;							//使用しているかどうか
}Shadow;

//影の種類
typedef enum
{
	SHADOW_PLAYER = 0,					//プレイヤーの影
	SHADOW_ENEMY,						//敵の影
	SHADOW_TOWER,						//塔の影
	SHADOW_BULLET,						//弾の影
	SHADOW_MAX
}ShadowType;

//プロトタイプ宣言
void InitShadow(void);								//影の初期化処理
void UninitShadow(void);							//影の終了処理
void UpdateShadow(void);							//影の更新処理
void DrawShadow(void);								//影の描画処理
int SetShadow(int nType);							//影の設定処理
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//影の位置設定処理
void ResetShadow(int *nIdxShadow);					//影のリセット処理

#endif