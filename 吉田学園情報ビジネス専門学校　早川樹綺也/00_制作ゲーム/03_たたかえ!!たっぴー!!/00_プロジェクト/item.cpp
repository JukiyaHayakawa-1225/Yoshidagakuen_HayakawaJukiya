//*****************************************************************************
//
//     �A�C�e���̏���[item.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "item.h"
#include "manager.h"
#include "boxCollider.h"
#include "renderer.h"
#include "basemode.h"
#include "game.h"
#include "title.h"
#include "server.h"
#include "player.h"
#include "effectManager.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define ITEM_EFFECT_IDX     (8)        // �A�C�e�����g���G�t�F�N�g�̔ԍ�
#define ITEM_SCALE_MAX      (1.1f)     // �傫���̍ő�l
#define ITEM_SCALE_MIN      (0.85f)    // �傫���̍ŏ��l
#define ITEM_SCALE_CHANGE   (0.005f)   // �傫���̕ω��ʏ����l
#define ITEM_LIFE_INI       (720)      // �A�C�e���̎��������l
#define ITEM_LIFE_DEATH     (480)      // �A�C�e���̎���(���S��)
#define ITEM_NOTDISP_TIMING (4)        // ���S���ɃA�C�e����`�悵�Ȃ��^�C�~���O

// �O���l�[�h�p
#define GRANADE_EFFECT_IDX  (9)        // �O���l�[�h�������Ƃ��Ɏg���G�t�F�N�g�̔ԍ�

//*****************************************************************************
//    �ÓI�����o�ϐ�
//*****************************************************************************
int CItem::m_nNumAll = 0;             // �A�C�e���̑���

