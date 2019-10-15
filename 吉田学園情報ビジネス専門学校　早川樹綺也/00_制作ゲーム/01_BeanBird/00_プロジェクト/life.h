//=============================================================================
//
// 体力の処理 [life.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

#include "main.h"
#include "scene.h"

#define MAX_LIFE (3)	//桁数

class CPolygon;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CLife : public CScene
{
public:
	//メンバ関数
	CLife();
	~CLife();
	HRESULT  Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetLife(int nLife);
	int GetLife(void);
	void SetPos(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);
	//静的メンバ関数
	static CLife *Create(D3DXVECTOR3 pos, float fWight, float fHeight);
	void CutLife(int nLife);
private:
	D3DXVECTOR3		m_pos;						// 位置
	//静的メンバ変数
	static CPolygon			*m_apPolygon[MAX_LIFE];	// テクスチャへのポインタ
	static int				m_nLife;
	static LPDIRECT3DTEXTURE9 m_pTexture;
protected:
};
#endif