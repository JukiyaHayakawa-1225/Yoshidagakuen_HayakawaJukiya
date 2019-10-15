//=============================================================================
//
// 敵の処理 [enemy.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "enemy.h"
#include "game.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "explotion.h"
#include "score.h"
#include "fade.h"
#include "result.h"
#include "player.h"
#include "barrier.h"
#include "defense.h"
#include "particle.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture = NULL;
int CEnemy::m_nNumEnemy = 0;

//=============================================================================
// マクロ定義
//=============================================================================
#define DAMAGE_TIME (5)	//ダメージの時間

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// 敵のコンストラクタ
//=============================================================================
CEnemy::CEnemy(int nPriority) : CScene2D(nPriority)
{
	m_fSpin = 1.0f;
	m_nLife = 1;							//体力の初期値
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置の初期値
	m_nNumEnemy++;							//総数加算
}
//=============================================================================
// 敵のデストラクタ
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// 敵の初期化処理
//=============================================================================
HRESULT CEnemy::Init(void)
{
	//CScene2Dの初期化処理
	CScene2D::Init();
	//種類の設定
	SetObjType(OBJTYPE_ENEMY);
	return S_OK;
}

//=============================================================================
// 敵の終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	//敵の総数を減らす
	m_nNumEnemy--;

	//CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 敵の更新処理
//=============================================================================
void CEnemy::Update(void)
{
	D3DXVECTOR3 pos;	//位置
	pos = GetPos();		//位置の取得

	switch (m_enemyState)
	{
	case ENEMYSTATE_NORMAL:	//通常状態
		break;
	case ENEMYSTATE_DAMEGE:	//ダメージ状態
		m_nCountState--;	//減算処理
		if (m_nCountState <= 0)
		{
			m_enemyState = ENEMYSTATE_NORMAL;

			//色の取得
			D3DXCOLOR col;
			col = GetCol();

			//色の指定
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//色の設置
			SetCol(col);

		}
	}
	if (pos.y >= SCREEN_HEIGHT + 50 || pos.y <= -500 || pos.x <= 250.0f || pos.x >= SCREEN_WIDTH)
	{//範囲外の場合
		//終了処理
		CEnemy::Uninit();
	}
}

//=============================================================================
// 敵の描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	//CScene2Dの描画処理
	CScene2D::Draw();
}
//=============================================================================
// 敵のダメージ処理
//=============================================================================
void CEnemy::HitDamage(int nDamage,CEnemy::ENEMY_TYPE type)
{
	CScore *pScore = CGame::GetScore();

	m_nLife -= nDamage;

	if (m_nLife <= 0)
	{// 敵のライフが0になったとき
		//位置の取得
		D3DXVECTOR3 pos;
		pos = GetPos();

		switch (type)
		{
		case ENEMY_TYPE_NOMAL:
			//スコアの加算
			pScore->AddScore(500);

			//爆発の生成
			//CExplotion::Create(pos, 50.0f,50);

			for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
			{
				m_fSpin += 0.05f;
				D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
				movePar.x = sinf(D3DX_PI * m_fSpin) * (rand() % 10 * 0.3f);
				movePar.y = cosf(D3DX_PI * m_fSpin) * (rand() % 10 * 0.3f);
				CParticle::Create(pos, movePar, D3DXCOLOR(rand() % 10 * 0.15f, rand() % 10 * 0.15f, rand() % 10 * 0.15f, 1.0f), rand() % 10 + 30.0f);
			}

			//終了処理
			Uninit();
			break;
		case ENEMY_TYPE_BOSS:
			//スコアの加算
			pScore->AddScore(10000);

			for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
			{
				m_fSpin += 0.05f;
				D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
				movePar.x = sinf(D3DX_PI * m_fSpin) * (rand() % 10 * 0.3f);
				movePar.y = cosf(D3DX_PI * m_fSpin) * (rand() % 10 * 0.3f);
				CParticle::Create(pos, movePar, D3DXCOLOR(rand() % 10 * 0.15f, rand() % 10 * 0.15f, rand() % 10 * 0.15f, 1.0f), rand() % 30 + 30.0f);
			}
			CGame::SetState(CGame::STATE_END);
			//終了処理
			Uninit();
			break;
		}

	}
	else
	{
		m_enemyState = ENEMYSTATE_DAMEGE;	//ダメージ状態にする
		m_nCountState = DAMAGE_TIME;		//ダメージの時間

		//色の取得
		D3DXCOLOR col;
		col = GetCol();

		//色の指定
		col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//色の設置
		SetCol(col);
	}
}

