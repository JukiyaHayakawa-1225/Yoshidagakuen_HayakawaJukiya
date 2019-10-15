//=============================================================================
//
// �G�̏��� [enemy.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "boxCollider.h"
#include "bullet.h"
#include "player.h"
#include "block.h"
#include "server.h"
#include "headquarters.h"
#include "river.h"
#include "game.h"
#include "item.h"
#include "tutorial.h"
#include "title.h"
#include "effectManager.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_SPAWN_EFFECT_IDX   (3)     // �G�������̃G�t�F�N�g�ԍ�
#define ENEMY_DEATH_EFFECT_IDX   (7)     // �G���S���̃G�t�F�N�g�ԍ�
#define ENEMY_SPAWN_SE_IDX       (15)    // �G�������̉��ԍ�
#define ENEMY_DEATH_SE_IDX       (16)    // �G���S���̉��ԍ�
#define ENEMY_AI_NONE_TIME       (2)     // �������~�߂Ă��瓮���o���܂ł̎���
#define ENEMY_AI_MOVE_TIME       (120)   // �����Ă��瓮�����~�߂���܂ł̎���
#define ENEMY_MASS_COLRANGE      (2.0f)  // �}�X�͈͓̔��Ɣ��肳���͈�
#define ENEMY_LIFE               (1)     // �G�̗̑�
#define ENEMY_HEAVY_LIFE         (3)     // �w�r�[�^���N�̗̑�
#define ENEMY_FAST_BULLETMOVE_UP (1.2f)  // ���˖C�^���N�̒e�̑������グ��{��
#define ENEMY_COL_PRIORITY       (4)     // �R���W�������s���D�揇��

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int CEnemy::m_nNumAll = 0;
int CEnemy::m_nSpawnCounter = 0;


//*****************************************************************************
// CEnemy�̏���
//*****************************************************************************
//=============================================================================
// �G�̃R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{

}

