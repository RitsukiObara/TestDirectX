//===================================
//
//パーティクルヘッダー[Particle.h]
//Author 小原立暉
//
//===================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include"main.h"

//パーティクルの要因
typedef enum
{
	PTCL_EXPL = 0,					//爆発パーティクル
	PTCL_MAX
}ParticleType;

//プロトタイプ宣言
void InitParticle(void);									//パーティクルの初期化処理
void UninitParticle(void);									//パーティクルの終了処理
void UpdateParticle(void);									//パーティクルの更新処理
void DrawParticle(void);									//パーティクルの描画処理
void SetParticle(D3DXVECTOR3 pos, int nType);				//パーティクルの設定処理

#endif