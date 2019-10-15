//=============================================================================
//
// �L�����Z���N�g���� [charaselect.cpp]
// Author : Hodaka Niwa & Jukiya Hayakawa
//
//=============================================================================
#include "charaselect.h"
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
#include "server.h"
#include "scene2D.h"
#include "title.h"
#include "sound.h"
#include "debugproc.h"


// �v���C���[�`��p
#include "player.h"
#include "playerManager.h"
#include "light.h"
#include "lightManager.h"
#include "scene3D.h"
#include "charaCylinder.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define CHARASELECT_SYSTEM_FILENAME              "data/TEXT/MODE/charaselect.ini"    // �������Ɏg�p����V�X�e���t�@�C����
#define CHARASELECT_CAMERA_POSV                  (D3DXVECTOR3(0.0f,40.0f,450.0f))    // �L�������f���J�����̎��_�ʒu
#define CHARASELECT_CIRCLE_RADIUS                (140.0f)                            // �~�̔��a(�~�`��]�̌v�Z�Ɏg�p)
#define CHARASELECT_CIRCLE_ROLLSPEED             (D3DX_PI * 2.0f / 120.0f)           // �~�`��]���̉�]�X�s�[�h
#define CHARASELECT_MODECHANGE_TIMING            (120)                               // �I����Ԃ����ʑJ�ڂ���܂ł̎���
#define CHARASELECT_BGM_IDX                      (1)                                 // �L�����Z���N�g�ōĐ�����BGM�̔ԍ�
#define CHARASELECT_SE_SELECT_IDX                (8)                                 // ���ڂ�I�����Ă��鎞��SE�̉��ԍ�
#define CHARASELECT_SE_DECIDE_IDX                (9)                                 // ����{�^���������ꂽ����SE�̉��ԍ�
#define CHARASELECT_SE_NOTDECIDE_IDX             (10)                                // �L�����Z���N�g���ł��Ȃ��Ƃ�����SE�̉��ԍ�

// �v�Z�u�������p�}�N��
#define CHARASELECT_MAPTYPE_NUMBER               (m_nSelectStageSide + (m_nSelectStageVertical * (CGame::MAPTYPE_MAX / 2)))

// �w�i�|���S���������p
#define CHARASELECTBG_POS_INI                    (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define CHARASELECTBG_COL_INI                    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define CHARASELECTBG_WIDTH_INI                  (SCREEN_WIDTH / 2.0f)
#define CHARASELECTBG_HEIGHT_INI                 (SCREEN_HEIGHT / 2.0f)
#define CHARASELECTBG_TEXIDX                     (7)

// �������|���S���������p
// �P��
#define CHARASELECTPREPARATION_0_POS_INI         (D3DXVECTOR3(SCREEN_WIDTH / 4.0f, 600.0f, 0.0f))
#define CHARASELECTPREPARATION_0_COL_INI         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define CHARASELECTPREPARATION_0_WIDTH_INI       (200.0f)
#define CHARASELECTPREPARATION_0_HEIGHT_INI      (80.0f)

// �Q��
#define CHARASELECTPREPARATION_1_POS_INI         (D3DXVECTOR3(SCREEN_WIDTH / 4.0f * 3.0f, 600.0f, 0.0f))
#define CHARASELECTPREPARATION_1_COL_INI         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define CHARASELECTPREPARATION_1_WIDTH_INI       (200.0f)
#define CHARASELECTPREPARATION_1_HEIGHT_INI      (80.0f)


// �v���C���[�ԍ��|���S���������p
// �P��
#define CHARASELECTPLAYERNUMBER_0_POS_INI        (D3DXVECTOR3(110.0f, 50.0f, 0.0f))
#define CHARASELECTPLAYERNUMBER_0_COL_INI        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define CHARASELECTPLAYERNUMBER_0_WIDTH_INI      (100.0f)
#define CHARASELECTPLAYERNUMBER_0_HEIGHT_INI     (40.0f)

// �Q��
#define CHARASELECTPLAYERNUMBER_1_POS_INI        (D3DXVECTOR3(750.0f, 50.0f, 0.0f))
#define CHARASELECTPLAYERNUMBER_1_COL_INI        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define CHARASELECTPLAYERNUMBER_1_WIDTH_INI      (100.0f)
#define CHARASELECTPLAYERNUMBER_1_HEIGHT_INI     (40.0f)

// �����̔ԍ��\���p�|���S���p
#define CHARASELECT_YOUPOLYGON_POS_INI           (D3DXVECTOR3(360.0f, 50.0f, 0.0f))
#define CHARASELECT_YOUPOLYGON_COL_INI           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define CHARASELECT_YOUPOLYGON_WIDTH_INI         (50.0f)
#define CHARASELECT_YOUPOLYGON_HEIGHT_INI        (20.0f)

// �����_�����O�|���S���������p
// �P��
#define CHARASELECT_RENDERERPOLYGON_0_POS_INI    (D3DXVECTOR3(SCREEN_WIDTH / 4.0f, 520.0f, 0.0f))
#define CHARASELECT_RENDERERPOLYGON_0_COL_INI    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define CHARASELECT_RENDERERPOLYGON_0_WIDTH_INI  (310.0f)
#define CHARASELECT_RENDERERPOLYGON_0_HEIGHT_INI (250.0f)

// �Q��
#define CHARASELECT_RENDERERPOLYGON_1_POS_INI    (D3DXVECTOR3(SCREEN_WIDTH / 4.0f * 3.0f, 520.0f, 0.0f))
#define CHARASELECT_RENDERERPOLYGON_1_COL_INI    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define CHARASELECT_RENDERERPOLYGON_1_WIDTH_INI  (310.0f)
#define CHARASELECT_RENDERERPOLYGON_1_HEIGHT_INI (250.0f)

// �X�e�[�W�I��p�|���S���������p
#define CHARASELECT_STAGEPOLYGON_POS_INI         (D3DXVECTOR3(SCREEN_WIDTH / 6.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define CHARASELECT_STAGEPOLYGON_COL_NONE        (D3DXCOLOR(0.2f,0.2f,0.2f,1.0f))
#define CHARASELECT_STAGEPOLYGON_WIDTH_NONE      (140.0f)
#define CHARASELECT_STAGEPOLYGON_HEIGHT_NONE     (130.0f)
#define CHARASELECT_STAGEPOLYGON_COL_SELECT      (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define CHARASELECT_STAGEPOLYGON_WIDTH_SELECT    (180.0f)
#define CHARASELECT_STAGEPOLYGON_HEIGHT_SELECT   (180.0f)
#define CHARASELECT_STAGEPOLYGON_INTERVAL_X      (SCREEN_WIDTH / 3.0f)

// �v���C���[�̑���|���S���������p
#define CHARASELECT_PEDESTAL_COL_INI             (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define CHARASELECT_PEDESTAL_WIDTH_INI           (60.0f)
#define CHARASELECT_PEDESTAL_HEIGHT_INI          (30.0f)
#define CHARASELECT_PEDESTAL_TEXIDX              (6)

// �l�ǂݍ��ݗp�̃p�X
// �e�N�X�`���p
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// �v���C���[�f�[�^�p
#define NUM_PLAYERDATA "NUM_PLAYERDATA = "
#define PLAYERDATA_FILENAME "PLAYERDATA_FILENAME = "

// �w�i�|���S���p
#define BGPOLYGONSET "BGPOLYGONSET"
#define END_BGPOLYGONSET "END_BGPOLYGONSET"
#define TEX_IDX "TEX_IDX = "
#define POS "POS = "
#define COL "COL = "
#define WIDTH "WIDTH = "
#define HEIGHT "HEIGHT = "

// �������|���S���p
#define PREPARATION_TEX_IDX "PREPARATION_TEX_IDX = "
#define PREPARATIONSET "PREPARATIONSET"
#define END_PREPARATIONSET "END_PREPARATIONSET"

// �v���C���[�ԍ��\���|���S���p
#define PLAYERNUMBER_TEX_IDX "PLAYERNUMBER_TEX_IDX = "
#define PLAYERNUMBERSET "PLAYERNUMBERSET"
#define END_PLAYERNUMBERSET "END_PLAYERNUMBERSET"

// �����̔ԍ��\���p�|���S���p
#define YOUPOLYGONSET "YOUPOLYGONSET"
#define END_YOUPOLYGONSET "END_YOUPOLYGONSET"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
int CCharaSelect::m_nSelectPlayer[MAX_NUM_PLAYER] = {};
int CCharaSelect::m_nSelectStage = CGame::MAPTYPE_HINAMATSURI;

//=============================================================================
// �L�����Z���N�g�̃R���X�g���N�^
//=============================================================================
CCharaSelect::CCharaSelect()
{

}