//*****************************************************************************
//    CBlock�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CItem::CItem(int nPriority, OBJTYPE objType) : CObject3D(nPriority, objType)
{
	// �e��l�̃N���A
	m_pMesh = NULL;                      // ���b�V���ւ̃|�C���^
	m_pBuffMat = NULL;                   // �}�e���A�����ւ̃|�C���^
	m_nNumMat = 0;                       // �}�e���A�����̐�
	m_VtxMax = INITIALIZE_D3DXVECTOR3;   // �ő�̒��_���W
	m_VtxMin = INITIALIZE_D3DXVECTOR3;   // �ŏ��̒��_���W
	m_fAlpha = 0.0f;                     // ���f���̓����x
	m_Scale = INITIALIZE_D3DXVECTOR3;    // ���݂̃X�P�[���̔{��
	m_fChangeScale = 0.0f;               // �X�P�[���{����ύX������l
	m_nLife = 0;                         // ����
	m_State = STATE_NORMAL;              // ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CItem::~CItem()
{

}

//=============================================================================
//    ��������
//=============================================================================
CItem *CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItem *pItem = NULL;      // �A�C�e���N���X�^�̃|�C���^
	if (pItem == NULL)
	{// ����������ɂȂ��Ă���
		pItem = new CItem(nPriority);
		if (pItem != NULL)
		{// �C���X�^���X�𐶐��ł���
		    // �e��l�̐ݒ�
			pItem->SetPos(pos);                                     // ���W
			pItem->SetRot(rot);                                     // ����
			pItem->SetType(nType);                                  // ��ޔԍ�
			pItem->SetModelIdx(nModelIdx);                          // �g�p���郂�f���̔ԍ�
			pItem->SetAlpha(1.0f);                                  // ���f���̓����x
			pItem->BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // ���f����񊄂蓖��

			// �����蔻��p�����f�����쐬
			pItem->SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
			pItem->CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);

			if (FAILED(pItem->Init()))
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

	return pItem;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CItem::Init(void)
{
	// �ԍ���ݒ�
	SetIdx(m_nNumAll);

	// �����𑝂₷
	m_nNumAll++;

	// �X�P�[���̔{���ύX�̒l��ݒ�
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_fChangeScale = ITEM_SCALE_CHANGE;

	// ������ݒ�
	m_nLife = ITEM_LIFE_INI;

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CItem::Uninit(void)
{
	// ���������炷
	m_nNumAll--;

	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// �I�����C��2�l�v���C�Ȃ��
		OnlineUninit();
	}

	// �I�u�W�F�N�g3D�̏I������
	CObject3D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CItem::Update(void)
{
	// �傫���̔{����ς���
	m_Scale.x += m_fChangeScale;
	m_Scale.y += m_fChangeScale;
	m_Scale.z += m_fChangeScale;

	// �傫���`�F�b�N
	if (m_Scale.x <= ITEM_SCALE_MIN)
	{
		m_Scale.x = ITEM_SCALE_MIN;
		m_Scale.y = ITEM_SCALE_MIN;
		m_Scale.z = ITEM_SCALE_MIN;
		m_fChangeScale *= -1;
	}
	if (m_Scale.x >= ITEM_SCALE_MAX)
	{
		m_Scale.x = ITEM_SCALE_MAX;
		m_Scale.y = ITEM_SCALE_MAX;
		m_Scale.z = ITEM_SCALE_MAX;
		m_fChangeScale *= -1;
	}

	// �G�t�F�N�g���o��
	CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
	if (pEffectManager != NULL)
	{
		pEffectManager->SetEffect(GetPos() + D3DXVECTOR3(0.0f, 10.0f, 0.0f), INITIALIZE_D3DXVECTOR3, ITEM_EFFECT_IDX);
	}

	// ���������炷
	m_nLife--;
	if (m_nLife <= 0)
	{
		switch (m_State)
		{
		case STATE_NORMAL:
			m_nLife = ITEM_LIFE_DEATH;
			m_State = STATE_DEATH;
			break;
		case STATE_DEATH:
			Uninit();
			break;
		}
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CItem::Draw(void)
{
	if (m_State == STATE_DEATH && m_nLife % ITEM_NOTDISP_TIMING == 0)
	{// ���S��Ԃŕ`�悵�Ȃ��^�C�~���O��������
		return;
	}

	D3DMATERIAL9 matDef;     // ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;      // �}�e���A���f�[�^�ւ̃|�C���^
	float fAlphaDef = 0.0f;  // �f�t�H���g�̓����x

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

			// �@�����𐳋K������(�X�P�[�����O���ɖ@����񂪂�������̂�)
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			if (m_pBuffMat != NULL && m_pMesh != NULL)
			{// X�t�@�C�����烂�f���f�[�^���ǂݍ��߂Ă���
			 // �}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
				{// �ݒ肳��Ă����}�e���A���̐������J��Ԃ�
				 // �����x�̐ݒ�
					fAlphaDef = pMat[nCntMat].MatD3D.Diffuse.a;
					pMat[nCntMat].MatD3D.Diffuse.a = m_fAlpha;

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					if (m_pTexture != NULL)
					{// �e�N�X�`����񂪈����o���Ă���
					 // �e�N�X�`���̐ݒ�
						pDevice->SetTexture(0, m_pTexture[nCntMat]);
					}
					else
					{// �e�N�X�`����񂪈����o���Ă��Ȃ�
					 // �e�N�X�`���̐ݒ�
						pDevice->SetTexture(0, NULL);
					}

					// ���f��(�p�[�c)�̕`��
					m_pMesh->DrawSubset(nCntMat);

					// �����x��߂�
					pMat[nCntMat].MatD3D.Diffuse.a = fAlphaDef;
				}
			}

			// �ۑ����Ă����}�e���A���ɖ߂�
			pDevice->SetMaterial(&matDef);

			// �@���𐳋K�����Ȃ�
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);
		}
	}

	// 3D�I�u�W�F�N�g�̕`��
	CObject3D::Draw();
}

//=============================================================================
// ���[���h�}�g���b�N�X�̌v�Z����
//=============================================================================
void CItem::SetMtxWorld(LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxWorld, mtxRot; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// ��]�s����쐬(D3DXMatrixRotationYawPitchRoll�Q��)
	D3DXVECTOR3 rot = GetRot();
	float fSinPitch = sinf(rot.x);
	float fCosPitch = cosf(rot.x);
	float fSinYaw = sinf(rot.y);
	float fCosYaw = cosf(rot.y);
	float fSinRoll = sinf(rot.z);
	float fCosRoll = cosf(rot.z);
	mtxRot._11 = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
	mtxRot._12 = fSinRoll * fCosPitch;
	mtxRot._13 = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
	mtxRot._21 = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
	mtxRot._22 = fCosRoll * fCosPitch;
	mtxRot._23 = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
	mtxRot._31 = fCosPitch * fSinYaw;
	mtxRot._32 = -fSinPitch;
	mtxRot._33 = fCosPitch * fCosYaw;

	// �傫���Ɖ�]�𔽉f����
	mtxWorld._11 = mtxRot._11 * m_Scale.x;
	mtxWorld._12 = mtxRot._12 * m_Scale.x;
	mtxWorld._13 = mtxRot._13 * m_Scale.x;
	mtxWorld._21 = mtxRot._21 * m_Scale.y;
	mtxWorld._22 = mtxRot._22 * m_Scale.y;
	mtxWorld._23 = mtxRot._23 * m_Scale.y;
	mtxWorld._31 = mtxRot._31 * m_Scale.z;
	mtxWorld._32 = mtxRot._32 * m_Scale.z;
	mtxWorld._33 = mtxRot._33 * m_Scale.z;

	// �I�t�Z�b�g�ʒu�𔽉f
	D3DXVECTOR3 pos = GetPos();
	mtxWorld._41 = pos.x;
	mtxWorld._42 = pos.y;
	mtxWorld._43 = pos.z;

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	CObject3D::SetMtxWorld(mtxWorld);
}

//=============================================================================
//    �����蔻��p�����f�����쐬���鏈��
//=============================================================================
void CItem::CreateBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth, bool bReturnFlag)
{
	CBoxCollider *pBoxCollider = CBoxCollider::Create(GetPos(), fBoxWidth, fBoxHeight, fBoxDepth, bReturnFlag);
	SetBoxCollider(pBoxCollider);
}


//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CItem::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItem::Hit(CScene *pScene)
{
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{
		Uninit();
	}
}

//=============================================================================
//    ���f�����ݒ菈��
//=============================================================================
void CItem::BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	m_pMesh = pMesh;
	m_pBuffMat = pBuffMat;
	m_nNumMat = nNumMat;
	m_pTexture = pTexture;
	m_VtxMax = VtxMax;
	m_VtxMin = VtxMin;
}

