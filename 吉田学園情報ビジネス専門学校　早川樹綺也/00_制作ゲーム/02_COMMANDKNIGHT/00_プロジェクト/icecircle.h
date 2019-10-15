//=============================================================================
//
// 円の氷処理 [icecircle.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _ICECIRCLE_H_
#define _ICECIRCLE_H_

#include "main.h"
#include "scene.h"
#include "scene3D.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CIceCircle : public CScene3D
{
public:	//誰からもアクセス可能
	//メンバ関数
	CIceCircle();
	~CIceCircle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CIceCircle *Create(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);

private:	//自分だけがアクセス可能
	int m_nTimer;		//タイマー
	static LPDIRECT3DTEXTURE9 m_pTexture;
};
#endif