//=============================================================================
// �L�����Z���N�g�̃f�X�g���N�^
//=============================================================================
CCharaSelect::~CCharaSelect()
{

}

//=============================================================================
// �L�����Z���N�g�̐�������
//=============================================================================
CCharaSelect *CCharaSelect::Create()
{
	CCharaSelect *pCharaSelect = NULL;
	if (pCharaSelect == NULL)
	{
		pCharaSelect = new CCharaSelect;
		if (pCharaSelect != NULL)
		{
			pCharaSelect->Init();
		}
	}
	return pCharaSelect;
}

//=============================================================================
// �L�����Z���N�g�̏���������
//=============================================================================
HRESULT CCharaSelect::Init(void)
{
	// �ϐ�������������
	ClearVariable();

	// �V�X�e���̏�����
	LoadSystem();

	// �J�����̐���
	CreateCamera();

	// �v���C���[�}�l�[�W���[�̐���
	CreatePlayerManager();

	// �v���C���[�̐���
	CreatePlayer();

	// �����_�����O�p�e�N�X�`���̐���
	CreateRenderTexture();

	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// �I�����C���v���C�Ȃ��
		// �N���C�A���g�̐���
		CManager::CreateClient();
	}

	// BGM�̍Đ�
	CManager::GetSound()->PlaySound(CHARASELECT_BGM_IDX);

	return S_OK;
}

//=============================================================================
// �L�����Z���N�g�̏I������
//=============================================================================
void CCharaSelect::Uninit(void)
{
	// ���ʂ̏I������
	CBasemode::Uninit();

	// �e��|�C���^���J��
	ReleaseBg();
	ReleasePlayer();
	ReleasePlayerManager();
	ReleaseRenderTexture();
	ReleaseRenderPolygon();
	ReleasePreparation();
	ReleasePlayerNumber();
	ReleaseYouPolygon();
	ReleaseStagePolygon();
	ReleasePlayerDataPointer();

	// �S�ẴI�u�W�F�N�g�J��
	CScene::ReleaseAll();

	// BGM���~
	CManager::GetSound()->StopSound(CHARASELECT_BGM_IDX);
}

//=============================================================================
// �L�����Z���N�g�̍X�V����
//=============================================================================
void CCharaSelect::Update(void)
{
	CDebugProc::Print(1, "�L�����Z���N�g���\n");

	// �S�Ă̍X�V����
	CScene::UpdateAll();

	int nClientId = 0;
	if (CManager::GetClient() != NULL)
	{
		CDebugProc::Print(1, "�����̃N���C�A���g�ԍ� : %d\n", CManager::GetClient()->GetClientId());
		nClientId = CManager::GetClient()->GetClientId();
	}

	// ��Ԃɂ���ď����킯
	switch (m_State[nClientId])
	{
	case STATE_SELECT:
		SelectUpdate(nClientId);
		break;
	case STATE_CHARACHANGE_TO_LEFT:
		CharaChangeToLeftUpdate(nClientId);
		break;
	case STATE_CHARACHANGE_TO_RIGHT:
		CharaChangeToRightUpdate(nClientId);
		break;
	case STATE_WAIT_PARTNER:
		WaitPartnerUpdate(nClientId);
		break;
	case STATE_STAGE_SELECT:
		StageSelectUpdate(nClientId);
		break;
	case STATE_WAIT_STAGESELECT:
		WaitStageSelectUpdate(nClientId);
		break;
	case STATE_END:
		EndUpdate();
		break;
	case STATE_END_TITLE:
		EndTitleUpdate();
		break;
	}


	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL2P)
	{// ���[�J����2P�v���C��������
	    // ��Ԃɂ���ď����킯
		switch (m_State[(nClientId + 1) % MAX_NUM_PLAYER])
		{
		case STATE_SELECT:
			SelectUpdate((nClientId + 1) % MAX_NUM_PLAYER);
			break;
		case STATE_CHARACHANGE_TO_LEFT:
			CharaChangeToLeftUpdate((nClientId + 1) % MAX_NUM_PLAYER);
			break;
		case STATE_CHARACHANGE_TO_RIGHT:
			CharaChangeToRightUpdate((nClientId + 1) % MAX_NUM_PLAYER);
			break;
		case STATE_WAIT_PARTNER:
			WaitPartnerUpdate((nClientId + 1) % MAX_NUM_PLAYER);
			break;
		case STATE_STAGE_SELECT:
			StageSelectUpdate((nClientId + 1) % MAX_NUM_PLAYER);
			break;
		case STATE_WAIT_STAGESELECT:
			WaitStageSelectUpdate((nClientId + 1) % MAX_NUM_PLAYER);
			break;
		case STATE_END:
			EndUpdate();
			break;
		case STATE_END_TITLE:
			EndTitleUpdate();
			break;
		}

		ReleaseYouPolygon();
	}
	if (CTitle::GetGameMode() != CTitle::GAMEMODE_ONLINE2P)
	{// �I�����C���v���C�ł͂Ȃ�
		return;
	}

	// ��Ԃɂ���ď����킯
	int nIdxClient = 0;
	CClient *pClient = CManager::GetClient();
	if (pClient == NULL) { return; }

	if (pClient != NULL)
	{
		nIdxClient = pClient->GetClientId();
	}

	switch (m_State[(nIdxClient + 1) % MAX_NUM_PLAYER])
	{
	case STATE_SELECT:
		SelectUpdate((nIdxClient + 1) % MAX_NUM_PLAYER);
		break;
	case STATE_CHARACHANGE_TO_LEFT:
		CharaChangeToLeftUpdate((nIdxClient + 1) % MAX_NUM_PLAYER);
		break;
	case STATE_CHARACHANGE_TO_RIGHT:
		CharaChangeToRightUpdate((nIdxClient + 1) % MAX_NUM_PLAYER);
		break;
	}

	// �T�[�o�[�֑���f�[�^��ݒ肷��
	SetDataToServer();

	// �N���C�A���g�̍X�V����
	CManager::ClientUpdate();

	// �T�[�o�[���瑗��ꂽ�f�[�^��ݒ肷��
	GetDataFromServer();
}

//=============================================================================
// �L�����Z���N�g�̕`�揈��
//=============================================================================
void CCharaSelect::Draw(void)
{
	// �J�����̐ݒ�
	if (GetCameraManager() != NULL)
	{
		GetCameraManager()->BindCamera();
	}

	// �e�N�X�`���Ƀv���C���[��`�悷��
	int nMaxPlayer = MAX_NUM_PLAYER;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{// ���[�J��1�l�v���C�Ȃ�Ε`�悷��񐔂����炷
		nMaxPlayer--;
	}
	for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
	{
		CharaDraw(nCntPlayer);
	}

	// ���ꂽ�e�N�X�`�����g���ĉ�ʂ�����������
	TextureDraw();
}


//*****************************************************************************
//
// �����p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �L�����Z���N�g�̃e�N�X�`���Ǌ��N���X��������
//=============================================================================
void CCharaSelect::CreateTexture(int nNumTex)
{
	CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
	SetTextureManager(pTextureManager);
}

//=============================================================================
// �L�����Z���N�g�̃J������������
//=============================================================================
void CCharaSelect::CreateCamera(void)
{
	CCameraManager *pCameraManager = CCameraManager::Create();
	if (pCameraManager == NULL) return;

	// �J�����𐶐�
	CCharaSelectCamera *pCamera = CCharaSelectCamera::Create(CHARASELECT_CAMERA_POSV, INITIALIZE_D3DXVECTOR3);
	if (pCamera != NULL)
	{
		pCameraManager->SetCamera(pCamera);
	}
	SetCameraManager(pCameraManager);
}

