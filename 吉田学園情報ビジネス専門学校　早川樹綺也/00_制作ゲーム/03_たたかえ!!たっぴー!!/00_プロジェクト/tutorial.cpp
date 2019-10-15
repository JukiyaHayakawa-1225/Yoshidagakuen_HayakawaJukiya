//=============================================================================
//
// �`���[�g���A������ [tutorial.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "tutorial.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "cameraManager.h"
#include "textureManager.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "input.h"
#include "fade.h"
#include "scene2D.h"
#include "map.h"
#include "respawn.h"
#include "server.h"
#include "player.h"
#include "playerManager.h"
#include "charaselect.h"
#include "characterManager.h"
#include "effectManager.h"
#include "bullet.h"
#include "modelcreate.h"
#include "title.h"
#include "sound.h"
#include "debugproc.h"

#include "goalCylinder.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TUTORIAL_SYSTEM_FILENAME        "data/TEXT/MODE/tutorial.ini"  // �������Ɏg�p����V�X�e���t�@�C����
#define TUTORIAL_PLAYER_RESPAWN_TIMING  (180)                          // �v���C���[�����X�|�[������܂ł̎���
#define TUTORIAL_MAPSTAGE_MAX           (3)                            // �`���[�g���A���Ŏg�p����}�b�v�̍ő吔
#define TUTORIAL_END_TIME               (180)                          // �I����ԂɂȂ��Ă����Ԃ��ς��܂ł̎���
#define TUTORIAL_BGM_IDX                (2)                            // �`���[�g���A���ōĐ�����BGM�̔ԍ�
#define TUTORIAL_SE_SELECT_IDX          (8)                            // ���ڂ�I�����Ă��鎞��SE�̉��ԍ�
#define TUTORIAL_SE_DECIDE_IDX          (9)                            // ����{�^���������ꂽ����SE�̉��ԍ�

// �I����Ԏ��̃J�����p�}�N��
#define TUTORIAL_END_CAMERA_POSY        (130.0f)                       // �J�����̍���
#define TUTORIAL_END_CAMERA_LENGTH      (240.0f)                       // ���_�����_�Ԃ̋���
#define TUTORIAL_END_CAMERA_MOVING      (0.09f)                        // �ړI�̈ʒu�܂ňړ�����{��

// �}�b�v�X�V���ɏ������I�u�W�F�N�g(����p�̃}�N���Ȃ̂ł����ɒǉ�)
#define TUTORIAL_DELETE_OBJECT (pScene->GetObjType() == CScene::OBJTYPE_ITEM || pScene->GetObjType() == CScene::OBJTYPE_BULLET || pScene->GetObjType() == CScene::OBJTYPE_PARTICLE || pScene->GetObjType() == CScene::OBJTYPE_PAREMITTER || pScene->GetObjType() == CScene::OBJTYPE_RINGEMITTER || pScene->GetObjType() == CScene::OBJTYPE_EMITTER || pScene->GetObjType() == CScene::OBJTYPE_BLOSSOMS)

// �X�L�b�v���S�������p
#define TUTORIAL_SKIPLOGO_POS_INI              (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 3.0f, 0.0f))
#define TUTORIAL_SKIPLOGO_COL_INI              (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TUTORIAL_SKIPLOGO_WIDTH_INI            (300.0f)
#define TUTORIAL_SKIPLOGO_HEIGHT_INI           (90.0f)

// �X�L�b�v�`�F�b�N���S�������p
// �P��
#define TUTORIAL_SKIPCHECKLOGO_0_POS_INI       (D3DXVECTOR3(SCREEN_WIDTH / 6.0f, SCREEN_HEIGHT / 3.0f * 2.0f, 0.0f))
#define TUTORIAL_SKIPCHECKLOGO_0_COL_NONE      (D3DXCOLOR(0.2f,0.2f,0.2f,1.0f))
#define TUTORIAL_SKIPCHECKLOGO_0_WIDTH_NONE    (180.0f)
#define TUTORIAL_SKIPCHECKLOGO_0_HEIGHT_NONE   (60.0f)
#define TUTORIAL_SKIPCHECKLOGO_0_COL_SELECT    (D3DXCOLOR(0.9f,0.9f,0.9f,1.0f))
#define TUTORIAL_SKIPCHECKLOGO_0_WIDTH_SELECT  (200.0f)
#define TUTORIAL_SKIPCHECKLOGO_0_HEIGHT_SELECT (80.0f)

// �Q��
#define TUTORIAL_SKIPCHECKLOGO_1_POS_INI       (D3DXVECTOR3(SCREEN_WIDTH / 6.0f * 4.0f, SCREEN_HEIGHT / 3.0f * 2.0f, 0.0f))
#define TUTORIAL_SKIPCHECKLOGO_1_COL_NONE      (D3DXCOLOR(0.2f,0.2f,0.2f,1.0f))
#define TUTORIAL_SKIPCHECKLOGO_1_WIDTH_NONE    (180.0f)
#define TUTORIAL_SKIPCHECKLOGO_1_HEIGHT_NONE   (60.0f)
#define TUTORIAL_SKIPCHECKLOGO_1_COL_SELECT    (D3DXCOLOR(0.9f,0.9f,0.9f,1.0f))
#define TUTORIAL_SKIPCHECKLOGO_1_WIDTH_SELECT  (200.0f)
#define TUTORIAL_SKIPCHECKLOGO_1_HEIGHT_SELECT (80.0f)

// ������@�\���p�|���S���������p
#define TUTORIAL_OPEINFO_POS_INI               (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 78.0f, 0.0f))
#define TUTORIAL_OPEINFO_COL_INI               (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TUTORIAL_OPEINFO_WIDTH_INI             (220.0f)
#define TUTORIAL_OPEINFO_HEIGHT_INI            (80.0f)
#define TUTORIAL_OPEINFO_TEXIDX                (3)
#define TUTORIAL_OPEINFO_PRIORITY              (7)

// �u���b�N���󂵂����X�|���X�\���p�|���S���������p
#define TUTORIAL_BREAKBLOCKINFO_POS_INI        (D3DXVECTOR3(145.0f, 210.0f, 0.0f))
#define TUTORIAL_BREAKBLOCKINFO_COL_INI        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TUTORIAL_BREAKBLOCKINFO_WIDTH_INI      (145.0f)
#define TUTORIAL_BREAKBLOCKINFO_HEIGHT_INI     (80.0f)
#define TUTORIAL_BREAKBLOCKINFO_TEXIDX         (4)
#define TUTORIAL_BREAKBLOCKINFO_PRIORITY       (7)
#define TUTORIAL_BREAKBLOCKINFO_DISP           (180)
#define TUTORIAL_BREAKBLOCKINFO_ALPHACUT       (0.02f)

// �v���C���[�ɒe�������������̃��X�|���X�\���p�|���S���������p
#define TUTORIAL_ATTACKPLAYERINFO_POS_INI      (D3DXVECTOR3(SCREEN_WIDTH - 145.0f, 390.0f, 0.0f))
#define TUTORIAL_ATTACKPLAYERINFO_COL_INI      (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TUTORIAL_ATTACKPLAYERINFO_WIDTH_INI    (145.0f)
#define TUTORIAL_ATTACKPLAYERINFO_HEIGHT_INI   (80.0f)
#define TUTORIAL_ATTACKPLAYERINFO_TEXIDX       (5)
#define TUTORIAL_ATTACKPLAYERINFO_PRIORITY     (7)
#define TUTORIAL_ATTACKPLAYERINFO_DISP         (180)
#define TUTORIAL_ATTACKPLAYERINFO_ALPHACUT     (0.02f)

// �S�[�����߂����X�|���X�\���p�|���S���������p
#define TUTORIAL_NEARGOALINFO_POS_INI          (D3DXVECTOR3(145.0f, 560.0f, 0.0f))
#define TUTORIAL_NEARGOALINFO_COL_INI          (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TUTORIAL_NEARGOALINFO_WIDTH_INI        (145.0f)
#define TUTORIAL_NEARGOALINFO_HEIGHT_INI       (80.0f)
#define TUTORIAL_NEARGOALINFO_TEXIDX           (6)
#define TUTORIAL_NEARGOALINFO_PRIORITY         (7)
#define TUTORIAL_NEARGOALINFO_DISP             (180)
#define TUTORIAL_NEARGOALINFO_ALPHACUT         (0.02f)

// �l�ǂݍ��ݗp�̃p�X
// �e�N�X�`���p
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// ���f���p
#define NUM_MODEL "NUM_MODEL = "
#define MODEL_FILENAME "MODEL_FILENAME = "

// �G�t�F�N�g�p
#define EFFECT_FILENAME "EFFECT_FILENAME = "

// �v���C���[�f�[�^�p
#define PLAYERDATA_FILENAME "PLAYERDATA_FILENAME = "

// �e�p
#define BULLET_MODEL_IDX "BULLET_MODEL_IDX = "

// �}�b�v�p
#define NUM_MAP "NUM_MAP = "

// �X�L�b�v���S�p
#define SKIPLOGOSET "SKIPLOGOSET"
#define END_SKIPLOGOSET "END_SKIPLOGOSET"
#define TEX_IDX "TEX_IDX = "
#define POS "POS = "
#define COL "COL = "
#define WIDTH "WIDTH = "
#define HEIGHT "HEIGHT = "

// �X�L�b�v�`�F�b�N���S�p
#define SKIPCHECKLOGOSET "SKIPCHECKLOGOSET"
#define END_SKIPCHECKLOGOSET "END_SKIPCHECKLOGOSET"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �`���[�g���A���̃R���X�g���N�^
//=============================================================================
CTutorial::CTutorial()
{

}

//=============================================================================
// �`���[�g���A���̃f�X�g���N�^
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// �`���[�g���A���̐�������
//=============================================================================
CTutorial *CTutorial::Create()
{
	CTutorial *pTutorial = NULL;
	if (pTutorial == NULL)
	{
		pTutorial = new CTutorial;
		if (pTutorial != NULL)
		{
			pTutorial->Init();
		}
	}

	return pTutorial;
}

//=============================================================================
// �`���[�g���A���̏���������
//=============================================================================
HRESULT CTutorial::Init(void)
{
	// �ϐ�������������
	ClearVariable();

	// �V�X�e���̏�����
	LoadSystem();

	// �J�����̐���
	CreateCamera();

	return S_OK;
}

//=============================================================================
// �`���[�g���A���̏I������
//=============================================================================
void CTutorial::Uninit(void)
{
	// ���ʂ̏I������
	CBasemode::Uninit();

	// �e��N���X�̊J��
	ReleaseSkipLogo();
	ReleaseSkipCheckLogo();
	ReleaseMapFilePointer();
	ReleasePlayer();
	ReleasePlayerManager();
	ReleaseOpeInfo();
	ReleaseBreakBlockInfo();
	ReleaseAttackPlayerInfo();
	ReleaseNearGoalInfo();

	// �S�ẴI�u�W�F�N�g�J��
	CScene::ReleaseAll();

	// BGM���~
	CManager::GetSound()->StopSound(TUTORIAL_BGM_IDX);
}

//=============================================================================
// �`���[�g���A���̍X�V����
//=============================================================================
void CTutorial::Update(void)
{
	CDebugProc::Print(1, "�`���[�g���A�����\n");

	// ���[�J��1P�v���C�Ȃ�΂�����l�̃v���C���[�������Ă���
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{
		if (m_pPlayer[MAX_NUM_PLAYER - 1] != NULL)
		{
			m_pPlayer[MAX_NUM_PLAYER - 1]->Uninit();
			m_pPlayer[MAX_NUM_PLAYER - 1] = NULL;
		}
	}

	// ���X�|���X�|���S���̊m�F
	CheckBreakBlockInfo();
	CheckAttackPlayerInfo();
	CheckNearGoalInfo();

	// �S�Ă̍X�V����
	CScene::UpdateAll();

	// ��Ԃɂ���ď����킯
	switch (m_State)
	{
	case STATE_SKIP_CHECK:
		SkipCheckUpdate();
		break;
	case STATE_SKIP_END:
		SkipEndUpdate();
		break;
	case STATE_NORMAL:
		NormalUpdate();
		break;
	case STATE_GOAL:
		GoalUpdate();
		break;
	case STATE_MAP_CHANGE:
		MapChangeUpdate();
		break;
	case STATE_END:
		EndUpdate();
		break;
	}


	// �v���C���[�����X�|�[�������邩�`�F�b�N
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P && m_State != STATE_SKIP_CHECK)
	{// ���[�J��1P�v���C�Ȃ��
		CheckPlayerResSpawn(0);
	}
	else if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL2P && m_State != STATE_SKIP_CHECK)
	{// ���[�J��2P�v���C�Ȃ��
		for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
		{
			CheckPlayerResSpawn(nCntPlayer);
		}
	}
	else if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P && m_State != STATE_SKIP_CHECK)
	{// �I�����C��2P�v���C�Ȃ��
		CheckPlayerResSpawn(CManager::GetClient()->GetClientId());
	}

	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{
		// �T�[�o�[�֑���f�[�^��ݒ肷��
		SetDataToServer();

		// �N���C�A���g�̍X�V����
		CManager::ClientUpdate();

		// �T�[�o�[���瑗��ꂽ�f�[�^��ݒ肷��
		GetDataFromServer();
	}

	// �f�[�^�N���A
	m_bDeletePlayerFlag = false;
	m_nNumDeleteEnemy = 0;
	strcpy(m_aDeleteEnemy, "\0");
	m_nNumDeleteBlock = 0;
	strcpy(m_aDeleteBlock, "\0");
	m_bHitBulletFlag = false;
}

