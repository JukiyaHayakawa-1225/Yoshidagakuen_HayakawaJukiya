//=============================================================================
//
// �����̏��� [number.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "number.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;	//�e�N�X�`���ւ̃|�C���^���

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CNumber::CNumber()
{
	m_pos = INITIALIZE_D3DXVECTOR3;				//�ʒu
	m_col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);		//�F
	m_fWidth = 0.0f;							//��
	m_fHeight = 0.0f;							//����
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
//�����̐���
//=============================================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CNumber *pNumber = NULL;	//�����̓��I�m��
	if (pNumber == NULL)
	{
		pNumber = new CNumber;				//�V�[���̓��I�m��
		if (pNumber != NULL)
		{
			pNumber->SetPos(pos);			//�ʒu�̐ݒu����
			pNumber->SetWidth(fWidth);		//���̐ݒu����
			pNumber->SetHeight(fHeight);	//�����̐ݒu����
			pNumber->Init();				//����������
		}
	}
	return pNumber;					//�l��Ԃ�
}

//=============================================================================
// �e�N�X�`���ǂݍ���
//=============================================================================
HRESULT CNumber::Load(void)
{
	//�����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME008,
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���j��
//=============================================================================
void CNumber::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CNumber::Init(void)
{
	//�����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)& pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CNumber::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CNumber::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CNumber::Draw(void)
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
// �ʒu�̐ݒu����
//=============================================================================
void CNumber::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// �F�̐ݒu����
//=============================================================================
void CNumber::SetCol(D3DXCOLOR col)
{
	m_col = col;

	// ���_�o�b�t�@�𐶐�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���̐ݒu����
//=============================================================================
void CNumber::SetWidth(float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
// �����̐ݒu����
//=============================================================================
void CNumber::SetHeight(float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
// �ʒu�̎擾����
//=============================================================================
D3DXVECTOR3 CNumber::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// �F�̎擾����
//=============================================================================
D3DXCOLOR CNumber::GetCol(void)
{
	return m_col;
}

//=============================================================================
// ���̎擾����
//=============================================================================
float CNumber::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
// �����̎擾����
//=============================================================================
float CNumber::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
// �����̐ݒu����
//=============================================================================
void CNumber::SetNumber(int nNumber)
{
	// ���_�o�b�t�@�𐶐�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * nNumber), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * nNumber), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * nNumber), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * nNumber), 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}