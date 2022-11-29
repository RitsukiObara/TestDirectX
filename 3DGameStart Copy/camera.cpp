//===========================================
//
//カメラのメイン処理[camera.cpp]
//Author 小原立暉
//
//===========================================
#include "main.h"
#include "camera.h"
#include "input.h"
#include "model.h"

//マクロ定義
#define ROT_Y_SPEED			(0.04f)				//Y軸の回転の速度
#define POS_SPEED			(7.0f)				//移動速度
#define DIS_SPEED			(4.0f)				//距離の移動量
#define ROTATION_SPEED		(0.05f)				//回り込み処理を行う基準のモデルの速度
#define DRAW_MIN_Z			(10.0f)				//Z軸の最小値
#define DRAW_MAX_Z			(2000.0f)			//Z軸の最大値
#define CAMERA_DISTANCE		(300.0f)			//カメラの距離
#define CAMERA_INIT_POS_Y	(300.0f)			//カメラの初期位置(Y軸)
#define ROTATION_ROT		(0.02f)				//カメラの角度の補正倍率
#define MIN_DISTANCE		(50.0f)				//距離の最小値
#define MAX_DISTANCE		(600.0f)			//距離の最大値
#define POSR_POINT			(40.0f)				//追従モードの注視点の位置
#define POSV_POINT			(40.0f)				//追従モードの視点の位置
#define CORRECT_POSR		(0.22f)				//注視点の補正倍率
#define CORRECT_POSV		(0.20f)				//視点の補正倍率

//プロトタイプ宣言
void MoveCamera(void);							//カメラの移動処理
void MovePosV(void);							//カメラの視点操作処理
void MovePosR(void);							//カメラの注視点操作処理
void MoveRange(void);							//カメラの距離操作処理
void ChaseCamera(Model *pModel);				//追従処理
void ResetCamera(void);							//カメラの情報のリセット処理
void RotationCamera(Model *pModel);				//カメラの回り込み処理

//グローバル変数
Camera g_camera;								//カメラの情報
CameraMode g_CameraMode;						//カメラのモード

//==========================================
//カメラの初期化処理
//==========================================
void InitCamera(void)
{
	//カメラの情報のリセット処理
	ResetCamera();

	//カメラのモードを初期化する
	g_CameraMode = CAMERA_CHASE;

	//回り込み処理をしない
	g_camera.bRotation = true;
}

//========================================
//弾のゲージの終了処理
//========================================
void UninitCamera(void)
{
	
}

//========================================
//カメラの更新処理
//========================================
void UpdateCamera(void)
{
	Model *pModel = GetModel();						//モデルの情報を初期化する

	if (g_CameraMode == CAMERA_EDIT)
	{//エディットモードだった場合
		//カメラの移動処理
		MoveCamera();

		//カメラの視点操作処理
		MovePosV();							

		//カメラの注視点操作処理
		MovePosR();							

		//カメラの距離操作処理
		MoveRange();							

		if (g_camera.rot.y > D3DX_PI)
		{//3.14fより大きくなった場合
			//-3.14fに補正する
			g_camera.rot.y = -D3DX_PI;
		}
		else if (g_camera.rot.y < -D3DX_PI)
		{//-3.14fより小さくなった場合
			//3.14fに補正する
			g_camera.rot.y = D3DX_PI;
		}

		//カメラの視点を更新する
		g_camera.posV.x = g_camera.posR.x + sinf(g_camera.rot.y) * -g_camera.Dis;
		g_camera.posV.z = g_camera.posR.z + cosf(g_camera.rot.y) * -g_camera.Dis;

		//カメラの注視点を更新する
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.Dis;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.Dis;
	}
	else if (g_CameraMode == CAMERA_CHASE)
	{//追従モードだった場合
		//追従処理
		ChaseCamera(pModel);

		if (GetKeyboardTrigger(DIK_F4) == true)
		{//F4キーを押した場合
			//回り込みモードを入れ替える
			g_camera.bRotation = g_camera.bRotation ? false : true;
		}

		if (g_camera.bRotation == true)
		{//回り込み処理を行う
			if (pModel->move.x <= ROTATION_SPEED &&
				pModel->move.z <= ROTATION_SPEED &&
				pModel->move.x >= -ROTATION_SPEED &&
				pModel->move.z >= -ROTATION_SPEED)
			{//移動量が0.01f以下かつ、移動量が-0.01f以上だった場合
				//カメラの回り込み処理
				RotationCamera(pModel);
			}
		}
	}

	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{//スペースキーを押した場合
		//カメラの情報のリセット処理
		ResetCamera();
	}

	if (GetKeyboardTrigger(DIK_F8) == true)
	{//F8キーを押した場合
		if (g_CameraMode == CAMERA_EDIT)
		{//エディットモードだった場合
			//追従モードに変更する
			g_CameraMode = CAMERA_CHASE;
		}
		else
		{//追従モードだった場合
			//エディットモードに変更する
			g_CameraMode = CAMERA_EDIT;
		}

		//カメラの情報のリセット処理
		ResetCamera();
	}
}

