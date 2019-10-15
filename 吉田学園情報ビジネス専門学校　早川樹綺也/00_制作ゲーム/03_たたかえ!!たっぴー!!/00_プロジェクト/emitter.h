//*****************************************************************************
//
//     �G�~�b�^�̏���[emitter.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _EMITTER_H_
#define _EMITTER_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//     �}�N���錾
//*****************************************************************************
#define EFFECT_PRIORITY    (5)                // �G�t�F�N�g�̏����D�揇��
#define EMITTER_MOVEMENT   (6.0f)             // �G�~�b�^�̈ړ���
#define EMITTER_ROTATION   (D3DX_PI / 60)     // �G�~�b�^�̉�]��

//*****************************************************************************
//     �O���錾
//*****************************************************************************
class CParData;
class CRingData;

//*****************************************************************************
//     �G�~�b�^�f�[�^�N���X�̒�`
//*****************************************************************************
class CEmitterData
{
public:    // �N�ł��A�N�Z�X�\
	CEmitterData();
	~CEmitterData();

	void SetType(const int nType) { m_nType = nType; }
	void SetTexIdx(const int nTexIdx) { m_nTexIdx = nTexIdx; }
	void SetEffectIdx(const int nEffectIdx) { m_nEffectIdx = nEffectIdx; }
	void SetRangeMax(const D3DXVECTOR3 RangeMax) { m_RangeMax = RangeMax; }
	void SetRangeMin(const D3DXVECTOR3 RangeMin) { m_RangeMin = RangeMin; }
	void SetLife(const int nLife) { m_nLife = nLife; }
	void SetLap(const int nLap) { m_nLap = nLap; }
	void SetAppear(const int nAppear) { m_nAppear = nAppear; }
	void SetLoop(const bool bLoop) { m_bLoop = bLoop; }

	int GetType(void) { return m_nType; }
	int GetTexIdx(void) { return m_nTexIdx; }
	int GetEffectIdx(void) { return m_nEffectIdx; }
	D3DXVECTOR3 GetRangeMax(void) { return m_RangeMax; }
	D3DXVECTOR3 GetRangeMin(void) { return m_RangeMin; }
	int GetLife(void) { return m_nLife; }
	int GetLap(void) { return m_nLap; }
	int GetAppear(void) { return m_nAppear; }
	bool GetLoop(void) { return m_bLoop; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int         m_nType;       // �g�p����G�t�F�N�g�̎��
	int         m_nTexIdx;     // �g�p����e�N�X�`���̔ԍ�
	int         m_nEffectIdx;  // �g�p����G�t�F�N�g�f�[�^�̔ԍ�
	D3DXVECTOR3 m_RangeMax;    // �G�t�F�N�g�𐶐�����͈͂̍ő�l
	D3DXVECTOR3 m_RangeMin;    // �G�t�F�N�g�𐶐�����͈͂̍ŏ��l
	int         m_nLife;       // ����
	int         m_nLap;        // 1��̕��o�ŏo���G�t�F�N�g�̌�
	int         m_nAppear;     // �G�t�F�N�g���o���Ԋu
	bool        m_bLoop;       // ���[�v���邩���Ȃ���
};

//*****************************************************************************
//     �G�~�b�^�N���X�̒�`
//*****************************************************************************
class CEmitter : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CEmitter(int nPriority = 3, OBJTYPE objType = OBJTYPE_EMITTER);
	~CEmitter();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetType(const int nType);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetTexIdx(const int nTexIdx);
	void SetEffectIdx(const int nEffectIdx);
	void SetRangeMax(const D3DXVECTOR3 RangeMax);
	void SetRangeMin(const D3DXVECTOR3 RangeMin);
	void SetLife(const int nLife);
	void SetLap(const int nLap);
	void SetAppear(const int nAppear);
	void SetLoop(const bool bLoop);

	int GetType(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXMATRIX GetMtxWorld(void);
	int GetTexIdx(void);
	int GetEffectIdx(void);
	D3DXVECTOR3 GetRangeMax(void);
	D3DXVECTOR3 GetRangeMin(void);
	int GetLife(void);
	int GetLap(void);
	int GetAppear(void);
	bool GetLoop(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	virtual void SetEffect(void) = 0;

private:   // ���̃N���X�������A�N�Z�X�\
	int                m_nType;      // ��ޔԍ�
	LPDIRECT3DTEXTURE9 m_pTexture;   // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3        m_Pos;        // ���W
	D3DXVECTOR3        m_Rot;        // ����
	D3DXMATRIX         m_MtxWorld;   // ���[���h�}�g���b�N�X
	int                m_nTexIdx;    // �g�p����e�N�X�`���ԍ�
	int                m_nEffectIdx; // �g�p����G�t�F�N�g�̔ԍ�
	D3DXVECTOR3        m_RangeMax;   // �G�t�F�N�g�𐶐�����͈͂̍ő�l
	D3DXVECTOR3        m_RangeMin;   // �G�t�F�N�g�𐶐�����͈͂̍ŏ��l
	int                m_nLife;      // �G�~�b�^�̎���
	int                m_nLap;       // 1��̕��o�^�C�~���O�ɏo���G�t�F�N�g�̌�
	int                m_nAppear;    // ���o����Ԋu
	int                m_nCounter;   // ���o�^�C�~���O���Ǘ�����J�E���^�[
	bool               m_bLoop;      // ���[�v���邩���Ȃ���
};

//*****************************************************************************
//     �p�[�e�B�N���G�~�b�^�N���X�̒�`
//*****************************************************************************
class CParEmitter : public CEmitter
{
public:    // �N�ł��A�N�Z�X�\
	CParEmitter(int nPriority = 3, OBJTYPE objType = OBJTYPE_PAREMITTER);
	~CParEmitter();

	static CParEmitter *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTexIdx, int nEffectIdx, D3DXVECTOR3 RangeMax, D3DXVECTOR3 RangeMin, int nLife, int nLap, int nAppear, bool bLoop, CParData *pParData, LPDIRECT3DTEXTURE9 pTexture, int nType, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetParData(CParData *pParData);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void SetEffect(void);

private:   // ���̃N���X�������A�N�Z�X�\
	CParData  *m_pParData;  // �p�[�e�B�N���f�[�^�ւ̃|�C���^
};

//*****************************************************************************
//     �����O�G�t�F�N�g�G�~�b�^�N���X�̒�`
//*****************************************************************************
class CRingEmitter : public CEmitter
{
public:    // �N�ł��A�N�Z�X�\
	CRingEmitter(int nPriority = 3, OBJTYPE objType = OBJTYPE_RINGEMITTER);
	~CRingEmitter();

	static CRingEmitter *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTexIdx, int nEffectIdx, D3DXVECTOR3 RangeMax, D3DXVECTOR3 RangeMin, int nLife, int nLap, int nAppear, bool bLoop, CRingData *pRingData, LPDIRECT3DTEXTURE9 pTexture, int nType, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetRingData(CRingData *pRingData);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void SetEffect(void);

private:   // ���̃N���X�������A�N�Z�X�\
	CRingData  *m_pRingData;  // �����O�G�t�F�N�g�f�[�^�ւ̃|�C���^
};

#endif