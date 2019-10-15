//*****************************************************************************
//
//     �S�[���p�̉~������[goalCylinder.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _GOALCYLINDER_H_
#define _GOALCYLINDER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "meshCylinder.h"

//*****************************************************************************
//    �S�[���p�̉~���N���X�̒�`
//*****************************************************************************
class CGoalCylinder : public CMeshCylinder
{
public:    // �N�ł��A�N�Z�X�\
	CGoalCylinder(int nPriority = 3, OBJTYPE objType = OBJTYPE_GOALCYLINDER);
	~CGoalCylinder();
	static CGoalCylinder *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, float fDecay, float fMag, float fUpValue, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, float fDecay, float fMag, float fUpValue);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	bool Collision(D3DXVECTOR3 *pPos);

	void SetDecay(const float fDecay);
	void SetMag(const float fMag);
	void SetUpValue(const float fUpValue);

	float GetDecay(void);
	float GetMag(void);
	float GetUpValue(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	float m_fDecay;     // �����x�̌����l
	float m_fMag;       // ���݂̔{��
	float m_fUpValue;   // �{�������Z�����
};

#endif