//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "result.h"
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
#include "scene2D.h"
#include "number.h"
#include "title.h"
#include "sound.h"
#include "debugproc.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define RESULT_HIGHSCORE_FILENAME            "data/TEXT/highscore.txt"      // �n�C�X�R�A��ۑ�����t�@�C����
#define RESULT_SYSTEM_FILENAME               "data/TEXT/MODE/result.ini"    // �������Ɏg�p����V�X�e���t�@�C����
#define RESULT_WAITSTATE_TIME                (200)                           // �ҋ@��Ԃ���ʏ��Ԃɕς��܂ł̎���
#define RESULT_NEXTHIGHSCORE_TIME            (235)                          // �ʏ��Ԃ���n�C�X�R�A��\�������Ԃɕς��܂ł̎���
#define RESULT_CHANGEMODE_TIME               (400)                          // �ʏ��Ԃ���I����Ԃɕς��܂ł̎���
#define RESULT_HIGHSCORE_FALSH_TIME          (2)                            // �n�C�X�R�A���S��_�ł�����^�C�~���O
#define RESULT_BGM_IDX                       (4)                            // ���U���g�ōĐ�����BGM�̔ԍ�

// ���U���g���S�������p
#define RESULT_LOGO_POS_INI                  (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define RESULT_LOGO_COL_INI                  (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define RESULT_LOGO_WIDTH_INI                (150.0f)
#define RESULT_LOGO_HEIGHT_INI               (100.0f)

// ���U���g�n�C�X�R�A���S�������p
#define RESULT_HIGHSCORE_FALSH_COL           (D3DXCOLOR(0.5f,0.5f,1.0f,1.0f))
#define RESULT_HIGHSCORELOGO_POS_INI         (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 220.0f, 0.0f))
#define RESULT_HIGHSCORELOGO_COL_INI         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define RESULT_HIGHSCORELOGO_WIDTH_INI       (400.0f)
#define RESULT_HIGHSCORELOGO_HEIGHT_INI      (60.0f)
#define RESULT_HIGHSCORELOGO_TEXIDX          (2)

// ���U���g�n�C�X�R�A�����������p
#define RESULT_HIGHSCORENUMBER_POS_INI       (D3DXVECTOR3(900.0f, 380.0f, 0.0f))
#define RESULT_HIGHSCORENUMBER_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define RESULT_HIGHSCORENUMBER_WIDTH_INI     (40.0f)
#define RESULT_HIGHSCORENUMBER_HEIGHT_INI    (40.0f)
#define RESULT_HIGHSCORENUMBER_INTERVAL_INI  (D3DXVECTOR3(-90.0f, 0.0f, 0.0f))

// �X�R�A���S�̏������p
#define RESULT_SCORE_POS_INI       (D3DXVECTOR3(330.0f, 190.0f, 0.0f))
#define RESULT_SCORE_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define RESULT_SCORE_WIDTH_INI     (240.0f)
#define RESULT_SCORE_HEIGHT_INI    (160.0f)
#define RESULT_SCORE_INTERVAL_INI  (D3DXVECTOR3(-90.0f, 0.0f, 0.0f))

// �W���}�[���S�̏������p
#define RESULT_JAMMER_POS_INI       (D3DXVECTOR3(950.0f, 190.0f, 0.0f))
#define RESULT_JAMMER_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define RESULT_JAMMER_WIDTH_INI     (240.0f)
#define RESULT_JAMMER_HEIGHT_INI    (160.0f)
#define RESULT_JAMMER_INTERVAL_INI  (D3DXVECTOR3(-90.0f, 0.0f, 0.0f))

// �N���[�����S�̏������p
#define RESULT_CREAM_POS_INI       (D3DXVECTOR3(330.0f, 530.0f, 0.0f))
#define RESULT_CREAM_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define RESULT_CREAM_WIDTH_INI     (240.0f)
#define RESULT_CREAM_HEIGHT_INI    (160.0f)
#define RESULT_CREAM_INTERVAL_INI  (D3DXVECTOR3(-90.0f, 0.0f, 0.0f))

// �R���N�^�[���S�̏������p
#define RESULT_COLLECTOR_POS_INI       (D3DXVECTOR3(950.0f, 530.0f, 0.0f))
#define RESULT_COLLECTOR_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define RESULT_COLLECTOR_WIDTH_INI     (240.0f)
#define RESULT_COLLECTOR_HEIGHT_INI    (160.0f)
#define RESULT_COLLECTOR_INTERVAL_INI  (D3DXVECTOR3(-90.0f, 0.0f, 0.0f))

// �l�ǂݍ��ݗp�̃p�X
// �n�C�X�R�A�p
#define HIGHSCORE "HIGHSCORE = "

