//=============================================================================
//
// 敵の防衛処理 [defense.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _DEFENSE_H_
#define _DEFENSE_H_

#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CDefense : public CScene2D
{
public:
	//メンバ関数
	CDefense();
	~CDefense();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void FollowPlayer(void);
	//静的メンバ関数
	static CDefense *Create(D3DXVECTOR3 pos, float fWight, float fHeight);
	static HRESULT Load(void);
	static void Unload(void);

private:
	int			m_nCounter;
	D3DXVECTOR3 m_move;						//移動量
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
};
#endif