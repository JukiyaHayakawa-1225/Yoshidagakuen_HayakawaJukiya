//*****************************************************************************
//
//     �����̏���[number.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "number.h"
#include "functionlib.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************

//*****************************************************************************
//    CNumberPolygon�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CNumberPolygon::CNumberPolygon(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	// �e��l�̃N���A
	m_State = STATE_NONE;  // ���
	m_nNumber = 0;         // �\�����鐔��
	m_nCounter = 0;        // �`����Ǘ�����J�E���^�[
	m_nFlashTiming = 0;    // �_�ł�����^�C�~���O
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CNumberPolygon::~CNumberPolygon()
{

}

//=============================================================================
//    ��������
//=============================================================================
CNumberPolygon *CNumberPolygon::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, STATE State,
	int nNumber, int nFlashTiming, int nPriority)
{
	CNumberPolygon *pNumberPolygon = NULL;  // �����|���S���N���X�^�̃|�C���^
	if (pNumberPolygon == NULL)
	{// ����������ɂȂ��Ă���
		pNumberPolygon = new CNumberPolygon(nPriority);
		if (pNumberPolygon != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pNumberPolygon->Init(pos, col, fWidth, fHeight, fRot, State, nNumber, nFlashTiming)))
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

	return pNumberPolygon;  // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CNumberPolygon::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, STATE State,
	int nNumber, int nFlashTiming)
{
	// �e��l�̐ݒ�
	SetPos(pos);                    // �|���S���̍��W
	SetRot(fRot);                   // �|���S���̌���
	SetCol(col);                    // �|���S���̐F
	SetWidth(fWidth);               // �|���S���̕�
	SetHeight(fHeight);             // �|���S���̍���
	m_nNumber = nNumber;            // ����
	m_State = State;                // ���
	m_nFlashTiming = nFlashTiming;  // �_�ł�����^�C�~���O

	if (FAILED(CScene2D::Init()))
	{// �����������Ɏ��s����
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CNumberPolygon::Uninit(void)
{
	// ���ʂ̏I������
	CScene2D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CNumberPolygon::Update(void)
{
	if (m_State == STATE_FLASH)
	{// �_�ł������Ԃ�������
		Flash();
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CNumberPolygon::Draw(void)
{
	// ���ʂ̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
//    �_�ł������Ԃ̎��̍X�V����
//=============================================================================
void CNumberPolygon::Flash(void)
{
	m_nCounter++;   // �J�E���^�[��i�߂�
	if (m_nCounter / m_nFlashTiming == 0)
	{
		SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (m_nCounter >= m_nFlashTiming * 2)
	{// �J�E���^�[������̒l�ɒB����
		m_nCounter = 0;
	}
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CNumberPolygon::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// ���_���
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �I�t�Z�b�g�̒��������߂�
	float fLength = sqrtf((GetWidth() * GetWidth()) + (GetHeight() * GetHeight()));
	SetLength(fLength);

	// �I�t�Z�b�g�̊p�x�����߂�
	float fAngle = atan2f(GetWidth(), -GetHeight());
	SetAngle(fAngle);

	// ���_�̉�]���l�����č��W���v�Z
	float XPos[4];
	float YPos[4];
	XPos[0] = CFunctionLib::RotationVectorX(GetRot(), -sinf(fAngle) * fLength, cosf(fAngle) * fLength);
	YPos[0] = CFunctionLib::RotationVectorY(GetRot(), -sinf(fAngle) * fLength, cosf(fAngle) * fLength);
	XPos[1] = CFunctionLib::RotationVectorX(GetRot(), sinf(fAngle) * fLength, cosf(fAngle) * fLength);
	YPos[1] = CFunctionLib::RotationVectorY(GetRot(), sinf(fAngle) * fLength, cosf(fAngle) * fLength);
	XPos[2] = CFunctionLib::RotationVectorX(GetRot(), -sinf(fAngle) * fLength, -cosf(fAngle) * fLength);
	YPos[2] = CFunctionLib::RotationVectorY(GetRot(), -sinf(fAngle) * fLength, -cosf(fAngle) * fLength);
	XPos[3] = CFunctionLib::RotationVectorX(GetRot(), sinf(fAngle) * fLength, -cosf(fAngle) * fLength);
	YPos[3] = CFunctionLib::RotationVectorY(GetRot(), sinf(fAngle) * fLength, -cosf(fAngle) * fLength);

	// ���_���W
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// ���_�̐������J��Ԃ�
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + GetPos();
	}

	// ���_�e�N�X�`��
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f + (m_nNumber * 0.1f), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + (m_nNumber * 0.1f), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (m_nNumber * 0.1f), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (m_nNumber * 0.1f), 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// ���_�o�b�t�@�̐ݒ�
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    �\�����鐔���̐ݒ菈��
//=============================================================================
void CNumberPolygon::SetNumber(const int nNumber)
{
	// �����̐ݒ�
	m_nNumber = nNumber;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@�ւ̃|�C���^���擾�ł���
	 // ���_����ݒ�
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + (nNumber * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (nNumber * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (nNumber * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (nNumber * 0.1f), 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ��Ԃ̐ݒ菈��
//=============================================================================
void CNumberPolygon::SetState(const STATE state)
{
	m_State = state;
}

//=============================================================================
//    �\�����鐔���̎擾����
//=============================================================================
int CNumberPolygon::GetNumber(void)
{
	return m_nNumber;
}

//=============================================================================
//    ��Ԃ̎擾����
//=============================================================================
CNumberPolygon::STATE CNumberPolygon::GetState(void)
{
	return m_State;
}

//*****************************************************************************
//    CNumber�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CNumber::CNumber(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	m_nNumber = 0;                                 // ���ݕ\�����Ă��鐔��
	m_nNumberDigit = 0;                            // �\�����Ă��鐔���̌���
	m_pNumberTexture = NULL;                       // �����|���S���Ɏg�p���Ă���e�N�X�`���ւ̃|�C���^
	m_pNumberPolygon = NULL;                       // �����|���S���N���X�ւ̃|�C���^
	m_NumberPos = INITIALIZE_D3DXVECTOR3;          // �����|���S���̈ʒu
	m_NumberCol = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);  // �����|���S���̐F
	m_fNumberWidth = 0.0f;                         // �����|���S���̕�
	m_fNumberHeight = 0.0f;                        // �����|���S���̍���
	m_NumberInterval = INITIALIZE_D3DXVECTOR3;     // �����|���S�����m�̊Ԋu
	m_fNumberRot = 0.0f;                           // �����|���S���̌���
	m_NumberState = CNumberPolygon::STATE_NONE;    // �����|���S���̏��
	m_nNumberFlashTiming = 0;                      // �����|���S����_�ł�����̂Ɏg��
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
//    ��������
//=============================================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight,
	D3DXVECTOR3 Interval, LPDIRECT3DTEXTURE9 pNumberTex, int nNumber,
	CNumberPolygon::STATE State, int nFlashTiming, float fRot, int nPriority)
{
	CNumber *pNumber = NULL;     // �����N���X�^�̃|�C���^
	if (pNumber == NULL)
	{// ����������ɂȂ��Ă���
		pNumber = new CNumber(nPriority);
		if (pNumber != NULL)
		{// �C���X�^���X�𐶐��ł���
			// �e��l�̐ݒ�
			pNumber->SetNumberPos(pos);
			pNumber->SetNumberCol(col);
			pNumber->SetNumberWidth(fWidth);
			pNumber->SetNumberHeight(fHeight);
			pNumber->SetNumberInterval(Interval);
			pNumber->SetNumberTexture(pNumberTex);
			pNumber->SetNumberRot(fRot);
			pNumber->SetNumberState(State);
			pNumber->SetNumberFlashTiming(nFlashTiming);
			pNumber->SetNumberValue(nNumber);

			if (FAILED(pNumber->Init()))
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

	return pNumber;  // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CNumber::Init(void)
{
	// �\�����鐔���̌������v�Z
	CalcNumberDigit();

	// �����|���S���̐���
	CreateNumberPolygon();

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CNumber::Uninit(void)
{
	// �����|���S�����J������
	ReleaseNumberPolygon(m_nNumberDigit);

	// ���S�t���O�𗧂Ă�
	CScene::Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CNumber::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CNumber::Draw(void)
{

}

//=============================================================================
//    �\�����鐔����ݒ肷�鏈��
//=============================================================================
void CNumber::SetNumber(const int nNumber)
{
	// ������ݒ�
	m_nNumber = nNumber;

	// ������ۑ�
	int nReleaseDigit = m_nNumberDigit;

	// �����̌������v�Z
	CalcNumberDigit();

	if (nReleaseDigit != m_nNumberDigit)
	{// ���ݕ\������Ă��鐔���̌����ƈقȂ�
		ReleaseNumberPolygon(nReleaseDigit);
		CreateNumberPolygon();
	}

	// �\�����鐔����ς���
	if (m_pNumberPolygon == NULL) { return; }
	int nDispNumber = 0;
	for (int nCntDigit = 0; nCntDigit < m_nNumberDigit; nCntDigit++)
	{
		if (m_pNumberPolygon[nCntDigit] != NULL)
		{
			// �\�����鐔���̌v�Z
			nDispNumber = m_nNumber % ((int)powf(10.0f, (float)nCntDigit) * 10) / (int)powf(10.0f, (float)nCntDigit);

			// �\�����鐔���̐ݒ�
			m_pNumberPolygon[nCntDigit]->SetNumber(nDispNumber);
		}
	}
}

//=============================================================================
//    �\�����鐔���|���S���̐F��ݒ肷�鏈��
//=============================================================================
void CNumber::SetNumberVtxBuffCol()
{
	if (m_pNumberPolygon == NULL) { return; }

	for (int nCntDigit = 0; nCntDigit < m_nNumberDigit; nCntDigit++)
	{
		if (m_pNumberPolygon[nCntDigit] != NULL)
		{
			m_pNumberPolygon[nCntDigit]->SetCol(m_NumberCol);
			m_pNumberPolygon[nCntDigit]->SetVtxBuffCol();
		}
	}
}

//=============================================================================
//    �\�����鐔���̌����v�Z����
//=============================================================================
void CNumber::CalcNumberDigit(void)
{
	m_nNumberDigit = (int)log10f((float)m_nNumber) + 1;	// ���������߂�
	if (m_nNumberDigit <= 0) { m_nNumberDigit = 1; }	// 0�ȉ��̂Ƃ�1�ɂ���
}

//=============================================================================
//    �����|���S���̐�������
//=============================================================================
void CNumber::CreateNumberPolygon(void)
{
	// �K�v�Ȍ��������������m��
	m_pNumberPolygon = new CNumberPolygon*[m_nNumberDigit];
	if (m_pNumberPolygon == NULL) { return; }

	// �����|���S���𐶐�����
	D3DXVECTOR3 NumberPos = m_NumberPos; // ���W
	D3DXCOLOR NumberCol = m_NumberCol;   // �F
	int nDispNumber = 0;                 // �\�����鐔���̌v�Z�p
	for (int nCntDigit = 0; nCntDigit < m_nNumberDigit; nCntDigit++)
	{
		// �\�����鐔���̌v�Z
		nDispNumber = m_nNumber % ((int)powf(10.0f, (float)nCntDigit) * 10) / (int)powf(10.0f, (float)nCntDigit);

		// �����|���S���̃C���X�^���X��������
		m_pNumberPolygon[nCntDigit] = NULL;
		m_pNumberPolygon[nCntDigit] = CNumberPolygon::Create(NumberPos, NumberCol, m_fNumberWidth, m_fNumberHeight, 0.0f, CNumberPolygon::STATE_NONE, nDispNumber, 3, GetPriority());
		if (m_pNumberPolygon[nCntDigit] != NULL)
		{
			m_pNumberPolygon[nCntDigit]->BindTexture(m_pNumberTexture);
		}

		// ���ɐi�߂�
		NumberPos.x += m_NumberInterval.x;
		NumberPos.y += m_NumberInterval.y;
	}
}

//=============================================================================
//    �����|���S���̊J������
//=============================================================================
void CNumber::ReleaseNumberPolygon(int nDigit)
{
	if (m_pNumberPolygon == NULL) { return; }

	for (int nCntDigit = 0; nCntDigit < nDigit; nCntDigit++)
	{
		if (m_pNumberPolygon[nCntDigit] != NULL)
		{
			m_pNumberPolygon[nCntDigit]->Uninit();
			m_pNumberPolygon[nCntDigit] = NULL;
		}
	}

	delete[] m_pNumberPolygon;
	m_pNumberPolygon = NULL;

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck();
}

//=============================================================================
//    �\�����鐔����ݒ肷��
//=============================================================================
void CNumber::SetNumberValue(const int nNumber)
{
	m_nNumber = nNumber;
}

//=============================================================================
//    �����|���S���Ɏg�p����e�N�X�`���ւ̃|�C���^�ݒ菈��
//=============================================================================
void CNumber::SetNumberTexture(const LPDIRECT3DTEXTURE9 pNumberTex)
{
	m_pNumberTexture = pNumberTex;
}

//=============================================================================
//    �����|���S���̌����ݒ菈��
//=============================================================================
void CNumber::SetNumberPos(const D3DXVECTOR3 NumberPos)
{
	m_NumberPos = NumberPos;
}

//=============================================================================
//    �����|���S���̌����ݒ菈��
//=============================================================================
void CNumber::SetNumberCol(const D3DXCOLOR NumberCol)
{
	m_NumberCol = NumberCol;
}

//=============================================================================
//    �����|���S���̌����ݒ菈��
//=============================================================================
void CNumber::SetNumberWidth(const float fNumberWidth)
{
	m_fNumberWidth = fNumberWidth;
}

//=============================================================================
//    �����|���S���̌����ݒ菈��
//=============================================================================
void CNumber::SetNumberHeight(const float fNumberHeight)
{
	m_fNumberHeight = fNumberHeight;
}

//=============================================================================
//    �����|���S�����m�̊Ԋu�ݒ菈��
//=============================================================================
void CNumber::SetNumberInterval(const D3DXVECTOR3 NumberInterval)
{
	m_NumberInterval = NumberInterval;
}

//=============================================================================
//    �����|���S���̌����ݒ菈��
//=============================================================================
void CNumber::SetNumberRot(const float fNumberRot)
{
	m_fNumberRot = fNumberRot;
}

//=============================================================================
//    �����|���S���̏�Ԑݒ菈��
//=============================================================================
void CNumber::SetNumberState(const CNumberPolygon::STATE state)
{
	m_NumberState = state;
}

//=============================================================================
//    �����|���S���̓_�Œl�ݒ菈��
//=============================================================================
void CNumber::SetNumberFlashTiming(const int nNumberFlashTiming)
{
	m_nNumberFlashTiming = nNumberFlashTiming;
}

//=============================================================================
//    ���ݕ\�����Ă��鐔���̎擾����
//=============================================================================
int CNumber::GetNumber(void)
{
	return m_nNumber;
}

//=============================================================================
//    �����̌����擾����
//=============================================================================
int CNumber::GetNumberDigit(void)
{
	return m_nNumberDigit;
}

//=============================================================================
//    �����|���S���N���X�ւ̃|�C���^�擾����
//=============================================================================
CNumberPolygon **CNumber::GetNumberPolygon(void)
{
	return m_pNumberPolygon;
}

//=============================================================================
//    �����|���S���Ɏg�p���Ă���e�N�X�`���ւ̃|�C���^�擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CNumber::GetNumberTexture(void)
{
	return m_pNumberTexture;
}

//=============================================================================
//    �����|���S���̈ʒu�擾����
//=============================================================================
D3DXVECTOR3 CNumber::GetNumberPos(void)
{
	return m_NumberPos;
}

//=============================================================================
//    �����|���S���̐F�擾����
//=============================================================================
D3DXCOLOR CNumber::GetNumberCol(void)
{
	return m_NumberCol;
}

//=============================================================================
//    �����|���S���̕��擾����
//=============================================================================
float CNumber::GetNumberWidth(void)
{
	return m_fNumberWidth;
}

//=============================================================================
//    �����|���S���̍����擾����
//=============================================================================
float CNumber::GetNumberHeight(void)
{
	return m_fNumberHeight;
}

//=============================================================================
//    �����|���S�����m�̊Ԋu�擾����
//=============================================================================
D3DXVECTOR3 CNumber::GetNumberInterval(void)
{
	return m_NumberInterval;
}

//=============================================================================
//    �����|���S���̌����擾����
//=============================================================================
float CNumber::GetNumberRot(void)
{
	return m_fNumberRot;
}

//=============================================================================
//    �����|���S���̏�Ԏ擾����
//=============================================================================
CNumberPolygon::STATE CNumber::GetNumberState(void)
{
	return m_NumberState;
}

//=============================================================================
//    �����|���S���̓_�Œl�擾����
//=============================================================================
int CNumber::GetNumberFlashTiming(void)
{
	return m_nNumberFlashTiming;
}