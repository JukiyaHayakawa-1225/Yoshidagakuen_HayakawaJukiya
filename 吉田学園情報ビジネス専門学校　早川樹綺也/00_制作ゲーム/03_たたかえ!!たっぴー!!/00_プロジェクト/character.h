//=============================================================================
//
// �L�����N�^�[�̏��� [character.h]
// Author : Jukiya Hayakawa & Hodaka Niwa
//
//=============================================================================
#ifndef _CHARCTER_H_
#define _CHARCTER_H_

#include "main.h"
#include "object3D.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;
class CMotionManager;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCharacter : public CObject3D
{
public:	//�N������A�N�Z�X�\
	typedef enum
	{
		ROT_UP = 0,
		ROT_DOWN,
		ROT_RIGHT,
		ROT_LEFT,
		ROT_MAX,
	}NOW_ROT_INFO;

	//�����o�֐�
	CCharacter(int nPriority = 3, OBJTYPE objtype = OBJTYPE_CHARACTER);
	~CCharacter();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetNumPart(const int nNumParts);
	void SetModel(CModel **apModel);
	void SetMotionManager(CMotionManager *pMotionManager);
	void SetAccel(const float fAccel);
	void SetInertia(const float fInertia);
	void SetRivisionRot(const float fRivisionRot);
	void SetPosOld(D3DXVECTOR3 posOld);
	void SetMove(D3DXVECTOR3 move);
	void SetShoot(bool bHit);
	void SetNowRotInfo(NOW_ROT_INFO nowRotInfo);
	void SetCntBullet(int nCntBullet);

	int GetNumPart(void);
	CModel **GetModel(void);
	CMotionManager *GetMotionManager(void);
	float GetAccel(void);
	float GetInertia(void);
	float GetRivisionRot(void);
	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetMove(void);
	bool GetShoot(void);
	NOW_ROT_INFO GetNowRotInfo(void);
	int GetCntBullet(void);

private:	//�����������A�N�Z�X�\
	void ClearVariable(void);
	void ReleaseModel(void);
	void ReleaseMotionManager(void);
	void ModelDraw(void);

	// �����o�ϐ�
	int                         m_nNumParts;                        // �K�v�ȃp�[�c��
	CModel						**m_apModel;						// ���f�����ւ̃|�C���^
	CMotionManager              *m_pMotionManager;                  // ���[�V�����Ǌ��N���X�ւ̃|�C���^
	float                       m_fAccel;                           // �ړ��ł���X�s�[�h��
	float                       m_fInertia;                         // ������
	float                       m_fRivisionRot;                     // ������␳����{��
	D3DXVECTOR3					m_posOld;							// �O��̈ʒu
	D3DXVECTOR3					m_move;								// �ړ���
	bool						m_bShoot;							// �����Ă��邩�ǂ���
	NOW_ROT_INFO				m_nowRotInfo;						// ���݂̌������
	int							m_nCntBullet;						// �e�̃J�E���^�[
};
#endif