//=============================================================================
// �G�̃f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// �G�̐���
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority)
{
	CEnemy *pEnemy = NULL;

	if (pEnemy == NULL)
	{// NULL�̏ꍇ
		pEnemy = new CEnemy(nPriority);
		if (pEnemy != NULL)
		{// NULL�łȂ��ꍇ
			pEnemy->SetPos(pos);
			pEnemy->SetRot(rot);
			pEnemy->SetColRange(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pEnemy->SetReturnFlag(true);
			pEnemy->Init();
		}
	}
	return pEnemy;
}
//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemy::Init(void)
{
	// �ϐ����N���A����
	ClearVariable();

	// �ԍ��̐ݒ�
	SetIdx(m_nSpawnCounter);

	// �����𑝂₷
	m_nNumAll++;

	// �X�|�[�����𑝂₷
	m_nSpawnCounter++;

	// �̗͂�ݒ�
	m_nLife = ENEMY_LIFE;

	// �L�����N�^�[�̏���������
	if (FAILED(CCharacter::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemy::Uninit(void)
{
	// ���������炷
	m_nNumAll--;

	// �G�̐������炷
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		CGame *pGame = CManager::GetGame();
		int nNumEnemy = pGame->GetNumEnemy();
		nNumEnemy--;
		pGame->SetNumEnemy(nNumEnemy);
		CClient *pClient = CManager::GetClient();
		if (nNumEnemy <= 0)
		{// �G���������Ȃ�
			if (CTitle::GetGameMode() != CTitle::GAMEMODE_ONLINE2P || pClient == NULL)
			{// ���[�J���v���C�Ȃ�ΏI����Ԃ�
				pGame->SetState(CGame::STATE_END);
				m_nSpawnCounter = 0;
			}
			else if(pClient != NULL && pClient->GetClientId() == 0)
			{// �z�X�g��������I����Ԃ�
				pGame->SetState(CGame::STATE_END);
				m_nSpawnCounter = 0;
			}
		}
		else
		{// �G���܂�����
			// ���g�̃N���C�A���g�ԍ����擾
			int nIdxClient = 0;
			CClient *pClient = CManager::GetClient();
			if (pClient != NULL)
			{
				nIdxClient = pClient->GetClientId();
			}

			// �A�C�e���𐶐�����
			if (m_bItem == true && nIdxClient == 0)
			{
				pGame->CreateItem(GetPos(), INITIALIZE_D3DXVECTOR3, m_nItemType);
			}
		}
	}

	// �I������
	CCharacter::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemy::Update(void)
{
	// �O��̈ʒu���X�V
	SetPosOld(GetPos());

	// �z�X�g��������G��AI����������
	if (CManager::GetClient() != NULL)
	{
		if (CManager::GetClient()->GetClientId() == 0)
		{
			HostUpdate();
		}
		else if(CManager::GetClient()->GetConnected() == false)
		{
			HostUpdate();
		}
	}
	else
	{
		HostUpdate();
	}

	// �����蔻��p�����f���𓮂���
	if (GetBoxCollider() != NULL)
	{
		GetBoxCollider()->SetPos(GetPos());
	}

	// �����蔻��̏���
	Collision();
}

//=============================================================================
// �z�X�g���������̍X�V����
//=============================================================================
void CEnemy::HostUpdate(void)
{
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂœG���������Ԃł���
		if (CManager::GetGame()->GetEnemyMove() == true)
		{
			// AI�̏���
			AI();

			// �ړ��ʉ��Z����
			Movement();
		}
	}
	else
	{// �Q�[����ʂł͂Ȃ�
		// AI�̏���
		AI();

		// �ړ��ʉ��Z����
		Movement();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	// �����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���[���h�}�g���b�N�X�ݒ菈��
	SetMtxWorld(pDevice);

	// �L�����N�^�[�̕`�揈��
	CCharacter::Draw();
}

//=============================================================================
// �I�u�W�F�N�g�Ƀq�b�g�����Ƃ��̏���
//=============================================================================
void CEnemy::Hit(CScene * pScene)
{
	// ���������I�u�W�F�N�g�ɂ���ď����킯
	if (pScene->GetObjType() == OBJTYPE_BULLET)
	{// �e������
		CBullet *pBullet = (CBullet*)pScene;
		if (pBullet->GetType() == CBullet::TYPE_PLAYER_0 || pBullet->GetType() == CBullet::TYPE_PLAYER_1)
		{// �v���C���[�̒e������
			// �̗͂����炷
			m_nLife--;
			if (m_nLife <= 0)
			{// �̗͂������Ȃ�
			    // ���S������
				Destroy();

				// �X�R�A�����Z����
				ScoreUp(pBullet);
			}
			else
			{// �̗͂��܂�����
				// �p�[�c�����
				DeleteParts();
			}
		}
	}
}

//=============================================================================
// �X�R�A�����Z�����鏈��
//=============================================================================
void CEnemy::ScoreUp(CBullet *pBullet)
{
	if (pBullet == NULL) { return; }

	// �X�R�A���A�b�v������v���C���[�̔ԍ���ݒ�
	int nPlayerIdx = pBullet->GetType();

	// �X�R�A�����Z����
	if (CManager::GetMode() != CManager::MODE_GAME) { return; }
	CGame *pGame = CManager::GetGame();
	if (pGame == NULL) { return; }

	switch (m_Type)
	{// ���g�̎�ނɂ���ď����킯
	case TYPE_NORMAL:
		pGame->ScoreUp_NormalEnemy(nPlayerIdx);
		break;
	case TYPE_ARMORE:
		pGame->ScoreUp_ArmoreEnemy(nPlayerIdx);
		break;
	case TYPE_FAST:
		pGame->ScoreUp_FastEnemy(nPlayerIdx);
		break;
	case TYPE_HEAVY:
		pGame->ScoreUp_HeavyEnemy(nPlayerIdx);
		break;
	}
}

//=============================================================================
// �G�̃p�[�c����鏈��
//=============================================================================
void CEnemy::DeleteParts(void)
{
	// �p�[�c�����擾
	int nNumParts = GetNumPart();

	// ���f���N���X�ւ̃|�C���^��j������
	CModel **pModel = GetModel();
	if (pModel == NULL || nNumParts <= 0) { return; }
	pModel[nNumParts - 1]->Uninit();
	delete pModel[nNumParts - 1];
	pModel[nNumParts - 1] = NULL;

	// �p�[�c��������炷
	SetNumPart(nNumParts - 1);
}

//=============================================================================
// AI�̏���
//=============================================================================
void CEnemy::AI(void)
{
	switch (m_AIState)
	{// ��Ԃɂ���ď����킯
	case AI_STATE_NONE:
		AI_None();
		break;
	case AI_STATE_CHANGEROT:
		AI_ChangeRot();
		break;
	case AI_STATE_MOVE:
		AI_AutoMove();
		break;
	}
}

//=============================================================================
// AI�̈ړ�����
//=============================================================================
void CEnemy::AI_None(void)
{
	// �ړ��ʂ𖳂����Ă���
	SetMove(INITIALIZE_D3DXVECTOR3);

	m_nAI_StateCounter++;
	if (m_nAI_StateCounter % ENEMY_AI_NONE_TIME == 0)
	{// ������x���Ԃ��~�܂���
		// ���Ɉړ�������������肷��
		SetNextMoveDirection();

		m_AIState = AI_STATE_CHANGEROT;
		m_nAI_StateCounter = 0;
	}
}

//=============================================================================
// AI�̎��Ɉړ�������������肷�鏈��
//=============================================================================
void CEnemy::SetNextMoveDirection(void)
{
	int nNextRot = m_RotInfoOld;  // ���Ɉړ��������

	// �����_���̕]���l���牺�����Ɉړ����邩����
	while (m_RotInfoOld == nNextRot)
	{
		nNextRot = rand() % (m_nAI_DownEva + 1);
	}
	if (nNextRot == 0)
	{// �������Ɉړ�����Ȃ��
		SetNowRotInfo(ROT_DOWN);
	}
	else
	{
		// ���Ɉړ���������������_���Ō���
		while (nNextRot == ROT_DOWN)
		{
			nNextRot = rand() % ROT_MAX;
			if (m_RotInfoOld == nNextRot)
			{
				nNextRot = ROT_DOWN;
			}
		}
		SetNowRotInfo((NOW_ROT_INFO)nNextRot);
	}

	// ���̖ړI�n�̃}�X���v�Z
	CalcDestMass();
}

//=============================================================================
// ���̖ړI�n�̃}�X�ڂ��Z�o����
//=============================================================================
void CEnemy::CalcDestMass(void)
{
	// ���݂���}�X�ڂ��Z�o
	D3DXVECTOR3 pos = GetPos();
	int nMassX = (int)((pos.x + (MASS_SIZE_X * MASS_BLOCK_X / 2)) / MASS_SIZE_X) + 1;
	int nMassZ = (int)((-pos.z + (MASS_SIZE_Z * MASS_BLOCK_Z / 2)) / MASS_SIZE_Z) + 1;

	// ���Ɍ�������������ړI�̃}�X�����肷��
	NOW_ROT_INFO nowRot = GetNowRotInfo();
	switch (GetNowRotInfo())
	{// ���݂̌������ɂ���ď����킯
	case ROT_UP:	// ������Ɍ����Ă���
		nMassZ--;
		break;
	case ROT_DOWN:	// �������Ɍ����Ă���
		nMassZ++;
		break;
	case ROT_LEFT:	// �������Ɍ����Ă���
		nMassX--;
		break;
	case ROT_RIGHT:	// �E�����Ɍ����Ă���
		nMassX++;
		break;
	}

	// �ړI�̃}�X������
	m_nAI_DestMassX = nMassX;
	m_nAI_DestMassZ = nMassZ;
}

//=============================================================================
// AI�̈ړ�����
//=============================================================================
void CEnemy::AI_ChangeRot(void)
{
	// �ړ��ʂ𖳂����Ă���
	SetMove(INITIALIZE_D3DXVECTOR3);

	// �e��l�̎擾
	D3DXVECTOR3 rot = GetRot();

	float fDiffAngle = 0.0f;
	float fDestAngle = 0.0f;
	switch (GetNowRotInfo())
	{// ���݂̌������ɂ���ď����킯
	case ROT_UP:	// ������Ɍ����Ă���
		fDestAngle = D3DX_PI;
		break;
	case ROT_DOWN:	// �������Ɍ����Ă���
		fDestAngle = 0.0f;
		break;
	case ROT_LEFT:	// �������Ɍ����Ă���
		fDestAngle = (D3DX_PI * 0.5f);
		break;
	case ROT_RIGHT:	// �E�����Ɍ����Ă���
		fDestAngle = (D3DX_PI * -0.5f);
		break;
	}

	// �����̌��������߂�
	fDiffAngle = fDestAngle - rot.y;

	// �����̕␳
	if (fDiffAngle > D3DX_PI)
	{
		fDiffAngle -= D3DX_PI * 2.0f;
	}
	if (fDiffAngle < -D3DX_PI)
	{
		fDiffAngle += D3DX_PI * 2.0f;
	}

	// �����̉��Z
	rot.y += fDiffAngle * GetRivisionRot();
	if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}

	// �ړI�̌����ɒB���Ă����瓮���o��
	if (rot.y <= fDestAngle + 0.2f && rot.y >= fDestAngle - 0.2f)
	{
		rot.y = fDestAngle;
		m_AIState = AI_STATE_MOVE;
	}
	if (fDestAngle == D3DX_PI && rot.y >= -D3DX_PI - 0.2f && rot.y <= D3DX_PI + 0.2f)
	{
		rot.y = fDestAngle;
		m_AIState = AI_STATE_MOVE;
	}

	// ������ݒ肷��
	SetRot(rot);
}

//=============================================================================
// AI�̈ړ�����
//=============================================================================
void CEnemy::AI_AutoMove(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 move = GetMove();

	// �ړ��̎�ޕʏ���
	switch (GetNowRotInfo())
	{
	case ROT_UP:	// ������Ɍ����Ă���
		move.z = GetAccel();
		break;
	case ROT_DOWN:	// �������Ɍ����Ă���
		move.z = -GetAccel();
		break;
	case ROT_LEFT:	// �������Ɍ����Ă���
		move.x = -GetAccel();
		break;
	case ROT_RIGHT:	// �E�����Ɍ����Ă���
		move.x = GetAccel();
		break;
	}

	// �e��l�̐ݒ�
	SetMove(move);

	// �e��ł���
	if (rand() % m_nAI_BulletEva == 0)
	{// �����_���̕]���l�Ō���
		SetBullet();
	}

	// ���̃}�X�ɓ��B���Ă��邩�ǂ�������
	if (CheckNextMovingMass() == true)
	{// ���̃}�X�ɓ��B���Ă���
		m_nAI_MassCount++;  // �}�X�Ɉړ������񐔂����Z

		// ���Ɍ�����������ς��邩�ǂ����������_���̕]���l����Z�o
		int nChangeMoving = 0;
		int nMassRandom = m_nAI_MassEva - m_nAI_MassCount;
		if (nMassRandom > 0)
		{
			nChangeMoving = rand() % nMassRandom;
		}
		if (nChangeMoving == 0)
		{// ���Ɍ�����������ς���Ȃ�
			ResetAIState();
		}
	}

	// ���������Ԃ����Z
	m_nAI_MoveCounter++;
}

//=============================================================================
// ���̃}�X�ɓ��B���Ă��邩�ǂ�������
//=============================================================================
bool CEnemy::CheckNextMovingMass(void)
{
	// �ړI�̃}�X�ڂ�����W���Z�o
	D3DXVECTOR3 DestPos = INITIALIZE_D3DXVECTOR3;
	float fAddWidth = (MASS_SIZE_X * MASS_BLOCK_X) / 2 + (MASS_SIZE_X / 2);
	float fAddDepth = (MASS_SIZE_Z * MASS_BLOCK_Z) / 2 + (MASS_SIZE_Z / 2);
	float fWidth = MASS_SIZE_X;
	float fDepth = MASS_SIZE_Z;
	DestPos.x = (m_nAI_DestMassX * fWidth) - fAddWidth;
	DestPos.z = -(m_nAI_DestMassZ * fDepth) + fAddDepth;

	// �Z�o�������W����͈͂�ݒ肵���͈͓̔��ł���Γ��B���Ă���Ɣ���
	D3DXVECTOR3 pos = GetPos();
	if (pos.x >= DestPos.x - ENEMY_MASS_COLRANGE && pos.x <= DestPos.x + ENEMY_MASS_COLRANGE
		&& pos.z >= DestPos.z - ENEMY_MASS_COLRANGE && pos.z <= DestPos.z + ENEMY_MASS_COLRANGE)
	{
		return true;
	}

	return false;
}

//=============================================================================
// �ړ��ʉ��Z����
//=============================================================================
void CEnemy::Movement(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();

	// �ړ��ʉ��Z
	pos += move;

	// ������������
	move.x += (0.0f - move.x) * GetInertia();
	move.z += (0.0f - move.z) * GetInertia();

	// �e��l�̐ݒ�
	SetPos(pos);
}

//=============================================================================
// �e��ł���
//=============================================================================
void CEnemy::SetBullet(void)
{
	D3DXVECTOR3 BulletMove;
	D3DXVECTOR3 pos = GetPos();

	switch (GetNowRotInfo())
	{// ���݌����Ă�������Őݒ�
	case ROT_UP:	//��������Ă���ꍇ
		BulletMove = D3DXVECTOR3(0.0f, 0.0f, 8.0f);
		break;
	case ROT_DOWN:	//���������Ă���ꍇ
		BulletMove = D3DXVECTOR3(0.0f, 0.0f, -8.0f);
		break;
	case ROT_RIGHT:	//�E�������Ă���ꍇ
		BulletMove = D3DXVECTOR3(8.0f, 0.0f, 0.0f);
		break;
	case ROT_LEFT:	//���������Ă���ꍇ
		BulletMove = D3DXVECTOR3(-8.0f, 0.0f, 0.0f);
		break;
	}

	// ���˖C�^���N��������e�̑��x���グ��
	if (m_Type == TYPE_FAST)
	{
		BulletMove *= ENEMY_FAST_BULLETMOVE_UP;
	}

	// �e�̐���
	CBulletEnemy::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), BulletMove, this); //�e�̐���
}

//=============================================================================
// �����蔻��̏���
//=============================================================================
void CEnemy::Collision(void)
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
	for (int nCntPriority = 0; nCntPriority < ENEMY_COL_PRIORITY + 1; nCntPriority++)
	{
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{
			pSceneNext = pScene->GetNext();
			CollisionCheck(&pos, &posOld, &move, colRange, pScene, &bDeath);
			pScene = pSceneNext;
		}
	}

	// �͈͊O�ɍs�����ꍇ���Ƃɖ߂�
	if (pos.x > ((MASS_SIZE_X * MASS_BLOCK_X) / 2) - MASS_SIZE_X_HALF)
	{
		pos.x = ((MASS_SIZE_X * MASS_BLOCK_X) / 2) - MASS_SIZE_X_HALF;
		ResetAIState();
	}
	if (pos.x < -((MASS_SIZE_X * MASS_BLOCK_X) / 2) + MASS_SIZE_X_HALF)
	{
		pos.x = -((MASS_SIZE_X * MASS_BLOCK_X) / 2) + MASS_SIZE_X_HALF;
		ResetAIState();
	}
	if (pos.z > ((MASS_SIZE_Z * MASS_BLOCK_Z) / 2) - MASS_SIZE_Z_HALF)
	{
		pos.z = ((MASS_SIZE_Z * MASS_BLOCK_Z) / 2) - MASS_SIZE_Z_HALF;
		ResetAIState();
	}
	if (pos.z < -((MASS_SIZE_Z * MASS_BLOCK_Z) / 2) + MASS_SIZE_Z_HALF)
	{
		pos.z = -((MASS_SIZE_Z * MASS_BLOCK_Z) / 2) + MASS_SIZE_Z_HALF;
		ResetAIState();
	}

	// �e��l�̐ݒ�
	SetPos(pos);
	SetPosOld(posOld);
	SetMove(move);

	// ���S����
	if (bDeath == true)
	{
		if (m_nLife <= 0)
		{// �̗͂������Ȃ�
		    // ���S������
			Destroy();
		}
		else
		{// �̗͂��܂�����
		    // �p�[�c�����
			DeleteParts();
		}
	}
}