//=============================================================================
// 敵の当たり判定処理
//=============================================================================
void CEnemy::Collision(void)
{
	D3DXVECTOR3 pos;	//位置
	pos = GetPos();		//位置の取得

	CBarrier::BARRIER_STATE pBarrierState;	//バリアの状態
	pBarrierState = CBarrier::GetState();	//バリアの取得

	for (int nCntPrioty = 0; nCntPrioty < NUM_PRIORITY; nCntPrioty++)
	{//優先度の数だけ繰り返し
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{//Sceneの数だけ繰り返し
			CScene2D *pScene;												//シーンのポインタ
			pScene = (CScene2D*)CScene::GetScene(nCntPrioty, nCntScene);	//シーンの取得
			if (pScene != NULL)
			{
				CScene::OBJTYPE objType;		//種類
				objType = pScene->GetObjType();	//種類の取得

				if (objType == OBJTYPE_PLAYER)
				{//プレイヤーの場合
					D3DXVECTOR3 PlayerPos;					//敵の位置
					PlayerPos = pScene->GetPos();			//位置の取得
					CPlayer *pPlayer = (CPlayer*)pScene;
					if (pos.x > PlayerPos.x - PLAYER_SIZE
						&& pos.x < PlayerPos.x + PLAYER_SIZE
						&& pos.y <  PlayerPos.y + PLAYER_SIZE
						&& pos.y > PlayerPos.y - PLAYER_SIZE)
					{
						if (pBarrierState == CBarrier::STATE_NONE && objType == OBJTYPE_PLAYER)
						{//バリアを使用していない場合
						 //ダメージ処理
							pPlayer->HitDamage(3);
						}
					}
				}
				else if (objType == OBJTYPE_BARRIER)
				{//バリアの場合
					D3DXVECTOR3 BarrierPos;					//敵の位置
					BarrierPos = pScene->GetPos();			//位置の取得
					CBarrier::BARRIER_STATE pBarrierState;
					pBarrierState = CBarrier::GetState();

					if (pos.x + 30.0f > BarrierPos.x - BARRIER_SIZE
						&& pos.x - 30.0f < BarrierPos.x + BARRIER_SIZE
						&& pos.y - 30.0f <  BarrierPos.y + BARRIER_SIZE
						&& pos.y + 30.0f > BarrierPos.y - BARRIER_SIZE)
					{

						if (pBarrierState == CBarrier::STATE_USE)
						{//バリアが使用されている場合
						 //終了処理
							CScore *pScore = CGame::GetScore();
							pScore->AddScore(250);
							for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
							{
								m_fSpin += 0.05f;
								D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
								movePar.x = sinf(D3DX_PI * m_fSpin) * (rand() % 10 * 0.3f);
								movePar.y = cosf(D3DX_PI * m_fSpin) * (rand() % 10 * 0.3f);
								CParticle::Create(pos, movePar, D3DXCOLOR(rand() % 10 * 0.15f, rand() % 10 * 0.15f, rand() % 10 * 0.15f, 1.0f), rand() % 10 + 30.0f);
							}
							//終了処理
							CEnemy::Uninit();
						}
					}
				}
			}
		}
	}
}

//=============================================================================
//
// 敵横移動の処理
//
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemySlide::m_pTexture = NULL;
//=============================================================================
// 敵のコンストラクタ
//=============================================================================
CEnemySlide::CEnemySlide() : CEnemy()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置の初期値
}
//=============================================================================
// 敵のデストラクタ
//=============================================================================
CEnemySlide::~CEnemySlide()
{

}

//=============================================================================
//敵の生成
//=============================================================================
CEnemySlide *CEnemySlide::Create(D3DXVECTOR3 pos, float fWight, float fHeight,CEnemySlide::SLIDE slide)
{
	CEnemySlide *pEnemy = NULL;
	if (pEnemy == NULL)
	{
		pEnemy = new CEnemySlide;			//シーンの動的確保
		pEnemy->SetPos(pos);				//位置の代入
		pEnemy->SetWidth(fWight);			//幅の代入
		pEnemy->SetHeight(fHeight);			//高さの代入
		pEnemy->m_slide = slide;			//状態の代入
		pEnemy->Init();						//初期化処理
		pEnemy->BindTexture(m_pTexture);	//テクスチャ
	}
	return pEnemy;							//値を返す
}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CEnemySlide::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME006,
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CEnemySlide::Unload(void)
{
	m_pTexture->Release();
	m_pTexture = NULL;
}


