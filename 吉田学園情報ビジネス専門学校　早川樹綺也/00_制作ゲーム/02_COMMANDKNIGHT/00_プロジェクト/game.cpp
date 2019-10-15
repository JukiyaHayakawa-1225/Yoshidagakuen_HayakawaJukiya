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
#include "meshfield.h"
#include "debugproc.h"
#include "fade.h"
#include "object.h"
#include "command.h"
#include "enemy.h"
#include "map.h"
#include "CommandUI.h"
#include "number.h"
#include "timer.h"
#include "pause.h"
#include "score.h"
#include "ui.h"
#include "status_number.h"
#include "particle.h"
#include "billboardgauge.h"
#include "logo.h"
#include "bullet.h"
#include "icecircle.h"
#include "icicle.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CPlayer			*CGame::m_pPlayer = NULL;				//プレイヤーのポインタ情報
CMeshField		*CGame::m_pMeshField = NULL;			//メッシュフィールドのポインタ情報
CCommand		*CGame::m_pCommand = NULL;				//コマンドのポインタ情報
CMap			*CGame::m_pMap = NULL;					//マップのポインタ情報
CTimer			*CGame::m_pTimer = NULL;				//タイマーのポインタ情報
CScore			*CGame::m_pScore = NULL;				//スコアのポインタ情報
CStatus_Number	*CGame::m_pStatuNumber = NULL;			//ステータス数字のポインタ情報
CUI_LifeGauge	*CGame::m_pUI_LifeGauge = NULL;			//UIのポインタ情報
CUI_SkillGauge	*CGame::m_pUI_SkillGauge = NULL;			//UIのポインタ情報
CUI_DamageGauge	*CGame::m_pUI_DamageGauge = NULL;			//UIのポインタ情報
CBillboardNumber	*CGame::m_pBillboard_Number = NULL;

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
	m_pPlayer = NULL;		//プレイヤーを初期化
	m_pMeshField = NULL;	//メッシュフィールドを初期化
	m_pMap = NULL;			//マップを初期化
	m_pTimer = NULL;		//タイマーを初期化
	m_pScore = NULL;		//スコアを初期化
}

//=============================================================================
// ゲームのデストラクタ
//=============================================================================
CGame *CGame::Create()
{
	CGame *pGame = NULL;	//ゲームのポインタ
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
	//マップを生成
	m_pMap = CMap::Create();

	//数字の読み込み
	CNumber::Load();

	//コマンドの読み込み
	CCommand::Load();

	//コマンドUIの読み込み
	CCommandUI::Load();

	//ゲージ体力UIの読み込み
	CUI_LifeGauge::Load();

	//ゲージとくぎUIの読み込み
	CUI_SkillGauge::Load();

	//ゲージダメージUIの読み込み
	CUI_DamageGauge::Load();

	//ゲージベースUIの読み込み
	CUI_BaseGauge::Load();

	//パーティクルの読み込み
	CParticle::Load();

	//ポーズの読み込み
	CPause::Load();

	//ビルボード体力ゲージの読み込み
	CBillboardLifeGauge::Load();

	//パーティクルスキル２の読み込み
	CSkill2Particle::Load();

	//パーティクルダメージの読み込み
	CDamageParticle::Load();

	//雪の読み込み
	CSnowParticle::Load();

	//噴水の読み込み
	CFountainParticle::Load();
	//HPロゴの読み込み
	CHPLogo::Load();

	//MPロゴの読み込み
	CMPLogo::Load();

	//弾の読み込み
	CBullet::Load();

	//円の氷の読み込み
	CIceCircle::Load();

	//氷柱の読み込み
	CIcicle::Load();

	//HPロゴの生成
	CHPLogo::Create(D3DXVECTOR3(SCREEN_WIDTH - 350.0f, 600.0f, 0.0f), 20.0f, 20.0f);

	//MPロゴの生成
	CMPLogo::Create(D3DXVECTOR3(SCREEN_WIDTH - 350.0f, 650.0f, 0.0f), 20.0f, 20.0f);

	//プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(600.0f, 0.0f, -1000.0f));

	//タイマーの生成
	m_pTimer = CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 50.0f, 75.0f, 0.0f), 40.0f, 40.0f);

	//スコアの生成
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 50.0f, 75.0f, 0.0f), 40.0f, 40.0f,5);

	//ステータス数字の生成
	m_pStatuNumber = CStatus_Number::Create(D3DXVECTOR3(SCREEN_WIDTH - 150.0f, SCREEN_HEIGHT - 120.0f, 0.0f), 15.0f, 15.0f);

	//コマンドの生成
	m_pCommand = CCommand::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	//コマンドUIの生成
	CCommandUI::Create();

	//ゲージ体力UIの生成
	m_pUI_LifeGauge = CUI_LifeGauge::Create();

	//ゲージとくぎUIの生成
	m_pUI_SkillGauge = CUI_SkillGauge::Create();

	//ゲージダメージUIの生成
	m_pUI_DamageGauge = CUI_DamageGauge::Create();

	//ゲージベースUIの生成
	CUI_BaseGauge::Create();

	//敵の生成
	CEnemyBird::Create(D3DXVECTOR3(600.0f, 0.0f, -600.0f));
	CEnemyBird::Create(D3DXVECTOR3(650.0f, 0.0f, -600.0f));
	CEnemyBird::Create(D3DXVECTOR3(550.0f, 0.0f, -600.0f));
	CEnemyBird::Create(D3DXVECTOR3(600.0f, 0.0f, -650.0f));
	CEnemyBird::Create(D3DXVECTOR3(100.0f, 0.0f, -650.0f));

	CEnemyBird::Create(D3DXVECTOR3(100.0f, 0.0f, -900.0f));
	CEnemyBird::Create(D3DXVECTOR3(200.0f, 0.0f, -1000.0f));
	CEnemyBird::Create(D3DXVECTOR3(1100.0f, 0.0f, -900.0f));
	CEnemyBird::Create(D3DXVECTOR3(900.0f, 0.0f, -1000.0f));

	CEnemyBird::Create(D3DXVECTOR3(100.0f, 0.0f, -400.0f));
	CEnemyBird::Create(D3DXVECTOR3(1000.0f, 0.0f, -700.0f));
	CEnemyBird::Create(D3DXVECTOR3(600.0f, 0.0f, -100.0f));
	CEnemyBird::Create(D3DXVECTOR3(100.0f, 0.0f, -100.0f));
	CEnemyBird::Create(D3DXVECTOR3(1000.0f, 0.0f, -500.0f));

	CEnemyFrog::Create(D3DXVECTOR3(100.0f, 0.0f, -1000.0f));
	CEnemyFrog::Create(D3DXVECTOR3(100.0f, 0.0f, -200.0f));
	CEnemyFrog::Create(D3DXVECTOR3(1000.0f, 0.0f, -1000.0f));
	CEnemyFrog::Create(D3DXVECTOR3(1000.0f, 0.0f, -200.0f));
	CEnemyFrog::Create(D3DXVECTOR3(550.0f, 0.0f, -100.0f));
	CEnemyFrog::Create(D3DXVECTOR3(650.0f, 0.0f, -100.0f));


	//メッシュフィールドの生成
	m_pMeshField = CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// ゲームの終了処理