// �e�N�X�`���p
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// ���U���g���S�p
#define RESULTLOGOSET "RESULTLOGOSET"
#define END_RESULTLOGOSET "END_RESULTLOGOSET"
#define TEX_IDX "TEX_IDX = "
#define POS "POS = "
#define COL "COL = "
#define WIDTH "WIDTH = "
#define HEIGHT "HEIGHT = "

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
bool CResult::m_bHighScore = false;   // �n�C�X�R�A��\�����邩�ǂ���
int CResult::m_nScore[MAX_NUM_PLAYER] = {};		// �X�R�A�̏��
int CResult::m_nJammer[MAX_NUM_PLAYER] = {};	// �W���}�[�̏��
int CResult::m_nCream[MAX_NUM_PLAYER] = {};		// �N���[���̏��
int CResult::m_nCollector[MAX_NUM_PLAYER] = {};	// �R���N�^�[�̏��

//=============================================================================
// ���U���g�̃R���X�g���N�^
//=============================================================================
CResult::CResult()
{

}

//=============================================================================
// ���U���g�̃f�X�g���N�^
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// ���U���g�̐�������
//=============================================================================
CResult *CResult::Create()
{
	CResult *pResult = NULL;
	if (pResult == NULL)
	{
		pResult = new CResult;
		if (pResult != NULL)
		{
			pResult->Init();
		}
	}
	return pResult;
}

//=============================================================================
// ���U���g�̏���������
//=============================================================================
HRESULT CResult::Init(void)
{
	// �ϐ�������������
	ClearVariable();

	// �V�X�e���̏�����
	LoadSystem();

	// �J�����̐���
	CreateCamera();

	// BGM�̍Đ�
	CManager::GetSound()->PlaySound(RESULT_BGM_IDX);

	return S_OK;
}

//=============================================================================
// ���U���g�̏I������
//=============================================================================
void CResult::Uninit(void)
{
	// ���ʂ̏I������
	CBasemode::Uninit();

	// �e��N���X�̊J��
	ReleaseLogo();
	ReleaseHighScore();
	ReleaseMVP();

	// �S�ẴI�u�W�F�N�g�J��
	CScene::ReleaseAll();

	// BGM���~
	CManager::GetSound()->StopSound(RESULT_BGM_IDX);

	// �n�C�X�R�A��\�����Ȃ���Ԃ�
	m_bHighScore = false;
}

//=============================================================================
// ���U���g�̍X�V����
//=============================================================================
void CResult::Update(void)
{
	CDebugProc::Print(1, "���U���g���\n");

	// ��Ԃɂ���ď����킯
	switch (m_State)
	{
	case STATE_WAIT:
		WaitUpdate();
		break;
	case STATE_MVP:
		MVPUpdate();
		break;
	case STATE_NORMAL:
		NormalUpdate();
		break;
	case STATE_HIGHSCORE:
		HighScoreUpdate();
		break;
	case STATE_END:
		EndUpdate();
		break;
	}

	// �S�Ă̍X�V����
	CScene::UpdateAll();
}

//=============================================================================
// ���U���g�̕`�揈��
//=============================================================================
void CResult::Draw(void)
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
// ���U���g�̃e�N�X�`���Ǌ��N���X��������
//=============================================================================
void CResult::CreateTexture(int nNumTex)
{
	CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
	SetTextureManager(pTextureManager);
}

//=============================================================================
// ���U���g�̃J������������
//=============================================================================
void CResult::CreateCamera(void)
{
	CCameraManager *pCameraManager = CCameraManager::Create();
	if (pCameraManager == NULL) return;

	// �J�����𐶐�
	CCamera *pCamera = CCamera::Create();
	if (pCamera != NULL)
	{
		pCameraManager->SetCamera(pCamera);
	}
	SetCameraManager(pCameraManager);
}

//=============================================================================
// ���U���g�̃��S��������
//=============================================================================
void CResult::CreateLogo(void)
{
	m_pLogo = CScene2D::Create(m_ResultLogoData.pos, m_ResultLogoData.col, m_ResultLogoData.fWidth, m_ResultLogoData.fHeight);
	if (m_pLogo != NULL && GetTextureManager() != NULL)
	{
		m_pLogo->BindTexture(GetTextureManager()->GetTexture(m_ResultLogoData.nTexIdx));
	}
}

//=============================================================================
// ���U���g�̃��S��������
//=============================================================================
void CResult::CreateHighScore(void)
{
	// ���S�|���S������
	if (m_pHighScoreLogo == NULL)
	{
		m_pHighScoreLogo = CScene2D::Create(RESULT_HIGHSCORELOGO_POS_INI, RESULT_HIGHSCORELOGO_COL_INI,
			RESULT_HIGHSCORELOGO_WIDTH_INI, RESULT_HIGHSCORELOGO_HEIGHT_INI);
		if (m_pHighScoreLogo != NULL && GetTextureManager() != NULL)
		{
			m_pHighScoreLogo->BindTexture(GetTextureManager()->GetTexture(RESULT_HIGHSCORELOGO_TEXIDX));
		}
	}

	// �����|���S������
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(1);
	}
	if (m_pHighScore == NULL)
	{
		m_pHighScore = CNumber::Create(RESULT_HIGHSCORENUMBER_POS_INI, RESULT_HIGHSCORENUMBER_COL_INI,
			RESULT_HIGHSCORENUMBER_WIDTH_INI, RESULT_HIGHSCORENUMBER_HEIGHT_INI, RESULT_HIGHSCORENUMBER_INTERVAL_INI,
			pTexture, CTitle::GetHighScore());
	}
}

