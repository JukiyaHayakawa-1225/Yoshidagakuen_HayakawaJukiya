//=============================================================================
//
// 数字の処理 [number.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"

#define TEXTURE_NAME008 "data\\TEXTURE\\number001.png"
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CNumber
{
public:
	//メンバ関数
	CNumber();
	~CNumber();
	HRESULT Init(D3DXVECTOR3 pos, float fWight, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetNumber(int nNumber);
	void SetCol(D3DXCOLOR col);

	//静的メンバ関数
	static CNumber *Create(D3DXVECTOR3 pos, float fWight, float fHeight);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9			m_pTexture;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9				m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXVECTOR3							m_pos;				// 位置
	D3DXCOLOR							m_col;		//色

protected:
};
#endif