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
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos);
	void SetCol(D3DXCOLOR col);
	void SetWidth(float fWidth);
	void SetHeight(float fHeight);

	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);

	void SetNumber(int nNumber);

	//静的メンバ関数
	static CNumber *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
	static HRESULT Load(void);
	static void Unload(void);

private:
	//メンバ変数
	LPDIRECT3DVERTEXBUFFER9				m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXVECTOR3							m_pos;		// 位置
	D3DXCOLOR							m_col;		// 色
	float								m_fWidth;	// 幅
	float								m_fHeight;	// 高さ

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9			m_pTexture;	// テクスチャへのポインタ

protected:
};
#endif