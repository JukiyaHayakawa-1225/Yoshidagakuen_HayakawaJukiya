//=============================================================================
//
// エフェクトの処理 [effect.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "scene2D.h"

#define TEXTURE_NAME012 "data\\TEXTURE\\effect000.jpg"			//テクスチャ名

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEffect : public CScene2D
{
public:
	//メンバ関数
	CEffect();
	~CEffect();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CEffect *Create(D3DXVECTOR3 pos,D3DXCOLOR col, float fWight, float fHeight);

private:
	//メンバ変数
	int m_nLife;		//体力
	D3DXVECTOR3 m_move;	//移動量
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
};
#endif