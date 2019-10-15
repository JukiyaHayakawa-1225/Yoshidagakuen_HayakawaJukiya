//*****************************************************************************
//
//     �G�~�b�^�̏���[emitter.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "emitter.h"
#include "manager.h"
#include "camera.h"
#include "particle.h"
#include "ringEffect.h"
#include "effectManager.h"
#include "input.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define EMITTER_EFFECT_PRIORITY   (5)    // �G�t�F�N�g�̏����D�揇��

//*****************************************************************************
//    CEmitterData�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CEmitterData::CEmitterData()
{
	m_nType = 0;                                // �g�p����G�t�F�N�g�̎��
	m_nTexIdx = 0;                              // �g�p����e�N�X�`���̔ԍ�
	m_nEffectIdx = 0;                           // �g�p����G�t�F�N�g�̔ԍ�
	m_RangeMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �G�t�F�N�g�𐶐�����͈͂̍ő�l
	m_RangeMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �G�t�F�N�g�𐶐�����͈͂̍ŏ��l
	m_nLife = 0;                                // ����
	m_nLap = 0;                                 // 1��̕��o�ŏo���G�t�F�N�g�̌�
	m_nAppear = 0;                              // �G�t�F�N�g���o���Ԋu
	m_bLoop = false;                            // ���[�v���邩���Ȃ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CEmitterData::~CEmitterData()
{

}

//*****************************************************************************
//    CEmitter�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CEmitter::CEmitter(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̐ݒ�
	m_nType = 0;                            // ��ޔԍ�
	m_pTexture = NULL;                      // �e�N�X�`���ւ̃|�C���^
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���W
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ����
	D3DXMatrixIdentity(&m_MtxWorld);        // ���[���h�}�g���b�N�X
	m_nTexIdx = 0;                          // �g�p����e�N�X�`���ԍ�
	m_nEffectIdx = 0;                       // �g�p����G�t�F�N�g�̔ԍ�
	m_nLife = 0;                            // �G�~�b�^�̎���
	m_nLap = 0;                             // 1��̕��o�^�C�~���O�ɏo���G�t�F�N�g�̌�
	m_nAppear = 0;                          // ���o����Ԋu
	m_nCounter = 0;                         // ���o�^�C�~���O���Ǘ�����J�E���^�[
	m_bLoop = false;                        // ���[�v���邩���Ȃ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CEmitter::~CEmitter()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CEmitter::Init(void)
{
	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CEmitter::Uninit(void)
{
	// ���g�̃|�C���^���폜
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CEmitter::Update(void)
{
	if (m_bLoop != true)
	{// ���[�v���Ȃ��ݒ�Ȃ��
		m_nLife--;  // ���������炷
	}

	if (m_nLife <= 0 && m_bLoop == false)
	{// �������s���ă��[�v���Ȃ��ݒ�Ȃ��
		Uninit();
	}
	else
	{// �������܂�����
		m_nCounter++;  // �J�E���^�[�𑝂₷
		if (m_nCounter % m_nAppear == 0)
		{// ���o����^�C�~���O�ɂȂ�����
			for (int nCntParticle = 0; nCntParticle < m_nLap; nCntParticle++)
			{// 1��̕��o�ŏo�������J��Ԃ�
				SetEffect();
			}
		}
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CEmitter::Draw(void)
{

}

//=============================================================================
//    ��ޔԍ���ݒ肷�鏈��
//=============================================================================
void CEmitter::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    �e�N�X�`���ւ̃|�C���^�ݒ菈��
//=============================================================================
void CEmitter::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    ���W�ݒ菈��
//=============================================================================
void CEmitter::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    �����ݒ菈��
//=============================================================================
void CEmitter::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CEmitter::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    �g�p����e�N�X�`���ԍ��ݒ菈��
//=============================================================================
void CEmitter::SetTexIdx(const int nTexIdx)
{
	m_nTexIdx = nTexIdx;
}

//=============================================================================
//    ��ސݒ菈��
//=============================================================================
void CEmitter::SetEffectIdx(const int nEffectIdx)
{
	m_nEffectIdx = nEffectIdx;
}

//=============================================================================
//    �G�t�F�N�g�𐶐�����͈͂̍ő�l�ݒ菈��
//=============================================================================
void CEmitter::SetRangeMax(const D3DXVECTOR3 RangeMax)
{
	m_RangeMax = RangeMax;
}

//=============================================================================
//    �G�t�F�N�g�𐶐�����͈͂̍ŏ��l�ݒ菈��
//=============================================================================
void CEmitter::SetRangeMin(const D3DXVECTOR3 RangeMin)
{
	m_RangeMin = RangeMin;
}

//=============================================================================
//    �����ݒ菈��
//=============================================================================
void CEmitter::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    1��̕��o�Ő�������G�t�F�N�g�̌��ݒ菈��
//=============================================================================
void CEmitter::SetLap(const int nLap)
{
	m_nLap = nLap;
}

//=============================================================================
//    ���o����Ԋu�ݒ菈��
//=============================================================================
void CEmitter::SetAppear(const int nAppear)
{
	m_nAppear = nAppear;
}

//=============================================================================
//    ���[�v���邩���Ȃ����ݒ菈��
//=============================================================================
void CEmitter::SetLoop(const bool bLoop)
{
	m_bLoop = bLoop;
}

//=============================================================================
//    ��ޔԍ����擾���鏈��
//=============================================================================
int CEmitter::GetType(void)
{
	return m_nType;
}

//=============================================================================
//    �e�N�X�`���ւ̃|�C���^�擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CEmitter::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    ���W�擾����
//=============================================================================
D3DXVECTOR3 CEmitter::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    �����擾����
//=============================================================================
D3DXVECTOR3 CEmitter::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CEmitter::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    �g�p����e�N�X�`���ԍ��擾����
//=============================================================================
int CEmitter::GetTexIdx(void)
{
	return m_nTexIdx;
}

//=============================================================================
//    ��ގ擾����
//=============================================================================
int CEmitter::GetEffectIdx(void)
{
	return m_nEffectIdx;
}

//=============================================================================
//    �G�t�F�N�g�𐶐�����͈͂̍ő�l�擾����
//=============================================================================
D3DXVECTOR3 CEmitter::GetRangeMax(void)
{
	return m_RangeMax;
}

//=============================================================================
//    �G�t�F�N�g�𐶐�����͈͂̍ŏ��l�擾����
//=============================================================================
D3DXVECTOR3 CEmitter::GetRangeMin(void)
{
	return m_RangeMin;
}

//=============================================================================
//    �����擾����
//=============================================================================
int CEmitter::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    1��̕��o�Ő�������G�t�F�N�g�̌��擾����
//=============================================================================
int CEmitter::GetLap(void)
{
	return m_nLap;
}

//=============================================================================
//    ���o����Ԋu�擾����
//=============================================================================
int CEmitter::GetAppear(void)
{
	return m_nAppear;
}

//=============================================================================
//    ���[�v���邩���Ȃ����擾����
//=============================================================================
bool CEmitter::GetLoop(void)
{
	return m_bLoop;
}

//*****************************************************************************
//    CParEmitter�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CParEmitter::CParEmitter(int nPriority, OBJTYPE objType) : CEmitter(nPriority, objType)
{
	// �e��l�̃N���A
	m_pParData = NULL;  // �p�[�e�B�N���f�[�^�N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CParEmitter::~CParEmitter()
{

}

//=============================================================================
//    ��������
//=============================================================================
CParEmitter *CParEmitter::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTexIdx, int nEffectIdx, D3DXVECTOR3 RangeMax, D3DXVECTOR3 RangeMin, int nLife, int nLap, int nAppear, bool bLoop, CParData *pParData, LPDIRECT3DTEXTURE9 pTexture, int nType, int nPriority)
{
	CParEmitter *pParEmitter = NULL;       // �p�[�e�B�N���G�~�b�^�N���X�^�̃|�C���^
	if (pParEmitter == NULL)
	{// ����������ɂȂ��Ă���
		pParEmitter = new CParEmitter(nPriority);
		if (pParEmitter != NULL)
		{// �C���X�^���X�𐶐��ł���
		    // �e��l�̐ݒ�
			pParEmitter->SetType(nType);
			pParEmitter->SetParData(pParData);
			pParEmitter->BindTexture(pTexture);
			pParEmitter->SetPos(pos);
			pParEmitter->SetRot(rot);
			pParEmitter->SetTexIdx(nTexIdx);
			pParEmitter->SetEffectIdx(nEffectIdx);
			pParEmitter->SetRangeMax(RangeMax);
			pParEmitter->SetRangeMin(RangeMin);
			pParEmitter->SetLife(nLife);
			pParEmitter->SetLap(nLap);
			pParEmitter->SetAppear(nAppear);
			pParEmitter->SetLoop(bLoop);

			if (FAILED(pParEmitter->Init()))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}

	return pParEmitter;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CParEmitter::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CEmitter::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CParEmitter::Uninit(void)
{
	// ���ʂ̏I������
	CEmitter::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CParEmitter::Update(void)
{
	// ���ʂ̍X�V����
	CEmitter::Update();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CParEmitter::Draw(void)
{
	// ���ʂ̕`�揈��
	CEmitter::Draw();
}

//=============================================================================
//    �p�[�e�B�N�����Z�b�g���鏈��
//=============================================================================
void CParEmitter::SetEffect(void)
{
	// �����ɕK�v�ȃf�[�^���쐬
	D3DXVECTOR3 pos = GetPos();                               // ���W
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // �ړ���
	D3DXVECTOR3 ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �ړ��ʂ̕ω���
	D3DXCOLOR InitCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // ���������̐F
	float fWidth = 0.0f;                                      // ��
	float fHeight = 0.0f;                                     // ����
	float fRot = 0.0f;                                        // ����
	D3DXCOLOR ChangeCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  // �F�̕ω���
	float fSpread = 0.0f;                                     // ���a�̕ω���
	float fChangeRot = 0.0f;                                  // �����̕ω���
	float fBouncing = 0.0f;                                   // �o�E���h��
	int nLife = 0;                                            // �̗�
	int nRotPattern = 0;                                      // ��]�̃p�^�[��
	bool bCollision = false;                                  // �����蔻������邩���Ȃ���
	bool bDrawAddtive = false;                                // ���Z�����ŕ`�悷�邩���Ȃ���
	bool bLighting = false;                                   // ���C�e�B���O���邩���Ȃ���

	// �f�[�^���쐬����
	if (m_pParData != NULL)
	{// ���������m�ۂ���Ă���
		// �f�[�^���������镔���̓f�[�^������������Ă���
		ChangeMove = m_pParData->GetChangeMove();
		InitCol = m_pParData->GetInitCol();
		ChangeCol = m_pParData->GetChangeCol();
		fSpread = m_pParData->GetSpread();
		fBouncing = m_pParData->GetBouncing();
		fChangeRot = m_pParData->GetRotSpeed();
		nRotPattern = m_pParData->GetRotPattern();
		bCollision = m_pParData->GetCollision();
		bDrawAddtive = m_pParData->GetDrawAddtive();
		bLighting = m_pParData->GetLighting();

		// �G�~�b�^���g�̌����𔽉f�����邽�߂ɍs��v�Z
		D3DXMATRIX mtxRot;
		D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y, GetRot().x, GetRot().z);

		// �����_���ɏo�͂���Ƃ��낾���v�Z����
		int nRange = 0;

		// ����
		int nMaxRot = (int)m_pParData->GetMaxRot();
		int nMinRot = (int)m_pParData->GetMinRot();
		nRange = nMaxRot - nMinRot;
		if (nRange >= 1)
		{
			fRot = (float)((rand() % nRange));
		}
		else
		{
			fRot = m_pParData->GetMinRot();
		}
		fRot = D3DXToRadian(fRot);

		// �͈͂̌v�Z
		D3DXVECTOR3 Range;
		D3DXVECTOR3 RangeMax = GetRangeMax();
		D3DXVECTOR3 RangeMin = GetRangeMin();
		// X����
		float fRangeX = 0.0f;
		int nRangeMaxX = (int)RangeMax.x;
		int nRangeMinX = (int)RangeMin.x;
		nRange = nRangeMaxX - nRangeMinX;
		if (nRange >= 1)
		{
			fRangeX = (float)(rand() % nRange + nRangeMinX);
		}
		else
		{
			fRangeX = RangeMin.x;
		}

		// Y����
		float fRangeY = 0.0f;
		int nRangeMaxY = (int)RangeMax.y;
		int nRangeMinY = (int)RangeMin.y;
		nRange = nRangeMaxY - nRangeMinY;
		if (nRange >= 1)
		{
			fRangeY = (float)(rand() % nRange + nRangeMinY);
		}
		else
		{
			fRangeY = RangeMin.y;
		}

		// Z����
		float fRangeZ = 0.0f;
		int nRangeMaxZ = (int)RangeMax.z;
		int nRangeMinZ = (int)RangeMin.z;
		nRange = nRangeMaxZ - nRangeMinZ;
		if (nRange >= 1)
		{
			fRangeZ = (float)(rand() % nRange + nRangeMinZ);
		}
		else
		{
			fRangeZ = RangeMin.z;
		}
		Range = D3DXVECTOR3(fRangeX, fRangeY, fRangeZ);
		D3DXVec3TransformCoord(&Range, &Range, &mtxRot);

		// �Z�o���ꂽ�͈͂��G�~�b�^�̍��W�ɉ��������̂��p�[�e�B�N���̕��o�ʒu�Ƃ���
		pos += Range;

		// �ړ��ʂ̌v�Z
		D3DXVECTOR3 MaxMove = m_pParData->GetMaxMove();
		D3DXVECTOR3 MinMove = m_pParData->GetMinMove();
		D3DXVECTOR3 Move;

		// �ړ��ʂ��v�Z����
		// X����
		float fMoveX = 0.0f;
		int nMaxMoveX = (int)MaxMove.x;
		int nMinMoveX = (int)MinMove.x;
		nRange = nMaxMoveX - nMinMoveX;
		if (nRange >= 1)
		{
			fMoveX = (float)(rand() % nRange + nMinMoveX);
		}
		else
		{
			fMoveX = MinMove.x;
		}

		// Y����
		float fMoveY = 0.0f;
		int nMaxMoveY = (int)MaxMove.y;
		int nMinMoveY = (int)MinMove.y;
		nRange = nMaxMoveY - nMinMoveY;
		if (nRange >= 1)
		{
			fMoveY = (float)(rand() % nRange + nMinMoveY);
		}
		else
		{
			fMoveY = MinMove.y;
		}

		// Z����
		float fMoveZ = 0.0f;
		int nMaxMoveZ = (int)MaxMove.z;
		int nMinMoveZ = (int)MinMove.z;
		nRange = nMaxMoveZ - nMinMoveZ;
		if (nRange >= 1)
		{
			fMoveZ = (float)(rand() % nRange + nMinMoveZ);
		}
		else
		{
			fMoveZ = MinMove.z;
		}

		// �����𔽉f�����ړ��ʂ�ݒ�
		Move = D3DXVECTOR3(fMoveX, fMoveY, fMoveZ);
		D3DXVec3TransformCoord(&Move, &Move, &mtxRot);
		D3DXVec3TransformCoord(&ChangeMove, &ChangeMove, &mtxRot);

		// �傫��
		// ��
		int nMaxWidth = (int)m_pParData->GetMaxWidth();
		int nMinWidth = (int)m_pParData->GetMinWidth();
		nRange = nMaxWidth - nMinWidth;
		if (nRange >= 1)
		{
			fWidth = (float)((rand() % nRange) + nMinWidth);
		}
		else
		{
			fWidth = (float)nMinWidth;
		}

		// ����
		int nMaxHeight = (int)m_pParData->GetMaxHeight();
		int nMinHeight = (int)m_pParData->GetMinHeight();
		nRange = nMaxHeight - nMinHeight;
		if (nRange >= 1)
		{
			fHeight = (float)((rand() % nRange) + nMinHeight);
		}
		else
		{
			fHeight = (float)nMinHeight;
		}

		// ����
		int nMaxLife = m_pParData->GetMaxLife();
		int nMinLife = m_pParData->GetMinLife();
		nRange = nMaxLife - nMinLife;
		if (nRange >= 1)
		{
			nLife = rand() % nRange + nMinLife;
		}
		else
		{
			nLife = nMinLife;
		}

		// �p�[�e�B�N���̐���
		CParticle *pParticle = CParticle::Create(pos, Move, ChangeMove, InitCol, fWidth, fHeight, fRot, ChangeCol, fSpread, fChangeRot, fBouncing, nLife, nRotPattern, bCollision, bDrawAddtive, bLighting, EMITTER_EFFECT_PRIORITY);
		if (pParticle != NULL)
		{// �p�[�e�B�N���̐����ɐ�������
			pParticle->BindTexture(GetTexture());
		}
	}
}

//=============================================================================
//    �p�[�e�B�N���f�[�^�ւ̃|�C���^�ݒ菈��
//=============================================================================
void CParEmitter::SetParData(CParData *pParData)
{
	m_pParData = pParData;
}

//*****************************************************************************
//    CRingEmitter�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CRingEmitter::CRingEmitter(int nPriority, OBJTYPE objType) : CEmitter(nPriority, objType)
{
	// �e��l�̃N���A
	m_pRingData = NULL;  // �����O�G�t�F�N�g�f�[�^�N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CRingEmitter::~CRingEmitter()
{

}

//=============================================================================
//    ��������
//=============================================================================
CRingEmitter *CRingEmitter::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTexIdx, int nEffectIdx, D3DXVECTOR3 RangeMax, D3DXVECTOR3 RangeMin, int nLife, int nLap, int nAppear, bool bLoop, CRingData *pRingData, LPDIRECT3DTEXTURE9 pTexture, int nType, int nPriority)
{
	CRingEmitter *pRingEmitter = NULL;     // �����O�G�t�F�N�g�G�~�b�^�N���X�^�̃|�C���^
	if (pRingEmitter == NULL)
	{// ����������ɂȂ��Ă���
		pRingEmitter = new CRingEmitter(nPriority);
		if (pRingEmitter != NULL)
		{// �C���X�^���X�𐶐��ł���
		 // �e��l�̐ݒ�
			pRingEmitter->SetType(nType);
			pRingEmitter->SetRingData(pRingData);
			pRingEmitter->BindTexture(pTexture);
			pRingEmitter->SetPos(pos);
			pRingEmitter->SetRot(rot);
			pRingEmitter->SetTexIdx(nTexIdx);
			pRingEmitter->SetEffectIdx(nEffectIdx);
			pRingEmitter->SetRangeMax(RangeMax);
			pRingEmitter->SetRangeMin(RangeMin);
			pRingEmitter->SetLife(nLife);
			pRingEmitter->SetLap(nLap);
			pRingEmitter->SetAppear(nAppear);
			pRingEmitter->SetLoop(bLoop);

			if (FAILED(pRingEmitter->Init()))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}

	return pRingEmitter;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CRingEmitter::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CEmitter::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CRingEmitter::Uninit(void)
{
	// ���ʂ̏I������
	CEmitter::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CRingEmitter::Update(void)
{
	// ���ʂ̍X�V����
	CEmitter::Update();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CRingEmitter::Draw(void)
{
	// ���ʂ̕`�揈��
	CEmitter::Draw();
}

//=============================================================================
//    �����O�G�t�F�N�g���Z�b�g���鏈��
//=============================================================================
void CRingEmitter::SetEffect(void)
{
	// �����ɕK�v�ȃf�[�^���쐬
	D3DXVECTOR3 pos = GetPos();                               // ���W
	D3DXVECTOR3 ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �ړ��ʂ̕ω���
	D3DXCOLOR InitCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // ���������̐F
	float fHeight = 0.0f;                                     // ����
	float fRadius = 0.0f;                                     // ���a
	float fDiffusion = 0.0f;                                  // ��
	float fChangeHeight = 0.0f;                               // �����𖈃t���[���ǂꂭ�炢�ω������邩
	float fChangeRadius = 0.0f;                               // ���a�𖈃t���[���ǂꂭ�炢�ω������邩
	float fChangeDiffusion = 0.0f;                            // ���𖈃t���[���ǂꂭ�炢�ω������邩
	float fRot = 0.0f;                                        // ����
	D3DXCOLOR ChangeCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  // �F�̕ω���
	float fGravity = 0.0f;                                    // �d��
	float fSpread = 0.0f;                                     // ���a�̕ω���
	D3DXVECTOR3 RotSpeed = D3DXVECTOR3(0.0f,0.0f,0.0f);       // �����̕ω���
	float fBouncing = 0.0f;                                   // �o�E���h��
	int nLife = 0;                                            // �̗�
	int nXBlock = 0;                                          // ���̕�����
	int nYBlock = 0;                                          // �c�̕�����
	int nRotPattern = 0;                                      // ��]�̃p�^�[��
	bool bCulling = false;                                    // �J�����O�����邩���Ȃ���
	bool bDrawAddtive = false;                                // ���Z�����ŕ`�悷�邩���Ȃ���

	// �f�[�^���쐬����
	if (m_pRingData != NULL)
	{// ���������m�ۂ���Ă���
	    // �f�[�^���������镔���̓f�[�^������������Ă���
		ChangeMove = m_pRingData->GetChangeMove();
		InitCol = m_pRingData->GetInitCol();
		ChangeCol = m_pRingData->GetChangeCol();
		fHeight = m_pRingData->GetHeight();
		fRadius = m_pRingData->GetRadius();
		fDiffusion = m_pRingData->GetDiffusion();
		fChangeHeight = m_pRingData->GetChangeHeight();
		fChangeRadius = m_pRingData->GetChangeRadius();
		fChangeDiffusion = m_pRingData->GetChangeDiffusion();
		RotSpeed = m_pRingData->GetRotSpeed();
		nXBlock = m_pRingData->GetXBlock();
		nYBlock = m_pRingData->GetYBlock();
		nRotPattern = m_pRingData->GetRotPattern();
		bCulling = m_pRingData->GetCulling();
		bDrawAddtive = m_pRingData->GetDrawAddtive();

		// �G�~�b�^���g�̌����𔽉f�����邽�߂ɍs��v�Z
		D3DXMATRIX mtxRot;
		D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y, GetRot().x, GetRot().z);

		// �����_���ɏo�͂���Ƃ��낾���v�Z����
		int nRange = 0;

		// �͈͂̌v�Z
		D3DXVECTOR3 Range;
		D3DXVECTOR3 RangeMax = GetRangeMax();
		D3DXVECTOR3 RangeMin = GetRangeMin();
		// X����
		float fRangeX = 0.0f;
		int nRangeMaxX = (int)RangeMax.x;
		int nRangeMinX = (int)RangeMin.x;
		nRange = nRangeMaxX - nRangeMinX;
		if (nRange >= 1)
		{
			fRangeX = (float)(rand() % nRange + nRangeMinX);
		}
		else
		{
			fRangeX = RangeMin.x;
		}

		// Y����
		float fRangeY = 0.0f;
		int nRangeMaxY = (int)RangeMax.y;
		int nRangeMinY = (int)RangeMin.y;
		nRange = nRangeMaxY - nRangeMinY;
		if (nRange >= 1)
		{
			fRangeY = (float)(rand() % nRange + nRangeMinY);
		}
		else
		{
			fRangeY = RangeMin.y;
		}

		// Z����
		float fRangeZ = 0.0f;
		int nRangeMaxZ = (int)RangeMax.z;
		int nRangeMinZ = (int)RangeMin.z;
		nRange = nRangeMaxZ - nRangeMinZ;
		if (nRange >= 1)
		{
			fRangeZ = (float)(rand() % nRange + nRangeMinZ);
		}
		else
		{
			fRangeZ = RangeMin.z;
		}
		Range = D3DXVECTOR3(fRangeX, fRangeY, fRangeZ);
		D3DXVec3TransformCoord(&Range, &Range, &mtxRot);

		// �Z�o���ꂽ�͈͂��G�~�b�^�̍��W�ɉ��������̂��p�[�e�B�N���̕��o�ʒu�Ƃ���
		pos += Range;

		// �����̌v�Z
		D3DXVECTOR3 MaxRot = m_pRingData->GetMaxRot();
		D3DXVECTOR3 MinRot = m_pRingData->GetMinRot();
		D3DXVECTOR3 Rot;

		// X����
		float fRotX = 0.0f;
		int nMaxRotX = (int)MaxRot.x;
		int nMinRotX = (int)MinRot.x;
		nRange = nMaxRotX - nMinRotX;
		if (nRange >= 1)
		{
			fRotX = (float)(rand() % nRange + nMinRotX);
		}
		else
		{
			fRotX = MinRot.x;
		}

		// Y����
		float fRotY = 0.0f;
		int nMaxRotY = (int)MaxRot.y;
		int nMinRotY = (int)MinRot.y;
		nRange = nMaxRotY - nMinRotY;
		if (nRange >= 1)
		{
			fRotY = (float)(rand() % nRange + nMinRotY);
		}
		else
		{
			fRotY = MinRot.y;
		}

		// Z����
		float fRotZ = 0.0f;
		int nMaxRotZ = (int)MaxRot.z;
		int nMinRotZ = (int)MinRot.z;
		nRange = nMaxRotZ - nMinRotZ;
		if (nRange >= 1)
		{
			fRotZ = (float)(rand() % nRange + nMinRotZ);
		}
		else
		{
			fRotZ = MinRot.z;
		}
		Rot = D3DXVECTOR3(fRotX, fRotY, fRotZ);

		// �G�~�b�^���g�̌��������Z����
		D3DXVECTOR3 EmitterRot = GetRot();
		EmitterRot.x = D3DXToDegree(EmitterRot.x);
		EmitterRot.y = D3DXToDegree(EmitterRot.y);
		EmitterRot.z = D3DXToDegree(EmitterRot.z);
		Rot += EmitterRot;

		// �������~�������Z�ɂ��Ă���
		Rot.x = D3DXToRadian(Rot.x);
		Rot.y = D3DXToRadian(Rot.y);
		Rot.z = D3DXToRadian(Rot.z);


		// �ړ��ʂ̌v�Z
		D3DXVECTOR3 MaxMove = m_pRingData->GetMaxMove();
		D3DXVECTOR3 MinMove = m_pRingData->GetMinMove();
		D3DXVECTOR3 Move;

		// �ړ��ʂ��v�Z����
		// X����
		float fMoveX = 0.0f;
		int nMaxMoveX = (int)MaxMove.x;
		int nMinMoveX = (int)MinMove.x;
		nRange = nMaxMoveX - nMinMoveX;
		if (nRange >= 1)
		{
			fMoveX = (float)(rand() % nRange + nMinMoveX);
		}
		else
		{
			fMoveX = MinMove.x;
		}

		// Y����
		float fMoveY = 0.0f;
		int nMaxMoveY = (int)MaxMove.y;
		int nMinMoveY = (int)MinMove.y;
		nRange = nMaxMoveY - nMinMoveY;
		if (nRange >= 1)
		{
			fMoveY = (float)(rand() % nRange + nMinMoveY);
		}
		else
		{
			fMoveY = MinMove.y;
		}

		// Z����
		float fMoveZ = 0.0f;
		int nMaxMoveZ = (int)MaxMove.z;
		int nMinMoveZ = (int)MinMove.z;
		nRange = nMaxMoveZ - nMinMoveZ;
		if (nRange >= 1)
		{
			fMoveZ = (float)(rand() % nRange + nMinMoveZ);
		}
		else
		{
			fMoveZ = MinMove.z;
		}

		// �����𔽉f�����ړ��ʂ�ݒ�
		Move = D3DXVECTOR3(fMoveX, fMoveY, fMoveZ);
		D3DXVec3TransformCoord(&Move, &Move, &mtxRot);
		D3DXVec3TransformCoord(&ChangeMove, &ChangeMove, &mtxRot);

		// ����
		int nMaxLife = m_pRingData->GetMaxLife();
		int nMinLife = m_pRingData->GetMinLife();
		nRange = nMaxLife - nMinLife;
		if (nRange >= 1)
		{
			nLife = rand() % nRange + nMinLife;
		}
		else
		{
			nLife = nMinLife;
		}

		// �����O�G�t�F�N�g�̐���
		CRingEffect *pRingEffect = CRingEffect::Create(pos, Rot, InitCol, fHeight, fRadius, fDiffusion, nXBlock, nYBlock, nLife, RotSpeed, nRotPattern, Move, ChangeMove, ChangeCol, fChangeHeight, fChangeRadius, fChangeDiffusion, bCulling, bDrawAddtive, EMITTER_EFFECT_PRIORITY);
		if (pRingEffect != NULL)
		{// �����O�G�t�F�N�g�̐����ɐ�������
			pRingEffect->BindTexture(GetTexture());
		}
	}
}

//=============================================================================
//    �����O�G�t�F�N�g�f�[�^�ւ̃|�C���^�ݒ菈��
//=============================================================================
void CRingEmitter::SetRingData(CRingData *pRingData)
{
	m_pRingData = pRingData;
}