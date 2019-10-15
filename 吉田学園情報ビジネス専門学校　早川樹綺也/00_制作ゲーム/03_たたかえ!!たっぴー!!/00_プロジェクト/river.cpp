//*****************************************************************************
//
//     ��̏���[river.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "river.h"
#include "boxCollider.h"
#include "meshfield.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define RIVER_ADDTEX_U (0.0012f)
#define RIVER_ADDTEX_V (0.0004f)

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CRiver::CRiver(int nPriority, OBJTYPE objType) : CObject3D(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CRiver::~CRiver()
{

}

//=============================================================================
//    ��������
//=============================================================================
CRiver *CRiver::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPDIRECT3DTEXTURE9 pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nXBlock, int nZBlock, int nTexIdx, int nPriority)
{
	CRiver *pRiver = NULL;  // ��N���X�^�̃|�C���^
	if (pRiver == NULL)
	{// ����������ɂȂ��Ă���
		pRiver = new CRiver(nPriority);
		if (pRiver != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pRiver->Init(pos, rot, pTexture, fBoxWidth, fBoxHeight, fBoxDepth, nXBlock, nZBlock, nTexIdx)))
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

	return pRiver;
}


//=============================================================================
//    ����������
//=============================================================================
HRESULT CRiver::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPDIRECT3DTEXTURE9 pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nXBlock, int nZBlock, int nTexIdx)
{
	// �e��l�̐ݒ�
	SetPos(pos);
	SetRot(rot);
	SetTexIdx(nTexIdx);

	// �����蔻��p�����f���𐶐�
	SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);

	// ��Ɏg���|���S���𐶐�
	m_pMeshField = CMeshField::Create(pos, rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MASS_SIZE_X, MASS_SIZE_X, nXBlock, nZBlock, nXBlock, nZBlock, NULL, true);
	m_pMeshField->BindTexture(pTexture);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CRiver::Uninit(void)
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
void CRiver::Update(void)
{
	if (m_pMeshField == NULL) { return; }
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();
	if (pVtxBuff == NULL) { return; }

	// �e�N�X�`��UV���W�𓮂���
	VERTEX_3D *pVtx;
	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nXBlock = m_pMeshField->GetXBlock();
	int nZBlock = m_pMeshField->GetZBlock();
	for (int nCntV = 0; nCntV < nZBlock; nCntV++)
	{// ���������̕����� + 1�����J��Ԃ�
		for (int nCntH = 0; nCntH < nXBlock; nCntH++)
		{// ���������̕����� + 1�����J��Ԃ�
			pVtx[nCntH].tex.x += RIVER_ADDTEX_U;
			pVtx[nCntH].tex.y += RIVER_ADDTEX_V;
		}

		// �|�C���^��i�߂�
		pVtx += nXBlock + 1;
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CRiver::Draw(void)
{
	// ���ʂ̕`�揈��
	CObject3D::Draw();
}

//=============================================================================
//    �����蔻��p�����f�����쐬���鏈��
//=============================================================================
void CRiver::CreateBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	CBoxCollider *pBoxCollider = CBoxCollider::Create(GetPos(), fBoxWidth, fBoxHeight, fBoxDepth, true);
	SetBoxCollider(pBoxCollider);
}

//=============================================================================
//    �g�p���Ă���e�N�X�`���̔ԍ���ݒ肷��
//=============================================================================
void CRiver::SetTexIdx(const int nTexIdx)
{
	m_nTexIdx = nTexIdx;
}

//=============================================================================
//    �g�p���Ă���e�N�X�`���̔ԍ����擾����
//=============================================================================
int CRiver::GetTexIdx(void)
{
	return m_nTexIdx;
}

//=============================================================================
//    ��p�̃��b�V���t�B�[���h�N���X���擾
//=============================================================================
CMeshField *CRiver::GetMeshField(void)
{
	return m_pMeshField;
}