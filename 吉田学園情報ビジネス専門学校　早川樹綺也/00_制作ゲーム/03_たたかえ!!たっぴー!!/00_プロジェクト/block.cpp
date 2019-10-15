//*****************************************************************************
//
//     �u���b�N�̏���[block.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "block.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "boxCollider.h"
#include "splash.h"
#include "game.h"
#include "charaselect.h"
#include "tutorial.h"
#include "bullet.h"
#include "effectManager.h"
#include "map.h"
#include "modelcreate.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define BLOCK_EFFECT_POSUP                (30.0f)  // �G�t�F�N�g���ǂꂭ�炢�������Đ������邩
#define BLOCK_DESTROY_EFFECT_IDX_BLOSSOMS (6)      // �u���b�N�j�󎞂̃G�t�F�N�g�ԍ�(���̏ꍇ)
#define BLOCK_UNINIT_TIMER				  (1200)   // �I������܂ł̃J�E���^�[
#define BLOCK_UNINIT_SIGN				  (1000)   // �I���̓_�ŃT�C��

// �򖗗p
#define BLOCK_SPLASH_PRIORITY             (5)

//*****************************************************************************
//    �ÓI�����o�ϐ�
//*****************************************************************************
int CBlock::m_nNumAll = 0;

//*****************************************************************************
//    CBlock�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBlock::CBlock(int nPriority, OBJTYPE objType) : CObject3D(nPriority, objType)
{
	// �e��l�̃N���A
	m_nType = 0;
	m_pModel = NULL;
	m_fAlpha = 0.0f;
	m_nModelIdx = 0;
	m_nIdx = 0;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBlock::~CBlock()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBlock *CBlock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CBlock *pBlock = NULL;      // �u���b�N�N���X�^�̃|�C���^
	if (pBlock == NULL)
	{// ����������ɂȂ��Ă���
		pBlock = new CBlock(nPriority);
		if (pBlock != NULL)
		{// �C���X�^���X�𐶐��ł���
		    // �e��l�̐ݒ�
			pBlock->SetPos(pos);                                     // ���W
			pBlock->SetRot(rot);                                     // ����
			pBlock->SetType(nType);                                  // ��ޔԍ�
			pBlock->SetModelIdx(nModelIdx);                          // �g�p���郂�f���̔ԍ�
			pBlock->SetAlpha(1.0f);                                  // ���f���̓����x
			pBlock->BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // ���f����񊄂蓖��

			// �����蔻��p�����f�����쐬
			pBlock->SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
			pBlock->CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);

			if (FAILED(pBlock->Init()))
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

	return pBlock;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBlock::Init(void)
{
	// �ԍ��̐ݒ�
	SetIdx(m_nNumAll);

	// �����𑝂₷
	m_nNumAll++;

	// ��ނ̐ݒu����
	SetObjType(OBJTYPE_BLOCK);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CBlock::Uninit(void)
{
	// ���������炷
	m_nNumAll--;

	// ���f���̊J��
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	// ���Ȃ��Ȃ������Ƃ�`����
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		CManager::GetGame()->DeleteBlock(m_nIdx);
	}
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		CManager::GetTutorial()->DeleteBlock(m_nIdx);
	}

	// �G�t�F�N�g���o��
	D3DXVECTOR3 EffectPos = GetPos();
	EffectPos.y += BLOCK_EFFECT_POSUP;
	CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
	if (pEffectManager != NULL)
	{
		switch (CCharaSelect::GetStageType())
		{// �X�e�[�W�̎�ނɂ���ď����킯
		case CGame::MAPTYPE_HINAMATSURI:  // �ЂȍՂ�X�e�[�W
			pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, BLOCK_DESTROY_EFFECT_IDX_BLOSSOMS);
			break;
		}
	}

	// �I�u�W�F�N�g3D�̏I������
	CObject3D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CBlock::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBlock::Draw(void)
{
	//�����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // ���[���h�}�g���b�N�X�ݒ菈��
			SetMtxWorld(pDevice);

			// ���f���̕`��
			if (m_pModel != NULL)
			{
				m_pModel->SetAlpha(m_fAlpha);
				m_pModel->Draw();
				m_pModel->SetAlpha(1.0f);
			}
		}
	}

	// 3D�I�u�W�F�N�g�̕`��
	CObject3D::Draw();
}

