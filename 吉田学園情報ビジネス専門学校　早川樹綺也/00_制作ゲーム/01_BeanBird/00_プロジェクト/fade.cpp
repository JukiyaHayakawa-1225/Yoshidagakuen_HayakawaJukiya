//=============================================================================
//
// フェードの処理 [bg.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "fade.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "scene2D.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CFade::FADE CFade::m_fade = CFade::FADE_NONE;

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// フェードのコンストラクタ
//=============================================================================
CFade::CFade() : CScene(7)
{
	m_pTexture = NULL;							// テクスチャへのポインタ
	m_pVtxBuff = NULL;							// 頂点バッファへのポインタ
	m_modeNext = CManager::MODE_TITLE;			// タイトル
	m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}
//=============================================================================
// フェードのデストラクタ
//=============================================================================
CFade::~CFade()
{

}

//=============================================================================
//フェードの生成
//=============================================================================
CFade *CFade::Create(CManager::MODE modeNext, D3DXVECTOR3 pos, float fWight, float fHeight)
{
	CFade *pFade = NULL;

	if (pFade == NULL)
	{
		if (m_fade == FADE_NONE)
		{
			pFade = new CFade;				//シーンの動的確保
			if (pFade != NULL)
			{
				pFade->m_pos = pos;				//位置の代入
				pFade->m_fWidth = fWight;		//幅の代入
				pFade->m_fHeight = fHeight;		//高さの代入
				pFade->m_modeNext = modeNext;	//モードの代入
				pFade->Init();					//初期化処理
			}
		}
	}
	return pFade;				//値を返す
}

//=============================================================================
// フェードの初期化処理
//=============================================================================
HRESULT CFade::Init(void)
{
	CScene::SetObjType(OBJTYPE_FADE);
	m_fade = FADE_OUT;											//フェードイン状態にする
	m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);			//色の初期化

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
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = m_colorFade;
	pVtx[1].col = m_colorFade;
	pVtx[2].col = m_colorFade;
	pVtx[3].col = m_colorFade;

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
// フェードの終了処理
//=============================================================================
void CFade::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	Release();
}

//=============================================================================
// フェードの更新処理
//=============================================================================
void CFade::Update(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ
						// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報(頂点カラー)の更新
	pVtx[0].col = m_colorFade;
	pVtx[1].col = m_colorFade;
	pVtx[2].col = m_colorFade;
	pVtx[3].col = m_colorFade;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
	if (m_fade != FADE_NONE)
	{//フェードしている場合
		if (m_fade == FADE_IN)
		{
			m_colorFade.a -= 0.015f;	//画面を透明にしていく

			if (m_colorFade.a <= 0.0f)
			{
				m_colorFade.a = 0.0f;
				m_fade = FADE_NONE;		//何もしていない状態にする
				Uninit();
			}
		}
		else if (m_fade == FADE_OUT)
		{
			m_colorFade.a += 0.015f;		//画面を不透明にする
			if (m_colorFade.a >= 1.0f)
			{//不透明になった場合
				m_colorFade.a = 1.0f;
				m_fade = FADE_IN;		//フェードイン状態にする

										//モードの設定
				CManager::SetMode(m_modeNext);
			}
		}
	}
}
//=============================================================================
// フェードの描画処理
//=============================================================================
void CFade::Draw(void)
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
// フェードの取得処理
//=============================================================================
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}