//=============================================================================
//
// キャラクターの処理 [character.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCharacter : public CScene
{
public:	//誰からもアクセス可能
	CCharacter(int nPriority = 3,OBJTYPE objtype = OBJTYPE_SCENEX);
	~CCharacter();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos);
	void SetPosOld(D3DXVECTOR3 posOld);
	void SetRot(D3DXVECTOR3 rot);
	void SetMove(D3DXVECTOR3 move);
	void SetLife(int nLife);
	void SetMaxLife(int nMaxLife);
	void SetColRange(float fColRange);

	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetMove(void);
	int			GetLife(void);
	int			GetMaxLife(void);
	float GetColRange(void);

	void SetDiffAngle(float fDiffAngle);

private:
	D3DXVECTOR3	m_pos;		//位置
	D3DXVECTOR3 m_posOld;	//過去の位置
	D3DXVECTOR3 m_rot;		//向き
	D3DXVECTOR3 m_move;		//移動量
	int			m_nLife;	//体力
	int			m_nMaxLife;	//体力最大値
	float m_fColRange;		//当たり判定大きさ
};
#endif