//=============================================================================
//    �����蔻��p�����f�����쐬���鏈��
//=============================================================================
void CBlock::CreateBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth, bool bReturnFlag)
{
	CBoxCollider *pBoxCollider = CBoxCollider::Create(GetPos(), fBoxWidth, fBoxHeight, fBoxDepth, bReturnFlag);
	SetBoxCollider(pBoxCollider);
}


//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CBlock::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���݂̓����蔻��p�����f�����J������
	CBoxCollider *pBoxCollider = GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		pBoxCollider->Uninit();
		delete pBoxCollider;
		pBoxCollider = NULL;
	}

	// �����蔻��p�����f���𐶐����Ȃ���
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);
}

//=============================================================================
//    �I�u�W�F�N�g�ɓ��������Ƃ��̏���
//=============================================================================
void CBlock::Hit(CScene *pScene)
{
	Uninit();
}

//=============================================================================
//    ���f�����ݒ菈��
//=============================================================================
void CBlock::BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	m_pModel = CModel::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, pMesh, pBuffMat, nNumMat, pTexture);
}

//=============================================================================
//    ��ޔԍ��ݒ菈��
//=============================================================================
void CBlock::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    �g�p���郂�f���̔ԍ��ݒ菈��
//=============================================================================
void CBlock::SetModelIdx(const int nModelIdx)
{
	m_nModelIdx = nModelIdx;
}

//=============================================================================
//    ���f���̓����x�ݒ菈��
//=============================================================================
void CBlock::SetAlpha(const float fAlpha)
{
	m_fAlpha = fAlpha;
}

//=============================================================================
//    �ԍ��̐ݒ菈��
//=============================================================================
void CBlock::SetIdx(const int nIdx)
{
	m_nIdx = nIdx;
}

//=============================================================================
//    ��ޔԍ��擾����
//=============================================================================
int CBlock::GetType(void)
{
	return m_nType;
}

//=============================================================================
//    �g�p���郂�f���̔ԍ��擾����
//=============================================================================
int CBlock::GetModelIdx(void)
{
	return m_nModelIdx;
}

//=============================================================================
//    ���f���̓����x�擾����
//=============================================================================
float CBlock::GetAlpha(void)
{
	return m_fAlpha;
}

//=============================================================================
//    �ԍ��̎擾����
//=============================================================================
int CBlock::GetIdx(void)
{
	return m_nIdx;
}

//=============================================================================
//    �����̎擾����
//=============================================================================
int CBlock::GetNumAll(void)
{
	return m_nNumAll;
}

//*****************************************************************************
//    CBlockType0�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBlockType0::CBlockType0(int nPriority, OBJTYPE objType) : CBlock(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBlockType0::~CBlockType0()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBlockType0 *CBlockType0::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CBlockType0 *pBlockType0 = NULL;      // �^�C�v0�̃u���b�N�N���X�^�̃|�C���^
	if (pBlockType0 == NULL)
	{// ����������ɂȂ��Ă���
		pBlockType0 = new CBlockType0(nPriority);
		if (pBlockType0 != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pBlockType0->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pBlockType0;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBlockType0::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���ʂ̏���������
	if (FAILED(CBlock::Init()))
	{
		return E_FAIL;
	}

	// �e��l�̐ݒ�
	SetPos(pos);                                     // ���W
	SetRot(rot);                                     // ����
	SetType(nType);                                  // ��ޔԍ�
	SetModelIdx(nModelIdx);                          // �g�p���郂�f���̔ԍ�
	SetAlpha(1.0f);                                  // ���f���̓����x
	BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // ���f����񊄂蓖��

	// �����蔻��p�����f�����쐬
	SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth, true);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CBlockType0::Uninit(void)
{
	// ���ʂ̏I������
	CBlock::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CBlockType0::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBlockType0::Draw(void)
{
	// ���ʂ̕`�揈��
	CBlock::Draw();
}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CBlockType0::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���݂̓����蔻��p�����f�����J������
	CBoxCollider *pBoxCollider = GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		pBoxCollider->Uninit();
		delete pBoxCollider;
		pBoxCollider = NULL;
	}

	// �����蔻��p�����f���𐶐����Ȃ���
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);
}