//=====================================
//カメラの設定処理
//=====================================
void SetCamera(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(45.0f),								//視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,			//画面のアスペクト比
		DRAW_MIN_Z,											//Z値の最小値
		DRAW_MAX_Z);										//Z値の最大値

	//プロジェクトマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,										//視点
		&g_camera.posR,										//注視点
		&g_camera.VecU);									//上方向ベクトル

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//===================================
//カメラの取得処理
//===================================
Camera GetCamera(void)
{
	//カメラを返す
	return g_camera;
}

//===================================
//カメラのモードの取得処理
//===================================
CameraMode GetCameraMode(void)
{
	//カメラのモードを返す
	return g_CameraMode;
}

//===================================
//カメラの情報のリセット処理
//===================================
void ResetCamera(void)
{
	//視点と注視点の距離を初期化する
	g_camera.Dis = CAMERA_DISTANCE;

	//視点の位置を初期化する
	g_camera.posV = D3DXVECTOR3(0.0f, CAMERA_INIT_POS_Y, -g_camera.Dis);

	//注視点の位置を初期化する
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//向きを初期化する
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//カメラの上方向ベクトルを初期化する
	g_camera.VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//カメラの目的の注視点を初期化する
	g_camera.posRDest = g_camera.posR;

	//カメラの目的の視点を初期化する
	g_camera.posVDest = g_camera.posV;

	//カメラの目的の向きを初期化する
	g_camera.rotDest = g_camera.rot.y;

	//カメラの向きの差分を初期化する
	g_camera.rotDiff = g_camera.rotDest - g_camera.rot.y;
}

//======================================
//カメラの回り込み処理
//======================================
void RotationCamera(Model *pModel)
{
	//カメラの目的の向きを設定する
	g_camera.rotDest = (pModel->rot.y);

	//カメラの向きの差分を設定する
	g_camera.rotDiff = g_camera.rotDest - g_camera.rot.y;

	if (g_camera.rotDiff > D3DX_PI)
	{//角度が3.14fより大きかった場合
		//角度から1周分減らす
		g_camera.rotDiff -= (2 * D3DX_PI);
	}
	else if (g_camera.rotDiff < -D3DX_PI)
	{//角度が-3.14fより小さかった場合
		//角度に1周分加える
		g_camera.rotDiff += (2 * D3DX_PI);
	}

	//カメラの向きを補正する
	g_camera.rot.y += g_camera.rotDiff * ROTATION_ROT;

	if (g_camera.rot.y > D3DX_PI)
	{//角度が3.14fより大きかった場合
		//角度から1周分減らす
		g_camera.rot.y = -D3DX_PI;
	}
	else if (g_camera.rot.y < -D3DX_PI)
	{//角度が-3.14fより小さかった場合
		//角度に1周分加える
		g_camera.rot.y = D3DX_PI;
	}
}

