//=============================================================================
//
// 雲の処理 [cloud.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "cloud.h"
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
#include "beans.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CCloud::m_pTexture = NULL;		//テクスチャのポインタ
//=============================================================================
// 雲のコンストラクタ
//=============================================================================
CCloud::CCloud() : CScene2D(2)
{
	m_move = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//移動量の初期値
	m_state = CLOUD_BEANS;					//状態の初期
}
//=============================================================================
// 雲のデストラクタ
//=============================================================================
CCloud::~CCloud()
{

}

//=============================================================================
//雲の生成
//=============================================================================
CCloud *CCloud::Create(D3DXVECTOR3 pos, float fWight, float fHeight, CLOUD_STATE state)
{
	CCloud *pCloud = NULL;									//雲のポインタ
	if (pCloud == NULL)
	{//NULLの場合
		pCloud = new CCloud;								//雲の動的確保
		pCloud->SetPos(pos);								//位置の代入
		pCloud->SetWidth(fWight);							//幅の代入
		pCloud->SetHeight(fHeight);							//高さの代入
		pCloud->Init();										//初期化処理
		pCloud->BindTexture(m_pTexture);					//テクスチャ
		pCloud->m_move = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//移動量の初期化
		pCloud->m_state = state;							//状態の代入
	}
	return pCloud;											//値を返す
}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CCloud::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\cloud.png",
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CCloud::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
// 雲の初期化処理
//=============================================================================
HRESULT CCloud::Init(void)
{
	//CScene2Dの初期化処理
	CScene2D::Init();
	//種類の設定
	SetObjType(OBJTYPE_CLOUD);
	return S_OK;
}

//=============================================================================
// 雲の終了処理
//=============================================================================
void CCloud::Uninit(void)
{
	//CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 雲の更新処理
//=============================================================================
void CCloud::Update(void)
{
	D3DXVECTOR3 pos;	//位置
	pos = GetPos();		//位置の取得

	//移動量代入
	pos += m_move;

	//位置の設置
	SetVtxPos(pos);

	//CScene2Dの更新処理
	CScene2D::Update();

}

//=============================================================================
// 雲の描画処理
//=============================================================================
void CCloud::Draw(void)
{
	//CScene2Dの描画処理
	CScene2D::Draw();
}

//=============================================================================
// 雲の当たり判定処理
//=============================================================================
void CCloud::HitCloud(void)
{
	D3DXVECTOR3 pos;	//位置
	pos = GetPos();		//位置の取得

	if (m_state == CLOUD_BEANS)
	{
		//豆の生成
		CBeans::Create(D3DXVECTOR3(pos.x,pos.y - 20.0f,pos.z), BEANS_SIZE, BEANS_SIZE);
		//状態を変更
		m_state = CLOUD_NONE;
	}
	if (pos.y >= SCREEN_HEIGHT + 50)
	{//画面外にいった場合
	 //雲の終了処理
		CCloud::Uninit();
	}
}

//=============================================================================
// 雲の取得処理
//=============================================================================
CCloud::CLOUD_STATE CCloud::GetCloud(void)
{
	return m_state;
}