//=============================================================================
//    ���������Ƃ��̏���
//=============================================================================
void CBlockType0::Hit(CScene *pScene)
{
	// �������Ă����I�u�W�F�N�g�̎�ނŏ����킯
	if (pScene->GetObjType() == OBJTYPE_BULLET)
	{// �e������
		Uninit();
	}
}

//*****************************************************************************
//    CBlockType1�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBlockType1::CBlockType1(int nPriority, OBJTYPE objType) : CBlock(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBlockType1::~CBlockType1()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBlockType1 *CBlockType1::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CBlockType1 *pBlockType1 = NULL;      // �^�C�v1�̃u���b�N�N���X�^�̃|�C���^
	if (pBlockType1 == NULL)
	{// ����������ɂȂ��Ă���
		pBlockType1 = new CBlockType1(nPriority);
		if (pBlockType1 != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pBlockType1->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pBlockType1;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBlockType1::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���ʂ̏���������
	if (FAILED(CBlock::Init()))
	{
		return E_FAIL;
	}

	// �e��l�̐ݒ�
	SetPos(pos);                                     // ���W
	SetRot(rot);                                     // ����
	SetType(nType);                                  // ��ޔԍ�
	SetModelIdx(nModelIdx);                          // �g�p���郂�f���̔ԍ�
	SetAlpha(1.0f);                                  // ���f���̓����x
	BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // ���f����񊄂蓖��

	// �����蔻��p�����f�����쐬
	SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth, false);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CBlockType1::Uninit(void)
{
	// ���ʂ̏I������
	CBlock::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CBlockType1::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBlockType1::Draw(void)
{
	// ���ʂ̕`�揈��
	CBlock::Draw();
}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CBlockType1::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���݂̓����蔻��p�����f�����J������
	CBoxCollider *pBoxCollider = GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		pBoxCollider->Uninit();
		delete pBoxCollider;
		pBoxCollider = NULL;
	}

	// �����蔻��p�����f���𐶐����Ȃ���
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth, false);
}

//=============================================================================
//    ���������Ƃ��̏���
//=============================================================================
void CBlockType1::Hit(CScene *pScene)
{
	// �������Ă����I�u�W�F�N�g�̎�ނŏ����킯
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{// �v���C���[������
		// �򖗃|���S������
		CreateSplash();

		CPlayer *pPlayer = (CPlayer*)pScene;

		//�N���[���̉��Z
		CManager::GetGame()->AddCream(pPlayer->GetPlayerIdx());

		// �I������
		Uninit();
	}
	else if (pScene->GetObjType() == OBJTYPE_ENEMY)
	{// �G������
		// �GAI�p�^�[���ω�

		// �I������
		Uninit();
	}
}

