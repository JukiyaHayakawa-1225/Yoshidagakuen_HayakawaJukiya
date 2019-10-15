//=============================================================================
//
// ゲーム処理 [manager.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "game.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "player.h"
#include "bullet.h"
#include "explotion.h"
#include "bg.h"
#include "enemy.h"
#include "beans.h"
#include "number.h"
#include "score.h"
#include "life.h"
#include "effect.h"
#include "stock.h"
#include "fade.h"
#include "barrier.h"
#include "avatar.h"
#include "skill.h"
#include "logo.h"
#include "pause.h"
#include "cloud.h"
#include "frame.h"
#include "defense.h"
#include "particle.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CScore *CGame::m_pScore = NULL;
CPlayer *CGame::m_pPlayer = NULL;
CEnemy *CGame::m_pEnemy = NULL;
CGame::GAME_STATE CGame::m_state = STATE_START;

//=============================================================================
// ゲームのコンストラクタ
//=============================================================================
CGame::CGame()
{

}

//=============================================================================
// ゲームのデストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ゲームのデストラクタ
//=============================================================================
CGame *CGame::Create()
{
	CGame *pGame = NULL;
	if (pGame == NULL)
	{
		pGame = new CGame;	//動的確保
		if (pGame != NULL)
		{
			pGame->Init();	//初期化処理
		}
	}
	return pGame;
}

//=============================================================================
// ゲームの初期化処理
//=============================================================================
HRESULT CGame::Init(void)
{
	m_nSpawnCloud = 0;
	m_nMainTimer = 0;
	m_nSpawnHuman = 0;
	m_state = STATE_START;

	//ゲーム背景の読み込み
	CBgGame::Load();
	//数字の読み込み
	CNumber::Load();
	//プレイヤーの読み込み
	CPlayer::Load();
	//敵の読み込み
	CEnemySlide::Load();
	CEnemyCurve::Load();
	CEnemyMeandering::Load();
	CEnemyTracking::Load();
	CEnemyHuman::Load();
	CEnemyBoss::Load();
	//敵の防衛の読み込み
	CDefense::Load();

	//弾の読み込み
	CBullet::Load();
	//爆発の読み込み
	CExplotion::Load();
	//豆の読み込み
	CBeans::Load();
	//ライフの読み込み
	CLife::Load();
	//残機の読み込み
	CStock::Load();
	//エフェクトの読み込み
	CEffect::Load();
	//パーティクルの読み込み
	CParticle::Load();
	//バリア読み込み
	CBarrier::Load();
	//分身の読み込み
	CAvatar::Load();
	//スキルの読み込み
	CSkill::Load();
	//UI背景の読み込み
	CBgUI::Load();
	//ポーズの読み込み
	CPause::Load();
	//雲の読み込み
	CCloud::Load();
	//フレームの読み込み
	CFrame::Load();

	//ターゲットの読み込み
	CTargetLogo::Load();

	//フレームの生成
	CFrame::Create(D3DXVECTOR3(145.0f, 90.0f ,0.0f), 145.0f, 90, CFrame::FRAME_SKILL);
	CFrame::Create(D3DXVECTOR3(145.0f, 270.0f, 0.0f), 145.0f, 90, CFrame::FRAME_STOCK);
	CFrame::Create(D3DXVECTOR3(145.0f, 450.0f, 0.0f), 145.0f, 90, CFrame::FRAME_LIFE);
	CFrame::Create(D3DXVECTOR3(145.0f, 630.0f, 0.0f), 145.0f, 90, CFrame::FRAME_SCORE);

	//背景の生成
	CBgGame::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 150, SCREEN_HEIGHT / 2, 0.0f), 500.0f, SCREEN_HEIGHT / 2);

	//スコアの生成
	m_pScore = CScore::Create(D3DXVECTOR3(260.0f, 665.0f, 0.0f),23.0f, 23.0f);

	//残機の生成
	CStock::Create(D3DXVECTOR3(70.0f, 305.0f, 0.0f), 35.0f, 35.0f);

	//ライフの生成
	CLife::Create(D3DXVECTOR3(70.0f, 485.0f, 0.0f),35.0f, 35.0f);

	//スキルの生成
	CSkill::Create(D3DXVECTOR3(47.5f, 135.0f, 0.0f), 30.0f, 30.0f);

	//プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2,650.0f, 0.0f), PLAYER_SIZE, PLAYER_SIZE,CPlayer::STATE_NOMAL);
	return S_OK;
}

