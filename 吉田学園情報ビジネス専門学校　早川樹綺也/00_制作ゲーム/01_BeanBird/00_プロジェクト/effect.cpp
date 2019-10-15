//=============================================================================
//
// エフェクトの処理 [effect.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "effect.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "explotion.h"
#include "enemy.h"
#include "beans.h"
#include "score.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// エフェクトのコンストラクタ
//=============================================================================
CEffect::CEffect() : CScene2D(1)
{
	m_nLife = 0;								//体力の初期値
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量の初期値
}
//=============================================================================
// エフェクトのデストラクタ
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
//エフェクトの生成
//=============================================================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWight, float fHeight)
{
	CEffect *pEffect = NULL;

	if (pEffect == NULL)
	{
		pEffect = new CEffect;				//シーンの動的確保
		pEffect->SetPos(pos);				//位置の代入
		pEffect->SetWidth(fWight);			//幅の代入
		pEffect->SetHeight(fHeight);		//高さの代入
		pEffect->Init();					//初期化処理
		pEffect->SetCol(col);				//色の代入
		pEffect->BindTexture(m_pTexture);	//テクスチャ
	}
	return pEffect;						//値を返す
}

//=============================================================================
// エフェクトのテクスチャ読み込み
//=============================================================================
HRESULT CEffect::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME012,
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// エフェクトのテクスチャ破棄
//=============================================================================
void CEffect::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
// エフェクトの初期化処理
//=============================================================================
HRESULT CEffect::Init(void)
{
	//CScene2Dの初期化処理
	CScene2D::Init();
	return S_OK;
}

//=============================================================================
// エフェクトの終了処理
//=============================================================================
void CEffect::Uninit(void)
{
	//CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// エフェクトの更新処理
//=============================================================================
void CEffect::Update(void)
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;		//色
	float fWidth;		//幅
	float fHeight;		//高さ

	//位置の取得
	pos = GetPos();
	//色の取得
	col = GetCol();
	//幅の取得
	fWidth = GetWidth();
	//高さの取得
	fHeight = GetHeight();

	//減算処理
	fWidth -= 2.5f;		//幅を減らす
	fHeight -= 2.5f;	//高さを減らす
	col.r -= 0.01f;		//色を減らす
	col.g -= 0.01f;		//色を減らす
	col.b -= 0.01f;		//色を減らす
	col.a -= 0.01f;		//透明度を減らす

	//位置の設置
	SetPos(pos);
	//幅の設置
	SetWidth(fWidth);
	//高さの設置
	SetHeight(fHeight);
	//位置の設置
	SetVtxPos(pos);
	//色の設置
	SetCol(col);

	if (fWidth <= 0.0f || fHeight <= 0.0f)
	{//幅または高さが0以下になった場合
		//終了処理
		Uninit();
	}

	//CScene2Dの更新処理
	CScene2D::Update();
}

//=============================================================================
// エフェクトの描画処理
//=============================================================================
void CEffect::Draw(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//CScene2Dの描画処理
	CScene2D::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}