//=============================================================================
//    �I�����C���p�̏I������
//=============================================================================
void CItem::OnlineUninit(void)
{
	int nIdxClient = 0;
	CClient *pClient = CManager::GetClient();
	if (pClient == NULL) { return; }

	// �������z�X�g�łȂ���Ώ������s
	nIdxClient = pClient->GetClientId();
	if (nIdxClient == 0) { return; }

	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂ�������
		CGame *pGame = CManager::GetGame();
		if (pGame == NULL)
		{
			pGame->DeleteItem(m_nIdx);
		}
	}
}

//=============================================================================
//    ��ޔԍ��ݒ菈��
//=============================================================================
void CItem::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    �g�p���郂�f���̔ԍ��ݒ菈��
//=============================================================================
void CItem::SetModelIdx(const int nModelIdx)
{
	m_nModelIdx = nModelIdx;
}

//=============================================================================
//    ���b�V���ւ̃|�C���^�ݒ菈��
//=============================================================================
void CItem::SetMesh(const LPD3DXMESH pMesh)
{
	m_pMesh = pMesh;
}

//=============================================================================
//    �}�e���A�����ւ̃|�C���^�ݒ菈��
//=============================================================================
void CItem::SetBuffMat(const LPD3DXBUFFER pBuffMat)
{
	m_pBuffMat = pBuffMat;
}

//=============================================================================
//    �}�e���A�����̐��ݒ菈��
//=============================================================================
void CItem::SetNumMat(const DWORD nNumMat)
{
	m_nNumMat = nNumMat;
}