//=============================================================================
// ゲームの終了処理
//=============================================================================
void CGame::Uninit(void)
{
	//ターゲットの破棄
	CTargetLogo::Unload();

	//フレームの破棄
	CFrame::Unload();
	//雲の破棄
	CCloud::Unload();

	//ポーズの破棄
	CPause::Unload();

	//スキルの破棄
	CSkill::Unload();

	//分身の破棄
	CAvatar::Unload();

	//背景の破棄
	CBgGame::Unload();

	//数字の破棄
	CNumber::Unload();

	//ライフの破棄
	CLife::Unload();

	//残機の破棄
	CStock::Unload();

	//プレイヤーの破棄
	CPlayer::Unload();

	//敵の防衛の破棄
	CDefense::Unload();

	//敵の破棄
	CEnemySlide::Unload();
	//敵の破棄
	CEnemyCurve::Unload();
	//敵の破棄
	CEnemyMeandering::Unload();
	//敵の破棄
	CEnemyHuman::Unload();
	//敵の破棄
	CEnemyBoss::Unload();
	//弾の破棄
	CBullet::Unload();

	//爆発の破棄
	CExplotion::Unload();

	//豆の破棄
	CBeans::Unload();

	//パーティクルの破棄
	CParticle::Unload();

	//エフェクトの破棄
	CEffect::Unload();

	//バリアの破棄
	CBarrier::Unload();

	//UI背景の破棄
	CBgUI::Unload();

	m_nSpawnCloud = 0;
	m_nMainTimer = 0;
	m_nSpawnHuman = 0;

	CScene::ReleaseAll();
}

