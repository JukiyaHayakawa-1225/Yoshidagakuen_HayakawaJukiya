//=============================================================================
//
// タイトルの処理 [title.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "title.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "debugproc.h"
#include "meshfield.h"
#include "fade.h"
#include "map.h"
#include "logo.h"
#include "pressenter.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CMap			*CTitle::m_pMap = NULL;					//マップのポインタ情報

//=============================================================================
// タイトルのコンストラクタ
//=============================================================================
CTitle::CTitle()
{
	m_pMap = NULL;			//マップを初期化
}

//=============================================================================
// タイトルのデストラクタ
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// タイトルのデストラクタ
//=============================================================================
CTitle *CTitle::Create()
{
	CTitle *pTitle = NULL;	//タイトルのポインタ

	if (pTitle == NULL)
	{
		pTitle = new CTitle;	//動的確保
		if (pTitle != NULL)
		{
			pTitle->Init();		//初期化処理
		}
	}
	return pTitle;
}

//=============================================================================
// タイトルの初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
{
	//タイトルロゴの読み込み
	CTitleLogo::Load();

	//プレスエンターの読み込み
	CPressEnter::Load();

	//マップを生成
	m_pMap = CMap::Create();

	//プレスエンターの生成
	CPressEnter::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, 0.0f), 300, 75);

	//メッシュフィールドの生成
	CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//タイトルロゴの生成
	CTitleLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 175, 0.0f), 500, 175);

	m_ntimer = 0;
	return S_OK;
}

//=============================================================================
// タイトルの終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	//プレスエンターの破棄
	CPressEnter::Unload();

	//タイトルロゴの破棄
	CTitleLogo::Unload();

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
// タイトルの更新処理
//=============================================================================
void CTitle::Update(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();
	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetJoypad();

	//フェードの取得
	CFade *pFade = CManager::GetFade();

	m_ntimer++;	//タイマーの加算

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
		if (pFade->GetFade() == CFade::FADE_NONE)
		{
			//チュートリアルへ遷移
			pFade->SetFade(CManager::MODE_TUTORIAL, pFade->FADE_OUT);
		}
	}
	if (m_ntimer >= 800)
	{//タイマーが８００以上の場合
		if (pFade->GetFade() == CFade::FADE_NONE)
		{
			pFade->SetFade(CManager::MODE_RANKING, pFade->FADE_OUT);	//ランキングへ画面遷移
		}
	}
	//デバッグ表示
	CDebugProc::Print(1, "Title");
}

//=============================================================================
// タイトルの描画処理
//=============================================================================
void CTitle::Draw(void)
{

}