//=============================================================================
//
// マネージャ処理 [manager.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "manager.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "scene3D.h"
#include "sceneX.h"
#include "debugproc.h"
#include "player.h"
#include "meshfield.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "fade.h"
#include "pause.h"
#include "timer.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CManager::MODE	CManager::m_mode = MODE::MODE_TITLE;			//モードの情報
CRenderer		*CManager::m_pRenderer = NULL;				//レンダリングポインタ
CInputKeyboard	*CManager::m_pInputKeyboard = NULL;			//キーボードポインタ
CInputJoypad	*CManager::m_pInputJoypad = NULL;			//ジョイパッドポインタ
CSound			*CManager::m_pSound = NULL;					//サウンドのポインタ
CCamera			*CManager::m_pCamera = NULL;				//カメラのポインタ
CLight			*CManager::m_pLight = NULL;					//ライトのポインタ
CDebugProc		*CManager::m_pDebugproc = NULL;				//デバッグのポインタ
CTitle			*CManager::m_pTitle = NULL;					//タイトルのポインタ
CTutorial		*CManager::m_pTutorial = NULL;				//チュートリアルのポインタ
CGame			*CManager::m_pGame = NULL;					//ゲームのポインタ
CResult			*CManager::m_pResult = NULL;				//リザルトのポインタ
CRanking		*CManager::m_pRanking = NULL;				//ランキングのポインタ
CFade			*CManager::m_pFade = NULL;					//フェードのポインタ
CCommandText	*CManager::m_pCommandText = NULL;			//コマンドテキストのポインタ
int				CManager::m_aRankingScore[5] = {};
int				CManager::m_nUpdateSocre = NULL;
int				CManager::m_nTimeData = 0;
int				CManager::m_nScore = 0;
int				CManager::m_nNumBird = 0;
int				CManager::m_nNumFrog = 0;

//=============================================================================
// マネージャのコンストラクタ
//=============================================================================
CManager::CManager()
{
}

//=============================================================================
// マネージャのデストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// マネージャの初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	//レンダリングの動的確保
	if (m_pRenderer == NULL)
	{
		m_pRenderer = new CRenderer;			//レンダリングの動的確保
	}

	//キーボードの動的確保
	if (m_pInputKeyboard == NULL)
	{
		m_pInputKeyboard = new CInputKeyboard;	//キーボードの動的確保
	}

	//ジョイパッドの動的確保
	if (m_pInputJoypad == NULL)
	{
		m_pInputJoypad = new CInputJoypad;		//ジョイパッドの動的確保
	}

	//サウンドの動的確保
	if (m_pSound == NULL)
	{
		m_pSound = new CSound;					//サウンドの動的確保
	}

	//カメラの動的確保
	if (m_pCamera == NULL)
	{
		m_pCamera = new CCamera;
	}

	//ライトの動的確保
	if (m_pLight == NULL)
	{
		m_pLight = new CLight;
	}

	//デバッグの動的確保
	if (m_pDebugproc == NULL)
	{
		m_pDebugproc = new CDebugProc;
	}

	//レンダリングの初期化処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Init(hWnd, bWindow);
	}

	//キーボードの初期化処理
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Init(hInstance,hWnd);
	}

	//ジョイパッドの初期化処理
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Init(hInstance, hWnd);
	}

	//サウンドの初期化処理
	if (m_pSound != NULL)
	{
		m_pSound->InitSound(hWnd);
	}

	//カメラの初期化処理
	if (m_pCamera != NULL)
	{
		m_pCamera->Init();
	}

	//ライトの初期化処理
	if (m_pLight != NULL)
	{
		m_pLight->Init();
	}

	//デバッグの初期化処理
	if (m_pDebugproc != NULL)
	{
		m_pDebugproc->Init();
	}

	int nScore = 5000;

	for (int nCntScore = 0; nCntScore < 5; nCntScore++)
	{
		m_aRankingScore[nCntScore] = nScore;
		nScore -= 1000;
	}
	m_nUpdateSocre = 0;

	//フェードの生成
	m_pFade = CFade::Create();
	//フェードの設置処理
	m_pFade->SetFade(m_mode, m_pFade->FADE_IN);
	//モードの設置
	//SetMode(m_mode);

	return S_OK;
}

