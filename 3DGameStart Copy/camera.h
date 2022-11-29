//===================================
//
//カメラヘッダー[camera.h]
//Author 小原立暉
//
//===================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//カメラの構造体
typedef struct
{
	D3DXVECTOR3 posV;					//視点
	D3DXVECTOR3 posR;					//注視点
	D3DXVECTOR3 posVDest;				//目的の視点
	D3DXVECTOR3 posRDest;				//目的の注視点
	D3DXVECTOR3 rot;					//向き
	float rotDest;						//目的の向き
	float rotDiff;						//向きの差分
	float Dis;							//距離
	D3DXVECTOR3 VecU;					//上方向ベクトル
	D3DXMATRIX mtxProjection;			//プロジェクションマトリックス
	D3DXMATRIX mtxView;					//ビューマトリックス
	bool bRotation;						//回り込み処理をするかどうか
}Camera;

//カメラのモード
typedef enum
{
	CAMERA_EDIT = 0,					//エディットモード
	CAMERA_CHASE,						//追従モード
	CAMERA_MAX
}CameraMode;

//プロトタイプ宣言
void InitCamera(void);					//カメラの初期化処理
void UninitCamera(void);				//カメラの終了処理
void UpdateCamera(void);				//カメラの更新処理
void SetCamera(void);					//カメラの設定処理
Camera GetCamera(void);					//カメラの取得処理
CameraMode GetCameraMode(void);			//カメラのモードの取得処理

#endif