//=============================================================================
// �`���[�g���A���̕`�揈��
//=============================================================================
void CTutorial::Draw(void)
{
	// �J�����̐ݒ�
	if (GetCameraManager() != NULL)
	{
		GetCameraManager()->BindCamera();
	}

	// �V�[���N���X�̕`�揈��
	CScene::DrawAll();
}


//*****************************************************************************
//
// �T�[�o�[�̍X�V�p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̏����u���b�N�̃f�[�^��ݒ�
//=============================================================================
void CTutorial::DeleteBlock(const int nIdx)
{
	m_nNumDeleteBlock++;
	char aData[64] = "\0";
	sprintf(aData, "%d", nIdx);
	strcat(m_aDeleteBlock, aData);
	strcat(m_aDeleteBlock, " ");
}

//=============================================================================
// �Q�[���̏����G�̃f�[�^��ݒ�
//=============================================================================
void CTutorial::DeleteEnemy(const int nIdx)
{
	m_nNumDeleteEnemy++;
	char aData[64] = "\0";
	sprintf(aData, "%d", nIdx);
	strcat(m_aDeleteEnemy, aData);
	strcat(m_aDeleteEnemy, " ");
}

//=============================================================================
// �Q�[���̃v���C���[����ɂ��鏈��
//=============================================================================
void CTutorial::DeletePlayer(CPlayer *pPlayer, const int nIdx)
{
	SetPlayer(pPlayer, nIdx);

	// �z�X�g����Ȃ�������
	if (CManager::GetClient() == NULL)return;
	if (CManager::GetClient()->GetClientId() == 0 && nIdx != 0)
	{
		m_bDeletePlayerFlag = true;
	}
}

//=============================================================================
// �Q�[���̑���v���C���[�̒e�������������ǂ����ݒ肷��
//=============================================================================
void CTutorial::HitBullet(void)
{
	m_bHitBulletFlag = true;
}