//=============================================================================
// �L�����Z���N�g�̃v���C���[��������
//=============================================================================
void CCharaSelect::CreatePlayer(void)
{
	// �v���C���[�𐶐�����
	int nMaxPlayer = MAX_NUM_PLAYER;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{// ���[�J��1�l�v���C�Ȃ�ΐl�������炵�Ă���
		nMaxPlayer--;
	}

	float fAddRot = 0.0f;
	for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
	{
		for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
		{
			// ���W�̌v�Z
			D3DXVECTOR3 Pos = D3DXVECTOR3(sinf(m_fSelectRot[nCntPlayer] - fAddRot) * CHARASELECT_CIRCLE_RADIUS, 0.0f, cosf(m_fSelectRot[nCntPlayer] - fAddRot) * CHARASELECT_CIRCLE_RADIUS);

			// �v���C���[�̐���
			m_pPlayer[nCntPlayer][nCntType] = m_pPlayerManager[nCntPlayer][nCntType]->SetPlayer(Pos,
				D3DXVECTOR3(0.0f, -D3DX_PI, 0.0f), nCntPlayer);

			// ����̐���
			if (m_pPlayerPedestal[nCntPlayer][nCntType] == NULL)
			{
				m_pPlayerPedestal[nCntPlayer][nCntType] = CScene3D::Create(Pos, INITIALIZE_D3DXVECTOR3,
					CHARASELECT_PEDESTAL_COL_INI, CHARASELECT_PEDESTAL_WIDTH_INI, CHARASELECT_PEDESTAL_HEIGHT_INI);
				if (m_pPlayerPedestal[nCntPlayer][nCntType] != NULL)
				{
					ChangePedestalTexPos(nCntPlayer, nCntType);
					m_pPlayerPedestal[nCntPlayer][nCntType]->BindTexture(GetTextureManager()->GetTexture(CHARASELECT_PEDESTAL_TEXIDX));
				}
			}

			// �~���̐���
			if (m_pCharaCylinder[nCntPlayer][0] == NULL)
			{
				m_pCharaCylinder[nCntPlayer][0] = CCharaCylinder::Create(Pos, INITIALIZE_D3DXVECTOR3,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f), 150.0f, 70.0f, 20, 1, (D3DX_PI * 0.1f), 0.0f, (D3DX_PI * 0.01f));
			}

			// ��]�ʉ��Z
			fAddRot += D3DX_PI * 2.0f / m_nNumPlayerData;
			if (fAddRot > D3DX_PI)
			{
				fAddRot -= D3DX_PI * 2.0f;
			}
		}
	}
}

//=============================================================================
// �L�����Z���N�g�̑���|���S���̃e�N�X�`�����W�����炷����
//=============================================================================
void CCharaSelect::ChangePedestalTexPos(int nCntPlayer, int nCntType)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pPlayerPedestal[nCntPlayer][nCntType]->GetVtxBuff();
	if (pVtxBuff == NULL) { return; }

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.25f * nCntType, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f + (0.25f * nCntType), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.25f * nCntType, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f + (0.25f * nCntType), 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();
}

//=============================================================================
// �L�����Z���N�g�̃X�e�[�W�̎�ޕ\���p�̃|���S���𐶐����鏈��
//=============================================================================
void CCharaSelect::CreateStagePolygon(void)
{
	D3DXVECTOR3 StagePolygonPos = CHARASELECT_STAGEPOLYGON_POS_INI;
	D3DXCOLOR StagePolygonCol = CHARASELECT_STAGEPOLYGON_COL_SELECT;
	float fStagePolygonWidth = CHARASELECT_STAGEPOLYGON_WIDTH_SELECT;
	float fStagePolygonHeight = CHARASELECT_STAGEPOLYGON_HEIGHT_SELECT;
	float fStagePolygonPosXDef = StagePolygonPos.x;

	for (int nCntType = 0; nCntType < CGame::MAPTYPE_MAX; nCntType++)
	{
		m_apStage[nCntType] = CScene2DFlash::Create(StagePolygonPos, StagePolygonCol,
			fStagePolygonWidth, fStagePolygonHeight, 0.0f, CScene2DFlash::STATE_NONE);
		if (m_apStage[nCntType] != NULL)
		{
			m_apStage[nCntType]->BindTexture(GetTextureManager()->GetTexture(6));
		}

		StagePolygonPos.x += CHARASELECT_STAGEPOLYGON_INTERVAL_X;
		if (nCntType == 0)
		{
			StagePolygonCol = CHARASELECT_STAGEPOLYGON_COL_NONE;
			fStagePolygonWidth = CHARASELECT_STAGEPOLYGON_WIDTH_NONE;
			fStagePolygonHeight = CHARASELECT_STAGEPOLYGON_HEIGHT_NONE;
		}
	}
}

//=============================================================================
// �L�����Z���N�g�̃v���C���[�f�[�^�p�̃|�C���^�𐶐����鏈��
//=============================================================================
void CCharaSelect::CreatePlayerDataPointer(void)
{
	// �t�@�C����
	m_aPlayerDataFileName = new char*[m_nNumPlayerData];
	if (m_aPlayerDataFileName != NULL)
	{
		for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
		{
			m_aPlayerDataFileName[nCntType] = new char[256];
			strcpy(m_aPlayerDataFileName[nCntType], "\0");
		}
	}

	// �v���C���[
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = new CPlayer*[m_nNumPlayerData];
		for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
		{
			m_pPlayer[nCntPlayer][nCntType] = NULL;
		}
	}

	// ����|���S��
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_pPlayerPedestal[nCntPlayer] = new CScene3D*[m_nNumPlayerData];
		for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
		{
			m_pPlayerPedestal[nCntPlayer][nCntType] = NULL;
		}
	}

	// �~��
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_pCharaCylinder[nCntPlayer] = new CCharaCylinder*[m_nNumPlayerData];
		for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
		{
			m_pCharaCylinder[nCntPlayer][nCntType] = NULL;
		}
	}

	// �v���C���[�}�l�[�W���[
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_pPlayerManager[nCntPlayer] = new CPlayerManager*[m_nNumPlayerData];
		for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
		{
			m_pPlayerManager[nCntPlayer][nCntType] = NULL;
		}
	}
}

//=============================================================================
// �L�����Z���N�g�̃����_�����O�p�̃e�N�X�`����������
//=============================================================================
void CCharaSelect::CreateRenderTexture(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	LPDIRECT3DSURFACE9 pRenderMT = NULL;
	LPDIRECT3DSURFACE9 pBuffMT = NULL;
	D3DXVECTOR3 PolygonPos[MAX_NUM_PLAYER] = { CHARASELECT_RENDERERPOLYGON_0_POS_INI, CHARASELECT_RENDERERPOLYGON_1_POS_INI };
	D3DXCOLOR PolygonCol[MAX_NUM_PLAYER] = { CHARASELECT_RENDERERPOLYGON_0_COL_INI, CHARASELECT_RENDERERPOLYGON_1_COL_INI };
	float fPolygonWidth[MAX_NUM_PLAYER] = { CHARASELECT_RENDERERPOLYGON_0_WIDTH_INI, CHARASELECT_RENDERERPOLYGON_1_WIDTH_INI };
	float fPolygonHeight[MAX_NUM_PLAYER] = { CHARASELECT_RENDERERPOLYGON_0_HEIGHT_INI, CHARASELECT_RENDERERPOLYGON_1_HEIGHT_INI };

	int nMaxPlayer = MAX_NUM_PLAYER;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{// ���[�J��1�l�v���C�Ȃ�΂��ׂĂ̒l��2�{�ɂ���
		nMaxPlayer--;
		PolygonPos[0].x *= 2.0f;
	}

	for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
	{// �v���C���[�̑������J��Ԃ�
		// �����_�����O�p�̃e�N�X�`�����쐬
		pDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, 1,
			D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_apRenderTexture[nCntPlayer], NULL);

		// �����_�����O�^�[�Q�b�g��ݒ�
		CManager::GetRenderer()->SetRenderTarget(m_apRenderTexture[nCntPlayer]);

		// �����_�����O�Ɏg���|���S��������
		m_apPolygonPlayer[nCntPlayer] = CScene2D::Create(PolygonPos[nCntPlayer], PolygonCol[nCntPlayer], fPolygonWidth[nCntPlayer], fPolygonHeight[nCntPlayer]);
		if (m_apPolygonPlayer[nCntPlayer] != NULL)
		{
			m_apPolygonPlayer[nCntPlayer]->BindTexture(m_apRenderTexture[nCntPlayer]);
		}
	}

	// �f�t�H���g�̃����_�����O�^�[�Q�b�g�ɖ߂�
	CManager::GetRenderer()->ResetRenderTarget();
}

