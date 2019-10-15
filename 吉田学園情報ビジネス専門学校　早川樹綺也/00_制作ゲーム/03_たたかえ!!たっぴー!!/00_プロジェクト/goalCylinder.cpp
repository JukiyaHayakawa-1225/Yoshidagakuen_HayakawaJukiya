//*****************************************************************************
//
//     �S�[���p�̉~������[goalCylinder.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "goalCylinder.h"
#include "manager.h"
#include "renderer.h"
#include "tutorial.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define GOALCYLINDER_COL_RANGE          (0.2f)  // �����蔻��Ɏg�p���钷���̔{��
#define GOALCYLINDER_COL_RANGE_TUTORIAL (5.0f)  // �S�[���ɋ߂����X�|���X��Ԃ��̂Ɏg�p���钷���̔{��

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CGoalCylinder::CGoalCylinder(int nPriority, OBJTYPE objType) : CMeshCylinder(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CGoalCylinder::~CGoalCylinder()
{

}

//=============================================================================
//    ��������
//=============================================================================
CGoalCylinder *CGoalCylinder::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, float fDecay, float fMag, float fUpValue, int nPriority)
{
	CGoalCylinder *pGoalCylinder = NULL;   // �S�[���p�~���N���X�^�̃|�C���^
	if (pGoalCylinder == NULL)
	{// ����������ɂȂ��Ă���
		pGoalCylinder = new CGoalCylinder(nPriority);
		if (pGoalCylinder != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pGoalCylinder->Init(pos, rot, col, fHeight, fRadius, nXBlock, nYBlock, fDecay, fMag, fUpValue)))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}

	return pGoalCylinder;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CGoalCylinder::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, float fDecay, float fMag, float fUpValue)
{
	// �e��l�̑��
	SetPos(pos);            // ���b�V���V�����_�[�̍��W
	SetRot(rot);            // ���b�V���V�����_�[�̌���
	SetCol(col);            // ���b�V���V�����_�[�̐F
	SetHeight(fHeight);     // ���b�V���V�����_�[�̍���
	SetRadius(fRadius);     // ���b�V���V�����_�[�̔��a
	SetXBlock(nXBlock);     // ���̕�����
	SetYBlock(nYBlock);     // �c�̕�����
	SetDecay(fDecay);       // �����l
	SetMag(fMag);           // �{��
	SetUpValue(fUpValue);   // �{���ɉ��Z�����

	// ���ʂ̏���������
	if (FAILED(CMeshCylinder::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CGoalCylinder::Uninit(void)
{
	// ���ʂ̏I������
	CMeshCylinder::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CGoalCylinder::Update(void)
{
	// �F���擾
	D3DXCOLOR col = GetCol();

	// �{���ɉ��Z����
	m_fMag += m_fUpValue;
	if (m_fMag > D3DX_PI)
	{
		m_fMag -= D3DX_PI;
	}

	// �����x��{���ɂ���ĕύX����
	col.a = sinf(m_fMag) * 1.0f;

	// ���_�̐F��{���ɂ���Ă��炷
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// �{���v�Z�p
		float fMag = m_fMag;

		for (int nCntV = 0; nCntV < GetYBlock() + 1; nCntV++)
		{// ���������̕����������J��Ԃ�
			for (int nCntH = 0; nCntH < GetXBlock() + 1; nCntH++)
			{// ���������̕����������J��Ԃ�
			    // ���_�J���[
				pVtx[nCntH].col = col;
			}
			pVtx += GetXBlock() + 1;   // ���_�o�b�t�@�̔ԍ������̕��������i�߂�
			if (nCntV % 2 == 0)
			{
				// �����x�̕ύX
				fMag += m_fDecay;
				if (fMag > D3DX_PI)
				{
					fMag -= D3DX_PI;
				}
				col.a = sinf(fMag);
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CGoalCylinder::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			DWORD Lighting;
			DWORD Culling;

			// �J�����O�����Ȃ��ݒ��
			pDevice->GetRenderState(D3DRS_CULLMODE, &Culling);
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			// ���u�����f�B���O�����Z�����ɐݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			// ���C�e�B���O�̐ݒ�
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, false);

			// ���ʂ̕`�揈��
			CMeshCylinder::Draw();

			// �J�����O�̐ݒ�����ɖ߂�
			pDevice->SetRenderState(D3DRS_CULLMODE, Culling);

			// ���C�e�B���O��߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);

			// ���u�����f�B���O�����ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CGoalCylinder::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * GetNumVertex(),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = 0;   // �p�x
	float fRadius = 0;  // ���a
	D3DXVECTOR3 pos;    // ���W
	D3DXCOLOR col;      // �F
	float Utex;         // �e�N�X�`��U���W
	float Vtex;         // �e�N�X�`��V���W

	fAngle = 0.0f;                          // �p�x��0��
	fRadius = GetRadius();                  // ���a��ݒ�
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ���W��ݒ�
	col = GetCol();                         // �F�̐ݒ�
	Utex = 0.0f;                            // �e�N�X�`��U���W��ݒ�
	Vtex = 0.0f;                            // �e�N�X�`��V���W��ݒ�

	// �����x��{���ɂ���ĕύX����
	col.a = sinf(m_fMag) * 1.0f;

	// �{���v�Z�p
	float fMag = m_fMag;

	for (int nCntV = 0; nCntV < GetYBlock() + 1; nCntV++)
	{// ���������̕����������J��Ԃ�
		for (int nCntH = 0; nCntH < GetXBlock() + 1; nCntH++)
		{// ���������̕����������J��Ԃ�
			pos.x = sinf(fAngle) * fRadius;
			pos.z = cosf(fAngle) * fRadius;

			// ���_���W
			pVtx[nCntH].pos = pos;

			// ���_�J���[
			pVtx[nCntH].col = col;

			// �e�N�X�`�����W
			pVtx[nCntH].tex = D3DXVECTOR2(Utex, Vtex);

			fAngle -= D3DX_PI / (GetXBlock() / 2.0f);  // �p�x��i�߂�
			Utex += 1.0f / (GetXBlock() / 2.0f);       // �e�N�X�`��U���W��i�߂�

			if (fAngle <= -D3DX_PI)
			{// �p�x���~�����𒴂���
				fAngle += D3DX_PI * 2.0f;
			}
		}
		pos.y += GetHeight() / (GetYBlock() + 1);  // ���W���グ��
		fAngle = 0.0f;                             // �p�x��0�ɖ߂�
		Utex = 0.0f;                               // �e�N�X�`��U���W��߂�
		Vtex -= 1.0f / GetYBlock();                // �e�N�X�`��V���W��i�߂�
		pVtx += GetXBlock() + 1;                   // ���_�o�b�t�@�̔ԍ������̕��������i�߂�

		// �����x�̕ύX
		if (nCntV % 2 == 0)
		{
			fMag += m_fDecay;
			if (fMag > D3DX_PI)
			{
				fMag -= D3DX_PI;
			}
			col.a = sinf(fMag);
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// ���_�o�b�t�@��ݒ肷��
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
bool CGoalCylinder::Collision(D3DXVECTOR3 *pPos)
{
	// ���������߂�
	D3DXVECTOR3 pos = GetPos();
	float fLength = sqrtf((pPos->x - pos.x) * (pPos->x - pos.x) + (pPos->y - pos.y) * (pPos->y - pos.y) + (pPos->z - pos.z) * (pPos->z - pos.z));

	if (fLength <= GetRadius() * GOALCYLINDER_COL_RANGE)
	{// ���������肷�钷�����Z��
		return true;
	}

	if (fLength <= GetRadius() * GOALCYLINDER_COL_RANGE_TUTORIAL && CManager::GetMode() == CManager::MODE_TUTORIAL)
	{// �`���[�g���A����ʂŃ��X�|���X��Ԃ������Ƀv���C���[������
		CTutorial *pTutorial = CManager::GetTutorial();
		if (pTutorial != NULL)
		{
			pTutorial->CreateNearGoalInfo();
		}
	}

	return false;
}

//=============================================================================
//    �����l��ݒ肷�鏈��
//=============================================================================
void CGoalCylinder::SetDecay(const float fDecay)
{
	m_fDecay = fDecay;
}

//=============================================================================
//    �{����ݒ肷�鏈��
//=============================================================================
void CGoalCylinder::SetMag(const float fMag)
{
	m_fMag = fMag;
}

//=============================================================================
//    �{�������Z����ʂ�ݒ菈��
//=============================================================================
void CGoalCylinder::SetUpValue(const float fUpValue)
{
	m_fUpValue = fUpValue;
}

//=============================================================================
//    �����l���擾���鏈��
//=============================================================================
float CGoalCylinder::GetDecay(void)
{
	return m_fDecay;
}

//=============================================================================
//    �{�����擾���鏈��
//=============================================================================
float CGoalCylinder::GetMag(void)
{
	return m_fMag;
}

//=============================================================================
//    �{�������Z����ʂ��擾���鏈��
//=============================================================================
float CGoalCylinder::GetUpValue(void)
{
	return m_fUpValue;
}