//=============================================================================
//    �򖗃|���S���𐶐����鏈��
//=============================================================================
void CBlockType1::CreateSplash(void)
{
	// �N���[���ɂ��o��
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂ�������
		CGame *pGame = CManager::GetGame();
		if (pGame == NULL) { return; }
		pGame->CreateMist();
	}

	// �򖗃|���S���o��
	CSplash::Create(D3DXVECTOR3(30.0f + (float)(rand() % 350), 35.0f + (float)(rand() % 200), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), 10.0f, 10.0f, 0,
		D3DXVECTOR3(0.1f, -1.0f, 0.0f), 0.0f, 0.005f, 125.0f, -1.0f, 360, BLOCK_SPLASH_PRIORITY);
	CSplash::Create(D3DXVECTOR3(45.0f + (float)(rand() % 350), 495.0f + (float)(rand() % 200), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), 10.0f, 10.0f, 0,
		D3DXVECTOR3(0.1f, -1.0f, 0.0f), 0.0f, 0.005f, 115.0f, -1.0f, 360, BLOCK_SPLASH_PRIORITY);
	CSplash::Create(D3DXVECTOR3(690.0f + (float)(rand() % 350), 45.0f + (float)(rand() % 200), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), 10.0f, 10.0f, 0,
		D3DXVECTOR3(0.1f, -1.0f, 0.0f), 0.0f, 0.005f, 140.0f, -1.0f, 360, BLOCK_SPLASH_PRIORITY);
	CSplash::Create(D3DXVECTOR3(810.0f + (float)(rand() % 350), 505.0f + (float)(rand() % 200), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), 10.0f, 10.0f, 0,
		D3DXVECTOR3(0.1f, -1.0f, 0.0f), 0.0f, 0.005f, 105.0f, -1.0f, 360, BLOCK_SPLASH_PRIORITY);
}

//*****************************************************************************
//    CBlockType2�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBlockType2::CBlockType2(int nPriority, OBJTYPE objType) : CBlock(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBlockType2::~CBlockType2()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBlockType2 *CBlockType2::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CBlockType2 *pBlockType2 = NULL;      // �^�C�v2�̃u���b�N�N���X�^�̃|�C���^
	if (pBlockType2 == NULL)
	{// ����������ɂȂ��Ă���
		pBlockType2 = new CBlockType2(nPriority);
		if (pBlockType2 != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pBlockType2->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pBlockType2;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBlockType2::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���ʂ̏���������
	if (FAILED(CBlock::Init()))
	{
		return E_FAIL;
	}

	// �e��l�̐ݒ�
	SetPos(pos);                                     // ���W
	SetRot(rot);                                     // ����
	SetType(nType);                                  // ��ޔԍ�
	SetModelIdx(nModelIdx);                          // �g�p���郂�f���̔ԍ�
	SetAlpha(1.0f);                                  // ���f���̓����x
	BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // ���f����񊄂蓖��

	// �����蔻��p�����f�����쐬
	SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CBlockType2::Uninit(void)
{
	// ���ʂ̏I������
	CBlock::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CBlockType2::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBlockType2::Draw(void)
{
	// ���ʂ̕`�揈��
	CBlock::Draw();
}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CBlockType2::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���݂̓����蔻��p�����f�����J������
	CBoxCollider *pBoxCollider = GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		pBoxCollider->Uninit();
		delete pBoxCollider;
		pBoxCollider = NULL;
	}

	// �����蔻��p�����f���𐶐����Ȃ���
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);
}

//=============================================================================
//    ���������Ƃ��̏���
//=============================================================================
void CBlockType2::Hit(CScene *pScene)
{
	// �������Ă����I�u�W�F�N�g�̎�ނŏ����킯
	if (pScene->GetObjType() == OBJTYPE_BULLET)
	{// �e������
		CBullet *pBullet = (CBullet*)pScene;
		if (pBullet->GetType() == CBullet::TYPE_ENEMY) { return; }

		// �v���C���[�̒e�̏ꍇ�ő勭�����Ȃ�Ή���悤�ɂ���
		CPlayer *pPlayer = (CPlayer*)pBullet->GetParent();
		if (pPlayer == NULL || pPlayer->GetObjType() != OBJTYPE_PLAYER) { return; }
		if (pPlayer->GetAllBlockDestroy() == true)
		{
			Uninit();
		}
	}
}

