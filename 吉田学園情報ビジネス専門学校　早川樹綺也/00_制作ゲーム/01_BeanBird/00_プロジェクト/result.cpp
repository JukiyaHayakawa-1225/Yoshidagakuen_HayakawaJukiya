//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "result.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "fade.h"
#include "bg.h"
#include "logo.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CResult::RESULT CResult::m_result = RESULT_NONE;

//=============================================================================
// リザルトのコンストラクタ
//=============================================================================
CResult::CResult()
{
	m_nTimer = 0;
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
	CResult *pResult = NULL;
	if (pResult == NULL)
	{
		pResult = new CResult;
		if (pResult != NULL)
		{
			pResult->Init();
		}
	}
	return pResult;
}

//=============================================================================
// リザルトの初期化処理
//=============================================================================
HRESULT CResult::Init(void)
{
	m_nTimer = 0;

	//背景の読み込み
	CBgResult::Load();

	//リザルトロゴの読み込み
	CResultLogo::Load();

	//背景の生成
	CBgResult::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 ,m_result);

	//リザルトロゴの読み込み
	CResultLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 200, 0.0f), 350, 150,m_result);
	return S_OK;
}

//=============================================================================
// リザルトの終了処理
//=============================================================================
void CResult::Uninit(void)
{
	//背景の破棄
	CBgResult::Unload();

	//リザルトロゴの破棄
	CResultLogo::Unload();

	CScene::ReleaseAll();
}

//=============================================================================
// リザルトの更新処理
//=============================================================================
void CResult::Update(void)
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
		CFade::Create(CManager::MODE_RANKING, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	if (m_nTimer % 600 == 0)
	{
		CFade::Create(CManager::MODE_RANKING, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
	}
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