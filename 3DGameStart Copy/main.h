//======================================
//
//GameMakingのメインヘッダー[main.h]
//Author 小原立暉
//
//======================================
#ifndef _MAIN_H_//このマクロ定義がされていなかったら
#define _MAIN_H_//2重インクルード防止のマクロを定義する

#include <windows.h>
#include "d3dx9.h"						//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)	//ビルド時の警告対処用マクロ
#include "dinput.h"						//入力処理に必要
#include"xaudio2.h"						//サウンド処理に必要

//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")			//描画処理に必要
#pragma comment(lib,"d3dx9.lib")		//[d3d9.lib]
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")		//システム時刻取得に必要
#pragma comment(lib,"dinput8.lib")		//入力処理に必要


//マクロ定義
#define SCREEN_WIDTH				(1280)						//ウインドウの幅
#define SCREEN_HEIGHT				(720)						//ウインドウの高さ
#define FVF_VERTEX_2D				(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)	//頂点フォーマット[2D]
#define FVF_VERTEX_3D				(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)	//頂点フォーマット[3D]
#define CENTER_MAGNI				(0.5f)						//中心位置を割り出すための数値(幅・高さと併用して使用が主)
#define REVERSE_SCALE				(-1)						//符号を反転させる数値(色・透明度の反転の仕様が主)
#define INIT_SHADOW					(-1)						//影の初期化処理
#define ALPHA_TEST_NUMBER			(75)						//アルファテストを有効にしたときの数値

//頂点情報[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;			//頂点座標
	float rhw;					//座標変換用係数(1.0fで固定)
	D3DCOLOR col;				//頂点カラー
	D3DXVECTOR2 tex;			//テクスチャ座標
}VERTEX_2D;

//頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;			//頂点座標
	D3DXVECTOR3 hor;			//法線ベクトル
	D3DCOLOR col;				//頂点カラー
	D3DXVECTOR2 tex;			//テクスチャ座標
}VERTEX_3D;

//画面(モード)の種類
typedef enum
{
	MODE_PROJECT = 0,			//プロジェクト画面
	MODE_TITLE,					//タイトル画面
	MODE_CHOICE,				//ゲームモード選択画面
	MODE_TUTORIAL,				//チュートリアル画面
	MODE_GAME,					//ゲーム画面
	MODE_RESULT,				//リザルト画面
	MODE_RANKING,				//ランキング画面
	MODE_MAX
}MODE;

//プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif