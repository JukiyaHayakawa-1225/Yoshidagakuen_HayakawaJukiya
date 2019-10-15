//=============================================================================
//
// ランキングスコアの処理 [rankingrankingscore.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "rankingscore.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "number.h"
#include "score.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CRankingScore::m_aRankingScore[MAX_RANKINGSCORE] = {};

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// ランキングスコアのコンストラクタ
//=============================================================================
CRankingScore::CRankingScore() : CScene(6)
{
}
//=============================================================================
// ランキングスコアのデストラクタ
//=============================================================================
CRankingScore::~CRankingScore()
{

}

//=============================================================================
//ランキングスコアの生成
//=============================================================================
CRankingScore *CRankingScore::Create(D3DXVECTOR3 pos, float fWight, float fHeight)
{
	CRankingScore *pRankingScore = NULL;
	if (pRankingScore == NULL)
	{
		pRankingScore = new CRankingScore;		//シーンの動的確保
		pRankingScore->m_pos = pos;				//位置の代入
		pRankingScore->m_fWight = fWight;		//幅の代入
		pRankingScore->m_fHeight = fHeight;		//高さの代入
		pRankingScore->Init();					//初期化処理
		//pNumber->BindTexture(m_pTexture);
	}
	return pRankingScore;					//値を返す
}

//=============================================================================
// ランキングスコアの初期化処理
//=============================================================================
HRESULT CRankingScore::Init(void)
{
	for (int nCntRanking = 0; nCntRanking < MAX_RANKINGSCORE; nCntRanking++)
	{
		//数字の生成
		m_apScore[nCntRanking] = CScore::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), m_fWight, m_fHeight);
		m_apScore[nCntRanking]->AddScore(CManager::GetGameScore(nCntRanking));
		m_pos.y += 105.0f;
	}
	m_nCountCol = 0;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	return S_OK;
}

//=============================================================================
// ランキングスコアの終了処理
//=============================================================================
void CRankingScore::Uninit(void)
{
	for (int nCntRanking = 0; nCntRanking < MAX_RANKINGSCORE; nCntRanking++)
	{
		//終了処理
		m_apScore[nCntRanking]->Uninit();

		//NULLにする
		m_apScore[nCntRanking] = NULL;
	}
	Release();
	//CScene::ReleaseAll();
}

//=============================================================================
// ランキングスコアの更新処理
//=============================================================================
void CRankingScore::Update(void)
{
	CManager *pManager = NULL;
	int nUpdateScore = pManager->GetUpdateScore();
	if (nUpdateScore >= 0 && nUpdateScore <= 4)
	{
		m_nCountCol++;
		if (m_nCountCol % 4 == 0)
		{// 一定時間が立った場合
			if (m_col.b == 0.0f)
			{// 赤の場合
				m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else
			{// 白の場合
				m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}
		}
		m_apScore[nUpdateScore]->SetCol(m_col);
	}
}

//=============================================================================
// ランキングスコアの描画処理
//=============================================================================
void CRankingScore::Draw(void)
{
}
