//===================================
//
//壁ヘッダー[Wall.h]
//Author 小原立暉
//
//===================================
#ifndef _WALL_H_
#define _WALL_H_

//マクロ定義
#define NUM_WALL			(8)					//壁の数

//壁の構造体
typedef struct
{
	D3DXVECTOR3 pos;							//位置
	D3DXVECTOR3 rot;							//向き
	D3DXMATRIX mtx;								//壁のワールドマトリックス
}Wall;

//プロトタイプ宣言
void InitWall(void);					//壁の初期化処理
void UninitWall(void);					//壁の終了処理
void UpdateWall(void);					//壁の更新処理
void DrawWall(void);					//壁の描画処理
Wall *GetWall(void);					//壁の取得処理

#endif