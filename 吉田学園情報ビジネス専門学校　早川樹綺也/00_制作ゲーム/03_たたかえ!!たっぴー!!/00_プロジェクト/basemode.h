//=============================================================================
//
// �x�[�X���[�h�̏���[basemode.h]
// Author : Jukiya Hayakawa & Hodaka Niwa
//
//=============================================================================
#ifndef _BASEMODE_H_
#define _BASEMODE_H_

#include "main.h"

//�O���錾
class CMap;
class CCameraManager;
class CModelCreate;
class CTextureManager;
class CEffectManager;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CBasemode
{
public:	//�N������A�N�Z�X�\
		//�����o�֐�
	CBasemode();
	~CBasemode();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void);
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	void SetMap(CMap *pMap);
	void SetCameraManager(CCameraManager *pCameraManager);
	void SetModelCreate(CModelCreate *pModelCreate);
	void SetTextureManager(CTextureManager *pTextureManager);
	void SetEffectManager(CEffectManager *pEffectManager);

	CMap *GetMap(void);
	CCameraManager *GetCameraManager(void);
	CModelCreate *GetModelCreate(void);
	CTextureManager *GetTextureManager(void);
	CEffectManager *GetEffectManager(void);

	void ReleaseMap(void);
	void ReleaseCameraManager(void);
	void ReleaseModelCreate(void);
	void ReleaseTextureManager(void);
	void ReleaseEffectManager(void);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:	// �����������A�N�Z�X�\
	CMap *m_pMap;			            // �}�b�v�̃|�C���^
	CCameraManager *m_pCameraManager;   // �J�����Ǌ��N���X�ւ̃|�C���^
	CModelCreate   *m_pModelCreate;     // ���f���Ǌ��N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager; // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	CEffectManager *m_pEffectManager;   // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
};
#endif