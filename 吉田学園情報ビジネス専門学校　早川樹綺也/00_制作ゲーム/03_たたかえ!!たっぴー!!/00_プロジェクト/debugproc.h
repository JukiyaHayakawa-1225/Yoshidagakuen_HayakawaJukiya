//=============================================================================
//
// デバッグ用プロシージャの処理 [debugproc.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

//前方宣言
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CHAR (10000)	//文字の最大数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CDebugProc
{
public:	//誰からもアクセス可能
	//メンバ関数
	CDebugProc();
	~CDebugProc();
	void Init(void);
	void Uninit(void);

	//静的メンバ関数
	static void Print(int nNumber,char *fmt, ...);
	static void ClearString(void);
	static void Draw(void);
	static void SwitchDisp(void);

private:	//自分だけがアクセス可能
	//静的メンバ変数
	static LPD3DXFONT m_pFont;				//フォント
	static char		  m_aStr[MAX_CHAR];		//文字列
	static bool       m_bDisp;              //描画するかどうか
};
#endif
