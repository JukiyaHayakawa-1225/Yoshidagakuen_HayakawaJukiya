//=============================================================================
//
// �`���[�g���A���̏��� [tutorial.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "basemode.h"
#include "item.h"
#include "enemy.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CFileLoader;
class CFileSaver;
class CScene2D;
class CScene2DFlash;
class CPlayer;
class CPlayerManager;
class CEnemy_ListData;
class CBullet;
class CBlock;
class CCharacterManager;
class CGoalCylinder;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTutorial : public CBasemode
{
public:	// �N������A�N�Z�X�\
	//--------------
	// �X�L�b�v�@�\
	//--------------
	typedef enum
	{
		SKIP_NONE = -1,
		SKIP_YES,
		SKIP_NO,
		SKIP_MAX
	}SKIP;

	//--------------
	// ���
	//--------------
	typedef enum
	{
		STATE_NONE = -1,
		STATE_SKIP_CHECK,
		STATE_SKIP_END,
		STATE_NORMAL,
		STATE_GOAL,
		STATE_MAP_CHANGE,
		STATE_END,
		STATE_MAX
	}STATE;

	// �����o�֐�
	CTutorial();
	~CTutorial();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DeletePlayer(CPlayer *pPlayer, const int nIdx);
	void DeleteBlock(const int nIdx);
	void DeleteEnemy(const int nIdx);
	void HitBullet(void);
	void CreateBreakBlockInfo(void);
	void CreateAttackPlayerInfo(void);
	void CreateNearGoalInfo(void);
	void SetGoalPlayerIdx(const int nIdx);

	void SetBulletModel(CBullet *pBullet);
	void SetPlayer(CPlayer *pPlayer, const int nIdx);
	void SetState(const STATE state);

	CPlayer *GetPlayer(const int nIdx);
	STATE GetState(void);

	// �ÓI�����o�֐�
	static CTutorial *Create(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:	// �����������A�N�Z�X�\
	void ClearVariable(void);
	void CheckBreakBlockInfo(void);
	void CheckAttackPlayerInfo(void);
	void CheckNearGoalInfo(void);

	// �����p�֐�
	void CreateTexture(int nNumTex);
	void CreateCamera(void);
	void CreateMap(void);
	void CreateMapFilePointer(void);
	void CreatePlayer(void);
	CEnemy *CreateEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CEnemy_ListData *pEnemyData);
	void CreateOpeInfo(void);


	// �J���p�֐�
	void ReleaseMapFilePointer(void);
	void ReleaseSkipLogo(void);
	void ReleaseSkipCheckLogo(void);
	void ReleasePlayer(void);
	void ReleasePlayerManager(void);
	void ReleaseEnemyManager(void);
	void ReleaseOpeInfo(void);
	void ReleaseBreakBlockInfo(void);
	void ReleaseAttackPlayerInfo(void);
	void ReleaseNearGoalInfo(void);


	// �T�[�o�[�Ƃ̃��b�Z�[�W�����p�֐�
	void SetDataToServer(void);
	void SetDataToServerFromPlayer(void);
	void SetDataToServerFromGame(void);
	void SetDataToServerFromEnemy(void);
	void SetDataToServerFromEnemyBullet(void);
	void SetDataToServerFromPlayerBullet(void);
	void SetDataToServerFromDeleteBlock(void);
	void SetDataToServerFromDeleteEnemy(void);
	void SetDataToServerFromHitBullet(void);
	char *GetDataToEnemy(CEnemy *pEnemy, char *pStr);
	char *GetDataToEnemyBullet(CBullet *pBullet, char *pStr);
	char *GetDataToPlayerBullet(CBullet *pBullet, char *pStr);
	void GetDataFromServer(void);
	char *SetDataToPlayerFromServer(char *pStr);
	char *SetDataToGameFromServer(char *pStr);
	char *SetDataToEnemyFromServer(char *pStr);
	char *SetDataToEnemy(CEnemy *pEnemy, char *pStr);
	char *SetDataToCreateEnemy(char *pStr);
	void ReleaseEnemy(int nNumEnemy);
	void ReleaseCheckEnemy(CEnemy *pEnemy, int *pNumEnemy);
	char *SetDataToPlayerBulletFromServer(char *pStr);
	void ReleasePlayerBullet(int nNumBullet);
	void ReleaseCheckPlayerBullet(CBullet *pBullet, int *pNumBullet);
	char *SetDataToPlayerBullet(CBullet *pBullet, char *pStr, int *pNumBullet);
	char *SetDataToCreatePlayerBullet(char *pStr);
	char *SetDataToEnemyBulletFromServer(char *pStr);
	void ReleaseEnemyBullet(int nNumBullet);
	void ReleaseCheckEnemyBullet(CBullet *pBullet, int *pNumBullet);
	char *SetDataToEnemyBullet(CBullet *pBullet, char *pStr, int *pNumBullet);
	char *SetDataToCreateEnemyBullet(char *pStr);
	char *SetDataToDeleteBlock(char *pStr);
	void ReleaseCheckBlock(CBlock *pBlock, int *pDeleteIdx, int *nNumDeleteBlock);
	char *SetDataToDeleteEnemy(char *pStr);
	void ReleaseCheckDeleteEnemy(CEnemy *pEnemy, int *pDeleteIdx, int *nNumDeleteEnemy);
	char *SetDataToHitBullet(char *pStr);


