//=============================================================================
//
// �G�̏��� [enemy.h]
// Author : Jukiya Hayakawa & Hodaka Niwa
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"
#include "character.h"
#include "player.h"
#include "block.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_PRIORITY          (4)
#define ENEMY_SCORE_NORMAL      (100)
#define ENEMY_SCORE_ARMORE      (250)
#define ENEMY_SCORE_FAST        (250)
#define ENEMY_SCORE_HEAVY       (500)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CBullet;
class CPlayer;
class CBlock;
class CHeadQuarters;
class CRiver;

//*****************************************************************************
// �G�N���X�̒�`
//*****************************************************************************
class CEnemy : public CCharacter
{
public:	// �N������A�N�Z�X�\
	//*****************************************************************************
	// �G�̎��
	//*****************************************************************************
	typedef enum
	{
		TYPE_NONE = -1,      // ����
		TYPE_NORMAL,         // �ʏ�
		TYPE_ARMORE,         // ���b��
		TYPE_FAST,           // ���˖C�^���N
		TYPE_HEAVY,          // �w�r�[�^���N
		TYPE_MAX,
	}TYPE;

	//*****************************************************************************
	// AI�̏��
	//*****************************************************************************
	typedef enum
	{
		AI_STATE_NONE = 0,   // �������Ȃ����
		AI_STATE_CHANGEROT,  // ������ς�����
		AI_STATE_MOVE,       // �����Ă�����
		AI_STATE_MAX,
	}AI_STATE;

	// �����o�֐�
	CEnemy(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CEnemy();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(CScene *pScene);
	void SetItemData(const int nItemType);
	void SetAIData(const int nBulletEva, const int nMassEva, const int nDownEva);
	void SetSpawnEffect(void);
	void SetDeathEffect(void);

	void SetIdx(const int nIdx);
	void SetLife(const int nLife);
	void SetType(const TYPE type);
	void SetAI_State(const AI_STATE AI_State);
	void SetAI_StateCounter(const int nStateCounter);
	void SetAI_BulletEva(const int nBulletEva);
	void SetAI_MassEva(const int nMassEva);
	void SetAI_MassCount(const int nMassCount);
	void SetAI_DownEva(const int nDownEva);
	void SetAI_NotDownCount(const int nNotDownCount);

	int GetIdx(void);
	int GetLife(void);
	TYPE GetType(void);
	AI_STATE GetAI_State(void);
	int GetAI_StateCounter(void);
	int GetAI_BulletEva(void);
	int GetAI_MassEva(void);
	int GetAI_MassCount(void);
	int GetAI_DownEva(void);
	int GetAI_NotDownCount(void);

	// �ÓI�����o�֐�
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);
	static void SetNumAll(const int nNumAll);
	static int GetNumAll(void);
	static int GetSpawnCounter(void);
	static void ResetSpawnCounter(void);

protected:	// �����Ɣh���N���X�������A�N�Z�X�\
	// �����o�֐�
	void AI(void);
	void AI_None(void);
	void AI_ChangeRot(void);
	void AI_AutoMove(void);
	void Movement(void);
	void SetBullet(void);
	void Collision(void);

private:	// �����������A�N�Z�X�\
	// �����o�֐�
	void ClearVariable(void);
	void HostUpdate(void);
	void SetNextMoveDirection(void);
	void CalcDestMass(void);
	bool CheckNextMovingMass(void);
	void ResetAIState(void);
	void CollisionCheck(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CScene *pScene, bool *pDeath);
	bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CPlayer *pPlayer);
	bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CEnemy *pEnemy);
	bool CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBullet *pBullet);
	bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBlock *pBlock);
	bool CollisionHeadQuarters(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHeadQuarters *pHead);
	bool CollisionRiver(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CRiver *pRiver);
	void Destroy(void);
	void ScoreUp(CBullet *pBullet);
	void DeleteParts(void);

	// �ÓI�����o�ϐ�
	static int m_nNumAll;
	static int m_nSpawnCounter;

	// �����o�ϐ�
	int m_nIdx;                 // �G�̔ԍ�(�N���C�A���g�Ԃœ���������̂ɕK�v)
	int m_nLife;                // �G�̗̑�
	bool m_bItem;               // �A�C�e���𗎂Ƃ����ǂ���
	int m_nItemType;            // �A�C�e���̔ԍ�
	TYPE m_Type;                // �G�̎��
	AI_STATE m_AIState;         // AI�̏��
	int m_nAI_StateCounter;     // AI�̏�Ԃ��Ǘ�����J�E���^�[
	int m_nAI_MoveCounter;      // �����n�߂Ă���ǂꂭ�炢���Ԃ���������
	int m_nAI_BulletEva;        // �e��łۂ̃����_���]���l
	int m_nAI_MassEva;          // �}�X���ړ������ۂɌ�����ς������邩���肷��Ƃ��̃����_���]���l
	int m_nAI_MassCount;        // �ǂꂭ�炢�}�X���ړ��������𐔂���J�E���^�[
	int m_nAI_DownEva;          // �������ɓ��������ǂ����𔻒肷��ۂ̃����_���]���l
	int m_nAI_NotDownCount;     // �������ɓ����Ȃ������񐔂𐔂���J�E���^�[
	int m_nAI_DestMassX;        // �ړI�n�̃}�X��(������)
	int m_nAI_DestMassZ;        // �ړI�n�̃}�X��(�c����)
	NOW_ROT_INFO m_RotInfoOld;  // �O������Ă�������
};

//*****************************************************************************
// �ʏ�̓G�^���N�N���X�̒�`
//*****************************************************************************
class CEnemyNormal : public CEnemy
{
public:	// �N������A�N�Z�X�\
	// �����o�֐�
	CEnemyNormal(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CEnemyNormal();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CEnemyNormal *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);

protected:	// �����Ɣh���N���X�������A�N�Z�X�\

private:	// �����������A�N�Z�X�\
};


//*****************************************************************************
// ���b�Ԃ̓G�N���X�̒�`
//*****************************************************************************
class CEnemyArmore : public CEnemy
{
public:	// �N������A�N�Z�X�\
	// �����o�֐�
	CEnemyArmore(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CEnemyArmore();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CEnemyArmore *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);

protected:	// �����Ɣh���N���X�������A�N�Z�X�\

private:	// �����������A�N�Z�X�\
};

//*****************************************************************************
// ���˖C�^���N�̓G�N���X�̒�`
//*****************************************************************************
class CEnemyFast : public CEnemy
{
public:	// �N������A�N�Z�X�\
	// �����o�֐�
	CEnemyFast(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CEnemyFast();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CEnemyFast *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);

protected:	// �����Ɣh���N���X�������A�N�Z�X�\

private:	// �����������A�N�Z�X�\
};

//*****************************************************************************
// �w�r�[�^���N�̓G�N���X�̒�`
//*****************************************************************************
class CEnemyHeavy : public CEnemy
{
public:	// �N������A�N�Z�X�\
	// �����o�֐�
	CEnemyHeavy(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CEnemyHeavy();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CEnemyHeavy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);

protected:	// �����Ɣh���N���X�������A�N�Z�X�\

private:	// �����������A�N�Z�X�\
};

#endif
