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
#include "bg.h"
#include "fade.h"
#include "pressenter.h"
#include "logo.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================


//=============================================================================
// タイトルのコンストラクタ
//=============================================================================
CTitle::CTitle()
{
	m_nTimer = 0;
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
	CTitle *pTitle = NULL;
	if (pTitle == NULL)
	{
		pTitle = new CTitle;
		if (pTitle != NULL)
		{
			pTitle->Init();
		}
	}
	return pTitle;
}

//=============================================================================
// タイトルの初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
{
	//タイマー初期化
	m_nTimer = 0;

	//背景の読み込み
	CBgTitle::Load();

	//プレスエンターの読み込み
	CPressEnter::Load();

	//タイトルロゴの読み込み
	CTitleLogo::Load();

	//プレスエンターの生成
	CPressEnter::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, 0.0f), 300, 75);
	//背景の生成
	CBgTitle::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	//タイトルロゴの生成
	CTitleLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 175, 0.0f), 500, 175);

	return S_OK;
}

//=============================================================================
// タイトルの終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	//タイトルロゴの破棄
	CTitleLogo::Unload();

	//背景の破棄
	CBgTitle::Unload();

	//プレスエンターの破棄
	CPressEnter::Unload();

	CScene::ReleaseAll();
}

//=============================================================================
// タイトルの更新処理
//=============================================================================
void CTitle::Update(void)
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
		CFade::Create(CManager::MODE_TUTORIAL, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	if (m_nTimer % 800 == 0)
	{
		CFade::Create(CManager::MODE_RANKING, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
	}
}

//=============================================================================
// タイトルの描画処理
//=============================================================================
void CTitle::Draw(void)
{

}