//=============================================================================
// MVP�ŕK�v�ȃ��S��������
//=============================================================================
void CResult::CreateMVP(void)
{
	// MVP�X�R�A�̐���
	CreateMVPScore();

	// MVP�W���}�[�̐���
	CreateMVPJammer();

	// MVP�N���[���̐���
	CreateMVPCream();

	// MVP�R���N�^�[�̐���
	CreateMVPCollector();
}


//=============================================================================
// MVP�X�R�A�̐�������
//=============================================================================
void CResult::CreateMVPScore(void)
{
	// �����|���S������
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(1);
	}

	// MVP�X�R�A���S�̐���
	if (m_pMVPScoreLogo == NULL)
	{
		m_pMVPScoreLogo = CScene2D::Create(RESULT_SCORE_POS_INI, RESULT_SCORE_COL_INI,
			RESULT_SCORE_WIDTH_INI, RESULT_SCORE_HEIGHT_INI);
		if (m_pMVPScoreLogo != NULL && GetTextureManager() != NULL)
		{
			m_pMVPScoreLogo->BindTexture(GetTextureManager()->GetTexture(3));
		}
	}

	bool bPlayer1Win = false;	//�v���C���[�P�������Ă��邩�ǂ���

	if (m_nScore[0] >= m_nScore[1])
	{//�PP�̂ق��������ꍇ
		bPlayer1Win = true;
	}
	else
	{//����ȊO�̏ꍇ
		bPlayer1Win = false;
	}

	switch (bPlayer1Win)
	{
	case true:
		for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
		{
			//�X�R�A�̐���
			if (m_pScoreNumber[nCntPlayer] == NULL)
			{
				m_pScoreNumber[nCntPlayer] = CNumber::Create(D3DXVECTOR3(445.0f, 205.0f + (97.0f * nCntPlayer), 0.0f),
					RESULT_SCORE_COL_INI,
					25.0f - (5.0f * nCntPlayer),
					25.0f - (5.0f * nCntPlayer),
					D3DXVECTOR3(-50.0f + (5.0f * nCntPlayer), 0.0f, 0.0f),
					pTexture, m_nScore[nCntPlayer]);
			}

			//�X�R�A�����N�̐���
			if (m_pScoreRank[nCntPlayer] == NULL)
			{
				m_pScoreRank[nCntPlayer] = CNumber::Create(D3DXVECTOR3(130.0f + (50.0f * nCntPlayer), 190.0f + (97.0f * nCntPlayer), 0.0f),
					RESULT_SCORE_COL_INI,
					30.0f - (5.0f * nCntPlayer),
					30.0f - (5.0f * nCntPlayer),
					D3DXVECTOR3(-50.0f + (5.0f * nCntPlayer), 0.0f, 0.0f),
					pTexture, 1 + nCntPlayer);
			}
		}
		break;
	case false:
		for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
		{
			//�X�R�A�̐���
			if (m_pScoreNumber[nCntPlayer] == NULL)
			{
				m_pScoreNumber[nCntPlayer] = CNumber::Create(D3DXVECTOR3(445.0f, 302.0f - (97.0f * nCntPlayer), 0.0f),
					RESULT_SCORE_COL_INI,
					25.0f + (5.0f * nCntPlayer),
					25.0f + (5.0f * nCntPlayer),
					D3DXVECTOR3(-50.0f - (5.0f * nCntPlayer), 0.0f, 0.0f),
					pTexture, m_nScore[nCntPlayer]);
			}

			//�X�R�A�����N�̐���
			if (m_pScoreRank[nCntPlayer] == NULL)
			{
				m_pScoreRank[nCntPlayer] = CNumber::Create(D3DXVECTOR3(130.0f + (50.0f * nCntPlayer), 190.0f + (97.0f * nCntPlayer), 0.0f),
					RESULT_SCORE_COL_INI,
					30.0f - (5.0f * nCntPlayer),
					30.0f - (5.0f * nCntPlayer),
					D3DXVECTOR3(-50.0f + (5.0f * nCntPlayer), 0.0f, 0.0f),
					pTexture, 2 - nCntPlayer);
			}
		}

		break;
	}

}