//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���f�[�^��ݒ肷�鏈��
//=============================================================================
void CTutorial::SetDataToServer(void)
{
	if (CManager::GetClient() == NULL || CManager::GetClient()->GetConnected() == false)return;

	// �v���C���[�̃f�[�^��ݒ�
	SetDataToServerFromPlayer();

	if (CManager::GetClient()->GetClientId() == 0)
	{// �������z�X�g�Ȃ�
	    // �Q�[���̃f�[�^��ݒ�
		SetDataToServerFromGame();
		// �G�̃f�[�^��ݒ�
		SetDataToServerFromEnemy();
		// �G�̒e�̃f�[�^��ݒ�
		SetDataToServerFromEnemyBullet();
		// �����u���b�N�̃f�[�^��ݒ�
		SetDataToServerFromDeleteBlock();
	}

	// �v���C���[�̒e����ݒ�
	SetDataToServerFromPlayerBullet();

	if (CManager::GetClient()->GetClientId() != 0)
	{// �������z�X�g�łȂ��Ȃ�
	 // �����u���b�N�̃f�[�^��ݒ�
		SetDataToServerFromDeleteBlock();

		// �����G�̃f�[�^��ݒ�
		SetDataToServerFromDeleteEnemy();
	}

	// �e�ɓ����������ǂ����ݒ�
	SetDataToServerFromHitBullet();
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���v���C���[�f�[�^��ݒ肷�鏈��
//=============================================================================
void CTutorial::SetDataToServerFromPlayer(void)
{
	// �v���C���[�̐l����ݒ�
	if (m_pPlayer[CManager::GetClient()->GetClientId()] == NULL)
	{
		CManager::GetClient()->Print("0");
		CManager::GetClient()->Print(" ");

		// �v���C���[�̎c�@����ݒ�
		CManager::GetClient()->Print("0");
		CManager::GetClient()->Print(" ");

		if (CManager::GetClient()->GetClientId() == 0)
		{
			CManager::GetClient()->Print("%d", (int)m_bDeletePlayerFlag);
			CManager::GetClient()->Print(" ");
		}
		return;
	}
	else if (m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
	{
		CManager::GetClient()->Print("1");
		CManager::GetClient()->Print(" ");

		// �v���C���[�̎c�@����ݒ�
		CManager::GetClient()->Print("0");
		CManager::GetClient()->Print(" ");

		// �v���C���[�̍��W��ݒ�
		D3DXVECTOR3 PlayerPos = m_pPlayer[CManager::GetClient()->GetClientId()]->GetPos();
		CManager::GetClient()->Print("%.1f %.1f %.1f", PlayerPos.x, PlayerPos.y, PlayerPos.z);
		CManager::GetClient()->Print(" ");

		// �v���C���[�̌�����ݒ�
		D3DXVECTOR3 PlayerRot = m_pPlayer[CManager::GetClient()->GetClientId()]->GetRot();
		CManager::GetClient()->Print("%.1f %.1f %.1f", PlayerRot.x, PlayerRot.y, PlayerRot.z);
		CManager::GetClient()->Print(" ");

		// �v���C���[�̏�Ԃ�ݒ�
		int nPlayerState = m_pPlayer[CManager::GetClient()->GetClientId()]->GetState();
		CManager::GetClient()->Print("%d", nPlayerState);
		CManager::GetClient()->Print(" ");

		// �v���C���[�����G��Ԃ��ǂ�����ݒ�
		int nPlayerHelmet = m_pPlayer[CManager::GetClient()->GetClientId()]->GetHelmet();
		CManager::GetClient()->Print("%d", nPlayerHelmet);
		CManager::GetClient()->Print(" ");
	}

	// �z�X�g��������
	if (CManager::GetClient()->GetClientId() == 0)
	{
		CManager::GetClient()->Print("%d", (int)m_bDeletePlayerFlag);
		CManager::GetClient()->Print(" ");
	}
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���Q�[���f�[�^��ݒ肷�鏈��
//=============================================================================
void CTutorial::SetDataToServerFromGame(void)
{
	// ��Ԃ�ݒ�
	CManager::GetClient()->Print("%d", m_State);
	CManager::GetClient()->Print(" ");

	// �}�b�v�̔ԍ���ݒ�
	CManager::GetClient()->Print("%d", m_nMapIdx);
	CManager::GetClient()->Print(" ");
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���G�f�[�^��ݒ肷�鏈��
//=============================================================================
void CTutorial::SetDataToServerFromEnemy(void)
{
	int nNumEnemy = 0;
	char aData[64] = "\0";
	char aEnemyData[MAX_SERVER_DATA] = "\0";

	// �G�̃f�[�^��ݒ�
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < ENEMY_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// �G�N���X������
				strcat(aEnemyData, GetDataToEnemy((CEnemy*)pScene, aData));
				strcpy(aData, "\0");
				nNumEnemy++;
			}
			pScene = pSceneNext;
		}
	}

	// �G�̐���ݒ�
	CManager::GetClient()->Print("%d", nNumEnemy);
	CManager::GetClient()->Print(" ");

	// �G�̃f�[�^��ݒ�
	CManager::GetClient()->Print("%s", aEnemyData);
}

//=============================================================================
// �G�̃f�[�^�𕶎���ɕϊ����鏈��
//=============================================================================
char *CTutorial::GetDataToEnemy(CEnemy *pEnemy, char *pStr)
{
	char aData[64] = "\0";

	// �G�̍��W��ݒ�
	D3DXVECTOR3 EnemyPos = pEnemy->GetPos();
	sprintf(aData, "%.1f %.1f %.1f", EnemyPos.x, EnemyPos.y, EnemyPos.z);
	strcat(pStr, aData);
	strcat(pStr, " ");

	// �G�̌�����ݒ�
	D3DXVECTOR3 EnemyRot = pEnemy->GetRot();
	sprintf(aData, "%.1f %.1f %.1f", EnemyRot.x, EnemyRot.y, EnemyRot.z);
	strcat(pStr, aData);
	strcat(pStr, " ");

	// �G�̔ԍ�
	int nEnemyType = pEnemy->GetType();
	sprintf(aData, "%d", nEnemyType);
	strcat(pStr, aData);
	strcat(pStr, " ");

	// �G�̔ԍ�
	int nEnemyIdx = pEnemy->GetIdx();
	sprintf(aData, "%d", nEnemyIdx);
	strcat(pStr, aData);
	strcat(pStr, " ");

	return pStr;
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���v���C���[�̒e�f�[�^��ݒ肷�鏈��
//=============================================================================
void CTutorial::SetDataToServerFromPlayerBullet(void)
{
	int nNumBullet = 0;
	char aData[64] = "\0";
	char aBulletData[MAX_SERVER_DATA] = "\0";

	// ���݂̃v���C���[�̒e�̐���ݒ�
	if (CManager::GetClient()->GetClientId() == 0)
	{
		nNumBullet = CBulletPlayer::GetNumAll_0();
	}
	else
	{
		nNumBullet = CBulletPlayer::GetNumAll_1();
	}

	// �e�̐���ݒ�
	CManager::GetClient()->Print("%d", nNumBullet);
	CManager::GetClient()->Print(" ");
	if (nNumBullet < 1) return;

	// �e�̃f�[�^��ݒ肷��
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < BULLET_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET)
			{// �e�N���X������
				strcat(aBulletData, GetDataToPlayerBullet((CBullet*)pScene, aData));
				strcpy(aData, "\0");
			}
			pScene = pSceneNext;
		}
	}

	// �e�̃f�[�^��ݒ�
	CManager::GetClient()->Print("%s", aBulletData);
}

//=============================================================================
// �v���C���[�̒e�̃f�[�^�𕶎���ɕϊ����鏈��
//=============================================================================
char *CTutorial::GetDataToPlayerBullet(CBullet *pBullet, char *pStr)
{
	if (pBullet->GetType() == CManager::GetClient()->GetClientId())
	{// �����̃v���C���[���������e�̏�����������
		char aData[64] = "\0";

		// �e�̍��W��ݒ�
		D3DXVECTOR3 BulletPos = pBullet->GetPos();
		sprintf(aData, "%.1f %.1f %.1f", BulletPos.x, BulletPos.y, BulletPos.z);
		strcat(pStr, aData);
		strcat(pStr, " ");

		// �e�̔ԍ���ݒ�
		int nBulletIdx = pBullet->GetIdx();
		sprintf(aData, "%d", nBulletIdx);
		strcat(pStr, aData);
		strcat(pStr, " ");

		// �e�̎�ނ�ݒ�
		int nBulletType = pBullet->GetType();
		sprintf(aData, "%d", nBulletType);
		strcat(pStr, aData);
		strcat(pStr, " ");
	}

	return pStr;
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���G�̒e�f�[�^��ݒ肷�鏈��
//=============================================================================
void CTutorial::SetDataToServerFromEnemyBullet(void)
{
	int nNumBullet = 0;
	char aData[64] = "\0";
	char aBulletData[MAX_SERVER_DATA] = "\0";

	// �G�̒e�̐���ݒ�
	nNumBullet = CBulletEnemy::GetNumAll();

	// �e�̐���ݒ�
	CManager::GetClient()->Print("%d", nNumBullet);
	CManager::GetClient()->Print(" ");
	if (nNumBullet < 1) return;

	// �e�̃f�[�^��ݒ肷��
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < BULLET_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET)
			{// �e�N���X������
				strcat(aBulletData, GetDataToEnemyBullet((CBullet*)pScene, aData));
				strcpy(aData, "\0");
			}
			pScene = pSceneNext;
		}
	}

	// �e�̃f�[�^��ݒ�
	CManager::GetClient()->Print("%s", aBulletData);
}

//=============================================================================
// �G�̒e�̃f�[�^�𕶎���ɕϊ����鏈��
//=============================================================================
char *CTutorial::GetDataToEnemyBullet(CBullet *pBullet, char *pStr)
{
	if (pBullet->GetType() == CBullet::TYPE_ENEMY)
	{// �G�̒e�f�[�^��ݒ肷��
		char aData[64] = "\0";

		// �e�̍��W��ݒ�
		D3DXVECTOR3 BulletPos = pBullet->GetPos();
		sprintf(aData, "%.1f %.1f %.1f", BulletPos.x, BulletPos.y, BulletPos.z);
		strcat(pStr, aData);
		strcat(pStr, " ");

		// �e�̔ԍ���ݒ�
		int nBulletIdx = pBullet->GetIdx();
		sprintf(aData, "%d", nBulletIdx);
		strcat(pStr, aData);
		strcat(pStr, " ");

		// �e�̎�ނ�ݒ�
		int nBulletType = pBullet->GetType();
		sprintf(aData, "%d", nBulletType);
		strcat(pStr, aData);
		strcat(pStr, " ");
	}

	return pStr;
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ�������u���b�N�̃f�[�^��ݒ肷�鏈��
//=============================================================================
void CTutorial::SetDataToServerFromDeleteBlock(void)
{
	CManager::GetClient()->Print("%d", m_nNumDeleteBlock);
	CManager::GetClient()->Print(" ");
	if (m_nNumDeleteBlock > 0)
	{
		CManager::GetClient()->Print("%s", m_aDeleteBlock);
		CManager::GetClient()->Print(" ");
	}
	m_nNumDeleteBlock = 0;
	strcpy(m_aDeleteBlock, "\0");
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ�������G�̃f�[�^��ݒ肷�鏈��
//=============================================================================
void CTutorial::SetDataToServerFromDeleteEnemy(void)
{
	CManager::GetClient()->Print("%d", m_nNumDeleteEnemy);
	CManager::GetClient()->Print(" ");
	if (m_nNumDeleteEnemy > 0)
	{
		CManager::GetClient()->Print("%s", m_aDeleteEnemy);
		CManager::GetClient()->Print(" ");
	}
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���|�����G�̐���ݒ肷�鏈��
//=============================================================================
void CTutorial::SetDataToServerFromHitBullet(void)
{
	CManager::GetClient()->Print("%d", (int)m_bHitBulletFlag);
	CManager::GetClient()->Print(" ");
}

//=============================================================================
// �Q�[���̃T�[�o�[���瑗��ꂽ�f�[�^��ݒ肷�鏈��
//=============================================================================
void CTutorial::GetDataFromServer(void)
{
	// ����ꂽ���b�Z�[�W���擾
	if (CManager::GetClient() == NULL)return;
	char *pStr = CManager::GetClient()->GetReceiveMessage();
	if (pStr == NULL || CManager::GetClient()->GetConnected() == false) return;
	if (*pStr == *"???") return;

	// �v���C���[�̃f�[�^��ݒ�
	pStr = SetDataToPlayerFromServer(pStr);

	if (CManager::GetClient()->GetClientId() != 0)
	{// �z�X�g����Ȃ�������
	 // �Q�[���̏�Ԃ�ݒ�
		pStr = SetDataToGameFromServer(pStr);
		// �G�̃f�[�^��ݒ�
		pStr = SetDataToEnemyFromServer(pStr);
		// �G�̒e�̃f�[�^��ݒ�
		pStr = SetDataToEnemyBulletFromServer(pStr);
		// �����u���b�N�̃f�[�^��ݒ�
		pStr = SetDataToDeleteBlock(pStr);
	}

	// �v���C���[�̒e�̃f�[�^��ݒ�
	pStr = SetDataToPlayerBulletFromServer(pStr);

	if (CManager::GetClient()->GetClientId() == 0)
	{// �z�X�g��������
	 // �����u���b�N�̃f�[�^��ݒ�
		pStr = SetDataToDeleteBlock(pStr);

		// �����G�̃f�[�^��ݒ�
		pStr = SetDataToDeleteEnemy(pStr);
	}

	// �e���q�b�g�������ǂ����ݒ�
	pStr = SetDataToHitBullet(pStr);

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck();
}

//=============================================================================
// �Q�[���̃T�[�o�[���瑗��ꂽ�f�[�^���v���C���[�ɐݒ肷�鏈��
//=============================================================================
char *CTutorial::SetDataToPlayerFromServer(char *pStr)
{
	int nWord = 0;
	int nNumPlayer = 0;
	int nNumStock = 0;
	int nNumber = (CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER;

	// �v���C���[�̐l����ǂݎ��
	nNumPlayer = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �v���C���[�̎c�@����ǂݎ��
	nNumStock = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	if (nNumPlayer == 0)
	{// �v���C���[�����݂��Ȃ�
		if (m_pPlayer[nNumber] != NULL && m_State == STATE_NORMAL)
		{// �v���C���[�������Ă���
			m_pPlayer[nNumber]->Uninit();
			m_pPlayer[nNumber] = NULL;
		}
	}
	else if (nNumPlayer == 1)
	{// �v���C���[�����݂���
		CMap *pMap = GetMap();
		if (pMap == NULL)
		{
			// �v���C���[�̍��W��ǂݎ��
			float fGetPlayerPosX = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerPosY = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerPosZ = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// �v���C���[�̌�����ǂݎ��
			float fGetPlayerRotX = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerRotY = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerRotZ = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// �v���C���[�̏�Ԃ�ǂݎ��
			int nGetPlayerState = CFunctionLib::ReadInt(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// �v���C���[�����G��Ԃ��ǂ�����ǂݎ��
			bool bPlayerHelmet = CFunctionLib::ReadBool(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
		}
		else if (m_pPlayer[nNumber] == NULL && pMap != NULL)
		{// ���������m�ۂ���Ă��Ȃ�
			// �v���C���[�̍��W��ǂݎ��
			float fGetPlayerPosX = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerPosY = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerPosZ = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// �v���C���[�̌�����ǂݎ��
			float fGetPlayerRotX = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerRotY = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerRotZ = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// �v���C���[�̏�Ԃ�ǂݎ��
			int nGetPlayerState = CFunctionLib::ReadInt(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// �v���C���[�����G��Ԃ��ǂ�����ǂݎ��
			bool bPlayerHelmet = CFunctionLib::ReadBool(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			m_pPlayer[nNumber] = m_pPlayerManager[nNumber]->SetPlayer(D3DXVECTOR3(fGetPlayerPosX, fGetPlayerPosY, fGetPlayerPosZ), D3DXVECTOR3(fGetPlayerRotX, fGetPlayerRotY, fGetPlayerRotZ), nNumber);
		}
		else if (m_pPlayer[nNumber] != NULL)
		{// ���������m�ۂł��Ă���
		    // �v���C���[�̍��W��ǂݎ��
			float fGetPlayerPosX = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerPosY = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerPosZ = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// �v���C���[�̌�����ǂݎ��
			float fGetPlayerRotX = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerRotY = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerRotZ = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// �v���C���[�̏�Ԃ�ǂݎ��
			int nGetPlayerState = CFunctionLib::ReadInt(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// �v���C���[�����G��Ԃ��ǂ�����ǂݎ��
			bool bPlayerHelmet = CFunctionLib::ReadBool(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// �v���C���[�ɒl��ݒ�
			if (m_pPlayer[nNumber] != NULL)
			{
				m_pPlayer[nNumber]->SetPos(D3DXVECTOR3(fGetPlayerPosX, fGetPlayerPosY, fGetPlayerPosZ));
				m_pPlayer[nNumber]->SetRot(D3DXVECTOR3(fGetPlayerRotX, fGetPlayerRotY, fGetPlayerRotZ));
				m_pPlayer[nNumber]->SetState((CPlayer::STATE)nGetPlayerState);
			}
		}
	}


	// �z�X�g����Ȃ�������
	if (CManager::GetClient()->GetClientId() != 0)
	{
		bool bDeletePlayerFlag = CFunctionLib::ReadBool(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;

		if (bDeletePlayerFlag == true && m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
		{
			m_pPlayer[CManager::GetClient()->GetClientId()]->Uninit();
			m_pPlayer[CManager::GetClient()->GetClientId()] = NULL;
		}
	}

	return pStr;
}

//=============================================================================
// �Q�[���̃T�[�o�[���瑗��ꂽ�f�[�^���Q�[���ɐݒ肷�鏈��
//=============================================================================
char *CTutorial::SetDataToGameFromServer(char *pStr)
{
	int nGameState = CFunctionLib::ReadInt(pStr, "");
	int nWord = 0;
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	int nMapIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// ��Ԃ�ݒ�
	m_StateOld = m_State;
	m_State = (STATE)nGameState;

	// �}�b�v�̔ԍ���ݒ�
	m_nMapIdx = nMapIdx;

	// ��Ԃɂ���ď����킯
	if (m_State == STATE_NORMAL && m_StateOld == STATE_SKIP_CHECK)
	{// �}�b�v�X�V��Ԃ�������
		ReleaseSkipLogo();
		ReleaseSkipCheckLogo();
		CScene::DeathCheck();
		CreateMap();
		CreatePlayer();
		CreateOpeInfo();
		SetPlayerPosToSpawn();
	}

	return pStr;
}

//=============================================================================
// �Q�[���̃T�[�o�[���瑗��ꂽ�f�[�^��G�ɐݒ肷�鏈��
//=============================================================================
char *CTutorial::SetDataToEnemyFromServer(char *pStr)
{
	int nWord = 0;
	int nNumEnemy = 0;

	// ��������Ă���G�̐���ǂݎ��
	nNumEnemy = CFunctionLib::ReadInt(pStr, "");
	m_nNumEnemy = nNumEnemy;
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �����킹
	ReleaseEnemy(nNumEnemy);
	if (nNumEnemy <= 0) return pStr;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < ENEMY_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// �G�N���X������
				pStr = SetDataToEnemy((CEnemy*)pScene, pStr);
				nNumEnemy--;
			}
			pScene = pSceneNext;
		}
	}

	// ����Ȃ��G�͐V���ɐ������f�[�^��ݒ�
	for (int nCntEnemy = 0; nCntEnemy < nNumEnemy; nCntEnemy++)
	{
		pStr = SetDataToCreateEnemy(pStr);
	}

	return pStr;
}

//=============================================================================
// ��������f�[�^�ɕϊ����G�ɐݒ肷��
//=============================================================================
char *CTutorial::SetDataToEnemy(CEnemy *pEnemy, char *pStr)
{
	int nWord = 0;
	float fEnemyPosX = 0.0f;
	float fEnemyPosY = 0.0f;
	float fEnemyPosZ = 0.0f;
	float fEnemyRotX = 0.0f;
	float fEnemyRotY = 0.0f;
	float fEnemyRotZ = 0.0f;
	int nEnemyType = 0;
	int nEnemyIdx = 0;

	// �G�̍��W��ǂݎ��
	fEnemyPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�̌�����ǂݎ��
	fEnemyRotX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyRotY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyRotZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�̎�ޔԍ�
	nEnemyType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�̔ԍ�
	nEnemyIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�Ƀf�[�^��ݒ�
	pEnemy->SetPos(D3DXVECTOR3(fEnemyPosX, fEnemyPosY, fEnemyPosZ));
	pEnemy->SetRot(D3DXVECTOR3(fEnemyRotX, fEnemyRotY, fEnemyRotZ));
	pEnemy->SetIdx(nEnemyIdx);

	return pStr;
}

//=============================================================================
// ��������f�[�^�ɕϊ������������G�ɐݒ肷��
//=============================================================================
char *CTutorial::SetDataToCreateEnemy(char *pStr)
{
	int nWord = 0;
	float fEnemyPosX = 0.0f;
	float fEnemyPosY = 0.0f;
	float fEnemyPosZ = 0.0f;
	float fEnemyRotX = 0.0f;
	float fEnemyRotY = 0.0f;
	float fEnemyRotZ = 0.0f;
	int nEnemyType = 0;
	int nEnemyIdx = 0;

	// �G�̍��W��ǂݎ��
	fEnemyPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�̌�����ǂݎ��
	fEnemyRotX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyRotY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyRotZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�̎�ޔԍ�
	nEnemyType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�̔ԍ�
	nEnemyIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�𐶐�����(�����ɕK�v�ȃf�[�^��ݒ�)
	CEnemy_ListData *pEnemyData = GetMap()->GetEnemyListData(nEnemyIdx);
	D3DXVECTOR3 pos = D3DXVECTOR3(fEnemyPosX, fEnemyPosY, fEnemyPosZ);
	D3DXVECTOR3 rot = D3DXVECTOR3(fEnemyRotX, fEnemyRotY, fEnemyRotZ);
	CEnemy *pEnemy = m_pEnemyManager[nEnemyType]->SetEnemy(pos, rot, nEnemyType);

	if (pEnemy == NULL)return NULL;
	// �G�̃f�[�^��ݒ肷��
	if (pEnemyData->GetItem() == true)
	{
		pEnemy->SetItemData(pEnemyData->GetItemType());
	}
	pEnemy->SetAIData(pEnemyData->GetAI_BulletEva(), pEnemyData->GetAI_MassEva(), pEnemyData->GetAI_DownEva());
	pEnemy->SetIdx(nEnemyIdx);
	return pStr;
}

//=============================================================================
// �v���C���[�̒e�̐������킹�鏈��
//=============================================================================
void CTutorial::ReleaseEnemy(int nNumEnemy)
{
	// ���݂̓G�̐���ݒ�
	int nAllEnemy = 0;
	nAllEnemy = CEnemy::GetNumAll();
	if (nAllEnemy < nNumEnemy)return;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < ENEMY_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY && nAllEnemy > nNumEnemy)
			{// �e�N���X������
				ReleaseCheckEnemy((CEnemy*)pScene, &nAllEnemy);
			}
			pScene = pSceneNext;
		}
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck();
}

//=============================================================================
// �G��j�����邩�`�F�b�N���鏈��
//=============================================================================
void CTutorial::ReleaseCheckEnemy(CEnemy *pEnemy, int *pNumEnemy)
{
	pEnemy->Uninit();
	pEnemy = NULL;
	*pNumEnemy = *pNumEnemy - 1;
}

//=============================================================================
// �Q�[���̃T�[�o�[���瑗��ꂽ�f�[�^���v���C���[�̒e�ɐݒ肷�鏈��
//=============================================================================
char *CTutorial::SetDataToPlayerBulletFromServer(char *pStr)
{
	int nWord = 0;
	int nNumBullet = 0;

	// ��������Ă���e�̐���ǂݎ��
	nNumBullet = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �����킹
	ReleasePlayerBullet(nNumBullet);
	if (nNumBullet <= 0) return pStr;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < BULLET_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET)
			{// �e�N���X������
				pStr = SetDataToPlayerBullet((CBullet*)pScene, pStr, &nNumBullet);
			}
			pScene = pSceneNext;
		}
	}

	// ����Ȃ��e�͐V���ɐ������f�[�^��ݒ�
	for (int nCntBullet = 0; nCntBullet < nNumBullet; nCntBullet++)
	{
		pStr = SetDataToCreatePlayerBullet(pStr);
	}

	return pStr;
}

//=============================================================================
// �v���C���[�̒e�̐������킹�鏈��
//=============================================================================
void CTutorial::ReleasePlayerBullet(int nNumBullet)
{
	// ���݂̃v���C���[�̒e�̐���ݒ�
	int nAllBullet = 0;
	if (CManager::GetClient()->GetClientId() == 0)
	{
		nAllBullet = CBulletPlayer::GetNumAll_1();
	}
	else
	{
		nAllBullet = CBulletPlayer::GetNumAll_0();
	}
	if (nAllBullet < nNumBullet)return;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < BULLET_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET && nAllBullet > nNumBullet)
			{// �e�N���X������
				ReleaseCheckPlayerBullet((CBullet*)pScene, &nAllBullet);
			}
			pScene = pSceneNext;
		}
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck(BULLET_PRIORITY - 1);
}

//=============================================================================
// �v���C���[�̒e��j�����邩�`�F�b�N���鏈��
//=============================================================================
void CTutorial::ReleaseCheckPlayerBullet(CBullet *pBullet, int *pNumBullet)
{
	if (pBullet->GetType() != CBullet::TYPE_ENEMY &&
		pBullet->GetType() != CManager::GetClient()->GetClientId())
	{
		pBullet->Uninit();
		pBullet = NULL;
		*pNumBullet = *pNumBullet - 1;
	}
}

//=============================================================================
// ��������f�[�^�ɕϊ����v���C���[�̒e�ɐݒ肷��
//=============================================================================
char *CTutorial::SetDataToPlayerBullet(CBullet *pBullet, char *pStr, int *pNumBullet)
{
	if (pBullet->GetType() == CBullet::TYPE_ENEMY ||
		pBullet->GetType() == CManager::GetClient()->GetClientId()) return pStr;

	int nWord = 0;
	float fBulletPosX = 0.0f;
	float fBulletPosY = 0.0f;
	float fBulletPosZ = 0.0f;
	int nBulletIdx = 0;
	int nBulletType = 0;

	// �e�̍��W��ǂݎ��
	fBulletPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�̔ԍ���ǂݎ��
	nBulletIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�̎�ނ�ǂݎ��
	nBulletType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �v���C���[�̒e�Ƀf�[�^��ݒ�
	pBullet->SetPos(D3DXVECTOR3(fBulletPosX, fBulletPosY, fBulletPosZ));
	pBullet->SetIdx(nBulletIdx);
	pBullet->SetType((CBullet::TYPE)nBulletType);
	*pNumBullet = *pNumBullet - 1;

	return pStr;
}

//=============================================================================
// ��������f�[�^�ɕϊ������������v���C���[�̒e�ɐݒ肷��
//=============================================================================
char *CTutorial::SetDataToCreatePlayerBullet(char *pStr)
{
	int nWord = 0;
	float fBulletPosX = 0.0f;
	float fBulletPosY = 0.0f;
	float fBulletPosZ = 0.0f;
	int nBulletIdx = 0;
	int nBulletType = 0;

	// �e�̍��W��ǂݎ��
	fBulletPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�̔ԍ���ǂݎ��
	nBulletIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�̎�ނ�ǂݎ��
	nBulletType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�𐶐�����(�����ɕK�v�ȃf�[�^��ݒ�)
	CBulletPlayer *pBullet = CBulletPlayer::Create(D3DXVECTOR3(fBulletPosX, fBulletPosY, fBulletPosZ), INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3,
		(CBullet::TYPE)((CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER), m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]);
	pBullet->SetIdx(nBulletIdx);
	pBullet->SetType((CBullet::TYPE)nBulletType);

	return pStr;
}

//=============================================================================
// �Q�[���̃T�[�o�[���瑗��ꂽ�f�[�^��G�̒e�ɐݒ肷�鏈��
//=============================================================================
char *CTutorial::SetDataToEnemyBulletFromServer(char *pStr)
{
	int nWord = 0;
	int nNumBullet = 0;

	// ��������Ă���e�̐���ǂݎ��
	nNumBullet = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �����킹
	ReleaseEnemyBullet(nNumBullet);
	if (nNumBullet <= 0) return pStr;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < BULLET_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET && nNumBullet > 0)
			{// �e�N���X������
				pStr = SetDataToEnemyBullet((CBullet*)pScene, pStr, &nNumBullet);
			}
			pScene = pSceneNext;
		}
	}

	// ����Ȃ��e�͐V���ɐ������f�[�^��ݒ�
	for (int nCntBullet = 0; nCntBullet < nNumBullet; nCntBullet++)
	{
		pStr = SetDataToCreateEnemyBullet(pStr);
	}

	return pStr;
}

//=============================================================================
// �G�̒e�̐������킹�鏈��
//=============================================================================
void CTutorial::ReleaseEnemyBullet(int nNumBullet)
{
	// ���݂̓G�̒e�̐���ݒ�
	int nAllBullet = 0;
	nAllBullet = CBulletEnemy::GetNumAll();
	if (nAllBullet < nNumBullet)return;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < BULLET_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET && nAllBullet > nNumBullet)
			{// �e�N���X������
				ReleaseCheckEnemyBullet((CBullet*)pScene, &nAllBullet);
			}
			pScene = pSceneNext;
		}
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck(BULLET_PRIORITY - 1);
}

//=============================================================================
// �G�̒e��j�����邩�`�F�b�N���鏈��
//=============================================================================
void CTutorial::ReleaseCheckEnemyBullet(CBullet *pBullet, int *pNumBullet)
{
	if (pBullet->GetType() == CBullet::TYPE_ENEMY)
	{
		pBullet->Uninit();
		pBullet = NULL;
		*pNumBullet = *pNumBullet - 1;
	}
}

//=============================================================================
// ��������f�[�^�ɕϊ����e�ɐݒ肷��
//=============================================================================
char *CTutorial::SetDataToEnemyBullet(CBullet *pBullet, char *pStr, int *pNumBullet)
{
	if (pBullet->GetType() == CBullet::TYPE_ENEMY)
	{
		int nWord = 0;
		float fBulletPosX = 0.0f;
		float fBulletPosY = 0.0f;
		float fBulletPosZ = 0.0f;
		int nBulletIdx = 0;
		int nBulletType = 0;

		// �e�̍��W��ǂݎ��
		fBulletPosX = CFunctionLib::ReadFloat(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
		fBulletPosY = CFunctionLib::ReadFloat(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
		fBulletPosZ = CFunctionLib::ReadFloat(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;

		// �e�̔ԍ���ǂݎ��
		nBulletIdx = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;

		// �e�̎�ނ�ǂݎ��
		nBulletType = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;

		// �e�Ƀf�[�^��ݒ�
		pBullet->SetPos(D3DXVECTOR3(fBulletPosX, fBulletPosY, fBulletPosZ));
		pBullet->SetIdx(nBulletIdx);
		pBullet->SetType((CBullet::TYPE)nBulletType);
		*pNumBullet = *pNumBullet - 1;
	}

	return pStr;
}

//=============================================================================
// ��������f�[�^�ɕϊ������������G�̒e�ɐݒ肷��
//=============================================================================
char *CTutorial::SetDataToCreateEnemyBullet(char *pStr)
{
	int nWord = 0;
	float fBulletPosX = 0.0f;
	float fBulletPosY = 0.0f;
	float fBulletPosZ = 0.0f;
	int nBulletIdx = 0;
	int nBulletType = 0;

	// �e�̍��W��ǂݎ��
	fBulletPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�̔ԍ���ǂݎ��
	nBulletIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�̎�ނ�ǂݎ��
	nBulletType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�𐶐�����(�����ɕK�v�ȃf�[�^��ݒ�)
	CBulletEnemy *pBullet = CBulletEnemy::Create(D3DXVECTOR3(fBulletPosX, fBulletPosY, fBulletPosZ), INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, NULL);
	pBullet->SetIdx(nBulletIdx);
	pBullet->SetType((CBullet::TYPE)nBulletType);

	return pStr;
}

//=============================================================================
// �Q�[���̏����u���b�N�̃f�[�^��ݒ�
//=============================================================================
char *CTutorial::SetDataToDeleteBlock(char *pStr)
{
	int nWord = 0;
	int nNumDeleteBlock = 0;
	int *pDeleteIdx = NULL;

	// �����u���b�N�̐���ǂݎ��
	nNumDeleteBlock = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	if (nNumDeleteBlock <= 0) return pStr;

	// �������������������m��
	pDeleteIdx = new int[nNumDeleteBlock];
	for (int nCnt = 0; nCnt < nNumDeleteBlock; nCnt++)
	{
		pDeleteIdx[nCnt] = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
	}

	// �����������u���b�N���J������
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < BLOCK_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK && m_State == STATE_NORMAL)
			{// �u���b�N�N���X������
				ReleaseCheckBlock((CBlock*)pScene, pDeleteIdx, &nNumDeleteBlock);
			}
			pScene = pSceneNext;
		}
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck(BLOCK_PRIORITY - 1);

	// �������̊J��
	if (pDeleteIdx != NULL)
	{
		delete[] pDeleteIdx;
		pDeleteIdx = NULL;
	}

	return pStr;
}

//=============================================================================
// �Q�[���̃u���b�N��j�����邩�`�F�b�N���鏈��
//=============================================================================
void CTutorial::ReleaseCheckBlock(CBlock *pBlock, int *pDeleteIdx, int *nNumDeleteBlock)
{
	int nIdx = pBlock->GetIdx();
	for (int nCntDeleteNum = 0; nCntDeleteNum < *nNumDeleteBlock; nCntDeleteNum++)
	{// ���������J��Ԃ�
		if (nIdx == pDeleteIdx[nCntDeleteNum] && pBlock != NULL)
		{// �ԍ���v
			pBlock->Uninit();
			pBlock = NULL;
		}
	}
}

//=============================================================================
// �Q�[���̏����G�̃f�[�^��ݒ�
//=============================================================================
char *CTutorial::SetDataToDeleteEnemy(char *pStr)
{
	int nWord = 0;
	int nNumDeleteEnemy = 0;
	int nCntDelete = 0;
	int *pDeleteIdx = NULL;

	// �����G�̐���ǂݎ��
	nNumDeleteEnemy = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	if (nNumDeleteEnemy <= 0) return pStr;

	// �������������������m��
	pDeleteIdx = new int[nNumDeleteEnemy];
	for (int nCnt = 0; nCnt < nNumDeleteEnemy; nCnt++)
	{
		pDeleteIdx[nCnt] = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
	}

	// �����������G���J������
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < ENEMY_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY && m_State == STATE_NORMAL)
			{// �G�N���X������
				ReleaseCheckDeleteEnemy((CEnemy*)pScene, pDeleteIdx, &nNumDeleteEnemy);
			}
			pScene = pSceneNext;
		}
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck();

	// �������̊J��
	if (pDeleteIdx != NULL)
	{
		delete[] pDeleteIdx;
		pDeleteIdx = NULL;
	}

	return pStr;
}

//=============================================================================
// �Q�[���̓G��j�����邩�`�F�b�N���鏈��
//=============================================================================
void CTutorial::ReleaseCheckDeleteEnemy(CEnemy *pEnemy, int *pDeleteIdx, int *nNumDeleteEnemy)
{
	int nIdx = pEnemy->GetIdx();
	for (int nCntDeleteNum = 0; nCntDeleteNum < *nNumDeleteEnemy; nCntDeleteNum++)
	{// ���������J��Ԃ�
		if (nIdx == pDeleteIdx[nCntDeleteNum] && pEnemy != NULL)
		{// �ԍ���v
			pEnemy->Uninit();
			pEnemy = NULL;
		}
	}
}

//=============================================================================
// �Q�[���̑���v���C���[�̒e�ɓ����������ǂ����擾����
//=============================================================================
char *CTutorial::SetDataToHitBullet(char *pStr)
{
	// �����������ǂ����ǂݎ��
	m_bHitBulletFlag = CFunctionLib::ReadBool(pStr, "");
	int nWord = 0;
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	int nIdxClient = 0;
	CClient *pClient = CManager::GetClient();
	if (pClient != NULL)
	{
		nIdxClient = pClient->GetClientId();
	}

	if (m_bHitBulletFlag == true && m_pPlayer[nIdxClient] != NULL)
	{// �e�ɓ������Ă���
		if (m_pPlayer[nIdxClient]->GetState() != CPlayer::STATE_STOP)
		{// �X�g�b�v��Ԃł͂Ȃ�
			m_pPlayer[nIdxClient]->SetState(CPlayer::STATE_STOP);
			m_pPlayer[nIdxClient]->SetStateCounter(0);
		}
	}

	return pStr;
}



//*****************************************************************************
//
// �����p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �`���[�g���A���̃e�N�X�`���Ǌ��N���X��������
//=============================================================================
void CTutorial::CreateTexture(int nNumTex)
{
	CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
	SetTextureManager(pTextureManager);
}

//=============================================================================
// �`���[�g���A���̃J������������
//=============================================================================
void CTutorial::CreateCamera(void)
{
	CCameraManager *pCameraManager = CCameraManager::Create();
	if (pCameraManager == NULL) return;

	// �J�����𐶐�
	CCamera *pCamera = CCamera::Create();
	if (pCamera != NULL)
	{
		pCamera->SetPosV(pCamera->GetPosV() + D3DXVECTOR3(-175.0f, 0.0f, 0.0f));
		pCamera->SetPosR(pCamera->GetPosR() + D3DXVECTOR3(-175.0f, 0.0f, 0.0f));
		pCameraManager->SetCamera(pCamera);
	}
	SetCameraManager(pCameraManager);
}

//=============================================================================
// �`���[�g���A���̃}�b�v��������
//=============================================================================
void CTutorial::CreateMap(void)
{
	CMap *pMap = CMap::Create(m_pMapFileName[m_nMapIdx]);
	SetMap(pMap);
	ResetCounter();
	CEnemy::ResetSpawnCounter();
	SetPlayerPosToSpawn();
	CheckEnemySpawn(m_nTutorialCounter);
}

//=============================================================================
// �`���[�g���A���̓ǂݍ��ރ}�b�v�t�@�C�����̃|�C���^�𐶐�����
//=============================================================================
void CTutorial::CreateMapFilePointer(void)
{
	m_pMapFileName = new char*[m_nNumMap];
	if (m_pMapFileName != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumMap; nCnt++)
		{
			m_pMapFileName[nCnt] = new char[256];
			strcpy(m_pMapFileName[nCnt], "\0");
		}
	}
}

//=============================================================================
// �`���[�g���A���̃v���C���[��������
//=============================================================================
void CTutorial::CreatePlayer(void)
{
	// �v���C���[�𐶐�����
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = m_pPlayerManager[nCntPlayer]->SetPlayer(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, nCntPlayer);
	}
}

//=============================================================================
// �`���[�g���A���̓G��������
//=============================================================================
CEnemy *CTutorial::CreateEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CEnemy_ListData *pEnemyData)
{
	CEnemy *pEnemy = m_pEnemyManager[pEnemyData->GetEnemyType()]->SetEnemy(pos, rot, pEnemyData->GetEnemyType());

	if (pEnemy == NULL)return NULL;
	// �A�C�e���̃f�[�^��ݒ肷��
	if (pEnemyData->GetItem() == true)
	{
		pEnemy->SetItemData(pEnemyData->GetItemType());
	}

	// AI�̃f�[�^��ݒ肷��
	pEnemy->SetAIData(pEnemyData->GetAI_BulletEva(), pEnemyData->GetAI_MassEva(), pEnemyData->GetAI_DownEva());

	return pEnemy;
}

//=============================================================================
// �`���[�g���A���̑�����@�\���p�|���S����������
//=============================================================================
void CTutorial::CreateOpeInfo(void)
{
	if (m_pOpeInfo != NULL) { return; }

	m_pOpeInfo = CScene2D::Create(TUTORIAL_OPEINFO_POS_INI, TUTORIAL_OPEINFO_COL_INI, TUTORIAL_OPEINFO_WIDTH_INI,
		TUTORIAL_OPEINFO_HEIGHT_INI, TUTORIAL_OPEINFO_PRIORITY);
	if (m_pOpeInfo != NULL)
	{
		m_pOpeInfo->BindTexture(GetTextureManager()->GetTexture(TUTORIAL_OPEINFO_TEXIDX));
	}
}

//=============================================================================
// �`���[�g���A���̃u���b�N���󂵂����X�|���X�\���p�|���S����������
//=============================================================================
void CTutorial::CreateBreakBlockInfo(void)
{
	if (m_pBreakBlockInfo == NULL)
	{// ��������Ă��Ȃ�
		m_pBreakBlockInfo = CScene2D::Create(TUTORIAL_BREAKBLOCKINFO_POS_INI, TUTORIAL_BREAKBLOCKINFO_COL_INI,
			TUTORIAL_BREAKBLOCKINFO_WIDTH_INI, TUTORIAL_BREAKBLOCKINFO_HEIGHT_INI, TUTORIAL_BREAKBLOCKINFO_PRIORITY);
		if (m_pBreakBlockInfo != NULL)
		{
			m_pBreakBlockInfo->BindTexture(GetTextureManager()->GetTexture(TUTORIAL_BREAKBLOCKINFO_TEXIDX));
		}
	}
	else
	{// ���ɐ�������Ă���
		D3DXCOLOR col = m_pBreakBlockInfo->GetCol();
		col.a = 1.0f;
		m_pBreakBlockInfo->SetCol(col);
		m_pBreakBlockInfo->SetVtxBuffCol();
	}

	// �\���J�E���^�[���Z�b�g
	m_nCntBreakBlockInfoDisp = 0;
}

//=============================================================================
// �`���[�g���A���̃v���C���[���U���������X�|���X�\���p�|���S����������
//=============================================================================
void CTutorial::CreateAttackPlayerInfo(void)
{
	if (m_pAttackPlayerInfo == NULL)
	{// ��������Ă��Ȃ�
		m_pAttackPlayerInfo = CScene2D::Create(TUTORIAL_ATTACKPLAYERINFO_POS_INI, TUTORIAL_ATTACKPLAYERINFO_COL_INI,
			TUTORIAL_ATTACKPLAYERINFO_WIDTH_INI, TUTORIAL_ATTACKPLAYERINFO_HEIGHT_INI,
			TUTORIAL_ATTACKPLAYERINFO_PRIORITY);
		if (m_pAttackPlayerInfo != NULL)
		{
			m_pAttackPlayerInfo->BindTexture(GetTextureManager()->GetTexture(TUTORIAL_ATTACKPLAYERINFO_TEXIDX));
		}
	}
	else
	{// ���ɐ�������Ă���
		D3DXCOLOR col = m_pAttackPlayerInfo->GetCol();
		col.a = 1.0f;
		m_pAttackPlayerInfo->SetCol(col);
		m_pAttackPlayerInfo->SetVtxBuffCol();
	}

	// �\���J�E���^�[���Z�b�g
	m_nCntAttackInfoDisp = 0;
}

//=============================================================================
// �`���[�g���A���̃S�[�����߂����X�|���X�\���p�|���S����������
//=============================================================================
void CTutorial::CreateNearGoalInfo(void)
{
	if (m_pNearGoalInfo == NULL)
	{// ��������Ă��Ȃ�
		m_pNearGoalInfo = CScene2D::Create(TUTORIAL_NEARGOALINFO_POS_INI, TUTORIAL_NEARGOALINFO_COL_INI,
			TUTORIAL_NEARGOALINFO_WIDTH_INI, TUTORIAL_NEARGOALINFO_HEIGHT_INI, TUTORIAL_NEARGOALINFO_PRIORITY);
		if (m_pNearGoalInfo != NULL)
		{
			m_pNearGoalInfo->BindTexture(GetTextureManager()->GetTexture(TUTORIAL_NEARGOALINFO_TEXIDX));
		}
	}
	else
	{// ���ɐ�������Ă���
		D3DXCOLOR col = m_pNearGoalInfo->GetCol();
		col.a = 1.0f;
		m_pNearGoalInfo->SetCol(col);
		m_pNearGoalInfo->SetVtxBuffCol();
	}

	// �\���J�E���^�[���Z�b�g
	m_nCntNearGoalInfoDisp = 0;
}

//=============================================================================
// �`���[�g���A���̃S�[�����߂����X�|���X�\���p�|���S���`�F�b�N����
//=============================================================================
void CTutorial::CheckBreakBlockInfo(void)
{
	// ��������Ă��Ȃ������珈�����Ȃ�
	if (m_pBreakBlockInfo == NULL) { return; }

	// �J�E���^�[����
	m_nCntBreakBlockInfoDisp++;

	if (m_nCntBreakBlockInfoDisp >= TUTORIAL_ATTACKPLAYERINFO_DISP)
	{// ��莞�ԕ\�����ꑱ����
		// �����x���グ��
		D3DXCOLOR col = m_pBreakBlockInfo->GetCol();
		col.a -= TUTORIAL_ATTACKPLAYERINFO_ALPHACUT;
		m_pBreakBlockInfo->SetCol(col);
		m_pBreakBlockInfo->SetVtxBuffCol();

		if (col.a <= 0.0f)
		{// �����ɂȂ�؂���
			ReleaseBreakBlockInfo();
		}
	}
}

//=============================================================================
// �`���[�g���A���̃S�[�����߂����X�|���X�\���p�|���S���`�F�b�N����
//=============================================================================
void CTutorial::CheckAttackPlayerInfo(void)
{
	// ��������Ă��Ȃ������珈�����Ȃ�
	if (m_pAttackPlayerInfo == NULL) { return; }

	// �J�E���^�[����
	m_nCntAttackInfoDisp++;

	if (m_nCntAttackInfoDisp >= TUTORIAL_ATTACKPLAYERINFO_DISP)
	{// ��莞�ԕ\�����ꑱ����
	 // �����x���グ��
		D3DXCOLOR col = m_pAttackPlayerInfo->GetCol();
		col.a -= TUTORIAL_ATTACKPLAYERINFO_ALPHACUT;
		m_pAttackPlayerInfo->SetCol(col);
		m_pAttackPlayerInfo->SetVtxBuffCol();

		if (col.a <= 0.0f)
		{// �����ɂȂ�؂���
			ReleaseAttackPlayerInfo();
		}
	}
}

//=============================================================================
// �`���[�g���A���̃S�[�����߂����X�|���X�\���p�|���S���`�F�b�N����
//=============================================================================
void CTutorial::CheckNearGoalInfo(void)
{
	// ��������Ă��Ȃ������珈�����Ȃ�
	if (m_pNearGoalInfo == NULL) { return; }

	// �J�E���^�[����
	m_nCntNearGoalInfoDisp++;

	if (m_nCntNearGoalInfoDisp >= TUTORIAL_NEARGOALINFO_DISP)
	{// ��莞�ԕ\�����ꑱ����
	 // �����x���グ��
		D3DXCOLOR col = m_pNearGoalInfo->GetCol();
		col.a -= TUTORIAL_NEARGOALINFO_ALPHACUT;
		m_pNearGoalInfo->SetCol(col);
		m_pNearGoalInfo->SetVtxBuffCol();

		if (col.a <= 0.0f)
		{// �����ɂȂ�؂���
			ReleaseNearGoalInfo();
		}
	}
}


//*****************************************************************************
//
// �J���p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �`���[�g���A���̓ǂݍ��ރ}�b�v�t�@�C�����̃|�C���^���J������
//=============================================================================
void CTutorial::ReleaseMapFilePointer(void)
{
	if (m_pMapFileName != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumMap; nCnt++)
		{
			delete[] m_pMapFileName[nCnt];
			m_pMapFileName[nCnt] = NULL;
		}

		delete[] m_pMapFileName;
		m_pMapFileName = NULL;
	}
}

//=============================================================================
// �`���[�g���A���̃X�L�b�v���S�|���S�����J������
//=============================================================================
void CTutorial::ReleaseSkipLogo(void)
{
	if (m_pSkipLogo != NULL)
	{
		m_pSkipLogo->Uninit();
		m_pSkipLogo = NULL;
	}
}

//=============================================================================
// �`���[�g���A���̃X�L�b�v�m�F���S�p�|���S�����J������
//=============================================================================
void CTutorial::ReleaseSkipCheckLogo(void)
{
	for (int nCntSkipCheck = 0; nCntSkipCheck < SKIP_MAX; nCntSkipCheck++)
	{
		if (m_pSkipCheckLogo[nCntSkipCheck] != NULL)
		{
			m_pSkipCheckLogo[nCntSkipCheck]->Uninit();
			m_pSkipCheckLogo[nCntSkipCheck] = NULL;
		}
	}
}

//=============================================================================
// �`���[�g���A���̃v���C���[�N���X���J������
//=============================================================================
void CTutorial::ReleasePlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_pPlayer[nCntPlayer] != NULL)
		{
			m_pPlayer[nCntPlayer]->Uninit();
			m_pPlayer[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// �`���[�g���A���̃v���C���[�Ǌ��N���X���J������
//=============================================================================
void CTutorial::ReleasePlayerManager(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_pPlayerManager[nCntPlayer] != NULL)
		{
			m_pPlayerManager[nCntPlayer]->Uninit();
			delete m_pPlayerManager[nCntPlayer];
			m_pPlayerManager[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// �`���[�g���A���̓G�f�[�^�Ǌ��N���X���J������
//=============================================================================
void CTutorial::ReleaseEnemyManager(void)
{
	for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
	{
		if (m_pEnemyManager[nCntEnemy] != NULL)
		{
			m_pEnemyManager[nCntEnemy]->Uninit();
			delete m_pEnemyManager[nCntEnemy];
			m_pEnemyManager[nCntEnemy] = NULL;
		}
	}
}

//=============================================================================
// �`���[�g���A���̑�����@�\���p�|���S�����J������
//=============================================================================
void CTutorial::ReleaseOpeInfo(void)
{
	if (m_pOpeInfo != NULL)
	{
		m_pOpeInfo->Uninit();
		m_pOpeInfo = NULL;
	}
}

//=============================================================================
// �`���[�g���A���̃u���b�N���󂵂����X�|���X�\���p�|���S���J������
//=============================================================================
void CTutorial::ReleaseBreakBlockInfo(void)
{
	if (m_pBreakBlockInfo != NULL)
	{
		m_pBreakBlockInfo->Uninit();
		m_pBreakBlockInfo = NULL;
	}
}

//=============================================================================
// �`���[�g���A���̃v���C���[���U���������X�|���X�\���p�|���S���J������
//=============================================================================
void CTutorial::ReleaseAttackPlayerInfo(void)
{
	if (m_pAttackPlayerInfo != NULL)
	{
		m_pAttackPlayerInfo->Uninit();
		m_pAttackPlayerInfo = NULL;
	}
}

//=============================================================================
// �`���[�g���A���̃S�[�����߂����X�|���X�\���p�|���S���J������
//=============================================================================
void CTutorial::ReleaseNearGoalInfo(void)
{
	if (m_pNearGoalInfo != NULL)
	{
		m_pNearGoalInfo->Uninit();
		m_pNearGoalInfo = NULL;
	}
}


//*****************************************************************************
//
// ��Ԃɂ�鏈�������p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �`���[�g���A���̃X�L�b�v�m�F��Ԃ̍X�V����
//=============================================================================
void CTutorial::SkipCheckUpdate(void)
{
	CDebugProc::Print(1, "�X�L�b�v�m�F���\n");
	CDebugProc::Print(1, "�I��ԍ� : %d\n", m_nSelectSkip);

	// �z�X�g���łȂ��Ȃ炱�̏��������Ȃ�
	if (CManager::GetClient() != NULL)
	{
		if (CManager::GetClient()->GetConnected() == true && CManager::GetClient()->GetClientId() != 0)
		{
			return;
		}
	}

	// ���̓N���X���擾
	CInputKeyboard *pKey = CManager::GetKeyboard();
	CXInput *pXInput = CManager::GetXInput();
	CFade *pFade = CManager::GetFade();
	if (pKey == NULL || pXInput == NULL || pFade == NULL)return;
	if (pFade->GetFade() != CFade::FADE_NONE) { return; }

	if (pKey->GetTrigger(DIK_A) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_2) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_18) == true ||
		pKey->GetRepeat(DIK_A) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_2) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_18) == true)
	{// �������̓��͂����ꂽ
		ChangeNoneSkipCheckPolygon(m_nSelectSkip);
		m_nSelectSkip = (m_nSelectSkip + (SKIP_MAX - 1)) % SKIP_MAX;
		ChangeSelectSkipCheckPolygon(m_nSelectSkip);
		CManager::GetSound()->PlaySound(TUTORIAL_SE_SELECT_IDX);
	}
	else if (pKey->GetTrigger(DIK_D) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_3) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_19) == true ||
		pKey->GetRepeat(DIK_D) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_3) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_19) == true)
	{// �E�����̓��͂����ꂽ
		ChangeNoneSkipCheckPolygon(m_nSelectSkip);
		m_nSelectSkip = (m_nSelectSkip + 1) % SKIP_MAX;
		ChangeSelectSkipCheckPolygon(m_nSelectSkip);
		CManager::GetSound()->PlaySound(TUTORIAL_SE_SELECT_IDX);
	}
	else if (pKey->GetTrigger(DIK_RETURN) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// ����{�^���������ꂽ
		CManager::GetSound()->PlaySound(TUTORIAL_SE_DECIDE_IDX);
		if (m_nSelectSkip == SKIP_YES)
		{// �͂���������
			SetState(STATE_SKIP_END);
		}
		else if(m_nSelectSkip == SKIP_NO)
		{// ��������������
			SetState(STATE_NORMAL);
			ReleaseSkipLogo();
			ReleaseSkipCheckLogo();
			CScene::DeathCheck();
			CreateMap();
			CreatePlayer();
			CreateOpeInfo();
			SetPlayerPosToSpawn();
			CManager::GetSound()->PlaySound(TUTORIAL_BGM_IDX);
		}
	}
}

//=============================================================================
// �`���[�g���A���̃X�L�b�v���ďI����Ԃ̍X�V����
//=============================================================================
void CTutorial::SkipEndUpdate(void)
{
	CDebugProc::Print(1, "�X�L�b�v���ďI�����\n");

	CFade *pFade = CManager::GetFade();
	if (pFade == NULL) return;

	if (pFade->GetFade() == CFade::FADE_NONE)
	{// �t�F�[�h���g�p����Ă��Ȃ�
		CManager::GetFade()->SetFade(CManager::MODE_GAME);
	}
}

//=============================================================================
// �`���[�g���A���̒ʏ��Ԃ̍X�V����
//=============================================================================
void CTutorial::NormalUpdate(void)
{
	CDebugProc::Print(1, "�ʏ���\n");

	if (m_pGoalCylinder == NULL)
	{
		m_pGoalCylinder = CGoalCylinder::Create(D3DXVECTOR3(300.0f, 0.0f, 300.0f), INITIALIZE_D3DXVECTOR3,
			D3DXCOLOR(0.2f, 0.7f, 1.0f, 1.0f), 100.0f, 100.0f, 20, 10, (D3DX_PI * 0.1f), 0.0f, (D3DX_PI * 0.01f), 4);
	}
}

//=============================================================================
// �`���[�g���A���̃S�[����Ԃ̍X�V����
//=============================================================================
void CTutorial::GoalUpdate(void)
{
	CDebugProc::Print(1, "�S�[�����\n");
}

//=============================================================================
// �`���[�g���A���̃}�b�v�ύX��Ԃ̍X�V����
//=============================================================================
void CTutorial::MapChangeUpdate(void)
{
	CDebugProc::Print(1, "�}�b�v�ύX���\n");
}

//=============================================================================
// �`���[�g���A���̏I����Ԃ̍X�V����
//=============================================================================
void CTutorial::EndUpdate(void)
{
	CDebugProc::Print(1, "�I�����\n");

	// �v���C���[�ɃJ���������킹��
	EndCameraMoving();

	// ��ԃJ�E���^�[���Z
	m_nStateCounter++;
	if (m_nStateCounter >= TUTORIAL_END_TIME)
	{// ��Ԃ��ς�鎞�ԂɂȂ��Ă���
		CFade *pFade = CManager::GetFade();
		if (pFade == NULL) return;
		if (pFade->GetFade() != CFade::FADE_NONE) return;

		// ���̃��[�h�ɐݒ�
		pFade->SetFade(CManager::MODE_GAME);
	}
}

//=============================================================================
// �`���[�g���A���̏I����Ԏ��̃J�����ړ�����
//=============================================================================
void CTutorial::EndCameraMoving(void)
{
	if (GetCameraManager() == NULL) { return; }

	// �J�������擾����
	CCamera *pCamera = GetCameraManager()->GetCamera();
	if (pCamera == NULL) { return; }

	// �J�����̒����_���v���C���[�̈ʒu�ɍ��킹��
	int nClientId = 0;
	if (CManager::GetClient() != NULL)
	{
		nClientId = CManager::GetClient()->GetClientId();
	}
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL2P)
	{// ���[�J��2�l�v���C�Ȃ��
		nClientId = m_nGoalPlayIdx;
	}
	if (m_pPlayer[nClientId] == NULL) { return; }
	D3DXVECTOR3 DestPos = m_pPlayer[nClientId]->GetPos();
	pCamera->SetPosR(DestPos);

	// �J�����̎��_�ړI�ʒu���v�Z
	D3DXVECTOR3 PlayerRot = m_pPlayer[nClientId]->GetRot();
	D3DXVECTOR3 CameraPosVDest;
	CameraPosVDest.x = DestPos.x + sinf(PlayerRot.y - (D3DX_PI) + (D3DX_PI * 0.1f)) * TUTORIAL_END_CAMERA_LENGTH;
	CameraPosVDest.y = TUTORIAL_END_CAMERA_POSY;
	CameraPosVDest.z = DestPos.z + cosf(PlayerRot.y - (D3DX_PI) + (D3DX_PI * 0.1f)) * TUTORIAL_END_CAMERA_LENGTH;

	// �J�����̎��_��ړI�̈ʒu�܂ŏ�����������
	D3DXVECTOR3 CameraPosVDiff;
	D3DXVECTOR3 CameraPosV = pCamera->GetPosV();
	CameraPosVDiff = CameraPosVDest - CameraPosV;
	CameraPosV += CameraPosVDiff * TUTORIAL_END_CAMERA_MOVING;
	pCamera->SetPosV(CameraPosV);
}

