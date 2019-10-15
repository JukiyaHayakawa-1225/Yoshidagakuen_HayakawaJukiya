//=============================================================================
//
// ポリゴン処理 [number.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

#define MAX_POLYGON_TEXTURE (2)
#define TEXTURE_NAME011 "data\\TEXTURE\\life.png"
#define TEXTURE_NAME013 "data\\TEXTURE\\stock.png"
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPolygon
{
public:
	//=========================================================================
	// 構造体の定義
	//=========================================================================
	typedef enum
	{
		POLYGON_TYPE_NONE = 0,
		POLYGON_TYPE_LIFE,		//体力
		POLYGON_TYPE_STOCK,		//残機
	} POLYGON_TYPE;
	//メンバ関数
	CPolygon();
	~CPolygon();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 texture);
	//静的メンバ関数
	static CPolygon *Create(D3DXVECTOR3 pos, float fWight, float fHeight);

private:
	LPDIRECT3DTEXTURE9					m_pTexture;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9				m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXVECTOR3							m_pos;				// 位置
	float								m_fWight;
	float								m_fHeight;

protected:
};
#endif