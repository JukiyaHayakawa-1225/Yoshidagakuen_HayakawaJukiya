//=============================================================================
//
// プレスエンターの処理 [player.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "pressenter.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CPressEnter::m_pTexture = NULL;
CPressEnter::PRESSENTERSTATE CPressEnter::m_pressEnterState = PRESSENTERSTATE_NONE;

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================
//=============================================================================
// プレスエンターのコンストラクタ
//=============================================================================
CPressEnter::CPressEnter() : CScene2D(7)
{
	m_nCounter = 0;
}
//=============================================================================
// プレスエンターのデストラクタ
//=============================================================================
CPressEnter::~CPressEnter()
{

}

//=============================================================================
//プレスエンターの生成
//=============================================================================
CPressEnter *CPressEnter::Create(D3DXVECTOR3 pos, float fWight, float fHeight)
{
	CPressEnter *pPressEnter = NULL;
	if (pPressEnter == NULL)
	{
		pPressEnter = new CPressEnter;				//シーンの動的確保
		if (pPressEnter != NULL)
		{
			pPressEnter->SetPos(pos);				//位置の代入
			pPressEnter->SetWidth(fWight);			//幅の代入
			pPressEnter->SetHeight(fHeight);		//高さの代入
			pPressEnter->Init();					//初期化処理
			pPressEnter->BindTexture(m_pTexture);
		}
	}
	return pPressEnter;								//値を返す
}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CPressEnter::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME014,
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CPressEnter::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// プレスエンターの初期化処理
//=============================================================================
HRESULT CPressEnter::Init(void)
{
	//状態の初期化
	m_pressEnterState = PRESSENTERSTATE_NONE;

	//CScene2Dの初期化処理
	CScene2D::Init();
	return S_OK;
}

//=============================================================================
// プレスエンターの終了処理
//=============================================================================
void CPressEnter::Uninit(void)
{
	//CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// プレスエンターの更新処理
//=============================================================================
void CPressEnter::Update(void)
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

	//フェードの取得
	CFade::FADE fade;
	CFade *pFade = NULL;
	fade = pFade->GetFade();

	m_nCounter++;

	if (m_nCounter >= 100)
	{
		m_nCounter = 0;
	}

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
		if (m_pressEnterState == PRESSENTERSTATE_NONE&& fade == CFade::FADE_NONE)
		{//押されていない状態かつフェードしていない状態の場合
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
			m_pressEnterState = PRESSENTERSTATE_PUSH;  // 押されている状態にする
		}
	}

	//CScene2Dの更新処理
	CScene2D::Update();
}

//=============================================================================
// プレスエンターの描画処理
//=============================================================================
void CPressEnter::Draw(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;

	pVtxBuff = CScene2D::GetVtx();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	if (m_pressEnterState == PRESSENTERSTATE_NONE)
	{// 押されていない状態の場合
		if (m_nCounter / 50 == 0)
		{// カウンターが一定の値になったら
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}
	else if (m_pressEnterState == PRESSENTERSTATE_PUSH)
	{// 押されていない状態の場合
		if (m_nCounter % 5 == 0)
		{// カウンターが一定の値になったら
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}
	//CScene2Dの描画処理
	//CScene2D::Draw();
}

//=============================================================================
// プレスエンターの状態取得
//=============================================================================
CPressEnter::PRESSENTERSTATE CPressEnter::GetPressEnter(void)
{
	return m_pressEnterState;
}