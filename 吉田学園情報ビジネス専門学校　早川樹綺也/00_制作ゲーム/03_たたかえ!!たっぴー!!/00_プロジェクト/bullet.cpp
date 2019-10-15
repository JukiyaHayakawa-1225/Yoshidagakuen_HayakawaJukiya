//=============================================================================
//
// �e�̏��� [bullet.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "modelcreate.h"
#include "boxCollider.h"
#include "player.h"
#include "enemy.h"
#include "block.h"
#include "headquarters.h"
#include "server.h"
#include "game.h"
#include "effectManager.h"
#include "title.h"
#include "tutorial.h"
#include "debugproc.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BULLET_ENEMY_BREAK_HEADQUARTERS    // �錾���G�̒e�ł��Q�[���I�[�o�[�ɂȂ�
#define BULLET_EFFECT_IDX      (2)         // �e�̃G�t�F�N�g�ԍ�
#define BULLET_MOVE_EFFECT_IDX (17)        // �e���ړ����Ă��鎞�̃G�t�F�N�g�ԍ�

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int CBulletPlayer::m_nNumAll_0 = 0;
int CBulletPlayer::m_nNumAll_1 = 0;
int CBulletEnemy::m_nNumAll = 0;

//*****************************************************************************
// CBullet�̏���
//*****************************************************************************
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBullet::CBullet(int nPriority, OBJTYPE objType) : CObject3D(nPriority, objType)
{
	m_pModel = NULL;
	m_pParent = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBullet::Init(void)
{
	// �{�b�N�X�R���C�_�[����
	CBoxCollider *pBoxCollider = CBoxCollider::Create(GetPos(), GetColRange().x, GetColRange().y, GetColRange().z, false);
	SetBoxCollider(pBoxCollider);

	// �e�̃��f����ݒ�
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		CGame *pGame = CManager::GetGame();
		pGame->SetBulletModel(this);
	}
	else if(CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		CTutorial *pTutorial = CManager::GetTutorial();
		pTutorial->SetBulletModel(this);
	}

	// �G�t�F�N�g���o��
	D3DXVECTOR3 rot = INITIALIZE_D3DXVECTOR3;
	if (m_Move.x > 0.0f)
	{// �E���ɓ����Ă���
		rot.z = D3DX_PI * 0.5f;
	}
	else if(m_Move.x < 0.0f)
	{// �����ɓ����Ă���
		rot.z = -D3DX_PI * 0.5f;
	}
	else if (m_Move.z > 0.0f)
	{// �����ɓ����Ă���
		rot.x = -D3DX_PI * 0.5f;
	}
	else if(m_Move.z < 0.0f)
	{// ��O���ɓ����Ă���
		rot.x = D3DX_PI * 0.5f;
	}

	CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
	if (pEffectManager != NULL)
	{
		pEffectManager->SetEffect(GetPos() + m_Move, rot, BULLET_EFFECT_IDX);
		pEffectManager->SetEffect(GetPos() - (m_Move / 2) + m_Move, rot, BULLET_EFFECT_IDX);
		pEffectManager->SetEffect(GetPos() - m_Move + (m_Move * 2), rot, BULLET_EFFECT_IDX);
		pEffectManager->SetEffect(GetPos() + (m_Move / 2) + m_Move, rot, BULLET_EFFECT_IDX);
		pEffectManager->SetEffect(GetPos() + m_Move + m_Move, rot, BULLET_EFFECT_IDX);
	}
	SetPosOld(GetPos());

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBullet::Uninit(void)
{
	// ���f���̏I������
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	// �I������
	CObject3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBullet::Update(void)
{
	// �ߋ��̈ʒu�ݒu����
	SetPosOld(GetPos());

	if (CManager::GetClient() != NULL)
	{
		if (CManager::GetClient()->GetConnected() == true && CManager::GetClient()->GetClientId() == m_Type)
		{
			// �ړ�����
			Move();
		}
	}
	else
	{
		// �ړ�����
		Move();
	}

	// �G�t�F�N�g���o��
	SetMoveEffect();

	// �����蔻�菈��
	Collision();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBullet::Draw(void)
{
	// �����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���[���h�}�g���b�N�X�̐ݒ菈��
	SetMtxWorld(pDevice);

	// ���f���̕`�揈��
	if (m_pModel != NULL)
	{
		m_pModel->Draw();
	}

	// ���ʂ̕`�揈��
	CObject3D::Draw();
}

//=============================================================================
// �e�̈ړ�����
//=============================================================================
void CBullet::Move(void)
{
	D3DXVECTOR3 pos = GetPos();

	// �ړ�������
	pos += m_Move;

	// �����蔻��p�����f���̈ʒu���ݒ肷��
	CBoxCollider *pCollider = NULL;
	pCollider = CObject3D::GetBoxCollider();
	if (pCollider != NULL)
	{
		pCollider->SetPos(pos);
	}

	// ���W�̐ݒ�
	SetPos(pos);
}

//=============================================================================
// �ړ����Ă��鎞�̃G�t�F�N�g���o��
//=============================================================================
void CBullet::SetMoveEffect(void)
{
	D3DXVECTOR3 rot = GetRot();
	float fAddPosX = 0.0f;
	float fAddPosZ = 0.0f;
	if (m_Move.x > 0.0f)
	{
		fAddPosX -= 25.0f;
	}
	else if(m_Move.x < 0.0f)
	{
		fAddPosX += 25.0f;
	}
	if (m_Move.z > 0.0f)
	{
		fAddPosZ -= 25.0f;
	}
	else if(m_Move.z < 0.0f)
	{
		fAddPosZ += 25.0f;
	}

	D3DXVECTOR3 LocalPos = D3DXVECTOR3(fAddPosX, 0.0f, fAddPosZ);
	LocalPos += GetPos();
	CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
	if (pEffectManager != NULL)
	{
		pEffectManager->SetEffect(LocalPos, INITIALIZE_D3DXVECTOR3, BULLET_MOVE_EFFECT_IDX);
	}
}

//=============================================================================
// �{�b�N�X�R���C�_�[�̈ʒu��ݒ肳����
//=============================================================================
void CBullet::SetBoxColliderPos(void)
{
	D3DXVECTOR3 pos = GetPos();
	if (GetBoxCollider() != NULL)
	{
		GetBoxCollider()->SetPos(pos);
	}
}

//=============================================================================
// �e�̃��f�����蓖�ď���
//=============================================================================
void CBullet::BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture)
{
	if (m_pModel == NULL)
	{
		m_pModel = CModel::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, pMesh, pBuffMat, nNumMat, pTexture);
	}
}

//=============================================================================
// �e�����������Ƃ��̏���
//=============================================================================
void CBullet::Hit(CScene *pScene)
{
	Destroy();
}

//=============================================================================
// �e�̑O��̈ʒu�ݒ菈��
//=============================================================================
void CBullet::SetPosOld(D3DXVECTOR3 posOld)
{
	m_PosOld = posOld;
}

//=============================================================================
// �e�̈ړ��ʂ̐ݒ菈��
//=============================================================================
void CBullet::SetMove(D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
// �e�̔ԍ��̐ݒ菈��
//=============================================================================
void CBullet::SetIdx(const int nIdx)
{
	m_nIdx = nIdx;
}

//=============================================================================
// �e�̎�ނ̐ݒ菈��
//=============================================================================
void CBullet::SetType(const TYPE type)
{
	m_Type = type;
}

//=============================================================================
// �e�̒e�𔭎˂����I�u�W�F�N�g�ւ̃|�C���^�̐ݒ菈��
//=============================================================================
void CBullet::SetParent(CScene *pParent)
{
	m_pParent = pParent;
}

//=============================================================================
// �e�̃��f���N���X�ւ̃|�C���^�̐ݒ菈��
//=============================================================================
void CBullet::SetModel(CModel *pModel)
{
	m_pModel = pModel;
}

//=============================================================================
// �e�̑O��̈ʒu�擾����
//=============================================================================
D3DXVECTOR3 CBullet::GetPosOld(void)
{
	return m_PosOld;
}

//=============================================================================
// �e�̈ړ��ʎ擾����
//=============================================================================
D3DXVECTOR3 CBullet::GetMove(void)
{
	return m_Move;
}

//=============================================================================
// �e�̔ԍ��擾����
//=============================================================================
int CBullet::GetIdx(void)
{
	return m_nIdx;
}

//=============================================================================
// �e�̎�ގ擾����
//=============================================================================
CBullet::TYPE CBullet::GetType(void)
{
	return m_Type;
}

//=============================================================================
// �e�̒e�𔭎˂����I�u�W�F�N�g�ւ̃|�C���^�擾����
//=============================================================================
CScene *CBullet::GetParent(void)
{
	return m_pParent;
}

//=============================================================================
// �e�̃��f���N���X�ւ̃|�C���^�擾����
//=============================================================================
CModel *CBullet::GetModel(void)
{
	return m_pModel;
}


//*****************************************************************************
// CBulletPlayer�̏���
//*****************************************************************************
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBulletPlayer::CBulletPlayer(int nPriority, OBJTYPE objType) : CBullet(nPriority, objType)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBulletPlayer::~CBulletPlayer()
{

}

//=============================================================================
// �e�̐���
//=============================================================================
CBulletPlayer *CBulletPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, TYPE type, CScene *pParent,
	LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, int nPriority)
{
	CBulletPlayer *pBullet = NULL;

	if (pBullet == NULL)
	{
		pBullet = new CBulletPlayer;
		if (pBullet != NULL)
		{
			// �e��l�̐ݒ�
			pBullet->SetPos(pos);
			pBullet->SetRot(rot);
			pBullet->SetMove(move);
			pBullet->SetType(type);
			pBullet->SetColRange(D3DXVECTOR3(18.75f, 18.75f, 18.75f));
			pBullet->SetParent(pParent);

			if (FAILED(pBullet->Init()))
			{
				return NULL;
			}
		}
	}
	return pBullet;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBulletPlayer::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CBullet::Init()))
	{
		return E_FAIL;
	}

	// ���g�̎�ނɂ���ď����킯
	switch (GetType())
	{
	case TYPE_PLAYER_0:
		SetIdx(m_nNumAll_0);
		m_nNumAll_0++;
		break;
	case TYPE_PLAYER_1:
		SetIdx(m_nNumAll_1);
		m_nNumAll_1++;
		break;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBulletPlayer::Uninit(void)
{
	// ���g�̎�ނɂ���ď����킯
	switch (GetType())
	{
	case TYPE_PLAYER_0:
		m_nNumAll_0--;
		break;
	case TYPE_PLAYER_1:
		m_nNumAll_1--;
		break;
	}

	CPlayer *pPlayer = NULL;	// �v���C���[�̃|�C���^���

	// �v���C���[���e��łĂ�悤�ɐݒ肷��
	pPlayer = (CPlayer*)GetParent();
	if (pPlayer != NULL)
	{
		pPlayer->SetCntBullet(pPlayer->GetCntBullet() - 1);
	}

	// ���ʂ̏I������
	CBullet::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBulletPlayer::Update(void)
{
	// �O��̈ʒu�ݒ�
	SetPosOld(GetPos());

	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// �I�����C���v���C�Ȃ��
		if (GetType() == CManager::GetClient()->GetClientId())
		{
			// �ړ�����
			Move();

			// �G�t�F�N�g���o��
			SetMoveEffect();

			// �����蔻�菈��
			Collision();
		}
		else
		{
			// �G�t�F�N�g���o��
			SetMoveEffect();
		}
	}
	else
	{// ���[�J���v���C�Ȃ��
		// �ړ�����
		Move();

		// �G�t�F�N�g���o��
		SetMoveEffect();

		// �����蔻�菈��
		Collision();
	}

	// �{�b�N�X�R���C�_�[���ړ�������
	SetBoxColliderPos();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBulletPlayer::Draw(void)
{
	// ���ʂ̕`�揈��
	CBullet::Draw();
}

//=============================================================================
// �����蔻�菈��
//=============================================================================
void CBulletPlayer::Collision(void)
{
	bool bDeath = false;                  // ���S�������ǂ���
	D3DXVECTOR3 pos = GetPos();           // ����̍��W
	D3DXVECTOR3 posDef = pos;             // �f�t�H���g�̍���̈ʒu(�߂���Ȃ��悤�ɕۑ�)
	D3DXVECTOR3 posOld = GetPosOld();     // �O��̈ʒu
	D3DXVECTOR3 move = GetMove();         // �ړ���
	D3DXVECTOR3 colRange = GetColRange(); // �����蔻������͈�

	// �����蔻��J�n
	CScene *pScene = NULL;     // �I�u�W�F�N�g�̃|�C���^
	CScene *pSceneNext = NULL; // ���̃I�u�W�F�N�g�̃|�C���^
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{
			pSceneNext = pScene->GetNext();
			CollisionCheck(&pos, &posOld, &move, colRange, pScene, &bDeath);
			pos = posDef;
			pScene = pSceneNext;
		}
	}

	// ��`�̓����蔻��
	if (pos.x >= (MASS_SIZE_X * MASS_BLOCK_X) / 2)
	{// �E��
		bDeath = true;
	}
	if (pos.x <= (-MASS_SIZE_X * MASS_BLOCK_X) / 2)
	{// ����
		bDeath = true;
	}
	if (pos.z >= (MASS_SIZE_Z * MASS_BLOCK_Z) / 2)
	{// ����
		bDeath = true;
	}
	if (pos.z <= (-MASS_SIZE_Z * MASS_BLOCK_Z) / 2)
	{// ��O��
		bDeath = true;
	}


	// ���S����`�F�b�N
	if (bDeath == true)
	{
		Destroy();
	}
}

//=============================================================================
// �����蔻��`�F�b�N����
//=============================================================================
void CBulletPlayer::CollisionCheck(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CScene *pScene, bool *pDeath)
{
	CObject3D *pObj = (CObject3D*)pScene;
	if (pObj->GetObjType() == OBJTYPE_PLAYER)
	{// �v���C���[��������
		CPlayer *pPlayer = (CPlayer*)pScene;
		if (CollisionPlayer(pPos, pPosOld, pMove, colRange, (CPlayer*)pScene) == true)
		{// �������Ă���
			if (pPlayer->GetPlayerIdx() != GetType())
			{
				if (CManager::GetMode() == CManager::MODE_TUTORIAL)
				{// �`���[�g���A����ʂ�������
					CTutorial *pTutorial = CManager::GetTutorial();
					if (pTutorial != NULL)
					{
						pTutorial->CreateAttackPlayerInfo();
						pTutorial->HitBullet();
					}
				}
				else if (CManager::GetMode() == CManager::MODE_GAME)
				{// �Q�[����ʂ�������
					CGame *pGame = CManager::GetGame();
					if (pGame != NULL)
					{
						pGame->HitBullet();
					}
				}
				pObj->Hit(this);
				*pDeath = true;
			}
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_ENEMY)
	{// �G��������
		if (CollisionEnemy(pPos, pPosOld, pMove, colRange, (CEnemy*)pScene) == true)
		{// �������Ă���
			((CEnemy*)pObj)->Hit(this);
			*pDeath = true;
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_BULLET && pScene != this)
	{// �e��������
		if (CollisionBullet(pPos, pPosOld, pMove, colRange, (CBullet*)pScene) == true)
		{// �������Ă���
			pObj->Hit(this);
			*pDeath = true;
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_BLOCK)
	{// �u���b�N��������
		if (CollisionBlock(pPos, pPosOld, pMove, colRange, (CBlock*)pScene) == true)
		{// �������Ă���
			pObj->Hit(this);
			*pDeath = true;
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_HEADQUARTERS)
	{// �i�ߕ���������
		if (CollisionHeadQuarters(pPos, pPosOld, pMove, colRange, (CHeadQuarters*)pScene) == true)
		{// �������Ă���
			pObj->Hit(this);
			*pDeath = true;
			if (CManager::GetMode() == CManager::MODE_GAME)
			{// �Q�[����ʂ�������
				CGame *pGame = CManager::GetGame();
				if (pGame != NULL)
				{
					pGame->HitHeadQuarters();
				}
			}
		}
	}
}

//=============================================================================
// �v���C���[�Ƃ̓����蔻�菈��
//=============================================================================
bool CBulletPlayer::CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CPlayer *pPlayer)
{
	CBoxCollider *pBoxCollider = pPlayer->GetBoxCollider();	//�����蔻�蔠�̎擾����
	if (pBoxCollider != NULL)
	{//�����蔻�蔠��NULL�łȂ��ꍇ
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange, NULL) == true)
		{//�����蔻�蔠�ɓ��������ꍇ
			return true;
		};
	}

	return false;
}

//=============================================================================
// �G�Ƃ̓����蔻�菈��
//=============================================================================
bool CBulletPlayer::CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CEnemy *pEnemy)
{
	CBoxCollider *pBoxCollider = pEnemy->GetBoxCollider();	//�����蔻�蔠�̎擾����
	if (pBoxCollider != NULL)
	{//�����蔻�蔠��NULL�łȂ��ꍇ
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange, NULL) == true)
		{//�����蔻�蔠�ɓ��������ꍇ
			return true;
		};
	}

	return false;
}

