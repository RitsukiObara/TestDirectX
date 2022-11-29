//=======================================
//
//入力ヘッダー[input.h]
//Author 小原立暉
//
//=======================================
#ifndef _IMPUT_H_//このマクロ定義がされていなかったら
#define _INPUT_H_//2重インクルード防止のマクロを定義する

#include "main.h"
#include<Xinput.h>

#pragma comment(lib,"xinput.lib")

//ゲームパッドのボタン
typedef enum
{
	BUTTON_UP = 0,			//上ボタン
	BUTTON_DOWN,			//下ボタン
	BUTTON_LEFT,			//左ボタン
	BUTTON_RIGHT,			//右ボタン
	BUTTON_START,			//STARTボタン
	BUTTON_BACK,			//BACKボタン
	BUTTON_LEFTSTICK,		//左のスティック押し込み
	BUTTON_RIGHTSTICK,		//右のスティック押し込み
	BUTTON_LB,				//LBボタン
	BUTTON_RB,				//RBボタン
	BUTTON_PPP1,			//不明
	BUTTON_PPP2,			//不明
	BUTTON_A,				//Aボタン
	BUTTON_B,				//Bボタン
	BUTTON_X,				//Xボタン
	BUTTON_Y,				//Yボタン
	BUTTON_STICKLX,			//Lスティック(X軸)(右に行くほど値は大きい)
	BUTTON_STICKLY,			//Lスティック(Y軸)(上に行くほど値は大きい)
	BUTTON_PPP3,			//不明
	BUTTON_PPP4,			//不明
	BUTTON_MAX
}JOYKEY;

//プロトタイプ宣言
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);			//入力の初期化処理
void UninitKeyboard(void);										//入力の終了処理
void UpdateKeyboard(void);										//入力の更新処理
bool GetKeyboardPress(int nKey);								//入力の描画処理

//キーボード処理
bool GetKeyboardTrigger(int nKey);								//入力のトリガー情報処理
bool GetKeyboardRelease(int nKey);								//入力のリリース情報処理
bool GetKeyboardRepeat(int nKey);								//入力のリピート情報処理

//ゲームパッド用
//プロトタイプ宣言
HRESULT InitGamePad(void);										//ゲームパッド入力の初期化処理
void UninitGamePad(void);										//ゲームパッド入力の終了処理
void UpdateGamePad(void);										//ゲームパッド入力の更新処理
bool GetGamePadPress(JOYKEY nKey, int nPlayer);					//ゲームパッド入力のプレス情報処理
bool GetGamePadTrigger(JOYKEY nKey, int nPlayer);				//ゲームパッド入力のトリガー情報処理
SHORT GetGameStickLXPress(JOYKEY nKey, int nPlayer);			//Lスティック(X軸)入力のプレス情報処理
SHORT GetGameStickLYPress(JOYKEY nKey, int nPlayer);			//Lスティック(Y軸)入力のプレス情報処理
SHORT GetGameStickLXTrigger(JOYKEY nKey, int nPlayer);			//Lスティック(X軸)入力のプレス情報処理
SHORT GetGameStickLYTrigger(JOYKEY nKey, int nPlayer);			//Lスティック(Y軸)入力のプレス情報処理


#endif