//=============================================================================
//
// スコアの処理 [score.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"
#include "scene.h"

class CNumber;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScore : public CScene
{
public:
	//メンバ関数
	CScore();
	~CScore();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos);
	void SetCol(D3DXCOLOR col);
	void SetWidth(float fWidth);
	void SetHeight(float fHeight);
	void SetDigit(int nDigit);
	void SetScore(int nScore);

	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetDigit(void);
	int GetScore(void);

	void AddScore(int nValue);

	//静的メンバ関数
	static CScore *Create(D3DXVECTOR3 pos, float fWidth, float fHeight,int nDigit);

private:
	D3DXVECTOR3		m_pos;				//位置
	D3DXCOLOR		m_col;				//色
	float			m_fWidth;			//幅
	float			m_fHeight;			//高さ
	int				m_nDigit;			//桁数
	int				m_nScore;			//スコア数

	//静的メンバ変数
	CNumber			**m_apNumber;
protected:
};
#endif