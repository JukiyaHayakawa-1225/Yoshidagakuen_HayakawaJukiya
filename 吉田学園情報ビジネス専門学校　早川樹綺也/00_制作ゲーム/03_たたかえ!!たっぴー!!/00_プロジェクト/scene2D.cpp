//=============================================================================
//
// �I�u�W�F�N�g2D���� [scene2D.cpp]
// Author : Jukiya Hayakawa & Hodaka Niwa
//
//=============================================================================
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "functionlib.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================


//*****************************************************************************
//    CScene2D�̏���
//*****************************************************************************
//=============================================================================
// �I�u�W�F�N�g�̃R���X�g���N�^
//=============================================================================
CScene2D::CScene2D(int nPriority,OBJTYPE objtype) : CScene(nPriority, objtype)
{
	m_pTexture = NULL;					        // �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;					        // ���_�o�b�t�@�ւ̃|�C���^
	m_pos = INITIALIZE_D3DXVECTOR3;		        // �|���S���̈ʒu
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �|���S���̐F
	m_fRot = 0.0f;							    // �|���S���̌���
	m_fWidth = 0.0f;					        // �|���S���̕�
	m_fHeight = 0.0f;					        // �|���S���̍���
	m_fAngle = 0.0f;					        // �x�N�g���̊p�x
	m_fLength = 0.0f;					        // �x�N�g���̒���
	m_fStartTexU = 0.0f;                        // �e�N�X�`��U���W(�n�_)
	m_fStartTexV = 0.0f;                        // �e�N�X�`��V���W(�n�_)
	m_fAddTexU = 0.0f;                          // �e�N�X�`��U���W�ɉ�����l
	m_fAddTexV = 0.0f;                          // �e�N�X�`��V���W�ɉ�����l

}
//=============================================================================
// �I�u�W�F�N�g�̃f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
//�I�u�W�F�N�g�̐���
//=============================================================================
CScene2D *CScene2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority, float fRot, float fStartTexU, float fStartTexV, float fAddTexU, float fAddTexV)
{
	CScene2D *pScene2D = NULL;	// CScene2D�̃|�C���^

	if (pScene2D == NULL)
	{
		pScene2D = new CScene2D(nPriority);
		if (pScene2D != NULL)
		{
			// �l�̐ݒ�
			pScene2D->SetPos(pos);
			pScene2D->SetCol(col);
			pScene2D->SetWidth(fWidth);
			pScene2D->SetHeight(fHeight);
			pScene2D->SetRot(fRot);
			pScene2D->SetStartTexU(fStartTexU);
			pScene2D->SetStartTexV(fStartTexV);
			pScene2D->SetAddTexU(fAddTexU);
			pScene2D->SetAddTexV(fAddTexV);

			if (FAILED(pScene2D->Init()))
			{// �������Ɏ��s
				return NULL;
			}
		}
	}
	return pScene2D;
}

//=============================================================================
// �I�u�W�F�N�g�̏���������
//=============================================================================
HRESULT CScene2D::Init(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{
			// ���_�o�b�t�@�̐���
			MakeVertex(pDevice);
		}
	}

	return S_OK;
}

//=============================================================================
// �I�u�W�F�N�g�̏I������
//=============================================================================
void CScene2D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �I�u�W�F�N�g�̍X�V����
//=============================================================================
void CScene2D::Update(void)
{

}

//=============================================================================
// �I�u�W�F�N�g�̕`�揈��
//=============================================================================
void CScene2D::Draw(void)
{
	//�����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// ���_�o�b�t�@�̐�������
//=============================================================================
void CScene2D::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// �|���S���̑傫���ݒ�
	SetScale(m_fWidth, m_fHeight);

	// ���_�o�b�t�@�֒l��ݒ肷�鏈��
	SetVtxBuffValue();
}

//=============================================================================
// �|���S���̑傫���ݒ菈��
//=============================================================================
void CScene2D::SetScale(const float fWidth, const float fHeight)
{
	// ���A�����̒l�ݒ�
	SetWidth(fWidth);
	SetHeight(fHeight);

	// �x�N�g���̒����Ɗp�x���v�Z
	m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));
	m_fAngle = atan2f(m_fWidth, -m_fHeight);
}

