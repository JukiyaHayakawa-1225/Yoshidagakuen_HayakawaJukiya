//=============================================================================
//
// ���͐}�̏��� [power_map.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "power_map.h"
#include "renderer.h"
#include "manager.h"
#include "scene2D.h"
#include "textureManager.h"
#include "UI.h"
#include "functionlib.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define POWERMAP_PRIORITY        (6)

// �v���C���[�̃A�C�R���|���S���������p
#define POWERMAP_PLAYERICON_POS_INI       (D3DXVECTOR3(169.0f, 685.0f, 0.0f))
#define POWERMAP_PLAYERICON_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define POWERMAP_PLAYERICON_WIDTH_INI     (32.0f)
#define POWERMAP_PLAYERICON_HEIGHT_INI    (32.0f)
#define POWERMAP_PLAYERICON_TEXIDX        (5)
#define POWERMAP_PLAYERICON_DESTADD       (35.5f)
#define POWERMAP_PLAYERICON_MOVEADD       (1.0f)

// �G�̃A�C�R���|���S���������p
#define POWERMAP_ENEMYICON_POS_INI        (D3DXVECTOR3(929.0f, 685.0f, 0.0f))
#define POWERMAP_ENEMYICON_COL_INI        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define POWERMAP_ENEMYICON_WIDTH_INI      (35.0f)
#define POWERMAP_ENEMYICON_HEIGHT_INI     (35.0f)
#define POWERMAP_ENEMYICON_TEXIDX         (15)

// �X�g���[�|���S���������p
#define POWERMAP_STRAW_POS_INI            (D3DXVECTOR3(SCREEN_WIDTH / 2.0f - UI_BG_WIDTH_INI - 1.0f, 690.0f, 0.0f))
#define POWERMAP_STRAW_COL_INI            (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define POWERMAP_STRAW_WIDTH_INI          (400.0f)
#define POWERMAP_STRAW_HEIGHT_INI         (35.0f)
#define POWERMAP_STRAW_TEXIDX             (32)

// �X�g���[�̒��g�|���S���������p
#define POWERMAP_STRAWCONTENTS_POS_INI    (D3DXVECTOR3(SCREEN_WIDTH / 2.0f - UI_BG_WIDTH_INI, 690.0f, 0.0f))
#define POWERMAP_STRAWCONTENTS_COL_INI    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define POWERMAP_STRAWCONTENTS_LEFT_INI   (170.0f)
#define POWERMAP_STRAWCONTENTS_HEIGHT_INI (18.0f)
#define POWERMAP_STRAWCONTENTS_TEXIDX     (33)
#define POWERMAP_STRAWCONTENTS_MOVEADDTEX (0.004f)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPowerMap::CPowerMap(int nPriority, OBJTYPE objtype) : CScene(nPriority, objtype)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPowerMap::~CPowerMap()
{

}

