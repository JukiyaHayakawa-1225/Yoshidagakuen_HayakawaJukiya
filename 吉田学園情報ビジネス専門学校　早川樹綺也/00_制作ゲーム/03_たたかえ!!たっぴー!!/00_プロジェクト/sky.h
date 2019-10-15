//*****************************************************************************
//
//     ��̏���[sky.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SKY_H_
#define _SKY_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "meshSphere.h"

//*****************************************************************************
//    ��N���X�̒�`
//*****************************************************************************
class CSky : public CMeshSphere
{
public:    // �N�ł��A�N�Z�X�\
	CSky(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHSPHERE);
	~CSky();
	static CSky *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, int nTexIdx, float fRollSpeed, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetTexIdx(const int nTexIdx);
	void SetRollSpeed(const float fRollSpeed);

	int GetTexIdx(void);
	float GetRollSpeed(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int m_nTexIdx;       // �g�p���Ă���e�N�X�`���ԍ�
	float m_fRollSpeed;  // ��]����X�s�[�h
};

#endif