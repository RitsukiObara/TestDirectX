//===================================
//
//爆発ヘッダー[Explosion.h]
//Author 小原立暉
//
//===================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"

//ビルボードの構造体
typedef struct
{
	D3DXVECTOR3 pos;								//位置
	D3DXMATRIX mtx;									//ワールドマトリックス
	int nLife;										//寿命
	int nAnimCounter;								//アニメカウンター
	int nAnimPattern;								//アニメパターンNo.
	bool bUse;										//使用しているかどうか
}Explosion;

//プロトタイプ宣言
void InitExplosion(void);							//爆発の初期化処理
void UninitExplosion(void);							//爆発の終了処理
void UpdateExplosion(void);							//爆発の更新処理
void DrawExplosion(void);							//爆発の描画処理
void SetExplosion(D3DXVECTOR3 pos);					//爆発の設定処理

#endif