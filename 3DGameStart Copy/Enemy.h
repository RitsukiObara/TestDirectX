//===================================
//
//敵ヘッダー[Enemy.h]
//Author 小原立暉
//
//===================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//マクロ定義
#define MAX_ENEMY					(128)			//敵の最大数

//モデルの構造体
typedef struct
{
	D3DXVECTOR3 pos;								//位置
	D3DXVECTOR3 posOld;								//前回の位置
	D3DXVECTOR3 move;								//移動量
	D3DXVECTOR3 rot;								//向き
	bool bUse;										//使用しているかどうか
	int nType;										//種類
	D3DXMATRIX mtx;									//ワールドマトリックス
	LPD3DXMESH g_pMeshEnemy = NULL;					//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER g_pBuffMatEnemy = NULL;			//マテリアルへのポインタ
	DWORD g_dwNumMatEnemy = 0;						//マテリアルの数
	int nShadow = -1;								//影のインデックス(影の番号)
	D3DXVECTOR3 VtxMinEnemy;						//敵の最小値
	D3DXVECTOR3 VtxMaxEnemy;						//敵の最大値
}Enemy;

//敵の種類
typedef enum
{
	ENEMY_SLIME = 0,								//スライム
	ENEMY_INSECT,									//虫
	ENEMY_MAX
}ENEMTTYPE;

//プロトタイプ宣言
void InitEnemy(void);							//敵の初期化処理
void UninitEnemy(void);							//敵の終了処理
void UpdateEnemy(void);							//敵の更新処理
void DrawEnemy(void);							//敵の描画処理
void SetEnemy(int nType);						//敵の設定処理
int GetEnemyCount(void);						//敵の総数取得処理
Enemy *GetEnemy(void);							//敵の取得処理

#endif