//===================================
//
//塔ヘッダー[model.h]
//Author 小原立暉
//
//===================================
#ifndef _TOWER_H_
#define _TOWER_H_

#include "main.h"

//モデルの構造体
typedef struct
{
	D3DXVECTOR3 pos;								//位置
	D3DXVECTOR3 rot;								//向き
	bool bUse;										//使用しているかどうか
	D3DXMATRIX mtx;									//ワールドマトリックス
	int TowerState;									//タワーの状態
	int nLife;										//体力
	int nDamageCount;								//ダメージカウント
	int nInvincibleCount;							//無敵カウント
	int nShadow;									//影のインデックス(影の番号)
	bool bFlash;									//点滅するかどうか
	D3DXVECTOR3 VtxMinTower;						//塔の最小値
	D3DXVECTOR3 VtxMaxTower;						//塔の最大値
}Tower;

//塔の状態
typedef enum
{
	TOWER_NORMAL = 0,								//通常状態
	TOWER_DAMAGE,									//ダメージ状態
	TOWER_INVINCIBLE,								//無敵状態
	TOWER_PINCH,									//ピンチ状態
	TOWER_DEATH,									//死亡状態
	TOWER_MAX
}TOWERSTATE;

//塔にぶつかる物の条件
typedef enum
{
	COLL_PLAYER = 0,
	COLL_ENEMY,
	COLL_MAX
}COLLISIONTYPE;

//プロトタイプ宣言
void InitTower(void);							//塔の初期化処理
void UninitTower(void);							//塔の終了処理
void UpdateTower(void);							//塔の更新処理
void DrawTower(void);							//塔の描画処理
Tower *GetTower(void);							//塔の取得処理
void CollisionTower(D3DXVECTOR3 MaxPos, D3DXVECTOR3 MinPos, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, int nType);			//塔の当たり判定

#endif