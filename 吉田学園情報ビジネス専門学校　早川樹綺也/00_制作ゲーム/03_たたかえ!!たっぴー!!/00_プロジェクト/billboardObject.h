//*****************************************************************************
//
//     �z�u�r���{�[�h�̏���[billboardObject.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _BILLBOARDOBJECT_H_
#define _BILLBOARDOBJECT_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "sceneBillboard.h"

//*****************************************************************************
//    �z�u�r���{�[�h�N���X�̒�`
//*****************************************************************************
class CBillboardObject : public CSceneBillboard
{
public:    // �N�ł��A�N�Z�X�\
	CBillboardObject(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJBILLBOARD);
	~CBillboardObject();

	static CBillboardObject *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, float fColRange = 0.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, float fColRange = 0.0f);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);

	void SetTexIdx(const int nTexIdx);
	void SetDrawAddtive(const bool bDrawAddtive);
	void SetColRange(const float fColRange);

	int GetTexIdx(void);
	bool GetDrawAddtive(void);
	float GetColRange(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int   m_nTexIdx;       // �g�p���Ă���e�N�X�`���̔ԍ�
	bool  m_bDrawAddtive;  // ���Z�����ŕ`�悷�邩���Ȃ���
	float m_fColRange;     // �����蔻������͈�
};

#endif