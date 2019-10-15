//*****************************************************************************
//
//     �򖗂̏���[splash.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "splash.h"
#include "scene2D.h"
#include "manager.h"
#include "basemode.h"
#include "textureManager.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define SPLASH_ALPHA_CUT         (0.02f)  // �򖗂̓����x��1�t���[���łǂꂭ�炢�����邩
#define SPLASH_APPEAR_SCALE_UP   (5.0f)   // �򖗂��o�������Ƃ��ǂꂭ�炢�傫���Ȃ邩
#define SPLASH_CHANGE_TIMING     (60)     // �򖗂̑傫���ω����ǂ̃^�C�~���O�ŕς��邩

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CSplash::CSplash(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_State = STATE_APPEAR;      // ���
	m_nCounter = 0;              // �򖗂̑傫���ω����Ǘ�����J�E���^�[
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CSplash::~CSplash()
{

}

//=============================================================================
//    ��������
//=============================================================================
CSplash *CSplash::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nTexIdx, D3DXVECTOR3 StartMove, float fPosYDown, float fRollSpeed, float fLengthMax, float fLengthTransition, int nLife, int nPriority, float fRot, float fStartTexU, float fStartTexV, float fAddTexU, float fAddTexV)
{
	CSplash *pSplash = NULL;  // �򖗃N���X�^�̃|�C���^
	if (pSplash == NULL)
	{// ����������ɂȂ��Ă���
		pSplash = new CSplash(nPriority);
		if (pSplash != NULL)
		{// �C���X�^���X�𐶐��ł���
			// �e��l�̐ݒ�
			pSplash->SetPos(pos);
			pSplash->SetCol(col);
			pSplash->SetWidth(fWidth);
			pSplash->SetHeight(fHeight);
			pSplash->SetTexIdx(nTexIdx);
			pSplash->SetStartMove(StartMove);
			pSplash->SetPosYDown(fPosYDown);
			pSplash->SetRollSpeed(fRollSpeed);
			pSplash->SetLengthMax(fLengthMax);
			pSplash->SetLengthTransition(fLengthTransition);
			pSplash->SetLife(nLife);
			pSplash->SetRot(fRot);
			pSplash->SetStartTexU(fStartTexU);
			pSplash->SetStartTexV(fStartTexV);
			pSplash->SetAddTexU(fAddTexU);
			pSplash->SetAddTexV(fAddTexV);

			if (FAILED(pSplash->Init()))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// �C���X�^���X�𐶐��ł��Ȃ�����
		return NULL;
	}

	return pSplash;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CSplash::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CScene2D::Init()))
	{
		return E_FAIL;
	}

	// �e�N�X�`����ݒ肷��
	CBasemode *pBase = CManager::GetBaseMode();
	if (pBase != NULL)
	{
		CTextureManager *pTexManager = pBase->GetTextureManager();
		if (pTexManager != NULL)
		{
			BindTexture(pTexManager->GetTexture(m_nTexIdx));
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CSplash::Uninit(void)
{
	// ���ʂ̏I������
	CScene2D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CSplash::Update(void)
{
	// ��]������
	float fRot = GetRot();
	fRot += m_fRollSpeed;
	if (fRot > D3DX_PI)
	{
		fRot -= D3DX_PI * 2.0f;
	}
	SetRot(fRot);

	switch (m_State)
	{// ��Ԃɂ���ď����킯
	case STATE_APPEAR:   // �o�����
		AppearUpdate();
		break;
	case STATE_NORMAL:   // �ʏ���
		NormalUpdate();
		break;
	case STATE_VANISH:   // ��������
		VanishUpdate();
		break;
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CSplash::Draw(void)
{
	// ���ʂ̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
//    �o����Ԃ̍X�V����
//=============================================================================
void CSplash::AppearUpdate(void)
{
	float fLength = GetLength();
	D3DXVECTOR3 pos = GetPos();

	// �g�傳����
	fLength += SPLASH_APPEAR_SCALE_UP;
	if (fLength >= m_fLengthMax)
	{// �傫�����ő�l�𒴂���
		fLength = m_fLengthMax;  // �傫����߂�
		m_State = STATE_NORMAL;  // �ʏ��Ԃɂ���
	}

	// �ʒu�����񂾂񂸂炷
	pos += m_StartMove;
	SetPos(pos);

	// �傫���̐ݒ�
	SetLength(fLength);
	SetVtxBuffPos();
}

//=============================================================================
//    �ʏ��Ԃ̍X�V����
//=============================================================================
void CSplash::NormalUpdate(void)
{
	float fLength = GetLength();
	D3DXVECTOR3 pos = GetPos();

	// �J�E���^�[�𑝂₷
	m_nCounter++;

	// �傫����ω�������
	fLength += m_fLengthTransition;
	if (m_nCounter % SPLASH_CHANGE_TIMING == 0)
	{// �傫���̕ω���������x������
		m_fLengthTransition *= -1;
	}

	// �ʒu�����񂾂񂸂炷
	pos.y += m_fPosYDown;
	SetPos(pos);

	// ���_�o�b�t�@�֒l��ݒ�
	SetLength(fLength);
	SetVtxBuffPos();

	// ���������炷
	m_nLife--;
	if (m_nLife <= 0)
	{
		m_nLife = 0;
		m_State = STATE_VANISH;
	}
}

//=============================================================================
//    �������Ԃ̍X�V����
//=============================================================================
void CSplash::VanishUpdate(void)
{
	// �ʏ펞�̏���
	NormalUpdate();

	// �����x�����炷
	D3DXCOLOR col = GetCol();
	col.a -= SPLASH_ALPHA_CUT;

	// �F�̐ݒ�
	SetCol(col);
	SetVtxBuffCol();

	if (col.a <= 0.0f)
	{// �����x���Ȃ��Ȃ���
		Uninit();
	}
}

//=============================================================================
//    ��Ԑݒ菈��
//=============================================================================
void CSplash::SetState(const STATE state)
{
	m_State = state;
}

//=============================================================================
//    �g�p����e�N�X�`���̔ԍ��ݒ菈��
//=============================================================================
void CSplash::SetTexIdx(const int nTexIdx)
{
	m_nTexIdx = nTexIdx;
}

//=============================================================================
//    �򖗃|���S�����o�������Ƃ��̈ړ��ʐݒ菈��
//=============================================================================
void CSplash::SetStartMove(const D3DXVECTOR3 StartMove)
{
	m_StartMove = StartMove;
}

//=============================================================================
//    �򖗃|���S����������Ƃ��̈ړ��ʐݒ菈��
//=============================================================================
void CSplash::SetPosYDown(const float fPosYDown)
{
	m_fPosYDown = fPosYDown;
}

//=============================================================================
//    �򖗃|���S���̉�]�X�s�[�h�ݒ菈��
//=============================================================================
void CSplash::SetRollSpeed(const float fRollSpeed)
{
	m_fRollSpeed = fRollSpeed;
}

//=============================================================================
//    �򖗃|���S���̍ő�̑傫���ݒ菈��
//=============================================================================
void CSplash::SetLengthMax(const float fLengthMax)
{
	m_fLengthMax = fLengthMax;
}

//=============================================================================
//    �򖗃|���S���̑傫���ω��ʐݒ菈��
//=============================================================================
void CSplash::SetLengthTransition(const float fLengthTransition)
{
	m_fLengthTransition = fLengthTransition;
}

//=============================================================================
//    �����ݒ菈��
//=============================================================================
void CSplash::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    �򖗂̑傫���ω����Ǘ�����J�E���^�[�ݒ菈��
//=============================================================================
void CSplash::SetCounter(const int nCounter)
{
	m_nCounter = nCounter;
}

//=============================================================================
//    ��Ԏ擾����
//=============================================================================
CSplash::STATE CSplash::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �g�p����e�N�X�`���̔ԍ��擾����
//=============================================================================
int CSplash::GetTexIdx(void)
{
	return m_nTexIdx;
}

//=============================================================================
//    �򖗃|���S���̍ő�̑傫���擾����
//=============================================================================
float CSplash::GetLengthMax(void)
{
	return m_fLengthMax;
}

//=============================================================================
//    �򖗃|���S���̑傫���ω��ʎ擾����
//=============================================================================
float CSplash::GetLengthTransition(void)
{
	return m_fLengthTransition;
}

//=============================================================================
//    �����擾����
//=============================================================================
int CSplash::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    �򖗂̑傫���ω����Ǘ�����J�E���^�[�擾����
//=============================================================================
int CSplash::GetCounter(void)
{
	return m_nCounter;
}

//=============================================================================
//    �򖗃|���S�����o�������Ƃ��̈ړ��ʎ擾����
//=============================================================================
D3DXVECTOR3 CSplash::GetMove(void)
{
	return m_StartMove;
}

//=============================================================================
//    �򖗃|���S����������Ƃ��̈ړ��ʎ擾����
//=============================================================================
float CSplash::GetPosYDown(void)
{
	return m_fPosYDown;
}

//=============================================================================
//    �򖗃|���S���̉�]�X�s�[�h�擾����
//=============================================================================
float CSplash::GetRollSpeed(void)
{
	return m_fRollSpeed;
}