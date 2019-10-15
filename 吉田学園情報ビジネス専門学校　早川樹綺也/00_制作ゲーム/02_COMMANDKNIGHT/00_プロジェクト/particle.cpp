//=============================================================================
//
// パーティクルの処理 [effect.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "particle.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "enemy.h"
#include "score.h"
#include "scenebillboard.h"

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
CParticle::CParticle() : CSceneBillboard()
{
	m_nLife = 30;								//体力の初期値
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
		pParticle->SetMove(move);				//移動量の設置処理
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
		"data\\TEXTURE\\effect000.png",
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
	CSceneBillboard::Init();
	return S_OK;
}

//=============================================================================
// オブジェクトの終了処理
//=============================================================================
void CParticle::Uninit(void)
{
	//終了処理
	CSceneBillboard::Uninit();
}

//=============================================================================
// オブジェクトの更新処理
//=============================================================================
void CParticle::Update(void)
{
	float		length;		//拡大
	int			nLife;		//体力

	//拡大の取得
	length = GetLength();
	//体力の取得
	nLife = GetLife();

	if (length <= 0.0f || nLife <= 0)
	{//拡大数または高さが0以下または体力が0以下になった場合
		//終了処理
		Uninit();
	}

	//CScene2Dの更新処理
	CSceneBillboard::Update();
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
	CSceneBillboard::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// 体力の設置処理
//=============================================================================
void CParticle::SetLife(int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
// 体力の設置処理
//=============================================================================
int CParticle::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
// とくぎ2パーティクルの処理
//=============================================================================
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CSkill2Particle::m_pTexture = NULL;

//=============================================================================
// オブジェクトのコンストラクタ
//=============================================================================
CSkill2Particle::CSkill2Particle() : CParticle()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量の初期値
}
//=============================================================================
// オブジェクトのデストラクタ
//=============================================================================
CSkill2Particle::~CSkill2Particle()
{

}

//=============================================================================
//オブジェクトの生成
//=============================================================================
CSkill2Particle *CSkill2Particle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fLength,int nLife)
{
	CSkill2Particle *pSkill2Particle = NULL;

	if (pSkill2Particle == NULL)
	{
		pSkill2Particle = new CSkill2Particle;			//シーンの動的確保
		pSkill2Particle->SetPos(pos);					//位置の代入
		pSkill2Particle->SetLength(fLength);			//拡大の代入
		pSkill2Particle->SetMove(move);					//移動量のせっち
		pSkill2Particle->SetLife(nLife);				//体力の設置処理
		pSkill2Particle->Init();						//初期化処理
		pSkill2Particle->SetCol(col);					//色の代入
		pSkill2Particle->BindTexture(m_pTexture);		//テクスチャ
	}
	return pSkill2Particle;							//値を返す
}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CSkill2Particle::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\particle_skill0.png",
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CSkill2Particle::Unload(void)
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
HRESULT CSkill2Particle::Init(void)
{
	//初期化処理
	CParticle::Init();
	return S_OK;
}

//=============================================================================
// オブジェクトの終了処理
//=============================================================================
void CSkill2Particle::Uninit(void)
{
	//終了処理
	CParticle::Uninit();
}

//=============================================================================
// オブジェクトの更新処理
//=============================================================================
void CSkill2Particle::Update(void)
{
	D3DXVECTOR3	pos,move;		//位置
	D3DXCOLOR	col;		//色
	float		length;		//拡大
	int			nLife;		//体力

	//位置の取得
	pos = GetPos();
	//色の取得
	col = GetCol();
	//拡大の取得
	length = GetLength();
	//体力の取得
	nLife = GetLife();
	//移動量の取得
	move = GetMove();

	//移動量加算
	pos += move;
	pos.y += 1.0f;

	//体力の減算
	nLife--;

	//位置の設置
	SetPos(pos);
	//拡大の設置
	SetLength(length);
	//位置の設置
	SetLengthPos();
	//色の設置
	SetCol(col);
	//体力の設置
	SetLife(nLife);

	//パーティクル更新処理
	CParticle::Update();
}

//=============================================================================
// オブジェクトの描画処理
//=============================================================================
void CSkill2Particle::Draw(void)
{
	CParticle::Draw();
	//CBillboard::Draw();
}

//=============================================================================
// ダメージパーティクルの処理
//=============================================================================
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CDamageParticle::m_pTexture = NULL;

