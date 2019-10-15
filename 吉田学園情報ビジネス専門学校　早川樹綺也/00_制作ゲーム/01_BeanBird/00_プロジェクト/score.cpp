//=============================================================================
//
// スコアの処理 [score.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "score.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "number.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CScore::m_nScore = 0;

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// スコアのコンストラクタ
//=============================================================================
CScore::CScore() : CScene(6)
{
}
//=============================================================================
// スコアのデストラクタ
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
//スコアの生成
//=============================================================================
CScore *CScore::Create(D3DXVECTOR3 pos, float fWight, float fHeight)
{
	CScore *pScore = NULL;
	if (pScore == NULL)
	{
		pScore = new CScore;		//シーンの動的確保
		pScore->m_pos = pos;		//位置の設置
		pScore->m_fWight = fWight;	//幅の設置
		pScore->m_fHeight = fHeight;//高さの設置
		pScore->Init();				//初期化処理
	}
	return pScore;					//値を返す
}

//=============================================================================
// スコアの初期化処理
//=============================================================================
HRESULT CScore::Init(void)
{
	for (int nCntNumber = 0; nCntNumber < SCORE_DIGIT; nCntNumber++)
	{
		m_apNumber[nCntNumber] = NULL;
	}

	for (int nCntNumber = 0; nCntNumber < SCORE_DIGIT; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] == NULL)
		{
			//数字の動的確保
			//m_apNumber[nCntNumber] = new CNumber;

			CManager::MODE mode = CManager::GetMode();
			if (mode == CManager::MODE_GAME)
			{
				//数字の生成
				m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x - (45 * nCntNumber), m_pos.y, m_pos.z), m_fWight, m_fHeight);
			}
			else if (mode == CManager::MODE_RANKING)
			{
				//数字の生成
				m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x - (90 * nCntNumber), m_pos.y, m_pos.z), m_fWight, m_fHeight);
			}
			m_nScore = 0;
		}
	}
	return S_OK;
}

//=============================================================================
// スコアの終了処理
//=============================================================================
void CScore::Uninit(void)
{
	//スコアの終了処理
	for (int nCntNumber = 0; nCntNumber < SCORE_DIGIT; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{
			//終了処理
			m_apNumber[nCntNumber]->Uninit();
			//メモリの開放
			delete m_apNumber[nCntNumber];
			//NULLにする
			m_apNumber[nCntNumber] = NULL;
		}
	}
	Release();
}

//=============================================================================
// スコアの更新処理
//=============================================================================
void CScore::Update(void)
{

}

//=============================================================================
// スコアの描画処理
//=============================================================================
void CScore::Draw(void)
{
	for (int nCntScore = 0; nCntScore < SCORE_DIGIT; nCntScore++)
	{
		//数字の描画処理
		m_apNumber[nCntScore]->Draw();
	}
}

//=============================================================================
// スコアの設置処理
//=============================================================================
void CScore::SetScore(int nScore)
{
	//スコアの加算
	m_nScore += nScore;
}

//=============================================================================
// スコアの加算
//=============================================================================
void CScore::AddScore(int nValue)
{
	int nAnswer = 1;
	int nScore;

	m_nScore += nValue;

	for (int nCntNumber = 0; nCntNumber < SCORE_DIGIT; nCntNumber++)
	{
		nScore = m_nScore % (nAnswer * 10) / nAnswer;
		//数字の設置
		m_apNumber[nCntNumber]->SetNumber(nScore);
		nAnswer *= 10;
	}
}


//=============================================================================
// スコアの取得
//=============================================================================
int CScore::GetScore(void)
{
	//値を返す
	return m_nScore;
}

//=============================================================================
// スコアの取得
//=============================================================================
void CScore::SetCol(D3DXCOLOR col)
{
	for (int nCntNumber = 0; nCntNumber < SCORE_DIGIT; nCntNumber++)
	{
		m_apNumber[nCntNumber]->SetCol(col);
	}
}