//=============================================================================
// マネージャの終了処理
//=============================================================================
void CManager::Uninit(void)
{
	//キーボードの終了処理
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	//ジョイパッドの終了処理
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}

	//レンダリングの終了処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	//フェードの終了処理
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = NULL;
	}
	//サウンドの終了処理
	if (m_pSound != NULL)
	{
		m_pSound->UninitSound();
		delete m_pSound;
		m_pSound = NULL;
	}

	//カメラの終了処理
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//ライトの終了処理
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	//デバッグの終了処理
	if (m_pDebugproc != NULL)
	{
		m_pDebugproc->Uninit();
		delete m_pDebugproc;
		m_pDebugproc = NULL;
	}

	if (m_pTitle != NULL)
	{
		//タイトルの終了処理
		m_pTitle->Uninit();
		//メモリの開放
		delete m_pTitle;
		m_pTitle = NULL;
	}
	if (m_pTutorial != NULL)
	{
		//チュートリアルの終了処理
		m_pTutorial->Uninit();
		//メモリの開放
		delete m_pTutorial;
		m_pTutorial = NULL;
	}
	if (m_pGame != NULL)
	{
		//ゲームの終了処理
		m_pGame->Uninit();
		//メモリの開放
		delete m_pGame;
		m_pGame = NULL;
	}
	if (m_pResult != NULL)
	{
		//リザルトの終了処理
		m_pResult->Uninit();
		//メモリの開放
		delete m_pResult;
		m_pResult = NULL;
	}
	if (m_pRanking != NULL)
	{
		//リザルトの終了処理
		m_pRanking->Uninit();
		//メモリの開放
		delete m_pRanking;
		m_pRanking = NULL;
	}

	//全ての開放処理
	CScene::ReleaseAll();
}

//=============================================================================
// マネージャの更新処理
//=============================================================================
void CManager::Update(void)
{
	//キーボードの更新処理
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	//ジョイパッドの更新処理
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Update();

	}
	//レンダリングの更新処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}
	//フェードの更新処理
	if (m_pFade != NULL)
	{
		m_pFade->Update();
	}
	//カメラの更新処理
	if (m_pCamera != NULL)
	{
		m_pCamera->Update();
	}

	//ライトの更新処理
	if (m_pLight != NULL)
	{
		m_pLight->Update();
	}

	switch (m_mode)
	{
	case MODE_TITLE:				//タイトルの場合
		if (m_pTitle != NULL)
		{
			//タイトルの更新処理
			m_pTitle->Update();
		}
		break;
	case MODE_TUTORIAL:				//チュートリアルの場合
		if (m_pTutorial != NULL)
		{
			//チュートリアルの更新処理
			m_pTutorial->Update();
		}
		break;
	case MODE_GAME:					//ゲームの場合
		if (m_pGame != NULL)
		{
			//ゲームの更新処理
			m_pGame->Update();
		}
		break;
	case MODE_RESULT:				//リザルトの場合
		if (m_pResult != NULL)
		{
			//リザルトの更新処理
			m_pResult->Update();
		}
		break;
	case MODE_RANKING:				//ランキングの場合
		if (m_pRanking != NULL)
		{
			//ランキングの更新処理
			m_pRanking->Update();
		}
		break;
	}

}

//=============================================================================
// マネージャの描画処理
//=============================================================================
void CManager::Draw(void)
{
	//レンダリングの描画処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}

	//カメラの設置処理
	if (m_pCamera != NULL)
	{
		m_pCamera->SetCamera();
	}
}

//=============================================================================
// レンダリングの取得
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;	//値を返す
}

//=============================================================================
// キーボードの取得
//=============================================================================
CInputKeyboard *CManager::GetKeyboard(void)
{
	return m_pInputKeyboard;	//値を返す
}

//=============================================================================
// ジョイパッドの取得
//=============================================================================
CInputJoypad *CManager::GetJoypad(void)
{
	return m_pInputJoypad;	//値を返す
}

//=============================================================================
// サウンドの取得
//=============================================================================
CSound *CManager::GetSound(void)
{
	return m_pSound;				//値を返す
}

//=============================================================================
// カメラの取得
//=============================================================================
CCamera *CManager::GetCamera(void)
{
	return m_pCamera;				//値を返す
}

//=============================================================================
// ライトの取得
//=============================================================================
CLight *CManager::GetLight(void)
{
	return m_pLight;				//値を返す
}

//=============================================================================
// モードの取得
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;				//値を返す
}
//=============================================================================
// モードの取得
//=============================================================================
CFade *CManager::GetFade(void)
{
	return m_pFade;				//値を返す
}

