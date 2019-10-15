//=============================================================================
//
// リザルトの処理 [result.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

#define MAX_RESULT (2)

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CResult
{
public:
	//=========================================================================
	// 構造体の定義
	//=========================================================================
	typedef enum
	{
		RESULT_NONE = 0,
		RESULT_CLEAR,	//クリア
		RESULT_GAMEOVER,	//ゲームオーバー
		RESULT_MAX,
	}RESULT;
	//メンバ関数
	CResult();
	~CResult();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetResult(RESULT result);
	RESULT GetResult(void);

	//静的メンバ関数
	static CResult *Create(void);

private:
	//メンバ変数
	int m_nTimer;
	static RESULT m_result;
};
#endif