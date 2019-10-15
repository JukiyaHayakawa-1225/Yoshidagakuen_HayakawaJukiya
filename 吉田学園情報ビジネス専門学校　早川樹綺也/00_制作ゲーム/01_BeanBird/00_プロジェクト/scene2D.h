//=============================================================================
//
// オブジェクト2D処理 [scene2D.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include "scene.h"

#define BG_POS_X			(0)								// 背景の左上X座標
#define BG_POS_Y			(0)								// 背景の左上Y座標
#define BG_WIDTH			(SCREEN_WIDTH)					// 背景の幅
#define BG_HEIGHT			(SCREEN_HEIGHT)					// 背景の高さ


//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScene2D : public CScene
{
public:
	//メンバ関数
	CScene2D(int nPriority = 3);
	~CScene2D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	void SetPos(D3DXVECTOR3 pos);
	void SetWidth(float width);
	void SetHeight(float height);
	void SetVtxPos(D3DXVECTOR3 pos);
	void SetVtxLenghtPos(D3DXVECTOR3 pos);
	void SetVtxRotPos(D3DXVECTOR3 pos);
	void SetLength(float fLength);
	float GetWidth(void);
	float GetHeight(void);
	float GetfLength(void);
	void SetCol(D3DXCOLOR col);
	void SetTex(int nAnimPattern, float texU,float texV);
	void BindTexture(LPDIRECT3DTEXTURE9 texture);
	LPDIRECT3DVERTEXBUFFER9 GetVtx(void);
	//静的メンバ関数
	static CScene2D *Create(D3DXVECTOR3 pos, float fWight, float fHeight);

private:
	LPDIRECT3DTEXTURE9			m_pTexture = NULL;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	D3DXVECTOR3					m_pos;				// 位置
	D3DXCOLOR					m_col;
	float						m_fWidth;
	float						m_fHeight;
	float						m_fLength;			//拡大
protected:
	//メンバ変数
	float						m_rot;				//向き
	float						m_fAddLength;		//加算数
};
#endif