//=============================================================================
// �������Ă��邩�ǂ����`�F�b�N���鏈��
//=============================================================================
void CEnemy::CollisionCheck(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CScene *pScene, bool *pDeath)
{
	CObject3D *pObj = (CObject3D*)pScene;
	if (pObj->GetObjType() == OBJTYPE_PLAYER)
	{// �v���C���[��������
		if (CollisionPlayer(pPos, pPosOld, pMove, colRange, (CPlayer*)pScene) == true)
		{// �������Ă���
			pObj->Hit(this);
			ResetAIState();
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_ENEMY)
	{// �G��������
		if (CollisionEnemy(pPos, pPosOld, pMove, colRange, (CEnemy*)pScene) == true)
		{// �������Ă���
			pObj->Hit(this);
			ResetAIState();
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_BULLET)
	{// �e��������
		CBullet *pBullet = (CBullet*)pScene;
		if (CollisionBullet(pPos, pPosOld, pMove, colRange, pBullet) == true)
		{// �������Ă���
			if (pBullet->GetType() == CBullet::TYPE_PLAYER_0 || pBullet->GetType() == CBullet::TYPE_PLAYER_1)
			{// �v���C���[�̒e������
				pBullet->Hit(this);
				*pDeath = true;

				// �̗͂����炷
				m_nLife--;
				if (m_nLife <= 0)
				{// �̗͂���������
					ScoreUp(pBullet);
				}
			}
			else if(pBullet->GetParent() != this)
			{// �����ȊO�̓G���������G�̒e������
				pBullet->Hit(this);
			}
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_BLOCK)
	{// �u���b�N��������
		if (CollisionBlock(pPos, pPosOld, pMove, colRange / 2, (CBlock*)pScene) == true)
		{// �������Ă���
			pObj->Hit(this);
			ResetAIState();
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_HEADQUARTERS)
	{// �i�ߕ���������
		if (CollisionHeadQuarters(pPos, pPosOld, pMove, colRange, (CHeadQuarters*)pScene) == true)
		{// �������Ă���
			pObj->Hit(this);
			ResetAIState();
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_RIVER)
	{// �삾������
		if (CollisionRiver(pPos, pPosOld, pMove, colRange, (CRiver*)pScene) == true)
		{// �������Ă���
			pObj->Hit(this);
			ResetAIState();
		}
	}
}

//=============================================================================
// �v���C���[�Ƃ̓����蔻�菈��
//=============================================================================
bool CEnemy::CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CPlayer *pPlayer)
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
bool CEnemy::CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CEnemy *pEnemy)
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
bool CEnemy::CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBullet *pBullet)
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
bool CEnemy::CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBlock *pBlock)
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
bool CEnemy::CollisionHeadQuarters(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHeadQuarters *pHead)
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
// �G�Ƃ̓����蔻�菈��
//=============================================================================
bool CEnemy::CollisionRiver(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CRiver *pRiver)
{
	CBoxCollider *pBoxCollider = pRiver->GetBoxCollider();	//�����蔻�蔠�̎擾����
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
// ���񂾂Ƃ��̏���
//=============================================================================
void CEnemy::Destroy(void)
{
	// �G�����������Ƃ����m�点
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		CGame *pGame = CManager::GetGame();
		CClient *pClient = CManager::GetClient();
		if (pClient == NULL)
		{
			SetDeathEffect();
			pGame->AddPowerMap();
			Uninit();
			return;
		}
		else if (pClient != NULL && pClient->GetConnected() == false)
		{
			SetDeathEffect();
			pGame->AddPowerMap();
			Uninit();
			return;
		}
		else if (pClient != NULL && pClient->GetClientId() == 0)
		{
			SetDeathEffect();
			pGame->AddPowerMap();
			Uninit();
			return;
		}
		if (pClient != NULL && CManager::GetClient()->GetClientId() != 0)
		{
			SetDeathEffect();
			pGame->DeleteEnemy(m_nIdx);
			pGame->AddPowerMap();
		}
	}
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		CTutorial *pTutorial = CManager::GetTutorial();
		CClient *pClient = CManager::GetClient();
		if (pClient == NULL)
		{
			SetDeathEffect();
			Uninit();
			return;
		}
		else if (pClient != NULL && pClient->GetConnected() == false)
		{
			SetDeathEffect();
			Uninit();
			return;
		}
		else if (pClient != NULL && pClient->GetClientId() == 0)
		{
			SetDeathEffect();
			Uninit();
			return;
		}
		if (pClient != NULL && CManager::GetClient()->GetClientId() != 0)
		{
			SetDeathEffect();
			pTutorial->DeleteEnemy(m_nIdx);
		}
	}
	else
	{
		SetDeathEffect();
		Uninit();
	}
}

