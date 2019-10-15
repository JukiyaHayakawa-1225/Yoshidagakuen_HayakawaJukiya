//=============================================================================
//
// �}�l�[�W������ [manager.cpp]
// Author : Jukiya Hayakawa & Hodaka Niwa
//
//=============================================================================
#include "manager.h"
#include "fileLoader.h"
#include "functionlib.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "server.h"
#include "debugproc.h"
#include "basemode.h"
#include "title.h"
#include "charaselect.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "fade.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MANAGER_SYSTEM_FILENAME "data/TEXT/system.ini"   // �ǂݍ��ރV�X�e���t�@�C����

// �l�ǂݍ��ݗp�̃p�X��
#define SERVER_ADDRESS "SERVER_ADDRESS = "
#define SOUND_FILENAME "SOUND_FILENAME = "
#define HIGHSCORE_FILENAME "HIGHSCORE_FILENAME = "

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CManager::MODE	CManager::m_mode = MODE::MODE_TITLE;  		// ���[�h�̏��
CRenderer		*CManager::m_pRenderer = NULL;				// �����_�����O�̃|�C���^
CInputKeyboard	*CManager::m_pInputKeyboard = NULL;			// �L�[�{�[�h�̃|�C���^
CInputMouse     *CManager::m_pMouse = NULL;                 // �}�E�X�̃|�C���^
CInputJoyStick	*CManager::m_pInputJoypad = NULL;			// �W���C�p�b�h�|�C���^
CXInput	        *CManager::m_pXInput = NULL;			    // XInput�|�C���^
CSound			*CManager::m_pSound = NULL;					// �T�E���h�̃|�C���^
CClient         *CManager::m_pClient = NULL;                // �N���C�A���g�̃|�C���^
CDebugProc		*CManager::m_pDebugproc = NULL;				// �f�o�b�O�̃|�C���^
CBasemode		*CManager::m_pBasemode = NULL;				// �x�[�X���[�h�̃|�C���^
CFade			*CManager::m_pFade = NULL;					// �t�F�[�h�̃|�C���^
char            CManager::m_aServerAddr[256] = "\0";        // �T�[�o�[��IP�A�h���X

//=============================================================================
// �}�l�[�W���̃R���X�g���N�^
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
// �}�l�[�W���̃f�X�g���N�^
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// �}�l�[�W���̏���������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// �T�[�o�[�̃X�^�[�g�A�b�v
	CServer::ServerStartUp();

	// �����_�����O�̐���
	CreateRenderer(hWnd, bWindow);

	// �C���v�b�g�̐���
	CreateInput(hInstance, hWnd);

	// �t�F�[�h�̐���
	CreateFade();

	// �f�o�b�O���O�̐���
	CreateDebugProc();

	// �V�X�e����ǂݍ���
	LoadSystem(hWnd);

	// �Q�[�����`���[�g���A������J�n�̏ꍇ�N���C�A���g�𐶐����鏈��������
	if (m_mode == MODE_GAME || m_mode == MODE_TUTORIAL)
	{
		CreateClient();
	}

	return S_OK;
}

//=============================================================================
// �}�l�[�W���̏I������
//=============================================================================
void CManager::Uninit(void)
{
	// �N���C�A���g�̔j��
	ReleaseClient();

	// �x�[�X���[�h�̔j��
	if (m_pBasemode != NULL)
	{
		m_pBasemode->Uninit();
		delete m_pBasemode;
		m_pBasemode = NULL;
	}

	// �L�[�{�[�h�̔j��
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// �}�E�X�̔j��
	if (m_pMouse != NULL)
	{
		m_pMouse->Uninit();
		delete m_pMouse;
		m_pMouse = NULL;
	}

	// �W���C�p�b�h�̔j��
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}

	// XInput�̔j��
	if (m_pXInput != NULL)
	{
		m_pXInput->Uninit();
		delete m_pXInput;
		m_pXInput = NULL;
	}

	// �����_�����O�̔j��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// �t�F�[�h�̔j��
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = NULL;
	}

	// �T�E���h�̔j��
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	// �f�o�b�O���O�̔j��
	if (m_pDebugproc != NULL)
	{
		m_pDebugproc->Uninit();
		delete m_pDebugproc;
		m_pDebugproc = NULL;
	}

	//�S�Ă̊J������
	CScene::ReleaseAll();

	// �T�[�o�[�̃N���[���A�b�v
	CServer::ServerCleanUp();

	// �n�C�X�R�A��ۑ�����
	CResult::SaveHighScore();
}

