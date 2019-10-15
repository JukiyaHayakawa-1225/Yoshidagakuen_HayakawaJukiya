//=============================================================================
//
// UIの処理 [ui.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "ui.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "score.h"
#include "player.h"
#include "game.h"
#include "debugproc.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXTURE_SIZE        (30)            // テクスチャのサイズ

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// UIコンストラクタ
//=============================================================================
CUI::CUI()
{
	m_pTexture = NULL;							//テクスチャをNULLにする
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の初期値
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色の初期値
	m_fWidth = 10.0f;							//幅の初期値
	m_fHeight = 10.0f;							//高さの初期値
}
//=============================================================================
// UIデストラクタ
//=============================================================================
CUI::~CUI()
{

}

//=============================================================================
//UI生成
//=============================================================================
CUI *CUI::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CUI *pUI = NULL;
	if (pUI == NULL)
	{
		pUI = new CUI;						//シーンの動的確保
		if (pUI != NULL)
		{
			pUI->SetPos(pos);				//位置の設置処理
			pUI->SetWidth(fWidth);			//幅の設置処理
			pUI->SetHeight(fHeight);		//高さの設置処理
			pUI->Init();					//初期化処理
		}
	}
	return pUI;								//値を返す
}

//=============================================================================
// UI初期化処理
//=============================================================================
HRESULT CUI::Init(void)
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
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

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
// UI終了処理
//=============================================================================
void CUI::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// UI更新処理
//=============================================================================
void CUI::Update(void)
{

}

//=============================================================================
// UI描画処理
//=============================================================================
void CUI::Draw(void)
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
	//UI描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//=============================================================================
// オブジェクトの取得処理
//=============================================================================
void CUI::BindTexture(LPDIRECT3DTEXTURE9 texture)
{
	m_pTexture = texture;
}

