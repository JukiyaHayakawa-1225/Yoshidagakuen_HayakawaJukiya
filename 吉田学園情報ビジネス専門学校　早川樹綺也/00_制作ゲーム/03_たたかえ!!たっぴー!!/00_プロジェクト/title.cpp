//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "title.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "cameraManager.h"
#include "textureManager.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "scene2D.h"
#include "number.h"
#include "sky.h"
#include "meshfield.h"
#include "light.h"
#include "lightManager.h"
#include "player.h"
#include "playerManager.h"
#include "effectManager.h"
#include "modelcreate.h"
#include "block.h"
#include "debugproc.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TITLE_SYSTEM_FILENAME               "data/TEXT/MODE/title.ini"    // �������Ɏg�p����V�X�e���t�@�C����
#define TITLE_TEAMLOGO_SOUND_TIMING         (1)                           // �`�[�����S���o���Ă��鎞�ɃT�E���h��炷�^�C�~���O
#define TITLE_FADE_SPEED                    (0.015f)                      // �t�F�[�h�̃X�s�[�h
#define TITLE_FADEOUT_TIMING                (180)                         // �`�[�����S��Ԃ���t�F�[�h�A�E�g��ԂɈڂ鎞��
#define TITLE_CHANGEMODE_TIMING             (1200)                        // �ʏ��Ԃ����ʑJ�ڂ���܂ł̎���
#define TITLE_BG_SLIDE_UV                   (0.001f)                      // �w�i�̃e�N�X�`�����W���ǂꂭ�炢��������
#define TITLE_PLAYER_PRIORITY               (6)                           // �v���C���[�̏����D�揇��
#define TITLE_BGM_IDX                       (0)                           // �^�C�g����BGM�̉��ԍ�
#define TITLE_SE_TEAMLOGO_IDX               (7)                           // �`�[�����S���o���Ƃ���SE�̉��ԍ�
#define TITLE_SE_SELECT_IDX                 (8)                           // ���ڂ�I�����Ă��鎞��SE�̉��ԍ�
#define TITLE_SE_DECIDE_IDX                 (9)                           // ����{�^���������ꂽ����SE�̉��ԍ�
#define TITLE_DASHEFFECT_IDX                (10)                          // �v���C���[�������Ă���Ƃ��̃G�t�F�N�g

// �J�����p
#define TITLECAMERA_POSV_INI                (D3DXVECTOR3(0.0f, 1000.0f, -320.0f))
#define TITLECAMERA_POSR_UNDER_Y            (60.0f)
#define TITLECAMERA_POSR_INI                (D3DXVECTOR3(0.0f, 1000.0f - TITLECAMERA_POSR_UNDER_Y, 0.0f))
#define TITLECAMERA_POS_DOWN                (3.8f)
#define TITLECAMERA_POSV_MIN_Y              (120.0f)

// �`�[�����S�p�w�i�|���S���������p
#define TITLETEAMBG_POS_INI                 (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define TITLETEAMBG_COL_INI                 (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLETEAMBG_WIDTH_INI               (SCREEN_WIDTH / 2.0f)
#define TITLETEAMBG_HEIGHT_INI              (SCREEN_HEIGHT / 2.0f)
#define TITLETEAMBG_PRIORITY                (6)

// �t�F�[�h�p�|���S���������p
#define TITLEFADE_POS_INI                   (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define TITLEFADE_COL_INI                   (D3DXCOLOR(0.0f,0.0f,0.0f,0.0f))
#define TITLEFADE_WIDTH_INI                 (SCREEN_WIDTH / 2.0f)
#define TITLEFADE_HEIGHT_INI                (SCREEN_HEIGHT / 2.0f)
#define TITLEFADE_PRIORITY                  (6)

// �`�[�����S�p�|���S���������p
#define TITLETEAMLOGO_POS_INI               (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define TITLETEAMLOGO_COL_INI               (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLETEAMLOGO_WIDTH_INI             (100.0f)
#define TITLETEAMLOGO_HEIGHT_INI            (100.0f)
#define TITLETEAMLOGO_PRIORITY              (6)

// �^�C�g�����S�������p
#define TITLELOGO_POS_INI                   (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 120.0f, 0.0f))
#define TITLELOGO_COL_INI                   (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLELOGO_WIDTH_INI                 (200.0f)
#define TITLELOGO_HEIGHT_INI                (120.0f)
#define TITLELOGO_PRIORITY                  (7)

// �v���X�ҋ@�������p
#define TITLEPRESS_POS_INI                  (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 500.0f, 0.0f))
#define TITLEPRESS_COL_INI                  (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLEPRESS_WIDTH_INI                (150.0f)
#define TITLEPRESS_HEIGHT_INI               (60.0f)
#define TITLEPRESS_PRIORITY                 (6)

// �Q�[�����[�h�w�i�|���S���������p
#define TITLEGAMEMODEBG_POS_INI             (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define TITLEGAMEMODEBG_COL_INI             (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLEGAMEMODEBG_WIDTH_INI           (SCREEN_WIDTH / 2.0f)
#define TITLEGAMEMODEBG_HEIGHT_INI          (SCREEN_HEIGHT / 2.0f)
#define TITLEGAMEMODEBG_PRIORITY            (6)

// �Q�[�����[�h���S�|���S���������p
#define TITLEGAMEMODELOGO_POS_INI           (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 200.0f, 0.0f))
#define TITLEGAMEMODELOGO_COL_INI           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLEGAMEMODELOGO_WIDTH_INI         (150.0f)
#define TITLEGAMEMODELOGO_HEIGHT_INI        (60.0f)
#define TITLEGAMEMODELOGO_INTERVAL_INI      (100.0f)
#define TITLEGAMEMODELOGO_PRIORITY          (7)

// ����|���S���������p
#define TITLEDECIDELOGO_POS_INI             (D3DXVECTOR3(980.0f, 685.0f, 0.0f))
#define TITLEDECIDELOGO_COL_INI             (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLEDECIDELOGO_WIDTH_INI           (90.0f)
#define TITLEDECIDELOGO_HEIGHT_INI          (30.0f)
#define TITLEDECIDELOGO_PRIORITY            (7)
#define TITLEDECIDELOGO_TEXIDX              (11)

// �߂郍�S�|���S���������p
#define TITLERETURNLOGO_POS_INI             (D3DXVECTOR3(1170.0f, 685.0f, 0.0f))
#define TITLERETURNLOGO_COL_INI             (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLERETURNLOGO_WIDTH_INI           (80.0f)
#define TITLERETURNLOGO_HEIGHT_INI          (28.0f)
#define TITLERETURNLOGO_PRIORITY            (7)
#define TITLERETURNLOGO_TEXIDX              (12)

// �^�C�g���n�C�X�R�A���S�������p
#define TITLE_HIGHSCORE_FALSH_COL           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLE_HIGHSCORELOGO_POS_INI         (D3DXVECTOR3(1010.0f, 475.0f, 0.0f))
#define TITLE_HIGHSCORELOGO_COL_INI         (D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))
#define TITLE_HIGHSCORELOGO_WIDTH_INI       (135.0f)
#define TITLE_HIGHSCORELOGO_HEIGHT_INI      (30.0f)
#define TITLE_HIGHSCORELOGO_TEXIDX          (13)
#define TITLE_HIGHSCORELOGO_PRIORITY        (7)

// �^�C�g���n�C�X�R�A�����������p
#define TITLE_HIGHSCORENUMBER_POS_INI       (D3DXVECTOR3(SCREEN_WIDTH - 90.0f, 555.0f, 0.0f))
#define TITLE_HIGHSCORENUMBER_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLE_HIGHSCORENUMBER_WIDTH_INI     (30.0f)
#define TITLE_HIGHSCORENUMBER_HEIGHT_INI    (25.0f)
#define TITLE_HIGHSCORENUMBER_INTERVAL_INI  (D3DXVECTOR3(-55.0f, 0.0f, 0.0f))
#define TITLE_HIGHSCORENUMBER_PRIORITY      (7)

// �^�C�g���A�C�R���������p
#define TITLE_ICON_POS_INI                  (D3DXVECTOR3(230.0f, 560.0f, 0.0f))
#define TITLE_ICON_COL_INI                  (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLE_ICON_WIDTH_INI                (130.0f)
#define TITLE_ICON_HEIGHT_INI               (130.0f)
#define TITLE_ICON_TEXIDX                   (14)
#define TITLE_ICON_PRIORITY                 (7)

// �^�C�g�����Ȃ胂�f���������p
#define TITLE_LOAD_MODEL_INTERVAL           (150.0f)
#define TITLE_LOAD_ZPOS_INI                 (3000.0f)
#define TITLE_LOAD_WIDTH                    (500.0f)
#define TITLE_LOAD_MODELIDX                 (0)
#define TITLE_LOAD_SETMODEL_NUM             (50)


// �^�C�g���z�u���������p
#define TITLE_OBJECT_MODELIDX               (1)
#define TITLE_OBJECT_NUM_MODELTYPE          (3)
#define TITLE_OBJECT_SET_WIDTH              (1300)
#define TITLE_OBJECT_SET_DEPTH              (3000)
#define TITLE_OBJECT_POS_INI                (D3DXVECTOR3(2000.0f, 560.0f, 2800.0f))
#define TITLE_OBJECT_SETMODEL_NUM           (700)

// �l�ǂݍ��ݗp�̃p�X
// �e�N�X�`���p
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// ���f���p
#define NUM_MODEL "NUM_MODEL = "
#define MODEL_FILENAME "MODEL_FILENAME = "

// �G�t�F�N�g�p
#define EFFECT_FILENAME "EFFECT_FILENAME = "

// �^�C�g�����S�p
#define TITLELOGOSET "TITLELOGOSET"
#define END_TITLELOGOSET "END_TITLELOGOSET"
#define TEX_IDX "TEX_IDX = "
#define POS "POS = "
#define COL "COL = "
#define WIDTH "WIDTH = "
#define HEIGHT "HEIGHT = "

// �v���X�ҋ@�p
#define PRESSSET "PRESSSET"
#define END_PRESSSET "END_PRESSSET"

