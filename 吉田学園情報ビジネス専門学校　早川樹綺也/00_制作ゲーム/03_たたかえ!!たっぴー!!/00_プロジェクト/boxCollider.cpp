//*****************************************************************************
//
//     �����蔻��p�����f���̏���[boxCollider.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "boxCollider.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
//#define BOX_COLLIDER_DISP
#define BOX_COLLIDER_LINECOLOR (D3DXCOLOR(0.0f,0.0f,1.0f,1.0f))  // ���̐F
#define BOX_COLLIDER_LINEWIDTH (1.2f)                            // ���̕�

//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBoxCollider::CBoxCollider()
{
	// �e��l�̃N���A
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ���W
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ����
	D3DXMatrixIdentity(&m_MtxWorld);          // ���[���h�}�g���b�N�X
	m_fWidth = 0.0f;                          // ���̉���
	m_fHeight = 0.0f;                         // ���̍���
	m_fDepth = 0.0f;                          // ���̉��s
	m_bReturnFlag = false;                    // ���������ۂɖ߂����ǂ���
	m_pLine = NULL;                           // ���`��p�C���X�^���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBoxCollider::~CBoxCollider()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBoxCollider *CBoxCollider::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth, bool bReturnFlag)
{
	CBoxCollider *pBoxCollider = NULL;      // �����蔻��p�����f���N���X�^�̃|�C���^
	if (pBoxCollider == NULL)
	{// ����������ɂȂ��Ă���
		pBoxCollider = new CBoxCollider;
		if (pBoxCollider != NULL)
		{// �C���X�^���X�𐶐��ł���
		    // �e��l�̐ݒ�
			pBoxCollider->SetPos(pos);                 // ���W
			pBoxCollider->SetWidth(fWidth);            // ���̉���
			pBoxCollider->SetHeight(fHeight);          // ���̍���
			pBoxCollider->SetDepth(fDepth);            // ���̉��s
			pBoxCollider->SetReturnFlag(bReturnFlag);  // ���������ۂɖ߂����ǂ���

			if (FAILED(pBoxCollider->Init()))
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

	return pBoxCollider;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBoxCollider::Init(void)
{
#ifdef BOX_COLLIDER_DISP
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			// ���`��p�C���X�^���X�𐶐�
			D3DXCreateLine(pDevice, &m_pLine);

			// ���_���W��o�^
			m_VecPos[0] = D3DXVECTOR3(-m_fWidth / 2, m_fHeight, m_fDepth / 2);
			m_VecPos[1] = D3DXVECTOR3(m_fWidth / 2, m_fHeight, m_fDepth / 2);
			m_VecPos[2] = D3DXVECTOR3(-m_fWidth / 2, m_fHeight, -m_fDepth / 2);
			m_VecPos[3] = D3DXVECTOR3(m_fWidth / 2, m_fHeight, -m_fDepth / 2);
			m_VecPos[4] = D3DXVECTOR3(-m_fWidth / 2, 0.0f, m_fDepth / 2);
			m_VecPos[5] = D3DXVECTOR3(m_fWidth / 2, 0.0f, m_fDepth / 2);
			m_VecPos[6] = D3DXVECTOR3(-m_fWidth / 2, 0.0f, -m_fDepth / 2);
			m_VecPos[7] = D3DXVECTOR3(m_fWidth / 2, 0.0f, -m_fDepth / 2);

			// ���_�C���f�b�N�X����o�^
			// �P�{��
			m_Index[0] = 0;
			m_Index[1] = 2;
			// �Q�{��
			m_Index[2] = 0;
			m_Index[3] = 1;
			// �R�{��
			m_Index[4] = 1;
			m_Index[5] = 3;
			// �S�{��
			m_Index[6] = 3;
			m_Index[7] = 2;
			// �T�{��
			m_Index[8] = 4;
			m_Index[9] = 6;
			// �U�{��
			m_Index[10] = 4;
			m_Index[11] = 5;
			// �V�{��
			m_Index[12] = 5;
			m_Index[13] = 7;
			// �W�{��
			m_Index[14] = 7;
			m_Index[15] = 6;
			// �X�{��
			m_Index[16] = 0;
			m_Index[17] = 4;
			// �P�O�{��
			m_Index[18] = 1;
			m_Index[19] = 5;
			// �P�P�{��
			m_Index[20] = 2;
			m_Index[21] = 6;
			// �P�Q�{��
			m_Index[22] = 3;
			m_Index[23] = 7;
		}
	}
#endif
	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CBoxCollider::Uninit(void)
{
	DIRECT_RELEASE(m_pLine);
}

//=============================================================================
//    �X�V����
//=============================================================================
void CBoxCollider::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBoxCollider::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			// ���[���h�}�g���b�N�X�̐ݒ菈��
			SetMtxWorld(pDevice);
#ifdef BOX_COLLIDER_DISP
			// �������e�ϊ��s����J��������쐬����
			D3DXMATRIX mtxView;
			D3DXMATRIX mtxProj;
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);
			D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxView);
			D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxProj);

			// ����`��
			D3DXVECTOR3 Vec[2];
			m_pLine->SetAntialias(TRUE);
			m_pLine->SetWidth(BOX_COLLIDER_LINEWIDTH);
			m_pLine->Begin();
			for (int nCnt = 0; nCnt < 12; nCnt++)
			{
				Vec[0] = m_VecPos[m_Index[(nCnt * 2)]];
				Vec[1] = m_VecPos[m_Index[(nCnt * 2) + 1]];
				m_pLine->DrawTransform(Vec, 2, &m_MtxWorld, BOX_COLLIDER_LINECOLOR);
			}
			m_pLine->End();
