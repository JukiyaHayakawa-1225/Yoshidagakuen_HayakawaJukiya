//=============================================================================
//
// オブジェクト3D処理 [object3D.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CBoxCollider;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CObject3D : public CScene
{
public:	//誰からもアクセス可能
	//=========================================================================
	// 構造体の定義
	//=========================================================================
	typedef enum
	{
		OBJ3DTYPE_NONE = 0,
		OBJ3DTYPE_MAX,
	} OBJ3DTYPE;

	//メンバ関数
	CObject3D(int nPriority = 3,OBJTYPE type = OBJTYPE_3D);
	~CObject3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void SetMtxWorld(LPDIRECT3DDEVICE9 pDevice);

	void SetPos(D3DXVECTOR3 pos);
	void SetRot(D3DXVECTOR3 rot);
	void SetBoxCollider(CBoxCollider *pBoxCollider);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetColRange(D3DXVECTOR3 colRange);
	void SetReturnFlag(bool bReturnFlag);

	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXMATRIX GetMtxWorld(void);
	D3DXVECTOR3 GetColRange(void);
	CBoxCollider *GetBoxCollider(void);
	bool GetReturnFlag(void);

	virtual	void Hit(CScene *pScene);

	//静的メンバ関数
	static CObject3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:	//自分だけがアクセス可能
	//メンバ変数
	D3DXVECTOR3					m_pos;								// 位置
	D3DXVECTOR3					m_rot;								// 向き
	D3DXVECTOR3					m_colRange;							// 当たり判定の大きさ
	bool						m_bReturnFlag;						// 当たった際に戻すかどうか
	D3DXMATRIX					m_mtxWorld;							// マトリックス
	CBoxCollider				*m_pBoxCollider;					// 当たり判定箱へのポインタ
};
#endif
