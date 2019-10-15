//=============================================================================
//
// タイマーの処理 [timer.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"
#include "scene.h"

class CNumber;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTimer : public CScene
{
public:
	//**********************************
	// タイマーの状態
	//**********************************
	typedef enum
	{
		TIMERSTATE_COUNT = 0,
		TIMERSTATE_NONE_COUNT,
		TIMERSTATE_MAX
	}TIMERSTATE;
	//メンバ関数
	CTimer();
	~CTimer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetTimer(int nTimer);

	//静的メンバ関数
	static CTimer *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
	static int GetTimer(void);
	bool GetTimerState(void);
private:
	D3DXVECTOR3							m_pos;						// 位置
	float								m_fWidth;
	float								m_fHeight;
	int									m_nCountTimer;
	bool								m_bStopTimer;

	CNumber			*m_apNumber[TIMER_DIGIT];	// テクスチャへのポインタ
	//静的メンバ変数
	static int		m_nTimer;
protected:
};
#endif