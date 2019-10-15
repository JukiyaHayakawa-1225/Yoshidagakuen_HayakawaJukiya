//=============================================================================
//
// 弾の処理 [bullet.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "player.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;	//テクスチャのポインタ

//=============================================================================
// コンストラクタ
//=============================================================================
CBullet::CBullet() : CSceneBillboard()
{
	m_nLife = 0;			//体力の初期値
}
//=============================================================================
// デストラクタ
//=============================================================================
CBullet::~CBullet()
{

}
//=============================================================================
// テクスチャ読み込み
//=============================================================================
HRESULT CBullet::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bullet000.png",
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャ破棄
//=============================================================================
void CBullet::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// ビルボード体力ゲージ生成
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, int nLife,D3DXVECTOR3 move)
{
	CBullet *pBullet = NULL;
	if (pBullet == NULL)
	{
		pBullet = new CBullet;						//シーンの動的確保
		if (pBullet != NULL)
		{
			pBullet->SetPos(pos);					//位置の設置処理
			pBullet->m_nLife = nLife;				//体力の設置処理
			pBullet->SetMove(move);					//移動量の設置処理
			pBullet->SetWidth(15.0f);				//幅の設置処理
			pBullet->SetHeight(15.0f);				//高さの設置処理
			pBullet->Init();						//初期化処理
			pBullet->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	//色の設置処理
			pBullet->BindTexture(m_pTexture);		//テクスチャの貼り付け
		}
	}
	return pBullet;									//値を返す
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBullet::Init(void)
{
	//ビルボードの初期化処理
	CSceneBillboard::Init();

	//色の設置処理
	SetCol(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBullet::Uninit(void)
{
	//ビルボードの終了処理
	CSceneBillboard::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBullet::Update(void)
{
	//プレイヤーの取得処理
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();

	D3DXVECTOR3 pos = GetPos();					//位置の取得処理
	D3DXVECTOR3 move = GetMove();				//移動量の取得処理
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();	//プレイヤーの取得処理
	pos += move;								//位置の代入

	//当たり判定の計算処理
	float fLength = powf((PlayerPos.x - pos.x)*(PlayerPos.x - pos.x) + (PlayerPos.z - pos.z)*(PlayerPos.z - pos.z), 0.5f);	//プレイヤーとの距離
	float fHit = powf(3.0f, 2.0f);	//半径
	if (fHit > fLength)
	{//プレイヤーの距離が範囲内の場合
		//ダメージ処理
		int nDamade = rand() % 5 + 3;
		pPlayer->HitDamage(nDamade);	//プレイヤーのダメージ処理
		//終了処理
		Uninit();
	}

	//体力の減算処理
	m_nLife--;

	if (m_nLife <= 0)
	{//体力が０以下の場合
		//終了処理
		Uninit();
	}
	//位置の設置処理
	SetPos(pos);
}

//=============================================================================
// 描画処理
//=============================================================================
void CBullet::Draw(void)
{
	//ビルボードの描画処理
	CSceneBillboard::Draw();
}