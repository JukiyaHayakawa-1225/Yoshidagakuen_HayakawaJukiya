//=============================================================================
//
// レンダリング処理 [renderer.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

class CPause;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRenderer
{
public:
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void); //{ return m_pD3DDevice; } //一行だけの処理の場合使用
	void RelesePause(void);
	static bool GetPause(void);

private:
#ifdef _DEBUG
	void DrawFPS(void);
	void DrawMode(void);
#endif
	static	CPause				*m_pPause;
	static bool m_bPause;
	LPDIRECT3D9					m_pD3D = NULL;			// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9			m_pD3DDevice = NULL;	// Deviceオブジェクト(描画に必要)
	LPD3DXFONT					m_pFont = NULL;			// フォントへのポインタ
};
#endif