//=============================================================================
// ゲームの更新処理
//=============================================================================
void CGame::Update(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();
	//フェードの取得
	CFade::FADE fade;
	CFade *pFade = NULL;
	fade = pFade->GetFade();
	if (CRenderer::GetPause() == false)
	{
		CEnemyBoss *pBoss = NULL;
		if (pBoss == NULL)
		{
			if (m_state == STATE_END)
			{
				CResult *pResult = NULL;
				CManager *pManager = NULL;
				if (pResult == NULL)
				{
					pResult->SetResult(pResult->RESULT_CLEAR);
				}
				CFade::Create(CManager::MODE_RESULT, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
				if (pManager == NULL && fade == CFade::FADE_NONE)
				{
					pManager->SetGameScore(m_pScore->GetScore());
				}
			}
		}
		if (m_pPlayer->GetState() == m_pPlayer->STATE_END)
		{
			CResult *pResult = NULL;
			CManager *pManager = NULL;
			if (pResult == NULL)
			{
				pResult->SetResult(pResult->RESULT_GAMEOVER);
			}
			CFade::Create(CManager::MODE_RESULT, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
			if (pManager == NULL && fade == CFade::FADE_NONE)
			{
				pManager->SetGameScore(m_pScore->GetScore());
			}
		}

		//生成処理
		Spawn();
	}
}

//=============================================================================
// ゲームの描画処理
//=============================================================================
void CGame::Draw(void)
{
}
//=============================================================================
// 生成処理
//=============================================================================
void CGame::Spawn(void)
{
	//言って時間経過ごとに敵出現
	m_nMainTimer ++;
	if (m_nMainTimer == 50)
	{
		SetEnemyPattern(0);
	}
	else if (m_nMainTimer == 400)
	{
		SetEnemyPattern(1);
	}
	else if (m_nMainTimer == 800)
	{
		SetEnemyPattern(2);
	}
	else if (m_nMainTimer == 1200)
	{
		SetEnemyPattern(3);
	}
	else if (m_nMainTimer == 1600)
	{
		SetEnemyPattern(0);
	}
	else if(D3DSPSM_SIGNNEG == 1800)
	{
		SetEnemyPattern(1);
	}
	else if (m_nMainTimer == 2200)
	{
		SetEnemyPattern(2);
	}
	else if (m_nMainTimer == 2400)
	{
		SetEnemyPattern(0);

	}
	else if (m_nMainTimer == 2600)
	{
		SetEnemyPattern(3);
	}
	else if (m_nMainTimer == 2600)
	{
		SetEnemyPattern(3);
	}
	else if (m_nMainTimer == 3000)
	{
		SetEnemyPattern(5);
	}
	else if (m_nMainTimer == 3400)
	{
		SetEnemyPattern(1);
	}
	else if (m_nMainTimer == 3600)
	{
		SetEnemyPattern(0);
	}
	else if (m_nMainTimer == 3800)
	{
		SetEnemyPattern(3);
	}
	else if (m_nMainTimer == 4400)
	{
		SetEnemyPattern(0);
	}
	else if (m_nMainTimer == 4600)
	{
		SetEnemyPattern(0);
	}
	else if (m_nMainTimer == 4800)
	{
		SetEnemyPattern(5);
	}
	else if (m_nMainTimer == 5000)
	{
		SetEnemyPattern(4);
	}
	if (m_nMainTimer < 5000)
	{
		float fRandPos = rand() % 900 + 400.0f;		//位置をランダム
		int	  nNumber = rand() % 2 + 1;				//種類のランダム
		m_nSpawnCloud++;							//スポーンのタイマー
		m_nSpawnHuman++;
		//雲の生成
		if (m_nSpawnCloud >= 250)
		{
			if (nNumber == 1)
			{
				CCloud::Create(D3DXVECTOR3(fRandPos, -30.0f, 0.0f), 50.0f, 50.0f, CCloud::CLOUD_BEANS);
			}
			else
			{
				CCloud::Create(D3DXVECTOR3(fRandPos, -30.0f, 0.0f), 50.0f, 50.0f, CCloud::CLOUD_NONE);
			}
		}
		if (m_nSpawnCloud >= 250)
		{
			m_nSpawnCloud = 0;
		}

		//人間の生成
		if (m_nSpawnHuman >= 1500)
		{
			CEnemyHuman::Create(D3DXVECTOR3(fRandPos, -30.0f, 0.0f), 50.0f, 50.0f);
		}
		if (m_nSpawnHuman >= 1500)
		{
			m_nSpawnHuman = 0;
		}
	}

}
//=============================================================================
// スコアの取得処理
//=============================================================================
CScore *CGame::GetScore(void)
{
	return m_pScore;
}

//=============================================================================
// プレイヤーの取得処理
//=============================================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// 敵のパターン設置処理
//=============================================================================
void CGame::SetEnemyPattern(int pattern)
{
	switch (pattern)
	{
	case 0:
		m_pEnemy = CEnemySlide::Create(D3DXVECTOR3(350, -30, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemySlide::SLIDE_R);
		m_pEnemy = CEnemySlide::Create(D3DXVECTOR3(SCREEN_WIDTH - 50, -30, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemySlide::SLIDE_L);
		m_pEnemy = CEnemySlide::Create(D3DXVECTOR3(350, -180, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemySlide::SLIDE_R);
		m_pEnemy = CEnemySlide::Create(D3DXVECTOR3(SCREEN_WIDTH - 50, -180, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemySlide::SLIDE_L);
		m_pEnemy = CEnemySlide::Create(D3DXVECTOR3(350, -310, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemySlide::SLIDE_R);
		m_pEnemy = CEnemySlide::Create(D3DXVECTOR3(SCREEN_WIDTH - 50, -310, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemySlide::SLIDE_L);
		break;
	case 1:
		m_pEnemy = CEnemyCurve::Create(D3DXVECTOR3(840, -30, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemyCurve::CURVE_R);
		m_pEnemy = CEnemyCurve::Create(D3DXVECTOR3(740, -30, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemyCurve::CURVE_L);
		m_pEnemy = CEnemyCurve::Create(D3DXVECTOR3(840, -180, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemyCurve::CURVE_R);
		m_pEnemy = CEnemyCurve::Create(D3DXVECTOR3(740, -180, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemyCurve::CURVE_L);
		m_pEnemy = CEnemyCurve::Create(D3DXVECTOR3(840, -330, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemyCurve::CURVE_R);
		m_pEnemy = CEnemyCurve::Create(D3DXVECTOR3(740, -330, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemyCurve::CURVE_L);
		break;
	case 2:
		m_pEnemy = CEnemyMeandering::Create(D3DXVECTOR3(840, -30, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemyMeandering::MEANDERING_R);
		m_pEnemy = CEnemyMeandering::Create(D3DXVECTOR3(740, -30, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemyMeandering::MEANDERING_L);
		m_pEnemy = CEnemyMeandering::Create(D3DXVECTOR3(840, -180, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemyMeandering::MEANDERING_R);
		m_pEnemy = CEnemyMeandering::Create(D3DXVECTOR3(740, -180, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemyMeandering::MEANDERING_L);
		m_pEnemy = CEnemyMeandering::Create(D3DXVECTOR3(840, -330, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemyMeandering::MEANDERING_R);
		m_pEnemy = CEnemyMeandering::Create(D3DXVECTOR3(740, -330, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemyMeandering::MEANDERING_L);

		break;
	case 3:
		m_pEnemy = CEnemyTracking::Create(D3DXVECTOR3(840, -30, 0), ENEMY_SIZE, ENEMY_SIZE);
		m_pEnemy = CEnemyTracking::Create(D3DXVECTOR3(740, -30, 0), ENEMY_SIZE, ENEMY_SIZE);
		m_pEnemy = CEnemyTracking::Create(D3DXVECTOR3(840, -180, 0), ENEMY_SIZE, ENEMY_SIZE);
		m_pEnemy = CEnemyTracking::Create(D3DXVECTOR3(740, -180, 0), ENEMY_SIZE, ENEMY_SIZE);
		m_pEnemy = CEnemyTracking::Create(D3DXVECTOR3(840, -330, 0), ENEMY_SIZE, ENEMY_SIZE);
		m_pEnemy = CEnemyTracking::Create(D3DXVECTOR3(740, -330, 0), ENEMY_SIZE, ENEMY_SIZE);
		break;
	case 4:
		m_pEnemy = CEnemyBoss::Create(D3DXVECTOR3(790, -30.0f, 0.0f), BOSS_SIZE, BOSS_SIZE);
		break;
	case 5:
		m_pEnemy = CEnemySlide::Create(D3DXVECTOR3(840, -30, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemySlide::SLIDE_R);
		m_pEnemy = CEnemySlide::Create(D3DXVECTOR3(740, -30, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemySlide::SLIDE_L);
		m_pEnemy = CEnemySlide::Create(D3DXVECTOR3(840, -180, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemySlide::SLIDE_R);
		m_pEnemy = CEnemySlide::Create(D3DXVECTOR3(740, -180, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemySlide::SLIDE_L);
		m_pEnemy = CEnemySlide::Create(D3DXVECTOR3(840, -310, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemySlide::SLIDE_R);
		m_pEnemy = CEnemySlide::Create(D3DXVECTOR3(740, -310, 0), ENEMY_SIZE, ENEMY_SIZE, CEnemySlide::SLIDE_L);
		break;
	}
}

//=============================================================================
// 敵の取得処理
//=============================================================================
CEnemy *CGame::GetEnemy(void)
{
	return m_pEnemy;
}
//=============================================================================
// ゲームの状態設置処理
//=============================================================================
void CGame::SetState(CGame::GAME_STATE state)
{
	m_state = state;
}
