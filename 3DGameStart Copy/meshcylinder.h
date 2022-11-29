//===================================
//
//メッシュシリンダーヘッダー[meshcylinder.h]
//Author 小原立暉
//
//===================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

//マクロ定義
#define BG_HEIGHT					(400.0f)		//背景の高さ

//プロトタイプ宣言
void InitmeshCylinder(void);						//メッシュシリンダーの初期化処理
void UninitmeshCylinder(void);						//メッシュシリンダーの終了処理
void UpdatemeshCylinder(void);						//メッシュシリンダーの更新処理
void DrawmeshCylinder(void);						//メッシュシリンダーの描画処理

#endif