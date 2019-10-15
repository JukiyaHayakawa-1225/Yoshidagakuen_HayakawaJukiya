//*****************************************************************************
//
//     �L�����N�^�[�Ǌ��̏���[characterManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _CHARACTERMANAGER_H_
#define _CHARACTERMANAGER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CFileLoader;
class CMotionAttack;
class CMotionCollision;
class CMotionOrbit;
class CMotion;
class CMotionManager;
class CModelCreate;
class CModel;
class CTextureManager;
class CObject;
class CEnemy;

//*****************************************************************************
//    �L�����N�^�[�Ǌ��N���X�̒�`
//*****************************************************************************
class CCharacterManager
{
public:    // �N�ł��A�N�Z�X�\
	CCharacterManager();
	~CCharacterManager();

	static CCharacterManager *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	CObject *SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);
	CEnemy *SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);

	void SetFileName(char *pFileName);
	void SetModelCreate(CModelCreate *pModelCreate);

	char *GetFileName(void);
	CModelCreate *GetModelCreate(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	HRESULT LoadScript(CFileLoader *pFileLoader, char *pStr);
	HRESULT LoadTextureSet(CFileLoader *pFileLoader, char *pStr);
	HRESULT LoadTexture(char *pStr, int nCntTex);
	HRESULT LoadModelSet(CFileLoader *pFileLoader, char *pStr);
	HRESULT LoadModel(char *pStr, int nCntModel);
	HRESULT LoadCharacterSet(CFileLoader *pFileLoader, char *pStr);
	HRESULT LoadPartsSet(CFileLoader *pFileLoader, char *pStr, int nCntParts);
	HRESULT LoadMotionSet(CFileLoader *pFileLoader, char *pStr, CMotion **apMotion);
	CMotionAttack *LoadMotionAttackSet(CFileLoader *pFileLoader, char *pStr);
	CMotionCollision **LoadMotionCollisionSet(CFileLoader *pFileLoader, char *pStr, int *pNumColData);
	CMotionOrbit **LoadMotionOrbitSet(CFileLoader *pFileLoader, char *pStr, int *pNumOrbitData);
	HRESULT LoadKeySet(CFileLoader *pFileLoader, char *pStr, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);
	HRESULT LoadKey(CFileLoader *pFileLoader, char *pStr, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);

	char            m_aFileName[256];    // �L�����N�^�[�f�[�^�̃X�N���v�g�t�@�C����
	int             m_nNumMotionData;    // �ǂݍ��񂾃��[�V�����f�[�^�̐�
	int             m_nNumModel;         // �ǂݍ��ރ��f����
	int             m_nNumTexture;       // �ǂݍ��ރe�N�X�`���̐�
	int             *m_pParent;          // �e���f���̔ԍ�(��Ŕz�u���邽�߂Ɏg��)
	CModel          **m_apModel;         // ���f���N���X�ւ̃|�C���^
	CMotionManager  *m_pMotionManager;   // ���[�V�����Ǘ��N���X�ւ̃|�C���^
	CModelCreate    *m_pModelCreate;     // ���f���Ǌ��N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager;  // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^

	typedef struct
	{
		float fAccel;
		float fInertia;
		float fRivisionRot;
		int nNumParts;
	}CHARA_DATA;
	CHARA_DATA      m_CharaData;         // �L�����N�^�[�f�[�^
};

#endif