// �`�[�����S�p
#define TEAMLOGOSET "TEAMLOGOSET"
#define END_TEAMLOGOSET "END_TEAMLOGOSET"

// �Q�[�����[�h�w�i�|���S���p
#define GAMEMODEBGSET "GAMEMODEBGSET"
#define END_GAMEMODEBGSET "END_GAMEMODEBGSET"

// �Q�[�����[�h�I���|���S���p
#define GAMEMODELOGOSET "GAMEMODELOGOSET"
#define END_GAMEMODELOGOSET "END_GAMEMODELOGOSET"
#define INTERVAL "INTERVAL = "
#define NONE "NONE"
#define END_NONE "END_NONE"
#define SELECT "SELECT"
#define END_SELECT "END_SELECT"

// �v���C���[�f�[�^�p
#define NUM_PLAYERDATA "NUM_PLAYERDATA = "
#define PLAYERDATA_FILENAME "PLAYERDATA_FILENAME = "
#define NUM_PLAYER "NUM_PLAYER = "
#define PLAYERSET "PLAYERSET"
#define END_PLAYERSET "END_PLAYERSET"
#define TYPE "TYPE = "
#define ROT "ROT = "

// ���C�g�p
#define NUM_LIGHT "NUM_LIGHT = "
#define LIGHTSET "LIGHTSET"
#define END_LIGHTSET "END_LIGHTSET"
#define DIR "DIR = "
#define DIFFUSE "DIFFUSE = "
#define AMBIENT "AMBIENT = "
#define SPECULAR "SPECULAR = "

// �n�ʗp
#define FIELDSET "FIELDSET"
#define END_FIELDSET "END_FIELDSET"
#define DEPTH "DEPTH = "
#define XBLOCK "XBLOCK = "
#define ZBLOCK "ZBLOCK = "
#define TEXU_SPLIT "TEXU_SPLIT = "
#define TEXV_SPLIT "TEXV_SPLIT = "
#define CULLING "CULLING = "
#define ADDTEX "ADDTEX = "

// ��p
#define SKYSET "SKYSET"
#define END_SKYSET "END_SKYSET"
#define RADIUS "RADIUS = "
#define ROLLSPEED "ROLLSPEED = "

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
int CTitle::m_nGameMode = CTitle::GAMEMODE_LOCAL1P;   // �Q�[�����[�h
int CTitle::m_nHighScore = 10000;                     // �Q�[���̃n�C�X�R�A

//*****************************************************************************
// CTitle�̏���
//*****************************************************************************
//=============================================================================
// �^�C�g���̃R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{

}

//=============================================================================
// �^�C�g���̃f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// �^�C�g���̐�������
//=============================================================================
CTitle *CTitle::Create()
{
	CTitle *pTitle = NULL;
	if (pTitle == NULL)
	{
		pTitle = new CTitle;
		if (pTitle != NULL)
		{
			pTitle->Init();
		}
	}
	return pTitle;
}

//=============================================================================
// �^�C�g���̏���������
//=============================================================================
HRESULT CTitle::Init(void)
{
	// �ϐ�������������
	ClearVariable();

	// �V�X�e���̏�����
	LoadSystem();

	// �J�����̐���
	CreateCamera();

	// ��𐶐�����
	CreateSky();

	// �n�ʂ𐶐�����
	CreateField();

	// ���C�g�𐶐�����
	CreateLight();

	// �v���C���[�Ǌ��N���X�𐶐�����
	CreatePlayerManager();

	// �v���C���[�N���X�𐶐�����
	CreatePlayer();

	// �����Ă���悤�ȃG�t�F�N�g�𐶐�����
	CreateDashEffect();

	// ���Ȃ�Ƀu���b�N��z�u����
	CreateLoad();

	// �z�u������
	CreateObject();

	// �Q�[�����[�h������
	m_nGameMode = GAMEMODE_LOCAL1P;

	return S_OK;
}

//=============================================================================
// �^�C�g���̏I������
//=============================================================================
void CTitle::Uninit(void)
{
	// ���ʂ̏I������
	CBasemode::Uninit();

	// �e��|���S�����J��
	ReleaseFadePolygon();
	ReleaseTeamLogoPolygon();
	ReleaseTeamBgPolygon();
	ReleaseTitleLogoPolygon();
	ReleasePress();
	ReleaseGameModeBg();
	ReleaseGameModeLogo();
	ReleaseSky();
	ReleaseField();
	ReleaseLight();
	ReleasePlayer();
	ReleasePlayerManager();
	ReleasePlayerManagerPointer();
	ReleasePlayerDataPointer();
	ReleaseLightDataPointer();
	ReleaseHighScore();

	// �S�ẴI�u�W�F�N�g�J��
	CScene::ReleaseAll();

	// BGM���~
	CManager::GetSound()->StopSound(TITLE_BGM_IDX);
}

//=============================================================================
// �^�C�g���̍X�V����
//=============================================================================
void CTitle::Update(void)
{
	// ��Ԃɂ���ď����킯
	switch (m_State)
	{
	case STATE_TEAMLOGO:
		TeamLogoUpdate();
		break;
	case STATE_FADEOUT:
		FadeOutUpdate();
		break;
	case STATE_FADEIN:
		FadeInUpdate();
		break;
	case STATE_NORMAL:
		NormalUpdate();
		break;
	case STATE_GAMEMODE:
		GameModeUpdate();
		break;
	}

	// �n�ʂ�UV���W��i�߂�
	AddFieldTexUV();

	// �S�Ă̍X�V����
	CScene::UpdateAll();

	CDebugProc::Print(1, "�^�C�g�����\n");
}

//=============================================================================
// �^�C�g���̕`�揈��
//=============================================================================
void CTitle::Draw(void)
{
	// �J�����̐ݒ�
	if (GetCameraManager() != NULL)
	{
		GetCameraManager()->BindCamera();
	}

	// �V�[���N���X�̕`�揈��
	CScene::DrawAll();
}

//=============================================================================
// �^�C�g���̃e�N�X�`���Ǌ��N���X��������
//=============================================================================
void CTitle::CreateTexture(int nNumTex)
{
	CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
	SetTextureManager(pTextureManager);
}


//=============================================================================
// �^�C�g���̃J������������
//=============================================================================
void CTitle::CreateCamera(void)
{
	CCameraManager *pCameraManager = CCameraManager::Create();
	if (pCameraManager == NULL) return;

	// �J�����𐶐�
	CCamera *pCamera = CCamera::Create();
	if (pCamera != NULL)
	{
		pCamera->SetPosV(TITLECAMERA_POSV_INI);
		pCamera->SetPosR(TITLECAMERA_POSR_INI);
		pCameraManager->SetCamera(pCamera);
	}
	SetCameraManager(pCameraManager);
}

//=============================================================================
// �^�C�g���̃t�F�[�h�p�|���S����������
//=============================================================================
void CTitle::CreateFadePolygon(void)
{
	m_pFade = CScene2D::Create(TITLEFADE_POS_INI, TITLEFADE_COL_INI, TITLEFADE_WIDTH_INI, TITLEFADE_HEIGHT_INI, TITLEFADE_PRIORITY);
}

//=============================================================================
// �^�C�g���̃`�[�����S�w�i�p�|���S����������
//=============================================================================
void CTitle::CreateTeamBgPolygon(void)
{
	m_pTeamBg = CScene2D::Create(TITLETEAMBG_POS_INI, TITLETEAMBG_COL_INI, TITLETEAMBG_WIDTH_INI, TITLETEAMBG_HEIGHT_INI, TITLETEAMBG_PRIORITY);
}

//=============================================================================
// �^�C�g���̃v���X�ҋ@�p�|���S����������
//=============================================================================
void CTitle::CreatePress(void)
{
	m_pPress = CScene2DPress::Create(m_PressData.pos, m_PressData.col, m_PressData.fWidth, m_PressData.fHeight, 0.0f,
		80, 100, 4, TITLEPRESS_PRIORITY);
	if (m_pPress != NULL && GetTextureManager() != NULL)
	{
		m_pPress->BindTexture(GetTextureManager()->GetTexture(m_PressData.nTexIdx));
	}
}

//=============================================================================
// �^�C�g���̃Q�[�����[�h�w�i�p�|���S����������
//=============================================================================
void CTitle::CreateGameModeBg(void)
{
	m_pGameLogoBg = CScene2D::Create(m_GameModeBgData.pos, m_GameModeBgData.col,
		m_GameModeBgData.fWidth, m_GameModeBgData.fHeight, TITLEGAMEMODEBG_PRIORITY);
	if (m_pGameLogoBg != NULL && GetTextureManager() != NULL)
	{
		m_pGameLogoBg->BindTexture(GetTextureManager()->GetTexture(m_GameModeBgData.nTexIdx));
	}

	m_pDecide = CScene2D::Create(TITLEDECIDELOGO_POS_INI, TITLEDECIDELOGO_COL_INI, TITLEDECIDELOGO_WIDTH_INI,
		TITLEDECIDELOGO_HEIGHT_INI, TITLEDECIDELOGO_PRIORITY);
	if (m_pDecide != NULL)
	{
		m_pDecide->BindTexture(GetTextureManager()->GetTexture(TITLEDECIDELOGO_TEXIDX));
	}

	m_pRetrun = CScene2D::Create(TITLERETURNLOGO_POS_INI, TITLERETURNLOGO_COL_INI, TITLERETURNLOGO_WIDTH_INI,
		TITLERETURNLOGO_HEIGHT_INI, TITLERETURNLOGO_PRIORITY);
	if (m_pRetrun != NULL)
	{
		m_pRetrun->BindTexture(GetTextureManager()->GetTexture(TITLERETURNLOGO_TEXIDX));
	}

	if (m_pIcon == NULL)
	{
		m_pIcon = CScene2D::Create(TITLE_ICON_POS_INI, TITLE_ICON_COL_INI,
			TITLE_ICON_WIDTH_INI, TITLE_ICON_HEIGHT_INI, TITLE_ICON_PRIORITY);
		if (m_pIcon != NULL && GetTextureManager() != NULL)
		{
			m_pIcon->BindTexture(GetTextureManager()->GetTexture(TITLE_ICON_TEXIDX));
		}
	}
}