//=============================================================================
// �e�Ƃ̓����蔻�菈��
//=============================================================================
bool CBulletPlayer::CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBullet *pBullet)
{
	CBoxCollider *pBoxCollider = pBullet->GetBoxCollider();	//�����蔻�蔠�̎擾����
	if (pBoxCollider != NULL)
	{//�����蔻�蔠��NULL�łȂ��ꍇ
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange, NULL) == true)
		{//�����蔻�蔠�ɓ��������ꍇ
			return true;
		};
	}

	return false;
}

//=============================================================================
// �u���b�N�Ƃ̓����蔻�菈��
//=============================================================================
bool CBulletPlayer::CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBlock *pBlock)
{
	CBoxCollider *pBoxCollider = pBlock->GetBoxCollider();	//�����蔻�蔠�̎擾����
	if (pBoxCollider != NULL)
	{//�����蔻�蔠��NULL�łȂ��ꍇ
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange, NULL) == true)
		{//�����蔻�蔠�ɓ��������ꍇ
			if (CManager::GetMode() == CManager::MODE_TUTORIAL)
			{// �`���[�g���A����ʂ�������
				CTutorial *pTutorial = CManager::GetTutorial();
				if (pTutorial != NULL)
				{
					pTutorial->CreateBreakBlockInfo();
				}
			}
			return true;
		};
	}

	return false;
}