//=============================================================================
// �X�|�[�������Ƃ��̃G�t�F�N�g�𐶐����鏈��
//=============================================================================
void CEnemy::SetSpawnEffect(void)
{
	D3DXVECTOR3 rot = INITIALIZE_D3DXVECTOR3;
	CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
	if (pEffectManager != NULL)
	{
		pEffectManager->SetEffect(GetPos(), rot, ENEMY_SPAWN_EFFECT_IDX);
		rot.y += D3DX_PI * 0.1f;
		pEffectManager->SetEffect(GetPos(), rot, ENEMY_SPAWN_EFFECT_IDX + 1);
		rot.y += D3DX_PI * 0.1f;
		pEffectManager->SetEffect(GetPos(), rot, ENEMY_SPAWN_EFFECT_IDX + 2);
	}

	// �����o��
	CManager::GetSound()->PlaySound(ENEMY_SPAWN_SE_IDX);
}

//=============================================================================
// ���񂾂Ƃ��̃G�t�F�N�g�𐶐����鏈��
//=============================================================================
void CEnemy::SetDeathEffect(void)
{
	CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
	if (pEffectManager != NULL)
	{
		pEffectManager->SetEffect(GetPos(), INITIALIZE_D3DXVECTOR3, ENEMY_DEATH_EFFECT_IDX);
	}

	// �����o��
	CManager::GetSound()->PlaySound(ENEMY_DEATH_SE_IDX);
}

