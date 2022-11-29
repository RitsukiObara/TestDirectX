//===================================
//
//経験値ヘッダー[exp.h]
//Author 小原立暉
//
//===================================
#ifndef _EXP_H_
#define _EXP_H_

#include"main.h"

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 rot;					//向き
	D3DXVECTOR3 move;					//移動量
	D3DXMATRIX mtx;						//ワールドマトリックス
	float fRadius;						//半径
	int nLife;							//寿命
	bool bUse;							//使用しているかどうか
}Exp;

//プロトタイプ宣言
void InitExp(void);						//経験値の初期化処理
void UninitExp(void);					//経験値の終了処理
void UpdateExp(void);					//経験値の更新処理
void DrawExp(void);						//経験値の描画処理
void SetExp(D3DXVECTOR3 pos);			//経験値の設定処理

#endif