//=============================================================================
// �^�C�g���̃Q�[�����[�h�I��p�|���S����������
//=============================================================================
void CTitle::CreateGameModeLogo(void)
{
	int nGameModeTexIdx = m_GameModeLogoData.nTexIdx;
	D3DXVECTOR3 GameModeLogoPos = m_GameModeLogoData.pos;
	D3DXCOLOR GameModeLogoCol;
	float fGameModeLogoWidth;
	float fGameModeLogoHeight;
	CScene2DFlash::STATE GameModeLogoState;
	for (int nCntLogo = 0; nCntLogo < GAMEMODE_MAX; nCntLogo++)
	{
		// �l��ݒ�
		if (nCntLogo == m_nGameMode)
		{// �I�����ꂽ�ԍ�������
			GameModeLogoCol = m_GameModeLogoPoly_SelectData.col;
			fGameModeLogoWidth = m_GameModeLogoPoly_SelectData.fWidth;
			fGameModeLogoHeight = m_GameModeLogoPoly_SelectData.fHeight;
			GameModeLogoState = CScene2DFlash::STATE_SELECT;
		}
		else
		{// �I������Ă��Ȃ��ԍ�������
			GameModeLogoCol = m_GameModeLogoPoly_NoneData.col;
			fGameModeLogoWidth = m_GameModeLogoPoly_NoneData.fWidth;
			fGameModeLogoHeight = m_GameModeLogoPoly_NoneData.fHeight;
			GameModeLogoState = CScene2DFlash::STATE_NONE;
		}

		// �|���S���𐶐�
		m_apGameModeLogo[nCntLogo] = CScene2DFlash::Create(GameModeLogoPos, GameModeLogoCol,
			fGameModeLogoWidth, fGameModeLogoHeight, 0.0f, GameModeLogoState, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f), 0.01f, 4, TITLEGAMEMODELOGO_PRIORITY);
		if (m_apGameModeLogo[nCntLogo] != NULL && GetTextureManager() != NULL)
		{
			m_apGameModeLogo[nCntLogo]->BindTexture(GetTextureManager()->GetTexture(nGameModeTexIdx));
		}
		nGameModeTexIdx++;
		GameModeLogoPos.y += m_GameModeLogoData.fInterval;
	}
}

//=============================================================================
// �^�C�g���̋󐶐�����
//=============================================================================
void CTitle::CreateSky(void)
{
	m_pSky = CSky::Create(m_SkyData.pos, m_SkyData.rot, m_SkyData.col, m_SkyData.fRadius,
		m_SkyData.nXBlock, m_SkyData.nZBlock, m_SkyData.nTexIdx, m_SkyData.fRollSpeed);
	if (m_pSky != NULL && GetTextureManager() != NULL)
	{
		m_pSky->BindTexture(GetTextureManager()->GetTexture(m_SkyData.nTexIdx));
	}
}

//=============================================================================
// �^�C�g���̒n�ʐ�������
//=============================================================================
void CTitle::CreateField(void)
{
	m_pMeshField = CMeshField::Create(m_FieldData.pos, m_FieldData.rot, m_FieldData.col,
		m_FieldData.fWidth, m_FieldData.fDepth, m_FieldData.nXBlock, m_FieldData.nZBlock,
		m_FieldData.nTexUSplit, m_FieldData.nTexVSplit, NULL, m_FieldData.bCulling);
	if (m_pMeshField != NULL && GetTextureManager() != NULL)
	{
		m_pMeshField->BindTexture(GetTextureManager()->GetTexture(m_FieldData.nTexIdx));
	}
}

//=============================================================================
// �^�C�g���̃��C�g��������
//=============================================================================
void CTitle::CreateLight(void)
{
	if (m_nNumLight <= 0) { return; }

	m_pLightManager = CLightManager::Create(m_nNumLight);
	if (m_pLightManager != NULL)
	{
		CLight *pLight = NULL;
		for (int nCntLight = 0; nCntLight < m_nNumLight; nCntLight++)
		{
			pLight = CDirectionalLight::Create(m_pLightData[nCntLight].Dir, m_pLightData[nCntLight].Diffuse,
				m_pLightData[nCntLight].Ambient, m_pLightData[nCntLight].Specular);
			m_pLightManager->SettingLight(pLight, nCntLight);
		}
	}
}

//=============================================================================
// �^�C�g���̃v���C���[�𐶐����鏈��
//=============================================================================
void CTitle::CreatePlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < m_nNumPlayer; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = m_pPlayerManager[m_pPlayerData[nCntPlayer].nType]->SetPlayer(m_pPlayerData[nCntPlayer].pos, m_pPlayerData[nCntPlayer].rot, 0, TITLE_PLAYER_PRIORITY);
	}
}

//=============================================================================
// �^�C�g���̃v���C���[�Ǌ��N���X�p�̃|�C���^�𐶐����鏈��
//=============================================================================
void CTitle::CreatePlayerManagerPointer(void)
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

	// �v���C���[�}�l�[�W���[
	m_pPlayerManager = new CPlayerManager*[m_nNumPlayerData];
	for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
	{
		m_pPlayerManager[nCntType] = NULL;
	}
}

//=============================================================================
// �^�C�g���̃v���C���[�Ǌ��N���X�𐶐����鏈��
//=============================================================================
void CTitle::CreatePlayerManager(void)
{
	for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
	{
		m_pPlayerManager[nCntType] = CPlayerManager::Create(m_aPlayerDataFileName[nCntType]);
	}
}

//=============================================================================
// �^�C�g���̃v���C���[�f�[�^�p�̃|�C���^�𐶐����鏈��
//=============================================================================
void CTitle::CreatePlayerDataPointer(void)
{
	// �v���C���[
	m_pPlayer = new CPlayer*[m_nNumPlayer];
	for (int nCntType = 0; nCntType < m_nNumPlayer; nCntType++)
	{
		m_pPlayer[nCntType] = NULL;
	}

	// �v���C���[�f�[�^
	m_pPlayerData = new PLAYERDATA[m_nNumPlayer];
	for (int nCntType = 0; nCntType < m_nNumPlayer; nCntType++)
	{
		m_pPlayerData[nCntType].nType = 0;
		m_pPlayerData[nCntType].pos = INITIALIZE_D3DXVECTOR3;
		m_pPlayerData[nCntType].rot = INITIALIZE_D3DXVECTOR3;
	}
}

//=============================================================================
// �^�C�g���̃��C�g�f�[�^�p�̃|�C���^�𐶐����鏈��
//=============================================================================
void CTitle::CreateLightDataPointer(void)
{
	m_pLightData = new LIGHTDATA[m_nNumLight];
	for (int nCntLight = 0; nCntLight < m_nNumLight; nCntLight++)
	{
		m_pLightData[nCntLight].Dir = INITIALIZE_D3DXVECTOR3;
		m_pLightData[nCntLight].Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		m_pLightData[nCntLight].Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		m_pLightData[nCntLight].Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

//=============================================================================
// �^�C�g���̃n�C�X�R�A�𐶐����鏈��
//=============================================================================
void CTitle::CreateHighScore(void)
{
	// ���S�|���S������
	if (m_pHighScoreLogo == NULL)
	{
		m_pHighScoreLogo = CScene2D::Create(TITLE_HIGHSCORELOGO_POS_INI, TITLE_HIGHSCORELOGO_COL_INI,
			TITLE_HIGHSCORELOGO_WIDTH_INI, TITLE_HIGHSCORELOGO_HEIGHT_INI, TITLE_HIGHSCORELOGO_PRIORITY);
		if (m_pHighScoreLogo != NULL && GetTextureManager() != NULL)
		{
			m_pHighScoreLogo->BindTexture(GetTextureManager()->GetTexture(TITLE_HIGHSCORELOGO_TEXIDX));
		}
	}

	// �����|���S������
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(10);
	}
	if (m_pHighScore == NULL)
	{
		m_pHighScore = CNumber::Create(TITLE_HIGHSCORENUMBER_POS_INI, TITLE_HIGHSCORENUMBER_COL_INI,
			TITLE_HIGHSCORENUMBER_WIDTH_INI, TITLE_HIGHSCORENUMBER_HEIGHT_INI, TITLE_HIGHSCORENUMBER_INTERVAL_INI,
			pTexture, CTitle::GetHighScore(), CNumberPolygon::STATE_NONE, 3, 0.0f, TITLE_HIGHSCORENUMBER_PRIORITY);
	}
}

//=============================================================================
// �^�C�g���̓��Ȃ胂�f���𐶐����鏈��
//=============================================================================
void CTitle::CreateLoad(void)
{
	CModelCreate *pModelCreate = GetModelCreate();
	if (pModelCreate == NULL) { return; }

	D3DXVECTOR3 LoadPos = D3DXVECTOR3(TITLE_LOAD_WIDTH, 0.0f, TITLE_LOAD_ZPOS_INI);

	// �E��
	for (int nCntModel = 0; nCntModel < TITLE_LOAD_SETMODEL_NUM; nCntModel++)
	{
		CBlock::Create(LoadPos, INITIALIZE_D3DXVECTOR3, 0, 0, pModelCreate->GetMesh(TITLE_LOAD_MODELIDX),
			pModelCreate->GetBuffMat(TITLE_LOAD_MODELIDX), pModelCreate->GetNumMat(TITLE_LOAD_MODELIDX),
			pModelCreate->GetTexture(TITLE_LOAD_MODELIDX));
		LoadPos.z -= TITLE_LOAD_MODEL_INTERVAL;
	}

	// ���W�������l�ɐݒ�
	LoadPos.x = -TITLE_LOAD_WIDTH;
	LoadPos.z = TITLE_LOAD_ZPOS_INI;

	// ����
	for (int nCntModel = 0; nCntModel < TITLE_LOAD_SETMODEL_NUM; nCntModel++)
	{
		CBlock::Create(LoadPos, INITIALIZE_D3DXVECTOR3, 0, 0, pModelCreate->GetMesh(TITLE_LOAD_MODELIDX),
			pModelCreate->GetBuffMat(TITLE_LOAD_MODELIDX), pModelCreate->GetNumMat(TITLE_LOAD_MODELIDX),
			pModelCreate->GetTexture(TITLE_LOAD_MODELIDX));
		LoadPos.z -= TITLE_LOAD_MODEL_INTERVAL;
	}
}

//=============================================================================
// �^�C�g���̔z�u����������
//=============================================================================
void CTitle::CreateObject(void)
{
	CModelCreate *pModelCreate = GetModelCreate();
	if (pModelCreate == NULL) { return; }

	int nModelIdx = 0;
	D3DXVECTOR3 LoadPos = D3DXVECTOR3(TITLE_LOAD_WIDTH, 0.0f, TITLE_LOAD_ZPOS_INI);

	// �E��
	for (int nCntModel = 0; nCntModel < TITLE_OBJECT_SETMODEL_NUM; nCntModel++)
	{
		// �g�p���郂�f���ԍ��̐ݒ�
		nModelIdx = rand() % TITLE_OBJECT_NUM_MODELTYPE + TITLE_OBJECT_MODELIDX;

		// �z�u������W�̐ݒ�
		LoadPos.x = (float)(-(rand() % TITLE_OBJECT_SET_WIDTH) + TITLE_OBJECT_POS_INI.x);
		LoadPos.z = (float)(-(rand() % TITLE_OBJECT_SET_DEPTH) + TITLE_OBJECT_POS_INI.z);

		// ���f���z�u
		CBlock::Create(LoadPos, INITIALIZE_D3DXVECTOR3, 0, 0, pModelCreate->GetMesh(nModelIdx),
			pModelCreate->GetBuffMat(nModelIdx), pModelCreate->GetNumMat(nModelIdx),
			pModelCreate->GetTexture(nModelIdx));
	}

	// ����
	for (int nCntModel = 0; nCntModel < TITLE_OBJECT_SETMODEL_NUM; nCntModel++)
	{
		// �g�p���郂�f���ԍ��̐ݒ�
		nModelIdx = rand() % TITLE_OBJECT_NUM_MODELTYPE + TITLE_OBJECT_MODELIDX;

		// �z�u������W�̐ݒ�
		LoadPos.x = (float)((rand() % TITLE_OBJECT_SET_WIDTH) - TITLE_OBJECT_POS_INI.x);
		LoadPos.z = (float)(-(rand() % TITLE_OBJECT_SET_DEPTH) + TITLE_OBJECT_POS_INI.z);

		// ���f���z�u
		CBlock::Create(LoadPos, INITIALIZE_D3DXVECTOR3, 0, 0, pModelCreate->GetMesh(nModelIdx),
			pModelCreate->GetBuffMat(nModelIdx), pModelCreate->GetNumMat(nModelIdx),
			pModelCreate->GetTexture(nModelIdx));
	}
}

//=============================================================================
// �^�C�g���̃t�F�[�h�p�|���S���J������
//=============================================================================
void CTitle::ReleaseFadePolygon(void)
{
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		m_pFade = NULL;
	}
}