//=============================================================================
// 位置の設置処理
//=============================================================================
void CUI::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// 色の設置処理
//=============================================================================
void CUI::SetCol(D3DXCOLOR col)
{
	m_col= col;

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 幅の設置処理
//=============================================================================
void CUI::SetWidth(float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
// 高さの設置処理
//=============================================================================
void CUI::SetHeight(float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
// 頂点情報の設置処理
//=============================================================================
void CUI::SetVtxPos(D3DXVECTOR3 pos)
{
	m_pos = pos;	//位置の代入

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// サイド選択型減算処理
//=============================================================================
void CUI::SetSideCut(int nType)
{
	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (nType == 0)
	{//左から減算
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
	}
	else if (nType == 1)
	{//右から減算
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);

	}
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 位置の取得処理
//=============================================================================
D3DXVECTOR3 CUI::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// 色の取得処理
//=============================================================================
D3DXCOLOR CUI::GetCol(void)
{
	return m_col;
}

//=============================================================================
// 幅の取得処理
//=============================================================================
float CUI::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
// 高さの取得処理
//=============================================================================
float CUI::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//
// UI体力ゲージの処理
// Author : Jukiya Hayakawa
//
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CUI_LifeGauge::m_pTexture = NULL;
CUI	*CUI_LifeGauge::m_pUI = NULL;
//=============================================================================
// UI体力ゲージコンストラクタ
//=============================================================================
CUI_LifeGauge::CUI_LifeGauge() : CScene(5, OBJTYPE_UI)
{

}

//=============================================================================
// UI体力ゲージデストラクタ
//=============================================================================
CUI_LifeGauge::~CUI_LifeGauge()
{

}

//=============================================================================
// UI体力ゲージのテクスチャ読み込み
//=============================================================================
HRESULT CUI_LifeGauge::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\GAUGE\\GAUGE_LIFE.png",
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// UI体力ゲージのテクスチャ破棄
//=============================================================================
void CUI_LifeGauge::Unload(void)
{

	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
// 体力ゲージの取得処理
//=============================================================================
CUI *CUI_LifeGauge::GetLifeGauge(void)
{
	return m_pUI;
}

//=============================================================================
// UI体力ゲージ生成
//=============================================================================
CUI_LifeGauge *CUI_LifeGauge::Create(void)
{
	CUI_LifeGauge *pUIGauge = NULL;
	if (pUIGauge == NULL)
	{
		pUIGauge = new CUI_LifeGauge;						//シーンの動的確保
		if (pUIGauge != NULL)
		{
			pUIGauge->Init();							//初期化処理
		}
	}
	return pUIGauge;									//値を返す
}

//=============================================================================
// UI体力ゲージ初期化処理
//=============================================================================
HRESULT CUI_LifeGauge::Init(void)
{
		m_pUI = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, SCREEN_HEIGHT - 100.0f, 0.0f),MAX_GAUGE_WIDTH,20.0f);	//UIの生成
		m_pUI->SetCol(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));																//色の設置処理
		m_pUI->SetSideCut(0);																							//左から減算処理
		m_pUI->BindTexture(m_pTexture);																		//テクスチャの読み込み
	return S_OK;
}

//=============================================================================
// UI体力ゲージ終了処理
//=============================================================================
void CUI_LifeGauge::Uninit(void)
{
	for (int nCntUI = 0; nCntUI < MAX_GAUGE_TEXTURE; nCntUI++)
	{//ゲージのテクスチャ分繰り返し
		if (m_pUI != NULL)
		{
			m_pUI->Uninit();
			//メモリの開放
			delete m_pUI;
			//NULLにする
			m_pUI = NULL;
		}
	}
	Release();
}

//=============================================================================
// UI体力ゲージ更新処理
//=============================================================================
void CUI_LifeGauge::Update(void)
{
	//プレイヤーの取得
	CPlayer	*pPlayer;
	pPlayer = CGame::GetPlayer();

	//UIダメージの取得処理
	CUI_DamageGauge *pUI_Damage;
	pUI_Damage = CGame::GetUI_DamageGauge();

	int nLife = 0;			//体力
	int nMaxLife = 0;		//体力の最大値
	float fData;			//データの格納
	if (pPlayer != NULL)
	{//プレイヤー情報がある場合
	 //体力の取得処理
		nLife = pPlayer->GetLife();
		//とくぎの取得処理
		nMaxLife = pPlayer->GetMaxLife();
	}

	float fWidth = m_pUI->GetWidth();	//幅
	D3DXVECTOR3 pos = m_pUI->GetPos();	//位置
	fData = MAX_GAUGE_WIDTH * nLife / nMaxLife;	//ゲージの計算
	if (fWidth >= 0.0f)
	{
		m_pUI->SetWidth(fData);		//幅の設置処理
		m_pUI->SetSideCut(0);		//左から減算処理
		if (fWidth <= MAX_GAUGE_WIDTH && fWidth >= (MAX_GAUGE_WIDTH / 2 + 1))
		{//ゲージが最大値以下かつ最大値の半分以上の場合
			m_pUI->SetCol(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));	//色の設置処理（緑）
		}
		else if (fWidth <= MAX_GAUGE_WIDTH / 2 && fWidth >= (MAX_GAUGE_WIDTH / 4 + 1))
		{//ゲージが最大値の半分以下かつ最大値の４分の１以上の場合
			m_pUI->SetCol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));	//色の設置処理（黄）
		}
		else if (fWidth <= MAX_GAUGE_WIDTH / 4 && fWidth >= 0)
		{//ゲージが最大値の半分以下かつ最大値の４分の１以上の場合
			m_pUI->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	//色の設置処理（赤）
		}
	}
	else
	{
		fWidth = 0.0f;				//値の固定
		m_pUI->SetWidth(fWidth);	//幅の設置処理
		m_pUI->SetSideCut(0);		//右から減算処理
	}
	pUI_Damage->SetDamageState(CUI_DamageGauge::STATE_MINUS);	//ゲージを減らす状態にする
}

//=============================================================================
// UI体力ゲージ描画処理
//=============================================================================
void CUI_LifeGauge::Draw(void)
{
	for (int nCntUI = 0; nCntUI < MAX_GAUGE_TEXTURE; nCntUI++)
	{//ゲージのテクスチャ分繰り返し
		if (m_pUI != NULL)
		{//情報がある場合
			m_pUI->Draw();		//描画処理
		}
	}
}

//=============================================================================
// UI体力ゲージ減算処理
//=============================================================================
void CUI_LifeGauge::CutLifeGauge(int nCut)
{

}