//=============================================================================
// MVP�W���}�[�̐�������
//=============================================================================
void CResult::CreateMVPJammer(void)
{
	// �����|���S������
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(1);
	}

	// MVP�W���}�[���S�̐���
	if (m_pMVPJammerLogo == NULL)
	{
		m_pMVPJammerLogo = CScene2D::Create(RESULT_JAMMER_POS_INI, RESULT_JAMMER_COL_INI,
			RESULT_JAMMER_WIDTH_INI, RESULT_JAMMER_HEIGHT_INI);

		if (m_pMVPJammerLogo != NULL && GetTextureManager() != NULL)
		{
			m_pMVPJammerLogo->BindTexture(GetTextureManager()->GetTexture(4));
		}
	}

	bool bPlayer1Win = false;	//�v���C���[�P�������Ă��邩�ǂ���

	if (m_nJammer[0] >= m_nJammer[1])
	{//�PP�̂ق��������ꍇ
		bPlayer1Win = true;
	}
	else
	{//����ȊO�̏ꍇ
		bPlayer1Win = false;
	}

	switch (bPlayer1Win)
	{
	case true:
		for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
		{
			//�W���}�[�̐���
			if (m_pJammerNumber[nCntPlayer] == NULL)
			{
				m_pJammerNumber[nCntPlayer] = CNumber::Create(D3DXVECTOR3(1070.0f, 205.0f + (97.0f * nCntPlayer), 0.0f),
					RESULT_SCORE_COL_INI,
					25.0f - (5.0f * nCntPlayer),
					25.0f - (5.0f * nCntPlayer),
					D3DXVECTOR3(-50.0f + (5.0f * nCntPlayer), 0.0f, 0.0f),
					pTexture, m_nJammer[nCntPlayer]);
			}

			//�W���}�[�����N�̐���
			if (m_pJammerRank[nCntPlayer] == NULL)
			{
				m_pJammerRank[nCntPlayer] = CNumber::Create(D3DXVECTOR3(750.0f + (50.0f * nCntPlayer), 190.0f + (97.0f * nCntPlayer), 0.0f),
					RESULT_SCORE_COL_INI,
					30.0f - (5.0f * nCntPlayer),
					30.0f - (5.0f * nCntPlayer),
					D3DXVECTOR3(-50.0f + (5.0f * nCntPlayer), 0.0f, 0.0f),
					pTexture, 1 + nCntPlayer);
			}
		}
		break;
	case false:
		for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
		{
			//�W���}�[�̐���
			if (m_pJammerNumber[nCntPlayer] == NULL)
			{
				m_pJammerNumber[nCntPlayer] = CNumber::Create(D3DXVECTOR3(1070.0f, 302.0f - (97.0f * nCntPlayer), 0.0f),
					RESULT_SCORE_COL_INI,
					25.0f + (5.0f * nCntPlayer),
					25.0f + (5.0f * nCntPlayer),
					D3DXVECTOR3(-50.0f - (5.0f * nCntPlayer), 0.0f, 0.0f),
					pTexture, m_nJammer[nCntPlayer]);
			}

			//�W���}�[�����N�̐���
			if (m_pJammerRank[nCntPlayer] == NULL)
			{
				m_pJammerRank[nCntPlayer] = CNumber::Create(D3DXVECTOR3(750.0f + (50.0f * nCntPlayer), 190.0f + (97.0f * nCntPlayer), 0.0f),
					RESULT_SCORE_COL_INI,
					30.0f - (5.0f * nCntPlayer),
					30.0f - (5.0f * nCntPlayer),
					D3DXVECTOR3(-50.0f + (5.0f * nCntPlayer), 0.0f, 0.0f),
					pTexture, 2 - nCntPlayer);
			}
		}

		break;
	}

}

//=============================================================================
// MVP�N���[���̐�������
//=============================================================================
void CResult::CreateMVPCream(void)
{
	// �����|���S������
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(1);
	}

	// MVP�N���[�����S�̐���
	if (m_pMVPCreamLogo == NULL)
	{
		m_pMVPCreamLogo = CScene2D::Create(RESULT_CREAM_POS_INI, RESULT_CREAM_COL_INI,
			RESULT_CREAM_WIDTH_INI, RESULT_CREAM_HEIGHT_INI);

		if (m_pMVPCreamLogo != NULL && GetTextureManager() != NULL)
		{
			m_pMVPCreamLogo->BindTexture(GetTextureManager()->GetTexture(5));
		}
	}

	bool bPlayer1Win = false;	//�v���C���[�P�������Ă��邩�ǂ���

	if (m_nCream[0] >= m_nCream[1])
	{//�PP�̂ق��������ꍇ
		bPlayer1Win = true;
	}
	else
	{//����ȊO�̏ꍇ
		bPlayer1Win = false;
	}

	switch (bPlayer1Win)
	{
	case true:
		for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
		{
			//�N���[���̐���
			if (m_pCreamNumber[nCntPlayer] == NULL)
			{
				m_pCreamNumber[nCntPlayer] = CNumber::Create(D3DXVECTOR3(445.0f, 545.0f + (97.0f * nCntPlayer), 0.0f),
					RESULT_SCORE_COL_INI,
					25.0f - (5.0f * nCntPlayer),
					25.0f - (5.0f * nCntPlayer),
					D3DXVECTOR3(-50.0f + (5.0f * nCntPlayer), 0.0f, 0.0f),
					pTexture, m_nCream[nCntPlayer]);
			}

			//�N���[�������N�̐���
			if (m_pCreamRank[nCntPlayer] == NULL)
			{
				m_pCreamRank[nCntPlayer] = CNumber::Create(D3DXVECTOR3(130.0f + (50.0f * nCntPlayer), 530.0f + (97.0f * nCntPlayer), 0.0f),
					RESULT_SCORE_COL_INI,
					30.0f - (5.0f * nCntPlayer),
					30.0f - (5.0f * nCntPlayer),
					D3DXVECTOR3(-50.0f + (5.0f * nCntPlayer), 0.0f, 0.0f),
					pTexture, 1 + nCntPlayer);
			}
		}
		break;
	case false:
		for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
		{
			//�N���[���̐���
			if (m_pCreamNumber[nCntPlayer] == NULL)
			{
				m_pCreamNumber[nCntPlayer] = CNumber::Create(D3DXVECTOR3(445.0f, 642.0f - (97.0f * nCntPlayer), 0.0f),
					RESULT_SCORE_COL_INI,
					25.0f + (5.0f * nCntPlayer),
					25.0f + (5.0f * nCntPlayer),
					D3DXVECTOR3(-50.0f - (5.0f * nCntPlayer), 0.0f, 0.0f),
					pTexture, m_nCream[nCntPlayer]);
			}

			//�N���[�������N�̐���
			if (m_pCreamRank[nCntPlayer] == NULL)
			{
				m_pCreamRank[nCntPlayer] = CNumber::Create(D3DXVECTOR3(130.0f + (50.0f * nCntPlayer), 530.0f + (97.0f * nCntPlayer), 0.0f),
					RESULT_SCORE_COL_INI,
					30.0f - (5.0f * nCntPlayer),
					30.0f - (5.0f * nCntPlayer),
					D3DXVECTOR3(-50.0f + (5.0f * nCntPlayer), 0.0f, 0.0f),
					pTexture, 2 - nCntPlayer);
			}
		}

		break;
	}

}