//=============================================================================
// �}�l�[�W���̍X�V����
//=============================================================================
void CManager::Update(void)
{
	// �f�o�b�O�R�}���h
	if (m_pInputKeyboard->GetTrigger(DIK_F1) == true && m_pInputKeyboard->GetPress(DIK_LSHIFT) == false)
	{
		m_pDebugproc->SwitchDisp();
	}

	// �L�[�{�[�h�̍X�V����
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	// �}�E�X�̍X�V����
	if (m_pMouse != NULL)
	{
		m_pMouse->Update();
	}

	// �W���C�p�b�h�̍X�V����
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Update();

	}

	// XInput�̍X�V����
	if (m_pXInput != NULL)
	{
		m_pXInput->Update();
	}

	// �t�F�[�h�̍X�V����
	if (m_pFade != NULL)
	{
		m_pFade->Update();
	}

	// �����_�����O�̍X�V����
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	// �x�[�X���[�h�̍X�V����
	if (m_pBasemode != NULL)
	{
		m_pBasemode->Update();
	}
}

//=============================================================================
// �}�l�[�W���̕`�揈��
//=============================================================================
void CManager::Draw(void)
{
	// �����_�����O�̕`�揈��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//=============================================================================
// �V�X�e����ǂݍ���
//=============================================================================
void CManager::LoadSystem(HWND hWnd)
{
	// �t�@�C���I�[�v��
	CFileLoader *pFileLoader = CFileLoader::Create(MANAGER_SYSTEM_FILENAME);
	if (pFileLoader == NULL) return;

	// �X�N���v�g��ǂݍ���
	char aStr[256] = "\0";
	strcpy(aStr, pFileLoader->GetString(aStr));
	if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
	{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
		LoadSystemScript(hWnd, pFileLoader, aStr);
	}

	// �t�@�C�������
	if (pFileLoader != NULL)
	{
		pFileLoader->Uninit();
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
// �X�N���v�g��ǂݍ���
//=============================================================================
void CManager::LoadSystemScript(HWND hWnd, CFileLoader *pFileLoader, char *pStr)
{
	char aSoundFileName[256] = "\0";
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SERVER_ADDRESS) == 0)
		{// �T�[�o�[��IP�A�h���X������
			strcpy(m_aServerAddr, CFunctionLib::ReadString(pStr, m_aServerAddr, SERVER_ADDRESS));
		}
		else if (CFunctionLib::Memcmp(pStr, SOUND_FILENAME) == 0)
		{// �T�E���h�f�[�^�̃t�@�C����������
			strcpy(aSoundFileName, CFunctionLib::ReadString(pStr, aSoundFileName, SOUND_FILENAME));
			CreateSound(hWnd, aSoundFileName);
		}
		else if (CFunctionLib::Memcmp(pStr, HIGHSCORE_FILENAME) == 0)
		{// �n�C�X�R�A�t�@�C��������
			CResult::LoadHighScore(CFunctionLib::ReadString(pStr, pStr, HIGHSCORE_FILENAME));
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��ݏI���̍��}������
			break;
		}
	}
}

//=============================================================================
// �����_�����O�̐���
//=============================================================================
void CManager::CreateRenderer(HWND hWnd, bool bWindow)
{
	if (m_pRenderer == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pRenderer = new CRenderer;
		if (m_pRenderer != NULL)
		{// ���������m�ۂł���
			m_pRenderer->Init(hWnd, bWindow);
		}
	}
}

//=============================================================================
// �C���v�b�g�̐���
//=============================================================================
void CManager::CreateInput(HINSTANCE hInstance, HWND hWnd)
{
	// �L�[�{�[�h�̐���
	CreateKeyboard(hInstance, hWnd);

	// �}�E�X�̐���
	CreateMouse(hInstance, hWnd);

	// �W���C�X�e�B�b�N�̐���
	//CreateJoyStick(hInstance, hWnd);

	// XInput�̐���
	CreateXInput();
}

//=============================================================================
// �L�[�{�[�h�̐���
//=============================================================================
void CManager::CreateKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// �L�[�{�[�h�̍쐬
	if (m_pInputKeyboard == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pInputKeyboard = new CInputKeyboard;
		if (m_pInputKeyboard != NULL)
		{// ���������m�ۂł���
			m_pInputKeyboard->Init(hInstance, hWnd);
		}
	}
}

//=============================================================================
// �}�E�X�̐���
//=============================================================================
void CManager::CreateMouse(HINSTANCE hInstance, HWND hWnd)
{
	// �}�E�X�̍쐬
	if (m_pMouse == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pMouse = new CInputMouse;
		if (m_pMouse != NULL)
		{// ���������m�ۂł���
			m_pMouse->Init(hInstance, hWnd);
		}
	}
}

//=============================================================================
// �W���C�X�e�B�b�N�̐���
//=============================================================================
void CManager::CreateJoyStick(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInputJoypad == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pInputJoypad = new CInputJoyStick(1);
		if (m_pInputJoypad != NULL)
		{// ���������m�ۂł���
			m_pInputJoypad->Init(hInstance, hWnd);
		}
	}
}

//=============================================================================
// XInput�̐���
//=============================================================================
void CManager::CreateXInput(void)
{
	if (m_pXInput == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pXInput = new CXInput;
		if (m_pXInput != NULL)
		{// ���������m�ۂł���
			m_pXInput->Init();
		}
	}
}

