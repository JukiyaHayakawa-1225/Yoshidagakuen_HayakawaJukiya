//=============================================================================
//
// スキルの処理 [skill.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "skill.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene2D.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CSkill::m_pTexture[MAX_SKILL] = {};
CSkill::SKILL_STATE CSkill::m_state[MAX_SKILL] = {};

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// スキルのコンストラクタ
//=============================================================================
CSkill::CSkill() :CScene(6)
{
}
//=============================================================================
// スキルのデストラクタ
//=============================================================================
CSkill::~CSkill()
{
}
//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CSkill::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\skill_2way.png",
		&m_pTexture[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\skill_speed.png",
		&m_pTexture[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\skill_barrier.png",
		&m_pTexture[2]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\skill_avatar.png",
		&m_pTexture[3]);
	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CSkill::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_SKILL; nCnt++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
//スキルの生成
//=============================================================================
CSkill *CSkill::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CSkill *pPause = NULL;

	if (pPause == NULL)
	{
		pPause = new CSkill;		//シーンの動的確保
		if (pPause != NULL)
		{
			for (int nCnt = 0; nCnt < MAX_SKILL; nCnt++)
			{
				pPause->m_pos = pos;			//位置の代入
				pPause->m_fWidth = fWidth;		//幅の代入
				pPause->m_fHeight = fHeight;	//高さの代入
				pPause->Init();					//初期化処理
			}
		}
	}
	return pPause;					//値を返す
}

//=============================================================================
// スキルの初期化処理
//=============================================================================
HRESULT CSkill::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_SKILL; nCnt++)
	{
		m_aCol[nCnt] = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);			//色を初期化
		m_state[nCnt] = SKILL_STATE_NONE;
	}

	float Pos = 0.0f;
	m_pVtxBuff = NULL;									//頂点情報を初期化

														//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//ポインタの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SKILL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

					//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)& pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SKILL; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth + Pos, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth + Pos, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth + Pos, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth + Pos, m_pos.y + m_fHeight, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = m_aCol[nCnt];
		pVtx[1].col = m_aCol[nCnt];
		pVtx[2].col = m_aCol[nCnt];
		pVtx[3].col = m_aCol[nCnt];

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
		Pos += 63.5f;
	}
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// スキルの終了処理
//=============================================================================
void CSkill::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// スキルの更新処理
//=============================================================================
void CSkill::Update(void)
{
	for (int nCntSkill = 0; nCntSkill < MAX_SKILL; nCntSkill++)
	{
		if (m_state[nCntSkill] == SKILL_STATE_NONE)
		{//スキルが使用されていない場合
			m_aCol[nCntSkill] = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
		}
		else if(m_state[nCntSkill] == SKILL_STATE_USE)
		{//スキルが使用されている場合
			m_aCol[nCntSkill] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

	}
	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)& pVtx, 0);
	for (int nCnt = 0; nCnt < MAX_SKILL; nCnt++)
	{
		//頂点カラーの設定
		pVtx[0].col = m_aCol[nCnt];
		pVtx[1].col = m_aCol[nCnt];
		pVtx[2].col = m_aCol[nCnt];
		pVtx[3].col = m_aCol[nCnt];
		pVtx += 4;
	}
		//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

//=============================================================================
// スキルの描画処理
//=============================================================================
void CSkill::Draw(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//ポインタの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPauseSelect = 0; nCntPauseSelect < MAX_SKILL; nCntPauseSelect++)
	{// 項目の数だけ繰り返し
	 // テクスチャの設定
		pDevice->SetTexture(0, m_pTexture[nCntPauseSelect]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPauseSelect * 4, 2);
	}
}

//=============================================================================
// スキルの終了処理
//=============================================================================
void CSkill::SetSkill(CSkill::SKILL skill, SKILL_STATE state)
{
	m_state[skill] = state;
}