//=============================================================================
// �v���C���[�}�l�[�W���[�̐���
//=============================================================================
void CCharaSelect::CreatePlayerManager(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
		{
			m_pPlayerManager[nCntPlayer][nCntType] = CPlayerManager::Create(m_aPlayerDataFileName[nCntType]);
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
// �L�����Z���N�g�̔w�i�|���S�����J������
//=============================================================================
void CCharaSelect::ReleaseBg(void)
{
	if (m_pBg != NULL)
	{
		m_pBg->Uninit();
		m_pBg = NULL;
	}
}

//=============================================================================
// �L�����Z���N�g�̃v���C���[���J������
//=============================================================================
void CCharaSelect::ReleasePlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		// �v���C���[
		if (m_pPlayer[nCntPlayer] != NULL)
		{
			for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
			{
				if (m_pPlayer[nCntPlayer][nCntType] != NULL)
				{
					m_pPlayer[nCntPlayer][nCntType]->Uninit();
					m_pPlayer[nCntPlayer][nCntType] = NULL;
				}
			}
			delete[] m_pPlayer[nCntPlayer];
			m_pPlayer[nCntPlayer] = NULL;
		}

		// ����|���S��
		if (m_pPlayerPedestal[nCntPlayer] != NULL)
		{
			for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
			{
				if (m_pPlayerPedestal[nCntPlayer][nCntType] != NULL)
				{
					m_pPlayerPedestal[nCntPlayer][nCntType]->Uninit();
					m_pPlayerPedestal[nCntPlayer][nCntType] = NULL;
				}
			}
			delete[] m_pPlayerPedestal[nCntPlayer];
			m_pPlayerPedestal[nCntPlayer] = NULL;
		}

		// �~��
		if (m_pCharaCylinder[nCntPlayer] != NULL)
		{
			for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
			{
				if (m_pCharaCylinder[nCntPlayer][nCntType] != NULL)
				{
					m_pCharaCylinder[nCntPlayer][nCntType]->Uninit();
					m_pCharaCylinder[nCntPlayer][nCntType] = NULL;
				}
			}
			delete[] m_pCharaCylinder[nCntPlayer];
			m_pCharaCylinder[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// �L�����Z���N�g�̃����_�����O�p�e�N�X�`�����J������
//=============================================================================
void CCharaSelect::ReleaseRenderTexture(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_apRenderTexture[nCntPlayer] != NULL)
		{
			m_apRenderTexture[nCntPlayer]->Release();
			m_apRenderTexture[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// �L�����Z���N�g�̃����_�����O�p�|���S�����J������
//=============================================================================
void CCharaSelect::ReleaseRenderPolygon(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_apPolygonPlayer[nCntPlayer] != NULL)
		{
			m_apPolygonPlayer[nCntPlayer]->Uninit();
			m_apPolygonPlayer[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// �L�����Z���N�g�̏������|���S�����J������
//=============================================================================
void CCharaSelect::ReleasePreparation(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_apPreparation[nCntPlayer] != NULL)
		{
			m_apPreparation[nCntPlayer]->Uninit();
			m_apPreparation[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// �L�����Z���N�g�̃v���C���[�ԍ��\���p�|���S�����J������
//=============================================================================
void CCharaSelect::ReleasePlayerNumber(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_apPlayerNumber[nCntPlayer] != NULL)
		{
			m_apPlayerNumber[nCntPlayer]->Uninit();
			m_apPlayerNumber[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// �L�����Z���N�g�̎����̔ԍ��\���p�|���S�����J������
//=============================================================================
void CCharaSelect::ReleaseYouPolygon(void)
{
	if (m_pYouPolygon != NULL)
	{
		m_pYouPolygon->Uninit();
		m_pYouPolygon = NULL;
	}
}

//=============================================================================
// �L�����Z���N�g�̃X�e�[�W�̎�ޕ\���p�|���S�����J������
//=============================================================================
void CCharaSelect::ReleaseStagePolygon(void)
{
	for (int nCntType = 0; nCntType < CGame::MAPTYPE_MAX; nCntType++)
	{
		if (m_apStage[nCntType] != NULL)
		{
			m_apStage[nCntType]->Uninit();
			m_apStage[nCntType] = NULL;
		}
	}
}

//=============================================================================
// �L�����Z���N�g�̃v���C���[�f�[�^�p�̃|�C���^���J�����鏈��
//=============================================================================
void CCharaSelect::ReleasePlayerDataPointer(void)
{
	// �v���C���[�f�[�^
	for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
	{
		if (m_aPlayerDataFileName[nCntType] != NULL)
		{
			delete[] m_aPlayerDataFileName[nCntType];
			m_aPlayerDataFileName[nCntType] = NULL;
		}
	}
	delete[] m_aPlayerDataFileName;
	m_aPlayerDataFileName = NULL;
}

//=============================================================================
// �v���C���[�}�l�[�W���[�̔j��
//=============================================================================
void CCharaSelect::ReleasePlayerManager(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
		{
			if (m_pPlayerManager[nCntPlayer][nCntType] != NULL)
			{
				m_pPlayerManager[nCntPlayer][nCntType]->Uninit();
				delete m_pPlayerManager[nCntPlayer][nCntType];
				m_pPlayerManager[nCntPlayer][nCntType] = NULL;
			}
		}
		delete[] m_pPlayerManager[nCntPlayer];
		m_pPlayerManager[nCntPlayer] = NULL;
	}
}


//*****************************************************************************
//
// �T�[�o�[�Ƃ̌�M�p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �L�����Z���N�g�̃T�[�o�[�֑���f�[�^��ݒ肷�鏈��
//=============================================================================
void CCharaSelect::SetDataToServer(void)
{
	// ���݂̏�Ԃ�ݒ�
	CManager::GetClient()->Print("%d ", (int)m_State[CManager::GetClient()->GetClientId()]);

	// ���݂̃v���C���[�ԍ���ݒ�
	CManager::GetClient()->Print("%d ", m_nSelectPlayer[CManager::GetClient()->GetClientId()]);

	// ���݂̃}�b�v�ԍ���ݒ�
	if (CManager::GetClient()->GetClientId() == 0)
	{
		CManager::GetClient()->Print("%d ", m_nSelectStage);
	}
}

//=============================================================================
// �L�����Z���N�g�̃T�[�o�[���瑗��ꂽ�f�[�^��ݒ肷�鏈��
//=============================================================================
void CCharaSelect::GetDataFromServer(void)
{
	int nWord = 0;
	char *pStr = CManager::GetClient()->GetReceiveMessage();
	if (pStr == NULL || CManager::GetClient()->GetConnected() == false)return;
	if (*pStr == *"???")return;

	// ���݂̏�Ԃ�ǂݎ��
	int nState = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	m_State[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] = (STATE)nState;

	// ���݂̃v���C���[�ԍ���ǂݎ��
	int nPlayerNumber = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	if (m_State[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] != STATE_END)
	{
		m_nSelectPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] = nPlayerNumber;
	}

	CDebugProc::Print(1, "����̏�� : %d\n", nState);
	CDebugProc::Print(1, "����̑I�����Ă���v���C���[�ԍ� : %d\n", nPlayerNumber);

	// ���݂̃}�b�v�ԍ���ǂݎ��
	if (CManager::GetClient()->GetClientId() != 0)
	{
		int nSelectStage = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
		if (m_nSelectStage != nSelectStage)
		{// ���݂̑I��ԍ��ƈقȂ��Ă���
			ChangeNoneStagePolygon(m_nSelectStage);
			m_nSelectStage = nSelectStage;
			ChangeSelectStagePolygon(m_nSelectStage);
		}
		CDebugProc::Print(1, "����̑I�����Ă���}�b�v�ԍ� : %d\n", m_nSelectStage);
	}

	// ����̏�Ԃɂ���ĉ�ʑJ�ڂ��邩���߂�
	if (nState == STATE_WAIT_PARTNER && m_State[CManager::GetClient()->GetClientId()] == STATE_WAIT_PARTNER)
	{
		// ����̏������|���S���̃e�N�X�`����ς���
		if (m_apPreparation[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] != NULL && GetTextureManager() != NULL)
		{
			m_apPreparation[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->BindTexture(GetTextureManager()->GetTexture(m_nPreparationTexIdx[1]));
		}

		// ��Ԃ�ݒ�
		if (CManager::GetClient()->GetClientId() == 0)
		{
			//ChangeState_WaitPartnerToStageSelect(CManager::GetClient()->GetClientId());
			SetState(STATE_END);
		}
		else
		{
			m_State[CManager::GetClient()->GetClientId()] = STATE_WAIT_STAGESELECT;
		}
	}
	else if (nState == STATE_WAIT_PARTNER && m_State[CManager::GetClient()->GetClientId()] != STATE_WAIT_PARTNER)
	{
		// ����̏������|���S���̃e�N�X�`����ς���
		if (m_apPreparation[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] != NULL && GetTextureManager() != NULL)
		{
			m_apPreparation[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->BindTexture(GetTextureManager()->GetTexture(m_nPreparationTexIdx[1]));
		}
	}
	else if (nState == STATE_END)
	{
		m_State[CManager::GetClient()->GetClientId()] = STATE_END;
	}
	else if (nState == STATE_END_TITLE)
	{
		m_State[CManager::GetClient()->GetClientId()] = STATE_END_TITLE;
	}
}


//*****************************************************************************
//
// ��Ԃɂ�鏈�������p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �L�����Z���N�g�̃Z���N�g��Ԃ̍X�V����
//=============================================================================
void CCharaSelect::SelectUpdate(int nIdx)
{
	CDebugProc::Print(1, "�Z���N�g���\n");
	CDebugProc::Print(1, "�����̌��݂̃L�����ԍ� : %d\n", m_nSelectPlayer[nIdx]);

	// �L�����ύX���󂯕t����
	if (m_State[nIdx] != STATE_END && m_State[nIdx] != STATE_WAIT_PARTNER)
	{
		WaitInputToChangeChara(nIdx);
		CheckCharaCylinderCol(nIdx);
	}
}

//=============================================================================
// �L�����Z���N�g�̍����̃L�����ɕύX�����Ԃ̍X�V����
//=============================================================================
void CCharaSelect::CharaChangeToLeftUpdate(int nIdx)
{
	CDebugProc::Print(1, "�����̃L�����ɕύX���\n");
	CDebugProc::Print(1, "�����̌��݂̃L�����ԍ� : %d\n", m_nSelectPlayer[nIdx]);

	// �~�`��]��i�߂�
	m_fSelectRot[nIdx] += CHARASELECT_CIRCLE_ROLLSPEED;
	if (m_fSelectRot[nIdx] < D3DX_PI)
	{
		m_fSelectRot[nIdx] -= D3DX_PI * 2.0f;
	}

	// ��]�����ʂ𑝂₷
	m_fAddSelectRot[nIdx] += CHARASELECT_CIRCLE_ROLLSPEED;
	if (m_fAddSelectRot[nIdx] > D3DX_PI * 2.0f / m_nNumPlayerData)
	{
		m_fAddSelectRot[nIdx] = 0.0f;
		m_fSelectRot[nIdx] -= CHARASELECT_CIRCLE_ROLLSPEED;
		m_State[nIdx] = STATE_SELECT;
	}

	// �v���C���[�̍��W�����ׂĈړ�������
	CircleRotation(nIdx);
}

//=============================================================================
// �L�����Z���N�g�̉E���̃L�����ɕύX�����Ԃ̍X�V����
//=============================================================================
void CCharaSelect::CharaChangeToRightUpdate(int nIdx)
{
	CDebugProc::Print(1, "�E���̃L�����ɕύX���\n");
	CDebugProc::Print(1, "�����̌��݂̃L�����ԍ� : %d\n", m_nSelectPlayer[nIdx]);

	// �~�`��]��i�߂�
	m_fSelectRot[nIdx] -= CHARASELECT_CIRCLE_ROLLSPEED;
	if (m_fSelectRot[nIdx] < -D3DX_PI)
	{
		m_fSelectRot[nIdx] += D3DX_PI * 2.0f;
	}

	// ��]�����ʂ𑝂₷
	m_fAddSelectRot[nIdx] += CHARASELECT_CIRCLE_ROLLSPEED;
	if (m_fAddSelectRot[nIdx] > D3DX_PI * 2.0f / m_nNumPlayerData)
	{
		m_fAddSelectRot[nIdx] = 0.0f;
		m_fSelectRot[nIdx] += CHARASELECT_CIRCLE_ROLLSPEED;
		m_State[nIdx] = STATE_SELECT;
	}

	// �v���C���[�̍��W�����ׂĈړ�������
	CircleRotation(nIdx);
}

//=============================================================================
// �L�����Z���N�g�̑���̃L�����Z���N�g��ҋ@�����Ԃ̍X�V����
//=============================================================================
void CCharaSelect::WaitPartnerUpdate(int nIdx)
{
	CDebugProc::Print(1, "����̃L�����Z���N�g�ҋ@���\n");
	CDebugProc::Print(1, "�����̌��݂̃L�����ԍ� : %d\n", m_nSelectPlayer[nIdx]);
	CDebugProc::Print(1, "����̌��݂̃L�����ԍ� : %d\n", m_nSelectPlayer[(nIdx + 1) % MAX_NUM_PLAYER]);

	// ���[�J��2�l�v���C����Ȃ��Ȃ炱�̐揈�����Ȃ�
	if (CTitle::GetGameMode() != CTitle::GAMEMODE_LOCAL2P) { return; }

	// �ǂ�����v���C���[�I�����I����Ă���Ȃ�I������
	if (m_State[0] == STATE_WAIT_PARTNER && m_State[1] == STATE_WAIT_PARTNER)
	{
		m_State[0] = m_State[1] = STATE_END;
	}

	// �^�C�g���ɖ߂鏈��
	CInputKeyboard *pKey = CManager::GetKeyboard();
	CFade *pFade = CManager::GetFade();
	if (pKey == NULL || pFade == NULL) return;
	if (pFade->GetFade() != CFade::FADE_NONE) { return; }
	if (pKey->GetTrigger(DIK_BACK) == true ||
		CManager::GetXInput()->GetTrigger(nIdx, CXInput::XIJS_BUTTON_5) == true)
	{// �߂�{�^���������ꂽ
		m_State[nIdx] = STATE_END_TITLE;
		m_State[(nIdx + 1) % MAX_NUM_PLAYER] = STATE_END_TITLE;
	}
}

//=============================================================================
// �L�����Z���N�g�̃X�e�[�W�I����Ԃ̍X�V����
//=============================================================================
void CCharaSelect::StageSelectUpdate(int nIdx)
{
	CDebugProc::Print(1, "�}�b�v�Z���N�g�ҋ@���\n");
	CDebugProc::Print(1, "���ݑI������Ă���ԍ� : %d\n", m_nSelectStage);
	WaitInputToStageSelect(nIdx);
}

//=============================================================================
// �L�����Z���N�g�̃X�e�[�W�I����҂��Ă����Ԃ̍X�V����
//=============================================================================
void CCharaSelect::WaitStageSelectUpdate(int nIdx)
{
	CDebugProc::Print(1, "����̃}�b�v�Z���N�g�ҋ@���\n");
}

//=============================================================================
// �L�����Z���N�g�̏I����Ԃ̍X�V����
//=============================================================================
void CCharaSelect::EndUpdate(void)
{
	CDebugProc::Print(1, "�I�����\n");
	CFade *pFade = CManager::GetFade();
	if (pFade == NULL) return;

	m_nStateCounter++;
	if (m_nStateCounter % CHARASELECT_MODECHANGE_TIMING == 0)
	{// �J�E���^�[�����l�ȏ�ɂȂ���
		if (pFade->GetFade() == CFade::FADE_NONE)
		{// �܂��t�F�[�h���J�n����Ă��Ȃ�
			pFade->SetFade(CManager::MODE_TUTORIAL);
		}
	}
}

//=============================================================================
// �L�����Z���N�g�̃^�C�g���ɖ߂�I����Ԃ̍X�V����
//=============================================================================
void CCharaSelect::EndTitleUpdate(void)
{
	CDebugProc::Print(1, "�^�C�g���ɖ߂�I�����\n");
	CFade *pFade = CManager::GetFade();
	if (pFade == NULL) return;

	if (pFade->GetFade() == CFade::FADE_NONE)
	{// �܂��t�F�[�h���J�n����Ă��Ȃ�
		pFade->SetFade(CManager::MODE_TITLE);
		CManager::ReleaseClient();
	}
}

//=============================================================================
// �L�����Z���N�g�̃L�����ԍ���ς��邽�߂̓��͑ҋ@����
//=============================================================================
void CCharaSelect::WaitInputToChangeChara(int nIdx)
{
	// �I�����C���v���C�̏ꍇ����������U��ꂽ�ԍ�����Ȃ��Ȃ珈�����Ȃ�
	int nControllerIdx = nIdx;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{
		CClient *pClient = CManager::GetClient();
		if (pClient == NULL) { return; }
		if (pClient->GetClientId() != nIdx) { return; }
		nControllerIdx = 0;
	}

	CInputKeyboard *pKey = CManager::GetKeyboard();
	CFade *pFade = CManager::GetFade();
	if (pKey == NULL || pFade == NULL) return;
	if (pFade->GetFade() != CFade::FADE_NONE) { return; }

	if (pKey->GetTrigger(DIK_A) == true ||
		CManager::GetXInput()->GetPress(nControllerIdx, CXInput::XIJS_BUTTON_2) == true ||
		CManager::GetXInput()->GetPress(nControllerIdx, CXInput::XIJS_BUTTON_18) == true)
	{// �������̓��͂����ꂽ
		m_nSelectPlayer[nIdx] = (m_nSelectPlayer[nIdx] + (m_nNumPlayerData - 1)) % m_nNumPlayerData;
		m_State[nIdx] = STATE_CHARACHANGE_TO_RIGHT;
		CManager::GetSound()->PlaySound(CHARASELECT_SE_SELECT_IDX);
	}
	else if (pKey->GetTrigger(DIK_D) == true ||
		CManager::GetXInput()->GetPress(nControllerIdx, CXInput::XIJS_BUTTON_3) == true ||
		CManager::GetXInput()->GetPress(nControllerIdx, CXInput::XIJS_BUTTON_19) == true)
	{// �E�����̓��͂����ꂽ
		m_nSelectPlayer[nIdx] = (m_nSelectPlayer[nIdx] + 1) % m_nNumPlayerData;
		m_State[nIdx] = STATE_CHARACHANGE_TO_LEFT;
		CManager::GetSound()->PlaySound(CHARASELECT_SE_SELECT_IDX);
	}
	else if (pKey->GetTrigger(DIK_RETURN) == true ||
		CManager::GetXInput()->GetTrigger(nControllerIdx, CXInput::XIJS_BUTTON_11) == true)
	{// ����{�^���������ꂽ
		if (m_State[(nIdx + 1) % MAX_NUM_PLAYER] != STATE_WAIT_PARTNER)
		{// �܂����肪�v���C���[�̎�ނ����肵�Ă��Ȃ�
			if (m_apPreparation[nIdx] != NULL && GetTextureManager() != NULL)
			{
				m_apPreparation[nIdx]->BindTexture(GetTextureManager()->GetTexture(m_nPreparationTexIdx[1]));
			}
			m_State[nIdx] = STATE_WAIT_PARTNER;
			CManager::GetSound()->PlaySound(CHARASELECT_SE_DECIDE_IDX);
			if (m_pCharaCylinder[nIdx][0] != NULL)
			{
				m_pCharaCylinder[nIdx][0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.3f));
			}
		}
		else if (m_nSelectPlayer[nIdx] != m_nSelectPlayer[(nIdx + 1) % MAX_NUM_PLAYER])
		{// ���łɑ��肪�I�񂾔ԍ��ł͂Ȃ�
			if (m_apPreparation[nIdx] != NULL && GetTextureManager() != NULL)
			{
				m_apPreparation[nIdx]->BindTexture(GetTextureManager()->GetTexture(m_nPreparationTexIdx[1]));
			}
			m_State[nIdx] = STATE_WAIT_PARTNER;
			CManager::GetSound()->PlaySound(CHARASELECT_SE_DECIDE_IDX);
			if (m_pCharaCylinder[nIdx][0] != NULL)
			{
				m_pCharaCylinder[nIdx][0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.3f));
			}
		}
		else
		{// �L�����Z���N�g���ł��Ȃ�����
			CManager::GetSound()->PlaySound(CHARASELECT_SE_NOTDECIDE_IDX);
		}

		if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
		{// ���[�J���v���C�Ȃ��
			m_State[nIdx] = STATE_END;
			CManager::GetSound()->PlaySound(CHARASELECT_SE_DECIDE_IDX);
			if (m_pCharaCylinder[nIdx][0] != NULL)
			{
				m_pCharaCylinder[nIdx][0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.3f));
			}
		}
	}
	else if (pKey->GetTrigger(DIK_BACK) == true ||
		CManager::GetXInput()->GetTrigger(nIdx, CXInput::XIJS_BUTTON_5) == true)
	{// �߂�{�^���������ꂽ
		m_State[nIdx] = STATE_END_TITLE;
		m_State[(nIdx + 1) % MAX_NUM_PLAYER] = STATE_END_TITLE;
	}
}

//=============================================================================
// �L�����Z���N�g�̃X�e�[�W�̎�ޔԍ���ς��邽�߂̓��͑ҋ@����
//=============================================================================
void CCharaSelect::WaitInputToStageSelect(int nIdx)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	if (pKey == NULL) return;

	if (pKey->GetTrigger(DIK_A) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_2) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_18) == true ||
		pKey->GetRepeat(DIK_A) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_2) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_18) == true)
	{// �������̓��͂����ꂽ
		ChangeNoneStagePolygon(m_nSelectStage);
		m_nSelectStage = (m_nSelectStage + (CGame::MAPTYPE_MAX - 1)) % CGame::MAPTYPE_MAX;
		ChangeSelectStagePolygon(m_nSelectStage);
		CManager::GetSound()->PlaySound(CHARASELECT_SE_SELECT_IDX);
	}
	else if (pKey->GetTrigger(DIK_D) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_3) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_19) == true ||
		pKey->GetRepeat(DIK_D) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_3) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_19) == true)
	{// �E�����̓��͂����ꂽ
		ChangeNoneStagePolygon(m_nSelectStage);
		m_nSelectStage = (m_nSelectStage + 1) % CGame::MAPTYPE_MAX;
		ChangeSelectStagePolygon(m_nSelectStage);
		CManager::GetSound()->PlaySound(CHARASELECT_SE_SELECT_IDX);
	}
	else if (pKey->GetTrigger(DIK_RETURN) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// ����{�^���������ꂽ
		m_State[nIdx] = STATE_END;
		CManager::GetSound()->PlaySound(CHARASELECT_SE_DECIDE_IDX);
	}
}

