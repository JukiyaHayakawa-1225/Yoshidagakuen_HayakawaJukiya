//=============================================================================
//
// ランキングの処理 [ranking.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "ranking.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "bg.h"
#include "fade.h"
#include "number.h"
#include "rankingscore.h"
#include "logo.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================


//=============================================================================
// ランキングのコンストラクタ
//=============================================================================
CRanking::CRanking()
{
	m_nTimer = 0;
}

//=============================================================================
// ランキングのデストラクタ
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
// ランキングのデストラクタ
//=============================================================================
CRanking *CRanking::Create()
{
	CRanking *pRanking = NULL;
	if (pRanking == NULL)
	{
		pRanking = new CRanking;
		if (pRanking != NULL)
		{
			pRanking->Init();
		}
	}
	return pRanking;
}

//=============================================================================
// ランキングの初期化処理
//=============================================================================
HRESULT CRanking::Init(void)
{
	m_nTimer = 0;

	//背景の読み込み
	CBgRanking::Load();
	//ランキングランクの読み子k実
	CRankingRank::Load();
	//数字の読み込み
	CNumber::Load();
	//ロゴの読み込み
	CRankingLogo::Load();

	//ランキングランクの生成
	CRankingRank::Create(D3DXVECTOR3(300.0f, SCREEN_HEIGHT / 2  + 225.0f / 2, 0.0f), 125, 270.0f);
	//スコアの生成
	CRankingScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 450, SCREEN_HEIGHT / 2- 100, 0.0f),50.0f, 50.0f);

	//背景の生成
	CBgRanking::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	//ロゴの生成
	CRankingLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 100, 0.0f), 400.0f, 100);


	return S_OK;
}

//=============================================================================
// ランキングの終了処理
//=============================================================================
void CRanking::Uninit(void)
{
	//ロゴの破棄
	CRankingLogo::Unload();
	//数字の破棄
	CNumber::Unload();
	//ランキングランクの破棄
	CRankingRank::Unload();
	//背景の破棄
	CBgRanking::Unload();

	CScene::ReleaseAll();
}

//=============================================================================
// ランキングの更新処理
//=============================================================================
void CRanking::Update(void)
{
	m_nTimer++;
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetJoypad();

	if (pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_X) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_A) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_B) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_Y) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_LB) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_RB) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_LT) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_RT) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_BACK) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_START) == true ||
		pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{// JoyPadのボタンまたはENTERキーが押された場合
		CFade::Create(CManager::MODE_TITLE, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	if (m_nTimer % 600 == 0)
	{
		CFade::Create(CManager::MODE_TITLE, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
	}
}

//=============================================================================
// ランキングの描画処理
//=============================================================================
void CRanking::Draw(void)
{
}