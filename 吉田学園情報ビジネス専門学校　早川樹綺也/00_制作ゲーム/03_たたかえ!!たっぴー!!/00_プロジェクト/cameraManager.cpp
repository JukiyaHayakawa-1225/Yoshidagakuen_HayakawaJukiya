//*****************************************************************************
//
//     �J�����Ǌ��̏���[cameraManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "cameraManager.h"
#include "camera.h"

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CCameraManager::CCameraManager()
{
	// �e��l�̃N���A
	m_nNumCamera = 0;     // �J�����̐�
	m_apCamera = NULL;    // �J�����N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�X�g���N�^
//=============================================================================
CCameraManager::~CCameraManager()
{

}

//=============================================================================
//    ��������
//=============================================================================
CCameraManager *CCameraManager::Create(int nNumCamera)
{
	CCameraManager *pCameraManager = NULL;  // �J�����Ǌ��N���X�^�̃|�C���^
	if (pCameraManager == NULL)
	{// ����������ɂȂ��Ă���
		pCameraManager = new CCameraManager;
		if (pCameraManager != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pCameraManager->Init(nNumCamera)))
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

	return pCameraManager;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CCameraManager::Init(int nNumCamera)
{
	// �J�����̐���ݒ�
	m_nNumCamera = nNumCamera;

	// �J�����N���X�̃|�C���^���m�ۂ���
	if (m_apCamera == NULL)
	{// ���������m�ۂł����Ԃł���
		m_apCamera = new CCamera*[nNumCamera];
		if (m_apCamera != NULL)
		{// ���������m�ۂł���
			for (int nCntCamera = 0; nCntCamera < nNumCamera; nCntCamera++)
			{// �J�����̐������J��Ԃ�
				m_apCamera[nCntCamera] = NULL;
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CCameraManager::Uninit(void)
{
	if (m_apCamera != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntCamera = 0; nCntCamera < m_nNumCamera; nCntCamera++)
		{// ���C�g�̐������J��Ԃ�
			if (m_apCamera[nCntCamera] != NULL)
			{// ���������m�ۂ���Ă���
				m_apCamera[nCntCamera]->Uninit();

				// �������̊J��
				delete m_apCamera[nCntCamera];
				m_apCamera[nCntCamera] = NULL;
			}
		}

		// �������̊J��
		delete m_apCamera;
		m_apCamera = NULL;
	}
}

//=============================================================================
//    �X�V����
//=============================================================================
void CCameraManager::Update(void)
{
	if (m_apCamera != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntCamera = 0; nCntCamera < m_nNumCamera; nCntCamera++)
		{// ���C�g�̐������J��Ԃ�
			if (m_apCamera[nCntCamera] != NULL)
			{// ���������m�ۂ���Ă���
				m_apCamera[nCntCamera]->Update();
			}
		}
	}
}

//=============================================================================
//    �J�����̐ݒ�
//=============================================================================
void CCameraManager::BindCamera(int nIdxCamera)
{
	if (m_apCamera != NULL)
	{// ���������m�ۂ���Ă���
		if (m_apCamera[nIdxCamera] != NULL)
		{// ���������m�ۂ���Ă���
			m_apCamera[nIdxCamera]->SetCamera();
		}
	}
}

//=============================================================================
//    �J�����̐���ݒ�
//=============================================================================
void CCameraManager::SetNumCamera(const int nNumCamera)
{
	m_nNumCamera = nNumCamera;
}

//=============================================================================
//    �J�����N���X�ւ̃|�C���^��ݒ�
//=============================================================================
void CCameraManager::SetCamera(CCamera *pCamera, const int nIdxCamera)
{
	m_apCamera[nIdxCamera] = pCamera;
}

//=============================================================================
//    �J�����̐����擾
//=============================================================================
int CCameraManager::GetNumCamera(void)
{
	return m_nNumCamera;
}

//=============================================================================
//    �J�����N���X�ւ̃|�C���^���擾
//=============================================================================
CCamera *CCameraManager::GetCamera(const int nIdxCamera)
{
	return m_apCamera[nIdxCamera];
}