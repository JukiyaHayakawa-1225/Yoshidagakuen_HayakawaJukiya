//=============================================================================
//
// �����_�����O���� [renderer.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "renderer.h"
#include "scene.h"
#include "debugproc.h"
#include "fade.h"
#include "basemode.h"

//=============================================================================
// �����_�����O�̃R���X�g���N�^
//=============================================================================
CRenderer::CRenderer()
{
	m_pD3D = NULL;			// Direct3D�I�u�W�F�N�g
	m_pD3DDevice = NULL;	// Device�I�u�W�F�N�g(�`��ɕK�v)
	m_pRenderOrg = NULL;    // �f�t�H���g�̃����_�����O�^�[�Q�b�g�ւ̃|�C���^
	m_pBuffOrg = NULL;      // �f�t�H���g�̊e��o�b�t�@�ւ̃|�C���^
	m_pTextureMT = NULL;    // �}���`�����_�����O�Ɏg�p����e�N�X�`���ւ̃|�C���^
	m_pRenderMT = NULL;     // �}���`�����_�����O�Ɏg�p���郌���_�����O�^�[�Q�b�g�ւ̃|�C���^
	m_pBuffMT = NULL;       // �}���`�����_�����O�Ɏg�p����e��o�b�t�@�ւ̃|�C���^
}
//=============================================================================
// �����_�����O�̃f�X�g���N�^
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	// �S�Ă̊J������
	CScene::ReleaseAll();

	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
    if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ���[�N���[���N���A
	d3dpp.BackBufferCount			 = 1;							// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth			 = SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight			 = SCREEN_HEIGHT;				// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat			 = d3ddm.Format;				// �J���[���[�h�̎w��
	d3dpp.SwapEffect				 = D3DSWAPEFFECT_DISCARD;		// �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil	 = TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat	 = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed					 = bWindow;						// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp, &m_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&d3dpp, &m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF, hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&d3dpp, &m_pD3DDevice)))
			{
				// �������s
				return E_FAIL;
			}
		}
	}

	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1 , D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// ���݂̏���ۑ�
	m_pD3DDevice->GetRenderTarget(0, &m_pRenderOrg);
	m_pD3DDevice->GetDepthStencilSurface(&m_pBuffOrg);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CRenderer::Uninit(void)
{
	// �}���`�����_�����O�Ɏg�p��������J��
	ReleaseMultRenderer();

	// �����_�����O�^�[�Q�b�g�̔j��
	if (m_pRenderOrg != NULL)
	{
		m_pRenderOrg->Release();
		m_pRenderOrg = NULL;
	}

	// �e��o�b�t�@��j��
	if (m_pBuffOrg != NULL)
	{
		m_pBuffOrg->Release();
		m_pBuffOrg = NULL;
	}

	// �f�o�C�X�̔j��
	if(m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if(m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CRenderer::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CRenderer::Draw(void)
{
	CFade *pFade;
	pFade = CManager::GetFade();

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0xff), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if(SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		if (CManager::GetBaseMode() != NULL)
		{
			CManager::GetBaseMode()->Draw();
		}

		pFade->Draw();

		//�f�o�b�O�̕\��
		CDebugProc::Draw();

		// Direct3D�ɂ��`��̏I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}

//=============================================================================
// �f�o�C�X�̎擾
//=============================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}

//=============================================================================
// �����_�����O�^�[�Q�b�g�����ւ��鏈��
//=============================================================================
void CRenderer::SetRenderTarget(const LPDIRECT3DTEXTURE9 pTexture)
{
	// �e�N�X�`���̃T�[�t�F�C�X�����擾
	pTexture->GetSurfaceLevel(0, &m_pRenderMT);

	// �e�N�X�`���Ƀ����_�����O����Ƃ��Ɏg���o�b�t�@�𐶐�
	m_pD3DDevice->CreateDepthStencilSurface(SCREEN_WIDTH, SCREEN_HEIGHT,
		D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pBuffMT, NULL);

	// �e�N�X�`���̃T�[�t�F�C�X����ݒ�
	m_pD3DDevice->SetRenderTarget(0, m_pRenderMT);
	m_pD3DDevice->SetDepthStencilSurface(m_pBuffMT);

	// �T�[�t�F�C�X�����N���A����
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0x00), 1.0f, 0);
}

//=============================================================================
// �����_�����O�^�[�Q�b�g���f�t�H���g�ɖ߂�����
//=============================================================================
void CRenderer::ResetRenderTarget(void)
{
	// �}���`�����_�����O���g�p����Ă���ΊJ������
	ReleaseMultRenderer();

	m_pD3DDevice->SetRenderTarget(0, m_pRenderOrg);
	m_pD3DDevice->SetDepthStencilSurface(m_pBuffOrg);
}

//=============================================================================
// �}���`�����_�����O�Ɏg�p���Ă������̂��J������
//=============================================================================
void CRenderer::ReleaseMultRenderer(void)
{
	// �����_�����O�^�[�Q�b�g
	if (m_pRenderMT != NULL)
	{
		m_pRenderMT->Release();
		m_pRenderMT = NULL;
	}

	// �e��o�b�t�@�ނւ̃|�C���^
	if (m_pBuffMT != NULL)
	{
		m_pBuffMT->Release();
		m_pBuffMT = NULL;
	}

	// �e�N�X�`���ւ̃|�C���^����ɂ���
	m_pTextureMT = NULL;
}