//=============================================================================
//
// パーティクルの処理 [particle.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "particle.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "explotion.h"
#include "enemy.h"
#include "beans.h"
#include "score.h"
#include "effect.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CParticle::m_pTexture = NULL;

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// オブジェクトのコンストラクタ
//=============================================================================
CParticle::CParticle() : CScene2D(1)
{
	m_nLife = 0;								//体力の初期値
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量の初期値
}
//=============================================================================
// オブジェクトのデストラクタ
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
//オブジェクトの生成
//=============================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos,D3DXVECTOR3 move, D3DXCOLOR col,float fLength)
{
	CParticle *pParticle = NULL;

	if (pParticle == NULL)
	{
		pParticle = new CParticle;				//シーンの動的確保
		pParticle->SetPos(pos);					//位置の代入
		pParticle->SetLength(fLength);			//拡大の代入
		pParticle->m_move = move;
		pParticle->Init();						//初期化処理
		pParticle->SetCol(col);					//色の代入
		pParticle->BindTexture(m_pTexture);		//テクスチャ
	}
	return pParticle;							//値を返す
}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CParticle::Load(void)
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
// オブジェクトのテクスチャ破棄
//=============================================================================
void CParticle::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
// オブジェクトの初期化処理
//=============================================================================
HRESULT CParticle::Init(void)
{
	//CScene2Dの初期化処理
	CScene2D::Init();
	return S_OK;
}

//=============================================================================
// オブジェクトの終了処理
//=============================================================================
void CParticle::Uninit(void)
{
	//CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// オブジェクトの更新処理
//=============================================================================
void CParticle::Update(void)
{
	D3DXVECTOR3	pos;		//位置
	D3DXCOLOR	col;		//色
	float		length;		//拡大

	//位置の取得
	pos = GetPos();
	//色の取得
	col = GetCol();
	//拡大の取得
	length = GetfLength();

	//減算処理
	length--;				//拡大数を減らす
	col.r -= 0.0005f;		//色を減らす
	col.g -= 0.0005f;		//色を減らす
	col.b -= 0.0005f;		//色を減らす
	col.a -= 0.0005f;		//透明度を減らす

	pos += m_move;
	//位置の設置
	SetPos(pos);
	//拡大の設置
	SetLength(length);
	//位置の設置
	SetVtxLenghtPos(pos);
	//色の設置
	SetCol(col);

	if (length <= 0.0f)
	{//幅または高さが0以下になった場合
	 //終了処理
		Uninit();
	}

	//CScene2Dの更新処理
	CScene2D::Update();
}

//=============================================================================
// オブジェクトの描画処理
//=============================================================================
void CParticle::Draw(void)
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