//=============================================================================
// �L�����Z���N�g�̃X�e�[�W�I��p�|���S����I������Ă��Ȃ���Ԃɂ���
//=============================================================================
void CCharaSelect::ChangeNoneStagePolygon(int nSelect)
{
	if (m_apStage[nSelect] != NULL)
	{
		m_apStage[nSelect]->SetState(CScene2DFlash::STATE_NONE);
		m_apStage[nSelect]->SetScale(CHARASELECT_STAGEPOLYGON_WIDTH_NONE, CHARASELECT_STAGEPOLYGON_HEIGHT_NONE);
		m_apStage[nSelect]->SetCol(CHARASELECT_STAGEPOLYGON_COL_NONE);
		m_apStage[nSelect]->SetVtxBuffValue();
	}
}

//=============================================================================
// �L�����Z���N�g�̃X�e�[�W�I��p�|���S����I�����ꂽ��Ԃɂ���
//=============================================================================
void CCharaSelect::ChangeSelectStagePolygon(int nSelect)
{
	if (m_apStage[nSelect] != NULL)
	{
		m_apStage[nSelect]->SetState(CScene2DFlash::STATE_SELECT);
		m_apStage[nSelect]->SetScale(CHARASELECT_STAGEPOLYGON_WIDTH_SELECT, CHARASELECT_STAGEPOLYGON_HEIGHT_SELECT);
		m_apStage[nSelect]->SetCol(CHARASELECT_STAGEPOLYGON_COL_SELECT);
		m_apStage[nSelect]->SetVtxBuffValue();
	}
}