//=============================================================================
void CGame::Uninit(void)
{
	//氷柱の破棄
	CIcicle::Unload();

	//円の氷の破棄
	CIceCircle::Unload();

	//弾の破棄
	CBullet::Unload();

	//MPロゴの破棄
	CMPLogo::Unload();

	//HPロゴの破棄
	CHPLogo::Unload();

	//噴水の破棄
	CFountainParticle::Unload();

	//雪の破棄
	CSnowParticle::Unload();

	//パーティクルダメージの破棄
	CDamageParticle::Unload();

	//パーティクルとくぎ２の破棄
	CSkill2Particle::Unload();

	//ビルボード体力ゲージの破棄
	CBillboardLifeGauge::Unload();

	//ポーズの破棄
	CPause::Unload();

	//パーティクルの破棄
	CParticle::Unload();

	//ゲージベースUIの破棄
	CUI_BaseGauge::Unload();

	//ゲージダメージUIの破棄
	CUI_DamageGauge::Unload();

	//ゲージとくぎUIの破棄
	CUI_SkillGauge::Unload();

	//ゲージ体力UIの破棄
	CUI_LifeGauge::Unload();

	//コマンドUIの破棄
	CCommandUI::Unload();

	//コマンドの破棄
	CCommand::Unload();

	//数字の破棄
	CNumber::Unload();

	//マップの終了処理
	if (m_pMap != NULL)
	{
		m_pMap->Uninit();
		delete m_pMap;
		m_pMap = NULL;
	}

	//全てのオブジェクト開放
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

	//フェードの取得処理
	CFade *pFade = CManager::GetFade();

	//プレイヤーが死んだ場合
	if (m_pPlayer->GetState() == CPlayer::PLAYER_STATE_DEATH)
	{
		//タイマーを０にする
		m_pTimer->SetTimer(0);
	}

	//敵の全滅またはタイマーが０の場合
	if (CEnemy::GetNumEnemy() <= 0 || m_pTimer->GetTimer() <= 0)
	{
		CManager *pManager = NULL;
		CEnemy *pEnemy = NULL;

		//マネージャーで必要な情報を設置
		pManager->SetScore(m_pScore->GetScore());
		pManager->SetBird(m_pPlayer->GetDeathBird());
		pManager->SetFrog(m_pPlayer->GetDeathFrog());
		pFade->SetFade(CManager::MODE_RESULT, pFade->FADE_OUT);

	}

	//デバッグ画面遷移
	if (pInputKeyboard->GetTrigger(DIK_F1) == true)
	{
		CManager *pManager = NULL;
		CEnemy *pEnemy = NULL;

		//マネージャーで必要な情報を設置
		pManager->SetScore(m_pScore->GetScore());
		pManager->SetBird(m_pPlayer->GetDeathBird());
		pManager->SetFrog(m_pPlayer->GetDeathFrog());
		pFade->SetFade(CManager::MODE_RESULT, pFade->FADE_OUT);
	}
#ifdef _DEBUG
	//デバッグ表示
	CDebugProc::Print(1, "Game");
#endif
}

//=============================================================================
// ゲームの描画処理
//=============================================================================
void CGame::Draw(void)
{
}

//=============================================================================
// プレイヤーの取得
//=============================================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;				//値を返す
}

//=============================================================================
// コマンドの取得
//=============================================================================
CScore * CGame::GetScore(void)
{
	return m_pScore;
}

//=============================================================================
// コマンドの取得
//=============================================================================
CCommand * CGame::GetCommad(void)
{
	return m_pCommand;
}

//=============================================================================
// オブジェクト3Dの取得
//=============================================================================
CMeshField *CGame::GetMeshField(void)
{
	return m_pMeshField;				//値を返す
}

//=============================================================================
// ステータス数字のの取得
//=============================================================================
CStatus_Number *CGame::GetStatusNumber(void)
{
	return m_pStatuNumber;				//値を返す
}

//=============================================================================
// ステータス数字のの取得
//=============================================================================
CUI_LifeGauge *CGame::GetUI_LifeGauge(void)
{
	return m_pUI_LifeGauge;				//値を返す
}

//=============================================================================
// ステータス数字のの取得
//=============================================================================
CUI_SkillGauge *CGame::GetUI_SkillGauge(void)
{
	return m_pUI_SkillGauge;				//値を返す
}

//=============================================================================
// ステータス数字のの取得
//=============================================================================
CUI_DamageGauge *CGame::GetUI_DamageGauge(void)
{
	return m_pUI_DamageGauge;				//値を返す
}

//=============================================================================
// ビルボード数字のの取得
//=============================================================================
CBillboardNumber *CGame::GetBillborad_Number(void)
{
	return m_pBillboard_Number;				//値を返す
}