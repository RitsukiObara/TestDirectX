//===================================
//
//弾ヘッダー[Bullet.h]
//Author 小原立暉
//
//===================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//ビルボードの構造体
typedef struct
{
	D3DXVECTOR3 pos;								//位置
	D3DXVECTOR3 rot;								//方向(発射の方向設定に必要)
	D3DXMATRIX mtx;									//ワールドマトリックス
	int nLife;										//寿命
	int nShadow;									//影のインデックス(影の番号)
	bool bUse;										//使用しているかどうか
}Bullet;

//プロトタイプ宣言
void InitBullet(void);								//弾の初期化処理
void UninitBullet(void);							//弾の終了処理
void UpdateBullet(void);							//弾の更新処理
void DrawBullet(void);								//弾の描画処理
void SetBullet(D3DXVECTOR3 rot, D3DXVECTOR3 pos);	//弾の設定処理

#endif