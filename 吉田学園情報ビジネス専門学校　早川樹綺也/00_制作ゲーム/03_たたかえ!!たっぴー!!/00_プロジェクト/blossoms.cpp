//=============================================================================
//
// �Ԃт�̏��� [blossoms.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "blossoms.h"
#include "renderer.h"
#include "manager.h"
#include "functionlib.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBlossoms::CBlossoms(int nPriority, OBJTYPE objtype) : CScene2D(nPriority, objtype)
{
	// �e��l�̃N���A
	m_Move = INITIALIZE_D3DXVECTOR3;
	m_fAngleSpeed = 0.0f;
	m_fRotSpeed = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBlossoms::~CBlossoms()
{

}

//=============================================================================
//��������
//=============================================================================
CBlossoms *CBlossoms::Create(D3DXVECTOR3 move, float fAngleSpeed, float fRotSpeed, D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority, float fRot, float fStartTexU, float fStartTexV, float fAddTexU, float fAddTexV)
{
	CBlossoms *pBlossoms = NULL;	// CBlossoms�̃|�C���^

	if (pBlossoms == NULL)
	{
		pBlossoms = new CBlossoms(nPriority);
		if (pBlossoms != NULL)
		{
			// �l�̐ݒ�
			pBlossoms->SetMove(move);
			pBlossoms->SetAngleSpeed(fAngleSpeed);
			pBlossoms->SetRotSpeed(fRotSpeed);
			pBlossoms->SetPos(pos);
			pBlossoms->SetCol(col);
			pBlossoms->SetWidth(fWidth);
			pBlossoms->SetHeight(fHeight);
			pBlossoms->SetRot(fRot);
			pBlossoms->SetStartTexU(fStartTexU);
			pBlossoms->SetStartTexV(fStartTexV);
			pBlossoms->SetAddTexU(fAddTexU);
			pBlossoms->SetAddTexV(fAddTexV);

			if (FAILED(pBlossoms->Init()))
			{// �������Ɏ��s
				return NULL;
			}
		}
	}
	return pBlossoms;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBlossoms::Init(void)
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
void CBlossoms::Uninit(void)
{
	// ���ʂ̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBlossoms::Update(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 pos = GetPos();
	float fAngle = GetAngle();
	float fRot = GetRot();

	// ���Z����
	pos += m_Move;
	fAngle += m_fAngleSpeed;
	fRot += m_fRotSpeed;
	if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2.0f;
	}
	if (fAngle > D3DX_PI)
	{
		fAngle -= D3DX_PI * 2.0f;
	}
	if (fRot < -D3DX_PI)
	{
		fRot += D3DX_PI * 2.0f;
	}
	if (fRot > D3DX_PI)
	{
		fRot -= D3DX_PI * 2.0f;
	}

	if (CheckOutOfScreen(pos) == true)
	{// ��ʊO�ɍs����
		Uninit();
	}
	else
	{
		// �e��l�̐ݒ�
		SetPos(pos);
		SetAngle(fAngle);
		SetRot(fRot);
		SetVtxBuffPos();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBlossoms::Draw(void)
{
	// �����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL)return;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();
	if (pDevice == NULL)return;

	// �A���t�@�e�X�g���s��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���ʂ̕`�揈��
	CScene2D::Draw();

	// �A���t�@�e�X�g���s��Ȃ��ݒ��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

//=============================================================================
// ��ʊO���菈��
//=============================================================================
bool CBlossoms::CheckOutOfScreen(D3DXVECTOR3 pos)
{
	// �e��l�̎擾
	float fWidth = GetWidth();
	float fHeight = GetHeight();

	if (pos.x + fWidth < 0.0f && pos.y - fHeight > SCREEN_HEIGHT)
	{// ��ʊO������
		return true;
	}

	return false;
}

//=============================================================================
// �ړ��ʐݒ菈��
//=============================================================================
void CBlossoms::SetMove(const D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
// �p�x����]������X�s�[�h�ݒ菈��
//=============================================================================
void CBlossoms::SetAngleSpeed(const float fAngleSpeed)
{
	m_fAngleSpeed = fAngleSpeed;
}

//=============================================================================
// ��������]������X�s�[�h�ݒ菈��
//=============================================================================
void CBlossoms::SetRotSpeed(const float fRotSpeed)
{
	m_fRotSpeed = fRotSpeed;
}

//=============================================================================
// �ړ��ʎ擾����
//=============================================================================
D3DXVECTOR3 CBlossoms::GetMove(void)
{
	return m_Move;
}

//=============================================================================
// �p�x����]������X�s�[�h�擾����
//=============================================================================
float CBlossoms::GetAngleSpeed(void)
{
	return m_fAngleSpeed;
}

//=============================================================================
// ��������]������X�s�[�h�擾����
//=============================================================================
float CBlossoms::GetRotSpeed(void)
{
	return m_fRotSpeed;
}