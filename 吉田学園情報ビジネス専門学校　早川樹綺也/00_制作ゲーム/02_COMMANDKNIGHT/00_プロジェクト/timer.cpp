//=============================================================================
//
// タイマーの処理 [timer.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "timer.h"
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
int CTimer::m_nTimer = 0;

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// タイマーのコンストラクタ
//=============================================================================
CTimer::CTimer() :CScene(6, OBJTYPE_NUMBER)
{
}
//=============================================================================
// タイマーのデストラクタ
//=============================================================================
CTimer::~CTimer()
{

}

//=============================================================================
//タイマーの生成
//=============================================================================
CTimer *CTimer::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CTimer *pTimer = NULL;
	if (pTimer == NULL)
	{
		pTimer = new CTimer;		//シーンの動的確保
		pTimer->m_pos = pos;		//位置の設置
		pTimer->m_fWidth = fWidth;	//幅の設置
		pTimer->m_fHeight = fHeight;//高さの設置
		pTimer->Init();				//初期化処理
	}
	return pTimer;					//値を返す
}

//=============================================================================
// タイマーの初期化処理
//=============================================================================
HRESULT CTimer::Init(void)
{
	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	int nTimerData = 0;	//タイマーの格納
	int nTime = 1;		//タイマーの桁
	m_bStopTimer = false;
	//モード別処理
	switch (mode)
	{
	case CManager::MODE_GAME:	//ゲームの場合
		m_nTimer = 240;	//タイマー代入
		break;
	case CManager::MODE_RESULT:	//リザルトの場合
		m_nTimer = CManager::GetTime();	//タイマーの取得
	}

	for (int nCntNumber = 0; nCntNumber < TIMER_DIGIT; nCntNumber++)
	{//桁の数だけ繰り返し
		m_apNumber[nCntNumber] = NULL;
	}

	for (int nCntNumber = 0; nCntNumber < TIMER_DIGIT; nCntNumber++)
	{//桁の数だけ繰り返し
		if (m_apNumber[nCntNumber] == NULL)
		{
			//モードの取得
			CManager::MODE mode = CManager::GetMode();

			if (mode == CManager::MODE_GAME)
			{//ゲームの場合
				//数字の生成
				m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x - (70.0f * nCntNumber), m_pos.y, m_pos.z), m_fWidth, m_fHeight);
				nTimerData = m_nTimer % (nTime * 10) / nTime;
				m_apNumber[nCntNumber]->SetNumber(nTimerData);
				nTime *= 10;

			}
			else if(mode == CManager::MODE_RESULT)
			{//リザルトの場合
				//数字の生成
				m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x - (70.0f * nCntNumber), m_pos.y, m_pos.z), m_fWidth, m_fHeight);
				nTimerData = m_nTimer % (nTime * 10) / nTime;
				m_apNumber[nCntNumber]->SetNumber(nTimerData);
				nTime *= 10;
			}
		}
	}
	return S_OK;
}

//=============================================================================
// タイマーの終了処理
//=============================================================================
void CTimer::Uninit(void)
{
	//タイマーの終了処理
	for (int nCntNumber = 0; nCntNumber < TIMER_DIGIT; nCntNumber++)
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
// タイマーの更新処理
//=============================================================================
void CTimer::Update(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetJoypad();

	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	//フェードの取得処理
	CFade *pFade = CManager::GetFade();

	switch (mode)
	{
	case CManager::MODE_GAME:
		int nTimerData;
		if (m_bStopTimer == TIMERSTATE_COUNT)
		{// タイマーを進める状態だったら
			int nTime = 1;
			m_nCountTimer++;
			if (m_nCountTimer % 60 == 0)
			{// 一秒間たったら
				m_nTimer--;
				if (m_nTimer >= 0)
				{
					for (int nCntTimer = 0; nCntTimer < TIMER_DIGIT; nCntTimer++)
					{// タイマーの桁数だけ繰り返し
						nTimerData = m_nTimer % (nTime * 10) / nTime;
						m_apNumber[nCntTimer]->SetNumber(nTimerData);
						nTime *= 10;
					}
				}
				else if (m_nTimer < 0 /*&& fade == FADE_NONE*/)
				{// タイマーが0になったら
					/*SetGameState(GAMESTATE_END);
					SetResult(RESULT_CLEAR);*/
					m_nTimer = 0;
				}
			}
		}
		break;
	case CManager::MODE_RESULT:
		//モードの取得
		CResult::COUNT_TYPE type;
		type = CResult::GetType();

		if (type == CResult::COUNT_TIMER)
		{
			if (m_bStopTimer == TIMERSTATE_COUNT)
			{
				if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
					pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_B) == true)
				{
					CScore *pScore = CResult::GetScore();
					pScore->AddScore(m_nTimer * 10);
					m_nTimer = 0;
					int nTime = 1;

					for (int nCntTimer = 0; nCntTimer < TIMER_DIGIT; nCntTimer++)
					{// タイマーの桁数だけ繰り返し
						nTimerData = m_nTimer % (nTime * 10) / nTime;
						m_apNumber[nCntTimer]->SetNumber(nTimerData);
						nTime *= 10;
					}
					m_bStopTimer = TIMERSTATE_NONE_COUNT;
					CResult::SetType(CResult::COUNT_BIRD);
				}
				int nTime = 1;
				m_nCountTimer++;
				if (m_nCountTimer % 2 == 0
					&& pFade->GetFade() == CFade::FADE_NONE)
				{// 一秒間たったら
					if (m_nTimer > 0)
					{
						m_nTimer--;
						for (int nCntTimer = 0; nCntTimer < TIMER_DIGIT; nCntTimer++)
						{// タイマーの桁数だけ繰り返し
							nTimerData = m_nTimer % (nTime * 10) / nTime;
							m_apNumber[nCntTimer]->SetNumber(nTimerData);
							nTime *= 10;
						}
						//サウンドの取得
						CSound *pSound;
						pSound = CManager::GetSound();
						//スコアの取得
						CScore *pScore = CResult::GetScore();
						pScore->AddScore(10);	//スコアの加算
						pSound->PlaySound(CSound::SOUND_LABEL_SE_COUNT);
					}
					else
					{
						CResult::SetType(CResult::COUNT_BIRD);
					}
				}
			}
		}
		break;
	}
}

//=============================================================================
// タイマーの描画処理
//=============================================================================
void CTimer::Draw(void)
{
	for (int nCntTimer = 0; nCntTimer < TIMER_DIGIT; nCntTimer++)
	{
		//数字の描画処理
		m_apNumber[nCntTimer]->Draw();
	}
}

//=============================================================================
// タイマーの設置処理
//=============================================================================
void CTimer::SetTimer(int nTimer)
{
	m_nTimer = nTimer;
}

//=============================================================================
// タイマーの取得処理
//=============================================================================
int CTimer::GetTimer(void)
{
	return m_nTimer;
}

//=============================================================================
// タイマーの状態取得処理
//=============================================================================
bool CTimer::GetTimerState(void)
{
	return m_bStopTimer;
}

