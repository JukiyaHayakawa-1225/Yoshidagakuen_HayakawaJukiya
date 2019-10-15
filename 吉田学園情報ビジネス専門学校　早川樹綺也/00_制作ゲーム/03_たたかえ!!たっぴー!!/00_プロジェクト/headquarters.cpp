//*****************************************************************************
//
//     �i�ߕ��̏���[headquarters.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "headquarters.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "boxCollider.h"
#include "game.h"
#include "debugproc.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define HEADQUARTERS_COL_HEIGHT (75.0f)   // �����蔻��̍���

//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CHeadQuarters::CHeadQuarters(int nPriority, OBJTYPE objType) : CObject3D(nPriority, objType)
{
	// �e��l�̃N���A
	m_pModel = NULL;   // ���f���N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CHeadQuarters::~CHeadQuarters()
{

}

//=============================================================================
//    ��������
//=============================================================================
CHeadQuarters *CHeadQuarters::Create(int nAreaX, int nAreaZ, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, int nPriority)
{
	CHeadQuarters *pHeadQuarters = NULL;  // �i�ߕ��N���X�^�̃|�C���^
	if (pHeadQuarters == NULL)
	{// ����������ɂȂ��Ă���
		pHeadQuarters = new CHeadQuarters(nPriority);
		if (pHeadQuarters != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pHeadQuarters->Init(nAreaX, nAreaZ, pMesh, pBuffMat, nNumMat, pTexture)))
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

	return pHeadQuarters;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CHeadQuarters::Init(int nAreaX, int nAreaZ, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture)
{
	// ������G���A������W���v�Z
	SetArea(nAreaX, nAreaZ);

	// ���f���N���X�̍쐬
	m_pModel = CModel::Create(INITIALIZE_D3DXVECTOR3, GetRot(), pMesh, pBuffMat, nNumMat, pTexture);

	// �����蔻��p�����f���̍쐬
	CBoxCollider *pBoxCollider = CBoxCollider::Create(GetPos(), MASS_SIZE_X, HEADQUARTERS_COL_HEIGHT, MASS_SIZE_X, true);
	SetBoxCollider(pBoxCollider);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CHeadQuarters::Uninit(void)
{
	// ���f���̔j��
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	// �I�u�W�F�N�g3D�̏I������
	CObject3D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CHeadQuarters::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CHeadQuarters::Draw(void)
{
	// �����_�����O�N���X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();  	// �f�o�C�X�̎擾
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // ���[���h�}�g���b�N�X�ݒ菈��
			SetMtxWorld(pDevice);

			// ���f���̕`��
			if (m_pModel != NULL)
			{
				m_pModel->Draw();
			}

			// �I�u�W�F�N�g3D�̕`�揈��
			CObject3D::Draw();
		}
	}
}

//=============================================================================
//    �����������̏���
//=============================================================================
void CHeadQuarters::Hit(CScene *pScene)
{
	// �e�ł͂Ȃ������珈�����Ȃ�
	if (pScene->GetObjType() != OBJTYPE_BULLET) return;

	// ���ꂽ��Ԃɂ���


	// �Q�[���I�[�o�[��Ԃɂ�����
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		CGame *pGame = CManager::GetGame();
		if (pGame == NULL) { return; }
		pGame->SetState(CGame::STATE_GAMEOVER);
	}
}

//=============================================================================
//    ���f���N���X�^�̃|�C���^�ݒ菈��
//=============================================================================
void CHeadQuarters::SetModel(CModel *pModel)
{
	m_pModel = pModel;
}

//=============================================================================
//    ���ǂ̃G���A�ɂ��邩��ݒ�
//=============================================================================
void CHeadQuarters::SetArea(int nAreaX, int nAreaZ)
{
	// �G���A��ݒ�
	m_nAreaX = nAreaX;
	m_nAreaZ = nAreaZ;

	// �G���A������W�����߂�
	D3DXVECTOR3 pos = INITIALIZE_D3DXVECTOR3;
	pos.x = -((MASS_SIZE_X * MASS_BLOCK_X) / 2) + (MASS_SIZE_X * m_nAreaX) - MASS_SIZE_X_HALF;
	pos.z = ((MASS_SIZE_X * MASS_BLOCK_Z) / 2) - (MASS_SIZE_X * m_nAreaZ) + MASS_SIZE_X_HALF;
	SetPos(pos);

	// �����蔻��p�����f�������炷
	if (GetBoxCollider() != NULL)
	{
		GetBoxCollider()->SetPos(pos);
	}
}

//=============================================================================
//    ���f���N���X�^�̃|�C���^�擾����
//=============================================================================
CModel *CHeadQuarters::GetModel(void)
{
	return m_pModel;
}

//=============================================================================
//    ���ǂ̃G���A(��)�ɂ��邩���擾
//=============================================================================
int CHeadQuarters::GetAreaX(void)
{
	return m_nAreaX;
}

//=============================================================================
//    ���ǂ̃G���A(���s)�ɂ��邩���擾
//=============================================================================
int CHeadQuarters::GetAreaZ(void)
{
	return m_nAreaZ;
}