//=============================================================================
// �i�ߕ��Ƃ̓����蔻�菈��
//=============================================================================
bool CBulletPlayer::CollisionHeadQuarters(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHeadQuarters *pHead)
{
	CBoxCollider *pBoxCollider = pHead->GetBoxCollider();	//�����蔻�蔠�̎擾����
	if (pBoxCollider != NULL)
	{//�����蔻�蔠��NULL�łȂ��ꍇ
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange, NULL) == true)
		{//�����蔻�蔠�ɓ��������ꍇ
			return true;
		};
	}

	return false;
}

//=============================================================================
// ���S����
//=============================================================================
void CBulletPlayer::Destroy(void)
{
	Uninit();
}

//=============================================================================
// �v���C���[�̒e�̑������擾���鏈��
//=============================================================================
int CBulletPlayer::GetNumAll_0(void)
{
	return m_nNumAll_0;
}
int CBulletPlayer::GetNumAll_1(void)
{
	return m_nNumAll_1;
}

//*****************************************************************************
// CBulletEnemy�̏���
//*****************************************************************************
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBulletEnemy::CBulletEnemy(int nPriority, OBJTYPE objType) : CBullet(nPriority, objType)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBulletEnemy::~CBulletEnemy()
{

}

//=============================================================================
// �e�̐���
//=============================================================================
CBulletEnemy *CBulletEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, CScene *pParent,
	LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, int nPriority)
{
	CBulletEnemy *pBullet = NULL;

	if (pBullet == NULL)
	{
		pBullet = new CBulletEnemy;
		if (pBullet != NULL)
		{
			// �e��l�̐ݒ�
			pBullet->SetPos(pos);
			pBullet->SetRot(rot);
			pBullet->SetMove(move);
			pBullet->SetType(TYPE_ENEMY);
			pBullet->SetColRange(D3DXVECTOR3(18.75f, 18.75f, 18.75f));
			pBullet->SetParent(pParent);

			if (FAILED(pBullet->Init()))
			{
				return NULL;
			}
		}
	}
	return pBullet;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBulletEnemy::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CBullet::Init()))
	{
		return E_FAIL;
	}

	// �ԍ���ݒ�
	SetIdx(m_nNumAll);

	// �e�̑����𑝂₷
	m_nNumAll++;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBulletEnemy::Uninit(void)
{
	// �e�̑��������炷
	m_nNumAll--;

	// ���ʂ̏I������
	CBullet::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBulletEnemy::Update(void)
{
	// �O��̈ʒu�ݒ�
	SetPosOld(GetPos());

	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// �I�����C���v���C�Ȃ��
		if (CManager::GetClient()->GetClientId() == 0)
		{
			// �ړ�����
			Move();

			// �G�t�F�N�g���o��
			SetMoveEffect();

			// �����蔻�菈��
			Collision();
		}
	}
	else
	{// ���[�J���v���C�Ȃ��
	    // �ړ�����
		Move();

		// �G�t�F�N�g���o��
		SetMoveEffect();

		// �����蔻�菈��
		Collision();
	}

	// �{�b�N�X�R���C�_�[�𓮂���
	SetBoxColliderPos();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBulletEnemy::Draw(void)
{
	// ���ʂ̕`�揈��
	CBullet::Draw();
}

//=============================================================================
// �����蔻�菈��
//=============================================================================
void CBulletEnemy::Collision(void)
{
	bool bDeath = false;                  // ���S�������ǂ���
	D3DXVECTOR3 pos = GetPos();           // ����̍��W
	D3DXVECTOR3 posOld = GetPosOld();     // �O��̈ʒu
	D3DXVECTOR3 move = GetMove();         // �ړ���
	D3DXVECTOR3 colRange = GetColRange(); // �����蔻������͈�

	// �����蔻��J�n
	CScene *pScene = NULL;     // �I�u�W�F�N�g�̃|�C���^
	CScene *pSceneNext = NULL; // ���̃I�u�W�F�N�g�̃|�C���^
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{
			pSceneNext = pScene->GetNext();
			CollisionCheck(&pos, &posOld, &move, colRange, pScene, &bDeath);
			pScene = pSceneNext;
		}
	}

	// ��`�̓����蔻��
	if (pos.x >= (MASS_SIZE_X * MASS_BLOCK_X) / 2)
	{// �E��
		bDeath = true;
	}
	if (pos.x <= (-MASS_SIZE_X * MASS_BLOCK_X) / 2)
	{// ����
		bDeath = true;
	}
	if (pos.z >= (MASS_SIZE_Z * MASS_BLOCK_Z) / 2)
	{// ����
		bDeath = true;
	}
	if (pos.z <= (-MASS_SIZE_Z * MASS_BLOCK_Z) / 2)
	{// ��O��
		bDeath = true;
	}

	// ���S����`�F�b�N
	if (bDeath == true)
	{
		Destroy();
	}
}

