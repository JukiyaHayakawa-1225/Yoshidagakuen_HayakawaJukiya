//=============================================================================
//
// コマンドUIの処理 [commandUI.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "commandUI.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "scene2D.h"
#include "command.h"
#include "game.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CCommandUI::m_apTexture[MAX_COMMANDUI] = {};		//テクスチャのポインタ

//=============================================================================
// オブジェクトのコンストラクタ
//=============================================================================
CCommandUI::CCommandUI()
{
}
//=============================================================================
// オブジェクトのデストラクタ
//=============================================================================
CCommandUI::~CCommandUI()
{

}

//=============================================================================
// タイトル背景のテクスチャ読み込み
//=============================================================================
HRESULT CCommandUI::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Command\\command_main.png",
		&m_apTexture[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Command\\command_skill.png",
		&m_apTexture[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Command\\command_item.png",
		&m_apTexture[2]);

	return S_OK;
}

//=============================================================================
// タイトル背景のテクスチャ破棄
//=============================================================================
void CCommandUI::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < MAX_COMMANDUI; nCntTexture++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCntTexture] != NULL)
		{
			m_apTexture[nCntTexture]->Release();
			m_apTexture[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
//コマンドUIの生成
//=============================================================================
CCommandUI *CCommandUI::Create(void)
{
	CCommandUI *pCommandUI = NULL;					//プレイヤーのポインタ
	if (pCommandUI == NULL)
	{
		pCommandUI = new CCommandUI;				//シーンの動的確保
		pCommandUI->Init();							//初期化処理
		pCommandUI->BindTexture(m_apTexture[0]);

	}
	return pCommandUI;					//値を返す
}

//=============================================================================
// オブジェクトの初期化処理
//=============================================================================
HRESULT CCommandUI::Init(void)
{
	m_fHeight = 100.0f;
	SetPos(D3DXVECTOR3(75.0f, 620.0f, 0.0f));
	SetWidth(75.0f);
	SetHeight(m_fHeight);
	CScene2D::Init();
	return S_OK;
}

//=============================================================================
// オブジェクトの終了処理
//=============================================================================
void CCommandUI::Uninit(void)
{
	CScene2D::Uninit();

	Release();
}

//=============================================================================
// オブジェクトの更新処理
//=============================================================================
void CCommandUI::Update(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//コマンドの取得
	CCommand *pCommand;
	pCommand = CGame::GetCommad();

	//コマンド状態の取得
	CCommand::COMMAND_STATE state;
	state = pCommand->GetCommandState();

	//コマンド種類の取得
	CCommand::COMMAND_TYPE type;
	type = pCommand->GetCommandType();

	if (pCommand->GetUseSkill() == true)
	{
		m_fHeight = 0;				//高さの初期化
		SetHeight(m_fHeight);		//高さの設置処理
		SetVtxPos(D3DXVECTOR3(200.0f, 580.0f, 0.0f));
	}
	//種類別処理
	switch (type)
	{
	case CCommand::COMMAND_MAIN:	//メイン画面の場合
		SetWidth(75.0f);			//幅
		SetVtxPos(D3DXVECTOR3(150.0f, 580.0f, 0.0f));	//位置の設置処理
		BindTexture(m_apTexture[0]);					//テクスチャの貼り付け
		break;
	case CCommand::COMMAND_ITEM:	//アイテム画面の場合
		SetWidth(175.0f);								//幅
		SetVtxPos(D3DXVECTOR3(250.0f, 580.0f, 0.0f));	//位置の設置処理
		BindTexture(m_apTexture[2]);					//テクスチャの貼り付け
		break;
	}

	//状態別処理
	switch (state)
	{
	case CCommand::COMMAND_NONE:	//通常状態の場合

		break;
	case CCommand::COMMAND_PROCESS:	//実行状態の場合
		m_fHeight = 0;				//高さの初期化
		SetHeight(m_fHeight);		//高さの設置処理
		SetVtxPos(D3DXVECTOR3(200.0f, 580.0f, 0.0f));
		break;
	case CCommand::COMMAND_SKILL:	//実行状態の場合
		m_fHeight = 0;				//高さの初期化
		SetHeight(m_fHeight);		//高さの設置処理
		SetVtxPos(D3DXVECTOR3(200.0f, 580.0f, 0.0f));
		break;

	case CCommand::COMMAND_END:		//終了状態の場合
		if (pCommand->GetUseSkill() == false)
		{
			m_fHeight += 2.0f;			//高さの加算
			if (m_fHeight >= 100.0f)
			{//高さが１００以上の場合
				m_fHeight = 100.0f;		//高さの固定
				pCommand->SetCommandState(CCommand::COMMAND_NONE);	//通常状態にする
			}
			SetHeight(m_fHeight);							//高さの設置処理
			SetVtxPos(D3DXVECTOR3(150.0f, 580.0f, 0.0f));	//位置の設置処理
			BindTexture(m_apTexture[0]);					//テクスチャの貼り付け
		}
		break;
	}
}

//=============================================================================
// オブジェクトの描画処理
//=============================================================================
void CCommandUI::Draw(void)
{
	CScene2D::Draw();	//描画処理
}

//=============================================================================
// コマンド状態取得処理
//=============================================================================
CCommandUI::COMMAND CCommandUI::GetCommandState(void)
{
	return m_state;
}

//=============================================================================
// コマンドの高さ状態取得処理
//=============================================================================
CCommandUI::COMMAND_HEIGHT CCommandUI::GetCommandHeightState(void)
{
	return m_heightState;
}

//=============================================================================
// コマンド状態設置処理
//=============================================================================
void CCommandUI::SetCommandState(CCommandUI::COMMAND state)
{
	m_state = state;
}

//=============================================================================
// コマンドの高さ状態設置処理
//=============================================================================
void CCommandUI::SetCommandHeightState(COMMAND_HEIGHT heightState)
{
	m_heightState = heightState;
}
