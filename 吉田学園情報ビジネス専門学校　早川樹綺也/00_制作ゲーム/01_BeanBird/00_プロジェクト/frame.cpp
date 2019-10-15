//=============================================================================
//
// フレームの処理 [frame.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "frame.h"
#include "game.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "player.h"
#include "barrier.h"
#include "avatar.h"
#include "skill.h"
#include "score.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CFrame::m_apTexture[MAX_FRAME] = {};		//テクスチャのポインタ

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// フレームのコンストラクタ
//=============================================================================
CFrame::CFrame() : CScene2D(6)
{

}
//=============================================================================
// フレームのデストラクタ
//=============================================================================
CFrame::~CFrame()
{

}

//=============================================================================
//フレームの生成
//=============================================================================
CFrame *CFrame::Create(D3DXVECTOR3 pos, float fWight, float fHeight,CFrame::FRAME_TYPE frame)
{
	CFrame *pFrame = NULL;									//フレームのポインタ
	if (pFrame == NULL)
	{//NULLの場合
		pFrame = new CFrame;								//フレームの動的確保
		pFrame->SetPos(pos);								//位置の代入
		pFrame->SetWidth(fWight);							//幅の代入
		pFrame->SetHeight(fHeight);							//高さの代入
		pFrame->Init();										//初期化処理
		pFrame->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		pFrame->BindTexture(m_apTexture[frame]);				//テクスチャ
	}
	return pFrame;											//値を返す
}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CFrame::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ScoreUI.png",
		&m_apTexture[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\StockUI.png",
		&m_apTexture[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LifeUI.png",
		&m_apTexture[2]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BeansUI.png",
		&m_apTexture[3]);

	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CFrame::Unload(void)
{
	for (int nCntEnemyTex = 0; nCntEnemyTex < MAX_FRAME; nCntEnemyTex++)
	{//テクスチャの数だけ繰り返し
	 // テクスチャの破棄
		if (m_apTexture[nCntEnemyTex] != NULL)
		{
			m_apTexture[nCntEnemyTex]->Release();
			m_apTexture[nCntEnemyTex] = NULL;
		}
	}
}

//=============================================================================
// フレームの初期化処理
//=============================================================================
HRESULT CFrame::Init(void)
{
	//CScene2Dの初期化処理
	CScene2D::Init();
	return S_OK;
}

//=============================================================================
// フレームの終了処理
//=============================================================================
void CFrame::Uninit(void)
{
	//CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// フレームの更新処理
//=============================================================================
void CFrame::Update(void)
{
	//CScene2Dの更新処理
	CScene2D::Update();
}

//=============================================================================
// フレームの描画処理
//=============================================================================
void CFrame::Draw(void)
{
	//CScene2Dの描画処理
	CScene2D::Draw();
}