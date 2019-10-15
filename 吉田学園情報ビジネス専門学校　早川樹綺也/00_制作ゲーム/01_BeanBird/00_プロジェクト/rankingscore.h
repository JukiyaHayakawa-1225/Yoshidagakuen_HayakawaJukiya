//=============================================================================
//
// ランキングスコアの処理 [rankingscore.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _RANKINGSCORE_H_
#define _RANKINGSCORE_H_

#include "main.h"
#include "scene.h"
class CScore;
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_RANKINGSCORE        (5)             // ランキングスコアの数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRankingScore : public CScene
{
public:
	//=========================================================================
	// 構造体の定義
	//=========================================================================
	typedef enum
	{
		RANKINGSCORESTATE_NONE = 0, // 更新されていない状態
		RANKINGSCORESTATE_UPDATE,   // 更新された状態
		RANKINGSCORESTATE_MAX
	}RANKINGSCORESTATE;


	//メンバ関数
	CRankingScore();
	~CRankingScore();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//RANKINGSCORE *GetRankingScore(void);
	//静的メンバ関数
	static CRankingScore *Create(D3DXVECTOR3 pos, float fWight, float fHeight);

private:
	D3DXVECTOR3		m_pos;				// 位置
	float			m_fWight;
	float			m_fHeight;
	CScore			*m_apScore[MAX_RANKINGSCORE];	// テクスチャへのポインタ
	static int		m_aRankingScore[MAX_RANKINGSCORE];
	int				m_nCountCol;
	D3DXCOLOR		m_col;
};
#endif