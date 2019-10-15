//=============================================================================
//
// モデルの処理 [model.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CModel
{
public:	//誰からもアクセス可能
	//メンバ関数
	CModel();
	~CModel();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPos(D3DXVECTOR3 pos);
	void SetRot(D3DXVECTOR3 rot);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	void SetMesh(LPD3DXMESH mesh);
	void SetBuffMat(LPD3DXBUFFER buffmat);
	void SetNumMat(DWORD nummat);
	void BindX(LPD3DXMESH mesh, LPD3DXBUFFER buffmat, DWORD nummat);
	void SetParent(CModel *pModel);
	D3DXMATRIX &GetMtxWorld(void);
	void SetSize(D3DXVECTOR3 size);

	//静的メンバ関数
	static CModel *Create(D3DXVECTOR3 pos);

private:	//自分だけがアクセス可能
	//メンバ変数
	LPD3DXMESH			m_pMesh = NULL;				//メッシュ情報のポインタ
	LPD3DXBUFFER		m_pBuffMat = NULL;			//マテリアル情報のポインタ
	DWORD				m_pNumMat = 0;				//マテリアル情報の数
	D3DXVECTOR3         m_pVtxMax;					// 頂点座標の最大値のポインタ
	D3DXVECTOR3         m_pVtxMin;					// 頂点座標の最小値のポインタ
	D3DXMATRIX			m_mtxWorld;					//ワールドマトリックス
	D3DXVECTOR3			m_pos;						//位置
	D3DXVECTOR3			m_rot;						//向き
	int					m_nIdx;						//インデックス番号
	CModel				*m_pParent;					//親子のポインタ
	D3DXVECTOR3			m_size;
};
#endif
