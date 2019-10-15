//*****************************************************************************
//
//     �򖗂̏���[splash.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SPLASH_H_
#define _SPLASH_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
//    �򖗃N���X�̒�`
//*****************************************************************************
class CSplash : public CScene2D
{
public:    // �N�ł��A�N�Z�X�\
	//------------------------
	//  ���
	//------------------------
	typedef enum
	{
		STATE_APPEAR = 0,  // �o��
		STATE_NORMAL,      // �ʏ�
		STATE_VANISH,      // ������
		STATE_MAX
	}STATE;

	CSplash(int nPriority = 3, OBJTYPE objType = OBJTYPE_SPLASH);
	~CSplash();

	static CSplash *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nTexIdx, D3DXVECTOR3 StartMove, float fPosYDown, float fRollSpeed, float fLengthMax, float fLengthTransition, int nLife, int nPriority = 3, float fRot = 0.0f, float fStartTexU = 0.0f, float fStartTexV = 0.0f, float fAddTexU = 1.0f, float fAddTexV = 1.0f);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(const STATE state);
	void SetTexIdx(const int nTexIdx);
	void SetStartMove(const D3DXVECTOR3 StartMove);
	void SetPosYDown(const float fPosYDown);
	void SetRollSpeed(const float fRollSpeed);
	void SetLengthMax(const float fLengthMax);
	void SetLengthTransition(const float fLengthTransition);
	void SetLife(const int nLife);
	void SetCounter(const int nCounter);

	STATE GetState(void);
	int GetTexIdx(void);
	D3DXVECTOR3 GetMove(void);
	float GetPosYDown(void);
	float GetRollSpeed(void);
	float GetLengthMax(void);
	float GetLengthTransition(void);
	int GetLife(void);
	int GetCounter(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void AppearUpdate(void);
	void NormalUpdate(void);
	void VanishUpdate(void);

	STATE m_State;              // ���
	int m_nTexIdx;              // �g�p����e�N�X�`���̔ԍ�
	D3DXVECTOR3 m_StartMove;    // �򖗃|���S�����o�������Ƃ��̈ړ���
	float m_fPosYDown;          // �򖗃|���S����������Ƃ��̈ړ���
	float m_fRollSpeed;         // �򖗃|���S���̉�]�X�s�[�h
	float m_fLengthMax;         // �򖗃|���S���̍ő�̑傫��
	float m_fLengthTransition;  // �򖗃|���S���̑傫���ω���
	int   m_nLife;              // ����
	int   m_nCounter;           // �򖗂̑傫���ω����Ǘ�����J�E���^�[

};

#endif