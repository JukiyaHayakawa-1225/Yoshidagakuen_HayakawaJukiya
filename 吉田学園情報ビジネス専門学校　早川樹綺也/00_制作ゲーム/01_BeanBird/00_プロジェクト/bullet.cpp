//=============================================================================
//
// 弾の処理 [bullte.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "bullet.h"
#include "game.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "explotion.h"
#include "enemy.h"
#include "beans.h"
#include "score.h"
#include "effect.h"
#include "life.h"
#include "barrier.h"
#include "player.h"
#include "cloud.h"
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture[MAX_BULLET] = {};

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// 弾のコンストラクタ
//=============================================================================
CBullet::CBullet() : CScene2D()
{
	m_nLife = 0;								//体力初期値
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量初期値
	m_bulletType = BULLETTYPE_NONE;				//種類初期
}
//=============================================================================
// 弾のデストラクタ
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
//弾の生成
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos,D3DXVECTOR3 move, float fWight, float fHeight,BULLETTYPE type)
{
	CBullet *pBullet = NULL;

	if (pBullet == NULL)
	{
		pBullet = new CBullet;				//シーンの動的確保
		pBullet->SetPos(pos);				//位置の代入
		pBullet->SetWidth(fWight);			//幅の代入
		pBullet->SetHeight(fHeight);		//高さの代入
		pBullet->Init();					//初期化処理
		pBullet->m_move = move;				//移動量を代入
		pBullet->m_bulletType = type;		//種類の代入
		switch (type)
		{
		case BULLETTYPE_PLAYER_SALIVA:
			pBullet->BindTexture(m_pTexture[0]);	//テクスチャ
			break;
		case BULLETTYPE_PLAYER_SHIT:
			pBullet->BindTexture(m_pTexture[1]);	//テクスチャ
			break;
		case BULLETTYPE_ENEMY:
			pBullet->BindTexture(m_pTexture[2]);	//テクスチャ
			break;
		}
	}
	return pBullet;							//値を返す
}

//=============================================================================
// 弾のテクスチャ読み込み
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
		TEXTURE_NAME001,
		&m_pTexture[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\egg.png",
		&m_pTexture[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\humanbullet.png",
		&m_pTexture[2]);
	return S_OK;
}

//=============================================================================
// 弾のテクスチャ破棄
//=============================================================================
void CBullet::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < MAX_BULLET; nCntTexture++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCntTexture] != NULL)
		{
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture]= NULL;
		}
	}
}
//=============================================================================
// 弾の初期化処理
//=============================================================================
HRESULT CBullet::Init(void)
{
	//CScene2Dの初期化処理
	CScene2D::Init();
	//種類の設定
	SetObjType(OBJTYPE_BULLET);
	return S_OK;
}

