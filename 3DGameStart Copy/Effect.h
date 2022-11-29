//===================================
//
//エフェクトヘッダー[Effect.h]
//Author 小原立暉
//
//===================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include"main.h"

//エフェクトの種類
typedef enum
{
	EFFECT_BULLET = 0,					//弾のエフェクト
	EFFECT_EXPL,						//爆発のエフェクト
	EFFECT_WALK,						//歩いている時の処理
	EFFECT_MAX
}EffectType;

//プロトタイプ宣言
void InitEffect(void);										//エフェクトの初期化処理
void UninitEffect(void);									//エフェクトの終了処理
void UpdateEffect(void);									//エフェクトの更新処理
void DrawEffect(void);										//エフェクトの描画処理
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRadius, int nLife, int nType);					//エフェクトの設定処理

#endif