//=============================================================================
// 敵の初期化処理
//=============================================================================
HRESULT CEnemySlide::Init(void)
{
	//CScene2Dの初期化処理
	CEnemy::Init();
	m_move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	return S_OK;
}

//=============================================================================
// 敵の終了処理
//=============================================================================
void CEnemySlide::Uninit(void)
{
	//CScene2Dの終了処理
	CEnemy::Uninit();
}

//=============================================================================
// 敵の更新処理
//=============================================================================
void CEnemySlide::Update(void)
{
	D3DXVECTOR3 pos;			//位置
	pos = GetPos();				//位置の取得

	for (int nCntPrioty = 0; nCntPrioty < 6; nCntPrioty++)
	{//優先度の数だけ繰り返し
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{//Sceneの4番だけ繰り返し
			CScene2D *pScene;												//シーンのポインタ
			pScene = (CScene2D*)CScene::GetScene(nCntPrioty, nCntScene);	//シーンの取得
			if (pScene != NULL)
			{//SceneがNULLでない場合
				CScene::OBJTYPE objType;		//種類
				objType = pScene->GetObjType();	//種類の取得
				if (objType == OBJTYPE_PLAYER)
				{//プレイヤーの場合
					D3DXVECTOR3 PlayerPos;					//プレイヤーの位置
					CPlayer *pPlayer = (CPlayer*)pScene;	//プレイヤーの動的確保
					PlayerPos = pPlayer->GetPos();			//位置の取得
					if (pPlayer != NULL)
					{//プレイヤーがNULLでない場合
					 	if (pos.y >= PlayerPos.y + 5)
					 	{
							switch (m_slide)
							{
							case SLIDE_R:
								m_move = D3DXVECTOR3(3.0f, 0.0f, 0.0f);
								break;
							case SLIDE_L:
								m_move = D3DXVECTOR3(-3.0f, 0.0f, 0.0f);
							}
					 	}
					}
				}
			}
		}
	}
	pos += m_move;
	//位置の設置
	SetVtxPos(pos);
	CEnemy::Update();
	CEnemy::Collision();
}

//=============================================================================
// 敵の描画処理
//=============================================================================
void CEnemySlide::Draw(void)
{
	//CScene2Dの描画処理
	CEnemy::Draw();
}

//=============================================================================
//
// 敵曲線移動の処理
//
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemyCurve::m_pTexture = NULL;
//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// 敵のコンストラクタ
//=============================================================================
CEnemyCurve::CEnemyCurve() : CEnemy()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//=============================================================================
// 敵のデストラクタ
//=============================================================================
CEnemyCurve::~CEnemyCurve()
{

}

//=============================================================================
//敵の生成
//=============================================================================
CEnemyCurve *CEnemyCurve::Create(D3DXVECTOR3 pos, float fWight, float fHeight,CEnemyCurve::CURVE curve)
{
	CEnemyCurve *pEnemy = NULL;
	if (pEnemy == NULL)
	{
		pEnemy = new CEnemyCurve;		//シーンの動的確保
		pEnemy->SetPos(pos);			//位置の代入
		pEnemy->SetWidth(fWight);		//幅の代入
		pEnemy->SetHeight(fHeight);		//高さの代入
		pEnemy->m_curve = curve;		//状態の代入
		pEnemy->Init();					//初期化処理
		pEnemy->BindTexture(m_pTexture);
	}
	return pEnemy;					//値を返す
}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CEnemyCurve::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy002.png",
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CEnemyCurve::Unload(void)
{
	m_pTexture->Release();
	m_pTexture = NULL;
}


//=============================================================================
// 敵の初期化処理
//=============================================================================
HRESULT CEnemyCurve::Init(void)
{
	//CScene2Dの初期化処理
	CEnemy::Init();
	m_move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	m_nCounter = 0;
	return S_OK;
}

//=============================================================================
// 敵の終了処理
//=============================================================================
void CEnemyCurve::Uninit(void)
{
	//CScene2Dの終了処理
	CEnemy::Uninit();
}

