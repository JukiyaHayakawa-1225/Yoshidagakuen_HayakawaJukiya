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
class CSound;
class CCamera;
class CLight;
class CDebugProc;
class CTitle;
class CTutorial;
class CGame;
class CResult;
class CRanking;
class CFade;
class CCommandText;

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

	//静的メンバ関数
	static CRenderer		*GetRenderer(void);
	static CInputKeyboard	*GetKeyboard(void);
	static CInputJoypad		*GetJoypad(void);
	static CSound			*GetSound(void);
	static CCamera			*GetCamera(void);
	static CLight			*GetLight(void);
	static MODE				GetMode(void);
	static void				SetMode(MODE mode);
	static CFade			*GetFade(void);
	void SetGameScore(int score);
	static int  GetGameScore(int nIdx);
	static int  GetUpdateScore(void);
	static int  GetTime(void);
	static int GetScore(void);
	void SetScore(int nScore);
	void SetBird(int nValue);
	void SetFrog(int nValue);
	static int GetBird(void);
	static int GetFrog(void);
private:	//自分だけがアクセス可能
	//静的メンバ変数
	static CRenderer		*m_pRenderer;		//レンダリングのポインタ
	static CInputKeyboard	*m_pInputKeyboard;	//キーボートのポインタ
	static CInputJoypad		*m_pInputJoypad;	//ジョイパッドのポインタ
	static CSound			*m_pSound;			//サウンドのポインタ
	static CCamera			*m_pCamera;			//カメラのポインタ
	static CLight			*m_pLight;			//ライトのポインタ
	static CDebugProc		*m_pDebugproc;		//デバッグのポインタ
	static MODE				m_mode;				//モードの情報
	static CTitle			*m_pTitle;			//タイトルのポインタ
	static CTutorial		*m_pTutorial;		//チュートリアルのポインタ
	static CGame			*m_pGame;			//ゲームのポインタ
	static CResult			*m_pResult;			//リザルトのポインタ
	static CRanking			*m_pRanking;		//ランキングのポインタ
	static CFade			*m_pFade;			//フェードのポインタ
	static CCommandText		*m_pCommandText;	//コマンドテキストのポインタ
	static int				m_aRankingScore[5];
	static int				m_nUpdateSocre;
	static int				m_nTimeData;
	static int				m_nScore;
	static int				m_nNumBird;
	static int				m_nNumFrog;
protected:	//自分と派生クラスだけがアクセス可能
};
#endif