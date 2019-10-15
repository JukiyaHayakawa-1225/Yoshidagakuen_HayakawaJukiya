//=============================================================================
//
// �I�u�W�F�N�g2D���� [scene2D.h]
// Author : Jukiya Hayakawa & Hodaka Niwa
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScene2D : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	//�����o�֐�
	CScene2D(int nPriority = 3, OBJTYPE objtype = OBJTYPE_SCENE2D);
	~CScene2D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void SetScale(const float fWidth, const float fHeight);
	void SetVtxBuffValue(void);
	void SetVtxBuffPos(void);
	void SetVtxBuffCol(void);
	void SetVtxBuffTex(void);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetRot(const float fRot);
	void SetAngle(const float fAngle);
	void SetLength(const float fLength);
	void SetStartTexU(const float fStartTexU);
	void SetStartTexV(const float fStartTexV);
	void SetAddTexU(const float fAddTexU);
	void SetAddTexV(const float fAddTexV);

	LPDIRECT3DTEXTURE9 GetTexture(void);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	float GetRot(void);
	float GetAngle(void);
	float GetLength(void);
	float GetStartTexU(void);
	float GetStartTexV(void);
	float GetAddTexU(void);
	float GetAddTexV(void);

	// �ÓI�����o�֐�
	static CScene2D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority = 3, float fRot = 0.0f, float fStartTexU = 0.0f, float fStartTexV = 0.0f, float fAddTexU = 1.0f, float fAddTexV = 1.0f);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3DTEXTURE9			m_pTexture = NULL;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3					m_pos;				// �|���S���̈ʒu
	D3DXCOLOR					m_col;				// �|���S���̐F
	float						m_fWidth;			// �|���S���̕�
	float						m_fHeight;			// �|���S���̍���
	float						m_fRot;				// �|���S���̌���
	float                       m_fAngle;           // �x�N�g���̊p�x
	float						m_fLength;			// �x�N�g���̒���
	float                       m_fStartTexU;       // �e�N�X�`��U���W(�n�_)
	float                       m_fStartTexV;       // �e�N�X�`��V���W(�n�_)
	float                       m_fAddTexU;         // �e�N�X�`��U���W�ɉ�����l
	float                       m_fAddTexV;         // �e�N�X�`��V���W�ɉ�����l
};

//*****************************************************************************
//    2D�|���S���_�ŃN���X�̒�`
//*****************************************************************************
class CScene2DFlash : public CScene2D
{
public:     // �N�ł��A�N�Z�X�\
	typedef enum
	{
		STATE_NONE = 0,  // �ʏ���
		STATE_SELECT,    // �I�����ꂽ���
		STATE_PUSH,      // �����ꂽ���
		STATE_MAX,
	}STATE;

	CScene2DFlash(int nPriority = 3, OBJTYPE objType = OBJTYPE_SCENE2D);
	~CScene2DFlash();

	static CScene2DFlash *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, STATE State = STATE_NONE,
		D3DXCOLOR ColMax = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR ColMin = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f),
		float fColChange = 0.01f, int nPushTiming = 4, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, STATE State = STATE_NONE,
		D3DXCOLOR ColMax = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR ColMin = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f),
		float fColChange = 0.01f, int nPushTiming = 4);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(const STATE State);
	void SetColMax(const D3DXCOLOR ColMax);
	void SetColMin(const D3DXCOLOR ColMin);
	void SetColChange(const float fColChange);

	STATE GetState(void);
	D3DXCOLOR GetColMax(void);
	D3DXCOLOR GetColMin(void);
	float GetColChange(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void Select(void);

	STATE     m_State;        // ���
	D3DXCOLOR m_ColMax;       // �F�̍ő�l
	D3DXCOLOR m_ColMin;       // �F�̍ŏ��l
	float     m_fColChange;   // �F�̕ω���
	int       m_nPushTiming;  // �`���؂�ւ���^�C�~���O(�����ꂽ��Ԃ̎�)
	int       m_nCounter;     // �`����Ǘ�����J�E���^�[
};

//*****************************************************************************
//    2D�|���S���v���X�N���X�̒�`
//*****************************************************************************
class CScene2DPress : public CScene2D
{
public:     // �N�ł��A�N�Z�X�\
	typedef enum
	{
		STATE_NONE = 0,  // �ʏ���
		STATE_PUSH,      // �����ꂽ���
		STATE_MAX,
	}STATE;

	CScene2DPress(int nPriority = 3, OBJTYPE objType = OBJTYPE_SCENE2D);
	~CScene2DPress();

	static CScene2DPress *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f,
		int nStopTiming = 80, int nAppearTiming = 100, int nPushTiming = 4, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f,
		int nStopTiming = 80, int nAppearTiming = 100, int nPushTiming = 4);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void FlashStart(void);

	void SetState(const STATE State);
	void SetAppearTiming(const int nAppearTiming);
	void SetStopTiming(const int nStopTiming);
	void SetCounter(const int nCounter);

	STATE GetState(void);
	int GetAppearTiming(void);
	int GetStopTiming(void);
	int GetCounter(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void None(void);

	STATE m_State;          // ���
	int   m_nStopTiming;    // �`����ꎞ��~����^�C�~���O
	int   m_nAppearTiming;  // �`����ĊJ����^�C�~���O
	int   m_nCounter;       // �`����Ǘ�����J�E���^�[
	int   m_nPushTiming;    // �`���؂�ւ���^�C�~���O(�����ꂽ��Ԃ̎�)
};

#endif