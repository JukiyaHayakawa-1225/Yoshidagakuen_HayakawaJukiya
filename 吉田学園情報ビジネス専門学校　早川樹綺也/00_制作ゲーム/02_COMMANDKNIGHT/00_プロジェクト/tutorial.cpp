//=============================================================================
//
// チュートリアルの処理 [tutorial.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "tutorial.h"
#include "scene.h"
#include "scene3D.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "debugproc.h"
#include "fade.h"
#include "meshfield.h"
#include "logo.h"
#include "pressenter.h"

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
	CTutorial *pTutorial = NULL;	//チュートリアルのポインタ
	if (pTutorial == NULL)
	{
		pTutorial = new CTutorial;	//動的確保
		if (pTutorial != NULL)
		{
			pTutorial->Init();		//初期化処理
		}
	}
	return pTutorial;
}

//=============================================================================
// チュートリアルの初期化処理
//=============================================================================
HRESULT CTutorial::Init(void)
{
	//チュートリアルロゴの読み込み
	CTutorialLogo::Load();

	//プレスエンターの読み込み
	//CPressEnter::Load();

	//プレスエンターの生成
	//CPressEnter::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, 0.0f), 300, 75);

	//チュートリアルロゴの生成
	CTutorialLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	//メッシュフィールドの生成
	CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_ntimer = 0;
	return S_OK;
}

//=============================================================================
// チュートリアルの終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	//プレスエンターの破棄
	//CPressEnter::Unload();

	//チュートリアルの破棄
	CTutorialLogo::Unload();
	//全てのオブジェクト開放
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

	//フェードの取得処理
	CFade *pFade = CManager::GetFade();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true||
		pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_B) == true)
	{//Enterキーを押下
		if (pFade->GetFade() == CFade::FADE_NONE)
		{

			//ゲームへ遷移
			pFade->SetFade(CManager::MODE_GAME, pFade->FADE_OUT);
		}
	}
	/*m_ntimer++;
	if (m_ntimer >= 5)
	{
		pFade->SetFade(CManager::MODE_GAME, pFade->FADE_OUT);

	}*/
	//デバッグ表示
	CDebugProc::Print(1, "Tutorial");
}

//=============================================================================
// チュートリアルの描画処理
//=============================================================================
void CTutorial::Draw(void)
{
}