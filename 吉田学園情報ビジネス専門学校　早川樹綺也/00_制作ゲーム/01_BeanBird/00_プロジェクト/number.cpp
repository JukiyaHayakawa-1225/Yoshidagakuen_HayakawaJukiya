//=============================================================================
//
// 数字の処理 [number.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "number.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "score.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXTURE_SIZE        (30)                            // テクスチャのサイズ

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// 数字のコンストラクタ
//=============================================================================
CNumber::CNumber()
{
}
//=============================================================================
// 数字のデストラクタ
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
//数字の生成
//=============================================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, float fWight, float fHeight)
{
	CNumber *pNumber = NULL;
	if (pNumber == NULL)
	{
		pNumber = new CNumber;							//シーンの動的確保
		pNumber->Init(pos, fWight, fHeight);			//初期化処理 位置の代入
	}
	return pNumber;					//値を返す
}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CNumber::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME008,
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CNumber::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 数字の初期化処理
//=============================================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, float fWight, float fHeight)
{
	m_pos = pos;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//ポインタの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)& pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - fWight, m_pos.y - fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + fWight, m_pos.y - fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - fWight, m_pos.y + fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + fWight, m_pos.y + fHeight, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 数字の終了処理
//=============================================================================
void CNumber::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// 数字の更新処理
//=============================================================================
void CNumber::Update(void)
{

}

//=============================================================================
// 数字の描画処理
//=============================================================================
void CNumber::Draw(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// 数字の設置処理
//=============================================================================
void CNumber::SetNumber(int nNumber)
{
	// 頂点バッファを生成
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * nNumber), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * nNumber), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * nNumber), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * nNumber), 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 数字の設置処理
//=============================================================================
void CNumber::SetCol(D3DXCOLOR col)
{
	m_col = col;

	// 頂点バッファを生成
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}