//=============================================================================
// UI体力ゲージ減算処理
//=============================================================================
void CUI_LifeGauge::ResetLifeGauge(int nLifeno)
{
	//プレイヤーの取得
	CPlayer	*pPlayer;
	pPlayer = CGame::GetPlayer();

	int nLife = 0;			//体力
	int nMaxLife = 0;		//体力の最大値
	float fData;			//データの格納

	if (pPlayer != NULL)
	{//プレイヤー情報がある場合
	 //体力の取得処理
		nLife = pPlayer->GetLife();
		//とくぎの取得処理
		nMaxLife = pPlayer->GetMaxLife();
	}

	D3DXVECTOR3 pos = m_pUI->GetPos();				//位置
	fData = MAX_GAUGE_WIDTH * nLife / nMaxLife;		//ゲージの計算
	m_pUI->SetWidth(fData);		//幅の設置処理
	m_pUI->SetSideCut(0);		//左から減算処理
}

//=============================================================================
//
// UIとくぎゲージの処理
// Author : Jukiya Hayakawa
//
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CUI_SkillGauge::m_apTexture[MAX_GAUGE_TEXTURE] = {};	//テクスチャのポインタ
CUI	*CUI_SkillGauge::m_apUI[MAX_GAUGE_TEXTURE] = {};					//UIのポインタ
//=============================================================================
// UIとくぎゲージコンストラクタ
//=============================================================================
CUI_SkillGauge::CUI_SkillGauge() : CScene(5, OBJTYPE_UI)
{

}

//=============================================================================
// UIとくぎゲージデストラクタ
//=============================================================================
CUI_SkillGauge::~CUI_SkillGauge()
{

}

//=============================================================================
// UIとくぎゲージのテクスチャ読み込み
//=============================================================================
HRESULT CUI_SkillGauge::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\GAUGE\\GAUGE_BASE.png",
		&m_apTexture[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\GAUGE\\GAUGE_LIFE.png",
		&m_apTexture[1]);

	return S_OK;
}

//=============================================================================
// UIとくぎゲージのテクスチャ破棄
//=============================================================================
void CUI_SkillGauge::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < MAX_GAUGE_TEXTURE; nCntTexture++)
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
// UIとくぎゲージ生成
//=============================================================================
CUI_SkillGauge *CUI_SkillGauge::Create(void)
{
	CUI_SkillGauge *pUIGauge = NULL;
	if (pUIGauge == NULL)
	{
		pUIGauge = new CUI_SkillGauge;						//シーンの動的確保
		if (pUIGauge != NULL)
		{
			pUIGauge->Init();								//初期化処理
		}
	}
	return pUIGauge;										//値を返す
}

//=============================================================================
// UIとくぎゲージ初期化処理
//=============================================================================
HRESULT CUI_SkillGauge::Init(void)
{
	for (int nCntUI = 0; nCntUI < MAX_GAUGE_TEXTURE; nCntUI++)
	{//ゲージのテクスチャ分繰り返し
		m_apUI[nCntUI] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, SCREEN_HEIGHT - 50.0f, 0.0f), 300.0f, 20.0f);	//UIの生成
		m_apUI[nCntUI]->SetCol(D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f));														//色の設置処理
		m_apUI[nCntUI]->SetSideCut(0);																					//左から減算処理
		m_apUI[nCntUI]->BindTexture(m_apTexture[nCntUI]);																//テクスチャの読み込み
	}
	return S_OK;
}

//=============================================================================
// UIとくぎゲージ終了処理
//=============================================================================
void CUI_SkillGauge::Uninit(void)
{
	for (int nCntUI = 0; nCntUI < MAX_GAUGE_TEXTURE; nCntUI++)
	{//ゲージのテクスチャ分繰り返し
		if (m_apUI[nCntUI] != NULL)
		{
			m_apUI[nCntUI]->Uninit();
			//メモリの開放
			delete m_apUI[nCntUI];
			//NULLにする
			m_apUI[nCntUI] = NULL;
		}
	}
	Release();
}

