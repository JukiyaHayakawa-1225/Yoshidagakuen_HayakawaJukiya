//=============================================================================
//
// �I�u�W�F�N�g3D���� [object3D.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "object3D.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "model.h"
#include "boxCollider.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
//CBoxCollider *CObject3D::m_pBoxCollider = NULL;

//=============================================================================
// �I�u�W�F�N�g3D�̃R���X�g���N�^
//=============================================================================
CObject3D::CObject3D(int nPriority,OBJTYPE type) : CScene(nPriority,type)
{
	m_pos = INITIALIZE_D3DXVECTOR3;				//�ʒu
	m_rot = INITIALIZE_D3DXVECTOR3;				//����
	m_colRange = INITIALIZE_D3DXVECTOR3;		//�����蔻��̑傫��
	m_bReturnFlag = true;						//���������ۂɖ߂����ǂ���
	m_pBoxCollider = NULL;
}
//=============================================================================
// �I�u�W�F�N�g3D�̃f�X�g���N�^
//=============================================================================
CObject3D::~CObject3D()
{

}
//=============================================================================
// �I�u�W�F�N�g3D�̐���
//=============================================================================
CObject3D *CObject3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CObject3D *pObject3D = NULL;

	if (pObject3D == NULL)
	{//NULL�̏ꍇ
		pObject3D = new CObject3D;	//���I�m��
		if (pObject3D != NULL)
		{//NULL�łȂ��ꍇ
			pObject3D->SetPos(pos);			//�ʒu�̐ݒu����
			pObject3D->SetRot(rot);			//�����̐ݒu����
			pObject3D->Init();				//����������
		}
	}
	return pObject3D;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CObject3D::Init()
{
	//�ʒu�̎擾����
	D3DXVECTOR3 pos = GetPos();

	//�����̎擾����
	D3DXVECTOR3 rot = GetRot();

	//�����蔻�蔠�̐�������
	if (m_pBoxCollider == NULL)
	{
		m_pBoxCollider = CBoxCollider::Create(pos, m_colRange.x, m_colRange.y, m_colRange.z,m_bReturnFlag);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CObject3D::Uninit(void)
{
	//�����蔻�蔠�̏I������
	if (m_pBoxCollider != NULL)
	{
		m_pBoxCollider->Uninit();
		delete m_pBoxCollider;
		m_pBoxCollider = NULL;
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CObject3D::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CObject3D::Draw(void)
{
	//�����蔻�蔠�̕`�揈��
	if (m_pBoxCollider != NULL)
	{
		m_pBoxCollider->Draw();
	}
}

//=============================================================================
// �ʒu�̐ݒu����
//=============================================================================
void CObject3D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// �����̐ݒu����
//=============================================================================
void CObject3D::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// �����蔻�蔠�̐ݒu����
//=============================================================================
void CObject3D::SetBoxCollider(CBoxCollider *pBoxCollider)
{
	m_pBoxCollider = pBoxCollider;
}

//=============================================================================
// �����蔻��̑傫���̐ݒu����
//=============================================================================
void CObject3D::SetColRange(D3DXVECTOR3 colRange)
{
	m_colRange = colRange;
}

//=============================================================================
// ���������ۖ߂����ǂ����̐ݒu����
//=============================================================================
void CObject3D::SetReturnFlag(bool bReturnFlag)
{
	m_bReturnFlag = bReturnFlag;
}

//=============================================================================
// ���[���h�}�g���b�N�X�̌v�Z����
//=============================================================================
void CObject3D::SetMtxWorld(LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�s����쐬(D3DXMatrixRotationYawPitchRoll�Q��)
	float fSinPitch = sinf(m_rot.x);
	float fCosPitch = cosf(m_rot.x);
	float fSinYaw = sinf(m_rot.y);
	float fCosYaw = cosf(m_rot.y);
	float fSinRoll = sinf(m_rot.z);
	float fCosRoll = cosf(m_rot.z);
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
	m_mtxWorld._11 = mtxRot._11;
	m_mtxWorld._12 = mtxRot._12;
	m_mtxWorld._13 = mtxRot._13;
	m_mtxWorld._21 = mtxRot._21;
	m_mtxWorld._22 = mtxRot._22;
	m_mtxWorld._23 = mtxRot._23;
	m_mtxWorld._31 = mtxRot._31;
	m_mtxWorld._32 = mtxRot._32;
	m_mtxWorld._33 = mtxRot._33;

	// �I�t�Z�b�g�ʒu�𔽉f
	m_mtxWorld._41 = m_pos.x;
	m_mtxWorld._42 = m_pos.y;
	m_mtxWorld._43 = m_pos.z;

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//=============================================================================
// ���[���h�}�g���b�N�X�̐ݒ菈��
//=============================================================================
void CObject3D::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_mtxWorld = mtxWorld;
}

//=============================================================================
// �ʒu�̎擾����
//=============================================================================
D3DXVECTOR3 CObject3D::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// �����̎擾����
//=============================================================================
D3DXVECTOR3 CObject3D::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// ���[���h�}�g���b�N�X�̎擾����
//=============================================================================
D3DXMATRIX CObject3D::GetMtxWorld(void)
{
	return m_mtxWorld;
}

//=============================================================================
// �����蔻��̑傫���̎擾����
//=============================================================================
D3DXVECTOR3 CObject3D::GetColRange(void)
{
	return m_colRange;
}

//=============================================================================
// ���������ۖ߂����ǂ����̎擾����
//=============================================================================
bool CObject3D::GetReturnFlag(void)
{
	return m_bReturnFlag;
}

//=============================================================================
// �����蔻��̏���
//=============================================================================
void CObject3D::Hit(CScene *pScene)
{

}

//=============================================================================
// �����蔻�蔠�̎擾����
//=============================================================================
CBoxCollider * CObject3D::GetBoxCollider(void)
{
	return m_pBoxCollider;
}