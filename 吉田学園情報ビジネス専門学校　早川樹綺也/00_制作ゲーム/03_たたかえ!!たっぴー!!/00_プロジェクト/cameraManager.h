//*****************************************************************************
//
//     �J�����Ǌ��̏���[cameraManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _CAMERAMANAGER_H_
#define _CAMERAMANAGER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CCamera;

//*****************************************************************************
//    �J�����Ǌ��N���X�̒�`
//*****************************************************************************
class CCameraManager
{
public:    // �N�ł��A�N�Z�X�\
	CCameraManager();
	~CCameraManager();

	static CCameraManager *Create(int nNumCamera = 1);

	HRESULT Init(int nNumCamera = 1);
	void Uninit(void);
	void Update(void);
	void BindCamera(int nIdxCamera = 0);

	void SetNumCamera(const int nNumCamera);
	void SetCamera(CCamera *pCamera, const int nIdxCamera = 0);

	int GetNumCamera(void);
	CCamera *GetCamera(const int nIdxCamera = 0);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int m_nNumCamera;        // �J�����̐�
	CCamera **m_apCamera;    // �J�����N���X�ւ̃|�C���^
};

#endif