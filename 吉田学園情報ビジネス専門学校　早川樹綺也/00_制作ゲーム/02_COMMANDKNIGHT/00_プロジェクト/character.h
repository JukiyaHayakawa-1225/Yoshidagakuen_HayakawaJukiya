//=============================================================================
//
// �L�����N�^�[�̏��� [character.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCharacter : public CScene
{
public:	//�N������A�N�Z�X�\
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
	D3DXVECTOR3	m_pos;		//�ʒu
	D3DXVECTOR3 m_posOld;	//�ߋ��̈ʒu
	D3DXVECTOR3 m_rot;		//����
	D3DXVECTOR3 m_move;		//�ړ���
	int			m_nLife;	//�̗�
	int			m_nMaxLife;	//�̗͍ő�l
	float m_fColRange;		//�����蔻��傫��
};
#endif