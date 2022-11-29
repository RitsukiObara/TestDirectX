//=========================================
//
//入力のメイン処理[input.cpp]
//Author 小原立暉
//
//=========================================
#include "input.h"

//マクロ定義
#define NUM_KEY_MAX			(256)			//キーの最大数
#define MAX_PLAYER			(4)				//プレイ出来る人数

//グローバル変数宣言
LPDIRECTINPUT8 g_pInput = NULL;				//DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;	//入力デバイス(キーボード)へのポインタ

											//キーボード用
BYTE g_aKeyState[NUM_KEY_MAX];				//キーボードのプレス情報
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];		//キーボードのトリガー情報
BYTE g_aKeyStateRelease[NUM_KEY_MAX];		//キーボードのリリース情報
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];		//キーボードのリピート情報

											//ゲームパッド用
XINPUT_STATE g_aGamePadState[NUM_KEY_MAX];			//ゲームパッドのプレス情報
XINPUT_STATE g_aGamePadStateTrigger[NUM_KEY_MAX];	//ゲームパッドのトリガー情報

//==================================
//キーボードの初期化処理
//==================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	g_pDevKeyboard->Acquire();
	return S_OK;
}

//==================================
//キーボードの終了処理
//==================================
void UninitKeyboard(void)
{
	//入力デバイス(キーボード)の破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();					//キーボードへのアクセス権を放棄

		g_pDevKeyboard->Release();

		g_pDevKeyboard = NULL;
	}

	//DirectInputオブジェクトの放棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();

		g_pInput = NULL;
	}
}

//=====================================
//キーボードの更新処理
//=====================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];						//キーボードの入力情報
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		//キーボードのトリガー情報を保存

			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];	//キーボードのリリース情報を保存

			g_aKeyState[nCntKey] = aKeyState[nCntKey];	//キーボードのプレス情報を保存
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();						//キーボードへのアクセス権を獲得
	}
}

//======================================
//キーボードのプレス情報を獲得
//======================================
bool GetKeyboardPress(int nKey)
{
	return(g_aKeyState[nKey] & 0x80) ? true : false;
}

//======================================
//キーボードのトリガー情報を獲得
//======================================
bool GetKeyboardTrigger(int nKey)
{
	return(g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//======================================
//キーボードのリリース情報を獲得
//======================================
bool GetKeyboardRelease(int nKey)
{
	return(g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//======================================
//キーボードのリピート情報を獲得
//======================================
bool GetKeyboardRepeat(int nKey)
{
	return(g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//======================================
//ゲームパッドの初期化処理
//======================================
HRESULT InitGamePad(void)
{
	int nGPCount;					//回数の変数を宣言する

									//ゲームパッド設定を有効にする
	XInputEnable(true);

	for (nGPCount = 0; nGPCount < MAX_PLAYER; nGPCount++)
	{//メモリの初期化
	 //ゲームパッドのプレス情報を用意する
		memset(&g_aGamePadState[nGPCount], 0, sizeof(XINPUT_STATE));

		//ゲームパッドのトリガー情報を用意する
		memset(&g_aGamePadStateTrigger[nGPCount], 0, sizeof(XINPUT_STATE));
	}

	return S_OK;
}

//======================================
//ゲームパッドの終了処理
//======================================
void UninitGamePad(void)
{
	//ゲームパッド設定を無効にする
	XInputEnable(false);
}

//======================================
//ゲームパッドの更新処理
//======================================
void UpdateGamePad(void)
{
	XINPUT_STATE aGamePadState[NUM_KEY_MAX];						//ゲームパッドの入力情報
	int nCntGP;

	for (nCntGP = 0; nCntGP < MAX_PLAYER; nCntGP++)
	{//それぞれのゲームパッド分設定する
	 //入力デバイスからデータを取得
		if (XInputGetState(nCntGP, &aGamePadState[nCntGP]) == ERROR_SUCCESS)
		{
			g_aGamePadStateTrigger[nCntGP].Gamepad.wButtons =
				~g_aGamePadState[nCntGP].Gamepad.wButtons & aGamePadState[nCntGP].Gamepad.wButtons
				;		//ゲームパッドのトリガー情報を保存

						//g_aKeyStateRelease[nCntKey] = (g_aGamePadState[nCntKey] ^ aGamePadState[nCntKey]) & ~aGamePadState[nCntKey];		//キーボードのリリース情報を保存

			g_aGamePadState[nCntGP] = aGamePadState[nCntGP];	//ゲームパッドのプレス情報を保存
		}
	}
}

//======================================
//ゲームパッドのプレス入力処理
//======================================
bool GetGamePadPress(JOYKEY nKey, int nPlayer)
{
	return(g_aGamePadState[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//======================================
//ゲームパッドのトリガー入力処理
//======================================
bool GetGamePadTrigger(JOYKEY nKey, int nPlayer)
{
	return(g_aGamePadStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//======================================
//Lスティック(X軸)入力のプレス情報処理
//======================================
SHORT GetGameStickLXPress(JOYKEY nKey, int nPlayer)
{
	return g_aGamePadState[nPlayer].Gamepad.sThumbLX;
}

//======================================
//Lスティック(Y軸)入力のプレス情報処理
//======================================
SHORT GetGameStickLYPress(JOYKEY nKey, int nPlayer)
{
	return g_aGamePadState[nPlayer].Gamepad.sThumbLY;
}

//======================================
//Lスティック(X軸)入力のトリガー情報処理
//======================================
SHORT GetGameStickLXTrigger(JOYKEY nKey, int nPlayer)
{
	return g_aGamePadStateTrigger[nPlayer].Gamepad.sThumbLX;
}

//======================================
//Lスティック(Y軸)入力のトリガー情報処理
//======================================
SHORT GetGameStickLYTrigger(JOYKEY nKey, int nPlayer)
{
	return g_aGamePadStateTrigger[nPlayer].Gamepad.sThumbLY;
}