//=============================================================================
// �`���[�g���A���̃X�L�b�v�m�F�p�|���S����I������Ă��Ȃ���Ԃɂ���
//=============================================================================
void CTutorial::ChangeNoneSkipCheckPolygon(int nSelect)
{
	D3DXCOLOR SkipCheckColLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_COL_NONE,  TUTORIAL_SKIPCHECKLOGO_1_COL_NONE };
	float fSkipCheckWidthLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_WIDTH_NONE,  TUTORIAL_SKIPCHECKLOGO_1_WIDTH_NONE };
	float fSkipCheckHeightLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_HEIGHT_NONE,  TUTORIAL_SKIPCHECKLOGO_1_HEIGHT_NONE };
	if (m_pSkipCheckLogo[nSelect] != NULL)
	{
		m_pSkipCheckLogo[nSelect]->SetState(CScene2DFlash::STATE_NONE);
		m_pSkipCheckLogo[nSelect]->SetScale(fSkipCheckWidthLogo[nSelect], fSkipCheckHeightLogo[nSelect]);
		m_pSkipCheckLogo[nSelect]->SetCol(SkipCheckColLogo[nSelect]);
		m_pSkipCheckLogo[nSelect]->SetVtxBuffValue();
	}
}

//=============================================================================
// �`���[�g���A���̃X�L�b�v�m�F�p�|���S����I�����ꂽ��Ԃɂ���
//=============================================================================
void CTutorial::ChangeSelectSkipCheckPolygon(int nSelect)
{
	D3DXCOLOR SkipCheckColLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_COL_SELECT,  TUTORIAL_SKIPCHECKLOGO_1_COL_SELECT };
	float fSkipCheckWidthLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_WIDTH_SELECT,  TUTORIAL_SKIPCHECKLOGO_1_WIDTH_SELECT };
	float fSkipCheckHeightLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_HEIGHT_SELECT,  TUTORIAL_SKIPCHECKLOGO_1_HEIGHT_SELECT };

	if (m_pSkipCheckLogo[nSelect] != NULL)
	{
		m_pSkipCheckLogo[nSelect]->SetState(CScene2DFlash::STATE_SELECT);
		m_pSkipCheckLogo[nSelect]->SetScale(fSkipCheckWidthLogo[nSelect], fSkipCheckHeightLogo[nSelect]);
		m_pSkipCheckLogo[nSelect]->SetCol(SkipCheckColLogo[nSelect]);
		m_pSkipCheckLogo[nSelect]->SetVtxBuffValue();
	}
}