//*****************************************************************************
//    CBlockType3�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBlockType3::CBlockType3(int nPriority, OBJTYPE objType) : CBlock(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBlockType3::~CBlockType3()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBlockType3 *CBlockType3::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CBlockType3 *pBlockType3 = NULL;      // �^�C�v3�̃u���b�N�N���X�^�̃|�C���^
	if (pBlockType3 == NULL)
	{// ����������ɂȂ��Ă���
		pBlockType3 = new CBlockType3(nPriority);
		if (pBlockType3 != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pBlockType3->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pBlockType3;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBlockType3::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���ʂ̏���������
	if (FAILED(CBlock::Init()))
	{
		return E_FAIL;
	}

	// �e��l�̐ݒ�
	SetPos(pos);                                     // ���W
	SetRot(rot);                                     // ����
	SetType(nType);                                  // ��ޔԍ�
	SetModelIdx(nModelIdx);                          // �g�p���郂�f���̔ԍ�
	SetAlpha(1.0f);                                  // ���f���̓����x
	BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // ���f����񊄂蓖��

	// �����蔻��p�����f�����쐬
	SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CBlockType3::Uninit(void)
{
	// ���ʂ̏I������
	CBlock::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CBlockType3::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBlockType3::Draw(void)
{
	// ���ʂ̕`�揈��
	CBlock::Draw();
}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CBlockType3::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���݂̓����蔻��p�����f�����J������
	CBoxCollider *pBoxCollider = GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		pBoxCollider->Uninit();
		delete pBoxCollider;
		pBoxCollider = NULL;
	}

	// �����蔻��p�����f���𐶐����Ȃ���
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);
}

//=============================================================================
//    ���������Ƃ��̏���
//=============================================================================
void CBlockType3::Hit(CScene *pScene)
{
	// �������Ă����I�u�W�F�N�g�̎�ނŏ����킯
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{// �v���C���[������
		// �򖗃|���S������
	}
	else if (pScene->GetObjType() == OBJTYPE_ENEMY)
	{// �G������
		// �GAI�p�^�[���ω�
	}
	else if (pScene->GetObjType() == OBJTYPE_BULLET)
	{// �e������
		CBullet *pBullet = (CBullet*)pScene;
		if (pBullet->GetType() == CBullet::TYPE_ENEMY) { return; }

		// �v���C���[�̒e�̏ꍇ�ő勭�����Ȃ�Ή���悤�ɂ���
		CPlayer *pPlayer = (CPlayer*)pBullet->GetParent();
		if (pPlayer == NULL || pPlayer->GetObjType() != OBJTYPE_PLAYER) { return;}
		if (pPlayer->GetAllBlockDestroy() == true)
		{
			Uninit();
		}
	}
}

