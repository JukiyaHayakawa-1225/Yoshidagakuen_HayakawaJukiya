//=============================================================================
//
// ビルボードゲージ処理 [billboardgauge.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _BILLBOARDGAUGE_H_
#define _BILLBOARDGAUGE_H_

#include "main.h"
#include "scenebillboard.h"
#define MAX_BILLBOARDGAGE_WIDTH (15.0f)	//ビルボードの幅の最大値

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBillboardLifeGauge : public CSceneBillboard
{
public:
	//メンバ関数
	CBillboardLifeGauge();
	~CBillboardLifeGauge();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CutLifeGauge(int cut);

	//静的メンバ関数
	static CBillboardLifeGauge *Create(D3DXVECTOR3 pos, int nLife, int nMaxLife);
	static HRESULT Load(void);
	static void Unload(void);

private:
	//メンバ変数
	int m_nLife;		//体力
	int	m_nMaxLife;		//体力最大値

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャのポインタ
};
#endif