//=============================================================================
// �^�C�g���̃`�[�����S�w�i�p�|���S���J������
//=============================================================================
void CTitle::ReleaseTeamLogoPolygon(void)
{
	if (m_pTeamLogo != NULL)
	{
		m_pTeamLogo->Uninit();
		m_pTeamLogo = NULL;
	}
}

//=============================================================================
// �^�C�g���̃`�[�����S�w�i�p�|���S���J������
//=============================================================================
void CTitle::ReleaseTeamBgPolygon(void)
{
	if (m_pTeamBg != NULL)
	{
		m_pTeamBg->Uninit();
		m_pTeamBg = NULL;
	}
}

//=============================================================================
// �^�C�g���̃^�C�g�����S�p�|���S���J������
//=============================================================================
void CTitle::ReleaseTitleLogoPolygon(void)
{
	if (m_pTitleLogo != NULL)
	{
		m_pTitleLogo->Uninit();
		m_pTitleLogo = NULL;
	}
}

//=============================================================================
// �^�C�g���̃v���X�ҋ@�p�|���S���J������
//=============================================================================
void CTitle::ReleasePress(void)
{
	if (m_pPress != NULL)
	{
		m_pPress->Uninit();
		m_pPress = NULL;
	}
}

//=============================================================================
// �^�C�g���̃Q�[�����[�h�w�i�p�|���S���J������
//=============================================================================
void CTitle::ReleaseGameModeBg(void)
{
	if (m_pGameLogoBg != NULL)
	{
		m_pGameLogoBg->Uninit();
		m_pGameLogoBg = NULL;
	}

	if (m_pDecide != NULL)
	{
		m_pDecide->Uninit();
		m_pDecide = NULL;
	}

	if (m_pRetrun != NULL)
	{
		m_pRetrun->Uninit();
		m_pRetrun = NULL;
	}

	if (m_pIcon != NULL)
	{
		m_pIcon->Uninit();
		m_pIcon = NULL;
	}
}

//=============================================================================
// �^�C�g���̃��C�g�J������
//=============================================================================
void CTitle::ReleaseLight(void)
{
	if (m_pLightManager != NULL)
	{
		m_pLightManager->Uninit();
		delete m_pLightManager;
		m_pLightManager = NULL;
	}
}

//=============================================================================
// �^�C�g���̃Q�[�����[�h�I��p�|���S���J������
//=============================================================================
void CTitle::ReleaseGameModeLogo(void)
{
	for (int nCntLogo = 0; nCntLogo < GAMEMODE_MAX; nCntLogo++)
	{
		if (m_apGameModeLogo[nCntLogo] != NULL)
		{
			m_apGameModeLogo[nCntLogo]->Uninit();
			m_apGameModeLogo[nCntLogo] = NULL;
		}
	}
}

//=============================================================================
// �^�C�g���̋�J������
//=============================================================================
void CTitle::ReleaseSky(void)
{
	if (m_pSky != NULL)
	{
		m_pSky->Uninit();
		m_pSky = NULL;
	}
}

//=============================================================================
// �^�C�g���̃v���C���[���J�����鏈��
//=============================================================================
void CTitle::ReleasePlayer(void)
{
	if (m_pPlayer == NULL) { return; }

	for (int nCntType = 0; nCntType < m_nNumPlayer; nCntType++)
	{
		if (m_pPlayer[nCntType] != NULL)
		{
			m_pPlayer[nCntType]->Uninit();
			m_pPlayer[nCntType] = NULL;
		}
	}
	delete[] m_pPlayer;
	m_pPlayer = NULL;
}

//=============================================================================
// �^�C�g���̃v���C���[�Ǌ��N���X�p�̃|�C���^���J�����鏈��
//=============================================================================
void CTitle::ReleasePlayerManagerPointer(void)
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
// �^�C�g���̃v���C���[�Ǌ��N���X���J�����鏈��
//=============================================================================
void CTitle::ReleasePlayerManager(void)
{
	if (m_pPlayerManager == NULL) { return; }

	for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
	{
		if (m_pPlayerManager[nCntType] != NULL)
		{
			m_pPlayerManager[nCntType]->Uninit();
			delete m_pPlayerManager[nCntType];
			m_pPlayerManager[nCntType] = NULL;
		}
	}
	delete[] m_pPlayerManager;
	m_pPlayerManager = NULL;
}

//=============================================================================
// �^�C�g���̃v���C���[�f�[�^�p�|�C���^���J�����鏈��
//=============================================================================
void CTitle::ReleasePlayerDataPointer(void)
{
	if (m_pPlayerData == NULL) { return; }

	delete[] m_pPlayerData;
	m_pPlayerData = NULL;
}


//=============================================================================
// �^�C�g���̃��C�g�f�[�^�p�|�C���^���J�����鏈��
//=============================================================================
void CTitle::ReleaseLightDataPointer(void)
{
	if (m_pLightData == NULL) { return; }

	delete[] m_pLightData;
	m_pLightData = NULL;
}

//=============================================================================
// �^�C�g���̒n�ʊJ������
//=============================================================================
void CTitle::ReleaseField(void)
{
	if (m_pMeshField != NULL)
	{
		m_pMeshField->Uninit();
		m_pMeshField = NULL;
	}
}

//=============================================================================
// �^�C�g���̃n�C�X�R�A�J������
//=============================================================================
void CTitle::ReleaseHighScore(void)
{
	// ���S�|���S���J��
	if (m_pHighScoreLogo != NULL)
	{
		m_pHighScoreLogo->Uninit();
		m_pHighScoreLogo = NULL;
	}

	// �����|���S���J��
	if (m_pHighScore != NULL)
	{
		m_pHighScore->Uninit();
		m_pHighScore = NULL;
	}
}

//=============================================================================
// �^�C�g���̃`�[�����S��Ԃ̍X�V����
//=============================================================================
void CTitle::TeamLogoUpdate(void)
{
	CDebugProc::Print(1, "�`�[�����S���\n");
	if (CManager::GetFade()->GetFade() != CFade::FADE_NONE) return;

	m_nStateCounter++;
	CDebugProc::Print(1, "%d\n", m_nStateCounter);
	if (m_nStateCounter >= TITLE_FADEOUT_TIMING)
	{// ���l�𒴂���
		m_nStateCounter = 0;
		CreateFadePolygon();
		m_State = STATE_FADEOUT;
	}
	else
	{
		WaitInputToNormal();
	}
	if (m_nStateCounter == TITLE_TEAMLOGO_SOUND_TIMING)
	{
		CManager::GetSound()->PlaySound(TITLE_SE_TEAMLOGO_IDX);
	}
}

//=============================================================================
// �^�C�g���̃t�F�[�h�A�E�g��Ԃ̍X�V����
//=============================================================================
void CTitle::FadeOutUpdate(void)
{
	CDebugProc::Print(1, "�t�F�[�h�A�E�g���\n");
	D3DXCOLOR FadeCol = m_pFade->GetCol();
	FadeCol.a += TITLE_FADE_SPEED;
	CDebugProc::Print(1, "%.1f\n", FadeCol.a);
	if (FadeCol.a >= 1.0f)
	{
		ReleaseTeamBgPolygon();
		ReleaseTeamLogoPolygon();
		FadeCol.a = 1.0f;
		m_State = STATE_FADEIN;
		CManager::GetSound()->PlaySound(TITLE_BGM_IDX);
	}

	if (m_pFade != NULL)
	{
		m_pFade->SetCol(FadeCol);
		m_pFade->SetVtxBuffCol();
	}
}