//=============================================================================
// �A�C�e���̃f�[�^��ݒ肷�鏈��
//=============================================================================
void CEnemy::SetItemData(const int nItemType)
{
	m_bItem = true;
	m_nItemType = nItemType;
}

//=============================================================================
// AI�̏�Ԃ����ɖ߂�����
//=============================================================================
void CEnemy::ResetAIState(void)
{
	if (GetAI_State() == AI_STATE_MOVE && m_nAI_MoveCounter >= ENEMY_AI_MOVE_TIME)
	{
		m_RotInfoOld = GetNowRotInfo();
		SetAI_State(AI_STATE_NONE);
		m_nAI_StateCounter = 0;
		m_nAI_MoveCounter = 0;
	}
}

//=============================================================================
// �ϐ����N���A���鏈��
//=============================================================================
void CEnemy::ClearVariable(void)
{
	m_nIdx = 0;                 // �G�̔ԍ�(�N���C�A���g�Ԃœ���������̂ɕK�v)
	m_nLife = 0;                // �G�̗̑�
	m_bItem = false;            // �A�C�e���𗎂Ƃ����ǂ���
	m_nItemType = 0;            // �A�C�e���̔ԍ�
	m_Type = TYPE_NORMAL;       // �G�̎��
	m_AIState = AI_STATE_NONE;  // AI�̏��
	m_nAI_StateCounter = 0;     // AI�̏�Ԃ��Ǘ�����J�E���^�[
	m_nAI_MoveCounter = 0;      // �����n�߂Ă���ǂꂭ�炢���Ԃ���������
	m_nAI_BulletEva = 0;        // �e��łۂ̃����_���]���l
	m_nAI_MassEva = 0;          // �}�X���ړ������ۂɌ�����ς������邩���肷��Ƃ��̃����_���]���l
	m_nAI_MassCount = 0;        // �ǂꂭ�炢�}�X���ړ��������𐔂���J�E���^�[
	m_nAI_DownEva = 0;          // �������ɓ��������ǂ����𔻒肷��ۂ̃����_���]���l
	m_nAI_NotDownCount = 0;     // �������ɓ����Ȃ������񐔂𐔂���J�E���^�[
	m_nAI_DestMassX = 0;        // �ړI�n�̃}�X��(������)
	m_nAI_DestMassZ = 0;        // �ړI�n�̃}�X��(�c����)
}