//*****************************************************************************
//
// �}�b�v��ς���p�֐�(�f�o�b�O�p�̊֐�����)
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �`���[�g���A���̊e��J�E���^�[������������
//=============================================================================
void CTutorial::ResetCounter(void)
{
	m_nSpawnEnemyCount = 0;
	m_nTutorialCounter = 0;
	m_nStateCounter = 0;
}

//=============================================================================
// �`���[�g���A���̎��̃}�b�v�ɐ؂�ւ��鏈��
//=============================================================================
void CTutorial::ChangeNextMap(void)
{
	// ���݂̃}�b�v��j��
	ReleaseMap();

	// �Q�[���Ɏg�p���Ă���I�u�W�F�N�g��j��
	DeleteGameObject();

	// �}�b�v�ԍ��𑝂₵�}�b�v��ǂݍ���
	m_nMapIdx++;
	if (m_nMapIdx >= TUTORIAL_MAPSTAGE_MAX)
	{
		m_nMapIdx--;
	}
	CreateMap();

	// �}�b�v�ԍ��̌������v�Z���Ă���
	CalcStageDigits();

	// �v���C���[�̈ʒu���X�|�[���ʒu��ݒ�
	SetPlayerPosToSpawn();

	// �e��J�E���^�[������
	m_nTutorialCounter = 0;
	m_nSpawnEnemyCount = 0;

	// ��Ԃ�ݒ�
	SetState(STATE_NORMAL);
}

