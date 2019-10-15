//*****************************************************************************
//
//     �����̏���[number.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
//    �����|���S���N���X�̒�`
//*****************************************************************************
class CNumberPolygon : public CScene2D
{
public:     // �N�ł��A�N�Z�X�\
	typedef enum
	{
		STATE_NONE = 0,  // �ʏ���
		STATE_FLASH,     // �_�ŕ\����������
		STATE_MAX
	}STATE;

	CNumberPolygon(int nPriority = 3, OBJTYPE objType = OBJTYPE_SCENE2D);
	~CNumberPolygon();

	static CNumberPolygon *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, STATE State = STATE_NONE,
		int nNumber = 0, int nFlashTiming = 3, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, STATE State = STATE_NONE,
		int nNumber = 0, int nFlashTiming = 3);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetNumber(const int nNumber);
	void SetState(const STATE State);

	int GetNumber(void);
	STATE GetState(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void Flash(void);

	STATE m_State;         // ���
	int   m_nNumber;       // �\�����鐔��
	int   m_nFlashTiming;  // �_�ł�����^�C�~���O
	int   m_nCounter;      // �`����Ǘ�����J�E���^�[
};

//*****************************************************************************
//    �����N���X�̒�`
//*****************************************************************************
class CNumber : CScene
{
public:     // �N�ł��A�N�Z�X�\
	CNumber(int nPriority = 3, OBJTYPE objType = OBJTYPE_NUMBER);
	~CNumber();

	static CNumber *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight,
		D3DXVECTOR3 fInterval, LPDIRECT3DTEXTURE9 pNumberTex, int nNumber = 0,
		CNumberPolygon::STATE State = CNumberPolygon::STATE_NONE, int nFlashTiming = 3,
		float fRot = 0.0f, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetNumber(const int nNumber);
	void SetNumberVtxBuffCol();

	void SetNumberValue(const int nNumber);
	void SetNumberTexture(const LPDIRECT3DTEXTURE9 pNumberTex);
	void SetNumberPos(const D3DXVECTOR3 NumberPos);
	void SetNumberCol(const D3DXCOLOR NumberCol);
	void SetNumberWidth(const float fNumberWidth);
	void SetNumberHeight(const float fNumberHeight);
	void SetNumberInterval(const D3DXVECTOR3 NumberInterval);
	void SetNumberRot(const float fNumberRot);
	void SetNumberState(const CNumberPolygon::STATE state);
	void SetNumberFlashTiming(const int nNumberFlashTiming);

	int GetNumber(void);
	int GetNumberDigit(void);
	CNumberPolygon **GetNumberPolygon(void);
	LPDIRECT3DTEXTURE9 GetNumberTexture(void);
	D3DXVECTOR3 GetNumberPos(void);
	D3DXCOLOR GetNumberCol(void);
	float GetNumberWidth(void);
	float GetNumberHeight(void);
	D3DXVECTOR3 GetNumberInterval(void);
	float GetNumberRot(void);
	CNumberPolygon::STATE GetNumberState(void);
	int GetNumberFlashTiming(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CalcNumberDigit(void);
	void CreateNumberPolygon(void);
	void ReleaseNumberPolygon(int nDigit);

	int                   m_nNumber;             // ���ݕ\�����Ă��鐔��
	int                   m_nNumberDigit;        // �\�����Ă��鐔���̌���
	CNumberPolygon        **m_pNumberPolygon;    // �����|���S���N���X�ւ̃|�C���^
	LPDIRECT3DTEXTURE9    m_pNumberTexture;      // �����|���S���Ɏg�p���Ă���e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3           m_NumberPos;           // �����|���S���̈ʒu
	D3DXCOLOR             m_NumberCol;           // �����|���S���̐F
	float                 m_fNumberWidth;        // �����|���S���̕�
	float                 m_fNumberHeight;       // �����|���S���̍���
	D3DXVECTOR3           m_NumberInterval;      // �����|���S�����m�̊Ԋu
	float                 m_fNumberRot;          // �����|���S���̌���
	CNumberPolygon::STATE m_NumberState;         // �����|���S���̏��
	int                   m_nNumberFlashTiming;  // �����|���S����_�ł�����̂Ɏg��
};

#endif