//=============================================================================
// 敵の更新処理
//=============================================================================
void CEnemyCurve::Update(void)
{
	D3DXVECTOR3 pos;			//位置

	pos = GetPos();				//位置の取得

	pos += m_move;				//移動量加算
	if (pos.y >= 0)
	{
		m_nCounter++;
	}
	switch (m_curve)
	{
	case CURVE_R:
		if (m_nCounter < 200)
		{//カウンタが２００以下まで移動
			m_move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
		}
		if (m_nCounter >= 200)
		{//２００以上になった場合
			m_move = D3DXVECTOR3(sinf(D3DX_PI*(m_nCounter - 200.0f) / 160.0f)*4.0f, sinf(D3DX_PI*(m_nCounter - 120.0f) / 160.0f)*4.0f, 0.0f);
		}
		if (m_nCounter >= 350)
		{//350以上になった場合
			m_move = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
		}
		break;

	case CURVE_L:
		if (m_nCounter < 200)
		{//200より小さくなった場合
			m_move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
		}
		if (m_nCounter >= 200)
		{//200以上になった場合
			m_move = D3DXVECTOR3(sinf(-D3DX_PI*(m_nCounter - 200.0f) / 160.0f)*4.0f, sinf(D3DX_PI*(m_nCounter - 120.0f) / 160.0f)*4.0f, 0.0f);
		}
		if (m_nCounter >= 350)
		{//350以上になった場合
			m_move = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
		}
	}
	//位置の設置
	SetVtxPos(pos);

	//更新処理
	CEnemy::Update();
	//当たり判定
	CEnemy::Collision();
}

//=============================================================================
// 敵の描画処理
//=============================================================================
void CEnemyCurve::Draw(void)
{
	//CScene2Dの描画処理
	CEnemy::Draw();
}

//=============================================================================
//
// 敵蛇行移動の処理
//
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemyMeandering::m_pTexture = NULL;
//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// 敵のコンストラクタ
//=============================================================================
CEnemyMeandering::CEnemyMeandering() : CEnemy()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//=============================================================================
// 敵のデストラクタ
//=============================================================================
CEnemyMeandering::~CEnemyMeandering()
{

}

//=============================================================================
//敵の生成
//=============================================================================
CEnemyMeandering *CEnemyMeandering::Create(D3DXVECTOR3 pos, float fWight, float fHeight, CEnemyMeandering::MEANDERING meandering)
{
	CEnemyMeandering *pEnemy = NULL;
	if (pEnemy == NULL)
	{
		pEnemy = new CEnemyMeandering;			//シーンの動的確保
		pEnemy->SetPos(pos);					//位置の代入
		pEnemy->SetWidth(fWight);				//幅の代入
		pEnemy->SetHeight(fHeight);				//高さの代入
		pEnemy->m_meandering = meandering;		//状態の代入
		pEnemy->Init();							//初期化処理
		pEnemy->BindTexture(m_pTexture);
	}
	return pEnemy;								//値を返す
}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CEnemyMeandering::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy004.png",
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CEnemyMeandering::Unload(void)
{
	m_pTexture->Release();
	m_pTexture = NULL;
}


//=============================================================================
// 敵の初期化処理
//=============================================================================
HRESULT CEnemyMeandering::Init(void)
{
	//CScene2Dの初期化処理
	CEnemy::Init();
	m_move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	m_nCounter = 0;
	return S_OK;
}

//=============================================================================
// 敵の終了処理
//=============================================================================
void CEnemyMeandering::Uninit(void)
{
	//CScene2Dの終了処理
	CEnemy::Uninit();
}

//=============================================================================
// 敵の更新処理
//=============================================================================
void CEnemyMeandering::Update(void)
{

	D3DXVECTOR3 pos;			//位置

	pos = GetPos();				//位置の取得
	if (pos.y >= 0)
	{
		m_nCounter++;
	}
	switch (m_meandering)
	{
	case MEANDERING_R:
		if (m_nCounter < 50)
		{
			m_move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
		}
		else if (m_nCounter > 50)
		{
			m_move = D3DXVECTOR3(sinf(D3DX_PI*(m_nCounter - 50.0f) / 160.0f)*2.0f,1.0f, 0.0f);
		}
		else if (m_nCounter > 200)
		{
			//m_move = D3DXVECTOR3(sinf(-D3DX_PI*(m_nCounter) / -160.0f)*3.0f, sinf(D3DX_PI*(m_nCounter - 120) / 80.0f)*3.0f, 0.0f);
		}
		break;

	case MEANDERING_L:
		if (m_nCounter < 50)
		{
			m_move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
		}
		if (m_nCounter > 50)
		{
			m_move = D3DXVECTOR3(sinf(-D3DX_PI*(m_nCounter - 50.0f) / 160.0f)*2.5f,1.0f, 0.0f);
		}
		if (m_nCounter >= 350)
		{
			//m_move = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
		}
	}
	pos += m_move;
	//位置の設置
	SetVtxPos(pos);

	CEnemy::Update();
	CEnemy::Collision();
}

