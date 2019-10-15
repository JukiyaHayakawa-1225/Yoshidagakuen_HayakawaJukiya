//*****************************************************************************
//
//     �z�u���̏���[object.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _OBJECT_H_
#define _OBJECT_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "object3D.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CModel;
class CMotionManager;

//*****************************************************************************
//    �z�u���N���X�̒�`
//*****************************************************************************
class CObject : public CObject3D
{
public:    // �N�ł��A�N�Z�X�\
	CObject(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECT);
	~CObject();

	static CObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ModelDraw(void);

	void SetType(const int nType);
	void SetNumModel(const int nNumModel);
	void SetModel(CModel **apModel);
	void SetModel(CModel *pModel, int nIdx);
	void SetMotionManager(CMotionManager *pMotionManager);
	void SetAlpha(const float fAlpha);

	int GetType(void);
	int GetNumModel(void);
	CModel **GetModel(void);
	CModel *GetModel(int nIdx);
	CMotionManager *GetMotionManager(void);
	float GetAlpha(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void ReleaseModel(void);
	void ReleaseMotionManager(void);

	int            m_nType;            // ��ޔԍ�
	int            m_nNumModel;        // �g�p���Ă��郂�f���̐�
	CModel         **m_apModel;        // ���f���N���X�ւ̃|�C���^
	CMotionManager *m_pMotionManager;  // ���[�V�����Ǌ��N���X�ւ̃|�C���^
	float          m_fAlpha;           // �z�u���̓����x
};

#endif