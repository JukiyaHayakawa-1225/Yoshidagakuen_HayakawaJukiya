//*****************************************************************************
//
//     ���C�g�Ǌ��̏���[lightManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _LIGHTMANAGER_H_
#define _LIGHTMANAGER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CLight;

//*****************************************************************************
//    ���C�g�Ǌ��N���X�̒�`
//*****************************************************************************
class CLightManager
{
public:    // �N�ł��A�N�Z�X�\
	CLightManager();
	~CLightManager();

	static CLightManager *Create(int nNumLight = 3);
	static CLightManager *Cpy(CLightManager *pLightManagerOld, int nNumLight);

	HRESULT Init(int nNumLight = 3);
	void Uninit(void);
	void Update(void);
	void SettingLight(CLight *pLight, const int nIdx);
	void SwitchLight(const int nIdx);

	void SetNumLight(const int nNumLight);

	int GetNumLight(void);
	CLight *GetLight(const int nIdx);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	CLight *GetCpyLight(int nIdx);

	int    m_nNumLight;     // ���C�g�̐�
	CLight **m_apLight;     // ���C�g�N���X�ւ̃|�C���^
};

#endif