//=============================================================================
//
// 爆発の処理 [explotion.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _EXPLOTION_H_
#define _EXPLOTION_H_

#include "main.h"
#include "scene2D.h"

#define TEXTURE_NAME002 "data\\TEXTURE\\explosion000.png"			//テクスチャ名
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CExplotion : public CScene2D
{
public:
	//メンバ関数
	CExplotion();
	~CExplotion();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CExplotion *Create(D3DXVECTOR3 pos, float fWight, float fHeight);

private:
	//メンバ変数
	int m_nCounterAnim;	// アニメーションカウンター
	int m_nPatternAnim;	// アニメーションパターン

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
};
#endif