//=============================================================================
// �G��AI�f�[�^��ݒ肷�鏈��
//=============================================================================
void CEnemy::SetAIData(const int nBulletEva, const int nMassEva, const int nDownEva)
{
	m_nAI_BulletEva = nBulletEva;
	m_nAI_MassEva = nMassEva;
	m_nAI_DownEva = nDownEva;

	// AI�̏�Ԃ͈ړ������Ԃ�
	SetAI_State(AI_STATE_MOVE);

	// �ړ�����������Z�o
	SetNextMoveDirection();
	switch (GetNowRotInfo())
	{// ���݂̌������ɂ���ď����킯
	case ROT_UP:	// ������Ɍ����Ă���
		SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
		break;
	case ROT_DOWN:	// �������Ɍ����Ă���
		SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		break;
	case ROT_LEFT:	// �������Ɍ����Ă���
		SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		break;
	case ROT_RIGHT:	// �E�����Ɍ����Ă���
		SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		break;
	}
}

//=============================================================================
// �G�̑�����ݒ肷��
//=============================================================================
void CEnemy::SetNumAll(const int nNumAll)
{
	m_nNumAll = nNumAll;
}

//=============================================================================
// �G�̑������擾����
//=============================================================================
int CEnemy::GetNumAll(void)
{
	return m_nNumAll;
}

//=============================================================================
// �X�|�[���J�E���^�[���擾���鏈��
//=============================================================================
int CEnemy::GetSpawnCounter(void)
{
	return m_nSpawnCounter;
}

