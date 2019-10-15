//*****************************************************************************
//
//     �����O�G�t�F�N�g�̏���[ringEffect.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RINGEFFECT_H_
#define _RINGEFFECT_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "meshRing.h"

//*****************************************************************************
//    �����O�G�t�F�N�g�f�[�^�N���X�̒�`
//*****************************************************************************
class CRingData
{
public:    // �N�ł��A�N�Z�X�\
	CRingData();
	~CRingData();

	void SetMaxRot(const D3DXVECTOR3 MaxRot) { m_MaxRot = MaxRot; }
	void SetMinRot(const D3DXVECTOR3 MinRot) { m_MinRot = MinRot; }
	void SetMaxMove(const D3DXVECTOR3 MaxMove) { m_MaxMove = MaxMove; }
	void SetMinMove(const D3DXVECTOR3 MinMove) { m_MinMove = MinMove; }
	void SetChangeMove(const D3DXVECTOR3 ChangeMove) { m_ChangeMove = ChangeMove; }
	void SetInitCol(const D3DXCOLOR InitCol) { m_InitCol = InitCol; }
	void SetChangeCol(const D3DXCOLOR ChangeCol) { m_ChangeCol = ChangeCol; }
	void SetHeight(const float fHeight) { m_fHeight = fHeight; }
	void SetRadius(const float fRadius) { m_fRadius = fRadius; }
	void SetDiffusion(const float fDiffusion) { m_fDiffusion = fDiffusion; }
	void SetChangeHeight(const float fChangeHeight) { m_fChangeHeight = fChangeHeight; }
	void SetChangeRadius(const float fChangeRadius) { m_fChangeRadius = fChangeRadius; }
	void SetChangeDiffusion(const float fChangeDiffusion) { m_fChangeDiffusion = fChangeDiffusion; }
	void SetMaxLife(const int nMaxLife) { m_nMaxLife = nMaxLife; }
	void SetMinLife(const int nMinLife) { m_nMinLife = nMinLife; }
	void SetXBlock(const int nXBlock) { m_nXBlock = nXBlock; }
	void SetYBlock(const int nYBlock) { m_nYBlock = nYBlock; }
	void SetRotPattern(const int nRotPattern) { m_nRotPattern = nRotPattern; }
	void SetRotSpeed(const D3DXVECTOR3 RotSpeed) { m_RotSpeed = RotSpeed; }
	void SetCulling(const bool bCulling) { m_bCulling = bCulling; }
	void SetDrawAddtive(const bool bDrawAddtive) { m_bDrawAddtive = bDrawAddtive; }

