//===================================
//
//メッシュドームヘッダー[meshdome.h]
//Author 小原立暉
//
//===================================
#ifndef _MESHDOME_H_
#define _MESHDOME_H_

//マクロ定義
#define SKY_RADIUS			(1000.0f)				//空の半径
#define SKY_HEIGHT			(500.0f)				//空の高さ

//プロトタイプ宣言
void InitmeshDome(void);							//メッシュドームの初期化処理
void UninitmeshDome(void);							//メッシュドームの終了処理
void UpdatemeshDome(void);							//メッシュドームの更新処理
void DrawmeshDome(void);							//メッシュドームの描画処理

#endif