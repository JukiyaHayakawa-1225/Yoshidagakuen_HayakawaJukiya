//*****************************************************************************
//
//     ���X�|�[���̏���[respawn.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RESPAWN_H_
#define _RESPAWN_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "object3D.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CModel;

//*****************************************************************************
//    ���X�|�[���N���X�̒�`
//*****************************************************************************
class CRespawn : public CObject3D
{
public:    // �N�ł��A�N�Z�X�\
	CRespawn();
	~CRespawn();

	static CRespawn *Create(int nAreaX, int nAreaZ);

	HRESULT Init(int nAreaX, int nAreaZ);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetModel(CModel *pModel);
	void SetArea(int nAreaX, int nAreaZ);

	CModel *GetModel(void);
	int GetAreaX(void);
	int GetAreaZ(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int m_nAreaX, m_nAreaZ; // ���ǂ̃G���A�ɂ��邩�𐔒l����������
	CModel *m_pModel;       // ���f���N���X�^�̃|�C���^
};

#endif