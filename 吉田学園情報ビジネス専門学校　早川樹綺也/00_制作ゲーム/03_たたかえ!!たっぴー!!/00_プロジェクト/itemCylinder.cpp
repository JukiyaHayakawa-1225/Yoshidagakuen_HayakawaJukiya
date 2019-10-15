//*****************************************************************************
//
//     �A�C�e���p�̉~������[itemCylinder.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "itemCylinder.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define ITEMCYLINDER_ALPHA_DECAY (-0.012f)
#define ITEMCYLINDER_ALPHA_DECAY_CUT (1.15f)

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CItemCylinder::CItemCylinder(int nPriority, OBJTYPE objType) : CMeshCylinder(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CItemCylinder::~CItemCylinder()
{

}

//=============================================================================
//    ��������
//=============================================================================
CItemCylinder *CItemCylinder::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, int nPriority)
{
	CItemCylinder *pItemCylinder = NULL;   // �A�C�e���p�~���N���X�^�̃|�C���^
	if (pItemCylinder == NULL)
	{// ����������ɂȂ��Ă���
		pItemCylinder = new CItemCylinder(nPriority);
		if (pItemCylinder != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pItemCylinder->Init(pos, rot, col, fHeight, fRadius, nXBlock, nYBlock)))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}

	return pItemCylinder;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CItemCylinder::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock)
{
	// �e��l�̑��
	SetPos(pos);            // ���b�V���V�����_�[�̍��W
	SetRot(rot);            // ���b�V���V�����_�[�̌���
	SetCol(col);            // ���b�V���V�����_�[�̐F
	SetHeight(fHeight);     // ���b�V���V�����_�[�̍���
	SetRadius(fRadius);     // ���b�V���V�����_�[�̔��a
	SetXBlock(nXBlock);     // ���̕�����
	SetYBlock(nYBlock);     // �c�̕�����
	m_State = STATE_NONE;   // ��Ԃ�������

	// ���ʂ̏���������
	if (FAILED(CMeshCylinder::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CItemCylinder::Uninit(void)
{
	// ���ʂ̏I������
	CMeshCylinder::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CItemCylinder::Update(void)
{
	switch (m_State)
	{// ��Ԃɂ���ď����킯
	case STATE_NONE:
		Normal();
		break;
	case STATE_DEATH:
		Death();
		break;
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CItemCylinder::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			DWORD Lighting;
			DWORD Culling;

			// �J�����O�����Ȃ��ݒ��
			pDevice->GetRenderState(D3DRS_CULLMODE, &Culling);
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			// ���u�����f�B���O�����Z�����ɐݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			// ���C�e�B���O�̐ݒ�
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, false);

			// ���ʂ̕`�揈��
			CMeshCylinder::Draw();

			// �J�����O�̐ݒ�����ɖ߂�
			pDevice->SetRenderState(D3DRS_CULLMODE, Culling);

			// ���C�e�B���O��߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);

			// ���u�����f�B���O�����ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CItemCylinder::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * GetNumVertex(),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = 0;   // �p�x
	float fRadius = 0;  // ���a
	D3DXVECTOR3 pos;    // ���W
	D3DXCOLOR col;      // �F
	float Utex;         // �e�N�X�`��U���W
	float Vtex;         // �e�N�X�`��V���W

	fAngle = 0.0f;                          // �p�x��0��
	fRadius = GetRadius();                  // ���a��ݒ�
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ���W��ݒ�
	col = GetCol();                         // �F�̐ݒ�
	Utex = 0.0f;                            // �e�N�X�`��U���W��ݒ�
	Vtex = 0.0f;                            // �e�N�X�`��V���W��ݒ�

	for (int nCntV = 0; nCntV < GetYBlock() + 1; nCntV++)
	{// ���������̕����������J��Ԃ�
		for (int nCntH = 0; nCntH < GetXBlock() + 1; nCntH++)
		{// ���������̕����������J��Ԃ�
			pos.x = sinf(fAngle) * fRadius;
			pos.z = cosf(fAngle) * fRadius;

			// ���_���W
			pVtx[nCntH].pos = pos;

			// ���_�J���[
			pVtx[nCntH].col = col;

			// �e�N�X�`�����W
			pVtx[nCntH].tex = D3DXVECTOR2(Utex, Vtex);

			fAngle -= D3DX_PI / (GetXBlock() / 2.0f);  // �p�x��i�߂�
			Utex += 1.0f / (GetXBlock() / 2.0f);       // �e�N�X�`��U���W��i�߂�

			if (fAngle <= -D3DX_PI)
			{// �p�x���~�����𒴂���
				fAngle += D3DX_PI * 2.0f;
			}
		}
		pos.y += GetHeight() / (GetYBlock() + 1);  // ���W���グ��
		fAngle = 0.0f;                             // �p�x��0�ɖ߂�
		Utex = 0.0f;                               // �e�N�X�`��U���W��߂�
		Vtex -= 1.0f / GetYBlock();                // �e�N�X�`��V���W��i�߂�
		pVtx += GetXBlock() + 1;                   // ���_�o�b�t�@�̔ԍ������̕��������i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// ���_�o�b�t�@��ݒ肷��
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    ��Ԃ̐ݒ�
//=============================================================================
void CItemCylinder::SetState(const STATE state)
{
	m_State = state;
}

//=============================================================================
//    ��Ԃ̎擾
//=============================================================================
CItemCylinder::STATE CItemCylinder::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �ʏ��Ԃ̏���
//=============================================================================
void CItemCylinder::Normal(void)
{

}

//=============================================================================
//    ���S��Ԃ̏���
//=============================================================================
void CItemCylinder::Death(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff == NULL) { return; }
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̓����x���グ��
	D3DXCOLOR col;
	float fAlphaDecay = ITEMCYLINDER_ALPHA_DECAY;
	bool bDeath = true;
	for (int nCntV = 0; nCntV < GetYBlock() + 1; nCntV++)
	{// ���������̕����������J��Ԃ�
		for (int nCntH = 0; nCntH < GetXBlock() + 1; nCntH++)
		{// ���������̕����������J��Ԃ�
			// ���_�J���[���擾
			col = pVtx[nCntH].col;

			// �����x���グ��
			col.a += fAlphaDecay;
			if (col.a > 0.0f)
			{
				bDeath = false;
			}
			else
			{
				col.a = 0.0f;
			}

			// ���_�J���[�̐ݒ�
			pVtx[nCntH].col = col;
		}
		// �����x��������ʂ����炷
		fAlphaDecay *= ITEMCYLINDER_ALPHA_DECAY_CUT;
		pVtx += GetXBlock() + 1;                   // ���_�o�b�t�@�̔ԍ������̕��������i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// �S�Ă̒��_�������ɂȂ���
	if (bDeath == true)
	{
		Uninit();
	}
}