#endif
		}
	}
}

//=============================================================================
//    ���[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CBoxCollider::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxWorld);

	// ��]�s����쐬(D3DXMatrixRotationYawPitchRoll�Q��)
	float fSinPitch = sinf(m_Rot.x);
	float fCosPitch = cosf(m_Rot.x);
	float fSinYaw = sinf(m_Rot.y);
	float fCosYaw = cosf(m_Rot.y);
	float fSinRoll = sinf(m_Rot.z);
	float fCosRoll = cosf(m_Rot.z);
	mtxRot._11 = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
	mtxRot._12 = fSinRoll * fCosPitch;
	mtxRot._13 = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
	mtxRot._21 = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
	mtxRot._22 = fCosRoll * fCosPitch;
	mtxRot._23 = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
	mtxRot._31 = fCosPitch * fSinYaw;
	mtxRot._32 = -fSinPitch;
	mtxRot._33 = fCosPitch * fCosYaw;

	// ��]�𔽉f����
	m_MtxWorld._11 = mtxRot._11;
	m_MtxWorld._12 = mtxRot._12;
	m_MtxWorld._13 = mtxRot._13;
	m_MtxWorld._21 = mtxRot._21;
	m_MtxWorld._22 = mtxRot._22;
	m_MtxWorld._23 = mtxRot._23;
	m_MtxWorld._31 = mtxRot._31;
	m_MtxWorld._32 = mtxRot._32;
	m_MtxWorld._33 = mtxRot._33;

	// �I�t�Z�b�g�ʒu�𔽉f
	m_MtxWorld._41 = m_Pos.x;
	m_MtxWorld._42 = m_Pos.y;
	m_MtxWorld._43 = m_Pos.z;

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
bool CBoxCollider::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand)
{
	bool bHit = false;  // �����������ǂ���

	// Y����̔���
	if (pPos->y < m_Pos.y + m_fHeight && pPos->y + ColRange.y > m_Pos.y)
	{// ���݂̍��������̒��ɂ���
	    // X����̔���
		if (pPos->z + ColRange.z > m_Pos.z - (m_fDepth / 2) && pPos->z - ColRange.z < m_Pos.z + (m_fDepth / 2))
		{// Z����ɂ��邩�ǂ�������
			if (pPosOld->x + ColRange.x <= m_Pos.x - (m_fWidth / 2) && pPos->x + ColRange.x >= m_Pos.x - (m_fWidth / 2))
			{// ���̍������������
				if (m_bReturnFlag == true)
				{// �ʒu��߂��Ȃ��
					pPos->x = m_Pos.x - ColRange.x - (m_fWidth / 2);
				}
				bHit = true;
			}
			else if (pPosOld->x - ColRange.x >= m_Pos.x + (m_fWidth / 2) && pPos->x - ColRange.x <= m_Pos.x + (m_fWidth / 2))
			{// ���̉E�����������
				if (m_bReturnFlag == true)
				{// �ʒu��߂��Ȃ��
					pPos->x = m_Pos.x + ColRange.x + (m_fWidth / 2);
				}
				bHit = true;
			}
		}
		// Z����̔���
		if (pPos->x + ColRange.x > m_Pos.x - (m_fWidth / 2) && pPos->x - ColRange.x < m_Pos.x + (m_fWidth / 2))
		{// X����ɂ��邩�ǂ�������
			if (pPosOld->z - ColRange.z >= m_Pos.z + (m_fDepth / 2) && pPos->z - ColRange.z <= m_Pos.z + (m_fDepth / 2))
			{// ���̉������������
				if (m_bReturnFlag == true)
				{// �ʒu��߂��Ȃ��
					pPos->z = m_Pos.z + ColRange.z + (m_fDepth / 2);
				}
				bHit = true;
			}
			else if (pPosOld->z + ColRange.z <= m_Pos.z - (m_fDepth / 2) && pPos->z + ColRange.z >= m_Pos.z - (m_fDepth / 2))
			{// ���̎�O�����������
				if (m_bReturnFlag == true)
				{// �ʒu��߂��Ȃ��
					pPos->z = m_Pos.z - ColRange.z - (m_fDepth / 2);
				}
				bHit = true;
			}
		}
	}

	// ���̓����ɂ��邩�ǂ�������
	if (pPos->x + ColRange.x >= m_Pos.x - (m_fWidth / 2) && pPos->x - ColRange.x <= m_Pos.x + (m_fWidth / 2)
		&& pPos->z + ColRange.z >= m_Pos.z - (m_fDepth / 2) && pPos->z - ColRange.z <= m_Pos.z + (m_fDepth / 2))
	{
		bHit = true;
	}

	if (pPosOld->y >= m_Pos.y + m_fHeight && pPos->y < m_Pos.y + m_fHeight)
	{// �������ォ�甠�̓����ɓ����Ă��܂���
		if (pPos->x + ColRange.x >= m_Pos.x - (m_fWidth / 2) && pPos->x - ColRange.x <= m_Pos.x + (m_fWidth / 2)
			&& pPos->z + ColRange.z >= m_Pos.z - (m_fDepth / 2) && pPos->z - ColRange.z <= m_Pos.z + (m_fDepth / 2))
		{// ���̓����ɂ��锻�肾����
			if (m_bReturnFlag == true)
			{// �ʒu��߂��Ȃ��
				pPos->y = m_Pos.y + m_fHeight;
			}
			bHit = true;
			if (pLand != NULL)
			{
				*pLand = true;
			}
		}
	}
	else if (pPosOld->y + ColRange.y <= m_Pos.y && pPos->y + ColRange.y > m_Pos.y)
	{// �����������甠�̓����ɓ����Ă��܂���
		if (pPos->x + ColRange.x >= m_Pos.x - (m_fWidth / 2) && pPos->x - ColRange.x <= m_Pos.x + (m_fWidth / 2)
			&& pPos->z + ColRange.z >= m_Pos.z - (m_fDepth / 2) && pPos->z - ColRange.z <= m_Pos.z + (m_fDepth / 2))
		{// ���̓����ɂ��锻�肾����
			if (m_bReturnFlag == true)
			{// �ʒu��߂��Ȃ��
				pPos->y = m_Pos.y - ColRange.y;
				pMove->y = 0.0f;
			}
			bHit = true;
		}
	}

	return bHit;
}

