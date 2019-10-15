//=============================================================================
//
// 敵の数の処理 [enemynumber.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "enemynumber.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "number.h"
#include "result.h"
#include "score.h"
#include "sound.h"
#include "fade.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CBirdNumber::m_nBirdNumber = 0;

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// 敵の数のコンストラクタ
//=============================================================================
CBirdNumber::CBirdNumber() :CScene(6, OBJTYPE_NUMBER)
{
}
//=============================================================================
// 敵の数のデストラクタ
//=============================================================================
CBirdNumber::~CBirdNumber()
{

}

//=============================================================================
//敵の数の生成
//=============================================================================
CBirdNumber *CBirdNumber::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CBirdNumber *pEnemyNumber = NULL;
	if (pEnemyNumber == NULL)
	{
		pEnemyNumber = new CBirdNumber;		//シーンの動的確保
		pEnemyNumber->m_pos = pos;			//位置の設置
		pEnemyNumber->m_fWidth = fWidth;	//幅の設置
		pEnemyNumber->m_fHeight = fHeight;	//高さの設置
		pEnemyNumber->Init();				//初期化処理
	}
	return pEnemyNumber;					//値を返す
}

//=============================================================================
// 敵の数の初期化処理
//=============================================================================
HRESULT CBirdNumber::Init(void)
{
	CManager::MODE mode;
	mode = CManager::GetMode();
	int nEnemyNumberData = 0;
	int nEnemy = 1;

	//総数取得
	m_nBirdNumber = CManager::GetBird();

	for (int nCntNumber = 0; nCntNumber < ENEMY_DIGIT; nCntNumber++)
	{
		m_apNumber[nCntNumber] = NULL;
	}

	for (int nCntNumber = 0; nCntNumber < ENEMY_DIGIT; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] == NULL)
		{
			//数字の生成
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x - (70.0f * nCntNumber), m_pos.y, m_pos.z), m_fWidth, m_fHeight);

			nEnemyNumberData = m_nBirdNumber % (nEnemy * 10) / nEnemy;

			m_apNumber[nCntNumber]->SetNumber(nEnemyNumberData);
			nEnemy *= 10;
		}
	}
	return S_OK;
}

//=============================================================================
// 敵の数の終了処理
//=============================================================================
void CBirdNumber::Uninit(void)
{
	//敵の数の終了処理
	for (int nCntNumber = 0; nCntNumber < ENEMY_DIGIT; nCntNumber++)
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
// 敵の数の更新処理
//=============================================================================
void CBirdNumber::Update(void)
{
	//フェードの取得処理
	CFade *pFade = CManager::GetFade();
	//モードの取得
	CResult::COUNT_TYPE type;
	type = CResult::GetType();

	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetJoypad();

	if (type == CResult::COUNT_BIRD)
	{
		int nData;
		int nEnemy = 1;

		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
			pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_B) == true)
		{
			CScore *pScore = CResult::GetScore();
			pScore->AddScore(m_nBirdNumber * 500);
			m_nBirdNumber = 0;
			int nTime = 1;

			for (int nCntTimer = 0; nCntTimer < ENEMY_DIGIT; nCntTimer++)
			{// タイマーの桁数だけ繰り返し
				nData = m_nBirdNumber % (nEnemy * 10) / nEnemy;
				m_apNumber[nCntTimer]->SetNumber(nData);
				nEnemy *= 10;
			}
			CResult::SetType(CResult::COUNT_FROG);
		}

		m_nCountTimer++;
		if (m_nCountTimer % 10 == 0
			&& pFade->GetFade() == CFade::FADE_NONE)
		{// 一秒間たったら
			if (m_nBirdNumber > 0)
			{
				m_nBirdNumber--;
				for (int nCntTimer = 0; nCntTimer < ENEMY_DIGIT; nCntTimer++)
				{// タイマーの桁数だけ繰り返し
					nData = m_nBirdNumber % (nEnemy * 10) / nEnemy;
					m_apNumber[nCntTimer]->SetNumber(nData);
					nEnemy *= 10;
				}
				CSound *pSound;
				pSound = CManager::GetSound();

				CScore *pScore = CResult::GetScore();
				pScore->AddScore(500);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_COUNT);
			}
			else
			{
				CResult::SetType(CResult::COUNT_FROG);
			}
		}
	}
}

//=============================================================================
// 敵の数の描画処理
//=============================================================================
void CBirdNumber::Draw(void)
{
	for (int nCntEnemyNumber = 0; nCntEnemyNumber < ENEMY_DIGIT; nCntEnemyNumber++)
	{
		//数字の描画処理
		m_apNumber[nCntEnemyNumber]->Draw();
	}
}