//=============================================================================
// オブジェクトのコンストラクタ
//=============================================================================
CDamageParticle::CDamageParticle() : CParticle()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量の初期値
}
//=============================================================================
// オブジェクトのデストラクタ
//=============================================================================
CDamageParticle::~CDamageParticle()
{

}

//=============================================================================
//オブジェクトの生成
//=============================================================================
CDamageParticle *CDamageParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fLength, int nLife)
{
	CDamageParticle *pDamageParticle = NULL;

	if (pDamageParticle == NULL)
	{
		pDamageParticle = new CDamageParticle;			//シーンの動的確保
		pDamageParticle->SetPos(pos);					//位置の代入
		pDamageParticle->SetLength(fLength);			//拡大の代入
		pDamageParticle->SetMove(move);					//移動量の設置処理
		pDamageParticle->SetLife(nLife);				//体力の設置処理
		pDamageParticle->Init();						//初期化処理
		pDamageParticle->SetCol(col);					//色の代入
		pDamageParticle->BindTexture(m_pTexture);		//テクスチャ
	}
	return pDamageParticle;							//値を返す
}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CDamageParticle::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect000.png",
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CDamageParticle::Unload(void)
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
HRESULT CDamageParticle::Init(void)
{
	//初期化処理
	CParticle::Init();
	return S_OK;
}

//=============================================================================
// オブジェクトの終了処理
//=============================================================================
void CDamageParticle::Uninit(void)
{
	//終了処理
	CParticle::Uninit();
}

//=============================================================================
// オブジェクトの更新処理
//=============================================================================
void CDamageParticle::Update(void)
{
	D3DXVECTOR3	pos,move;	//位置
	D3DXCOLOR	col;		//色
	float		length;		//拡大
	int			nLife;		//体力

	//位置の取得
	pos = GetPos();
	//色の取得
	col = GetCol();
	//拡大の取得
	length = GetLength();
	//体力の取得
	nLife = GetLife();
	//移動量の取得
	move = GetMove();

	//移動量加算
	pos += m_move;
	//pos.y += 1.0f;

	length -= 1.0f;
	//体力の減算
	nLife--;
	pos += move;

	//位置の設置
	SetPos(pos);
	//拡大の設置
	SetLength(length);
	//位置の設置
	SetLengthPos();
	//色の設置
	SetCol(col);
	//体力の設置
	SetLife(nLife);

	//パーティクル更新処理
	CParticle::Update();
}

//=============================================================================
// オブジェクトの描画処理
//=============================================================================
void CDamageParticle::Draw(void)
{
	CParticle::Draw();
	//CBillboard::Draw();
}
//=============================================================================
// 雪パーティクルの処理
//=============================================================================
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CSnowParticle::m_pTexture = NULL;

//=============================================================================
// オブジェクトのコンストラクタ
//=============================================================================
CSnowParticle::CSnowParticle() : CParticle()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量の初期値
}
//=============================================================================
// オブジェクトのデストラクタ
//=============================================================================
CSnowParticle::~CSnowParticle()
{

}

//=============================================================================
//オブジェクトの生成
//=============================================================================
CSnowParticle *CSnowParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fLength, int nLife)
{
	CSnowParticle *pSnowParticle = NULL;

	if (pSnowParticle == NULL)
	{
		pSnowParticle = new CSnowParticle;			//シーンの動的確保
		pSnowParticle->SetPos(pos);					//位置の代入
		pSnowParticle->SetLength(fLength);			//拡大の代入
		pSnowParticle->SetMove(move);				//移動量の設置処理
		pSnowParticle->SetLife(nLife);				//体力の設置処理
		pSnowParticle->Init();						//初期化処理
		pSnowParticle->SetCol(col);					//色の代入
		pSnowParticle->BindTexture(m_pTexture);		//テクスチャ
	}
	return pSnowParticle;							//値を返す
}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CSnowParticle::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\particle_skill0.png",
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CSnowParticle::Unload(void)
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
HRESULT CSnowParticle::Init(void)
{
	//初期化処理
	CParticle::Init();
	m_nTimer = 0;
	return S_OK;
}

//=============================================================================
// オブジェクトの終了処理
//=============================================================================
void CSnowParticle::Uninit(void)
{
	//終了処理
	CParticle::Uninit();
}