//=============================================================================
// �^�C�g���̃t�F�[�h�C����Ԃ̍X�V����
//=============================================================================
void CTitle::FadeInUpdate(void)
{
	CDebugProc::Print(1, "�t�F�[�h�C�����\n");
	D3DXCOLOR FadeCol = m_pFade->GetCol();
	FadeCol.a -= TITLE_FADE_SPEED;
	CDebugProc::Print(1, "%.1f\n", FadeCol.a);
	if (FadeCol.a <= 0.0f)
	{
		ReleaseFadePolygon();
		m_State = STATE_NORMAL;
	}

	if (m_pFade != NULL)
	{
		m_pFade->SetCol(FadeCol);
		m_pFade->SetVtxBuffCol();
	}
}

//=============================================================================
// �^�C�g���̒ʏ��Ԃ̍X�V����
//=============================================================================
void CTitle::NormalUpdate(void)
{
	CDebugProc::Print(1, "�ʏ���\n");

	// �J���������ɉ�����
	bool bAdvance = false;
	CCameraManager *pCameraManager = GetCameraManager();
	if (pCameraManager == NULL) { return; }
	CCamera *pCamera = pCameraManager->GetCamera();
	if (pCamera != NULL)
	{
		D3DXVECTOR3 CameraPosV = pCamera->GetPosV();
		D3DXVECTOR3 CameraPosR = pCamera->GetPosR();
		CameraPosV.y -= TITLECAMERA_POS_DOWN;
		CameraPosR.y -= TITLECAMERA_POS_DOWN;
		if (CameraPosV.y <= TITLECAMERA_POSV_MIN_Y)
		{
			CameraPosV.y = TITLECAMERA_POSV_MIN_Y;
			CameraPosR.y = TITLECAMERA_POSV_MIN_Y - TITLECAMERA_POSR_UNDER_Y;
			bAdvance = true;

			// �v���X�|���S��������ĂȂ���ΐ�������
			if (m_pPress == NULL)
			{
				CreatePress();
			}
		}

		// ����{�^���������ꂽ��J�����̈ʒu�����ɂ���
		CInputKeyboard *pKeyboard = CManager::GetKeyboard();
		CXInput *pXInput = CManager::GetXInput();
		if (pKeyboard == NULL || pXInput == NULL) { return; }
		if (pKeyboard->GetTrigger(DIK_RETURN) == true)
		{// ����{�^���������ꂽ
			CameraPosV.y = TITLECAMERA_POSV_MIN_Y;
			CameraPosR.y = TITLECAMERA_POSV_MIN_Y - TITLECAMERA_POSR_UNDER_Y;
		}
		else
		{// �L�[�{�[�h��������Ă��Ȃ�
			for (int nCntButton = CXInput::XIJS_BUTTON_8; nCntButton < CXInput::XIJS_BUTTON_16; nCntButton++)
			{// ���肷��{�^���̐������J��Ԃ�
				if (CManager::GetXInput()->GetTrigger(0, (CXInput::XIJS_BUTTON)nCntButton) == true)
				{// �����{�^���������ꂽ
					CameraPosV.y = TITLECAMERA_POSV_MIN_Y;
					CameraPosR.y = TITLECAMERA_POSV_MIN_Y - TITLECAMERA_POSR_UNDER_Y;
				}
			}
		}

		pCamera->SetPosV(CameraPosV);
		pCamera->SetPosR(CameraPosR);
		pCameraManager->SetCamera(pCamera);
	}

	// �J���������ɉ�����؂��Ă��Ȃ������炱��ȏ㏈�����Ȃ�
	if (bAdvance == false) { return; }

	// �t�F�[�h���g�p����Ă����炱��ȏ㏈�����Ȃ�
	if (CManager::GetFade()->GetFade() != CFade::FADE_NONE) return;

	m_nStateCounter++;
	CDebugProc::Print(1, "%d\n", m_nStateCounter);
	if (m_nStateCounter >= TITLE_CHANGEMODE_TIMING)
	{// ���l�𒴂���
		SetNextMode_None();
	}
	else
	{// �܂������Ă��Ȃ�
		WaitInputToGameMode();
	}
}

//=============================================================================
// �^�C�g���̃Q�[�����[�h�I����Ԃ̍X�V����
//=============================================================================
void CTitle::GameModeUpdate(void)
{
	// �t�F�[�h���g�p����Ă����炱��ȏ㏈�����Ȃ�
	if (CManager::GetFade()->GetFade() != CFade::FADE_NONE) return;

	// �Q�[�����[�h��I��������
	CInputKeyboard *pKey = CManager::GetKeyboard();
	CXInput *pXInput = CManager::GetXInput();
	if (pKey == NULL || pXInput == NULL)return;

	if (pKey->GetTrigger(DIK_W) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_0) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_16) == true ||
		pKey->GetRepeat(DIK_W) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_0) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_16) == true)
	{// ������̓��͂����ꂽ
		ChangeNoneGameModeLogoPoly(m_nGameMode);
		m_nGameMode = (m_nGameMode + (GAMEMODE_MAX - 1)) % GAMEMODE_MAX;
		ChangeSelectGameModeLogoPoly(m_nGameMode);
		CManager::GetSound()->PlaySound(TITLE_SE_SELECT_IDX);
		if (m_pIcon != NULL)
		{
			m_pIcon->BindTexture(GetTextureManager()->GetTexture(TITLE_ICON_TEXIDX + m_nGameMode));
		}
	}
	else if (pKey->GetTrigger(DIK_S) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_1) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_17) == true ||
		pKey->GetRepeat(DIK_S) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_1) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_17) == true)
	{// �������̓��͂����ꂽ
		ChangeNoneGameModeLogoPoly(m_nGameMode);
		m_nGameMode = (m_nGameMode + 1) % GAMEMODE_MAX;
		ChangeSelectGameModeLogoPoly(m_nGameMode);
		CManager::GetSound()->PlaySound(TITLE_SE_SELECT_IDX);
		if (m_pIcon != NULL)
		{
			m_pIcon->BindTexture(GetTextureManager()->GetTexture(TITLE_ICON_TEXIDX + m_nGameMode));
		}
	}
	else if (pKey->GetTrigger(DIK_RETURN) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// ����{�^���������ꂽ
		SetNextMode_Push();
		CManager::GetSound()->PlaySound(TITLE_SE_DECIDE_IDX);
	}
	else if (pKey->GetTrigger(DIK_BACKSPACE) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_10) == true)
	{// �߂�{�^���������ꂽ
		m_nGameMode = CTitle::GAMEMODE_LOCAL1P;
		SetState(STATE_NORMAL);
		ReleaseGameModeBg();
		ReleaseGameModeLogo();
		ReleaseHighScore();
		CreatePress();
		CScene::DeathCheck();
	}
}

//=============================================================================
// �^�C�g���̒ʏ��Ԃɍs�����߂̓��͑ҋ@����
//=============================================================================
void CTitle::WaitInputToNormal(void)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	CFade *pFade = CManager::GetFade();
	if (pKey == NULL || pFade == NULL) return;

	if (pFade->GetFade() != CFade::FADE_NONE) { return; }

	if (pKey->GetTrigger(DIK_RETURN) == true)
	{// ����{�^���������ꂽ
		m_nStateCounter = 0;
		CreateFadePolygon();
		m_State = STATE_FADEOUT;
		CManager::GetSound()->StopSound(TITLE_SE_TEAMLOGO_IDX);
	}
	else
	{// �L�[�{�[�h��������Ă��Ȃ�
		for (int nCntButton = CXInput::XIJS_BUTTON_8; nCntButton < CXInput::XIJS_BUTTON_16; nCntButton++)
		{// ���肷��{�^���̐������J��Ԃ�
			if (CManager::GetXInput()->GetTrigger(0, (CXInput::XIJS_BUTTON)nCntButton) == true)
			{// �����{�^���������ꂽ
				m_nStateCounter = 0;
				CreateFadePolygon();
				m_State = STATE_FADEOUT;
				CManager::GetSound()->StopSound(TITLE_SE_TEAMLOGO_IDX);
			}
		}
	}
}

//=============================================================================
// �^�C�g���̎��̃��[�h�ɍs�����߂̓��͑ҋ@����
//=============================================================================
void CTitle::WaitInputToGameMode(void)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	CFade *pFade = CManager::GetFade();
	if (pKey == NULL || pFade == NULL) return;

	if (pFade->GetFade() != CFade::FADE_NONE) { return; }

	if (pKey->GetTrigger(DIK_RETURN) == true)
	{// ����{�^���������ꂽ
		m_nStateCounter = 0;
		SetState(STATE_GAMEMODE);
		ReleasePress();
		CreateGameModeBg();
		CreateGameModeLogo();
		CreateHighScore();
		CManager::GetSound()->PlaySound(TITLE_SE_DECIDE_IDX);
	}
	else
	{// �L�[�{�[�h��������Ă��Ȃ�
		for (int nCntButton = CXInput::XIJS_BUTTON_8; nCntButton < CXInput::XIJS_BUTTON_16; nCntButton++)
		{// ���肷��{�^���̐������J��Ԃ�
			if (CManager::GetXInput()->GetTrigger(0, (CXInput::XIJS_BUTTON)nCntButton) == true)
			{// �����{�^���������ꂽ
				m_nStateCounter = 0;
				SetState(STATE_GAMEMODE);
				ReleasePress();
				CreateGameModeBg();
				CreateGameModeLogo();
				CreateHighScore();
				CManager::GetSound()->PlaySound(TITLE_SE_DECIDE_IDX);
			}
		}
	}
}

//=============================================================================
// �^�C�g���̎��̃��[�h�ɐݒ肷�鏈��(���͂���Ȃ������ꍇ)
//=============================================================================
void CTitle::SetNextMode_None(void)
{
	CManager::GetFade()->SetFade(CManager::MODE_TITLE);
}