//=============================================================================
// 敵の描画処理
//=============================================================================
void CEnemyMeandering::Draw(void)
{
	//CScene2Dの描画処理
	CEnemy::Draw();
}

//=============================================================================
//
// 敵追尾移動の処理
//
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemyTracking::m_pTexture = NULL;
//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// 敵のコンストラクタ
//=============================================================================
CEnemyTracking::CEnemyTracking() : CEnemy()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//=============================================================================
// 敵のデストラクタ
//=============================================================================
CEnemyTracking::~CEnemyTracking()
{

}

//=============================================================================
//敵の生成
//=============================================================================
CEnemyTracking *CEnemyTracking::Create(D3DXVECTOR3 pos, float fWight, float fHeight)
{
	CEnemyTracking *pEnemy = NULL;
	if (pEnemy == NULL)
	{
		pEnemy = new CEnemyTracking;			//シーンの動的確保
		pEnemy->SetPos(pos);					//位置の代入
		pEnemy->SetWidth(fWight);				//幅の代入
		pEnemy->SetHeight(fHeight);				//高さの代入
		pEnemy->Init();							//初期化処理
		pEnemy->BindTexture(m_pTexture);
	}
	return pEnemy;								//値を返す
}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CEnemyTracking::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy003.png",
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CEnemyTracking::Unload(void)
{
	m_pTexture->Release();
	m_pTexture = NULL;
}


//=============================================================================
// 敵の初期化処理
//=============================================================================
HRESULT CEnemyTracking::Init(void)
{
	//CScene2Dの初期化処理
	CEnemy::Init();
	m_move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	m_nCounter = 0;
	return S_OK;
}

//=============================================================================
// 敵の終了処理
//=============================================================================
void CEnemyTracking::Uninit(void)
{
	//CScene2Dの終了処理
	CEnemy::Uninit();
}

//=============================================================================
// 敵の更新処理
//=============================================================================
void CEnemyTracking::Update(void)
{
	D3DXVECTOR3 pos;			//位置

	pos = GetPos();				//位置の取得
	if (pos.y >= 0)
	{
		m_nCounter++;
	}
	for (int nCntPrioty = 0; nCntPrioty < 6; nCntPrioty++)
	{//優先度の数だけ繰り返し
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{//Sceneの4番だけ繰り返し
			CScene2D *pScene;												//シーンのポインタ
			pScene = (CScene2D*)CScene::GetScene(nCntPrioty, nCntScene);	//シーンの取得
			if (pScene != NULL)
			{//SceneがNULLでない場合
				CScene::OBJTYPE objType;		//種類
				objType = pScene->GetObjType();	//種類の取得
				if (objType == OBJTYPE_PLAYER)
				{//プレイヤーの場合
					D3DXVECTOR3 PlayerPos;					//プレイヤーの位置
					CPlayer *pPlayer = (CPlayer*)pScene;	//プレイヤーの動的確保
					PlayerPos = pPlayer->GetPos();			//位置の取得
					if (pPlayer != NULL)
					{//プレイヤーがNULLでない場合
							float fData = atan2f(PlayerPos.x - pos.x, PlayerPos.y - pos.y);
							if (m_nCounter == 200)
							{
								m_move = D3DXVECTOR3(sinf(fData) * 3.0f, cosf(fData) *3.0f, 0.0f);
							}
							//CBullet::Create(pos, D3DXVECTOR3(sinf(fData) * 7.0f, cosf(fData) *7.0f, 0.0f),10.0,10.0, CBullet::BULLETTYPE_ENEMY);
					}
				}
			}
		}
	}

	pos += m_move;
	//位置の設置
	SetVtxPos(pos);

	CEnemy::Update();
	CEnemy::Collision();
}

//=============================================================================
// 敵の描画処理
//=============================================================================
void CEnemyTracking::Draw(void)
{
	//CScene2Dの描画処理
	CEnemy::Draw();
}