//=============================================================================
// �X�|�[���J�E���^�[�����������鏈��
//=============================================================================
void CEnemy::ResetSpawnCounter(void)
{
	m_nSpawnCounter = 0;
}

//=============================================================================
// �ԍ���ݒ肷��
//=============================================================================
void CEnemy::SetIdx(const int nIdx)
{
	m_nIdx = nIdx;
}

//=============================================================================
// �̗͂�ݒ肷��
//=============================================================================
void CEnemy::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
// ��ނ�ݒ肷��
//=============================================================================
void CEnemy::SetType(const TYPE type)
{
	m_Type = type;
}

//=============================================================================
// AI�̏�Ԃ�ݒ肷��
//=============================================================================
void CEnemy::SetAI_State(const AI_STATE AI_State)
{
	m_AIState = AI_State;
}

//=============================================================================
// AI���Ǘ�����J�E���^�[��ݒ肷��
//=============================================================================
void CEnemy::SetAI_StateCounter(const int nStateCounter)
{
	m_nAI_StateCounter = nStateCounter;
}

//=============================================================================
// �e��łۂ̃����_���]���l��ݒ肷��
//=============================================================================
void CEnemy::SetAI_BulletEva(const int nBulletEva)
{
	m_nAI_BulletEva = nBulletEva;
}

//=============================================================================
// �}�X���ړ������ۂɌ�����ς������邩���肷��Ƃ��̃����_���]���l���擾����
//=============================================================================
void CEnemy::SetAI_MassEva(const int nMassEva)
{
	m_nAI_MassEva = nMassEva;
}

//=============================================================================
// �ǂꂭ�炢�}�X���ړ���������ݒ肷��
//=============================================================================
void CEnemy::SetAI_MassCount(const int nMassCount)
{
	m_nAI_MassCount = nMassCount;
}

//=============================================================================
// �������ɓ��������ǂ����𔻒肷��ۂ̃����_���]���l��ݒ肷��
//=============================================================================
void CEnemy::SetAI_DownEva(const int nDownEva)
{
	m_nAI_DownEva = nDownEva;
}

//=============================================================================
// �������ɓ����Ȃ������񐔂𐔂���J�E���^�[��ݒ肷��
//=============================================================================
void CEnemy::SetAI_NotDownCount(const int nNotDownCount)
{
	m_nAI_NotDownCount = nNotDownCount;
}

//=============================================================================
// �ԍ����擾����
//=============================================================================
int CEnemy::GetIdx(void)
{
	return m_nIdx;
}

//=============================================================================
// �̗͂��擾����
//=============================================================================
int CEnemy::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
// �G�̎�ނ��擾����
//=============================================================================
CEnemy::TYPE CEnemy::GetType(void)
{
	return m_Type;
}

//=============================================================================
// AI�̏�Ԃ��擾����
//=============================================================================
CEnemy::AI_STATE CEnemy::GetAI_State(void)
{
	return m_AIState;
}

//=============================================================================
// AI�̏�Ԃ��Ǘ�����J�E���^�[���擾����
//=============================================================================
int CEnemy::GetAI_StateCounter(void)
{
	return m_nAI_StateCounter;
}

//=============================================================================
// �e��łۂ̃����_���]���l���擾����
//=============================================================================
int CEnemy::GetAI_BulletEva(void)
{
	return m_nAI_BulletEva;
}

//=============================================================================
// �}�X���ړ������ۂɌ�����ς������邩���肷��Ƃ��̃����_���]���l���擾����
//=============================================================================
int CEnemy::GetAI_MassEva(void)
{
	return m_nAI_MassEva;
}

//=============================================================================
// �ǂꂭ�炢�}�X���ړ��������𐔂���J�E���^�[���擾����
//=============================================================================
int CEnemy::GetAI_MassCount(void)
{
	return m_nAI_MassCount;
}

//=============================================================================
// �������ɓ��������ǂ����𔻒肷��ۂ̃����_���]���l���擾����
//=============================================================================
int CEnemy::GetAI_DownEva(void)
{
	return m_nAI_DownEva;
}

//=============================================================================
// �������ɓ����Ȃ������񐔂𐔂���J�E���^�[���擾����
//=============================================================================
int CEnemy::GetAI_NotDownCount(void)
{
	return m_nAI_NotDownCount;
}


//*****************************************************************************
// CEnemyNormal�̏���
//*****************************************************************************
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnemyNormal::CEnemyNormal(int nPriority, OBJTYPE objType) : CEnemy(nPriority, objType)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemyNormal::~CEnemyNormal()
{

}