//=============================================================================
// �^�C�g���̎��̃��[�h�ɐݒ肷�鏈��(���͂��ꂽ�ꍇ)
//=============================================================================
void CTitle::SetNextMode_Push(void)
{
	CManager::GetFade()->SetFade(CManager::MODE_CHARASELECT);
}

//=============================================================================
// �^�C�g���̃Q�[�����[�h�p�|���S����I������Ă��Ȃ���Ԃɂ���
//=============================================================================
void CTitle::ChangeNoneGameModeLogoPoly(int nSelect)
{
	D3DXCOLOR GameModeLogoColLogo = m_GameModeLogoPoly_NoneData.col;
	float fGameModeLogoWidthLogo = m_GameModeLogoPoly_NoneData.fWidth;
	float fGameModeLogoHeightLogo = m_GameModeLogoPoly_NoneData.fHeight;
	if (m_apGameModeLogo[nSelect] != NULL)
	{
		m_apGameModeLogo[nSelect]->SetState(CScene2DFlash::STATE_NONE);
		m_apGameModeLogo[nSelect]->SetScale(fGameModeLogoWidthLogo, fGameModeLogoHeightLogo);
		m_apGameModeLogo[nSelect]->SetCol(GameModeLogoColLogo);
		m_apGameModeLogo[nSelect]->SetVtxBuffValue();
	}
}

//=============================================================================
// �^�C�g���̃Q�[�����[�h�p�|���S����I�����ꂽ��Ԃɂ���
//=============================================================================
void CTitle::ChangeSelectGameModeLogoPoly(int nSelect)
{
	D3DXCOLOR GameModeLogoColLogo = m_GameModeLogoPoly_SelectData.col;
	float fGameModeLogoWidthLogo = m_GameModeLogoPoly_SelectData.fWidth;
	float fGameModeLogoHeightLogo = m_GameModeLogoPoly_SelectData.fHeight;

	if (m_apGameModeLogo[nSelect] != NULL)
	{
		m_apGameModeLogo[nSelect]->SetState(CScene2DFlash::STATE_SELECT);
		m_apGameModeLogo[nSelect]->SetScale(fGameModeLogoWidthLogo, fGameModeLogoHeightLogo);
		m_apGameModeLogo[nSelect]->SetCol(GameModeLogoColLogo);
		m_apGameModeLogo[nSelect]->SetVtxBuffValue();
	}
}

//=============================================================================
// �^�C�g���̒n�ʂ�UV�l��i�߂鏈��
//=============================================================================
void CTitle::AddFieldTexUV(void)
{
	if (m_pMeshField == NULL) { return; }

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();
	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		int nXBlock = m_pMeshField->GetXBlock();
		int nZBlock = m_pMeshField->GetZBlock();

		for (int nCntV = 0; nCntV < nZBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
				pVtx[nCntH].tex.y += m_FieldData.fAddTex;
			}
			// �|�C���^��i�߂�
			pVtx += nXBlock + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//=============================================================================
// �^�C�g���̒n�ʂ𑖂�G�t�F�N�g�𐶐����鏈��
//=============================================================================
void CTitle::CreateDashEffect(void)
{
	// �G�t�F�N�g�Ǌ��N���X���擾
	CEffectManager *pEffectManager = GetEffectManager();
	if (pEffectManager == NULL) { return; }

	// �G�t�F�N�g���o��
	for (int nCntPlayer = 0; nCntPlayer < m_nNumPlayer; nCntPlayer++)
	{
		D3DXVECTOR3 EffectPos = m_pPlayerData[nCntPlayer].pos;
		EffectPos.x += 20.0f + (m_pPlayerData[nCntPlayer].pos.x * 0.05f);
		EffectPos.z += 40.0f;
		pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, TITLE_DASHEFFECT_IDX);
		EffectPos.x -= 40.0f - (m_pPlayerData[nCntPlayer].pos.x * 0.05f);
		pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, TITLE_DASHEFFECT_IDX);
	}
}

//=============================================================================
// �^�C�g���̃V�X�e������ǂݍ���
//=============================================================================
void CTitle::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(TITLE_SYSTEM_FILENAME);
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
// �^�C�g���̃V�X�e�������t�@�C������ǂݍ���
//=============================================================================
void CTitle::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
{
	int nCntTex = 0;
	int nCntModel = 0;
	int nCntPlayerData = 0;
	int nCntPlayer = 0;
	int nCntLight = 0;
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
		else if (CFunctionLib::Memcmp(pStr, TITLELOGOSET) == 0)
		{// �^�C�g�����S��񂾂���
			LoadTitleLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, PRESSSET) == 0)
		{// �v���X�ҋ@��񂾂���
			LoadPress(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, TEAMLOGOSET) == 0)
		{// �`�[�����S��񂾂���
			LoadTeamLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, GAMEMODEBGSET) == 0)
		{// �Q�[�����[�h�p�w�i��񂾂���
			LoadGameModebg(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, GAMEMODELOGOSET) == 0)
		{// �Q�[�����[�h�I��p�|���S����񂾂���
			LoadGameModeLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_PLAYERDATA) == 0)
		{// �ǂݍ��ރv���C���[�f�[�^�̑���������
			m_nNumPlayerData = CFunctionLib::ReadInt(pStr, NUM_PLAYERDATA);
			CreatePlayerManagerPointer();
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYERDATA_FILENAME) == 0)
		{// �ǂݍ��ރv���C���[�f�[�^�̃t�@�C����������
			LoadPlayerDataFileName(pStr, nCntPlayerData);
			nCntPlayerData++;
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_PLAYER) == 0)
		{// �ǂݍ��ރv���C���[���̑���������
			m_nNumPlayer = CFunctionLib::ReadInt(pStr, NUM_PLAYER);
			CreatePlayerDataPointer();
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYERSET) == 0)
		{// �ǂݍ��ރv���C���[��񂾂���
			LoadPlayerData(pFileLoader, pStr, nCntPlayer);
			nCntPlayer++;
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_LIGHT) == 0)
		{// �ǂݍ��ރv���C���[���̑���������
			m_nNumLight = CFunctionLib::ReadInt(pStr, NUM_LIGHT);
			CreateLightDataPointer();
		}
		else if (CFunctionLib::Memcmp(pStr, LIGHTSET) == 0)
		{// �ǂݍ��ރ��C�g��񂾂���
			LoadLight(pFileLoader, pStr, nCntLight);
			nCntLight++;
		}
		else if (CFunctionLib::Memcmp(pStr, FIELDSET) == 0)
		{// �ǂݍ��ޒn�ʏ�񂾂���
			LoadField(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, SKYSET) == 0)
		{// �ǂݍ��ދ��񂾂���
			LoadSky(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�t�@�C���I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �^�C�g���̓ǂݍ��ރe�N�X�`���t�@�C������ǂݍ���
//=============================================================================
void CTitle::LoadTexFileName(char *pStr, int nCntTex)
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
// �^�C�g���̃��f����ǂݍ���
//=============================================================================
void CTitle::LoadModel(char *pStr, int nCntModel)
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
// �^�C�g���̓ǂݍ��ރG�t�F�N�g�t�@�C������ǂݍ���
//=============================================================================
void CTitle::LoadEffectFileName(char *pStr)
{
	char aEffectFileName[256] = "\0";
	strcpy(aEffectFileName, CFunctionLib::ReadString(pStr, aEffectFileName, EFFECT_FILENAME));

	// �G�t�F�N�g�Ǌ��N���X�𐶐�
	CEffectManager *pEffectManager = CEffectManager::Create(aEffectFileName);
	SetEffectManager(pEffectManager);
}

//=============================================================================
// �^�C�g���̃^�C�g�����S�|���S���̒l��ǂݍ���
//=============================================================================
void CTitle::LoadTitleLogo(CFileLoader *pFileLoader, char *pStr)
{
	int nLogoTexIdx = 0;
	D3DXVECTOR3 LogoPos = TITLELOGO_POS_INI;
	D3DXCOLOR LogoCol = TITLELOGO_COL_INI;
	float fLogoWidth = TITLELOGO_WIDTH_INI;
	float fLogoHeight = TITLELOGO_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			nLogoTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			LogoPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			LogoCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			fLogoWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			fLogoHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_TITLELOGOSET) == 0)
		{// �^�C�g�����S���I���̍��}������
			break;
		}
	}

	// �|���S������
	m_pTitleLogo = CTitleLogo::Create(LogoPos, LogoCol, fLogoWidth, fLogoHeight, TITLELOGO_PRIORITY);
	if (m_pTitleLogo != NULL && GetTextureManager() != NULL)
	{
		m_pTitleLogo->BindTexture(GetTextureManager()->GetTexture(nLogoTexIdx));
	}
}

//=============================================================================
// �^�C�g���̃v���X�ҋ@�|���S���̒l��ǂݍ���
//=============================================================================
void CTitle::LoadPress(CFileLoader *pFileLoader, char *pStr)
{
	m_PressData.nTexIdx = 0;
	m_PressData.pos = TITLEPRESS_POS_INI;
	m_PressData.col = TITLEPRESS_COL_INI;
	m_PressData.fWidth = TITLEPRESS_WIDTH_INI;
	m_PressData.fHeight = TITLEPRESS_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			m_PressData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			m_PressData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			m_PressData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			m_PressData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			m_PressData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_PRESSSET) == 0)
		{// �v���X�ҋ@���I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �^�C�g���̃`�[�����S�p�|���S���̒l��ǂݍ���
//=============================================================================
void CTitle::LoadTeamLogo(CFileLoader *pFileLoader, char *pStr)
{
	int nTeamLogoTexIdx = 0;
	D3DXVECTOR3 TeamLogoPos = TITLETEAMLOGO_POS_INI;
	D3DXCOLOR TeamLogoCol = TITLETEAMLOGO_COL_INI;
	float fTeamLogoWidth = TITLETEAMLOGO_WIDTH_INI;
	float fTeamLogoHeight = TITLETEAMLOGO_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			nTeamLogoTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			TeamLogoPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			TeamLogoCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			fTeamLogoWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			fTeamLogoHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_TEAMLOGOSET) == 0)
		{// �`�[�����S���I���̍��}������
			break;
		}
	}

	// �`�[�����S�p�w�i�|���S���̐���
	CreateTeamBgPolygon();

	// �|���S������
	m_pTeamLogo = CScene2D::Create(TeamLogoPos, TeamLogoCol, fTeamLogoWidth, fTeamLogoHeight, TITLETEAMLOGO_PRIORITY);
	if (m_pTeamLogo != NULL && GetTextureManager() != NULL)
	{
		m_pTeamLogo->BindTexture(GetTextureManager()->GetTexture(nTeamLogoTexIdx));
	}
}

