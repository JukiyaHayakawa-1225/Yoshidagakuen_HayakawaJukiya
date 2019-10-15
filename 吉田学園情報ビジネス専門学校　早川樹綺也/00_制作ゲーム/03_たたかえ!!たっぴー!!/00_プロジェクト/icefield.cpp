//*****************************************************************************
//
//     �X�̏���[icefield.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "icefield.h"
#include "boxCollider.h"
#include "meshfield.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************


//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CIceField::CIceField(int nPriority, OBJTYPE objType) : CObject3D(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CIceField::~CIceField()
{

}

//=============================================================================
//    ��������
//=============================================================================
CIceField *CIceField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPDIRECT3DTEXTURE9 pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nXBlock, int nZBlock, int nTexIdx, int nPriority)
{
	CIceField *pIceField = NULL;  // �X�N���X�^�̃|�C���^
	if (pIceField == NULL)
	{// ����������ɂȂ��Ă���
		pIceField = new CIceField(nPriority);
		if (pIceField != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pIceField->Init(pos, rot, pTexture, fBoxWidth, fBoxHeight, fBoxDepth, nXBlock, nZBlock, nTexIdx)))
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

	return pIceField;
}


//=============================================================================
//    ����������
//=============================================================================
HRESULT CIceField::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPDIRECT3DTEXTURE9 pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nXBlock, int nZBlock, int nTexIdx)
{
	// �e��l�̐ݒ�
	SetPos(pos);
	SetRot(rot);
	SetTexIdx(nTexIdx);

	// �����蔻��p�����f���𐶐�
	SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);

	// �X�Ɏg���|���S���𐶐�
	m_pMeshField = CMeshField::Create(pos, rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MASS_SIZE_X, MASS_SIZE_X, nXBlock, nZBlock, nXBlock, nZBlock, NULL, true);
	m_pMeshField->BindTexture(pTexture);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CIceField::Uninit(void)
{
	// �n�ʂ̔j��
	if (m_pMeshField != NULL)
	{
		m_pMeshField->Uninit();
		m_pMeshField = NULL;
	}

	// ���ʂ̏I������
	CObject3D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CIceField::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CIceField::Draw(void)
{
	// ���ʂ̕`�揈��
	CObject3D::Draw();
}

//=============================================================================
//    �����蔻��p�����f�����쐬���鏈��
//=============================================================================
void CIceField::CreateBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	CBoxCollider *pBoxCollider = CBoxCollider::Create(GetPos(), fBoxWidth, fBoxHeight, fBoxDepth, false);
	SetBoxCollider(pBoxCollider);
}

//=============================================================================
//    �g�p���Ă���e�N�X�`���̔ԍ���ݒ肷��
//=============================================================================
void CIceField::SetTexIdx(const int nTexIdx)
{
	m_nTexIdx = nTexIdx;
}

//=============================================================================
//    �g�p���Ă���e�N�X�`���̔ԍ����擾����
//=============================================================================
int CIceField::GetTexIdx(void)
{
	return m_nTexIdx;
}

//=============================================================================
//    �X�p�̃��b�V���t�B�[���h�N���X���擾
//=============================================================================
CMeshField *CIceField::GetMeshField(void)
{
	return m_pMeshField;
}