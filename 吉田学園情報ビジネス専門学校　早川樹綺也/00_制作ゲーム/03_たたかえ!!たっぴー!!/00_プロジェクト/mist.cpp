//=============================================================================
//
// �N���[���ɂ̏��� [mist.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "mist.h"
#include "renderer.h"
#include "manager.h"
#include "functionlib.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCreamMist::CCreamMist(int nPriority, OBJTYPE objtype) : CScene2D(nPriority, objtype)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCreamMist::~CCreamMist()
{

}

//=============================================================================
//��������
//=============================================================================
CCreamMist *CCreamMist::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority, float fRot, float fStartTexU, float fStartTexV, float fAddTexU, float fAddTexV)
{
	CCreamMist *pCreamMist = NULL;	// CCreamMist�̃|�C���^

	if (pCreamMist == NULL)
	{
		pCreamMist = new CCreamMist(nPriority);
		if (pCreamMist != NULL)
		{
			// �l�̐ݒ�
			pCreamMist->SetPos(pos);
			pCreamMist->SetCol(col);
			pCreamMist->SetWidth(fWidth);
			pCreamMist->SetHeight(fHeight);
			pCreamMist->SetRot(fRot);
			pCreamMist->SetStartTexU(fStartTexU);
			pCreamMist->SetStartTexV(fStartTexV);
			pCreamMist->SetAddTexU(fAddTexU);
			pCreamMist->SetAddTexV(fAddTexV);

			if (FAILED(pCreamMist->Init()))
			{// �������Ɏ��s
				return NULL;
			}
		}
	}
	return pCreamMist;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCreamMist::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CScene2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCreamMist::Uninit(void)
{
	// ���ʂ̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCreamMist::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CCreamMist::Draw(void)
{
	// �����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL)return;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();
	if (pDevice == NULL)return;

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���ʂ̕`�揈��
	CScene2D::Draw();

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}