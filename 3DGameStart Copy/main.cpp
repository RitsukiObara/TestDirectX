//=========================================
//
//GameMaking[main.cpp]
//Author 小原立暉
//
//=========================================
#include <stdio.h>			//デバッグ表示に必要
#include <string.h>			//デバッグ表示に必要
#include "main.h"
#include "input.h"
#include "Polygon.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "Shadow.h"
#include "wall.h"
#include "billboard.h"
#include "Effect.h"
#include "sound.h"
#include "Particle.h"
#include "Bullet.h"
#include "Explosion.h"
#include "Enemy.h"
#include "meshfield.h"
#include "Tower.h"
#include "meshwall.h"
#include "meshcylinder.h"
#include "meshdome.h"
#include "exp.h"

//マクロ定義
#define CLASS_NAME					"WindowClass"				//ウインドウクラスの名前
#define WINDOW_NAME					"3DGame"					//ウインドウの名前(キャプションに表示)
#define SCORE_INIT_NUMBER			(-1)						//スコアの初期値
#define BONUS_INIT_NUMBER			(0)							//ボーナス関係の初期値
#define RECORD_INIT_NUMBER			(-1)						//ニューレコードの初期値

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);		//初期化処理
void Uninit(void);												//終了処理
void Update(void);												//更新処理
void Draw(void);												//描画処理
void DrawFPS();													//FPSの表示
void DrawCamera();												//カメラ関係の表示
void DrawControl();												//操作の表示
void DrawDebug();												//デバッグの表示
void DrawDebugModel();											//モデル関係の表示
void DrawDebugEnemy();											//敵のデバッグ表示

//グローバル変数宣言
LPDIRECT3D9 g_pD3D = NULL;										//Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;							//Direct3Dデバイスへのポインタ
LPD3DXFONT g_pFont = NULL;										//フォントへのポインタ
int g_nCountFPS = 0;											//FPSカウンタ
bool g_bDispDebug = false;										//デバッグ表示のON/OFF
MODE g_mode = MODE_PROJECT;										//現在のモード
bool g_Wire;													//ワイヤーフレームのON/OFF

//=========================================
//メイン関数
//=========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),										//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,												//ウインドウのスタイル
		WindowProc,												//ウインドウプロシージャ
		0,														//0にする(通常は使用しない)
		0,														//0にする(通常は使用しない)
		hInstance,												//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),							//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),								//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),								//クライアント領域の背景色
		NULL,													//メニューバー
		CLASS_NAME,												//ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)							//ファイルのアイコン
	};
	HWND hWnd;													//ウインドウハンドル(識別子)
	MSG msg;													//メッセージを格納する変数

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };				//画面サイズの構造体

																//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	DWORD dwCurrentTime;										//現在時刻
	DWORD dwExecLastTime;										//最後に処理した時刻
	DWORD dwFrameCount;											//フレームカウント
	DWORD dwFPSLastTime;										//最後にFPSを計測した時刻

																//ウインドウを生成
	hWnd = CreateWindowEx(0,									//拡張ウインドウスタイル
		CLASS_NAME,												//ウインドウクラスの名前
		WINDOW_NAME,											//ウインドウの名前
		WS_OVERLAPPEDWINDOW,									//ウインドウスタイル
		CW_USEDEFAULT,											//ウインドウの左上X座標
		CW_USEDEFAULT,											//ウインドウの左下Y座標
		(rect.right - rect.left),								//ウインドウの幅
		(rect.bottom - rect.top),								//ウインドウの高さ
		NULL,													//親ウインドウのハンドル
		NULL,													//メニューハンドルまたは子ウインドウID
		hInstance,												//インスタンスハンドル
		NULL);													//ウインドウ作成データ

																//初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化処理が失敗した場合
		return -1;
	}

	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;											//初期化する
	dwExecLastTime = timeGetTime();								//現在時刻を取得(保存)
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);									//ウインドウの表示状態を設定
	UpdateWindow(hWnd);											//クライアント領域を更新

																//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
			 //抜け出す
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);							//仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);							//ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();						//現在時刻を取得
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
			 //FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;					//FPSを測定した時刻を保存

				dwFrameCount = 0;								//フレームカウントをクリア
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過
				dwExecLastTime = dwCurrentTime;					//処理開始の時刻[現在時刻]を保存
																//更新処理
				Update();

				//描画処理
				Draw();

				dwFrameCount++;									//フレームカウントを加算
			}
		}
	}

	//終了処理
	Uninit();

	//分解能を戻す
	timeEndPeriod(1);

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//ウインドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;							//返り値を格納する変数を設定する

	switch (uMsg)
	{
	case WM_DESTROY:					//ウィンドウ破棄のメッセージ
										//WM_QUITメッセージを送る
		PostQuitMessage(0);

		//抜け出す
		break;

	case WM_KEYDOWN:											//キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:											//[ESKキー]が押された
																//終了メッセージを表示
			nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{//「はい」を選んだ場合
			 //ウィンドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			else
			{
				return 0;										//0を返さないと終了してしまう
			}
			//抜け出す
			break;
		}
		//抜け出す
		break;

	case WM_CLOSE:											//コマンド発行のメッセージ
															//終了メッセージを表示
		nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{//「はい」を選んだ場合
		 //ウィンドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;											//0を返さないと終了してしまう
		}
		//抜け出す
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);			//規定の処理を返す
}