//=============================================================================
// ���_�o�b�t�@�֒l��ݒ肷�鏈��
//=============================================================================
void CScene2D::SetVtxBuffValue(void)
{
	// ���_���
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̉�]���l�����č��W���v�Z
	float XPos[4];
	float YPos[4];
	XPos[0] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[0] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[1] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[1] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[2] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[2] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	XPos[3] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[3] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);

	// ���_���W
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// ���_�̐������J��Ԃ�
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + m_pos;
	}

	// ���_�e�N�X�`��
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(m_fStartTexU, m_fStartTexV);
	pVtx[1].tex = D3DXVECTOR2(m_fStartTexU + m_fAddTexU, m_fStartTexV);
	pVtx[2].tex = D3DXVECTOR2(m_fStartTexU, m_fStartTexV + m_fAddTexV);
	pVtx[3].tex = D3DXVECTOR2(m_fStartTexU + m_fAddTexU, m_fStartTexV + m_fAddTexV);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���_�o�b�t�@�֍��W��ݒ肷�鏈��
//=============================================================================
void CScene2D::SetVtxBuffPos(void)
{
	// ���_���
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̉�]���l�����č��W���v�Z
	float XPos[4];
	float YPos[4];
	XPos[0] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[0] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[1] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[1] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[2] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[2] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	XPos[3] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[3] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);

	// ���_���W
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// ���_�̐������J��Ԃ�
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + m_pos;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���_�o�b�t�@�֐F��ݒ肷�鏈��
//=============================================================================
void CScene2D::SetVtxBuffCol(void)
{
	// ���_���
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���_�o�b�t�@�փe�N�X�`�����W��ݒ肷�鏈��
//=============================================================================
void CScene2D::SetVtxBuffTex(void)
{
	// ���_���
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(m_fStartTexU, m_fStartTexV);
	pVtx[1].tex = D3DXVECTOR2(m_fStartTexU + m_fAddTexU, m_fStartTexV);
	pVtx[2].tex = D3DXVECTOR2(m_fStartTexU, m_fStartTexV + m_fAddTexV);
	pVtx[3].tex = D3DXVECTOR2(m_fStartTexU + m_fAddTexU, m_fStartTexV + m_fAddTexV);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �e�N�X�`���̊��蓖�ď���
//=============================================================================
void CScene2D::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// ���_�o�b�t�@�̐ݒ菈��
//=============================================================================
void CScene2D::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
// �|���S���̈ʒu�ݒ菈��
//=============================================================================
void CScene2D::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// �|���S���̐F�ݒ菈��
//=============================================================================
void CScene2D::SetCol(const D3DXCOLOR col)
{
	m_col = col;
}

//=============================================================================
// �|���S���̕��ݒ菈��
//=============================================================================
void CScene2D::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
// �|���S���̍����ݒ菈��
//=============================================================================
void CScene2D::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
// �|���S���̌����ݒ菈��
//=============================================================================
void CScene2D::SetRot(const float fRot)
{
	m_fRot = fRot;
}

//=============================================================================
// �x�N�g���̊p�x�ݒ菈��
//=============================================================================
void CScene2D::SetAngle(const float fAngle)
{
	m_fAngle = fAngle;
}

//=============================================================================
// �x�N�g���̒����ݒ菈��
//=============================================================================
void CScene2D::SetLength(const float fLength)
{
	m_fLength = fLength;
}

//=============================================================================
// �e�N�X�`��U���W(�n�_)�ݒ菈��
//=============================================================================
void CScene2D::SetStartTexU(const float fStartTexU)
{
	m_fStartTexU = fStartTexU;
}

//=============================================================================
// �e�N�X�`��V���W(�n�_)�ݒ菈��
//=============================================================================
void CScene2D::SetStartTexV(const float fStartTexV)
{
	m_fStartTexV = fStartTexV;
}

//=============================================================================
// �e�N�X�`��U���W�ɉ�����l�ݒ菈��
//=============================================================================
void CScene2D::SetAddTexU(const float fAddTexU)
{
	m_fAddTexU = fAddTexU;
}

//=============================================================================
// �e�N�X�`��V���W�ɉ�����l�ݒ菈��
//=============================================================================
void CScene2D::SetAddTexV(const float fAddTexV)
{
	m_fAddTexV = fAddTexV;
}

//=============================================================================
// �e�N�X�`���ւ̃|�C���^�擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CScene2D::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
// ���_�o�b�t�@�ւ̃|�C���^�擾����
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CScene2D::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
// �|���S���̈ʒu�擾����
//=============================================================================
D3DXVECTOR3 CScene2D::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// �|���S���̐F�擾����
//=============================================================================
D3DXCOLOR CScene2D::GetCol(void)
{
	return m_col;
}

//=============================================================================
// �|���S���̕��擾����
//=============================================================================
float CScene2D::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
// �|���S���̍����擾����
//=============================================================================
float CScene2D::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
// �|���S���̌����擾����
//=============================================================================
float CScene2D::GetRot(void)
{
	return m_fRot;
}

//=============================================================================
// �x�N�g���̊p�x�擾����
//=============================================================================
float CScene2D::GetAngle(void)
{
	return m_fAngle;
}

//=============================================================================
// �x�N�g���̒����擾����
//=============================================================================
float CScene2D::GetLength(void)
{
	return m_fLength;
}

//=============================================================================
// �e�N�X�`��U���W(�n�_)�擾����
//=============================================================================
float CScene2D::GetStartTexU(void)
{
	return m_fStartTexU;
}

//=============================================================================
// �e�N�X�`��V���W(�n�_)�擾����
//=============================================================================
float CScene2D::GetStartTexV(void)
{
	return m_fStartTexV;
}

//=============================================================================
// �e�N�X�`��U���W�ɉ�����l�擾����
//=============================================================================
float CScene2D::GetAddTexU(void)
{
	return m_fAddTexU;
}

//=============================================================================
// �e�N�X�`��V���W�ɉ�����l�擾����
//=============================================================================
float CScene2D::GetAddTexV(void)
{
	return m_fAddTexV;
}

//*****************************************************************************
//    CScene2DFlash�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CScene2DFlash::CScene2DFlash(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	// �e��l�̃N���A
	m_State = STATE_NONE;                           // ���
	m_ColMax = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // �F�̍ő�l
	m_ColMin = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // �F�̍ŏ��l
	m_fColChange = 0.0f;                            // �F�̕ω���
	m_nPushTiming = 0;                              // �`���؂�ւ���^�C�~���O(�����ꂽ��Ԃ̎�)
	m_nCounter = 0;                                 // �`����Ǘ�����J�E���^�[
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CScene2DFlash::~CScene2DFlash()
{

}

//=============================================================================
//    ��������
//=============================================================================
CScene2DFlash *CScene2DFlash::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot,
	STATE State, D3DXCOLOR ColMax, D3DXCOLOR ColMin, float fColChange, int nPushTiming, int nPriority)
{
	CScene2DFlash *pScene2DFlash = NULL;   // 2D�|���S���_�ŃN���X�^�̃|�C���^
	if (pScene2DFlash == NULL)
	{// ����������ɂȂ��Ă���
		pScene2DFlash = new CScene2DFlash(nPriority);
		if (pScene2DFlash != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pScene2DFlash->Init(pos, col, fWidth, fHeight, fRot, State, ColMax, ColMin, fColChange, nPushTiming)))
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

	return pScene2DFlash;  // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CScene2DFlash::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot,
	STATE State, D3DXCOLOR ColMax, D3DXCOLOR ColMin, float fColChange, int nPushTiming)
{
	// �e��l�̐ݒ�
	SetPos(pos);                  // �|���S���̍��W
	SetRot(fRot);                 // �|���S���̌���
	SetCol(col);                  // �|���S���̐F
	SetWidth(fWidth);             // �|���S���̕�
	SetHeight(fHeight);           // �|���S���̍���
	m_State = State;              // ���
	m_ColMax = ColMax;            // �F�̍ő�l
	m_ColMin = ColMin;            // �F�̍ŏ��l
	m_fColChange = fColChange;    // �F�̕ω���
	m_nPushTiming = nPushTiming;  // �`���؂�ւ���^�C�~���O(�����ꂽ��Ԃ̎�)
	SetStartTexU(1.0f);           // ����̃e�N�X�`��U���W
	SetStartTexV(1.0f);           // ����̃e�N�X�`��V���W
	SetAddTexU(1.0f);             // �e�N�X�`��U���W�ɉ�����l
	SetAddTexV(1.0f);             // �e�N�X�`��V���W�ɉ�����l

	if (FAILED(CScene2D::Init()))
	{// �����������Ɏ��s����
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CScene2DFlash::Uninit(void)
{
	// ���ʂ̏I������
	CScene2D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CScene2DFlash::Update(void)
{
	switch (m_State)
	{// ��Ԃɂ���ď����킯
	case STATE_NONE:   // �ʏ���
		break;
	case STATE_SELECT: // �I�����ꂽ���
		Select();
		break;
	case STATE_PUSH:   // �����ꂽ���
		m_nCounter++;  // �J�E���^�[�𑝂₷
		break;
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CScene2DFlash::Draw(void)
{
	if (m_State != STATE_PUSH)
	{// �����ꂽ��ԂłȂ�
	 // ���ʂ̕`�揈��
		CScene2D::Draw();
	}
	else
	{// �����ꂽ��Ԃł���
		if (m_nCounter % m_nPushTiming == 0)
		{// �J�E���^�[������̒l�ɂȂ���
		 // ���ʂ̕`�揈��
			CScene2D::Draw();
		}
	}
}

//=============================================================================
//    �I������Ă���Ƃ��̏���
//=============================================================================
void CScene2DFlash::Select(void)
{
	D3DXCOLOR col = GetCol();  // �F���擾

	// �F��ω�������
	col.r += m_fColChange;  // �ԐF
	col.g += m_fColChange;  // �ΐF
	col.b += m_fColChange;  // �F

	if (col.r < 0.3f || col.r > 1.0f
		|| col.g < 0.3f || col.g > 1.0f
		|| col.b < 0.3f || col.b > 1.0f)
	{// �F������̒l�𒴂���
		m_fColChange *= -1;  // �F�̕ω����t�]������
	}

	// �F�̐ݒ�
	SetCol(col);
	SetVtxBuffCol();
}

//=============================================================================
//    ��Ԃ�ݒ肷�鏈��
//=============================================================================
void CScene2DFlash::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    �F�̍ő�l��ݒ肷�鏈��
//=============================================================================
void CScene2DFlash::SetColMax(const D3DXCOLOR ColMax)
{
	m_ColMax = ColMax;
}

//=============================================================================
//    �F�̍ŏ��l��ݒ肷�鏈��
//=============================================================================
void CScene2DFlash::SetColMin(const D3DXCOLOR ColMin)
{
	m_ColMin = ColMin;
}

//=============================================================================
//    �F�̕ω��ʂ�ݒ肷�鏈��
//=============================================================================
void CScene2DFlash::SetColChange(const float fColChange)
{
	m_fColChange = fColChange;
}

//=============================================================================
//    ��Ԃ��擾���鏈��
//=============================================================================
CScene2DFlash::STATE CScene2DFlash::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �F�̍ő�l���擾���鏈��
//=============================================================================
D3DXCOLOR CScene2DFlash::GetColMax(void)
{
	return m_ColMax;
}

//=============================================================================
//    �F�̍ŏ��l���擾���鏈��
//=============================================================================
D3DXCOLOR CScene2DFlash::GetColMin(void)
{
	return m_ColMin;
}

//=============================================================================
//    �F�̕ω��ʂ��擾���鏈��
//=============================================================================
float CScene2DFlash::GetColChange(void)
{
	return m_fColChange;
}


//*****************************************************************************
//    CScene2DPress�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CScene2DPress::CScene2DPress(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	// �e��l�̃N���A
	m_State = STATE_NONE;   // ���
	m_nStopTiming = 0;      // �`����ꎞ��~����^�C�~���O
	m_nAppearTiming = 0;    // �`����ĊJ����^�C�~���O
	m_nCounter = 0;         // �`����Ǘ�����J�E���^�[
	m_nPushTiming = 0;      // �`���؂�ւ���^�C�~���O(�����ꂽ��Ԃ̎�)
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CScene2DPress::~CScene2DPress()
{

}

//=============================================================================
//    ��������
//=============================================================================
CScene2DPress *CScene2DPress::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot,
	int nStopTiming, int nAppearTiming, int nPushTiming, int nPriority)
{
	CScene2DPress *pScene2DPress = NULL;   // 2D�|���S���v���X�N���X�^�̃|�C���^
	if (pScene2DPress == NULL)
	{// ����������ɂȂ��Ă���
		pScene2DPress = new CScene2DPress(nPriority);
		if (pScene2DPress != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pScene2DPress->Init(pos, col, fWidth, fHeight, fRot, nStopTiming, nAppearTiming, nPushTiming)))
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

	return pScene2DPress;  // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CScene2DPress::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot,
	int nStopTiming, int nAppearTiming, int nPushTiming)
{
	// �e��l�̐ݒ�
	SetPos(pos);                      // �|���S���̍��W
	SetRot(fRot);                     // �|���S���̌���
	SetCol(col);                      // �|���S���̐F
	SetWidth(fWidth);                 // �|���S���̕�
	SetHeight(fHeight);               // �|���S���̍���
	m_nStopTiming = nStopTiming;      // �`����ꎞ��~����^�C�~���O
	m_nAppearTiming = nAppearTiming;  // �`����ĊJ����^�C�~���O
	m_nPushTiming = nPushTiming;      // �`���؂�ւ���^�C�~���O(�����ꂽ��Ԃ̎�)
	SetStartTexU(1.0f);               // ����̃e�N�X�`��U���W
	SetStartTexV(1.0f);               // ����̃e�N�X�`��V���W
	SetAddTexU(1.0f);                 // �e�N�X�`��U���W�ɉ�����l
	SetAddTexV(1.0f);                 // �e�N�X�`��V���W�ɉ�����l

	if (FAILED(CScene2D::Init()))
	{// �����������Ɏ��s����
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CScene2DPress::Uninit(void)
{
	// ���ʂ̏I������
	CScene2D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CScene2DPress::Update(void)
{
	m_nCounter++;  // �J�E���^�[�𑝂₷
	switch (m_State)
	{// ��Ԃɂ���ď����킯
	case STATE_NONE:   // �ʏ���
		None();
		break;
	case STATE_PUSH:   // �����ꂽ���
		break;
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CScene2DPress::Draw(void)
{
	if (m_State != STATE_PUSH)
	{// �����ꂽ��ԂłȂ�
		if (m_nCounter / m_nStopTiming == 0)
		{// �J�E���^�[������̒l��菬����
		    // ���ʂ̕`�揈��
			CScene2D::Draw();
		}
	}
	else
	{// �����ꂽ��Ԃł���
		if (m_nCounter % m_nPushTiming == 0)
		{// �J�E���^�[������̒l�ɂȂ���
		 // ���ʂ̕`�揈��
			CScene2D::Draw();
		}
	}
}

//=============================================================================
//    �ʏ��Ԃ̍X�V����
//=============================================================================
void CScene2DPress::FlashStart(void)
{
	m_State = STATE_PUSH;  // �����ꂽ��Ԃ�
	m_nCounter = 0;        // �J�E���^�[��߂��Ă���
}

//=============================================================================
//    �ʏ��Ԃ̍X�V����
//=============================================================================
void CScene2DPress::None(void)
{
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();   // �L�[�{�[�h�̎擾

	if (m_nCounter >= m_nAppearTiming)
	{// �J�E���^�[������̒l�ɒB����
		m_nCounter = 0;   // �J�E���^�[��߂�
	}
}

//=============================================================================
//    ��Ԃ�ݒ肷�鏈��
//=============================================================================
void CScene2DPress::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    �`����ĊJ����^�C�~���O��ݒ肷�鏈��
//=============================================================================
void CScene2DPress::SetAppearTiming(const int nAppearTiming)
{
	m_nAppearTiming = nAppearTiming;
}

//=============================================================================
//    �`����ꎞ��~����^�C�~���O��ݒ肷�鏈��
//=============================================================================
void CScene2DPress::SetStopTiming(const int nStopTiming)
{
	m_nStopTiming = nStopTiming;
}

//=============================================================================
//    �`����Ǘ�����J�E���^�[��ݒ肷�鏈��
//=============================================================================
void CScene2DPress::SetCounter(const int nCounter)
{
	m_nCounter = nCounter;
}

//=============================================================================
//    ��Ԃ��擾���鏈��
//=============================================================================
CScene2DPress::STATE CScene2DPress::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �`����ĊJ����^�C�~���O���擾���鏈��
//=============================================================================
int CScene2DPress::GetAppearTiming(void)
{
	return m_nAppearTiming;
}

//=============================================================================
//    �`����ꎞ��~����^�C�~���O���擾���鏈��
//=============================================================================
int CScene2DPress::GetStopTiming(void)
{
	return m_nStopTiming;
}

//=============================================================================
//    �`����Ǘ�����J�E���^�[���擾���鏈��
//=============================================================================
int CScene2DPress::GetCounter(void)
{
	return m_nCounter;
}