//==============================
//カメラの移動処理
//==============================
void MoveCamera(void)
{
	if (GetKeyboardPress(DIK_W) == true)
	{//Wキーを押した場合
		if (GetKeyboardPress(DIK_D) == true)
		{//Dキーを押した場合
			g_camera.posR.x += sinf(D3DX_PI * 0.25f + g_camera.rot.y) * POS_SPEED;
			g_camera.posV.x += sinf(D3DX_PI * 0.25f + g_camera.rot.y) * POS_SPEED;

			g_camera.posR.z += cosf(D3DX_PI * 0.25f + g_camera.rot.y) * POS_SPEED;
			g_camera.posV.z += cosf(D3DX_PI * 0.25f + g_camera.rot.y) * POS_SPEED;
		}
		else if (GetKeyboardPress(DIK_A) == true)
		{//Aキーを押した場合
			g_camera.posR.x += sinf(-D3DX_PI * 0.25f + g_camera.rot.y) * POS_SPEED;
			g_camera.posV.x += sinf(-D3DX_PI * 0.25f + g_camera.rot.y) * POS_SPEED;

			g_camera.posR.z += cosf(-D3DX_PI * 0.25f + g_camera.rot.y) * POS_SPEED;
			g_camera.posV.z += cosf(-D3DX_PI * 0.25f + g_camera.rot.y) * POS_SPEED;
		}
		else
		{//Wキーのみを押した場合
			g_camera.posR.x += sinf(g_camera.rot.y) * POS_SPEED;
			g_camera.posV.x += sinf(g_camera.rot.y) * POS_SPEED;

			g_camera.posR.z += cosf(g_camera.rot.y) * POS_SPEED;
			g_camera.posV.z += cosf(g_camera.rot.y) * POS_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{//Sキーを押した場合
		if (GetKeyboardPress(DIK_D) == true)
		{//Dキーを押した場合
			g_camera.posR.x += sinf(D3DX_PI * 0.75f + g_camera.rot.y) * POS_SPEED;
			g_camera.posV.x += sinf(D3DX_PI * 0.75f + g_camera.rot.y) * POS_SPEED;

			g_camera.posR.z += cosf(D3DX_PI * 0.75f + g_camera.rot.y) * POS_SPEED;
			g_camera.posV.z += cosf(D3DX_PI * 0.75f + g_camera.rot.y) * POS_SPEED;
		}
		else if (GetKeyboardPress(DIK_A) == true)
		{//Aキーを押した場合
			g_camera.posR.x += sinf(-D3DX_PI * 0.75f + g_camera.rot.y) * POS_SPEED;
			g_camera.posV.x += sinf(-D3DX_PI * 0.75f + g_camera.rot.y) * POS_SPEED;

			g_camera.posR.z += cosf(-D3DX_PI * 0.75f + g_camera.rot.y) * POS_SPEED;
			g_camera.posV.z += cosf(-D3DX_PI * 0.75f + g_camera.rot.y) * POS_SPEED;
		}
		else
		{//Sキーのみを押した場合
			g_camera.posR.x -= sinf(g_camera.rot.y) * POS_SPEED;
			g_camera.posV.x -= sinf(g_camera.rot.y) * POS_SPEED;

			g_camera.posR.z -= cosf(g_camera.rot.y) * POS_SPEED;
			g_camera.posV.z -= cosf(g_camera.rot.y) * POS_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{//Dキーを押した場合
		g_camera.posR.x += sinf(D3DX_PI * 0.5f + g_camera.rot.y) * POS_SPEED;
		g_camera.posV.x += sinf(D3DX_PI * 0.5f + g_camera.rot.y) * POS_SPEED;

		g_camera.posR.z += cosf(D3DX_PI * 0.5f + g_camera.rot.y) * POS_SPEED;
		g_camera.posV.z += cosf(D3DX_PI * 0.5f + g_camera.rot.y) * POS_SPEED;
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{//Aキーを押した場合
		g_camera.posR.x -= sinf(D3DX_PI * 0.5f + g_camera.rot.y) * POS_SPEED;
		g_camera.posV.x -= sinf(D3DX_PI * 0.5f + g_camera.rot.y) * POS_SPEED;

		g_camera.posR.z += cosf(D3DX_PI * 0.5f - g_camera.rot.y) * POS_SPEED;
		g_camera.posV.z += cosf(D3DX_PI * 0.5f - g_camera.rot.y) * POS_SPEED;
	}
}

//================================
//カメラの視点操作処理
//================================
void MovePosV(void)
{
	if (GetKeyboardPress(DIK_Y) == true)
	{//Yキーを押した場合
		//視点を上に動かす
		g_camera.posV.y += POS_SPEED;
	}
	else if (GetKeyboardPress(DIK_N) == true)
	{//Nキーを押した場合
		//視点を下に動かす
		g_camera.posV.y -= POS_SPEED;
	}
	else if (GetKeyboardPress(DIK_Z) == true)
	{//Zキーを押した場合
		//視点を右に回す
		g_camera.rot.y -= ROT_Y_SPEED;
	}
	else if (GetKeyboardPress(DIK_C) == true)
	{//Cキーを押した場合
		//視点を左に回す
		g_camera.rot.y += ROT_Y_SPEED;
	}
}

//====================================
//カメラの注視点操作処理
//====================================
void MovePosR(void)
{
	if (GetKeyboardPress(DIK_T) == true)
	{//Tキーを押した場合
		//注視点を上に動かす
		g_camera.posR.y += POS_SPEED;
	}
	else if (GetKeyboardPress(DIK_B) == true)
	{//Bキーを押した場合
		//注視点を下に動かす
		g_camera.posR.y -= POS_SPEED;
	}
	else if (GetKeyboardPress(DIK_Q) == true)
	{//Qキーを押した場合
		//注視点を右に回す
		g_camera.rot.y -= ROT_Y_SPEED;

		//注視点を更新する
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.Dis;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.Dis;
	}
	else if (GetKeyboardPress(DIK_E) == true)
	{//Eキーを押した場合
		//注視点を左に回す
		g_camera.rot.y += ROT_Y_SPEED;

		//注視点を更新する
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.Dis;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.Dis;
	}
}

//================================
//カメラの距離操作処理
//================================
void MoveRange(void)
{
	if (GetKeyboardPress(DIK_U) == true)
	{//Uキーを押した場合
		g_camera.Dis -= DIS_SPEED;

		if (g_camera.Dis <= MIN_DISTANCE)
		{//100.0f以下の場合
			//100.0fにする
			g_camera.Dis = MIN_DISTANCE;
		}
	}
	else if (GetKeyboardPress(DIK_M) == true)
	{//Mキーを押した場合
		g_camera.Dis += DIS_SPEED;

		if (g_camera.Dis >= MAX_DISTANCE)
		{//800.0f以上の場合
			//800.0fにする
			g_camera.Dis = MAX_DISTANCE;
		}
	}
}

//=====================================
//追従処理
//=====================================
void ChaseCamera(Model *pModel)
{
	//目的の注視点を設定する
	g_camera.posRDest.x = pModel->pos.x + sinf(pModel->rot.y) * POSR_POINT;
	g_camera.posRDest.z = pModel->pos.z + cosf(pModel->rot.y) * POSR_POINT;

	//目的の視点を設定する
	g_camera.posVDest.x = g_camera.posRDest.x + sinf(g_camera.rot.y) * -g_camera.Dis + sinf(pModel->rot.y) * POSV_POINT;
	g_camera.posVDest.z = g_camera.posRDest.z + cosf(g_camera.rot.y) * -g_camera.Dis + cosf(pModel->rot.y) * POSV_POINT;

	//注視点を補正
	g_camera.posR.x += (g_camera.posRDest.x - g_camera.posR.x) * CORRECT_POSR;
	g_camera.posR.z += (g_camera.posRDest.z - g_camera.posR.z) * CORRECT_POSR;

	//視点を補正
	g_camera.posV.x += (g_camera.posVDest.x - g_camera.posV.x) * CORRECT_POSV;
	g_camera.posV.z += (g_camera.posVDest.z - g_camera.posV.z) * CORRECT_POSV;
}