//=============================================================================
// �T�E���h�̐���
//=============================================================================
void CManager::CreateSound(HWND hWnd, char *pFileName)
{
	if (m_pSound == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pSound = new CSound;
		if (m_pSound != NULL)
		{// ���������m�ۂł���
			m_pSound->Init(hWnd, pFileName);
		}
	}
}

//=============================================================================
// �t�F�[�h�̐���
//=============================================================================
void CManager::CreateFade(void)
{
	m_pFade = CFade::Create();
	m_pFade->SetFade(m_mode, m_pFade->FADE_IN);
}

//=============================================================================
// �f�o�b�O���O�̐���
//=============================================================================
void CManager::CreateDebugProc(void)
{
	if (m_pDebugproc == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pDebugproc = new CDebugProc;
		if (m_pDebugproc != NULL)
		{// ���������m�ۂł���
			m_pDebugproc->Init();
		}
	}
}

//=============================================================================
// �N���C�A���g�̐���
//=============================================================================
void CManager::CreateClient(void)
{
	m_pClient = CClient::Create(m_aServerAddr);
}

//=============================================================================
// �N���C�A���g�̍X�V
//=============================================================================
void CManager::ClientUpdate(void)
{
	if (m_pClient != NULL)
	{
		m_pClient->Update();
	}
}

//=============================================================================
// �N���C�A���g�̊J��
//=============================================================================
void CManager::ReleaseClient(void)
{
	if (m_pClient != NULL)
	{
		m_pClient->Uninit();
		delete m_pClient;
		m_pClient = NULL;
	}
}

//=============================================================================
// �����_�����O�̎擾
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
// �L�[�{�[�h�̎擾
//=============================================================================
CInputKeyboard *CManager::GetKeyboard(void)
{
	return m_pInputKeyboard;
}

//=============================================================================
// �}�E�X�N���X�̎擾
//=============================================================================
CInputMouse *CManager::GetMouse(void)
{
	return m_pMouse;
}

//=============================================================================
// �W���C�p�b�h�̎擾
//=============================================================================
CInputJoyStick *CManager::GetJoypad(void)
{
	return m_pInputJoypad;
}

//=============================================================================
// XInput�̎擾
//=============================================================================
CXInput *CManager::GetXInput(void)
{
	return m_pXInput;
}

//=============================================================================
// �T�E���h�̎擾
//=============================================================================
CSound *CManager::GetSound(void)
{
	return m_pSound;
}

//=============================================================================
// �N���C�A���g�̎擾
//=============================================================================
CClient *CManager::GetClient(void)
{
	return m_pClient;
}

//=============================================================================
// �x�[�X���[�h�̎擾
//=============================================================================
CBasemode *CManager::GetBaseMode(void)
{
	return m_pBasemode;
}

//=============================================================================
// �^�C�g���̎擾
//=============================================================================
CTitle *CManager::GetTitle(void)
{
	return (CTitle*)m_pBasemode;
}

//=============================================================================
// �L�����Z���N�g�̎擾
//=============================================================================
CCharaSelect *CManager::GetCharaSelect(void)
{
	return (CCharaSelect*)m_pBasemode;
}

//=============================================================================
// �`���[�g���A���̎擾
//=============================================================================
CTutorial *CManager::GetTutorial(void)
{
	return (CTutorial*)m_pBasemode;
}

//=============================================================================
// �Q�[���̎擾
//=============================================================================
CGame *CManager::GetGame(void)
{
	return (CGame*)m_pBasemode;
}

//=============================================================================
// ���U���g�̎擾
//=============================================================================
CResult *CManager::GetResult(void)
{
	return (CResult*)m_pBasemode;
}

//=============================================================================
// ���[�h�̎擾
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}

//=============================================================================
// ���[�h�̎擾
//=============================================================================
CFade *CManager::GetFade(void)
{
	return m_pFade;
}

//=============================================================================
// ���[�h�̐ݒu
//=============================================================================
void CManager::SetMode(MODE mode)
{
	// �O��̃��[�h��j��
	if (m_pBasemode != NULL)
	{
		m_pBasemode->Uninit();
		delete m_pBasemode;
		m_pBasemode = NULL;
	}

	m_mode = mode;	//���

	switch (mode)
	{// ���[�h�̂���ď����킯
	case MODE_TITLE:        // �^�C�g��
		if (m_pBasemode == NULL)
		{
			m_pBasemode = CTitle::Create();
		}
		break;
	case MODE_CHARASELECT:  // �L�����Z���N�g
		if (m_pBasemode == NULL)
		{
			m_pBasemode = CCharaSelect::Create();
		}
		break;
	case MODE_TUTORIAL:     // �`���[�g���A��
		if (m_pBasemode == NULL)
		{
			m_pBasemode = CTutorial::Create();
		}
		break;
	case MODE_GAME:         // �Q�[��
		if (m_pBasemode == NULL)
		{
			m_pBasemode = CGame::Create();
		}
		break;
	case MODE_RESULT:       // ���U���g
		if (m_pBasemode == NULL)
		{
			m_pBasemode = CResult::Create();
		}
		break;
	}
}