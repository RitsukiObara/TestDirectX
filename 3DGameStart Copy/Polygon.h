//===================================
//
//ポリゴンヘッダー[Polygon.h]
//Author 小原立暉
//
//===================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

//マクロ定義
#define POLYGON_RADIUS			(600.0f)			//ポリゴンの半径

//プロトタイプ宣言
void InitPolygon(void);								//ポリゴンの初期化処理
void UninitPolygon(void);							//ポリゴンの終了処理
void UpdatePolygon(void);							//ポリゴンの更新処理
void DrawPolygon(void);								//ポリゴンの描画処理

#endif