//*****************************************************************************
//
//     �e�̏���[bullet.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _BULLET_H_
#define _BULLET_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "object3D.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define BULLET_PRIORITY (4)

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CModel;
class CPlayer;
class CEnemy;
class CBlock;
class CHeadQuarters;

//*****************************************************************************
//    �e�N���X�̒�`
//*****************************************************************************
class CBullet : public CObject3D
{
public:    // �N�ł��A�N�Z�X�\
	//------------------
	// �e�̎��
	//------------------
	typedef enum
	{
		TYPE_PLAYER_0 = 0,
		TYPE_PLAYER_1,
		TYPE_ENEMY,
		TYPE_MAX,
	}TYPE;

	CBullet(int nPriority = 3, OBJTYPE objType = OBJTYPE_BULLET);
	~CBullet();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindModel(LPD3DXMESH pMesh = NULL, LPD3DXBUFFER pBuffMat = NULL, DWORD nNumMat = 0, LPDIRECT3DTEXTURE9 *pTexture = NULL);
	void Hit(CScene *pScene);

	void SetPosOld(const D3DXVECTOR3 posold);
	void SetMove(const D3DXVECTOR3 move);
	void SetIdx(const int nIdx);
	void SetType(const TYPE type);
	void SetParent(CScene *pParent);
	void SetModel(CModel *pModel);

	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetMove(void);
	int GetIdx(void);
	TYPE GetType(void);
	CScene *GetParent(void);
	CModel *GetModel(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void Move(void);
	void SetMoveEffect(void);
	void SetBoxColliderPos(void);
	virtual void Collision(void) = 0;
	virtual void Destroy(void) = 0;

private:   // ���̃N���X�������A�N�Z�X�\
	D3DXVECTOR3 m_PosOld;   // �O��̒e�̈ʒu
	D3DXVECTOR3 m_Move;     // �ړ���
	int m_nIdx;             // �ԍ�(�T�[�o�[�ŊǗ�����Ƃ��ɕK�v)
	TYPE m_Type;            // ���
	CScene *m_pParent;      // �e�𔭎˂����I�u�W�F�N�g�ւ̃|�C���^
	CModel *m_pModel;       // ���f���N���X�ւ̃|�C���^
};

//*****************************************************************************
//    �v���C���[�̒e�N���X�̒�`
//*****************************************************************************
class CBulletPlayer : public CBullet
{
public:    // �N�ł��A�N�Z�X�\
	CBulletPlayer(int nPriority = 3, OBJTYPE objType = OBJTYPE_BULLET);
	~CBulletPlayer();

	static CBulletPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, TYPE type, CScene *pParent = NULL,
		LPD3DXMESH pMesh = NULL, LPD3DXBUFFER pBuffMat = NULL, DWORD nNumMat = 0, LPDIRECT3DTEXTURE9 *pTexture = NULL, int nPriority = 3);
	static int GetNumAll_0(void);
	static int GetNumAll_1(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void Collision(void);
	void Destroy(void);

private:   // ���̃N���X�������A�N�Z�X�\
	static int m_nNumAll_0;
	static int m_nNumAll_1;

	void CollisionCheck(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CScene *pScene, bool *pDeath);
	bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CPlayer *pPlayer);
	bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CEnemy *pEnemy);
	bool CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBullet *pBullet);
	bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBlock *pBlock);
	bool CollisionHeadQuarters(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHeadQuarters *pHead);
};

//*****************************************************************************
//    �G�̒e�N���X�̒�`
//*****************************************************************************
class CBulletEnemy : public CBullet
{
public:    // �N�ł��A�N�Z�X�\
	CBulletEnemy(int nPriority = 2, OBJTYPE objType = OBJTYPE_BULLET);
	~CBulletEnemy();

	static CBulletEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, CScene *pParent = NULL,
		LPD3DXMESH pMesh = NULL, LPD3DXBUFFER pBuffMat = NULL, DWORD nNumMat = 0, LPDIRECT3DTEXTURE9 *pTexture = NULL, int nPriority = 3);
	static int GetNumAll(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void Collision(void);
	void Destroy(void);

private:   // ���̃N���X�������A�N�Z�X�\
	static int m_nNumAll;

	void CollisionCheck(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CScene *pScene, bool *pDeath);
	bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CPlayer *pPlayer);
	bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CEnemy *pEnemy);
	bool CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBullet *pBullet);
	bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBlock *pBlock);
	bool CollisionHeadQuarters(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHeadQuarters *pHead);
};

#endif