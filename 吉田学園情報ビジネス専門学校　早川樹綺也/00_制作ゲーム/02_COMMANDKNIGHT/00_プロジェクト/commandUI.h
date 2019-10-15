//=============================================================================
//
// コマンドUIの処理 [commandUI.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _COMMANDUI_H_
#define _COMMANDUI_H_

#include "main.h"
#include "scene2D.h"

#define MAX_COMMANDUI (3)											//背景の最大数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCommandUI : public CScene2D
{
public:
	typedef enum
	{
		COMMAND_MAIN = 0,
		COMMAND_SKILL,
		COMMAND_ITEM,
		COMMAND_MAX,
	}COMMAND;
	typedef enum
	{
		COMMAND_HEIGHT_NONE = 0,		//コマンド通常状態
		COMMAND_HEIGHT_PROCESS,			//コマンド拡大状態
	}COMMAND_HEIGHT;

	//メンバ関数
	CCommandUI();
	~CCommandUI();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	COMMAND GetCommandState(void);
	COMMAND_HEIGHT GetCommandHeightState(void);
	void SetCommandState(COMMAND state);
	void SetCommandHeightState(COMMAND_HEIGHT heightState);
	//静的メンバ関数
	static CCommandUI *Create(void);
	static HRESULT Load(void);
	static void Unload(void);
private:
	//メンバ変数
	float						m_fHeight;		//高さ
	COMMAND						m_state;		//コマンド状態
	COMMAND_HEIGHT				m_heightState;	//コマンドの高さ状態
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9	m_apTexture[3];	//テクスチャのポインタ

};
#endif