//=============================================================================
// �L�����Z���N�g�̉~���̐F��ς��邩�ǂ������肷�鏈��
//=============================================================================
void CCharaSelect::CheckCharaCylinderCol(int nIdx)
{
	if (m_State[(nIdx + 1) % MAX_NUM_PLAYER] == STATE_WAIT_PARTNER &&
		m_nSelectPlayer[nIdx] == m_nSelectPlayer[(nIdx + 1) % MAX_NUM_PLAYER])
	{// ���肪�����I�������v���C���[�ԍ�������
		if (m_pCharaCylinder[nIdx][0] != NULL)
		{
			if (m_pCharaCylinder[nIdx][0]->GetCol() != D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.3f))
			{
				m_pCharaCylinder[nIdx][0]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.3f));
			}
		}
	}
	else
	{// �I������Ă��Ȃ��ԍ��Ȃ��
		if (m_pCharaCylinder[nIdx][0] != NULL)
		{
			if (m_pCharaCylinder[nIdx][0]->GetCol() != D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f))
			{
				m_pCharaCylinder[nIdx][0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
			}
		}
	}
}

//=============================================================================
// �L�����Z���N�g�̉~�`�Ƀv���C���[���ړ������鏈��
//=============================================================================
void CCharaSelect::CircleRotation(int nIdx)
{
	float fAddRot = 0.0f;
	for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(sinf(m_fSelectRot[nIdx] - fAddRot) * CHARASELECT_CIRCLE_RADIUS, 0.0f, cosf(m_fSelectRot[nIdx] - fAddRot) * CHARASELECT_CIRCLE_RADIUS);

		// �v���C���[
		if (m_pPlayer[nIdx][nCntType] != NULL)
		{
			m_pPlayer[nIdx][nCntType]->SetPos(Pos);
		}

		// ����|���S��
		if (m_pPlayerPedestal[nIdx][nCntType] != NULL)
		{
			m_pPlayerPedestal[nIdx][nCntType]->SetPos(Pos);
		}
		//m_pCharaCylinder[nIdx][nCntType]->SetPos(Pos);

		// ��]�ʉ��Z
		fAddRot += D3DX_PI * 2.0f / m_nNumPlayerData;
		if (fAddRot > D3DX_PI)
		{
			fAddRot -= D3DX_PI * 2.0f;
		}
	}
}