//=============================================================================
//
// 人間の処理
//
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemyHuman::m_pTexture = NULL;
//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// 敵のコンストラクタ
//=============================================================================
CEnemyHuman::CEnemyHuman() : CEnemy(2)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//=============================================================================
// 敵のデストラクタ
//=============================================================================
CEnemyHuman::~CEnemyHuman()
{

}

//=============================================================================
//敵の生成
//=============================================================================
CEnemyHuman *CEnemyHuman::Create(D3DXVECTOR3 pos, float fWight, float fHeight)
{
	CEnemyHuman *pEnemy = NULL;
	if (pEnemy == NULL)
	{
		pEnemy = new CEnemyHuman;			//シーンの動的確保
		pEnemy->SetPos(pos);				//位置の代入
		pEnemy->SetWidth(fWight);			//幅の代入
		pEnemy->SetHeight(fHeight);			//高さの代入
		pEnemy->Init();						//初期化処理
		pEnemy->BindTexture(m_pTexture);
	}
	return pEnemy;					//値を返す
}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CEnemyHuman::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME010,
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CEnemyHuman::Unload(void)
{
	m_pTexture->Release();
	m_pTexture = NULL;
}


//=============================================================================
// 敵の初期化処理
//=============================================================================
HRESULT CEnemyHuman::Init(void)
{
	//CScene2Dの初期化処理
	CEnemy::Init();
	m_move = D3DXVECTOR3(0.0f, 0.25f, 0.0f);
	m_nCounter = 0;
	SetObjType(OBJTYPE_HUMAN);
	return S_OK;
}

//=============================================================================
// 敵の終了処理
//=============================================================================
void CEnemyHuman::Uninit(void)
{
	//CScene2Dの終了処理
	CEnemy::Uninit();
}

//=============================================================================
// 敵の更新処理
//=============================================================================
void CEnemyHuman::Update(void)
{
	D3DXVECTOR3 pos;			//位置

	pos = GetPos();				//位置の取得

	if (pos.y >= 0)
	{
		for (int nCntPrioty = 0; nCntPrioty < 6; nCntPrioty++)
		{//優先度の数だけ繰り返し
			for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
			{//Sceneの4番だけ繰り返し
				CScene2D *pScene;												//シーンのポインタ
				pScene = (CScene2D*)CScene::GetScene(nCntPrioty, nCntScene);	//シーンの取得
				if (pScene != NULL)
				{//SceneがNULLでない場合
					CScene::OBJTYPE objType;		//種類
					objType = pScene->GetObjType();	//種類の取得
					if (objType == OBJTYPE_PLAYER)
					{//プレイヤーの場合
						D3DXVECTOR3 PlayerPos;					//プレイヤーの位置
						CPlayer *pPlayer = (CPlayer*)pScene;	//プレイヤーの動的確保
						PlayerPos = pPlayer->GetPos();			//位置の取得
						if (pPlayer != NULL)
						{//プレイヤーがNULLでない場合
							float fData = atan2f(PlayerPos.x - pos.x, PlayerPos.y - pos.y);
							//弾の生成
							if (rand() % 100 == 0)
							{
								CBullet::Create(pos, D3DXVECTOR3(sinf(fData) * 7.0f, cosf(fData) *7.0f, 0.0f), 20.0f, 20.0f, CBullet::BULLETTYPE_ENEMY);
							}
						}
					}
				}
			}
		}
	}
	pos += m_move;
	//位置の設置
	SetVtxPos(pos);
}

//=============================================================================
// 敵の描画処理
//=============================================================================
void CEnemyHuman::Draw(void)
{
	//CScene2Dの描画処理
	CEnemy::Draw();
}

//=============================================================================
//
// ボスの処理
//
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemyBoss::m_pTexture = NULL;

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// 敵のコンストラクタ
//=============================================================================
CEnemyBoss::CEnemyBoss() : CEnemy()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//=============================================================================
// 敵のデストラクタ
//=============================================================================
CEnemyBoss::~CEnemyBoss()
{

}

//=============================================================================
//敵の生成
//=============================================================================
CEnemyBoss *CEnemyBoss::Create(D3DXVECTOR3 pos, float fWight, float fHeight)
{
	CEnemyBoss *pEnemy = NULL;
	if (pEnemy == NULL)
	{
		pEnemy = new CEnemyBoss;			//シーンの動的確保
		pEnemy->SetPos(pos);				//位置の代入
		pEnemy->SetWidth(fWight);			//幅の代入
		pEnemy->SetHeight(fHeight);			//高さの代入
		pEnemy->Init();						//初期化処理
		pEnemy->BindTexture(m_pTexture);
	}
	return pEnemy;					//値を返す
}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CEnemyBoss::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy005.png",
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CEnemyBoss::Unload(void)
{
	m_pTexture->Release();
	m_pTexture = NULL;
}


