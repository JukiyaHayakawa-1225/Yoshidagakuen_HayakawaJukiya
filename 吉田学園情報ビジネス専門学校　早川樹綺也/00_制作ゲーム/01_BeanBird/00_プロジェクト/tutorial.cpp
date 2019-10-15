//=============================================================================
//
// チュートリアルの処理 [tutorial.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "tutorial.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "bg.h"
#include "fade.h"
#include "sound.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================


//=============================================================================
// チュートリアルのコンストラクタ
//=============================================================================
CTutorial::CTutorial()
{

}

//=============================================================================
// チュートリアルのデストラクタ
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// チュートリアルの生成
//=============================================================================
CTutorial *CTutorial::Create()
{
	CTutorial *pTutorial = NULL;
	if (pTutorial == NULL)
	{
		pTutorial = new CTutorial;
		if (pTutorial != NULL)
		{
			pTutorial->Init();
		}
	}
	return pTutorial;
}

//=============================================================================
// チュートリアルの初期化処理
//=============================================================================
HRESULT CTutorial::Init(void)
{
	m_counter = 0;
	//背景の読み込み
	CBgTutorial::Load();

	//背景の生成
	CBgTutorial::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,CBgTutorial::BG_0);
	//CBgTutorial::Create(D3DXVECTOR3(SCREEN_WIDTH + 640, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, CBgTutorial::BG_1);

	return S_OK;
}

//=============================================================================
// チュートリアルの終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	//背景の破棄
	CBgTutorial::Unload();

	CScene::ReleaseAll();
}

//=============================================================================
// チュートリアルの更新処理
//=============================================================================
void CTutorial::Update(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetJoypad();

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	CBgTutorial *pTutorial = NULL;

	if (pTutorial->GetTutorialState() == CBgTutorial::STATE_NOMAL &&
		m_counter == 0)
	{
		if (pInputKeyboard->GetTrigger(DIK_D) == true||
			pInputKeyboard->GetTrigger(DIK_RIGHT) == true||
			pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_LSTICK_RIGHT) == true ||
			pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_RIGHT) == true)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_SLIDE);
			m_counter++;
			CBgTutorial::SetTutorialState(CBgTutorial::STATE_MOVE);
		}
	}
	else if (pTutorial->GetTutorialState() == CBgTutorial::STATE_NOMAL &&
		m_counter == 1)
	{
		if (pInputKeyboard->GetTrigger(DIK_A) == true ||
			pInputKeyboard->GetTrigger(DIK_LEFT) == true ||
			pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_LSTICK_LEFT) == true ||
			pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_LEFT) == true)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_SLIDE);

			m_counter--;
			CBgTutorial::SetTutorialState(CBgTutorial::STATE_REMOVE);
		}
	}
	if (m_counter == 1)
	{
		if (pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_START) == true ||
			pInputKeyboard->GetTrigger(DIK_RETURN) == true)
		{// JoyPadのボタンまたはENTERキーが押された場合]
			pSound->PlaySound(CSound::SOUND_LABEL_SE_KETTEI);

			CFade::Create(CManager::MODE_GAME, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
		}
	}
}

//=============================================================================
// チュートリアルの描画処理
//=============================================================================
void CTutorial::Draw(void)
{
}