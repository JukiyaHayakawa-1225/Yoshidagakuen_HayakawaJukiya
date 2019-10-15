//=============================================================================
//
// 氷柱の処理 [icicle.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _ICICLE_H_
#define _ICICLE_H_

#include "main.h"
#include "scene.h"
#include "sceneX.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CIcicle : public CSceneX
{
public:	//誰からもアクセス可能
	typedef enum
	{
		ICICLE_RIGIHT = 0,
		ICICLE_LEFT,
		ICICLE_MAX,
	}ICICLE_TYPE;
	CIcicle();
	~CIcicle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CIcicle *Create(D3DXVECTOR3 pos, ICICLE_TYPE Type);
	static HRESULT Load(void);
	static void Unload(void);

private:	//自分だけがアクセス可能
	static LPD3DXMESH			m_pMesh;					//メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMat;					//マテリアル情報へのポインタ
	static DWORD				m_nNumMat;					//マテリアル情報の数
	static LPDIRECT3DTEXTURE9	m_pTexture;				//テクスチャのポインタ
	int m_nCounter;
	ICICLE_TYPE m_type;
};
#endif