//=============================================================================
// MVP�R���N�^�[�̐�������
//=============================================================================
void CResult::CreateMVPCollector(void)
{
	// �����|���S������
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(1);
	}

	// MVP�R���N�^�[���S�̐���
	if (m_pMVPCollectorLogo == NULL)
	{
		m_pMVPCollectorLogo = CScene2D::Create(RESULT_COLLECTOR_POS_INI, RESULT_COLLECTOR_COL_INI,
			RESULT_COLLECTOR_WIDTH_INI, RESULT_COLLECTOR_HEIGHT_INI);

		if (m_pMVPCollectorLogo != NULL && GetTextureManager() != NULL)
		{
			m_pMVPCollectorLogo->BindTexture(GetTextureManager()->GetTexture(6));
		}
	}

	bool bPlayer1Win = false;	//�v���C���[�P�������Ă��邩�ǂ���

	if (m_nCollector[0] >= m_nCollector[1])
	{//�PP�̂ق��������ꍇ
		bPlayer1Win = true;
	}
	else
	{//����ȊO�̏ꍇ
		bPlayer1Win = false;
	}

	switch (bPlayer1Win)
	{
	case true:
		for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
		{
			//�R���N�^�[�̐���
			if (m_pCollectorNumber[nCntPlayer] == NULL)
			{
				m_pCollectorNumber[nCntPlayer] = CNumber::Create(D3DXVECTOR3(1070.0f, 545.0f + (97.0f * nCntPlayer), 0.0f),
					RESULT_SCORE_COL_INI,
					25.0f - (5.0f * nCntPlayer),
					25.0f - (5.0f * nCntPlayer),
					D3DXVECTOR3(-50.0f + (5.0f * nCntPlayer), 0.0f, 0.0f),
					pTexture, m_nCollector[nCntPlayer]);
			}

			//�R���N�^�[�����N�̐���
			if (m_pCollectorRank[nCntPlayer] == NULL)
			{
				m_pCollectorRank[nCntPlayer] = CNumber::Create(D3DXVECTOR3(750.0f + (50.0f * nCntPlayer), 530.0f + (97.0f * nCntPlayer), 0.0f),
					RESULT_SCORE_COL_INI,
					30.0f - (5.0f * nCntPlayer),
					30.0f - (5.0f * nCntPlayer),
					D3DXVECTOR3(-50.0f + (5.0f * nCntPlayer), 0.0f, 0.0f),
					pTexture, 1 + nCntPlayer);
			}
		}
		break;
	case false:
		for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
		{
			//�R���N�^�[�̐���
			if (m_pCollectorNumber[nCntPlayer] == NULL)
			{
				m_pCollectorNumber[nCntPlayer] = CNumber::Create(D3DXVECTOR3(1070.0f, 642.0f - (97.0f * nCntPlayer), 0.0f),
					RESULT_SCORE_COL_INI,
					25.0f + (5.0f * nCntPlayer),
					25.0f + (5.0f * nCntPlayer),
					D3DXVECTOR3(-50.0f - (5.0f * nCntPlayer), 0.0f, 0.0f),
					pTexture, m_nCollector[nCntPlayer]);
			}

			//�R���N�^�[�����N�̐���
			if (m_pCollectorRank[nCntPlayer] == NULL)
			{
				m_pCollectorRank[nCntPlayer] = CNumber::Create(D3DXVECTOR3(750.0f + (50.0f * nCntPlayer), 530.0f + (97.0f * nCntPlayer), 0.0f),
					RESULT_SCORE_COL_INI,
					30.0f - (5.0f * nCntPlayer),
					30.0f - (5.0f * nCntPlayer),
					D3DXVECTOR3(-50.0f + (5.0f * nCntPlayer), 0.0f, 0.0f),
					pTexture, 2 - nCntPlayer);
			}
		}

		break;
	}

}

