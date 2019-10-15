//=============================================================================
//
// リザルトの処理 [result.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

#define MAX_RESULT (RESULT_MAX)	//リザルトの数
class CScore;
class CTimer;
class CMap;
class CBirdNumber;
class CFrogNumber;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CResult
{
public:	//誰からもアクセス可能
	//=========================================================================
	// 構造体の定義
	//=========================================================================
	typedef enum
	{
		RESULT_NONE = 0,
		RESULT_CLEAR,		//クリア
		RESULT_GAMEOVER,	//ゲームオーバー
		RESULT_MAX,
	}RESULT;
	typedef enum
	{
		COUNT_TIMER = 0,
		COUNT_BIRD,
		COUNT_FROG,
		COUNT_END,
		COUNT_MAX
	}COUNT_TYPE;
	//メンバ関数
	CResult();
	~CResult();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetResult(RESULT result);
	RESULT GetResult(void);
	static void SetType(COUNT_TYPE);
	static COUNT_TYPE GetType(void);

	//静的メンバ関数
	static CResult *Create(void);
	static CScore		*GetScore(void);
	static CBirdNumber	*GetBirdNumber(void);
	static CFrogNumber	*GetFrogNumber(void);

private:	//自分だけがアクセス可能
	//メンバ変数
	int m_nTimer;			//タイマー
	static	CScore			*m_pScore;	// テクスチャへのポインタ
	static CTimer			*m_pTimer;		//タイマーのポインタ
	static CMap				*m_pMap;
	static CBirdNumber		*m_pBirdNumber;
	static CFrogNumber		*m_pFrogNumber;
	//静的メンバ変数
	static RESULT m_result;	//リザルトの情報
	static COUNT_TYPE				m_type;
};
#endif