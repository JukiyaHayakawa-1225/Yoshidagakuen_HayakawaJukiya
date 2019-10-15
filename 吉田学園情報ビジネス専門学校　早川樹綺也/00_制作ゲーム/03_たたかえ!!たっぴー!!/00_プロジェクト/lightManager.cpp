//*****************************************************************************
//
//     ���C�g�Ǌ��̏���[lightManager.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "lightManager.h"
#include "light.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CLightManager::CLightManager()
{
	m_nNumLight = 0;    // ���C�g�̐�
	m_apLight = NULL;   // ���C�g�N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CLightManager::~CLightManager()
{

}

//=============================================================================
//    ��������
//=============================================================================
CLightManager *CLightManager::Create(int nNumLight)
{
	CLightManager *pLightManager = NULL;  // ���C�g�Ǌ��N���X�^�̃|�C���^
	if (pLightManager == NULL)
	{// ����������ɂȂ��Ă���
		pLightManager = new CLightManager;
		if (pLightManager != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pLightManager->Init(nNumLight)))
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

	return pLightManager;
}

//=============================================================================
//    ���g���R�s�[���鏈��
//=============================================================================
CLightManager *CLightManager::Cpy(CLightManager *pLightManagerOld, int nNumLight)
{
	// �O��̃��C�g�̐����L��
	int nNumLightOld = pLightManagerOld->GetNumLight();

	// �O��̃��C�g��S��Off�ɐݒ�
	for (int nCntLight = 0; nCntLight < nNumLightOld; nCntLight++)
	{// ����K�v�Ȑ������J��Ԃ�
		CManager::GetRenderer()->GetDevice()->LightEnable(nCntLight, false);
	}

	// ����K�v�Ȑ��������C�g��ݒ肷��
	CLight *pLight = NULL;
	CLightManager *pLightManager = CLightManager::Create(nNumLight);
	for (int nCntLight = 0; nCntLight < nNumLightOld; nCntLight++)
	{// �R�s�[�̂��߂ɑO��̃��C�g�̐����J��Ԃ�
		if (nCntLight < nNumLight)
		{// ����K�v�Ȑ��𒴂��Ă��Ȃ�
			// ���C�g�𐶐�
			pLight = pLightManagerOld->GetCpyLight(nCntLight);

			// ���C�g��ݒ�
			pLightManager->SettingLight(pLight, nCntLight);
		}
	}

	// ����Ȃ����̃��C�g�͓K���ɒl�����߂Đ���
	for (int nCntLight = nNumLightOld; nCntLight < nNumLight; nCntLight++)
	{// �R�s�[�̂��߂ɑO��̃��C�g�̐����J��Ԃ�
	    // ���C�g�𐶐�
		pLight = CDirectionalLight::Create(D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// ���C�g��ݒ�
		pLightManager->SettingLight(pLight, nCntLight);
	}


	// �O��̃��C�g�Ǌ��N���X��j������
	if (pLightManagerOld != NULL)
	{
		pLightManagerOld->Uninit();
		delete pLightManagerOld;
		pLightManagerOld = NULL;
	}

	return pLightManager;
}

//=============================================================================
//    ���C�g�����R�s�[���鏈��
//=============================================================================
CLight *CLightManager::GetCpyLight(int nIdx)
{
	// ���C�g���Q�b�g����
	CLight *pLightCpy = GetLight(nIdx);

	// �����R�s�[���ăA�h���X��Ԃ�
	CLight *pLight = NULL;
	D3DLIGHT9 Light = pLightCpy->GetLight();
	if (pLightCpy->GetType() == CLight::TYPE_DIRECTIONAL)
	{// �f�B���N�V���i�����C�g��������
		pLight = CDirectionalLight::Create(Light.Direction, Light.Diffuse, Light.Ambient, Light.Specular);
	}
	else if (pLightCpy->GetType() == CLight::TYPE_POINT)
	{// �|�C���g���C�g��������
		pLight = CPointLight::Create(Light.Position, Light.Diffuse, Light.Ambient, Light.Specular,
			Light.Attenuation0, Light.Attenuation1, Light.Attenuation2, Light.Range);
	}
	else if (pLightCpy->GetType() == CLight::TYPE_SPOT)
	{// �X�|�b�g���C�g��������
		pLight = CSpotLight::Create(Light.Position, Light.Direction, Light.Diffuse, Light.Ambient, Light.Specular,
			Light.Attenuation0, Light.Attenuation1, Light.Attenuation2, Light.Range, Light.Falloff,Light.Theta,Light.Phi);
	}

	return pLight;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CLightManager::Init(int nNumLight)
{
	// ���C�g�̐���ݒ�
	m_nNumLight = nNumLight;

	// ���C�g�N���X�̃|�C���^���m�ۂ���
	if (m_apLight == NULL && m_nNumLight > 0)
	{// ���������m�ۂł����Ԃł���
		m_apLight = new CLight*[nNumLight];
		if (m_apLight != NULL)
		{// ���������m�ۂł���
			for (int nCntLight = 0; nCntLight < m_nNumLight; nCntLight++)
			{// ���C�g�̐������J��Ԃ�
				m_apLight[nCntLight] = NULL;
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CLightManager::Uninit(void)
{
	if (m_apLight != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntLight = 0; nCntLight < m_nNumLight; nCntLight++)
		{// ���C�g�̐������J��Ԃ�
			if (m_apLight[nCntLight] != NULL)
			{// ���������m�ۂ���Ă���
				m_apLight[nCntLight]->Uninit();

				// �������̊J��
				delete m_apLight[nCntLight];
				m_apLight[nCntLight] = NULL;
			}
		}

		// �������̊J��
		delete m_apLight;
		m_apLight = NULL;
	}
}

//=============================================================================
//    �X�V����
//=============================================================================
void CLightManager::Update(void)
{
	if (m_apLight != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntLight = 0; nCntLight < m_nNumLight; nCntLight++)
		{// ���C�g�̐������J��Ԃ�
			if (m_apLight[nCntLight] != NULL)
			{// ���������m�ۂ���Ă���
				m_apLight[nCntLight]->Update();
			}
		}
	}
}

//=============================================================================
//    ���C�g��ݒ肷��
//=============================================================================
void CLightManager::SettingLight(CLight *pLight, const int nIdx)
{
	if (m_apLight != NULL)
	{// ���C�g�p�̃A�h���X���m�ۂ���Ă���
		m_apLight[nIdx] = pLight;

		CRenderer *pRenderer = CManager::GetRenderer();
		if (pRenderer != NULL)
		{// �����_�����O�N���X���擾�ł���
			LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();  	// �f�o�C�X�̎擾
			if (pDevice != NULL)
			{// �f�o�C�X���擾�ł���
			    // ���C�g��ݒ肷��
				pDevice->SetLight(nIdx, &m_apLight[nIdx]->GetLight());

				// ���C�g��ON/OFF��؂�ւ�
				pDevice->LightEnable(nIdx, m_apLight[nIdx]->GetSwitch());
			}
		}
	}
}

//=============================================================================
//    ���C�g��ON/OFF��؂�ւ��邷��
//=============================================================================
void CLightManager::SwitchLight(const int nIdx)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();  	// �f�o�C�X�̎擾
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // ���C�g��ON/OFF��؂�ւ�
			bool bSwitch = m_apLight[nIdx]->GetSwitch();
			bSwitch = bSwitch ? false : true;
			m_apLight[nIdx]->SetSwitch(bSwitch);
			pDevice->LightEnable(nIdx, m_apLight[nIdx]->GetSwitch());
		}
	}
}

//=============================================================================
//    ���C�g�̐���ݒ肷��
//=============================================================================
void CLightManager::SetNumLight(const int nNumLight)
{
	m_nNumLight = nNumLight;
}

//=============================================================================
//    ���C�g�̐����擾����
//=============================================================================
int CLightManager::GetNumLight(void)
{
	return m_nNumLight;
}

//=============================================================================
//    ���C�g�N���X�ւ̃|�C���^���擾����
//=============================================================================
CLight *CLightManager::GetLight(const int nIdx)
{
	return m_apLight[nIdx];
}