//=========================================
//初期化処理
//=========================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;										//ディスプレイモード

	D3DPRESENT_PARAMETERS d3dpp;								//プレゼンテーションパラメータ

																//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							//パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						//ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						//ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						//バックバッファの形式
	d3dpp.BackBufferCount = 1;									//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;						//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					//デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;									//ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//インターバル

																//Direct3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,			//描画処理と頂点処理をハードウェアで行う
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,		//描画処理をハードウェアで行い、頂点処理をCPUで行う
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,	//描画処理と頂点処理をCPUで行う
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//カリングの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//デバッグ表示用のフォントの生成
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	//キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//サウンドの初期化処理
	InitSound(hWnd);

	//ゲームパッドの初期化処理
	InitGamePad();

	//影の初期化処理
	InitShadow();

	//カメラの初期化処理
	InitCamera();

	//ライトの初期化処理
	InitLight();

	////ポリゴンの初期化処理
	//InitPolygon();

	//メッシュフィールドの初期化処理
	Initmeshfield();

	////壁の初期化処理
	//InitWall();

	//メッシュウォールの初期化処理
	Initmeshwall();

	//モデルの初期化処理
	InitModel();

	//ビルボードの初期化処理
	InitBillboard();

	//エフェクトの初期化処理
	InitEffect();

	//パーティクルの初期化処理
	InitParticle();

	//弾の初期化処理
	InitBullet();

	//爆発の初期化処理
	InitExplosion();

	//敵の初期化処理
	InitEnemy();

	//塔の初期化処理
	InitTower();

	//メッシュシリンダーの初期化処理
	InitmeshCylinder();

	//メッシュドームの初期化処理
	InitmeshDome();

	//経験値の初期化処理
	InitExp();

	//ワイヤーフレームのOFF
	g_Wire = false;													

	////ゲーム音楽を流す
	//PlaySound(SOUND_LABEL_BGM_GAME);

	return S_OK;
}

//=============================================
//終了処理
//=============================================
void Uninit(void)
{
	//サウンドの終了処理
	UninitSound();

	//キーボードの終了処理
	UninitKeyboard();

	//ゲームパッドの終了処理
	UninitGamePad();

	//影の終了処理
	UninitShadow();

	//カメラの終了処理
	UninitCamera();

	//ライトの終了処理
	UninitLight();

	////ポリゴンの終了処理
	//UninitPolygon();

	//メッシュフィールドの終了処理
	Uninitmeshfield();

	////壁の終了処理
	//UninitWall();

	//メッシュウォールの終了処理
	Uninitmeshwall();

	//モデルの終了処理
	UninitModel();

	//ビルボードの終了処理
	UninitBillboard();

	//エフェクトの終了処理
	UninitEffect();

	//パーティクルの終了処理
	UninitParticle();

	//弾の終了処理
	UninitBullet();

	//爆発の終了処理
	UninitExplosion();

	//敵の終了処理
	UninitEnemy();

	//塔の終了処理
	UninitTower();

	//メッシュシリンダーの終了処理
	UninitmeshCylinder();

	//メッシュドームの終了処理
	UninitmeshDome();

	//経験値の終了処理
	UninitExp();

	//デバッグ表示用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	//Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();

		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();

		g_pD3D = NULL;
	}
}

