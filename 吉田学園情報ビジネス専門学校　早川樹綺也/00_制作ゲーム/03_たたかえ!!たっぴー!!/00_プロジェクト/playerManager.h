//=============================================================================
//
// �v���C���[�}�l�[�W���[�̏��� [playerManager.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _PLAYERMANAGER_H_
#define _PLAYERMANAGER_H_

#include "main.h"
#include "scene.h"
#include "player.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;
class CFileLoader;
class CModelCreate;
class CMotionManager;
class CMotion;
class CPlayer;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_STOCK (1)	//�v���C���[�̎c�@��

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPlayerManager
{
public:	//�N������A�N�Z�X�\
	//*************************************************************************
	// �\���̂̒�`
	//*************************************************************************
	typedef enum
	{
		SELECT_PLAYER_0 = 0,
		SELECT_PLAYER_1,
		SELECT_PLAYER_2,
		SELECT_PLAYER_3,
		SELECT_PLAYER_MAX,
	}SELECT_PLAYER;

	//�����o�֐�
	CPlayerManager();
	~CPlayerManager();
	HRESULT Init(char *PlayerFileName);
	void Uninit(void);
	CPlayer *SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdx, int nPriority = 3);

	CModel **GetModel(void);
	CMotionManager *GetMotionManager(void);
	int GetParts(void);

	//�ÓI�����o�֐�
	static CPlayerManager *Create(char *PlayerFileName);

private:	//�����������A�N�Z�X�\
	//�����o�֐�
	HRESULT LoadPlayer(char *pStr, CFileLoader *pFileLoader,int nPlayerIdx);
	CModel **ReadCharacterSet(char *pStr, CFileLoader *pFileLoader, int *pNumParts, int *pParent,int nPlayerIdx);
	CModel *ReadParts(char *pStr, CFileLoader *pFileLoader, int *pParent, CModel **apModel,CModelCreate *pModelCreate);
	CMotion *ReadMotionSet(char *pStr, CFileLoader *pFileLoader, int *pNumParts, CMotion *pMotion, int nCntMotion);
	void ReadKeySet(char *pStr, CFileLoader *pFileLoader, int nCntKeySet, int *pPlayBackKey, D3DXVECTOR3 *pAddPos, D3DXVECTOR3 *pDestRot);
	void ReadKey(char *pStr, CFileLoader *pFileLoader, int nCntKey, D3DXVECTOR3 *pAddPos, D3DXVECTOR3 *pDestPos);
	void CreatePlayer(CModel **apModel, CModelCreate *pModelCreate, CMotionManager *pMotionManager,int nNumParts, int *pParent,int nPlayer);

	//�����o�ϐ�
	int m_nNumPlayer;
	CModelCreate **m_pModelCreate;				//���f�������̃|�C���^���
	int *m_pParent;                             // �e���f���̔ԍ�(��Ŕz�u���邽�߂Ɏg��)
	CModel **m_apModel;							//���f���̃|�C���^���
	CMotionManager *m_pMotionManager;			//���[�V�����}�l�[�W���[�̃|�C���^���
	//CMotion *m_pMotion[CPlayer::MOTION_MAX];	//���[�V�����̃|�C���^���
	int m_nNumParts;
};
#endif