//=============================================================================
// �����蔻��`�F�b�N����
//=============================================================================
void CBulletEnemy::CollisionCheck(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CScene *pScene, bool *pDeath)
{
	CObject3D *pObj = (CObject3D*)pScene;
	if (pObj->GetObjType() == OBJTYPE_PLAYER)
	{// �v���C���[��������
		if (CollisionPlayer(pPos, pPosOld, pMove, colRange, (CPlayer*)pScene) == true)
		{// �������Ă���
			pObj->Hit(this);
			*pDeath = true;
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_ENEMY)
	{// �G��������
		if (CollisionEnemy(pPos, pPosOld, pMove, colRange, (CEnemy*)pScene) == true)
		{// �������Ă���
			if (GetParent() != pScene)
			{// �����̐e�|�C���^�ł͂Ȃ�
				*pDeath = true;
			}
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_BULLET)
	{// �e��������
		if (CollisionBullet(pPos, pPosOld, pMove, colRange, (CBullet*)pScene) == true)
		{// �������Ă���
			if (pObj != this)
			{// �����ł͂Ȃ�
				pObj->Hit(this);
				*pDeath = true;
			}
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_BLOCK)
	{// �u���b�N��������
		if (CollisionBlock(pPos, pPosOld, pMove, colRange, (CBlock*)pScene) == true)
		{// �������Ă���
			pObj->Hit(this);
			*pDeath = true;
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_HEADQUARTERS)
	{// �i�ߕ���������
		if (CollisionHeadQuarters(pPos, pPosOld, pMove, colRange, (CHeadQuarters*)pScene) == true)
		{// �������Ă���
			pObj->Hit(this);
			*pDeath = true;
		}
	}
}

//=============================================================================
// �v���C���[�Ƃ̓����蔻�菈��
//=============================================================================
bool CBulletEnemy::CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CPlayer *pPlayer)
{
	CBoxCollider *pBoxCollider = pPlayer->GetBoxCollider();	//�����蔻�蔠�̎擾����
	if (pBoxCollider != NULL)
	{//�����蔻�蔠��NULL�łȂ��ꍇ
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange, NULL) == true)
		{//�����蔻�蔠�ɓ��������ꍇ
			return true;
		};
	}

	return false;
}

//=============================================================================
// �G�Ƃ̓����蔻�菈��
//=============================================================================
bool CBulletEnemy::CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CEnemy *pEnemy)
{
	CBoxCollider *pBoxCollider = pEnemy->GetBoxCollider();	//�����蔻�蔠�̎擾����
	if (pBoxCollider != NULL)
	{//�����蔻�蔠��NULL�łȂ��ꍇ
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange, NULL) == true)
		{//�����蔻�蔠�ɓ��������ꍇ
			return true;
		};
	}

	return false;
}

