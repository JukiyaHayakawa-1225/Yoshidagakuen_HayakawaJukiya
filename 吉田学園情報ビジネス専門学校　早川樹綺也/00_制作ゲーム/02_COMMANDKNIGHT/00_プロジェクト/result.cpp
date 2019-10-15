//=============================================================================
//
// リザルト処理 [manager.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "result.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "debugproc.h"
#include "player.h"
#include "fade.h"
#include "score.h"
#include "number.h"
#include "timer.h"
#include "map.h"
#include "meshfield.h"
#include "logo.h"
#include "enemynumber.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CResult::RESULT CResult::m_result = RESULT_NONE;		//リザルトのポインタ
CScore			*CResult::m_pScore = NULL;				//スコアのポインタ情報
CTimer			*CResult::m_pTimer = NULL;				//タイマーのポインタ情報
CMap			*CResult::m_pMap = NULL;				//マップのポインタ情報
CBirdNumber	*CResult::m_pBirdNumber = NULL;				//鳥数字ポインタ情報
CFrogNumber	*CResult::m_pFrogNumber = NULL;				//かえる数字ポインタ情報
CResult::COUNT_TYPE CResult::m_type = COUNT_TIMER;		//

//=============================================================================
// リザルトのコンストラクタ
//=============================================================================
CResult::CResult()
{
	m_nTimer = 0;			//タイマーの初期値
	m_pScore = NULL;		//スコアを初期化
	m_pBirdNumber = NULL;	//鳥数字の初期化
	m_pFrogNumber = NULL;	//かえる数字の初期化
	m_type = COUNT_TIMER;
}

//=============================================================================
// リザルトのデストラクタ
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// リザルトのデストラクタ
//=============================================================================
CResult *CResult::Create()
{
	CResult *pResult = NULL;	//リザルトのポインタ
	if (pResult == NULL)
	{
		pResult = new CResult;	//動的確保
		if (pResult != NULL)
		{
			pResult->Init();	//初期化処理
		}
	}
	return pResult;
}

//=============================================================================
// リザルトの初期化処理
//=============================================================================
HRESULT CResult::Init(void)
{
	//リザルトロゴの読み込み
	CResultLogo::Load();
	//リザルトスコアロゴの読み込み
	CResultScoreLogo::Load();
	//数字の読み込み
	CNumber::Load();

	//リザルトロゴの生成
	CResultLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 75.0f, 0.0f), 250.0f, 75.0f);
	//リザルトスコアロゴの読み込み
	CResultScoreLogo::Create(D3DXVECTOR3(300.0f, 325.0f, 0.0f), 200.0f, 200.0f);

	//マップを生成
	m_pMap = CMap::Create();
	//メッシュフィールドの生成
	CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_nTimer = 0;	//タイマーの初期化処理
	//スコアの生成
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200.0f,600.0f, 0.0f), 40.0f, 40.0f,5);
	//スコアを取得し加算
	m_pScore->AddScore(CManager::GetScore());
	//タイマーの生成
	m_pTimer = CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 100.0f, 200.0f, 0.0f), 40.0f, 40.0f);

	m_pBirdNumber = CBirdNumber::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 100.0f, 330.0f, 0.0f), 40.0f, 40.0f);
	m_pFrogNumber = CFrogNumber::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 100.0f, 450.0f, 0.0f), 40.0f, 40.0f);
	return S_OK;
}

//=============================================================================
// リザルトの終了処理
//=============================================================================
void CResult::Uninit(void)
{
	//数字の破棄
	CNumber::Unload();
	//リザルトスコアロゴの破棄
	CResultScoreLogo::Unload();
	//リザルトロゴの破棄
	CResultLogo::Unload();

	//マップの終了処理
	if (m_pMap != NULL)
	{
		m_pMap->Uninit();
		delete m_pMap;
		m_pMap = NULL;
	}

	//全てのオブジェクト開放
	CScene::ReleaseAll();
}

//=============================================================================
// リザルトの更新処理
//=============================================================================
void CResult::Update(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();
	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetJoypad();
	//フェードの取得処理
	CFade *pFade = CManager::GetFade();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_B) == true)
	{
		if (m_type == COUNT_END && pFade->GetFade() == CFade::FADE_NONE)
		{//Enterキーを押下した場合
			//ランキングへ遷移
			pFade->SetFade(CManager::MODE_RANKING, pFade->FADE_OUT);
			CManager *pManager = NULL;
			pManager->SetGameScore(m_pScore->GetScore());
		}
	}

	m_nTimer++;	//タイマーの加算
	if (m_nTimer >= 1500 && pFade->GetFade() == CFade::FADE_NONE)
	{//タイマーが１５００以上の場合
		//スコアの設置処理
		CManager *pManager = NULL;
		pManager->SetGameScore(m_pScore->GetScore());
		//ランキングに画面遷移
		pFade->SetFade(CManager::MODE_RANKING, pFade->FADE_OUT);


	}
	//デバッグ表示
	CDebugProc::Print(1, "Result");
}

//=============================================================================
// リザルトの描画処理
//=============================================================================
void CResult::Draw(void)
{
}

//=============================================================================
// リザルトの設置処理
//=============================================================================
void CResult::SetResult(RESULT result)
{
	m_result = result;
}
//=============================================================================
// リザルトの取得処理
//=============================================================================
CResult::RESULT CResult::GetResult(void)
{
	return m_result;
}

//=============================================================================
// 状態の設置
//=============================================================================
void CResult::SetType(COUNT_TYPE type)
{
	m_type = type;
}

//=============================================================================
// 状態の取得
//=============================================================================
CResult::COUNT_TYPE CResult::GetType(void)
{
	return m_type;
}
//=============================================================================
// コマンドの取得
//=============================================================================
CScore * CResult::GetScore(void)
{
	return m_pScore;
}

//=============================================================================
// コマンドの取得
//=============================================================================
CBirdNumber * CResult::GetBirdNumber(void)
{
	return m_pBirdNumber;
}

//=============================================================================
// コマンドの取得
//=============================================================================
CFrogNumber * CResult::GetFrogNumber(void)
{
	return m_pFrogNumber;
}