//*****************************************************************************
//    CBlockScoop�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBlockScoop::CBlockScoop(int nPriority, OBJTYPE objType) : CBlock(nPriority, objType)
{
	m_nUninitTimer = 0;		//�I������܂ł̃J�E���^�[
	m_bUninitSign = false;	//�I���̓_�ŃT�C��
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBlockScoop::~CBlockScoop()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBlockScoop *CBlockScoop::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CBlockScoop *pBlockScoop = NULL;      // �^�C�v2�̃u���b�N�N���X�^�̃|�C���^
	if (pBlockScoop == NULL)
	{// ����������ɂȂ��Ă���
		pBlockScoop = new CBlockScoop(nPriority);
		if (pBlockScoop != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pBlockScoop->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pBlockScoop;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBlockScoop::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���ʂ̏���������
	if (FAILED(CBlock::Init()))
	{
		return E_FAIL;
	}

	// �e��l�̐ݒ�
	SetPos(pos);                                     // ���W
	SetRot(rot);                                     // ����
	SetType(nType);                                  // ��ޔԍ�
	SetModelIdx(nModelIdx);                          // �g�p���郂�f���̔ԍ�
	SetAlpha(1.0f);                                  // ���f���̓����x
	BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // ���f����񊄂蓖��

													 // �����蔻��p�����f�����쐬
	SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);

	m_nUninitTimer = 0;								 // �I������܂ł̃J�E���^�[
	m_bUninitSign = false;							 // �I���̓_�ŃT�C��
	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CBlockScoop::Uninit(void)
{
	// ���ʂ̏I������
	CBlock::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CBlockScoop::Update(void)
{
	//�I������܂ł̃J�E���^�[�̉��Z
	m_nUninitTimer++;

	if (m_nUninitTimer == BLOCK_UNINIT_SIGN)
	{
		//�u���b�N�̐���
		CreateBlock();
	}
	if (m_nUninitTimer >= BLOCK_UNINIT_SIGN)
	{
		m_bUninitSign = true;
	}
	if (m_nUninitTimer >= BLOCK_UNINIT_TIMER)
	{//�I������܂ł̃J�E���^�[����萔�𒴂����ꍇ
		//�I������
		Uninit();
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBlockScoop::Draw(void)
{
	if (m_bUninitSign == false)
	{//�I���̓_�ŃT�C����false�̏ꍇ
	 // ���ʂ̕`�揈��
		CBlock::Draw();
	}
	else if (m_bUninitSign == true)
	{//�I���̓_�ŃT�C����false�̏ꍇ
		if (m_nUninitTimer % 4 != 0)
		{
			// ���ʂ̕`�揈��
			CBlock::Draw();
		}
	}

}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CBlockScoop::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���݂̓����蔻��p�����f�����J������
	CBoxCollider *pBoxCollider = GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		pBoxCollider->Uninit();
		delete pBoxCollider;
		pBoxCollider = NULL;
	}

	// �����蔻��p�����f���𐶐����Ȃ���
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);
}

//=============================================================================
//    ���������Ƃ��̏���
//=============================================================================
void CBlockScoop::Hit(CScene *pScene)
{
	// �������Ă����I�u�W�F�N�g�̎�ނŏ����킯
	if (pScene->GetObjType() == OBJTYPE_BULLET)
	{// �e������
		CBullet *pBullet = (CBullet*)pScene;
		if (pBullet->GetType() == CBullet::TYPE_ENEMY) { return; }

		// �v���C���[�̒e�̏ꍇ�ő勭�����Ȃ�Ή���悤�ɂ���
		CPlayer *pPlayer = (CPlayer*)pBullet->GetParent();
		if (pPlayer == NULL || pPlayer->GetObjType() != OBJTYPE_PLAYER) { return; }
		if (pPlayer->GetAllBlockDestroy() == true)
		{
			Uninit();
		}
	}
}

//=============================================================================
//    �u���b�N�̐�������
//=============================================================================
void CBlockScoop::CreateBlock(void)
{
	//�}�b�v�̎擾
	CMap *pMap = CManager::GetBaseMode()->GetMap();

	//�Q�[���̎擾
	CGame *pGame = CManager::GetGame();

	D3DXVECTOR3 BlockPos = GetPos();
	if (pGame->GetStageIdx() == 0)
	{//�X�e�[�W0�̏ꍇ
		CBlockType0::Create(D3DXVECTOR3(BlockPos.x, 0.0f, BlockPos.z), INITIALIZE_D3DXVECTOR3,
			0, 0, pMap->GetModelCreate()->GetMesh(7), pMap->GetModelCreate()->GetBuffMat(7), pMap->GetModelCreate()->GetNumMat(7), pMap->GetModelCreate()->GetTexture(7)
			, MASS_SIZE_X_HALF, MASS_SIZE_Z_HALF, MASS_SIZE_Z_HALF);
	}
	else if (pGame->GetStageIdx() == 1 ||
		pGame->GetStageIdx() == 2)
	{//�X�e�[�W1�`2�̏ꍇ
		CBlockType0::Create(D3DXVECTOR3(BlockPos.x, 0.0f, BlockPos.z), INITIALIZE_D3DXVECTOR3,
			0, 0, pMap->GetModelCreate()->GetMesh(6), pMap->GetModelCreate()->GetBuffMat(6), pMap->GetModelCreate()->GetNumMat(6), pMap->GetModelCreate()->GetTexture(6)
			, MASS_SIZE_X_HALF, MASS_SIZE_Z_HALF, MASS_SIZE_Z_HALF);
	}
}