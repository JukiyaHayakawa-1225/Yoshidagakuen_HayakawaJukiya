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
	void SetScore(int nScore);
	int GetScore(void);
	void AddScore(int nValue);
	void SetCol(D3DXCOLOR col);
	D3DXCOLOR GetCol(void);
	//静的メンバ関数
	static CScore *Create(D3DXVECTOR3 pos, float fWight, float fHeight);

private:
	D3DXVECTOR3		m_pos;						// 位置
	float								m_fWight;
	float								m_fHeight;
	//静的メンバ変数
	CNumber			*m_apNumber[SCORE_DIGIT];	// テクスチャへのポインタ
	static int		m_nScore;
	D3DXCOLOR		m_col;
protected:
};
#endif