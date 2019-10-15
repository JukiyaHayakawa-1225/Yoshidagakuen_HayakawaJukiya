//=============================================================================
//
// �J�������� [camera.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// �J�����N���X�̒�`
//*****************************************************************************
class CCamera
{
public:	//�N������A�N�Z�X�\
	//---------------------
	// ���
	//---------------------
	typedef enum
	{
		TYPE_NONE = -1,
		TYPE_CHARASELECT,
		TYPE_GAME,
		TYPE_MAX
	}TYPE;

	//�����o�֐�
	CCamera();
	~CCamera();
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Reset(void);
	void SetCamera(void);

	TYPE GetType(void);
	D3DXVECTOR3 GetPosV(void);
	D3DXVECTOR3 GetPosR(void);
	D3DXVECTOR3 GetRot(void);
	float GetLength(void);
	bool GetMouseMove(void);
	bool GetChengeLength(void);
	float GetAngleOfView(void);

	void SetType(TYPE type);
	void SetPosV(D3DXVECTOR3 posV);
	void SetPosR(D3DXVECTOR3 posR);
	void SetRot(D3DXVECTOR3 Rot);
	void SetLength(float fLength);
	void SetMouseMove(bool bMouseMove);
	void SetChengeLength(bool bChengeLength);
	void SetAngleOfView(float fAngleOfView);

	// �ÓI�����o�֐�
	static CCamera *Create(void);

private://�����������A�N�Z�X�\
	TYPE        m_Type;             // ���
	D3DXVECTOR3 m_posV;				// ���_
	D3DXVECTOR3 m_posR;				// �����_
	D3DXVECTOR3 m_vecU;				// ������x�N�g��
	D3DXMATRIX	m_mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX	m_mtxView;			// �r���[�}�g���b�N�X
	D3DXVECTOR3 m_rot;				// ����
	D3DXVECTOR3 m_rotDest;			// �ړI�̌���
	D3DXVECTOR3	m_rotDiff;          // ���݂̌����ƖړI�̌����̍���
	float		m_fLength;			// ����
	bool        m_bMouseMove;       // �}�E�X�œ������邩�ǂ���
	bool        m_bChengeLength;    // ������ς����邩�ǂ���
	float       m_fAngleOfView;     // �J�����̉�p
};

//*****************************************************************************
// �L�����Z���N�g�p�J�����N���X�̒�`
//*****************************************************************************
class CCharaSelectCamera : public CCamera
{
public:	//�N������A�N�Z�X�\
	//�����o�֐�
	CCharaSelectCamera();
	~CCharaSelectCamera();
	HRESULT Init(D3DXVECTOR3 posV, D3DXVECTOR3 posR);
	void Uninit(void);
	void Update(void);

	// �ÓI�����o�֐�
	static CCharaSelectCamera *Create(D3DXVECTOR3 posV, D3DXVECTOR3 posR);

private://�����������A�N�Z�X�\

};

//*****************************************************************************
// �Q�[���p�J�����N���X�̒�`
//*****************************************************************************
class CGameCamera : public CCamera
{
public:	//�N������A�N�Z�X�\
	//---------------------
	// ���
	//---------------------
	typedef enum
	{
		STATE_NONE = -1,
		STATE_NORMAL,
		STATE_SHAKE,
		STATE_MAX
	}STATE;

	//�����o�֐�
	CGameCamera();
	~CGameCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void StartShake(int nTimer, float fShake, float fShakeCut);

	void SetState(const STATE state);
	STATE GetState(void);

	// �ÓI�����o�֐�
	static CGameCamera *Create(void);

private://�����������A�N�Z�X�\
	void Normal(void);
	void Shake(void);

	D3DXVECTOR3 m_PosVDef;    // �f�t�H���g�̎��_
	D3DXVECTOR3 m_PosRDef;    // �f�t�H���g�̒����_
	STATE m_State;            // ���
	int m_nShakeTimer;        // �h��鎞�Ԃ��v������^�C�}�[
	float m_fShakeValue;      // �ǂꂭ�炢�h��邩
	float m_fShakeCutValue;   // �h����ǂꂭ�炢���炵�Ă�����
};

#endif
