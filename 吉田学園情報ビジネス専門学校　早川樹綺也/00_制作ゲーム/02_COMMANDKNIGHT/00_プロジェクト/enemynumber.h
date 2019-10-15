//=============================================================================
//
// 敵の数処理 [enemynumber.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _ENEMYNUMBER_H_
#define _ENEMYNUMBER_H_

#include "main.h"
#include "scene.h"

class CNumber;

#define MAX_ENEMY_TYPE (2)
#define ENEMY_DIGIT (2)
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBirdNumber : public CScene
{
public:
	//メンバ関数
	CBirdNumber();
	~CBirdNumber();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//静的メンバ関数
	static CBirdNumber *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
private:
	D3DXVECTOR3							m_pos;						// 位置
	float								m_fWidth;
	float								m_fHeight;
	int									m_nCountTimer;
	CNumber			*m_apNumber[ENEMY_DIGIT];	// テクスチャへのポインタ
	//静的メンバ変数
	static int		m_nBirdNumber;

protected:
};

class CFrogNumber : public CScene
{
public:
	//メンバ関数
	CFrogNumber();
	~CFrogNumber();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//静的メンバ関数
	static CFrogNumber *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
private:
	D3DXVECTOR3							m_pos;						// 位置
	float								m_fWidth;
	float								m_fHeight;
	int									m_nCountTimer;
	CNumber			*m_apNumber[ENEMY_DIGIT];	// テクスチャへのポインタ
												//静的メンバ変数
	static int		m_nFrogNumber;

protected:
};

#endif