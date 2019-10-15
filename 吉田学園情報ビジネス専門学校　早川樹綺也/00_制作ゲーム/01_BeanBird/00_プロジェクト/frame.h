//=============================================================================
//
// フレームの処理 [frame.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _FRAME_H_
#define _FRAME_H_

#include "main.h"
#include "scene2D.h"

#define MAX_FRAME (FRAME_MAX)
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CFrame : public CScene2D
{
public:
	//=========================================================================
	// 構造体の定義
	//=========================================================================
	typedef enum
	{
		FRAME_SCORE = 0,
		FRAME_STOCK,
		FRAME_LIFE,
		FRAME_SKILL,
		FRAME_MAX,
	} FRAME_TYPE;

	//メンバ関数
	CFrame();
	~CFrame();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//静的メンバ関数
	static CFrame *Create(D3DXVECTOR3 pos, float fWight, float fHeight,FRAME_TYPE type);

private:
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_FRAME];
};
#endif