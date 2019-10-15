//=============================================================================
//
// �e�N�X�`���}�l�[�W���[�̏��� [textureManager.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "textureManager.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �e�N�X�`���}�l�[�W���[�̃R���X�g���N�^
//=============================================================================
CTextureManager::CTextureManager()
{
	m_pTexture = NULL;			//�e�N�X�`���̃|�C���^
	m_pFileName = NULL;	    	//�t�@�C�����̃|�C���^
	m_nNumTexture = 0;			//�e�N�X�`���̑���
}
//=============================================================================
// �e�N�X�`���}�l�[�W���[�̃R���X�g���N�^
//=============================================================================
CTextureManager::~CTextureManager()
{

}
//=============================================================================
// �e�N�X�`���}�l�[�W���[�̐���
//=============================================================================
CTextureManager *CTextureManager::Create(int nNumTexture)
{
	CTextureManager *pTextureManager = NULL;				//�e�̃|�C���^

	if (pTextureManager == NULL)
	{//NULL�̏ꍇ
		pTextureManager = new CTextureManager;				//���I�m��
		if (pTextureManager != NULL)
		{//NULL�łȂ��ꍇ
			pTextureManager->SetNumTexture(nNumTexture);	//�ʒu�̐ݒu����
			pTextureManager->Init();						//����������
		}
	}
	return pTextureManager;
}
//=============================================================================
// ����������
//=============================================================================
void CTextureManager::Init(void)
{
	if (m_pTexture == NULL)
	{
		//�e�N�X�`���̓��I�m��
		m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumTexture];
	}

	if (m_pFileName == NULL)
	{//�t�@�C�����̓��I�m��
		m_pFileName = new char*[m_nNumTexture];
		if (m_pFileName != NULL)
		{
			for (int nCntTexture = 0; nCntTexture < m_nNumTexture; nCntTexture++)
			{//������̓��I�m��
				m_pFileName[nCntTexture] = new char[256];
			}
		}
	}
}

//=============================================================================
// �I������
//=============================================================================
void CTextureManager::Uninit(void)
{
	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		for (int nCntTexture = 0; nCntTexture < m_nNumTexture; nCntTexture++)
		{
			if (m_pTexture[nCntTexture] != NULL)
			{
				m_pTexture[nCntTexture]->Release();
				m_pTexture[nCntTexture] = NULL;
			}
		}
		delete[] m_pTexture;
		m_pTexture = NULL;
	}

	//�t�@�C�����̔j��
	if (m_pFileName != NULL)
	{
		for (int nCntTexture = 0; nCntTexture < m_nNumTexture; nCntTexture++)
		{//�e�N�X�`���̑������J��Ԃ�
			delete[] m_pFileName[nCntTexture];
			m_pFileName[nCntTexture] = NULL;
		}
		delete[] m_pFileName;
		m_pFileName = NULL;
	}
}

//=============================================================================
// �e�N�X�`���̐ݒu����
//=============================================================================
void CTextureManager::SetTexture(LPDIRECT3DTEXTURE9 pTexture, int nIdx)
{
	m_pTexture[nIdx] = pTexture;
}

//=============================================================================
// �e�N�X�`���̑����ݒu����
//=============================================================================
void CTextureManager::SetNumTexture(int nNumTexture)
{
	m_nNumTexture = nNumTexture;
}

//=============================================================================
// �t�@�C�����̐ݒu����
//=============================================================================
void CTextureManager::SetFileName(char *pFileName,int nIdx)
{
	strcpy(m_pFileName[nIdx], pFileName);
}

//=============================================================================
// �t�@�C�����̎擾����
//=============================================================================
char *CTextureManager::GetFileName(int nIdx)
{
	return m_pFileName[nIdx];
}

//=============================================================================
// �e�N�X�`���̑����擾����
//=============================================================================
int CTextureManager::GetNumTexture(void)
{
	return m_nNumTexture;
}

//=============================================================================
// �e�N�X�`���̎擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CTextureManager::GetTexture(int nIdx)
{
	return m_pTexture[nIdx];
}