//=============================================================================
// ���U���g�̃��S�J������
//=============================================================================
void CResult::ReleaseLogo(void)
{
	if (m_pLogo != NULL)
	{
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}
}

//=============================================================================
// ���U���g�̃n�C�X�R�A�J������
//=============================================================================
void CResult::ReleaseHighScore(void)
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
// ���U���g��MVP�֘A�̊J������
//=============================================================================
void CResult::ReleaseMVP(void)
{
	// �X�R�A���S�̊J��
	if (m_pMVPScoreLogo != NULL)
	{
		m_pMVPScoreLogo->Uninit();
		m_pMVPScoreLogo = NULL;
	}

	// �W���}�[���S�J��
	if (m_pMVPJammerLogo != NULL)
	{
		m_pMVPJammerLogo->Uninit();
		m_pMVPJammerLogo = NULL;
	}

	// �N���[�����S�̊J��
	if (m_pMVPCreamLogo != NULL)
	{
		m_pMVPCreamLogo->Uninit();
		m_pMVPCreamLogo = NULL;
	}

	// �R���N�^�[���S�J��
	if (m_pMVPCollectorLogo != NULL)
	{
		m_pMVPCollectorLogo->Uninit();
		m_pMVPCollectorLogo = NULL;
	}


	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		// �X�R�A�J��
		if (m_pScoreNumber[nCntPlayer] != NULL)
		{
			m_pScoreNumber[nCntPlayer]->Uninit();
			m_pScoreNumber[nCntPlayer] = NULL;
		}

		// �W���}�[�J��
		if (m_pJammerNumber[nCntPlayer] != NULL)
		{
			m_pJammerNumber[nCntPlayer]->Uninit();
			m_pJammerNumber[nCntPlayer] = NULL;
		}

		// �N���[���J��
		if (m_pCreamNumber[nCntPlayer] != NULL)
		{
			m_pCreamNumber[nCntPlayer]->Uninit();
			m_pCreamNumber[nCntPlayer] = NULL;
		}

		// �R���N�^�[�J��
		if (m_pCollectorNumber[nCntPlayer] != NULL)
		{
			m_pCollectorNumber[nCntPlayer]->Uninit();
			m_pCollectorNumber[nCntPlayer] = NULL;
		}

		// �X�R�A�����N�J��
		if (m_pScoreRank[nCntPlayer] != NULL)
		{
			m_pScoreRank[nCntPlayer]->Uninit();
			m_pScoreRank[nCntPlayer] = NULL;
		}

		// �W���}�[�����N�J��
		if (m_pJammerRank[nCntPlayer] != NULL)
		{
			m_pJammerRank[nCntPlayer]->Uninit();
			m_pJammerRank[nCntPlayer] = NULL;
		}

		// �N���[�������N�J��
		if (m_pCreamRank[nCntPlayer] != NULL)
		{
			m_pCreamRank[nCntPlayer]->Uninit();
			m_pCreamRank[nCntPlayer] = NULL;
		}

		// �R���N�^�[�����N�J��
		if (m_pCollectorRank[nCntPlayer] != NULL)
		{
			m_pCollectorRank[nCntPlayer]->Uninit();
			m_pCollectorRank[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// ���U���g�̑ҋ@��Ԃ̍X�V����
//=============================================================================
void CResult::WaitUpdate(void)
{
	CDebugProc::Print(1, "�ҋ@���\n");

	m_nStateCounter++;
	if (m_nStateCounter % RESULT_WAITSTATE_TIME == 0)
	{// �J�E���^�[�����l�ɒB����
		SetState(STATE_MVP);
		m_nStateCounter = 0;
	}

	CDebugProc::Print(1, "%d\n", m_nStateCounter);
}

//=============================================================================
// ���U���g��MVP��Ԃ̍X�V����
//=============================================================================
void CResult::MVPUpdate(void)
{
	CDebugProc::Print(1, "MVP���\n");

	if (m_nStateCounter % RESULT_NEXTHIGHSCORE_TIME == 0)
	{// �J�E���^�[�����l�ɒB����
	 // �n�C�X�R�A��\�������Ԃ�
		SetState(STATE_NORMAL);
		m_nStateCounter = 0;

		// ���U���g���S���J��
		ReleaseLogo();

		// MVP�\���ɕK�v�Ȃ��̂𐶐�
		CreateMVP();
	}

	CDebugProc::Print(1, "%d\n", m_nStateCounter);
}

//=============================================================================
// ���U���g�̒ʏ��Ԃ̍X�V����
//=============================================================================
void CResult::NormalUpdate(void)
{
	CDebugProc::Print(1, "�ʏ���\n");

	// �J�E���^�[���Z
	m_nStateCounter++;

	if (m_bHighScore == true)
	{// �n�C�X�R�A���X�V����Ă���
		if (m_nStateCounter % RESULT_NEXTHIGHSCORE_TIME == 0)
		{// �J�E���^�[�����l�ɒB����
			// �n�C�X�R�A��\�������Ԃ�
			SetState(STATE_HIGHSCORE);
			m_nStateCounter = 0;

			// MVP���J��
			ReleaseMVP();

			// �n�C�X�R�A�\���ɕK�v�Ȃ��̂𐶐�
			CreateHighScore();
		}

		// ���̏�ԑҋ@����
		WaitInputToHighScore();
	}
	else
	{// �n�C�X�R�A���X�V����Ă��Ȃ�
		if (m_nStateCounter % RESULT_CHANGEMODE_TIME == 0)
		{// �J�E���^�[�����l�ɒB����
			SetState(STATE_END);
			m_nStateCounter = 0;
		}

		// ���̃��[�h�ҋ@����
		WaitInputToNextMode();
	}

	CDebugProc::Print(1, "%d\n", m_nStateCounter);
}

//=============================================================================
// ���U���g�̃n�C�X�R�A��\�������Ԃ̍X�V����
//=============================================================================
void CResult::HighScoreUpdate(void)
{
	CDebugProc::Print(1, "�n�C�X�R�A�\�����\n");

	// �X�R�A��_�ł�����
	if (m_nStateCounter % RESULT_HIGHSCORE_FALSH_TIME == 0)
	{
		// �_�ł̐F��ݒ�
		if (m_FalshCol.r < 1.0f)
		{
			m_FalshCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			m_FalshCol = RESULT_HIGHSCORE_FALSH_COL;
		}

		// �|���S���̐F��ݒ�
		// ���S�|���S��
		if (m_pHighScoreLogo != NULL)
		{
			m_pHighScoreLogo->SetCol(m_FalshCol);
			m_pHighScoreLogo->SetVtxBuffCol();
		}

		// �����|���S��
		if (m_pHighScore != NULL)
		{
			m_pHighScore->SetNumberCol(m_FalshCol);
			m_pHighScore->SetNumberVtxBuffCol();
		}
	}

	// �J�E���^�[���Z
	m_nStateCounter++;

	if (m_nStateCounter % RESULT_CHANGEMODE_TIME == 0)
	{// �J�E���^�[�����l�ɒB����
		SetState(STATE_END);
		m_nStateCounter = 0;
	}

	CDebugProc::Print(1, "%d\n", m_nStateCounter);

	// ���̃��[�h�ҋ@����
	WaitInputToNextMode();
}

//=============================================================================
// ���U���g�̏I����Ԃ̍X�V����
//=============================================================================
void CResult::EndUpdate(void)
{
	CDebugProc::Print(1, "�I�����\n");

	CFade *pFade = CManager::GetFade();
	if (pFade == NULL) return;

	if (pFade->GetFade() == CFade::FADE_NONE)
	{// �t�F�[�h���g�p����Ă��Ȃ�
		pFade->SetFade(CManager::MODE_TITLE);
	}
}

//=============================================================================
// ���U���g�̃n�C�X�R�A��\�������Ԃɍs�����߂̑ҋ@����
//=============================================================================
void CResult::WaitInputToHighScore(void)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	if (pKey == NULL) return;

	if (pKey->GetTrigger(DIK_RETURN) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// ����{�^���������ꂽ
		// ��Ԃ�ݒ�
		SetState(STATE_HIGHSCORE);
		m_nStateCounter = 0;

		// ���U���g���S���J��
		ReleaseLogo();

		// �n�C�X�R�A�\���ɕK�v�Ȃ��̂𐶐�
		CreateHighScore();
	}
}

//=============================================================================
// ���U���g�̎��̃��[�h�ɍs�����߂̑ҋ@����
//=============================================================================
void CResult::WaitInputToNextMode(void)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	if (pKey == NULL) return;

	if (pKey->GetTrigger(DIK_RETURN) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// ����{�^���������ꂽ
		SetState(STATE_END);
	}
}

//=============================================================================
// ���U���g�̃V�X�e������ǂݍ���
//=============================================================================
void CResult::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(RESULT_SYSTEM_FILENAME);
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
// ���U���g�̃V�X�e�������t�@�C������ǂݍ���
//=============================================================================
void CResult::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
{
	int nCntTex = 0;

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
		else if (CFunctionLib::Memcmp(pStr, RESULTLOGOSET) == 0)
		{// ���U���g���S��񂾂���
			LoadLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�t�@�C���I���̍��}������
			break;
		}
	}
}