//======================================
//更新処理
//======================================
void Update(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	//ゲームパッドの更新処理
	UpdateGamePad();

	//影の更新処理
	UpdateShadow();

	//ライトの更新処理
	UpdateLight();

	////ポリゴンの更新処理
	//UpdatePolygon();

	//メッシュフィールドの更新処理
	Updatemeshfield();

	////壁の更新処理
	//UpdateWall();

	//メッシュウォールの更新処理
	Updatemeshwall();

	//モデルの更新処理
	UpdateModel();

	//カメラの更新処理
	UpdateCamera();

	//ビルボードの更新処理
	UpdateBillboard();

	//エフェクトの更新処理
	UpdateEffect();

	//パーティクルの更新処理
	UpdateParticle();

	//弾の更新処理
	UpdateBullet();

	//爆発の更新処理
	UpdateExplosion();

	//塔の更新処理
	UpdateTower();

	//敵の更新処理
	UpdateEnemy();

	//メッシュシリンダーの更新処理
	UpdatemeshCylinder();

	//メッシュドームの更新処理
	UpdatemeshDome();

	//経験値の更新処理
	UpdateExp();

	if (GetKeyboardTrigger(DIK_F2) == true)
	{//F2キーを押す
		//敵の設定処理
		SetEnemy(ENEMY_INSECT);
	}

	if (GetKeyboardTrigger(DIK_F3) == true)
	{//F3キーを押す
		//経験値の発生
		SetExp(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (GetKeyboardTrigger(DIK_F1) == true)
	{//F1キーを押した
		if (g_bDispDebug == true)
		{//キーがONだった場合
		 //キーをOFFにする
			g_bDispDebug = false;
		}
		else if (g_bDispDebug == false)
		{//キーがOFFだった場合
		 //キーをONにする
			g_bDispDebug = true;
		}
	}
}

//======================================
//描画処理
//======================================
void Draw(void)
{
	//画面クリア(バックバッファとZバッファのクリア)
	g_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

		if (GetKeyboardTrigger(DIK_F9) == true)
		{//F9キーを押した場合
			//ワイヤーフレームのON/OFF
			g_Wire = g_Wire ? false : true;

			if (g_Wire == true)
			{//ワイヤーモードONの場合
				//ワイヤーフレームをONにする
				g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}
			else
			{//ワイヤーモードOFFの場合
				//ワイヤーフレームをOFFにする
				g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			}
		}

		//カメラの設定処理
		SetCamera();

		////ポリゴンの描画処理
		//DrawPolygon();

		////メッシュシリンダーの描画処理
		//DrawmeshCylinder();

		////メッシュドームの描画処理
		//DrawmeshDome();

		//メッシュフィールドの描画処理
		Drawmeshfield();

		//影の描画処理
		DrawShadow();

		//モデルの描画処理
		DrawModel();

		//ビルボードの描画処理
		DrawBillboard();

		//エフェクトの描画処理
		DrawEffect();

		//パーティクルの描画処理
		DrawParticle();

		//経験値の描画処理
		DrawExp();

		////壁の描画処理
		//DrawWall();

		//メッシュウォールの描画処理
		Drawmeshwall();

		//弾の描画処理
		DrawBullet();

		//爆発の描画処理
		DrawExplosion();

		//敵の描画処理
		DrawEnemy();

		//塔の描画処理
		DrawTower();

		//デバッグの描画
		DrawDebug();

		//描画終了
		g_pD3DDevice->EndScene();
	}
	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//===========================================
//デバイスの取得
//===========================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//============================================
//FPSの表示
//============================================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[128];

	//文字列に代入
	sprintf(&aStr[0], "FPS:%d", g_nCountFPS);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0, 0, 0, 255));
}