//=============================================================================
//    ���W�ݒ菈��
//=============================================================================
void CBoxCollider::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    �����ݒ菈��
//=============================================================================
void CBoxCollider::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CBoxCollider::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    ���̉����ݒ菈��
//=============================================================================
void CBoxCollider::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    ���̍����ݒ菈��
//=============================================================================
void CBoxCollider::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    ���̉��s�ݒ菈��
//=============================================================================
void CBoxCollider::SetDepth(const float fDepth)
{
	m_fDepth = fDepth;
}

//=============================================================================
//    ���������ۂɖ߂����ǂ����ݒ菈��
//=============================================================================
void CBoxCollider::SetReturnFlag(const bool bReturnFlag)
{
	m_bReturnFlag = bReturnFlag;
}

//=============================================================================
//    ���W�擾����
//=============================================================================
D3DXVECTOR3 CBoxCollider::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    �����擾����
//=============================================================================
D3DXVECTOR3 CBoxCollider::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CBoxCollider::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    ���̉����擾����
//=============================================================================
float CBoxCollider::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    ���̍����擾����
//=============================================================================
float CBoxCollider::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    ���̉��s�擾����
//=============================================================================
float CBoxCollider::GetDepth(void)
{
	return m_fDepth;
}

//=============================================================================
//    ���������ۂɖ߂����ǂ����擾����
//=============================================================================
bool CBoxCollider::GetReturnFlag(void)
{
	return m_bReturnFlag;
}