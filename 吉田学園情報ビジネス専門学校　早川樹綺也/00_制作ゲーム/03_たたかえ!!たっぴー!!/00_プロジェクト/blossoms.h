//=============================================================================
//
// �Ԃт�̏��� [blossoms.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _BLOSSOMS_H_
#define _BLOSSOMS_H_

#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CBlossoms : public CScene2D
{
public:    // �N�ł��A�N�Z�X�\
		   //�����o�֐�
	CBlossoms(int nPriority = 3, OBJTYPE objtype = OBJTYPE_BLOSSOMS);
	~CBlossoms();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetMove(const D3DXVECTOR3 move);
	void SetAngleSpeed(const float fAngleSpeed);
	void SetRotSpeed(const float fRotSpeed);

	D3DXVECTOR3 GetMove(void);
	float GetAngleSpeed(void);
	float GetRotSpeed(void);

	// �ÓI�����o�֐�
	static CBlossoms *Create(D3DXVECTOR3 move, float fAngleSpeed, float fRotSpeed, D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority = 3, float fRot = 0.0f, float fStartTexU = 0.0f, float fStartTexV = 0.0f, float fAddTexU = 1.0f, float fAddTexV = 1.0f);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	bool CheckOutOfScreen(D3DXVECTOR3 pos);

	D3DXVECTOR3 m_Move;     // �ړ���
	float m_fAngleSpeed;    // �p�x����]������X�s�[�h
	float m_fRotSpeed;      // ��������]������X�s�[�h
};

#endif