//=============================================================================
//    �e�N�X�`���ւ̃|�C���^�ݒ菈��
//=============================================================================
void CItem::SetTexture(LPDIRECT3DTEXTURE9 *pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    �ő�̒��_���W�ݒ菈��
//=============================================================================
void CItem::SetVtxMax(const D3DXVECTOR3 VtxMax)
{
	m_VtxMax = VtxMax;
}

//=============================================================================
//    �ŏ��̒��_���W�ݒ菈��
//=============================================================================
void CItem::SetVtxMin(const D3DXVECTOR3 VtxMin)
{
	m_VtxMin = VtxMin;
}

//=============================================================================
//    ���f���̓����x�ݒ菈��
//=============================================================================
void CItem::SetAlpha(const float fAlpha)
{
	m_fAlpha = fAlpha;
}

//=============================================================================
//    �ԍ��ݒ菈��
//=============================================================================
void CItem::SetIdx(const int nIdx)
{
	m_nIdx = nIdx;
}


//=============================================================================
//    ��ޔԍ��擾����
//=============================================================================
int CItem::GetType(void)
{
	return m_nType;
}

//=============================================================================
//    �g�p���郂�f���̔ԍ��擾����
//=============================================================================
int CItem::GetModelIdx(void)
{
	return m_nModelIdx;
}

//=============================================================================
//    ���b�V���ւ̃|�C���^�擾����
//=============================================================================
LPD3DXMESH CItem::GetMesh(void)
{
	return m_pMesh;
}

//=============================================================================
//    �}�e���A�����ւ̃|�C���^�擾����
//=============================================================================
LPD3DXBUFFER CItem::GetBuffMat(void)
{
	return m_pBuffMat;
}

//=============================================================================
//    �}�e���A�����̐��擾����
//=============================================================================
DWORD CItem::GetNumMat(void)
{
	return m_nNumMat;
}

//=============================================================================
//    �e�N�X�`���ւ̃|�C���^�擾����
//=============================================================================
LPDIRECT3DTEXTURE9 *CItem::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    �ő�̒��_���W�擾����
//=============================================================================
D3DXVECTOR3 CItem::GetVtxMax(void)
{
	return m_VtxMax;
}

//=============================================================================
//    �ŏ��̒��_���W�擾����
//=============================================================================
D3DXVECTOR3 CItem::GetVtxMin(void)
{
	return m_VtxMin;
}

//=============================================================================
//    ���f���̓����x�擾����
//=============================================================================
float CItem::GetAlpha(void)
{
	return m_fAlpha;
}

//=============================================================================
//    �ԍ��擾����
//=============================================================================
int CItem::GetIdx(void)
{
	return m_nIdx;
}

//=============================================================================
//    ���������Z�b�g����
//=============================================================================
void CItem::ResetNumAll(void)
{
	m_nNumAll = 0;
}

//=============================================================================
//    �������擾����
//=============================================================================
int CItem::GetNumAll(void)
{
	return m_nNumAll;
}


//*****************************************************************************
//    CItemStar�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CItemStar::CItemStar(int nPriority, OBJTYPE objType) : CItem(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CItemStar::~CItemStar()
{

}

//=============================================================================
//    ��������
//=============================================================================
CItemStar *CItemStar::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItemStar *pItemStar = NULL;      // �A�C�e��(�X�^�[)�N���X�^�̃|�C���^
	if (pItemStar == NULL)
	{// ����������ɂȂ��Ă���
		pItemStar = new CItemStar(nPriority);
		if (pItemStar != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pItemStar->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pItemStar;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CItemStar::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���ʂ̏���������
	if (FAILED(CItem::Init()))
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
void CItemStar::Uninit(void)
{
	// ���ʂ̏I������
	CItem::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CItemStar::Update(void)
{
	// ���ʂ̍X�V����
	CItem::Update();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CItemStar::Draw(void)
{
	// ���ʂ̕`�揈��
	CItem::Draw();
}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CItemStar::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemStar::Hit(CScene *pScene)
{
	// �������Ă����I�u�W�F�N�g�̎�ނŏ����킯
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{
		// �v���C���[�N���X�̃|�C���^�ɃL���X�g
		CPlayer *pPlayer = (CPlayer*)pScene;

		// �A�C�e���̃C�x���g���N����
		int nPlayerNumber = pPlayer->GetPlayerIdx();
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			CGame *pGame = CManager::GetGame();
			pGame->ItemEvent_Star(nPlayerNumber);
		}

		// �A�C�e��������
		Uninit();
	}
}

//*****************************************************************************
//    CItemGrenade�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CItemGrenade::CItemGrenade(int nPriority, OBJTYPE objType) : CItem(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CItemGrenade::~CItemGrenade()
{

}

//=============================================================================
//    ��������
//=============================================================================
CItemGrenade *CItemGrenade::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItemGrenade *pItemGrenade = NULL;      // �A�C�e��(��֒e)�N���X�^�̃|�C���^
	if (pItemGrenade == NULL)
	{// ����������ɂȂ��Ă���
		pItemGrenade = new CItemGrenade(nPriority);
		if (pItemGrenade != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pItemGrenade->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pItemGrenade;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CItemGrenade::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���ʂ̏���������
	if (FAILED(CItem::Init()))
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
void CItemGrenade::Uninit(void)
{
	// ���ʂ̏I������
	CItem::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CItemGrenade::Update(void)
{
	// ���ʂ̍X�V����
	CItem::Update();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CItemGrenade::Draw(void)
{
	// ���ʂ̕`�揈��
	CItem::Draw();
}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CItemGrenade::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemGrenade::Hit(CScene *pScene)
{
	// �������Ă����I�u�W�F�N�g�̎�ނŏ����킯
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{
		// �v���C���[�N���X�̃|�C���^�ɃL���X�g
		CPlayer *pPlayer = (CPlayer*)pScene;

		// �A�C�e���̃C�x���g���N����
		int nPlayerNumber = pPlayer->GetPlayerIdx();
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			CGame *pGame = CManager::GetGame();
			pGame->ItemEvent_Grenade(nPlayerNumber);

			// �G�t�F�N�g���o��
			CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
			if (pEffectManager != NULL)
			{
				pEffectManager->SetEffect(GetPos(), INITIALIZE_D3DXVECTOR3, GRANADE_EFFECT_IDX);
			}
		}

		// �A�C�e��������
		Uninit();
	}
}

//*****************************************************************************
//    CItem1up�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CItem1up::CItem1up(int nPriority, OBJTYPE objType) : CItem(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CItem1up::~CItem1up()
{

}

//=============================================================================
//    ��������
//=============================================================================
CItem1up *CItem1up::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItem1up *pItem1Up = NULL;      // �A�C�e��(�c�@UP)�N���X�^�̃|�C���^
	if (pItem1Up == NULL)
	{// ����������ɂȂ��Ă���
		pItem1Up = new CItem1up(nPriority);
		if (pItem1Up != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pItem1Up->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pItem1Up;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CItem1up::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���ʂ̏���������
	if (FAILED(CItem::Init()))
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
void CItem1up::Uninit(void)
{
	// ���ʂ̏I������
	CItem::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CItem1up::Update(void)
{
	// ���ʂ̍X�V����
	CItem::Update();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CItem1up::Draw(void)
{
	// ���ʂ̕`�揈��
	CItem::Draw();
}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CItem1up::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItem1up::Hit(CScene *pScene)
{
	// �������Ă����I�u�W�F�N�g�̎�ނŏ����킯
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{
		// �v���C���[�N���X�̃|�C���^�ɃL���X�g
		CPlayer *pPlayer = (CPlayer*)pScene;

		// �A�C�e���̃C�x���g���N����
		int nPlayerNumber = pPlayer->GetPlayerIdx();
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			CGame *pGame = CManager::GetGame();
			pGame->ItemEvent_1Up(nPlayerNumber);
		}

		// �A�C�e��������
		Uninit();
	}
}

//*****************************************************************************
//    CItemScoop�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CItemScoop::CItemScoop(int nPriority, OBJTYPE objType) : CItem(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CItemScoop::~CItemScoop()
{

}

//=============================================================================
//    ��������
//=============================================================================
CItemScoop *CItemScoop::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItemScoop *pItemScoop = NULL;      // �A�C�e��(�X�R�b�v)�N���X�^�̃|�C���^
	if (pItemScoop == NULL)
	{// ����������ɂȂ��Ă���
		pItemScoop = new CItemScoop(nPriority);
		if (pItemScoop != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pItemScoop->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pItemScoop;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CItemScoop::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���ʂ̏���������
	if (FAILED(CItem::Init()))
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
void CItemScoop::Uninit(void)
{
	// ���ʂ̏I������
	CItem::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CItemScoop::Update(void)
{
	// ���ʂ̍X�V����
	CItem::Update();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CItemScoop::Draw(void)
{
	// ���ʂ̕`�揈��
	CItem::Draw();
}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CItemScoop::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemScoop::Hit(CScene *pScene)
{
	// �������Ă����I�u�W�F�N�g�̎�ނŏ����킯
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{
		// �v���C���[�N���X�̃|�C���^�ɃL���X�g
		CPlayer *pPlayer = (CPlayer*)pScene;

		// �A�C�e���̃C�x���g���N����
		int nPlayerNumber = pPlayer->GetPlayerIdx();
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			CGame *pGame = CManager::GetGame();
			pGame->ItemEvent_Scoop(nPlayerNumber);
		}

		// �A�C�e��������
		Uninit();
	}
}

//*****************************************************************************
//    CItemClock�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CItemClock::CItemClock(int nPriority, OBJTYPE objType) : CItem(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CItemClock::~CItemClock()
{

}

//=============================================================================
//    ��������
//=============================================================================
CItemClock *CItemClock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItemClock *pItemClock = NULL;      // �A�C�e��(���v)�N���X�^�̃|�C���^
	if (pItemClock == NULL)
	{// ����������ɂȂ��Ă���
		pItemClock = new CItemClock(nPriority);
		if (pItemClock != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pItemClock->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pItemClock;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CItemClock::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���ʂ̏���������
	if (FAILED(CItem::Init()))
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
void CItemClock::Uninit(void)
{
	// ���ʂ̏I������
	CItem::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CItemClock::Update(void)
{
	// ���ʂ̍X�V����
	CItem::Update();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CItemClock::Draw(void)
{
	// ���ʂ̕`�揈��
	CItem::Draw();
}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CItemClock::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemClock::Hit(CScene *pScene)
{
	// �������Ă����I�u�W�F�N�g�̎�ނŏ����킯
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{
		// �v���C���[�N���X�̃|�C���^�ɃL���X�g
		CPlayer *pPlayer = (CPlayer*)pScene;

		// �A�C�e���̃C�x���g���N����
		int nPlayerNumber = pPlayer->GetPlayerIdx();
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			CGame *pGame = CManager::GetGame();
			pGame->ItemEvent_Clock(nPlayerNumber);
		}

		// �A�C�e��������
		Uninit();
	}
}

//*****************************************************************************
//    CItemHelmet�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CItemHelmet::CItemHelmet(int nPriority, OBJTYPE objType) : CItem(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CItemHelmet::~CItemHelmet()
{

}

//=============================================================================
//    ��������
//=============================================================================
CItemHelmet *CItemHelmet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItemHelmet *pItemHelmet = NULL;      // �A�C�e��(�w�����b�g)�N���X�^�̃|�C���^
	if (pItemHelmet == NULL)
	{// ����������ɂȂ��Ă���
		pItemHelmet = new CItemHelmet(nPriority);
		if (pItemHelmet != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pItemHelmet->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pItemHelmet;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CItemHelmet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���ʂ̏���������
	if (FAILED(CItem::Init()))
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
void CItemHelmet::Uninit(void)
{
	// ���ʂ̏I������
	CItem::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CItemHelmet::Update(void)
{
	// ���ʂ̍X�V����
	CItem::Update();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CItemHelmet::Draw(void)
{
	// ���ʂ̕`�揈��
	CItem::Draw();
}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CItemHelmet::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemHelmet::Hit(CScene *pScene)
{
	// �������Ă����I�u�W�F�N�g�̎�ނŏ����킯
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{
		// �v���C���[�N���X�̃|�C���^�ɃL���X�g
		CPlayer *pPlayer = (CPlayer*)pScene;

		// �A�C�e���̃C�x���g���N����
		int nPlayerNumber = pPlayer->GetPlayerIdx();
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			CGame *pGame = CManager::GetGame();
			pGame->ItemEvent_Helmet(nPlayerNumber);
		}

		// �A�C�e��������
		Uninit();
	}
}