//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CParticle : public CScene2D
{
public:
	//メンバ関数
	CParticle();
	~CParticle();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CParticle *Create(D3DXVECTOR3 pos,D3DXVECTOR3 move, D3DXCOLOR col,float fLength);

private:
	//メンバ変数
	int m_nLife;		//体力
	D3DXVECTOR3 m_move;	//移動量
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
};
#endif