//=============================================================================
// モードの設置
//=============================================================================
void CManager::SetMode(MODE mode)
{
	switch (m_mode)
	{
	case MODE_TITLE:				//タイトルの場合
		if (m_pTitle != NULL)
		{
			m_pSound->StopSound(CSound::SOUND_LABEL_TITLE);
			//タイトルの終了処理
			m_pTitle->Uninit();
			//メモリの開放
			delete m_pTitle;
			m_pTitle = NULL;
		}
		break;
	case MODE_TUTORIAL:				//チュートリアルの場合
		if (m_pTutorial != NULL)
		{
			m_pSound->StopSound(CSound::SOUND_LABEL_TUTORIAL);

			//チュートリアルの終了処理
			m_pTutorial->Uninit();
			//メモリの開放
			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		break;
	case MODE_GAME:					//ゲームの場合
		if (m_pGame != NULL)
		{
			m_pSound->StopSound(CSound::SOUND_LABEL_GAME);

			m_nTimeData = CTimer::GetTimer();
			//ゲームの終了処理
			m_pGame->Uninit();
			//メモリの開放
			delete m_pGame;
			m_pGame = NULL;
		}
		break;
	case MODE_RESULT:				//リザルトの場合
		if (m_pResult != NULL)
		{
			m_pSound->StopSound(CSound::SOUND_LABEL_RESULT);

			//リザルトの終了処理
			m_pResult->Uninit();
			//メモリの開放
			delete m_pResult;
			m_pResult = NULL;
		}
		break;
	case MODE_RANKING:				//ランキングの場合
		if (m_pRanking != NULL)
		{
			m_pSound->StopSound(CSound::SOUND_LABEL_RANKING);

			//ランキングの終了処理
			m_pRanking->Uninit();
			//メモリの開放
			delete m_pRanking;
			m_pRanking = NULL;
		}
		break;
	}

	m_mode = mode;	//代入

	switch (mode)
	{
	case MODE_TITLE:				//タイトルの場合
		if (m_pTitle == NULL)
		{
			m_pSound->PlaySound(CSound::SOUND_LABEL_TITLE);

			m_pCamera->Init();
			//タイトルの生成
			m_pTitle = CTitle::Create();
		}
		break;
	case MODE_TUTORIAL:				//チュートリアルの場合
		if (m_pTutorial == NULL)
		{
			m_pSound->PlaySound(CSound::SOUND_LABEL_TUTORIAL);

			m_pCamera->Init();
			//チュートリアルの生成
			m_pTutorial = CTutorial::Create();
		}
		break;
	case MODE_GAME:					//ゲームの場合
		if (m_pGame == NULL)
		{
			m_pSound->PlaySound(CSound::SOUND_LABEL_GAME);

			m_pCamera->Init();
			//ゲームの生成
			m_pGame = CGame::Create();
		}
		break;
	case MODE_RESULT:				//リザルトの場合
		if (m_pResult == NULL)
		{
			m_pSound->PlaySound(CSound::SOUND_LABEL_RESULT);
			m_pCamera->Init();

			//リザルトの生成
			m_pResult = CResult::Create();
		}
		break;
	case MODE_RANKING:				//ランキングの場合
		if (m_pRanking == NULL)
		{
			m_pSound->PlaySound(CSound::SOUND_LABEL_RANKING);
			m_pCamera->Init();

			//ランキングの生成
			m_pRanking = CRanking::Create();
		}
		break;
	}
}

//=============================================================================
// ゲームスコアの比較設置処理
//=============================================================================
void CManager::SetGameScore(int score)
{
	int nData;	//データ格納
	int nUpdateScore = 0;
	if (m_aRankingScore[4] < score)
	{//一番下のスコアより大きい場合代入する
		m_aRankingScore[4] = score;
		nUpdateScore = score;
	}
	else
	{
		m_nUpdateSocre = 6;
	}

	//降順処理(バブルソート)
	for (int nCntData = 0; nCntData < 5; nCntData++)
	{
		for (int nCntNumber = nCntData + 1; nCntNumber < 5; nCntNumber++)
		{
			if (m_aRankingScore[nCntData] < m_aRankingScore[nCntNumber])
			{
				nData = m_aRankingScore[nCntNumber];
				m_aRankingScore[nCntNumber] = m_aRankingScore[nCntData];
				m_aRankingScore[nCntData] = nData;
			}
		}
		if (m_aRankingScore[nCntData] == nUpdateScore)
		{
			m_nUpdateSocre = nCntData;
		}
	}
}

//=============================================================================
// 数字の設置処理
//=============================================================================
int CManager::GetGameScore(int nIdx)
{
	return m_aRankingScore[nIdx];
}

//=============================================================================
// 数字の設置処理
//=============================================================================
int CManager::GetUpdateScore(void)
{
	return m_nUpdateSocre;
}

//=============================================================================
//	タイムの取得処理
//=============================================================================
int CManager::GetTime(void)
{
	return m_nTimeData;
}

//=============================================================================
//	スコアの取得処理
//=============================================================================
int CManager::GetScore(void)
{
	return m_nScore;
}

//=============================================================================
//	スコアの設置処理
//=============================================================================
void CManager::SetScore(int nScore)
{
	m_nScore = nScore;
}

//=============================================================================
//	鳥の加算処理
//=============================================================================
void CManager::SetBird(int nValue)
{
	m_nNumBird = nValue;
}

//=============================================================================
//	かえるの加算処理
//=============================================================================
void CManager::SetFrog(int nValue)
{
	m_nNumFrog = nValue;
}

//=============================================================================
//	鳥の加算処理
//=============================================================================
int CManager::GetBird(void)
{
	return m_nNumBird;
}

//=============================================================================
//	かえるの加算処理
//=============================================================================
int CManager::GetFrog(void)
{
	return m_nNumFrog;
}
