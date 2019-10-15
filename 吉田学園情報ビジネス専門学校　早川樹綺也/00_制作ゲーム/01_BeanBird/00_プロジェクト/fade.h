//=============================================================================
//
// フェードの処理 [fade.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "manager.h"
#include "scene.h"
#include "scene2D.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CFade : public CScene
{
public:
	//*************************************
	// フェードの状態
	//*************************************
	typedef enum
	{
		FADE_NONE = 0,		// 何もしていない状態
		FADE_IN,			// フェードイン状態
		FADE_OUT,			// フェードアウト状態
		FADE_MAX
	} FADE;
	//メンバ関数
	CFade();
	~CFade();
	HRESULT  Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//静的メンバ関数
	static CFade *Create(CManager::MODE modeNext, D3DXVECTOR3 pos, float fWight, float fHeight);
	FADE GetFade(void);				//フェードの取得処理

private:
	//メンバ変数
	D3DXVECTOR3		m_pos;
	float			m_fWidth;
	float			m_fHeight;
	LPDIRECT3DTEXTURE9				m_pTexture;
	//静的メンバ変数
	static FADE		m_fade;
	CManager::MODE	m_modeNext;
	D3DXCOLOR		m_colorFade;
	LPDIRECT3DVERTEXBUFFER9				m_pVtxBuff;			// 頂点バッファへのポインタ
};
#endif