//=============================================================================
// 弾の終了処理
//=============================================================================
void CBullet::Uninit(void)
{
	//CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 弾の更新処理
//=============================================================================
void CBullet::Update(void)
{
	//弾の当たり判定
	Collision();

	//CScene2Dの更新処理
	CScene2D::Update();
}

//=============================================================================
// 弾の描画処理
//=============================================================================
void CBullet::Draw(void)
{
	//CScene2Dの描画処理
	CScene2D::Draw();
}
//=============================================================================
// 弾の当たり判定
//=============================================================================
void CBullet::Collision(void)
{
	D3DXVECTOR3 pos;			//位置
	bool bDisappear = false;	//消滅してるかどうか

	pos = GetPos();				//位置の取得

	m_nLife++;					//体力の加算
	pos += m_move;				//座標の代入

	//位置の設置
	CScene2D::SetVtxPos(pos);

	switch (m_bulletType)
	{
	case BULLETTYPE_PLAYER_SALIVA:
		//エフェクトの生成
		CEffect::Create(pos, D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f), 20.0f, 20.0f);
		break;
	case BULLETTYPE_PLAYER_SHIT:
		//エフェクトの生成
		CEffect::Create(pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 20.0f, 20.0f);
		break;
	case BULLETTYPE_ENEMY:
		//エフェクトの生成
		CEffect::Create(pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 20.0f, 20.0f);
		break;
	}

	if (m_bulletType == BULLETTYPE_PLAYER_SALIVA || m_bulletType == BULLETTYPE_ENEMY)
	{
		if (pos.y < 0 || m_nLife > 40)
		{//画面外の場合
		 //終了処理
			bDisappear = true;

			//体力の初期化
			m_nLife = 0;

			//爆発の生成
			CExplotion::Create(pos, 50.0f, 50.0f);
		}
	}
	else if (m_bulletType == BULLETTYPE_PLAYER_SHIT)
	{
		if (pos.y < 0 || m_nLife > 20)
		{//画面外の場合
		 //終了処理
			bDisappear = true;

			//体力の初期化
			m_nLife = 0;

			//爆発の生成
			CExplotion::Create(pos, 50.0f, 50.0f);
		}
	}

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

				//=========================================================================
				// プレイヤー唾の弾処理
				//=========================================================================
				if (m_bulletType == BULLETTYPE_PLAYER_SALIVA)
				{//プレイヤーの弾の場合
					if (objType == OBJTYPE_ENEMY)
					{//当たったのが敵の場合

						CEnemy *pEnemy = (CEnemy*)pScene;
						D3DXVECTOR3 EnemyPos;			//敵の位置
						EnemyPos = pScene->GetPos();	//位置の取得
						if (pos.x > EnemyPos.x - ENEMY_SIZE
							&& pos.x < EnemyPos.x + ENEMY_SIZE
							&& pos.y <  EnemyPos.y + ENEMY_SIZE
							&& pos.y > EnemyPos.y - ENEMY_SIZE)
						{
							//敵のダメージ処理
							pEnemy->HitDamage(1, CEnemy::ENEMY_TYPE_NOMAL);

							//弾の終了処理
							bDisappear = true;
						}
					}
					else if (objType == OBJTYPE_BOSS)
					{//当たったのがボスの場合
						CEnemyBoss *pBoss = (CEnemyBoss*)pScene;
						D3DXVECTOR3 BossPos;			//ボスの位置
						BossPos = pScene->GetPos();		//位置の取得
						if (pos.x > BossPos.x - BOSS_SIZE
							&& pos.x < BossPos.x + BOSS_SIZE
							&& pos.y <  BossPos.y + BOSS_SIZE
							&& pos.y > BossPos.y - BOSS_SIZE)
						{
							//ボスのダメージ処理
							pBoss->HitDamage(1,CEnemy::ENEMY_TYPE_BOSS);

							//弾の終了処理
							bDisappear = true;
						}
					}
					else if (objType == OBJTYPE_CLOUD)
					{//当たったのが雲の場合
						D3DXVECTOR3 CloudPos;				//敵の位置
						CloudPos = pScene->GetPos();		//位置の取得
						CCloud *pCloud = (CCloud*)pScene;	//雲の動的確保

						if (pCloud != NULL && pCloud->GetCloud() == pCloud->CLOUD_BEANS)
						{
							if (pos.x > CloudPos.x - CLOUD_SIZE
								&& pos.x < CloudPos.x + CLOUD_SIZE
								&& pos.y <  CloudPos.y + CLOUD_SIZE
								&& pos.y > CloudPos.y - CLOUD_SIZE)
							{
								//雲の移動判定処理
								pCloud->HitCloud();

								//弾の終了処理
								bDisappear = true;

							}

						}
					}
					else if (objType == OBJTYPE_BEANS)
					{//当たったのが豆の場合
						D3DXVECTOR3 BeansPos;				//敵の位置
						BeansPos = pScene->GetPos();		//位置の取得
						CBeans *pBeans = (CBeans*)pScene;	//豆の動的確保
						CScore *pScore = CGame::GetScore();	//スコアの取得

						if (pBeans != NULL)
						{
							if (pos.x > BeansPos.x - BEANS_SIZE
								&& pos.x < BeansPos.x + BEANS_SIZE
								&& pos.y <  BeansPos.y + BEANS_SIZE
								&& pos.y > BeansPos.y - BEANS_SIZE)
							{
								//豆の移動判定処理
								pBeans->HitBeans(pos);

								//スコアの加算
								pScore->AddScore(100);

								//弾の終了処理
								bDisappear = true;

							}
						}
					}
				}
				//=========================================================================
				// プレイヤー糞の弾処理
				//=========================================================================

				else if (m_bulletType == BULLETTYPE_PLAYER_SHIT)
				{
					if (objType == OBJTYPE_HUMAN)
					{//当たったのが敵の場合

						CEnemy *pEnemy = (CEnemy*)pScene;
						D3DXVECTOR3 EnemyPos;			//敵の位置
						EnemyPos = pScene->GetPos();	//位置の取得
						if (pos.x > EnemyPos.x - ENEMY_SIZE
							&& pos.x < EnemyPos.x + ENEMY_SIZE
							&& pos.y <  EnemyPos.y + ENEMY_SIZE
							&& pos.y > EnemyPos.y - ENEMY_SIZE)
						{
							//敵のダメージ処理
							pEnemy->HitDamage(1,CEnemy::ENEMY_TYPE_NOMAL);

							//弾の終了処理
							bDisappear = true;
						}
					}
				}
				//=========================================================================
				// 敵の弾処理
				//=========================================================================
				else if (m_bulletType == BULLETTYPE_ENEMY)
				{//敵の弾の場合
					if (objType == OBJTYPE_PLAYER)
					{//当たったのがプレイヤーの場合
						D3DXVECTOR3 PlayerPos;					//敵の位置
						PlayerPos = pScene->GetPos();			//位置の取得
						CLife *pLife = (CLife*)pScene;			//ライフの動的確保
						CPlayer *pPlayer = (CPlayer*)pScene;	//プレイヤーの動的確保
						CBarrier::BARRIER_STATE pBarrierState;	//バリアの状態
						pBarrierState = CBarrier::GetState();	//バリアの取得処理

						if (pos.x > PlayerPos.x - PLAYER_SIZE
							&& pos.x < PlayerPos.x + PLAYER_SIZE
							&& pos.y <  PlayerPos.y + PLAYER_SIZE
							&& pos.y > PlayerPos.y - PLAYER_SIZE)
						{
							if (pBarrierState == CBarrier::STATE_NONE && pPlayer->GetState() == CPlayer::STATE_NOMAL)
							{//バリア使用状態ではないまたはプレイヤーが通常状態の場合
							 //プレイヤーのダメージ処理
								pPlayer->HitDamage(1);
								bDisappear = true;
							}
							else if (pBarrierState == CBarrier::STATE_USE)
							{
								bDisappear = true;
							}
						}
					}
					else if (objType == OBJTYPE_BARRIER)
					{//当たったのがバリアの場合
					 //CEffect::Create(pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 12.0f);
						D3DXVECTOR3 BarrierPos;					//敵の位置
						BarrierPos = pScene->GetPos();			//位置の取得
						CLife *pLife = (CLife*)pScene;			//プレイヤーの動的確保
						CBarrier::BARRIER_STATE pBarrierState;	//バリアの状態
						pBarrierState = CBarrier::GetState();	//バリアの取得

						if (pos.x + 30.0f > BarrierPos.x - BARRIER_SIZE
							&& pos.x - 30.0f < BarrierPos.x + BARRIER_SIZE
							&& pos.y - 30.0f <  BarrierPos.y + BARRIER_SIZE
							&& pos.y + 30.0f > BarrierPos.y - BARRIER_SIZE)
						{
							if (pBarrierState == CBarrier::STATE_USE)
							{//バリアが使用されている状態の場合
								bDisappear = true;
							}
						}
					}
				}
			}
		}
	}
	if (bDisappear == true)
	{//弾がtrueになった場合
	 //終了処理
		Uninit();
	}
}