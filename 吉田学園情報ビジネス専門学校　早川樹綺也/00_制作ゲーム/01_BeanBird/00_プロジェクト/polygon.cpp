//=============================================================================
//
// ポリゴンの処理 [polygon.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "polygon.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "score.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================


//=============================================================================
// マクロ定義
//=============================================================================
#define TEXTURE_SIZE        (30)                            // テクスチャのサイズ

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// ポリゴンのコンストラクタ
//=============================================================================
CPolygon::CPolygon()
{
	m_pTexture = NULL;

}
//=============================================================================
// ポリゴンのデストラクタ
//=============================================================================
CPolygon::~CPolygon()
{

}

//=============================================================================
//ポリゴンの生成
//=============================================================================
CPolygon *CPolygon::Create(D3DXVECTOR3 pos, float fWight, float fHeight)
{
	CPolygon *pPolygon = NULL;
	if (pPolygon == NULL)
	{
		pPolygon = new CPolygon;			//シーンの動的確保
		pPolygon->m_pos = pos;				//位置の代入
		pPolygon->m_fWight = fWight;		//幅の代入
		pPolygon->m_fHeight = fHeight;		//高さの代入
		pPolygon->Init();					//初期化処理
	}
	return pPolygon;						//値を返す
}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT CPolygon::Init(void)
{
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
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWight, m_pos.y - m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWight, m_pos.y - m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWight, m_pos.y + m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWight, m_pos.y + m_fHeight, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CPolygon::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//// テクスチャの破棄
	//if (m_apTexture != NULL)
	//{
	//	m_apTexture->Release();
	//	m_apTexture = NULL;
	//}

	//オブジェクトの破棄
	//Release();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CPolygon::Update(void)
{

}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CPolygon::Draw(void)
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
// オブジェクトの取得処理
//=============================================================================
void CPolygon::BindTexture(LPDIRECT3DTEXTURE9 texture)
{
	m_pTexture = texture;
}