//=============================================================================
// ���U���g�̓ǂݍ��ރe�N�X�`���t�@�C������ǂݍ���
//=============================================================================
void CResult::LoadTexFileName(char *pStr, int nCntTex)
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
// ���U���g�̃��S�|���S������ǂݍ���
//=============================================================================
void CResult::LoadLogo(CFileLoader *pFileLoader, char *pStr)
{
	// �f�[�^������
	m_ResultLogoData.nTexIdx = 0;
	m_ResultLogoData.pos = RESULT_LOGO_POS_INI;
	m_ResultLogoData.col = RESULT_LOGO_COL_INI;
	m_ResultLogoData.fWidth = RESULT_LOGO_WIDTH_INI;
	m_ResultLogoData.fHeight = RESULT_LOGO_HEIGHT_INI;

	// �f�[�^�ǂݍ���
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			m_ResultLogoData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			m_ResultLogoData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			m_ResultLogoData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			m_ResultLogoData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			m_ResultLogoData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_RESULTLOGOSET) == 0)
		{// ���U���g���S�|���S�����I���̍��}������
			break;
		}
	}

	// �|���S������
	CreateLogo();
}

//=============================================================================
// ���U���g�̃n�C�X�R�A���t�@�C������ǂݍ��ޏ���
//=============================================================================
void CResult::LoadHighScore(char *pFileName)
{
	CFileLoader *pFileLoader = NULL;  // �t�@�C���ǂݍ��ݗp�N���X�ւ̃|�C���^
	pFileLoader = CFileLoader::Create(pFileName);
	if (pFileLoader == NULL) { return; }

	// �t�@�C����ǂݐi�߂�
	char aStr[256];
	strcpy(aStr, pFileLoader->GetString(aStr));
	if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
	{// �X�N���v�g�ǂݍ��݊J�n�̍��}��������
		while (1)
		{
			strcpy(aStr, pFileLoader->GetString(aStr));
			if (CFunctionLib::Memcmp(aStr, HIGHSCORE) == 0)
			{// �n�C�X�R�A��񂾂���
				CTitle::SetHighScore(CFunctionLib::ReadInt(aStr, HIGHSCORE));
			}
			else if (CFunctionLib::Memcmp(aStr, END_SCRIPT) == 0)
			{// �X�N���v�g�ǂݍ��ݏI���̍��}��������
				break;
			}
		}
	}

	// �������̊J��
	if (pFileLoader != NULL)
	{
		pFileLoader->Uninit();
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
// ���U���g�̃n�C�X�R�A��ۑ����鏈��
//=============================================================================
void CResult::SaveHighScore(void)
{
	CFileSaver *pFileSaver = NULL;  // �t�@�C���ۑ��p�N���X�ւ̃|�C���^
	pFileSaver = CFileSaver::Create(RESULT_HIGHSCORE_FILENAME);
	if (pFileSaver != NULL)
	{// �t�@�C�����ǂݍ��߂�
	    // �t�@�C���̖`��������������
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# �n�C�X�R�A�X�N���v�g�t�@�C�� [highscore.txt]\n");
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# ���̍s�͐�Ώ����Ȃ����ƁI\n", SCRIPT);
		pFileSaver->Print("\n");

		// �n�C�X�R�A����������
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("# �n�C�X�R�A\n");
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("%s%d\n\n", HIGHSCORE, CTitle::GetHighScore());

		// �X�N���v�g�I���̍��}����������
		pFileSaver->Print("%s		# ���̍s�͐�Ώ����Ȃ����ƁI\n", END_SCRIPT);
	}

	// �������̊J��
	if (pFileSaver != NULL)
	{
		pFileSaver->Uninit();
		delete pFileSaver;
		pFileSaver = NULL;
	}
}

//=============================================================================
// ���U���g�̕ϐ�������������
//=============================================================================
void CResult::ClearVariable(void)
{
	m_State = STATE_WAIT;
	m_nStateCounter = 0;
	m_pLogo = NULL;
	m_pHighScore = NULL;
	m_pHighScoreLogo = NULL;

	m_pMVPScoreLogo = NULL;
	m_pMVPJammerLogo = NULL;
	m_pMVPCreamLogo = NULL;
	m_pMVPCollectorLogo = NULL;

	// MVP�̏���������
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_pScoreNumber[nCntPlayer] = NULL;
		m_pJammerNumber[nCntPlayer] = NULL;
		m_pCreamNumber[nCntPlayer] = NULL;
		m_pCollectorNumber[nCntPlayer] = NULL;
		m_pScoreRank[nCntPlayer] = NULL;
		m_pJammerRank[nCntPlayer] = NULL;
		m_pCreamRank[nCntPlayer] = NULL;
		m_pCollectorRank[nCntPlayer] = NULL;
	}
}

//=============================================================================
// ���U���g�̏�Ԃ�ݒ肷��
//=============================================================================
void CResult::SetState(const STATE state)
{
	m_State = state;
	m_nStateCounter = 0;
}

//=============================================================================
// ���U���g�̃n�C�X�R�A���o�����ǂ�����ݒ肷��
//=============================================================================
void CResult::SetHighScore(void)
{
	m_bHighScore = true;
}

//=============================================================================
// ���U���g�̏�Ԃ��擾����
//=============================================================================
CResult::STATE CResult::GetState(void)
{
	return m_State;
}

//=============================================================================
// �X�R�A�̐ݒu����
//=============================================================================
void CResult::SetScore(int nScore, int nIdx)
{
	m_nScore[nIdx] = nScore;
}

//=============================================================================
// �W���}�[�̐ݒu����
//=============================================================================
void CResult::SetJammer(int nJammer, int nIdx)
{
	m_nJammer[nIdx] = nJammer;
}

//=============================================================================
// �N���[���̐ݒu����
//=============================================================================
void CResult::SetCream(int nCream, int nIdx)
{
	m_nCream[nIdx] = nCream;
}

//=============================================================================
// �R���N�^�[�̐ݒu����
//=============================================================================
void CResult::SetCollector(int nCollector, int nIdx)
{
	m_nCollector[nIdx] = nCollector;
}