//=============================================================================
// �`���[�g���A���̑O�̃}�b�v�ɐ؂�ւ��鏈��
//=============================================================================
void CTutorial::ChangePrevMap(void)
{
	// ���݂̃}�b�v��j��
	ReleaseMap();

	// �Q�[���Ɏg�p���Ă���I�u�W�F�N�g��j��
	DeleteGameObject();

	// �}�b�v�ԍ������炵�}�b�v��ǂݍ���
	m_nMapIdx--;
	if (m_nMapIdx < 0)
	{
		m_nMapIdx++;
	}
	CreateMap();

	// �}�b�v�ԍ��̌������v�Z���Ă���
	CalcStageDigits();

	// �v���C���[�̈ʒu���X�|�[���ʒu�ɐݒ�
	SetPlayerPosToSpawn();

	// �e��J�E���^�[������
	m_nTutorialCounter = 0;
	m_nSpawnEnemyCount = 0;

	// ��Ԃ�ݒ�
	SetState(STATE_NORMAL);
}

//=============================================================================
// �`���[�g���A���̃`���[�g���A���Ɏg�p���Ă���I�u�W�F�N�g��j�����鏈��
//=============================================================================
void CTutorial::DeleteGameObject(void)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (TUTORIAL_DELETE_OBJECT)
			{// �`���[�g���A���Ɏg�p���Ă���I�u�W�F�N�g�N���X������
				pScene->Uninit();
				pScene = NULL;
			}
			pScene = pSceneNext;
		}
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck();
}