	D3DXVECTOR3 GetMaxRot(void) { return m_MaxRot; }
	D3DXVECTOR3 GetMinRot(void) { return m_MinRot; }
	D3DXVECTOR3 GetMaxMove(void) { return m_MaxMove; }
	D3DXVECTOR3 GetMinMove(void) { return m_MinMove; }
	D3DXVECTOR3 GetChangeMove(void) { return m_ChangeMove; }
	D3DXCOLOR GetInitCol(void) { return m_InitCol; }
	D3DXCOLOR GetChangeCol(void) { return m_ChangeCol; }
	float GetHeight(void) { return m_fHeight; }
	float GetRadius(void) { return m_fRadius; }
	float GetDiffusion(void) { return m_fDiffusion; }
	float GetChangeHeight(void) { return m_fChangeHeight; }
	float GetChangeRadius(void) { return m_fChangeRadius; }
	float GetChangeDiffusion(void) { return m_fChangeDiffusion; }
	int GetMaxLife(void) { return m_nMaxLife; }
	int GetMinLife(void) { return m_nMinLife; }
	int GetXBlock(void) { return m_nXBlock; }
	int GetYBlock(void) { return m_nYBlock; }
	int GetRotPattern(void) { return m_nRotPattern; }
	D3DXVECTOR3 GetRotSpeed(void) { return m_RotSpeed; }
	bool GetCulling(void) { return m_bCulling; }
	bool GetDrawAddtive(void) { return m_bDrawAddtive; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	D3DXVECTOR3 m_MaxRot;            // �����̍ő�l
	D3DXVECTOR3 m_MinRot;            // �����̍ŏ��l
	D3DXVECTOR3 m_MaxMove;           // �ړ��ʂ̍ő�l
	D3DXVECTOR3 m_MinMove;           // �ړ��ʂ̍ŏ��l
	D3DXVECTOR3 m_ChangeMove;        // �ړ��ʂ̕ω���
	D3DXCOLOR   m_InitCol;           // �������̐F
	D3DXCOLOR   m_ChangeCol;         // �t���[�����̐F�̕ω���
	float       m_fHeight;           // ����
	float       m_fRadius;           // ���a
	float       m_fDiffusion;        // ��
	float       m_fChangeHeight;     // �����𖈃t���[���ǂꂭ�炢�ω������邩
	float       m_fChangeRadius;     // ���a���ǂꂭ�炢���t���[���ω������邩
	float       m_fChangeDiffusion;  // �����O�̕����ǂꂭ�炢���t���[���ω������邩
	int         m_nXBlock;           // ���̕�����
	int         m_nYBlock;           // �c�̕�����
	int         m_nMaxLife;          // �����̍ő�l
	int         m_nMinLife;          // �����̍ŏ��l
	int         m_nRotPattern;       // ��]�̎��[ 0:���v��� 1:�����v��� 2:�����_�� ]
	D3DXVECTOR3 m_RotSpeed;          // ��]����X�s�[�h
	bool        m_bCulling;          // �J�����O���邩�ǂ���[ 0:�Ȃ� 1:���� ]
	bool        m_bDrawAddtive;      // ���Z�������邩���Ȃ���[ 0:�Ȃ� 1:���� ]
};

//*****************************************************************************
//    �����O�G�t�F�N�g�N���X�̒�`
//*****************************************************************************
class CRingEffect : public CMeshRing
{
public:    // �N�ł��A�N�Z�X�\
	CRingEffect(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHRING);
	~CRingEffect();
	static CRingEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, float fDiffusion, int nXBlock, int nYBlock,
		int nLife, D3DXVECTOR3 RotSpeed, int nRotPattern, D3DXVECTOR3 Move, D3DXVECTOR3 ChangeMove, D3DXCOLOR ChangeCol, float fChangeHeight,
		float fChangeRadius, float fChangeDiffusion, bool bCulling, bool bDrawAddtive, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, float fDiffusion, int nXBlock, int nYBlock,
		int nLife, D3DXVECTOR3 RotSpeed, int nRotPattern, D3DXVECTOR3 Move, D3DXVECTOR3 ChangeMove, D3DXCOLOR ChangeCol, float fChangeHeight,
		float fChangeRadius, float fChangeDiffusion, bool bCulling, bool bDrawAddtive);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ChangeVertex(void);

	void SetLife(const int nLife);
	void SetRotSpeed(const D3DXVECTOR3 RotSpeed);
	void SetRotPattern(const int nRotPattern);
	void SetMove(const D3DXVECTOR3 Move);
	void SetChangeMove(const D3DXVECTOR3 ChangeMove);
	void SetChangeCol(const D3DXCOLOR ChangeCol);
	void SetChangeHeight(const float fChangeHeight);
	void SetChangeRadius(const float fChangeRadisu);
	void SetChangeDiffusion(const float fChangeDiffusion);
	void SetCulling(const bool bCulling);
	void SetDrawAddtive(const bool bDrawAddtive);

	int GetLife(void);
	D3DXVECTOR3 GetRotSpeed(void);
	int GetRotPattern(void);
	D3DXVECTOR3 GetMove(void);
	D3DXVECTOR3 GetChangeMove(void);
	D3DXCOLOR GetChangeCol(void);
	float GetChangeHeight(void);
	float GetChangeRadius(void);
	float GetChangeDiffusion(void);
	bool GetCulling(void);
	bool GetDrawAddtive(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int         m_nLife;             // ����
	D3DXVECTOR3 m_RotSpeed;          // ��]��
	int         m_nRotPattern;       // ��]�̃p�^�[��
	D3DXVECTOR3 m_Move;              // �ړ���
	D3DXVECTOR3 m_ChangeMove;        // �ړ��ʂ𖈃t���[���ǂꂭ�炢�ω������邩
	D3DXCOLOR   m_ChangeCol;         // �F�𖈃t���[���ǂꂭ�炢�ω������邩
	float       m_fChangeHeight;     // �����𖈃t���[���ǂꂭ�炢�ω������邩
	float       m_fChangeRadius;     // ���a���ǂꂭ�炢���t���[���ω������邩
	float       m_fChangeDiffusion;  // �����O�̕����ǂꂭ�炢���t���[���ω������邩
	bool        m_bCulling;          // �J�����O���邩�ǂ���
	bool        m_bDrawAddtive;      // ���Z�����ŕ`�悷�邩���Ȃ���
};

#endif