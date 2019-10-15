//=============================================================================
//
// 残機の処理 [stock.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _STOCK_H_
#define _STOCK_H_

#include "main.h"
#include "scene.h"

#define MAX_STOCK (3)	//桁数

class CPolygon;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CStock : public CScene
{
public:
	//メンバ関数
	CStock();
	~CStock();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetStock(int nStock);
	int GetStock(void);
	void SetPos(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);
	//静的メンバ関数
	static CStock *Create(D3DXVECTOR3 pos, float fWight, float fHeight);
	void CutStock(int nStock);

private:
	D3DXVECTOR3		m_pos;								// 位置
														//静的メンバ変数
	static CPolygon			*m_apPolygon[MAX_STOCK];	// テクスチャへのポインタ
	static int				m_nStock;

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
protected:
};
#endif