//=============================================================================
// �e�Ƃ̓����蔻�菈��
//=============================================================================
bool CBulletEnemy::CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBullet *pBullet)
{
	CBoxCollider *pBoxCollider = pBullet->GetBoxCollider();	//�����蔻�蔠�̎擾����
	if (pBoxCollider != NULL)
	{//�����蔻�蔠��NULL�łȂ��ꍇ
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange, NULL) == true)
		{//�����蔻�蔠�ɓ��������ꍇ
			return true;
		};
	}

	return false;
}

//=============================================================================
// �u���b�N�Ƃ̓����蔻�菈��
//=============================================================================
bool CBulletEnemy::CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBlock *pBlock)
{
	CBoxCollider *pBoxCollider = pBlock->GetBoxCollider();	//�����蔻�蔠�̎擾����
	if (pBoxCollider != NULL)
	{//�����蔻�蔠��NULL�łȂ��ꍇ
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange, NULL) == true)
		{//�����蔻�蔠�ɓ��������ꍇ
			return true;
		};
	}

	return false;
}

//=============================================================================
// �i�ߕ��Ƃ̓����蔻�菈��
//=============================================================================
bool CBulletEnemy::CollisionHeadQuarters(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHeadQuarters *pHead)
{
	CBoxCollider *pBoxCollider = pHead->GetBoxCollider();	//�����蔻�蔠�̎擾����
	if (pBoxCollider != NULL)
	{//�����蔻�蔠��NULL�łȂ��ꍇ
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange, NULL) == true)
		{//�����蔻�蔠�ɓ��������ꍇ
#ifdef BULLET_ENEMY_BREAK_HEADQUARTERS
			return true;
#endif
		};
	}

	return false;
}

//=============================================================================
// ���S����
//=============================================================================
void CBulletEnemy::Destroy(void)
{
	Uninit();
}

//=============================================================================
// �G�̒e�̑������擾���鏈��
//=============================================================================
int CBulletEnemy::GetNumAll(void)
{
	return m_nNumAll;
}