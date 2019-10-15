//=============================================================================
//
// マネージャ処理 [manager.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

//前方宣言
class CInputKeyboard;
class CInputJoypad;
class CTitle;
class CTutorial;
class CGame;
class CResult;
class CRanking;
class CSound;
class CScore;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CManager
{
public:	//誰からもアクセス可能
	typedef enum
	{
		MODE_NONE = 0,
		MODE_TITLE,		//タイトル
		MODE_TUTORIAL,	//チュートリアル
		MODE_GAME,		//ゲーム
		MODE_RESULT,	//リザルト
		MODE_RANKING,	//ランキング
		MODE_MAX,
	}MODE;
	//メンバ関数
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetGameScore(int score);
	static int  GetGameScore(int nIdx);
	static int  GetUpdateScore(void);

	//静的メンバ関数
	static CRenderer		*GetRenderer(void);
	static CInputKeyboard	*GetKeyboard(void);
	static CInputJoypad		*GetJoypad(void);
	static CSound			*GetSound(void);
	static MODE				GetMode(void);
	static void				SetMode(MODE mode);

private:	//自分だけがアクセス可能
	//静的メンバ変数
	static CRenderer		*m_pRenderer;
	static CInputKeyboard	*m_pInputKeyboard;
	static CInputJoypad		*m_pInputJoypad;
	static MODE				m_mode;
	static CTitle			*m_pTitle;
	static CTutorial		*m_pTutorial;
	static CGame			*m_pGame;
	static CResult			*m_pResult;
	static CRanking			*m_pRanking;
	static CSound			*m_pSound;
	static CScore			*m_pScore;
	static int				m_aRankingScore[5];
	static int				m_nUpdateSocre;
protected:	//自分と派生クラスだけがアクセス可能
};
#endif