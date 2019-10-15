//=============================================================================
//
// �v���C���[�̏��� [player.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene.h"
#include "character.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_MAX_BULLET   (2)     // �v���C���[�����Ă�ő�e��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObject3D;
class CBullet;
class CEnemy;
class CBlock;
class CItem;
class CGoalCylinder;
class CRiver;
class CIceField;
class CHeadQuarters;
class CHinaarare;
class CBulletPlayer;
class CInputKeyboard;
class CXInput;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPlayer : public CCharacter
{
public:	//�N������A�N�Z�X�\
	//*************************************************************************
	// �\���̂̒�`
	//*************************************************************************
	typedef enum
	{
		MOTION_NEUTAL = 0,
		MOTION_MOVE,
		MOTION_MAX,
	}MOTION;

	typedef enum
	{
		STATE_NOMAL = 0,//�ʏ���
		STATE_STOP,		//��~���
		STATE_RESPAWN,	//���X�|�[�����
		STATE_DEATH,	//���S���
		STATE_MAX,
	}STATE;

	typedef enum
	{
		PLAYER_ABILITY_NOMAL = 0,		//�ʏ�
		PLAYER_ABILITY_BULLET_SPEEDUP,	//�e�X�s�[�h�A�b�v
		PLAYER_ABILITY_MOVE_SPEEDUP,	//�ړ����x�A�b�v
		PLAYER_ABILITY_DOUBLEBULLET,	//�e�Q�����Ă�
		PLAYER_ABILITY_ALLBLOCKDESTROY,	//�S�Ẵu���b�N������
		PLAYER_ABILITY_MAX,
	}PLAYER_ABILITY;

	//�����o�֐�
	CPlayer(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYER);
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(CScene *pScene);
	void SetDeathEffect(void);
	void SwitchAbility(void);

	void SetPlayer(CPlayer *pPlayer);
	void SetPlayerIdx(int nPlayerIdx);
	void SetState(STATE state);
	void SetStateCounter(int nStateCounter);
	void SetMaxBullet(int nMaxBullet);
	void SetSplash(bool bSplash);
	void SetAllBlockDestroy(bool bAllBlockDestroy);
	void SetHelmet(bool bHelmet);

	CPlayer *GetPlayer(void);
	int GetPlayerIdx(void);
	STATE GetState(void);
	int GetMaxBullet(void);
	bool GetSplash(void);
	bool GetAllBlockDestroy(void);
	bool GetHelmet(void);

	//�ÓI�����o�֐�
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPlayerIdx, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, int nPriority = 3);

private:	//�����������A�N�Z�X�\
	void ClearVariable(void);

	void InputMove_Game(void);
	void InputMove_Tutorial(void);
	void InputMove(void);
	void InputAction(CInputKeyboard *pKeyboard, CXInput *pXInput);
	bool InputMove_Keyboard(D3DXVECTOR3 *pMove, float *pDiffAngle, D3DXVECTOR3 rot, float fAccel, CInputKeyboard *pKeyboard);
	bool InputMove_Controller(D3DXVECTOR3 *pMove, float *pDiffAngle, D3DXVECTOR3 rot, float fAccel, CXInput *pXInput);
	void Move(void);
	void Collision(void);
	void State(void);
	void SetDiffAngle(float fDiffAngle);
	void CreateBullet(void);
	void SetInvincibleEffect(void);
	void SetMoveEffect(void);
	void SwitchItem(CItem *pItem);

	bool CollisionObject3D(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CObject3D *pObject3D);
	bool CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBullet *pBullet);
	bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CEnemy *pEnemy);
	bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBlock *pBlock);
	bool CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CItem *pItem);
	void CollisionGoalCylinder(D3DXVECTOR3 *pPos,CGoalCylinder *pGoalCylinder);
	bool CollisionRiver(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CRiver *pRiver);
	void CollisionIceField(D3DXVECTOR3 pos ,CIceField *pIceField, bool *pLandIce);
	bool CollisionHeadQuarters(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHeadQuarters *pHeadQuarters);
	bool CollisionHinaarare(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHinaarare *pHinaarare);

	//�����o�ϐ�
	CPlayer						*m_pPlayer;					//�v���C���[
	int							m_nPlayerIdx;				//�v���C���[�̔ԍ�
	STATE						m_state;					//�v���C���[�̏��
	int							m_nCntState;				//�v���C���[�̏�ԃJ�E���^�[
	int							m_nMaxBullet;				//�ő�e��
	int							m_nCntAbility;				//�\�̓J�E���^�[
	bool						m_bAllBlockDestroy;			//�S�Ẵu���b�N�������邩�ǂ���
	bool						m_bSplash;					//����Ă��邩�ǂ���
	int							m_nCntSplash;				//����J�E���^�[
	MOTION						m_motion;                   //���[�V�������
	PLAYER_ABILITY				m_ability;					//�v���C���[�\��
	int							m_nCntBullet;               //�e�̃J�E���^�[
	bool						m_bHelmet;					//�w�����b�g�����悤���Ă��邩�ǂ���
	int							m_nCntHelmet;				//�w�����b�g�J�E���^�[
	CBulletPlayer               *m_pBulletPlayer[PLAYER_MAX_BULLET];
};
#endif