//==============================================
//カメラ関係の表示
//==============================================
void DrawCamera()
{
	RECT rect = { 0,18,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[256];

	Camera CameraInfo = GetCamera();					//カメラの情報を取得する
	CameraMode CameraMode= GetCameraMode();				//カメラのモードを取得する

	//文字列に代入
	sprintf(&aStr[0], "カメラの視点:(%f,%f,%f)\nカメラの注視点:(%f,%f,%f)\nカメラの向き:(%f,%f,%f)\nカメラの距離:%f\nカメラの目的の向き:[%f]\n====================================================",
		CameraInfo.posV.x, CameraInfo.posV.y, CameraInfo.posV.z,
		CameraInfo.posR.x, CameraInfo.posR.y, CameraInfo.posR.z,
		CameraInfo.rot.x, CameraInfo.rot.y, CameraInfo.rot.z,
		CameraInfo.Dis, CameraInfo.rotDest);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0, 0, 0, 255));
}

//==============================================
//操作の表示
//==============================================
void DrawControl()
{
	RECT rect = { 0,144,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[256];

	if (g_Wire == true)
	{//ワイヤーモードONだった場合
		//文字列に代入
		sprintf(&aStr[0], "カメラ移動:[W/A/S/D]\n視点操作:[Y/N/Z/C]\n注視点操作:[T/B/Q/E]\n距離の調整:[U/M]\nリセット:[SPACE]\nモード変更:[F8]\nワイヤー:[F9]:ON\n====================================================");
	}
	else
	{//ワイヤーモードOFFだった場合
		//文字列に代入
		sprintf(&aStr[0], "カメラ移動:[W/A/S/D]\n視点操作:[Y/N/Z/C]\n注視点操作:[T/B/Q/E]\n距離の調整:[U/M]\nリセット:[SPACE]\nモード変更:[F8]\nワイヤー:[F9]:OFF\n====================================================");
	}

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0, 0, 0, 255));
}

//==============================================
//モデル関係の表示
//==============================================
void DrawDebugModel()
{
	RECT rect = { 0,288,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[256];

	Model *pModel = GetModel();					//モデルの位置を取得する

	//文字列に代入
	sprintf(&aStr[0], "モデルの移動:[↑/↓/→/←]\n弾発射:[0]\nモデルの位置リセット:[ENTER]\nモデルの座標:(%f,%f,%f)\nモデルの向き:(%f,%f,%f)\nモデルの移動量:[%f,%f,%f]", pModel->pos.x, pModel->pos.y, pModel->pos.z, pModel->rot.x, pModel->rot.y, pModel->rot.z,pModel->move.x, pModel->move.y, pModel->move.z);
	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0, 0, 0, 255));
}

//================================================
//敵のデバッグ表示
//================================================
void DrawDebugEnemy()
{
	RECT rect = { 0,400,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[256];

	int nEnemyCount = GetEnemyCount();					//敵の総数を取得する

	//文字列に代入
	sprintf(&aStr[0], "====================================================\n敵の数:[%d]\n敵の生産:[F2]\n経験値の生産:[F3]",nEnemyCount);
	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0, 0, 0, 255));
}

//==============================================
//デバッグの表示
//==============================================
void DrawDebug()
{
	//DrawFPSのオンオフスイッチ
#if 1

	if (g_bDispDebug == false)
	{
		//デバッグの時のみ表示する
#ifdef _DEBUG

		//FPSの表示
		DrawFPS();

		//敵の総数表示
		DrawDebugEnemy();
#endif
		//カメラの表示
		DrawCamera();

		//操作の表示
		DrawControl();

		//モデル関係の表示
		DrawDebugModel();

		//敵のデバッグ表示
		DrawDebugEnemy();
	}

#endif
}

//==============================================
//モードの設定
//==============================================
void SetMode(MODE mode)
{

}

//==============================================
//モードの取得
//==============================================
MODE GetMode(void)
{
	return g_mode;
}