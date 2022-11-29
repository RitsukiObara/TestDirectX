//===================================
//
//ビルボードヘッダー[billboard.h]
//Author 小原立暉
//
//===================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"

//ビルボードの構造体
typedef struct
{
	D3DXVECTOR3 pos;								//位置
	D3DXVECTOR3 rot;								//方向(発射の方向設定に必要)
	D3DXMATRIX mtx;									//ワールドマトリックス
	int Type;										//種類
	int nLife;										//寿命
	int nAnimCounter;								//アニメカウンター
	int nAnimPattern;								//アニメパターンNo.
	int nShadow;									//影のインデックス(影の番号)
	bool bUse;										//使用しているかどうか
}Billboard;

//ビルボードの種類
typedef enum
{
	BILLBOARD_EXPL = 0,								//爆発
	BILLBOARD_BULLET,								//弾
	BILLBOARD_MAX
}Billtype;

//プロトタイプ宣言
void InitBillboard(void);							//ビルボードの初期化処理
void UninitBillboard(void);							//ビルボードの終了処理
void UpdateBillboard(void);							//ビルボードの更新処理
void DrawBillboard(void);							//ビルボードの描画処理
void SetBillboard(D3DXVECTOR3 rot, D3DXVECTOR3 pos, int nType);	//ビルボードの設定処理

#endif