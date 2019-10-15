//=============================================================================
//
// �N���[���ɂ̏��� [mist.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _MIST_H_
#define _MIST_H_

#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCreamMist : public CScene2D
{
public:    // �N�ł��A�N�Z�X�\
	//�����o�֐�
	CCreamMist(int nPriority = 3, OBJTYPE objtype = OBJTYPE_MIST);
	~CCreamMist();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CCreamMist *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority = 3, float fRot = 0.0f, float fStartTexU = 0.0f, float fStartTexV = 0.0f, float fAddTexU = 1.0f, float fAddTexV = 1.0f);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
};

#endif