//=============================================================================
// �^�C�g���̃Q�[�����[�h�w�i�p�|���S���̒l��ǂݍ���
//=============================================================================
void CTitle::LoadGameModebg(CFileLoader *pFileLoader, char *pStr)
{
	m_GameModeBgData.nTexIdx = 0;
	m_GameModeBgData.pos = TITLEGAMEMODEBG_POS_INI;
	m_GameModeBgData.col = TITLEGAMEMODEBG_COL_INI;
	m_GameModeBgData.fWidth = TITLEGAMEMODEBG_WIDTH_INI;
	m_GameModeBgData.fHeight = TITLEGAMEMODEBG_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			m_GameModeBgData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			m_GameModeBgData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			m_GameModeBgData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			m_GameModeBgData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			m_GameModeBgData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_GAMEMODEBGSET) == 0)
		{// �Q�[�����[�h�w�i���I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �^�C�g���̃Q�[�����[�h�I��p�|���S���̒l��ǂݍ���
//=============================================================================
void CTitle::LoadGameModeLogo(CFileLoader *pFileLoader, char *pStr)
{
	m_GameModeLogoData.nTexIdx = 0;
	m_GameModeLogoData.pos = TITLEGAMEMODELOGO_POS_INI;
	m_GameModeLogoData.fInterval = TITLEGAMEMODELOGO_INTERVAL_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			m_GameModeLogoData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			m_GameModeLogoData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, INTERVAL) == 0)
		{// �|���S���ƃ|���S���̊Ԋu������
			m_GameModeLogoData.fInterval = CFunctionLib::ReadFloat(pStr, INTERVAL);
		}
		else if (CFunctionLib::Memcmp(pStr, NONE) == 0)
		{// �I������Ă��Ȃ��|���S���̃f�[�^������
			LoadGameModeLogo_PolyNone(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, SELECT) == 0)
		{// �I������Ă���|���S���̃f�[�^������
			LoadGameModeLogo_PolySelect(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_GAMEMODELOGOSET) == 0)
		{// �Q�[�����[�h�I��p�|���S�����I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �^�C�g���̃Q�[�����[�h�I��p�|���S���̒l��ǂݍ���(�I������ĂȂ���)
//=============================================================================
void CTitle::LoadGameModeLogo_PolyNone(CFileLoader *pFileLoader, char *pStr)
{
	m_GameModeLogoPoly_NoneData.col = TITLEGAMEMODELOGO_COL_INI;
	m_GameModeLogoPoly_NoneData.fWidth = TITLEGAMEMODELOGO_WIDTH_INI;
	m_GameModeLogoPoly_NoneData.fHeight = TITLEGAMEMODELOGO_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			m_GameModeLogoPoly_NoneData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			m_GameModeLogoPoly_NoneData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			m_GameModeLogoPoly_NoneData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_NONE) == 0)
		{// �I������Ă��Ȃ��Q�[�����[�h�I��p�|���S�����I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �^�C�g���̃Q�[�����[�h�I��p�|���S���̒l��ǂݍ���(�I������Ă��鎞)
//=============================================================================
void CTitle::LoadGameModeLogo_PolySelect(CFileLoader *pFileLoader, char *pStr)
{
	m_GameModeLogoPoly_SelectData.col = TITLEGAMEMODELOGO_COL_INI;
	m_GameModeLogoPoly_SelectData.fWidth = TITLEGAMEMODELOGO_WIDTH_INI;
	m_GameModeLogoPoly_SelectData.fHeight = TITLEGAMEMODELOGO_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			m_GameModeLogoPoly_SelectData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			m_GameModeLogoPoly_SelectData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			m_GameModeLogoPoly_SelectData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SELECT) == 0)
		{// �I������Ă���Q�[�����[�h�I��p�|���S�����I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �^�C�g���̓ǂݍ��ރv���C���[�f�[�^�̃t�@�C������ǂݍ���
//=============================================================================
void CTitle::LoadPlayerDataFileName(char *pStr, int nCntPlayerData)
{
	strcpy(m_aPlayerDataFileName[nCntPlayerData], CFunctionLib::ReadString(pStr, m_aPlayerDataFileName[nCntPlayerData], PLAYERDATA_FILENAME));
}

//=============================================================================
// �^�C�g���̓ǂݍ��ރv���C���[�f�[�^�̃t�@�C������ǂݍ���
//=============================================================================
void CTitle::LoadPlayerData(CFileLoader *pFileLoader, char *pStr, int nCntPlayer)
{
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TYPE) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			m_pPlayerData[nCntPlayer].nType = CFunctionLib::ReadInt(pStr, TYPE);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �ʒu������
			m_pPlayerData[nCntPlayer].pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{// ����������
			m_pPlayerData[nCntPlayer].rot = CFunctionLib::ReadVector3(pStr, ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_PLAYERSET) == 0)
		{// �v���C���[���I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �^�C�g���̃��C�g����ǂݍ���
//=============================================================================
void CTitle::LoadLight(CFileLoader *pFileLoader, char *pStr, int nCntLight)
{
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, DIR) == 0)
		{// ����������
			m_pLightData[nCntLight].Dir = CFunctionLib::ReadVector3(pStr, DIR);
		}
		else if (CFunctionLib::Memcmp(pStr, DIFFUSE) == 0)
		{// �g�U��������
			m_pLightData[nCntLight].Diffuse = CFunctionLib::ReadVector4(pStr, DIFFUSE);
		}
		else if (CFunctionLib::Memcmp(pStr, AMBIENT) == 0)
		{// ����������
			m_pLightData[nCntLight].Ambient = CFunctionLib::ReadVector4(pStr, AMBIENT);
		}
		else if (CFunctionLib::Memcmp(pStr, SPECULAR) == 0)
		{// ���ˌ�������
			m_pLightData[nCntLight].Specular = CFunctionLib::ReadVector4(pStr, SPECULAR);
		}
		else if (CFunctionLib::Memcmp(pStr, END_LIGHTSET) == 0)
		{// ���C�g���I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �^�C�g���̒n�ʏ���ǂݍ���
//=============================================================================
void CTitle::LoadField(CFileLoader *pFileLoader, char *pStr)
{
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			m_FieldData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ���W������
			m_FieldData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{// ����������
			m_FieldData.rot = CFunctionLib::ReadVector3(pStr, ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �F������
			m_FieldData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// 1�}�X���̕�������
			m_FieldData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, DEPTH) == 0)
		{// 1�}�X���̉��s������
			m_FieldData.fDepth = CFunctionLib::ReadFloat(pStr, DEPTH);
		}
		else if (CFunctionLib::Memcmp(pStr, XBLOCK) == 0)
		{// ���̕�����������
			m_FieldData.nXBlock = CFunctionLib::ReadInt(pStr, XBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, ZBLOCK) == 0)
		{// ���s�̕�����������
			m_FieldData.nZBlock = CFunctionLib::ReadInt(pStr, ZBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, TEXU_SPLIT) == 0)
		{// �e�N�X�`�����W�̉��̕�����������
			m_FieldData.nTexUSplit = CFunctionLib::ReadInt(pStr, TEXU_SPLIT);
		}
		else if (CFunctionLib::Memcmp(pStr, TEXV_SPLIT) == 0)
		{// �e�N�X�`�����W�̏c�̕�����������
			m_FieldData.nTexVSplit = CFunctionLib::ReadInt(pStr, TEXV_SPLIT);
		}
		else if (CFunctionLib::Memcmp(pStr, CULLING) == 0)
		{// �J�����O���邩���Ȃ���������
			m_FieldData.bCulling = CFunctionLib::ReadBool(pStr, CULLING);
		}
		else if (CFunctionLib::Memcmp(pStr, ADDTEX) == 0)
		{// �ǂꂭ�炢�e�N�X�`�����W��i�߂邩������
			m_FieldData.fAddTex = CFunctionLib::ReadFloat(pStr, ADDTEX);
		}
		else if (CFunctionLib::Memcmp(pStr, END_FIELDSET) == 0)
		{// �n�ʏ��I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �^�C�g���̋����ǂݍ���
//=============================================================================
void CTitle::LoadSky(CFileLoader *pFileLoader, char *pStr)
{
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			m_SkyData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ���W������
			m_SkyData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{// ����������
			m_SkyData.rot = CFunctionLib::ReadVector3(pStr, ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �F������
			m_SkyData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, RADIUS) == 0)
		{// ���a������
			m_SkyData.fRadius = CFunctionLib::ReadFloat(pStr, RADIUS);
		}
		else if (CFunctionLib::Memcmp(pStr, XBLOCK) == 0)
		{// ���̕�����������
			m_SkyData.nXBlock = CFunctionLib::ReadInt(pStr, XBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, ZBLOCK) == 0)
		{// ���s�̕�����������
			m_SkyData.nZBlock = CFunctionLib::ReadInt(pStr, ZBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, ROLLSPEED) == 0)
		{// ��]������X�s�[�h������
			m_SkyData.fRollSpeed = CFunctionLib::ReadFloat(pStr, ROLLSPEED);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SKYSET) == 0)
		{// ����I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �^�C�g���̕ϐ�������������
//=============================================================================
void CTitle::ClearVariable(void)
{
	m_State = STATE_TEAMLOGO;
	m_pFade = NULL;
	m_pTeamLogo = NULL;
	m_pTeamBg = NULL;
	m_pTitleLogo = NULL;
	m_nStateCounter = 0;
	ClearPress();
	ClearGameModeBg();
	ClearGameModeLogo();
	ClearGameModeLogo_PolyNone();
	ClearGameModeLogo_PolySelect();
	ClearPlayer();
	ClearHighScore();
}

//=============================================================================
// �^�C�g���̃v���X�ҋ@�p�ϐ�������������
//=============================================================================
void CTitle::ClearPress(void)
{
	m_PressData.nTexIdx = 0;
	m_PressData.pos = INITIALIZE_D3DXVECTOR3;
	m_PressData.col = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	m_PressData.fWidth = 0.0f;
	m_PressData.fHeight = 0.0f;
	m_pPress = NULL;
}

//=============================================================================
// �^�C�g���̃Q�[�����[�h�w�i�p�ϐ�������������
//=============================================================================
void CTitle::ClearGameModeBg(void)
{
	m_GameModeBgData.nTexIdx = 0;
	m_GameModeBgData.pos = INITIALIZE_D3DXVECTOR3;
	m_GameModeBgData.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_GameModeBgData.fWidth = 0.0f;
	m_GameModeBgData.fHeight = 0.0f;
	m_pGameLogoBg = NULL;
	m_pDecide = NULL;
	m_pRetrun = NULL;
	m_pIcon = NULL;
}

//=============================================================================
// �^�C�g���̃Q�[�����[�h���S�p�ϐ�������������
//=============================================================================
void CTitle::ClearGameModeLogo(void)
{
	m_GameModeLogoData.nTexIdx = 0;
	m_GameModeLogoData.pos = INITIALIZE_D3DXVECTOR3;
	m_GameModeLogoData.fInterval = 0.0f;
	for (int nCntLogo = 0; nCntLogo < GAMEMODE_MAX; nCntLogo++)
	{
		m_apGameModeLogo[nCntLogo] = NULL;
	}
}

//=============================================================================
// �^�C�g���̑I������Ă��Ȃ��Q�[�����[�h���S�|���S���p�ϐ�������������
//=============================================================================
void CTitle::ClearGameModeLogo_PolyNone(void)
{
	m_GameModeLogoPoly_NoneData.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_GameModeLogoPoly_NoneData.fWidth = 0.0f;
	m_GameModeLogoPoly_NoneData.fHeight = 0.0f;
}

//=============================================================================
// �^�C�g���̑I������Ă���Q�[�����[�h���S�|���S���p�ϐ�������������
//=============================================================================
void CTitle::ClearGameModeLogo_PolySelect(void)
{
	m_GameModeLogoPoly_SelectData.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_GameModeLogoPoly_SelectData.fWidth = 0.0f;
	m_GameModeLogoPoly_SelectData.fHeight = 0.0f;
}

//=============================================================================
// �^�C�g���̋�p�ϐ�������������
//=============================================================================
void CTitle::ClearSky(void)
{
	m_SkyData.nTexIdx = 0;
	m_SkyData.pos = INITIALIZE_D3DXVECTOR3;
	m_SkyData.rot = INITIALIZE_D3DXVECTOR3;
	m_SkyData.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_SkyData.nXBlock = 0;
	m_SkyData.nZBlock = 0;
	m_SkyData.fRadius = 0.0f;
	m_SkyData.fRollSpeed = 0.0f;
	m_pSky = NULL;
}

//=============================================================================
// �^�C�g���̒n�ʗp�ϐ�������������
//=============================================================================
void CTitle::ClearField(void)
{
	m_FieldData.nTexIdx = 0;
	m_FieldData.pos = INITIALIZE_D3DXVECTOR3;
	m_FieldData.rot = INITIALIZE_D3DXVECTOR3;
	m_FieldData.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_FieldData.fWidth = 0.0f;
	m_FieldData.fDepth = 0.0f;
	m_FieldData.nXBlock = 0;
	m_FieldData.nZBlock = 0;
	m_FieldData.nTexUSplit = 0;
	m_FieldData.nTexVSplit = 0;
	m_FieldData.bCulling = false;
	m_FieldData.fAddTex = 0.0f;
	m_pMeshField = NULL;
}

//=============================================================================
// �^�C�g���̃��C�g�p�ϐ�������������
//=============================================================================
void CTitle::ClearLight(void)
{
	m_nNumLight = 0;
	m_pLightData = NULL;
	m_pLightManager = NULL;
}

//=============================================================================
// �^�C�g���̃v���C���[�p�ϐ�������������
//=============================================================================
void CTitle::ClearPlayer(void)
{
	m_nNumPlayerData = 0;
	m_nNumPlayer = 0;
	m_pPlayerManager = NULL;
	m_pPlayerData = NULL;
	m_pPlayer = NULL;
	m_aPlayerDataFileName = NULL;
}

//=============================================================================
// �^�C�g���̃n�C�X�R�A�p�ϐ�������������
//=============================================================================
void CTitle::ClearHighScore(void)
{
	m_pHighScore = NULL;
	m_pHighScoreLogo = NULL;
}

//=============================================================================
// �^�C�g���̏�Ԃ�ݒ肷��
//=============================================================================
void CTitle::SetState(const STATE state)
{
	m_State = state;
}

//=============================================================================
// �^�C�g���̏�Ԃ��擾����
//=============================================================================
CTitle::STATE CTitle::GetState(void)
{
	return m_State;
}

//=============================================================================
// �^�C�g���̃Q�[�����[�h���擾����
//=============================================================================
int CTitle::GetGameMode(void)
{
	return m_nGameMode;
}

//=============================================================================
// �^�C�g���̃n�C�X�R�A��ݒ肷��
//=============================================================================
void CTitle::SetHighScore(const int nHighScore)
{
	m_nHighScore = nHighScore;
}

//=============================================================================
// �^�C�g���̃n�C�X�R�A���擾����
//=============================================================================
int CTitle::GetHighScore(void)
{
	return m_nHighScore;
}



//*****************************************************************************
// CTitleLogo�̏���
//*****************************************************************************
//=============================================================================
// �^�C�g�����S�̃R���X�g���N�^
//=============================================================================
CTitleLogo::CTitleLogo(int nPriority, OBJTYPE objtype) : CScene3D(nPriority, objtype)
{

}

//=============================================================================
// �^�C�g�����S�̃f�X�g���N�^
//=============================================================================
CTitleLogo::~CTitleLogo()
{

}

//=============================================================================
//�^�C�g�����S�̐���
//=============================================================================
CTitleLogo *CTitleLogo::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority, float fRot, float fStartTexU, float fStartTexV, float fAddTexU, float fAddTexV)
{
	CTitleLogo *pTitleLogo = NULL;	// CTitleLogo�N���X�^�̃|�C���^
	if (pTitleLogo == NULL)
	{
		pTitleLogo = new CTitleLogo(nPriority);
		if (pTitleLogo != NULL)
		{
			// �l�̐ݒ�
			pTitleLogo->SetPos(INITIALIZE_D3DXVECTOR3);
			pTitleLogo->SetPolypos(pos);
			pTitleLogo->SetCol(col);
			pTitleLogo->SetWidth(fWidth);
			pTitleLogo->SetHeight(fHeight);

			if (FAILED(pTitleLogo->Init()))
			{// �������Ɏ��s
				return NULL;
			}
		}
	}
	return pTitleLogo;
}

//=============================================================================
// �^�C�g�����S�̏���������
//=============================================================================
HRESULT CTitleLogo::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CScene3D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// �^�C�g�����S�̏I������
//=============================================================================
void CTitleLogo::Uninit(void)
{
	// ���ʂ̏I������
	CScene3D::Uninit();
}

//=============================================================================
// �^�C�g�����S�̍X�V����
//=============================================================================
void CTitleLogo::Update(void)
{

}

//=============================================================================
// �^�C�g�����S�̕`�揈��
//=============================================================================
void CTitleLogo::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			DWORD Lighting;
			D3DXMATRIX mtxProjOrg;
			D3DXMATRIX mtxProj;

			// ���݂̏���ۑ�����
			pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjOrg);

			// �v���W�F�N�V�����}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxProj);

			// �v���W�F�N�V�����}�g���b�N�X���쐬
			D3DXMatrixOrthoLH(&mtxProj,
				(float)SCREEN_WIDTH,
				(float)SCREEN_HEIGHT,
				1.0f,
				3000.0f);

			// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);

			// ���C�e�B���O�̐ݒ���O��
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// Z�e�X�g�����Ȃ��ݒ��
			pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

		    // ���ʂ̕`�揈��
			CScene3D::Draw();

			// Z�e�X�g�̐ݒ���f�t�H���g�ɖ߂�
			pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

			// ���C�e�B���O�̐ݒ��߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);

			// �`��̈�̐ݒ��߂�
			pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjOrg);
		}
	}
}

