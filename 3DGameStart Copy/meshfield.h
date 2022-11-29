//===================================
//
//メッシュフィールドヘッダー[meshfield.h]
//Author 小原立暉
//
//===================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//マクロ定義
#define MESH_RADIUS			(600.0f)				//ポリゴンの半径

//プロトタイプ宣言
void Initmeshfield(void);							//メッシュフィールドの初期化処理
void Uninitmeshfield(void);							//メッシュフィールドの終了処理
void Updatemeshfield(void);							//メッシュフィールドの更新処理
void Drawmeshfield(void);							//メッシュフィールドの描画処理

#endif