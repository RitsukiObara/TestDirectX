//=============================================================================
//
// サウンド処理 [sound.h]
// Author 小原立暉
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_GAME,		//ゲームBGM
	SOUND_LABEL_SE_CLEAR,		//クリア音
	SOUND_LABEL_BGM_TITLE,		//オープニングBGM
	SOUND_LABEL_SE_SELECT,		//決定音
	SOUND_LABEL_BGM_TUTORIAL,	//チュートリアルBGM
	SOUND_LABEL_BGM_RESULT,		//リザルトBGM
	SOUND_LABEL_SE_CHOICE,		//選択音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
HRESULT ReplaySound(SOUND_LABEL label);

#endif