//=============================================================================
// ����̃L�����Z���N�g��҂�Ԃ���X�e�[�W�I����҂�Ԃɂ��鏈��
//=============================================================================
void CCharaSelect::ChangeState_WaitPartnerToStageSelect(int nIdx)
{
	// ��Ԃ�ݒ�
	m_State[nIdx] = STATE_STAGE_SELECT;

	// �L�����Z���N�g�p�̃|���S����j��
	ReleasePlayer();
	ReleaseRenderPolygon();
	ReleasePreparation();
	ReleasePlayerNumber();
	ReleaseYouPolygon();

	// �X�e�[�W�I��p�̃|���S���𐶐�
	CreateStagePolygon();
}


//*****************************************************************************
//
// �`��p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �L�����Z���N�g�̃v���C���[�`�揈��
//=============================================================================
void CCharaSelect::CharaDraw(int nCntTex)
{
	if (m_apRenderTexture[nCntTex] == NULL) { return; }

	// �����_�����O�^�[�Q�b�g���e�N�X�`���ɐݒ�
	CManager::GetRenderer()->SetRenderTarget(m_apRenderTexture[nCntTex]);

	//���C�g�}�l�[�W���[�̐���
	CLightManager *pLightManager = NULL;
	pLightManager = CLightManager::Create(1);
	pLightManager->SettingLight(CDirectionalLight::Create(D3DXVECTOR3(-0.10f, 0.63f, -0.77f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)), 0);

	// �v���C���[
	if (m_pPlayer[nCntTex] != NULL)
	{
		for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
		{// �v���C���[�̎�ނ̑������J��Ԃ�
			m_pPlayer[nCntTex][nCntType]->Draw();
		}
	}

	// ����|���S��
	DWORD Lighting;
	CManager::GetRenderer()->GetDevice()->GetRenderState(D3DRS_LIGHTING, &Lighting);
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	if (m_pPlayerPedestal[nCntTex] != NULL)
	{
		for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
		{// �v���C���[�̎�ނ̑������J��Ԃ�
			m_pPlayerPedestal[nCntTex][nCntType]->Draw();
		}
	}
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_LIGHTING, Lighting);

	// �~��
	if (m_pCharaCylinder[nCntTex] != NULL)
	{
		for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
		{// �v���C���[�̎�ނ̑������J��Ԃ�
			if (m_pCharaCylinder[nCntTex][nCntType] != NULL)
			{
				m_pCharaCylinder[nCntTex][nCntType]->Draw();
			}
		}
	}

	// �f�t�H���g�̃����_�����O�^�[�Q�b�g�ɖ߂�
	CManager::GetRenderer()->ResetRenderTarget();

	//���C�g�}�l�[�W���[�̔j��
	if (pLightManager != NULL)
	{
		pLightManager->Uninit();
		delete pLightManager;
		pLightManager = NULL;
	}
}

//=============================================================================
// �L�����Z���N�g�̕`�悵���e�N�X�`���Ńo�b�N�o�b�t�@�ɕ`�悷�鏈��
//=============================================================================
void CCharaSelect::TextureDraw()
{
	// �����_�����O�^�[�Q�b�g�����ɖ߂�
	CManager::GetRenderer()->ResetRenderTarget();

	// �w�i�|���S����`��
	if (m_pBg != NULL)
	{
		m_pBg->Draw();
	}

	// �|���S�����Q���`�悷��
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_apPolygonPlayer[nCntPlayer] != NULL)
		{
			m_apPolygonPlayer[nCntPlayer]->Draw();
		}
	}

	// �������|���S����`�悷��
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_apPreparation[nCntPlayer] != NULL)
		{
			m_apPreparation[nCntPlayer]->Draw();
		}
	}

	// �v���C���[�ԍ��\���p�|���S����`�悷��
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_apPlayerNumber[nCntPlayer] != NULL)
		{
			m_apPlayerNumber[nCntPlayer]->Draw();
		}
	}

	// �����̔ԍ��\���p�|���S���p
	if (m_pYouPolygon != NULL)
	{
		// �����̃N���C�A���g�ԍ��ɂ���č��W��ݒ�
		if (CManager::GetClient() != NULL)
		{
			D3DXVECTOR3 YouPolygonPos = m_pYouPolygon->GetPos();
			float fPosX = m_fYouPolygonPosXDef + (SCREEN_WIDTH / 2.0f) * CManager::GetClient()->GetClientId();
			YouPolygonPos.x = fPosX;
			m_pYouPolygon->SetPos(YouPolygonPos);
			m_pYouPolygon->SetVtxBuffPos();
		}

		m_pYouPolygon->Draw();
	}

	// �X�e�[�W�I��p�̃|���S����`��
	for (int nCntType = 0; nCntType < CGame::MAPTYPE_MAX; nCntType++)
	{
		if (m_apStage[nCntType] != NULL)
		{
			m_apStage[nCntType]->Draw();
		}
	}
}


//*****************************************************************************
//
// �X�N���v�g�ǂݍ��ݗp�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �L�����Z���N�g�̃V�X�e������ǂݍ���
//=============================================================================
void CCharaSelect::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(CHARASELECT_SYSTEM_FILENAME);
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
// �L�����Z���N�g�̃V�X�e�������t�@�C������ǂݍ���
//=============================================================================
void CCharaSelect::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
{
	int nCntTex = 0;
	int nCntPreTex = 0;
	int nCntPlayerNumTex = 0;
	int nCntPre = 0;
	int nCntPlayerNum = 0;
	int nCntPlayerData = 0;

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
		else if (CFunctionLib::Memcmp(pStr, NUM_PLAYERDATA) == 0)
		{// �ǂݍ��ރv���C���[�f�[�^�̑���������
			m_nNumPlayerData = CFunctionLib::ReadInt(pStr, NUM_PLAYERDATA);
			CreatePlayerDataPointer();
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYERDATA_FILENAME) == 0)
		{// �ǂݍ��ރv���C���[�f�[�^�̃t�@�C����������
			LoadPlayerDataFileName(pStr, nCntPlayerData);
			nCntPlayerData++;
		}
		else if (CFunctionLib::Memcmp(pStr, BGPOLYGONSET) == 0)
		{// �w�i�p�|���S����񂾂���
			LoadBgPolygon(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, PREPARATION_TEX_IDX) == 0)
		{// �������|���S���̎g�p����e�N�X�`���ԍ���񂾂���
			m_nPreparationTexIdx[nCntPreTex] = CFunctionLib::ReadInt(pStr, PREPARATION_TEX_IDX);
			nCntPreTex++;
		}
		else if (CFunctionLib::Memcmp(pStr, PREPARATIONSET) == 0)
		{// �������|���S����񂾂���
			LoadPreparation(pFileLoader, pStr, nCntPre);
			nCntPre++;
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYERNUMBER_TEX_IDX) == 0)
		{// �v���C���[�ԍ��\���|���S���̎g�p����e�N�X�`���ԍ���񂾂���
			m_nPlayerNumberTexIdx[nCntPlayerNumTex] = CFunctionLib::ReadInt(pStr, PLAYERNUMBER_TEX_IDX);
			nCntPlayerNumTex++;
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYERNUMBERSET) == 0)
		{// �v���C���[�ԍ��\���|���S����񂾂���
			LoadPlayerNumber(pFileLoader, pStr, nCntPlayerNum);
			nCntPlayerNum++;
		}
		else if (CFunctionLib::Memcmp(pStr, YOUPOLYGONSET) == 0)
		{// �����̔ԍ��\���|���S����񂾂���
			LoadYouPolygon(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�t�@�C���I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �L�����Z���N�g�̓ǂݍ��ރe�N�X�`���t�@�C������ǂݍ���
//=============================================================================
void CCharaSelect::LoadTexFileName(char *pStr, int nCntTex)
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
// �L�����Z���N�g�̓ǂݍ��ރv���C���[�f�[�^�̃t�@�C������ǂݍ���
//=============================================================================
void CCharaSelect::LoadPlayerDataFileName(char *pStr, int nCntPlayerData)
{
	strcpy(m_aPlayerDataFileName[nCntPlayerData], CFunctionLib::ReadString(pStr, m_aPlayerDataFileName[nCntPlayerData], PLAYERDATA_FILENAME));
}

//=============================================================================
// �L�����Z���N�g�̔w�i�|���S���̒l��ǂݍ���
//=============================================================================
void CCharaSelect::LoadBgPolygon(CFileLoader *pFileLoader, char *pStr)
{
	int nBgTexIdx = 0;
	D3DXVECTOR3 BgPos = CHARASELECTBG_POS_INI;
	D3DXCOLOR BgCol = CHARASELECTBG_COL_INI;
	float fBgWidth = CHARASELECTBG_WIDTH_INI;
	float fBgHeight = CHARASELECTBG_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			nBgTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			BgPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			BgCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			fBgWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			fBgHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_BGPOLYGONSET) == 0)
		{// �w�i�|���S�����I���̍��}������
			break;
		}
	}

	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{// ���[�J��1�l�v���C�Ȃ�Δw�i�e�N�X�`���ԍ���ς��Ă���
		nBgTexIdx = CHARASELECTBG_TEXIDX;
	}

	// �|���S������
	m_pBg = CScene2D::Create(BgPos, BgCol, fBgWidth, fBgHeight);
	if (m_pBg != NULL && GetTextureManager() != NULL)
	{
		m_pBg->BindTexture(GetTextureManager()->GetTexture(nBgTexIdx));
	}
}