	// ��Ԃɂ�鏈�������p�֐�
	void SkipCheckUpdate(void);
	void SkipEndUpdate(void);
	void NormalUpdate(void);
	void GoalUpdate(void);
	void MapChangeUpdate(void);
	void EndUpdate(void);
	void EndCameraMoving(void);
	void ChangeNoneSkipCheckPolygon(int nSelect);
	void ChangeSelectSkipCheckPolygon(int nSelect);


	// �X�N���v�g�ǂݍ��ݗp�֐�
	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadTexFileName(char *pStr, int nCntTex);
	void LoadModel(char *pStr, int nCntModel);
	void LoadEffectFileName(char *pStr);
	void LoadMapFileName(char *pStr, int nCntMap);
	void LoadPlayerFileName(char *pStr, int nCntPlayer);
	void LoadSkipLogo(CFileLoader *pFileLoader, char *pStr);
	void LoadSkipCheckLogo(CFileLoader *pFileLoader, char *pStr, int nCntSkipCheck);


	// �Q�[�����X�|�[�������p�֐�
	void SetPlayerPosToSpawn(void);
	void CheckPlayerResSpawn(int nCntPlayer);
	void CheckEnemySpawn(int nTime);
	void EnemySpawn(CMap *pMap, CEnemy_ListData *pEnemyData, int nCnt);
	void CalcStageDigits(void);

	// �}�b�v��ς���p�֐�(�f�o�b�O�p�̊֐�����)
	void ResetCounter(void);
	void ChangeNextMap(void);
	void ChangePrevMap(void);
	void DeleteGameObject(void);

	STATE m_State;                              // ���݂̏��
	STATE m_StateOld;                           // �O��̏��
	int m_nStateCounter;                        // ��Ԃ��Ǘ�����J�E���^�[
	int m_nNumMap;                              // �}�b�v��ǂݍ��ސ�
	int m_nMapIdx;                              // ���݂̃}�b�v�ԍ�
	char **m_pMapFileName;                      // �ǂݍ��ރ}�b�v�̃t�@�C����
	CScene2D *m_pSkipLogo;                      // �X�L�b�v���邩��\�����郍�S�|���S��
	CScene2DFlash *m_pSkipCheckLogo[SKIP_MAX];  // �X�L�b�v�m�F���� �͂� ������ ��\������p�̃��S�|���S��
	int m_nSelectSkip;                          // �X�L�b�v���邩�ǂ���
	int m_nTutorialCounter;                     // �`���[�g���A���̎��Ԃ𐔂���J�E���^�[
	int m_nSpawnEnemyCount;                     // �X�|�[�������G�̐��𐔂���J�E���^�[
	int m_nNumEnemy;                            // �o������G�̐�
	int m_nNumberTexIdx;                        // �����|���S�����g�p����e�N�X�`���̔ԍ�
	int m_nNumNumberDigits;                     // ���݂̃X�e�[�W�ԍ��̌���
	int m_nBulletModelIdx;                      // �e���g�p���郂�f���̔ԍ�
	CGoalCylinder *m_pGoalCylinder;             // �S�[���p�~���N���X�ւ̃|�C���^
	CScene2D *m_pOpeInfo;                       // ������@�\���p�|���S��

	// ���X�|���X�p
	CScene2D *m_pBreakBlockInfo;
	int m_nCntBreakBlockInfoDisp;
	CScene2D *m_pAttackPlayerInfo;
	int m_nCntAttackInfoDisp;
	CScene2D *m_pNearGoalInfo;
	int m_nCntNearGoalInfoDisp;

	// �v���C���[�p
	CPlayer *m_pPlayer[MAX_NUM_PLAYER];
	CPlayerManager *m_pPlayerManager[MAX_NUM_PLAYER];
	int m_nPlayerRespawnCounter;
	int m_nGoalPlayIdx;

	// �G�f�[�^�p
	CCharacterManager *m_pEnemyManager[CEnemy::TYPE_MAX];

	// �v���C���[�̓����ɕK�v
	bool m_bDeletePlayerFlag;

	// �u���b�N�̓����ɕK�v
	int m_nNumDeleteBlock;
	char m_aDeleteBlock[2048];

	// �G�̓����ɕK�v
	int m_nNumDeleteEnemy;
	char m_aDeleteEnemy[2048];

	// �v���C���[���m�̒e�q�b�g�̓����ɕK�v
	bool m_bHitBulletFlag;
};
#endif