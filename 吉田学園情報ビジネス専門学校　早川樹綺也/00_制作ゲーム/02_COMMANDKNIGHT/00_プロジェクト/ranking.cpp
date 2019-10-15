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
#include "debugproc.h"
#include "fade.h"
#include "number.h"
#include "rankingscore.h"
#include "map.h"
#include "meshfield.h"
#include "logo.h"
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CMap			*CRanking::m_pMap = NULL;					//マップのポインタ情報

//=============================================================================
// ランキングのコンストラクタ
//=============================================================================
CRanking::CRanking()
{
	m_nTimer = 0;	//タイマーの初期化
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
	CRanking *pRanking = NULL;	//ランキングのポインタ
	if (pRanking == NULL)
	{
		pRanking = new CRanking;	//動的確保
		if (pRanking != NULL)
		{
			pRanking->Init();		//初期化処理
		}
	}
	return pRanking;
}

//=============================================================================
// ランキングの初期化処理
//=============================================================================
HRESULT CRanking::Init(void)
{
	//数字の読み込み
	CNumber::Load();

	//ランキングランクの読み子k実
	CRankingRank::Load();

	//ランキングロゴの読み込み
	CRankingLogo::Load();

	//ランキングランクの生成
	CRankingRank::Create(D3DXVECTOR3(300.0f, SCREEN_HEIGHT / 2 + 225.0f / 2, 0.0f), 125.0f, 250.0f);

	//ロゴの生成
	CRankingLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 100, 0.0f), 400.0f, 100);

	//スコアの生成
	CRankingScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 450, SCREEN_HEIGHT / 2 - 100, 0.0f), 50.0f, 50.0f);

	//メッシュフィールドの生成
	CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//マップを生成
	m_pMap = CMap::Create();

	return S_OK;
}

//=============================================================================
// ランキングの終了処理
//=============================================================================
void CRanking::Uninit(void)
{
	//ランキングロゴの破棄
	CRankingLogo::Unload();

	//ランキングランクの破棄
	CRankingRank::Unload();

	//数字の破棄
	CNumber::Unload();

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
// ランキングの更新処理
//=============================================================================
void CRanking::Update(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetJoypad();

	//フェードの取得処理
	CFade *pFade = CManager::GetFade();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true||
		pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_B) == true)
	{//Enterキーを押下した場合
		//タイトルへ遷移
		pFade->SetFade(CManager::MODE_TITLE, pFade->FADE_OUT);
	}

	m_nTimer++;
	if (m_nTimer >= 500)
	{
		pFade->SetFade(CManager::MODE_TITLE, pFade->FADE_OUT);

	}
	//デバッグ表示
	CDebugProc::Print(1, "Ranking");
}

//=============================================================================
// ランキングの描画処理
//=============================================================================
void CRanking::Draw(void)
{
}