//=============================================================================
// 敵の初期化処理
//=============================================================================
HRESULT CEnemyBoss::Init(void)
{
	//CScene2Dの初期化処理
	CEnemy::Init();
	m_move = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_nCounter = 0;
	m_nLife = 100;
	SetObjType(OBJTYPE_BOSS);
	return S_OK;
}

//=============================================================================
// 敵の終了処理
//=============================================================================
void CEnemyBoss::Uninit(void)
{
	//CScene2Dの終了処理
	CEnemy::Uninit();
}

//=============================================================================
// 敵の更新処理
//=============================================================================
void CEnemyBoss::Update(void)
{
	D3DXVECTOR3 pos;			//位置
	pos = GetPos();				//位置の取得
	m_nCounter++;
	if (m_nCounter >= 250)
	{
		CBarrier::BARRIER_STATE pBarrierState;	//バリアの状態
		pBarrierState = CBarrier::GetState();	//バリアの取得

		for (int nCntPrioty = 0; nCntPrioty < NUM_PRIORITY; nCntPrioty++)
		{//優先度の数だけ繰り返し
			for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
			{//Sceneの数だけ繰り返し
				CScene2D *pScene;												//シーンのポインタ
				pScene = (CScene2D*)CScene::GetScene(nCntPrioty, nCntScene);	//シーンの取得
				if (pScene != NULL)
				{
					CScene::OBJTYPE objType;		//種類
					objType = pScene->GetObjType();	//種類の取得

					if (objType == OBJTYPE_PLAYER)
					{//プレイヤーの場合
						D3DXVECTOR3 PlayerPos;					//敵の位置
						PlayerPos = pScene->GetPos();			//位置の取得
						CPlayer *pPlayer = (CPlayer*)pScene;
						if (pos.x > PlayerPos.x - BULLET_HIT
							&& pos.x < PlayerPos.x + BULLET_HIT
							&& pos.y <  PlayerPos.y + BULLET_HIT
							&& pos.y > PlayerPos.y - BULLET_HIT)
						{
							if (pBarrierState == CBarrier::STATE_NONE)
							{//バリアを使用していない場合
							 //ダメージ処理
								pPlayer->HitDamage(3);
							}
						}
					}
					else if (objType == OBJTYPE_BARRIER)
					{//バリアの場合
						D3DXVECTOR3 BarrierPos;					//敵の位置
						BarrierPos = pScene->GetPos();			//位置の取得
						CBarrier::BARRIER_STATE pBarrierState;
						pBarrierState = CBarrier::GetState();

						if (pos.x + 30.0f > BarrierPos.x - BULLET_HIT
							&& pos.x - 30.0f < BarrierPos.x + BULLET_HIT
							&& pos.y - 30.0f <  BarrierPos.y + BULLET_HIT
							&& pos.y + 30.0f > BarrierPos.y - BULLET_HIT)
						{

							if (pBarrierState == CBarrier::STATE_USE)
							{//バリアが使用されている場合
							}
						}
					}
				}
			}
		}
		//敵の移動処理
		m_move = D3DXVECTOR3(sinf(D3DX_PI*(m_nCounter) / -160.0f)*5.0f, sinf(D3DX_PI*(m_nCounter - 120) / 80.0f)*3.0f, 0.0f);
		if (rand() % 50 == 0)
		{
			//弾の生成
			CBullet::Create(D3DXVECTOR3(pos.x, pos.y + 20, pos.z), D3DXVECTOR3(0.0f, 10.0f, 0.0f), 20.0f, 20.0f, CBullet::BULLETTYPE_ENEMY);

			//CDefense::Create(D3DXVECTOR3(pos.x,pos.y,pos.z), 10.0f, 10.0f);
		}
	}

	pos += m_move;
	//位置の設置
	SetVtxPos(pos);
	CEnemy::Update();
}

//=============================================================================
// 敵の描画処理
//=============================================================================
void CEnemyBoss::Draw(void)
{
	//CScene2Dの描画処理
	CEnemy::Draw();
}