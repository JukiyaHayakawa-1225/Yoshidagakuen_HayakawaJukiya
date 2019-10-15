//=============================================================================
//
// ビルボードの処理 [scenebillboard.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _SCENEBILLBOARD_H_
#define _SCENEBILLBOARD_H_

#include "main.h"
#include "scene.h"
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSceneBillboard : public CScene
{
public:	//誰からもアクセス可能
	//メンバ関数
	CSceneBillboard(int nPriority = 5, OBJTYPE objtype = OBJTYPE_BILLBOARD);
	~CSceneBillboard();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void BindTexture(LPDIRECT3DTEXTURE9 texture);
	void SetPos(D3DXVECTOR3 pos);
	void SetRot(D3DXVECTOR3 rot);
	void SetCol(D3DXCOLOR col);
	void SetWidth(float fWidth);
	void SetHeight(float fHeight);
	void SetLength(float fLength);
	void SetMove(D3DXVECTOR3 move);
	void SetLengthPos(void);

	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	float GetLength(void);
	void CutSide(int nType);
	D3DXVECTOR3 GetMove(void);

	//静的メンバ関数
	static CSceneBillboard *Create(void);

private:	//自分だけがアクセス可能
	//メンバ変数
	D3DXVECTOR3 m_pos;			//位置
	D3DXCOLOR	m_col;			//色
	float		m_fWidth;		//幅
	float		m_fHeight;		//高さ
	float		m_fLength;		//長さ
	D3DXVECTOR3 m_rot;			//向き
	D3DXMATRIX	m_mtxWorld;		//ワールドマトリックス
	D3DXVECTOR3 m_move;			//移動量
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff = NULL;					// 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9			m_pTexture = NULL;					// テクスチャへのポインタ
};
#endif