//=============================================================================
// ��������
//=============================================================================
CEnemyNormal *CEnemyNormal::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority)
{
	CEnemyNormal *pEnemyNormal = NULL;

	if (pEnemyNormal == NULL)
	{// NULL�̏ꍇ
		pEnemyNormal = new CEnemyNormal(nPriority);
		if (pEnemyNormal != NULL)
		{// NULL�łȂ��ꍇ
			pEnemyNormal->SetPos(pos);
			pEnemyNormal->SetRot(rot);
			pEnemyNormal->SetType((CEnemy::TYPE)nType);
			pEnemyNormal->SetColRange(D3DXVECTOR3(70.0f, 70.0f, 70.0f));
			pEnemyNormal->SetReturnFlag(true);
			pEnemyNormal->Init();
		}
	}
	return pEnemyNormal;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemyNormal::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CEnemy::Init()))
	{
		return E_FAIL;
	}

	// �G�̎�ނ�ݒ�
	SetType(TYPE_NORMAL);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemyNormal::Uninit(void)
{
	CEnemy::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemyNormal::Update(void)
{
	CEnemy::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemyNormal::Draw(void)
{
	CEnemy::Draw();
}


//*****************************************************************************
// CEnemyArmore�̏���
//*****************************************************************************
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnemyArmore::CEnemyArmore(int nPriority, OBJTYPE objType) : CEnemy(nPriority, objType)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemyArmore::~CEnemyArmore()
{

}

//=============================================================================
// ��������
//=============================================================================
CEnemyArmore *CEnemyArmore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority)
{
	CEnemyArmore *pEnemyArmore = NULL;

	if (pEnemyArmore == NULL)
	{// NULL�̏ꍇ
		pEnemyArmore = new CEnemyArmore(nPriority);
		if (pEnemyArmore != NULL)
		{// NULL�łȂ��ꍇ
			pEnemyArmore->SetPos(pos);
			pEnemyArmore->SetRot(rot);
			pEnemyArmore->SetType((CEnemy::TYPE)nType);
			pEnemyArmore->SetColRange(D3DXVECTOR3(70.0f, 70.0f, 70.0f));
			pEnemyArmore->SetReturnFlag(true);
			pEnemyArmore->Init();
		}
	}
	return pEnemyArmore;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemyArmore::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CEnemy::Init()))
	{
		return E_FAIL;
	}

	// �G�̎�ނ�ݒ�
	SetType(TYPE_ARMORE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemyArmore::Uninit(void)
{
	CEnemy::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemyArmore::Update(void)
{
	CEnemy::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemyArmore::Draw(void)
{
	CEnemy::Draw();
}


//*****************************************************************************
// CEnemyFast�̏���
//*****************************************************************************
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnemyFast::CEnemyFast(int nPriority, OBJTYPE objType) : CEnemy(nPriority, objType)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemyFast::~CEnemyFast()
{

}

//=============================================================================
// ��������
//=============================================================================
CEnemyFast *CEnemyFast::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority)
{
	CEnemyFast *pEnemyFast = NULL;

	if (pEnemyFast == NULL)
	{// NULL�̏ꍇ
		pEnemyFast = new CEnemyFast(nPriority);
		if (pEnemyFast != NULL)
		{// NULL�łȂ��ꍇ
			pEnemyFast->SetPos(pos);
			pEnemyFast->SetRot(rot);
			pEnemyFast->SetType((CEnemy::TYPE)nType);
			pEnemyFast->SetColRange(D3DXVECTOR3(70.0f, 70.0f, 70.0f));
			pEnemyFast->SetReturnFlag(true);
			pEnemyFast->Init();
		}
	}
	return pEnemyFast;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemyFast::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CEnemy::Init()))
	{
		return E_FAIL;
	}

	// �G�̎�ނ�ݒ�
	SetType(TYPE_FAST);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemyFast::Uninit(void)
{
	CEnemy::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemyFast::Update(void)
{
	CEnemy::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemyFast::Draw(void)
{
	CEnemy::Draw();
}


//*****************************************************************************
// CEnemyHeavy�̏���
//*****************************************************************************
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnemyHeavy::CEnemyHeavy(int nPriority, OBJTYPE objType) : CEnemy(nPriority, objType)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemyHeavy::~CEnemyHeavy()
{

}

//=============================================================================
// ��������
//=============================================================================
CEnemyHeavy *CEnemyHeavy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority)
{
	CEnemyHeavy *pEnemyHeavy = NULL;

	if (pEnemyHeavy == NULL)
	{// NULL�̏ꍇ
		pEnemyHeavy = new CEnemyHeavy(nPriority);
		if (pEnemyHeavy != NULL)
		{// NULL�łȂ��ꍇ
			pEnemyHeavy->SetPos(pos);
			pEnemyHeavy->SetRot(rot);
			pEnemyHeavy->SetType((CEnemy::TYPE)nType);
			pEnemyHeavy->SetColRange(D3DXVECTOR3(70.0f, 70.0f, 70.0f));
			pEnemyHeavy->SetReturnFlag(true);
			pEnemyHeavy->Init();
		}
	}
	return pEnemyHeavy;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemyHeavy::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CEnemy::Init()))
	{
		return E_FAIL;
	}

	// �G�̎�ނ�ݒ�
	SetType(TYPE_HEAVY);

	// �̗͂�ݒ�
	SetLife(ENEMY_HEAVY_LIFE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemyHeavy::Uninit(void)
{
	CEnemy::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemyHeavy::Update(void)
{
	CEnemy::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemyHeavy::Draw(void)
{
	CEnemy::Draw();
}