//=============================================================================
// �L�����Z���N�g�̏������|���S���̒l��ǂݍ���
//=============================================================================
void CCharaSelect::LoadPreparation(CFileLoader *pFileLoader, char *pStr, int nCntPre)
{
	D3DXVECTOR3 PreparationPos = CHARASELECTPREPARATION_0_POS_INI;
	D3DXCOLOR PreparationCol = CHARASELECTPREPARATION_0_COL_INI;
	float fPreparationWidth = CHARASELECTPREPARATION_0_WIDTH_INI;
	float fPreparationHeight = CHARASELECTPREPARATION_0_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			PreparationPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			PreparationCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			fPreparationWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			fPreparationHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_PREPARATIONSET) == 0)
		{// �������|���S�����I���̍��}������
			break;
		}
	}

	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P && nCntPre >= MAX_NUM_PLAYER - 1)
	{// ���[�J��1�l�v���C�Ȃ��2P�p�̃|���S���͐������Ȃ�
		return;
	}

	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{// ���[�J��1�l�v���C�Ȃ�΍��W�����炵�Ă���
		PreparationPos.x *= 2.0f;
	}

	// �|���S������
	m_apPreparation[nCntPre] = CScene2D::Create(PreparationPos, PreparationCol, fPreparationWidth, fPreparationHeight);
	if (m_apPreparation[nCntPre] != NULL && GetTextureManager() != NULL)
	{
		m_apPreparation[nCntPre]->BindTexture(GetTextureManager()->GetTexture(m_nPreparationTexIdx[0]));
	}
}

//=============================================================================
// �L�����Z���N�g�̃v���C���[�ԍ��\���p�|���S���̒l��ǂݍ���
//=============================================================================
void CCharaSelect::LoadPlayerNumber(CFileLoader *pFileLoader, char *pStr, int nCntPlayerNum)
{
	D3DXVECTOR3 PlayerNumberPos = CHARASELECTPLAYERNUMBER_0_POS_INI;
	D3DXCOLOR PlayerNumberCol = CHARASELECTPLAYERNUMBER_0_COL_INI;
	float fPlayerNumberWidth = CHARASELECTPLAYERNUMBER_0_WIDTH_INI;
	float fPlayerNumberHeight = CHARASELECTPLAYERNUMBER_0_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			PlayerNumberPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			PlayerNumberCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			fPlayerNumberWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			fPlayerNumberHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_PLAYERNUMBERSET) == 0)
		{// �v���C���[�ԍ��\���|���S�����I���̍��}������
			break;
		}
	}

	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P && nCntPlayerNum >= MAX_NUM_PLAYER - 1)
	{// ���[�J��1�l�v���C�Ȃ��2P�p�̃|���S���͐������Ȃ�
		return;
	}

	// �|���S������
	m_apPlayerNumber[nCntPlayerNum] = CScene2D::Create(PlayerNumberPos, PlayerNumberCol, fPlayerNumberWidth, fPlayerNumberHeight);
	if (m_apPlayerNumber[nCntPlayerNum] != NULL && GetTextureManager() != NULL)
	{
		m_apPlayerNumber[nCntPlayerNum]->BindTexture(GetTextureManager()->GetTexture(m_nPlayerNumberTexIdx[nCntPlayerNum]));
	}
}

//=============================================================================
// �L�����Z���N�g�̎����̔ԍ��\���p�|���S���̒l��ǂݍ���
//=============================================================================
void CCharaSelect::LoadYouPolygon(CFileLoader *pFileLoader, char *pStr)
{
	int nYouPolygonTexIdx = 0;
	D3DXVECTOR3 YouPolygonPos = CHARASELECT_YOUPOLYGON_POS_INI;
	D3DXCOLOR YouPolygonCol = CHARASELECT_YOUPOLYGON_COL_INI;
	float fYouPolygonWidth = CHARASELECT_YOUPOLYGON_WIDTH_INI;
	float fYouPolygonHeight = CHARASELECT_YOUPOLYGON_HEIGHT_INI;
	m_fYouPolygonPosXDef = CHARASELECT_YOUPOLYGON_POS_INI.x;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			nYouPolygonTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			YouPolygonPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			YouPolygonCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			fYouPolygonWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			fYouPolygonHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_YOUPOLYGONSET) == 0)
		{// �w�i�|���S�����I���̍��}������
			break;
		}
	}

	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{// ���[�J��1�l�v���C�Ȃ�΃|���S���͐������Ȃ�
		return;
	}

	// �|���S������
	m_pYouPolygon = CScene2D::Create(YouPolygonPos, YouPolygonCol, fYouPolygonWidth, fYouPolygonHeight);
	if (m_pYouPolygon != NULL && GetTextureManager() != NULL)
	{
		m_pYouPolygon->BindTexture(GetTextureManager()->GetTexture(nYouPolygonTexIdx));
	}
	m_fYouPolygonPosXDef = YouPolygonPos.x;
}


//*****************************************************************************
//
// �l�ύX�p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �L�����Z���N�g�̕ϐ�������������
//=============================================================================
void CCharaSelect::ClearVariable(void)
{
	m_nStateCounter = 0;
	m_nNumPlayerData = 1;
	m_aPlayerDataFileName = NULL;
	m_pYouPolygon = NULL;
	m_fYouPolygonPosXDef = 0.0f;

	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{// �v���C���[�̑������J��Ԃ�
		m_State[nCntPlayer] = STATE_SELECT;
		m_fSelectRot[nCntPlayer] = 0.0f;
		m_fAddSelectRot[nCntPlayer] = 0.0f;
		m_apPolygonPlayer[nCntPlayer] = NULL;
		m_apRenderTexture[nCntPlayer] = NULL;
		m_apPlayerNumber[nCntPlayer] = NULL;
		m_apPreparation[nCntPlayer] = NULL;
		m_nSelectPlayer[nCntPlayer] = 0;
		m_nPreparationTexIdx[nCntPlayer] = 0;
		m_nPlayerNumberTexIdx[nCntPlayer] = 0;
		m_pPlayer[nCntPlayer] = NULL;
		m_pPlayerManager[nCntPlayer] = NULL;
		m_pPlayerPedestal[nCntPlayer] = NULL;
		m_pCharaCylinder[nCntPlayer] = NULL;
	}

	for (int nCntType = 0; nCntType < CGame::MAPTYPE_MAX; nCntType++)
	{// �X�e�[�W�̎�ނ̐������J��Ԃ�
		m_apStage[nCntType] = NULL;
	}
}

//=============================================================================
// �L�����Z���N�g�̏�Ԃ�ݒ肷��
//=============================================================================
void CCharaSelect::SetState(const STATE state)
{
	m_State[CManager::GetClient()->GetClientId()] = state;
}

//=============================================================================
// �L�����Z���N�g�̃X�e�[�W�̎�ޔԍ���ݒ肷��
//=============================================================================
void CCharaSelect::SetStageType(const int nType)
{
	m_nSelectStage = nType;
}

//=============================================================================
// �L�����Z���N�g�̏�Ԃ��擾����
//=============================================================================
CCharaSelect::STATE CCharaSelect::GetState(void)
{
	return m_State[CManager::GetClient()->GetClientId()];
}

//=============================================================================
// �L�����Z���N�g�̎g�p����v���C���[�̎�ޔԍ����擾����
//=============================================================================
int CCharaSelect::GetPlayerNumber(int nIdx)
{
	return m_nSelectPlayer[nIdx];
}

//=============================================================================
// �L�����Z���N�g�̎g�p����}�b�v�̎�ޔԍ����擾����
//=============================================================================
int CCharaSelect::GetStageType(void)
{
	return m_nSelectStage;
}