//=============================================================================
//
// 弾の処理 [bullet.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
#include "scenebillboard.h"
#define MAX_BILLBOARDGAGE_WIDTH (15.0f)	//ビルボードゲージの幅最大値
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBullet : public CSceneBillboard
{
public:
	//メンバ関数
	CBullet();
	~CBullet();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CBullet *Create(D3DXVECTOR3 pos, int nLife, D3DXVECTOR3 move);
	static HRESULT Load(void);
	static void Unload(void);

private:
	//メンバ変数
	int m_nLife;		//体力

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
};
#endif