//=============================================================================
// オブジェクトの更新処理
//=============================================================================
void CSnowParticle::Update(void)
{
	D3DXVECTOR3	pos, move;		//位置
	D3DXCOLOR	col;		//色
	float		length;		//拡大
	int			nLife;		//体力

	//位置の取得
	pos = GetPos();
	//色の取得
	col = GetCol();
	//拡大の取得
	length = GetLength();
	//体力の取得
	nLife = GetLife();
	//移動量の取得
	move = GetMove();

	//移動量加算
	//pos += m_move;
	//pos.y += 1.0f;
	pos.y -= 1.0f;
	m_nTimer++;

	if (m_nTimer >= 20)
	{

		int nData = rand() % 4 + 1;
		if (nData == 1 || nData == 2)
		{
			pos.x -= 1.0f;
		}
		else if (nData == 3 || nData == 4)
		{
			pos.x += 1.0f;

		}
		m_nTimer = 0;
	}
	//length -= 1.0f;
	//体力の減算
	nLife--;
	//pos += move;

	//位置の設置
	SetPos(pos);
	//拡大の設置
	SetLength(length);
	//位置の設置
	SetLengthPos();
	//色の設置
	SetCol(col);
	//体力の設置
	SetLife(nLife);

	//パーティクル更新処理
	CParticle::Update();
}

//=============================================================================
// オブジェクトの描画処理
//=============================================================================
void CSnowParticle::Draw(void)
{
	CParticle::Draw();
	//CBillboard::Draw();
}


//=============================================================================
// 噴水パーティクルの処理
//=============================================================================
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CFountainParticle::m_pTexture = NULL;

//=============================================================================
// オブジェクトのコンストラクタ
//=============================================================================
CFountainParticle::CFountainParticle() : CParticle()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量の初期値
}
//=============================================================================
// オブジェクトのデストラクタ
//=============================================================================
CFountainParticle::~CFountainParticle()
{

}

//=============================================================================
//オブジェクトの生成
//=============================================================================
CFountainParticle *CFountainParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fLength, int nLife)
{
	CFountainParticle *pFountainParticle = NULL;

	if (pFountainParticle == NULL)
	{
		pFountainParticle = new CFountainParticle;		//シーンの動的確保
		pFountainParticle->SetPos(pos);					//位置の代入
		pFountainParticle->SetLength(fLength);			//拡大の代入
		pFountainParticle->SetMove(move);				//移動量の設置処理
		pFountainParticle->SetLife(nLife);				//体力の設置処理
		pFountainParticle->Init();						//初期化処理
		pFountainParticle->SetCol(col);					//色の代入
		pFountainParticle->BindTexture(m_pTexture);		//テクスチャ
	}
	return pFountainParticle;							//値を返す
}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CFountainParticle::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\particle_skill0.png",
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CFountainParticle::Unload(void)
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
HRESULT CFountainParticle::Init(void)
{
	//初期化処理
	CParticle::Init();
	m_nTimer = 0;
	return S_OK;
}

//=============================================================================
// オブジェクトの終了処理
//=============================================================================
void CFountainParticle::Uninit(void)
{
	//終了処理
	CParticle::Uninit();
}

//=============================================================================
// オブジェクトの更新処理
//=============================================================================
void CFountainParticle::Update(void)
{
	D3DXVECTOR3	pos, move;		//位置
	D3DXCOLOR	col;		//色
	float		length;		//拡大
	int			nLife;		//体力

	//位置の取得
	pos = GetPos();
	//色の取得
	col = GetCol();
	//拡大の取得
	length = GetLength();
	//体力の取得
	nLife = GetLife();
	//移動量の取得
	move = GetMove();

	//移動量加算
	pos += move;
	//pos.y += 1.0f;
	pos.y -= 5.0f;

	//length -= 1.0f;
	//体力の減算
	nLife--;
	//pos += move;

	//位置の設置
	SetPos(pos);
	//拡大の設置
	SetLength(length);
	//位置の設置
	SetLengthPos();
	//色の設置
	SetCol(col);
	//体力の設置
	SetLife(nLife);

	//パーティクル更新処理
	CParticle::Update();
}

//=============================================================================
// オブジェクトの描画処理
//=============================================================================
void CFountainParticle::Draw(void)
{
	CParticle::Draw();
	//CBillboard::Draw();
}