//=============================================================================
//��������
//=============================================================================
CPowerMap *CPowerMap::Create(CTextureManager *pTextureManager, int nPriority)
{
	CPowerMap *pPowerMap = NULL;	// CPowerMap�̃|�C���^

	if (pPowerMap == NULL)
	{
		pPowerMap = new CPowerMap(nPriority);
		if (pPowerMap != NULL)
		{
			pPowerMap->SetTextureManager(pTextureManager);
			if (FAILED(pPowerMap->Init()))
			{// �������Ɏ��s
				return NULL;
			}
		}
	}
	return pPowerMap;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPowerMap::Init(void)
{
	// �ϐ��̃N���A
	ClearVariable();

	// �X�g���[�̒��g�̐���
	CreateStrawContents();

	// �X�g���[�̐���
	CreateStraw();

	// �G�̃A�C�R���̐���
	CreateEnemyIcon();

	// �v���C���[�̃A�C�R������
	CreatePlayerIcon();

	// �v���C���[�̃A�C�R���̖ړI�n��ݒ�
	m_fPlayerIconDestPosX = POWERMAP_PLAYERICON_POS_INI.x;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPowerMap::Uninit(void)
{
	// �X�g���[�̒��g�̊J��
	ReleaseStrawContents();

	// �X�g���[�̊J��
	ReleaseStraw();

	// �G�̃A�C�R���̊J��
	ReleaseEnemyIcon();

	// �v���C���[�̃A�C�R���J��
	ReleasePlayerIcon();

	// ���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPowerMap::Update(void)
{
	// �|�C���^�`�F�b�N
	if (m_pPlayerIcon == NULL || m_pStrawContents == NULL) { return; }

	// �v���C���[�̃A�C�R�����E�ɓ�����
	D3DXVECTOR3 PlayerIconPos = m_pPlayerIcon->GetPos();
	PlayerIconPos.x += POWERMAP_PLAYERICON_MOVEADD;
	m_pPlayerIcon->SetPos(PlayerIconPos);
	m_pPlayerIcon->SetVtxBuffPos();
	if (PlayerIconPos.x >= m_fPlayerIconDestPosX)
	{
		PlayerIconPos.x = m_fPlayerIconDestPosX;
		m_pPlayerIcon->SetPos(PlayerIconPos);
		m_pPlayerIcon->SetVtxBuffPos();
		return;
	}

	// �X�g���[�̒��g�̉E�[���A�C�R���ɂ��낦��
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pStrawContents->GetVtxBuff();
	if (pVtxBuff == NULL) { return; }

	// ���_���
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos.x = POWERMAP_STRAWCONTENTS_LEFT_INI;
	pVtx[1].pos.x = PlayerIconPos.x;
	pVtx[2].pos.x = POWERMAP_STRAWCONTENTS_LEFT_INI;
	pVtx[3].pos.x = PlayerIconPos.x;

	// �e�N�X�`�����W
	pVtx[1].tex.x += POWERMAP_STRAWCONTENTS_MOVEADDTEX;
	pVtx[3].tex.x += POWERMAP_STRAWCONTENTS_MOVEADDTEX;

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPowerMap::Draw(void)
{

}

//=============================================================================
// �Q�[�W�𑝂₷����
//=============================================================================
void CPowerMap::AddGauge(void)
{
	// �v���C���[�̃A�C�R���̖ړI�n��ݒ�
	m_fPlayerIconDestPosX += POWERMAP_PLAYERICON_DESTADD;
}

//=============================================================================
// �Q�[�W�����Z�b�g���鏈��
//=============================================================================
void CPowerMap::ResetGauge(void)
{
	// �|�C���^�`�F�b�N
	if (m_pPlayerIcon == NULL || m_pStrawContents == NULL) { return; }

	// �v���C���[�̃A�C�R���������l��
	D3DXVECTOR3 PlayerIconPos = m_pPlayerIcon->GetPos();
	PlayerIconPos.x = POWERMAP_PLAYERICON_POS_INI.x;
	m_pPlayerIcon->SetPos(PlayerIconPos);
	m_pPlayerIcon->SetVtxBuffPos();

	// �X�g���[�̒��g�̉E�[���A�C�R���ɂ��낦��
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pStrawContents->GetVtxBuff();
	if (pVtxBuff == NULL) { return; }

	// ���_���
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos.x = POWERMAP_STRAWCONTENTS_LEFT_INI;
	pVtx[1].pos.x = POWERMAP_STRAWCONTENTS_LEFT_INI;
	pVtx[2].pos.x = POWERMAP_STRAWCONTENTS_LEFT_INI;
	pVtx[3].pos.x = POWERMAP_STRAWCONTENTS_LEFT_INI;

	// �e�N�X�`�����W
	pVtx[1].tex.x = 0.0f;
	pVtx[3].tex.x = 0.0f;

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// �ړI�̒l�����Z�b�g
	m_fPlayerIconDestPosX = POWERMAP_PLAYERICON_POS_INI.x;
}

//=============================================================================
// �ϐ��̃N���A����
//=============================================================================
void CPowerMap::ClearVariable(void)
{
	m_pPlayerIcon = NULL;
	m_pEnemyIcon = NULL;
	m_pStraw = NULL;
	m_pStrawContents = NULL;
	m_fPlayerIconDestPosX = 0.0f;
}

//=============================================================================
// �v���C���[�̃A�C�R����������
//=============================================================================
void CPowerMap::CreatePlayerIcon(void)
{
	if (m_pPlayerIcon != NULL) { return; }

	m_pPlayerIcon = CScene2D::Create(POWERMAP_PLAYERICON_POS_INI, POWERMAP_PLAYERICON_COL_INI,
		POWERMAP_PLAYERICON_WIDTH_INI, POWERMAP_PLAYERICON_HEIGHT_INI, POWERMAP_PRIORITY);

	// �e�N�X�`����ݒ�
	if (m_pPlayerIcon == NULL || m_pTextureManager == NULL) { return; }
	m_pPlayerIcon->BindTexture(m_pTextureManager->GetTexture(POWERMAP_PLAYERICON_TEXIDX));
}

//=============================================================================
// �G�̃A�C�R����������
//=============================================================================
void CPowerMap::CreateEnemyIcon(void)
{
	if (m_pEnemyIcon != NULL) { return; }

	m_pEnemyIcon = CScene2D::Create(POWERMAP_ENEMYICON_POS_INI, POWERMAP_ENEMYICON_COL_INI,
		POWERMAP_ENEMYICON_WIDTH_INI, POWERMAP_ENEMYICON_HEIGHT_INI, POWERMAP_PRIORITY);

	// �e�N�X�`����ݒ�
	if (m_pEnemyIcon == NULL || m_pTextureManager == NULL) { return; }
	m_pEnemyIcon->BindTexture(m_pTextureManager->GetTexture(POWERMAP_ENEMYICON_TEXIDX));
}

//=============================================================================
// �X�g���[�̐�������
//=============================================================================
void CPowerMap::CreateStraw(void)
{
	if (m_pStraw != NULL) { return; }

	m_pStraw = CScene2D::Create(POWERMAP_STRAW_POS_INI, POWERMAP_STRAW_COL_INI,
		POWERMAP_STRAW_WIDTH_INI, POWERMAP_STRAW_HEIGHT_INI, POWERMAP_PRIORITY);

	// �e�N�X�`����ݒ�
	if (m_pStraw == NULL || m_pTextureManager == NULL) { return; }
	m_pStraw->BindTexture(m_pTextureManager->GetTexture(POWERMAP_STRAW_TEXIDX));
}

//=============================================================================
// �X�g���[�̒��g��������
//=============================================================================
void CPowerMap::CreateStrawContents(void)
{
	if (m_pStrawContents != NULL) { return; }

	m_pStrawContents = CScene2D::Create(POWERMAP_STRAWCONTENTS_POS_INI, POWERMAP_STRAWCONTENTS_COL_INI,
		0.0f, POWERMAP_STRAWCONTENTS_HEIGHT_INI, POWERMAP_PRIORITY, 0.0f, 0.0f, 0.0f,
		0.0f);

	// �e�N�X�`����ݒ�
	if (m_pStrawContents == NULL || m_pTextureManager == NULL) { return; }
	m_pStrawContents->BindTexture(m_pTextureManager->GetTexture(POWERMAP_STRAWCONTENTS_TEXIDX));
}

//=============================================================================
// �v���C���[�̃A�C�R���J������
//=============================================================================
void CPowerMap::ReleasePlayerIcon(void)
{
	if (m_pPlayerIcon != NULL)
	{
		m_pPlayerIcon->Uninit();
		m_pPlayerIcon = NULL;
	}
}

//=============================================================================
// �G�̃A�C�R���̊J������
//=============================================================================
void CPowerMap::ReleaseEnemyIcon(void)
{
	if (m_pEnemyIcon != NULL)
	{
		m_pEnemyIcon->Uninit();
		m_pEnemyIcon = NULL;
	}
}

//=============================================================================
// �X�g���[�̊J������
//=============================================================================
void CPowerMap::ReleaseStraw(void)
{
	if (m_pStraw != NULL)
	{
		m_pStraw->Uninit();
		m_pStraw = NULL;
	}
}

//=============================================================================
// �X�g���[�̒��g�J������
//=============================================================================
void CPowerMap::ReleaseStrawContents(void)
{
	if (m_pStrawContents != NULL)
	{
		m_pStrawContents->Uninit();
		m_pStrawContents = NULL;
	}
}

//=============================================================================
// �v���C���[�̃A�C�R���ւ̃|�C���^���擾
//=============================================================================
void CPowerMap::SetPlayerIcon(CScene2D *pPlayerIcon)
{
	m_pPlayerIcon = pPlayerIcon;
}

//=============================================================================
// �G�̃A�C�R���ւ̃|�C���^���擾
//=============================================================================
void CPowerMap::SetEnemyIcon(CScene2D *pEnemyIcon)
{
	m_pEnemyIcon = pEnemyIcon;
}

//=============================================================================
// �X�g���[�ւ̃|�C���^���擾
//=============================================================================
void CPowerMap::SetStraw(CScene2D *pStraw)
{
	m_pStraw = pStraw;
}

//=============================================================================
// �X�g���[�̒��g�ւ̃|�C���^���擾
//=============================================================================
void CPowerMap::SetStrawContents(CScene2D *pStrawContents)
{
	 m_pStrawContents= pStrawContents;
}

//=============================================================================
// �e�N�X�`���Ǌ��N���X�ւ̃|�C���^��ݒ�
//=============================================================================
void CPowerMap::SetTextureManager(CTextureManager *pTextureManager)
{
	m_pTextureManager = pTextureManager;
}

//=============================================================================
// �v���C���[�̃A�C�R���ւ̃|�C���^���擾
//=============================================================================
CScene2D *CPowerMap::GetPlayerIcon(void)
{
	return m_pPlayerIcon;
}

//=============================================================================
// �G�̃A�C�R���ւ̃|�C���^���擾
//=============================================================================
CScene2D *CPowerMap::GetEnemyIcon(void)
{
	return m_pEnemyIcon;
}

//=============================================================================
// �X�g���[�ւ̃|�C���^���擾
//=============================================================================
CScene2D *CPowerMap::GetStraw(void)
{
	return m_pStraw;
}

//=============================================================================
// �X�g���[�̒��g�ւ̃|�C���^���擾
//=============================================================================
CScene2D *CPowerMap::GetStrawContents(void)
{
	return m_pStrawContents;
}

//=============================================================================
// �e�N�X�`���Ǌ��N���X�ւ̃|�C���^���擾
//=============================================================================
CTextureManager *CPowerMap::GetTextureManager(void)
{
	return m_pTextureManager;
}