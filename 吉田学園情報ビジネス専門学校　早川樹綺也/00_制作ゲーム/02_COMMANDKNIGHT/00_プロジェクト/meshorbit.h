//=============================================================================
//
// メッシュオービットのD処理 [meshorbit.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _MESHORBIT_H_
#define _MESHORBIT_H_

#include "main.h"
#include "scene.h"

#define MAX_X (50)
#define MAX_Z (1)

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMeshOrbit : public CScene
{
public:
	//メンバ関数
	CMeshOrbit(int nPriority = 2, OBJTYPE objtype = OBJTYPE_SCENE3D);
	~CMeshOrbit();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxParent(D3DXMATRIX *pMtx);

	//静的メンバ関数
	static CMeshOrbit *Create(void);
private:
	//メンバ変数
	int			m_nNumVertex;
	int			m_nNumPolygon;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXMATRIX	m_mtxWorld;
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9			m_pTexture = NULL;	// テクスチャへのポインタ
	D3DXMATRIX					*m_pMtxParent;		//親のマトリックスへのポインタ
	D3DXVECTOR3					m_aPosVertex[(MAX_X + 1) * (MAX_Z + 1)];	//算出した頂点座標
	D3DXCOLOR					m_aColVertex[(MAX_X + 1) * (MAX_Z + 1)];	//算出した色情報
	D3DXVECTOR2					m_aTexVertexX[(MAX_X + 1) * (MAX_Z + 1)];	//算出したテクスチャ座標
	D3DXVECTOR3					m_aOffset[2];		//両端のオフセット
};
#endif