//=============================================================================
//
// 敵の数の処理 [enemynumber.cpp]
//
//=============================================================================
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CFrogNumber::m_nFrogNumber = 0;

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// 敵の数のコンストラクタ
//=============================================================================
CFrogNumber::CFrogNumber() :CScene(6, OBJTYPE_NUMBER)
{
}
//=============================================================================
// 敵の数のデストラクタ
//=============================================================================
CFrogNumber::~CFrogNumber()
{

}

//=============================================================================
//敵の数の生成
//=============================================================================
CFrogNumber *CFrogNumber::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CFrogNumber *pEnemyNumber = NULL;
	if (pEnemyNumber == NULL)
	{
		pEnemyNumber = new CFrogNumber;		//シーンの動的確保
		pEnemyNumber->m_pos = pos;		//位置の設置
		pEnemyNumber->m_fWidth = fWidth;	//幅の設置
		pEnemyNumber->m_fHeight = fHeight;//高さの設置
		pEnemyNumber->Init();				//初期化処理
	}
	return pEnemyNumber;					//値を返す
}

//=============================================================================
// 敵の数の初期化処理
//=============================================================================
HRESULT CFrogNumber::Init(void)
{
	CManager::MODE mode;
	mode = CManager::GetMode();
	int nEnemyNumberData = 0;
	int nEnemy = 1;

	//総数取得
	m_nFrogNumber = CManager::GetFrog();

	for (int nCntNumber = 0; nCntNumber < ENEMY_DIGIT; nCntNumber++)
	{
		m_apNumber[nCntNumber] = NULL;
	}

	for (int nCntNumber = 0; nCntNumber < ENEMY_DIGIT; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] == NULL)
		{
			//数字の生成
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x - (70.0f * nCntNumber), m_pos.y, m_pos.z), m_fWidth, m_fHeight);

			nEnemyNumberData = m_nFrogNumber % (nEnemy * 10) / nEnemy;

			m_apNumber[nCntNumber]->SetNumber(nEnemyNumberData);
			nEnemy *= 10;
		}
	}
	return S_OK;
}

//=============================================================================
// 敵の数の終了処理
//=============================================================================
void CFrogNumber::Uninit(void)
{
	//敵の数の終了処理
	for (int nCntNumber = 0; nCntNumber < ENEMY_DIGIT; nCntNumber++)
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
// 敵の数の更新処理
//=============================================================================
void CFrogNumber::Update(void)
{
	//フェードの取得処理
	CFade *pFade = CManager::GetFade();
	//モードの取得
	CResult::COUNT_TYPE type;
	type = CResult::GetType();
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetJoypad();


	if (type == CResult::COUNT_FROG)
	{
		int nData;
		int nEnemy = 1;

		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
			pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_B) == true)
		{
			CScore *pScore = CResult::GetScore();
			pScore->AddScore(m_nFrogNumber * 500);
			m_nFrogNumber = 0;
			int nTime = 1;

			for (int nCntTimer = 0; nCntTimer < ENEMY_DIGIT; nCntTimer++)
			{// タイマーの桁数だけ繰り返し
				nData = m_nFrogNumber % (nEnemy * 10) / nEnemy;
				m_apNumber[nCntTimer]->SetNumber(nData);
				nEnemy *= 10;
			}
			CResult::SetType(CResult::COUNT_END);
		}

		m_nCountTimer++;
		if (m_nCountTimer % 10 == 0
			&& pFade->GetFade() == CFade::FADE_NONE)
		{// 一秒間たったら
			if (m_nFrogNumber > 0)
			{
				m_nFrogNumber--;
				for (int nCntTimer = 0; nCntTimer < ENEMY_DIGIT; nCntTimer++)
				{// タイマーの桁数だけ繰り返し
					nData = m_nFrogNumber % (nEnemy * 10) / nEnemy;
					m_apNumber[nCntTimer]->SetNumber(nData);
					nEnemy *= 10;
				}
				CSound *pSound;
				pSound = CManager::GetSound();

				CScore *pScore = CResult::GetScore();
				pScore->AddScore(500);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_COUNT);
			}
			else
			{
				CResult::SetType(CResult::COUNT_END);
			}
		}
	}
}

//=============================================================================
// 敵の数の描画処理
//=============================================================================
void CFrogNumber::Draw(void)
{
	for (int nCntEnemyNumber = 0; nCntEnemyNumber < ENEMY_DIGIT; nCntEnemyNumber++)
	{
		//数字の描画処理
		m_apNumber[nCntEnemyNumber]->Draw();
	}
}