//=============================================================================
// �`���[�g���A���̃X�e�[�W�ԍ��̌������v�Z���鏈��
//=============================================================================
void CTutorial::CalcStageDigits(void)
{
	m_nNumNumberDigits = (int)log10f((float)(m_nMapIdx + 1)) + 1;	// ���������߂�
	if (m_nNumNumberDigits <= 0) { m_nNumNumberDigits = 1; }	    // 0�ȉ��̂Ƃ�1�ɂ���
}



//*****************************************************************************
//
// �`���[�g���A�����X�|�[�������p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �`���[�g���A���̃v���C���[�����X�|�[���ʒu�ɂ��炷����
//=============================================================================
void CTutorial::SetPlayerPosToSpawn(void)
{
	// �v���C���[�̃��X�|�[���ʒu���擾
	D3DXVECTOR3 PlayerPos = INITIALIZE_D3DXVECTOR3;
	CMap *pMap = GetMap();
	if (pMap == NULL) { return; }

	// �v���C���[�̈ʒu�����X�|�[���ʒu�ɐݒ�
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// �I�����C���v���C�Ȃ��
		PlayerPos = pMap->GetPlayerRespawn(CManager::GetClient()->GetClientId())->GetPos();
		if (m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
		{
			m_pPlayer[CManager::GetClient()->GetClientId()]->SetPos(PlayerPos);
			m_pPlayer[CManager::GetClient()->GetClientId()]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
			m_pPlayer[CManager::GetClient()->GetClientId()]->SetNowRotInfo(CCharacter::ROT_UP);
		}
	}
	else
	{// ���[�J���v���C�Ȃ��
		for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
		{// �v���C���[�̐l�����J��Ԃ�
			PlayerPos = pMap->GetPlayerRespawn(nCntPlayer)->GetPos();
			if (m_pPlayer[nCntPlayer] != NULL)
			{
				m_pPlayer[nCntPlayer]->SetPos(PlayerPos);
				m_pPlayer[nCntPlayer]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
				m_pPlayer[nCntPlayer]->SetNowRotInfo(CCharacter::ROT_UP);
			}
		}
	}
}

//=============================================================================
// �`���[�g���A���̃v���C���[�����X�|�[�������邩�`�F�b�N����
//=============================================================================
void CTutorial::CheckPlayerResSpawn(int nCntPlayer)
{
	if (m_pPlayer[nCntPlayer] == NULL)
	{
		m_nPlayerRespawnCounter++;
		if (m_nPlayerRespawnCounter % TUTORIAL_PLAYER_RESPAWN_TIMING == 0)
		{
			m_nPlayerRespawnCounter = 0;
			m_pPlayer[nCntPlayer] = m_pPlayerManager[nCntPlayer]->SetPlayer(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, nCntPlayer);

			// �v���C���[�̃��X�|�[���ʒu���擾
			D3DXVECTOR3 PlayerPos = INITIALIZE_D3DXVECTOR3;
			CMap *pMap = GetMap();
			if (pMap != NULL)
			{
				PlayerPos = pMap->GetPlayerRespawn(nCntPlayer)->GetPos();
			}

			// �v���C���[�̈ʒu�����炷
			m_pPlayer[nCntPlayer]->SetPos(PlayerPos);
		}
	}
}

//=============================================================================
// �`���[�g���A���̓G�̐�������^�C�~���O���ǂ������肷��
//=============================================================================
void CTutorial::CheckEnemySpawn(int nTime)
{
	CMap *pMap = GetMap();
	if (pMap == NULL)return;

	int nNumEnemyListData = pMap->GetNumEnemyListData();
	CEnemy_ListData *pEnemyData = NULL;

	// �����G���o�Ȃ��̂ł���΂��̎��_�ŏ����I��
	if (m_nSpawnEnemyCount == nNumEnemyListData) return;

	// �J�E���^�[����
	m_nTutorialCounter++;
	for (int nCnt = 0; nCnt < nNumEnemyListData; nCnt++)
	{// �G�𐶐����鐔���J��Ԃ�
		pEnemyData = pMap->GetEnemyListData(nCnt);
		if (pEnemyData->GetRespawnTime() == nTime)
		{// ���X�|�[������^�C�~���O�ł���
			EnemySpawn(pMap, pEnemyData, nCnt);
			m_nSpawnEnemyCount++;
		}
	}
}

//=============================================================================
// �`���[�g���A���̓G���X�|�[��������
//=============================================================================
void CTutorial::EnemySpawn(CMap *pMap, CEnemy_ListData *pEnemyData, int nCnt)
{
	// �G�̃��X�|�[���ʒu���擾
	D3DXVECTOR3 EnemyPos = pMap->GetEnemyRespawn(pEnemyData->GetRespawnIdx())->GetPos();

	// �G�̐���
	CreateEnemy(EnemyPos, INITIALIZE_D3DXVECTOR3, pEnemyData);
}




//*****************************************************************************
//
// �X�N���v�g�ǂݍ��ݗp�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �`���[�g���A���̃V�X�e������ǂݍ���
//=============================================================================
void CTutorial::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(TUTORIAL_SYSTEM_FILENAME);
	if (pFileLoader != NULL)
	{
		strcpy(aStr, pFileLoader->GetString(aStr));
		if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
		{// �ǂݍ��݊J�n�̍��}������
			LoadSystemScript(pFileLoader, aStr);
		}

		// �������̊J��
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}
	}
}