//=============================================================================
// �^�C�g�����S�̒��_�o�b�t�@��������
//=============================================================================
void CTitleLogo::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �I�t�Z�b�g�̒��������߂�
	float fWidth = GetWidth();
	float fHeight = GetHeight();
	float fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight));
	SetLength(fLength);

	// �I�t�Z�b�g�̊p�x�����߂�
	float fAngle = atan2f(fWidth, fHeight);
	SetAngle(fAngle);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-sinf(fAngle) * fLength, cosf(fAngle) * fLength, 2.0f) + m_Polypos;
	pVtx[1].pos = D3DXVECTOR3(sinf(fAngle) * fLength, cosf(fAngle) * fLength, 2.0f) + m_Polypos;
	pVtx[2].pos = D3DXVECTOR3(-sinf(fAngle) * fLength, -cosf(fAngle) * fLength, 2.0f) + m_Polypos;
	pVtx[3].pos = D3DXVECTOR3(sinf(fAngle) * fLength, -cosf(fAngle) * fLength, 2.0f) + m_Polypos;

	// ���_�J���[
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// ���_�o�b�t�@�̐ݒ�
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
// �^�C�g�����S�̃|���S�����W�ݒ藝
//=============================================================================
void CTitleLogo::SetPolypos(const D3DXVECTOR3 Polypos)
{
	m_Polypos = Polypos;
}

//=============================================================================
// �^�C�g�����S�̃|���S�����W�擾����
//=============================================================================
D3DXVECTOR3 CTitleLogo::GetPolypos(void)
{
	return m_Polypos;
}