//=============================================================================
// UIとくぎゲージ更新処理
//=============================================================================
void CUI_SkillGauge::Update(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//プレイヤーの取得
	CPlayer	*pPlayer;
	pPlayer = CGame::GetPlayer();

	int nSkill = 0;			//体力
	int nMaxSkill = 0;		//体力の最大値
	float fData;			//データの格納
	if (pPlayer != NULL)
	{//プレイヤー情報がある場合
	 //体力の取得処理
		nSkill = pPlayer->GetSkill();
		//とくぎの取得処理
		nMaxSkill = pPlayer->GetMaxSkill();
	}

	float fWidth = m_apUI[1]->GetWidth();
	D3DXVECTOR3 pos = m_apUI[1]->GetPos();
	fData = MAX_GAUGE_WIDTH * nSkill / nMaxSkill;
	if (fWidth >= 0.0f)
	{
		m_apUI[1]->SetWidth(fData);	//幅の設置処理
		m_apUI[1]->SetSideCut(0);		//左から減算処理
	}
	else
	{
		fWidth = 0.0f;	//値の固定
		m_apUI[1]->SetWidth(fWidth);	//幅の設置処理
		m_apUI[1]->SetSideCut(0);		//右から減算処理
	}
}

//=============================================================================
// UIとくぎゲージ描画処理
//=============================================================================
void CUI_SkillGauge::Draw(void)
{
	for (int nCntUI = 0; nCntUI < MAX_GAUGE_TEXTURE; nCntUI++)
	{//ゲージのテクスチャ分繰り返し
		if (m_apUI[nCntUI] != NULL)
		{//情報がある場合
			m_apUI[nCntUI]->Draw();		//描画処理
		}
	}
}

//=============================================================================
// UI体力ゲージ減算処理
//=============================================================================
void CUI_SkillGauge::CutSkillGauge(int nCut)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//プレイヤーの取得
	CPlayer	*pPlayer;
	pPlayer = CGame::GetPlayer();

	int nSkill = 0;			//体力
	int nMaxSkill = 0;		//体力の最大値
	float fData;			//データの格納
	if (pPlayer != NULL)
	{//プレイヤー情報がある場合
	 //体力の取得処理
		nSkill = pPlayer->GetSkill();
		//とくぎの取得処理
		nMaxSkill = pPlayer->GetMaxSkill();
	}

	float fWidth = m_apUI[1]->GetWidth();
	D3DXVECTOR3 pos = m_apUI[1]->GetPos();
	fData = MAX_GAUGE_WIDTH * nSkill / nMaxSkill;
	if (fWidth >= 0.0f)
	{
		m_apUI[1]->SetWidth(fData);	//幅の設置処理
		m_apUI[1]->SetSideCut(0);		//左から減算処理
	}
	else
	{
		fWidth = 0.0f;	//値の固定
		m_apUI[1]->SetWidth(fWidth);	//幅の設置処理
		m_apUI[1]->SetSideCut(0);		//右から減算処理
	}
}

//=============================================================================
//
// ダメージゲージの処理
// Author : Jukiya Hayakawa
//
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CUI_DamageGauge::m_pTexture = NULL;
CUI	*CUI_DamageGauge::m_pUI = NULL;

//=============================================================================
// ダメージゲージコンストラクタ
//=============================================================================
CUI_DamageGauge::CUI_DamageGauge() : CScene(4, OBJTYPE_UI)
{

}

//=============================================================================
// ダメージゲージデストラクタ
//=============================================================================
CUI_DamageGauge::~CUI_DamageGauge()
{

}

//=============================================================================
// ダメージゲージのテクスチャ読み込み
//=============================================================================
HRESULT CUI_DamageGauge::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\GAUGE\\DAMAGE_GAUGE.png",
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// ダメージゲージのテクスチャ破棄
//=============================================================================
void CUI_DamageGauge::Unload(void)
{

		// テクスチャの破棄
		if (m_pTexture != NULL)
		{
			m_pTexture->Release();
			m_pTexture = NULL;
		}
}

//=============================================================================
// ダメージゲージ生成
//=============================================================================
CUI_DamageGauge *CUI_DamageGauge::Create(void)
{
	CUI_DamageGauge *pUIGauge = NULL;
	if (pUIGauge == NULL)
	{
		pUIGauge = new CUI_DamageGauge;						//シーンの動的確保
		if (pUIGauge != NULL)
		{
			pUIGauge->Init();							//初期化処理
		}
	}
	return pUIGauge;									//値を返す
}

//=============================================================================
// ダメージゲージ初期化処理
//=============================================================================
HRESULT CUI_DamageGauge::Init(void)
{
	m_state = STATE_NOMAL;	//状態の初期化
	m_pUI = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, SCREEN_HEIGHT - 100.0f, 0.0f), 300.0f, 20.0f);	//UIの生成
	m_pUI->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));														//色の設置処理
	m_pUI->SetSideCut(0);																					//左から減算処理
	m_pUI->BindTexture(m_pTexture);																			//テクスチャの読み込み
	return S_OK;
}