//=============================================================================
// �`���[�g���A���̃V�X�e�������t�@�C������ǂݍ���
//=============================================================================
void CTutorial::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
{
	int nCntTex = 0;
	int nCntModel = 0;
	int nCntPlayer = 0;
	int nCntMap = 0;
	int nCntSkipCheck = 0;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_TEXTURE) == 0)
		{// �ǂݍ��ރe�N�X�`���̐�������
			CreateTexture(CFunctionLib::ReadInt(pStr, NUM_TEXTURE));
		}
		else if (CFunctionLib::Memcmp(pStr, TEXTURE_FILENAME) == 0)
		{// �ǂݍ��ރe�N�X�`���̃t�@�C����������
			LoadTexFileName(pStr, nCntTex);
			nCntTex++;
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_MODEL) == 0)
		{// �ǂݍ��ރ��f���̐�������
			int nNumModel = CFunctionLib::ReadInt(pStr, NUM_MODEL);
			CModelCreate *pModelCreate = CModelCreate::Create(nNumModel);
			SetModelCreate(pModelCreate);
		}
		else if (CFunctionLib::Memcmp(pStr, MODEL_FILENAME) == 0)
		{// �ǂݍ��ރ��f���̃t�@�C����������
			LoadModel(pStr, nCntModel);
			nCntModel++;
		}
		else if (CFunctionLib::Memcmp(pStr, EFFECT_FILENAME) == 0)
		{// �ǂݍ��ރG�t�F�N�g�̃t�@�C����������
			LoadEffectFileName(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYERDATA_FILENAME) == 0)
		{// �v���C���[�f�[�^�̃X�N���v�g�t�@�C����������
			LoadPlayerFileName(pStr, nCntPlayer);
			nCntPlayer++;
		}
		else if (CFunctionLib::Memcmp(pStr, BULLET_MODEL_IDX) == 0)
		{// �e���g�p���郂�f���̔ԍ�������
			m_nBulletModelIdx = CFunctionLib::ReadInt(pStr, BULLET_MODEL_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_MAP) == 0)
		{// �ǂݍ��ރ}�b�v�̐�������
			m_nNumMap = CFunctionLib::ReadInt(pStr, NUM_MAP);
			CreateMapFilePointer();
		}
		else if (CFunctionLib::Memcmp(pStr, MAP_FILENAME) == 0)
		{// �ǂݍ��ރ}�b�v�t�@�C����������
			LoadMapFileName(pStr, nCntMap);
			nCntMap++;
		}
		else if (CFunctionLib::Memcmp(pStr, SKIPLOGOSET) == 0)
		{// �X�L�b�v���S�|���S����񂾂���
			LoadSkipLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, SKIPCHECKLOGOSET) == 0)
		{// �X�L�b�v�m�F���S�p�|���S����񂾂���
			LoadSkipCheckLogo(pFileLoader, pStr, nCntSkipCheck);
			nCntSkipCheck++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�t�@�C���I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �`���[�g���A���̓ǂݍ��ރe�N�X�`���t�@�C������ǂݍ���
//=============================================================================
void CTutorial::LoadTexFileName(char *pStr, int nCntTex)
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	char aTexFileName[256] = "\0";
	strcpy(aTexFileName, CFunctionLib::ReadString(pStr, aTexFileName, TEXTURE_FILENAME));

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), aTexFileName, &pTexture);

	// �e�N�X�`���Ǌ��N���X�Ƀ|�C���^��ݒ肷��
	CTextureManager *pTextureManager = GetTextureManager();
	pTextureManager->SetTexture(pTexture, nCntTex);
	pTextureManager->SetFileName(pStr, nCntTex);
}

//=============================================================================
// �`���[�g���A���̓ǂݍ��ރA�C�e�����f����ǂݍ���
//=============================================================================
void CTutorial::LoadModel(char *pStr, int nCntModel)
{
	// ���f���̃t�@�C���p�X����ǂݎ��
	LPD3DXMESH pMesh = NULL;
	LPD3DXBUFFER pBuffMat = NULL;
	DWORD nNumMat = 0;
	char aModelFileName[256] = "\0";
	strcpy(aModelFileName, CFunctionLib::ReadString(pStr, aModelFileName, MODEL_FILENAME));

	// x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(aModelFileName, D3DXMESH_SYSTEMMEM, CManager::GetRenderer()->GetDevice(), NULL,
		&pBuffMat, NULL, &nNumMat, &pMesh);

	// ���f���Ǌ��N���X�ɒl��ݒ肷��
	CModelCreate *pModelCreate = GetModelCreate();
	if (pModelCreate == NULL)return;
	pModelCreate->SetMesh(pMesh, nCntModel);
	pModelCreate->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);
	pModelCreate->SetFileName(pStr, nCntModel);
}

//=============================================================================
// �`���[�g���A���̓ǂݍ��ރG�t�F�N�g�t�@�C������ǂݍ���
//=============================================================================
void CTutorial::LoadEffectFileName(char *pStr)
{
	char aEffectFileName[256] = "\0";
	strcpy(aEffectFileName, CFunctionLib::ReadString(pStr, aEffectFileName, EFFECT_FILENAME));

	// �G�t�F�N�g�Ǌ��N���X�𐶐�
	CEffectManager *pEffectManager = CEffectManager::Create(aEffectFileName);
	SetEffectManager(pEffectManager);
}

//=============================================================================
// �`���[�g���A���̓ǂݍ��ރv���C���[�t�@�C������ǂݍ���
//=============================================================================
void CTutorial::LoadPlayerFileName(char *pStr, int nCntPlayer)
{
	char aPlayerFileName[256] = "\0";
	strcpy(aPlayerFileName, CFunctionLib::ReadString(pStr, aPlayerFileName, PLAYERDATA_FILENAME));

	for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
	{
		if (nCntPlayer == CCharaSelect::GetPlayerNumber(nCnt))
		{
			m_pPlayerManager[nCnt] = CPlayerManager::Create(aPlayerFileName);
		}
	}
}

//=============================================================================
// �`���[�g���A���̓ǂݍ��ރ}�b�v�t�@�C������ǂݍ���
//=============================================================================
void CTutorial::LoadMapFileName(char *pStr, int nCntMap)
{
	strcpy(m_pMapFileName[nCntMap], CFunctionLib::ReadString(pStr, m_pMapFileName[nCntMap], MAP_FILENAME));
}

//=============================================================================
// �`���[�g���A���̃X�L�b�v���S�|���S������ǂݍ���
//=============================================================================
void CTutorial::LoadSkipLogo(CFileLoader *pFileLoader, char *pStr)
{
	int nSkipLogoTexIdx = 0;
	D3DXVECTOR3 SkipLogoPos = TUTORIAL_SKIPLOGO_POS_INI;
	D3DXCOLOR SkipLogoCol = TUTORIAL_SKIPLOGO_COL_INI;
	float fSkipLogoWidth = TUTORIAL_SKIPLOGO_WIDTH_INI;
	float fSkipLogoHeight = TUTORIAL_SKIPLOGO_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			nSkipLogoTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			SkipLogoPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			SkipLogoCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			fSkipLogoWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			fSkipLogoHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SKIPLOGOSET) == 0)
		{// �X�L�b�v���S�|���S�����I���̍��}������
			break;
		}
	}

	// �|���S������
	m_pSkipLogo = CScene2D::Create(SkipLogoPos, SkipLogoCol, fSkipLogoWidth, fSkipLogoHeight);

	// �e�N�X�`�����蓖��
	if (m_pSkipLogo != NULL && GetTextureManager() != NULL)
	{
		m_pSkipLogo->BindTexture(GetTextureManager()->GetTexture(nSkipLogoTexIdx));
	}
}

//=============================================================================
// �`���[�g���A���̃X�L�b�v�m�F���S�p�|���S������ǂݍ���
//=============================================================================
void CTutorial::LoadSkipCheckLogo(CFileLoader *pFileLoader, char *pStr, int nCntSkipCheck)
{
	int nSkipCheckTexIdx[SKIP_MAX] = {};
	D3DXVECTOR3 SkipCheckPosLogo[SKIP_MAX] = {TUTORIAL_SKIPCHECKLOGO_0_POS_INI,  TUTORIAL_SKIPCHECKLOGO_1_POS_INI };
	D3DXCOLOR SkipCheckColLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_COL_NONE,  TUTORIAL_SKIPCHECKLOGO_1_COL_NONE };
	float fSkipCheckWidthLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_WIDTH_NONE,  TUTORIAL_SKIPCHECKLOGO_1_WIDTH_NONE };
	float fSkipCheckHeightLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_HEIGHT_NONE,  TUTORIAL_SKIPCHECKLOGO_1_HEIGHT_NONE };

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			nSkipCheckTexIdx[nCntSkipCheck] = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			SkipCheckPosLogo[nCntSkipCheck] = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			SkipCheckColLogo[nCntSkipCheck] = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			fSkipCheckWidthLogo[nCntSkipCheck] = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			fSkipCheckHeightLogo[nCntSkipCheck] = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SKIPCHECKLOGOSET) == 0)
		{// �X�L�b�v�m�F���S�p�|���S�����I���̍��}������
			break;
		}
	}

	// �|���S������
	m_pSkipCheckLogo[nCntSkipCheck] = CScene2DFlash::Create(SkipCheckPosLogo[nCntSkipCheck],
		SkipCheckColLogo[nCntSkipCheck], fSkipCheckWidthLogo[nCntSkipCheck],
		fSkipCheckHeightLogo[nCntSkipCheck], 0.0f, CScene2DFlash::STATE_NONE);
	if (nCntSkipCheck == 0)m_pSkipCheckLogo[nCntSkipCheck]->SetState(CScene2DFlash::STATE_SELECT);

	// �e�N�X�`�����蓖��
	if (m_pSkipCheckLogo[nCntSkipCheck] != NULL && GetTextureManager() != NULL)
	{
		m_pSkipCheckLogo[nCntSkipCheck]->BindTexture(GetTextureManager()->GetTexture(nSkipCheckTexIdx[nCntSkipCheck]));
	}
}



//*****************************************************************************
//
// �ݒ�A�擾���F�X�֐�(�O���Ƃ̑������܂߂Ă����ɏ����Ă܂�)
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �`���[�g���A���̕ϐ�������������
//=============================================================================
void CTutorial::ClearVariable(void)
{
	m_State = STATE_SKIP_CHECK;
	m_nStateCounter = 0;
	m_nSelectSkip = 0;
	m_pSkipLogo = NULL;
	m_nNumMap = 0;
	m_pMapFileName = NULL;
	for (int nCntSkip = 0; nCntSkip < SKIP_MAX; nCntSkip++)
	{
		m_pSkipCheckLogo[nCntSkip] = NULL;
	}
	m_pMapFileName = NULL;
	m_nStateCounter = 0;
	m_nMapIdx = 0;
	m_nNumMap = 0;
	m_nSpawnEnemyCount = 0;
	m_nNumEnemy = 0;
	m_nNumberTexIdx = 0;
	m_nNumNumberDigits = NULL;
	m_nBulletModelIdx = 0;
	m_nPlayerRespawnCounter = 0;
	m_nNumDeleteBlock = 0;
	strcpy(m_aDeleteBlock, "\0");
	m_nNumDeleteEnemy = 0;
	strcpy(m_aDeleteEnemy, "\0");
	m_bDeletePlayerFlag = 0;
	m_pGoalCylinder = NULL;
	m_pOpeInfo = NULL;
	m_nGoalPlayIdx = 0;
	m_bHitBulletFlag = false;
	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = NULL;
		m_pPlayerManager[nCntPlayer] = NULL;
	}
	for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
	{
		m_pEnemyManager[nCntEnemy] = NULL;
	}

	// ���X�|���X�p
	m_pBreakBlockInfo = NULL;
	m_nCntBreakBlockInfoDisp = 0;
	m_pAttackPlayerInfo = NULL;
	m_nCntAttackInfoDisp = 0;
	m_pNearGoalInfo = NULL;
	m_nCntNearGoalInfoDisp = 0;
}

//=============================================================================
// �`���[�g���A���̒e���f����ݒ肷�鏈��
//=============================================================================
void CTutorial::SetBulletModel(CBullet *pBullet)
{
	pBullet->BindModel(GetModelCreate()->GetMesh(m_nBulletModelIdx), GetModelCreate()->GetBuffMat(m_nBulletModelIdx),
		GetModelCreate()->GetNumMat(m_nBulletModelIdx), GetModelCreate()->GetTexture(m_nBulletModelIdx));
}

//=============================================================================
// �`���[�g���A���̏�Ԃ�ݒ肷��
//=============================================================================
void CTutorial::SetState(const STATE state)
{
	m_State = state;
	m_nStateCounter = 0;
}

//=============================================================================
// �`���[�g���A���̃S�[�������v���C���[�̔ԍ���ݒ肷��
//=============================================================================
void CTutorial::SetGoalPlayerIdx(const int nIdx)
{
	m_nGoalPlayIdx = nIdx;
}

//=============================================================================
// �`���[�g���A���̃v���C���[�N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CTutorial::SetPlayer(CPlayer *pPlayer, const int nIdx)
{
	m_pPlayer[nIdx] = pPlayer;
}

//=============================================================================
// �`���[�g���A���̏�Ԃ��擾����
//=============================================================================
CTutorial::STATE CTutorial::GetState(void)
{
	return m_State;
}

//=============================================================================
// �`���[�g���A���̃v���C���[�N���X�ւ̃|�C���^���擾����
//=============================================================================
CPlayer *CTutorial::GetPlayer(const int nIdx)
{
	return m_pPlayer[nIdx];
}
