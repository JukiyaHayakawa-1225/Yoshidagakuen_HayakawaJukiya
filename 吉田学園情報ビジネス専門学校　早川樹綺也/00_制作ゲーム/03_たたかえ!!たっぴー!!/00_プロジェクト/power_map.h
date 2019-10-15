//=============================================================================
//
// ���͐}�̏��� [power_map.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _POWER_MAP_H_
#define _POWER_MAP_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;
class CTextureManager;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPowerMap : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	//�����o�֐�
	CPowerMap(int nPriority = 3, OBJTYPE objtype = OBJTYPE_POWERMAP);
	~CPowerMap();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddGauge(void);
	void ResetGauge(void);

	void SetPlayerIcon(CScene2D *pPlayerIcon);
	void SetEnemyIcon(CScene2D *pEnemyIcon);
	void SetStraw(CScene2D *pStraw);
	void SetStrawContents(CScene2D *pStrawContents);
	void SetTextureManager(CTextureManager *pTextureManager);

	CScene2D *GetPlayerIcon(void);
	CScene2D *GetEnemyIcon(void);
	CScene2D *GetStraw(void);
	CScene2D *GetStrawContents(void);
	CTextureManager *GetTextureManager(void);

	// �ÓI�����o�֐�
	static CPowerMap *Create(CTextureManager *pTextureManager, int nPriority = 3);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void ClearVariable(void);

	void CreatePlayerIcon(void);
	void CreateEnemyIcon(void);
	void CreateStraw(void);
	void CreateStrawContents(void);

	void ReleasePlayerIcon(void);
	void ReleaseEnemyIcon(void);
	void ReleaseStraw(void);
	void ReleaseStrawContents(void);

	CScene2D        *m_pPlayerIcon;
	CScene2D        *m_pEnemyIcon;
	CScene2D        *m_pStraw;
	CScene2D        *m_pStrawContents;
	CTextureManager *m_pTextureManager;
	float           m_fPlayerIconDestPosX;
};

#endif