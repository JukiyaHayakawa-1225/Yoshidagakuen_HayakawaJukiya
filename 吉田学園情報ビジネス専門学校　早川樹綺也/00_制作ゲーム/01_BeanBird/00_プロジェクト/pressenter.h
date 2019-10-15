//=============================================================================
//
// プレスエンターの処理 [pressenter.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _PRESSENTER_H_
#define _PRESSENTER_H_

#include "main.h"
#include "scene2D.h"

#define TEXTURE_NAME014 "data\\TEXTURE\\pressenter.png"
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPressEnter : public CScene2D
{
public:
	//=========================================================================
	// 構造体の定義
	//=========================================================================
	typedef enum
	{
		PRESSENTERSTATE_NONE = 0,
		PRESSENTERSTATE_PUSH,
		PRESSENTERSTATE_MAX
	}PRESSENTERSTATE;

	//メンバ関数
	CPressEnter();
	~CPressEnter();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	PRESSENTERSTATE GetPressEnter(void);

	//静的メンバ関数
	static CPressEnter *Create(D3DXVECTOR3 pos, float fWight, float fHeight);
	static HRESULT Load(void);
	static void Unload(void);

private:
	int m_nCounter;
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9	m_pTexture;
	static PRESSENTERSTATE      m_pressEnterState;                 // 状態
};
#endif