//=============================================================================
// ダメージゲージ終了処理
//=============================================================================
void CUI_DamageGauge::Uninit(void)
{
	if (m_pUI != NULL)
	{
		m_pUI->Uninit();
		//メモリの開放
		delete m_pUI;
		//NULLにする
		m_pUI = NULL;
	}
	Release();
}

//=============================================================================
// ダメージゲージ更新処理
//=============================================================================
void CUI_DamageGauge::Update(void)
{
	CUI *pLifeGauge = NULL;
	pLifeGauge = CUI_LifeGauge::GetLifeGauge();

	float fWidth;
	float fLifeWidth;
	fWidth = m_pUI->GetWidth();
	fLifeWidth = pLifeGauge->GetWidth();
	switch (m_state)
	{
	case STATE_NOMAL:		//通常状態の場合
		break;
	case STATE_MINUS:		//減算状態の場合
		fWidth -= 0.5f;	//減算処理
		if (fWidth <= fLifeWidth)
		{//幅が一致した場合
			fWidth = fLifeWidth;	//幅の固定
		}
		break;
	}
	m_pUI->SetWidth(fWidth);
	m_pUI->SetSideCut(0);
}

//=============================================================================
// ダメージゲージ描画処理
//=============================================================================
void CUI_DamageGauge::Draw(void)
{

	if (m_pUI != NULL)
	{//情報がある場合
		m_pUI->Draw();		//描画処理
	}
}

//=============================================================================
// 状態設置処理
//=============================================================================
void CUI_DamageGauge::SetDamageState(DAMAGE_STATE state)
{
	m_state = state;
}

//=============================================================================
//
// UIベースゲージの処理
// Author : Jukiya Hayakawa
//
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CUI_BaseGauge::m_pTexture = NULL;
CUI	*CUI_BaseGauge::m_pUI = NULL;
//=============================================================================
// UIベースゲージコンストラクタ
//=============================================================================
CUI_BaseGauge::CUI_BaseGauge() : CScene(3, OBJTYPE_UI)
{

}

//=============================================================================
// UIベースゲージデストラクタ
//=============================================================================
CUI_BaseGauge::~CUI_BaseGauge()
{

}

//=============================================================================
// UIベースゲージのテクスチャ読み込み
//=============================================================================
HRESULT CUI_BaseGauge::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\GAUGE\\GAUGE_BASE.png",
		&m_pTexture);
	return S_OK;
}

//=============================================================================
// UIベースゲージのテクスチャ破棄
//=============================================================================
void CUI_BaseGauge::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// UIベースゲージ生成
//=============================================================================
CUI_BaseGauge *CUI_BaseGauge::Create(void)
{
	CUI_BaseGauge *pUIGauge = NULL;
	if (pUIGauge == NULL)
	{
		pUIGauge = new CUI_BaseGauge;						//シーンの動的確保
		if (pUIGauge != NULL)
		{
			pUIGauge->Init();							//初期化処理
		}
	}
	return pUIGauge;									//値を返す
}

//=============================================================================
// UIベースゲージ初期化処理
//=============================================================================
HRESULT CUI_BaseGauge::Init(void)
{
	m_pUI = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, SCREEN_HEIGHT - 100.0f, 0.0f), 300.0f, 20.0f);	//UIの生成
	m_pUI->SetCol(D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f));														//色の設置処理
	m_pUI->SetSideCut(0);																					//左から減算処理
	m_pUI->BindTexture(m_pTexture);																			//テクスチャの読み込み

	return S_OK;
}

//=============================================================================
// UIベースゲージ終了処理
//=============================================================================
void CUI_BaseGauge::Uninit(void)
{
	if (m_pUI != NULL)
	{
		m_pUI->Uninit();
		//メモリの開放
		delete m_pUI;
		//NULLにする
		m_pUI = NULL;
	}

	Release();
}

//=============================================================================
// UIベースゲージ更新処理
//=============================================================================
void CUI_BaseGauge::Update(void)
{
}

//=============================================================================
// UIベースゲージ描画処理
//=============================================================================
void CUI_BaseGauge::Draw(void)
{
	if (m_pUI != NULL)
	{//情報がある場合
		m_pUI->Draw();		//描画処理
	}
}

