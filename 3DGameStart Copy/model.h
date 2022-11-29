//===================================
//
//モデルヘッダー[model.h]
//Author 小原立暉
//
//===================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//モデルの構造体
typedef struct
{
	D3DXVECTOR3 pos;								//現在の位置
	D3DXVECTOR3 posOld;								//前回の位置
	D3DXVECTOR3 move;								//移動量
	D3DXVECTOR3 rot;								//向き
	float fRotDest;									//目標の向き
	float fRotDiff;									//向きの差分
	bool bUse;										//使用しているかどうか
	bool bMove;										//移動しているかどうか
	D3DXMATRIX mtxModel;							//モデルのワールドのマトリックス
	int nShadow;									//影のインデックス(影の番号)
	D3DXVECTOR3 VtxMinModel;						//モデルの最小値
	D3DXVECTOR3 VtxMaxModel;						//モデルの最大値
	int nPoint;										//ポイント
}Model;

//プロトタイプ宣言
void InitModel(void);							//モデルの初期化処理
void UninitModel(void);							//モデルの終了処理
void UpdateModel(void);							//モデルの更新処理
void DrawModel(void);							//モデルの描画処理
Model *GetModel(void);							//モデルの取得処理

#endif