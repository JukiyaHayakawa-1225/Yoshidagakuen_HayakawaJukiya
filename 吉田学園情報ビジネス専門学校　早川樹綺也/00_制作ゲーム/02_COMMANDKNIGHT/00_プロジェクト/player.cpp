//=============================================================================
//
// �v���C���[�̏��� [player.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "player.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "debugproc.h"
#include "scene3D.h"
#include "meshfield.h"
#include "model.h"
#include "game.h"
#include "object.h"
#include "command.h"
#include "enemy.h"
#include "loadfilefunction.h"
#include "status_number.h"
#include "ui.h"
#include "particle.h"
#include "meshorbit.h"
#include "command.h"
#include "icecircle.h"
#include "icicle.h"

#define MOTION_TXT_NAME "data\\TEXT\\motion.txt"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
int g_nNumModel = 2;			//���f���̑���
char g_aFilenameModel[MAX_PLAYER_MODEL][256];	//

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPD3DXMESH			CPlayer::m_pMesh[MAX_PLAYER_MODEL] = {};
LPD3DXBUFFER		CPlayer::m_pBuffMat[MAX_PLAYER_MODEL] = {};
DWORD				CPlayer::m_nNumMat[MAX_PLAYER_MODEL] = {};

//=============================================================================
// �v���C���[�̃R���X�g���N�^
//=============================================================================
CPlayer::CPlayer() : CCharacter(3, OBJTYPE_PLAYER)
{
	m_bJump = false;								//�W�����v���Ă��Ȃ����
	m_bAction = false;								//�s�����Ă��Ȃ����
	m_motionType = MOTION_NEUTRAL;					//���[�V������������
	m_nCountFrame = 0;								//�J�E���g�̏����l
	m_nLife = 100;									//�̗͏����l
	m_nSkill = 40;									//�Ƃ��������l
	m_nMaxLife = 100;								//�̗͍ő�l������
	m_nMaxSkill = 40;								//�Ƃ����̍ő�l������
	m_state = PLAYER_STATE_NOMAL;
	m_useItem = USE_ITEM0;
	m_nNumDeathBird = 0;
	m_nNumDeathFrog = 0;
	m_pMeshOrbit = NULL;
	for (int nCntSelect = 0; nCntSelect < MAX_SELECT; nCntSelect++)
	{
		m_aSkillLevel[nCntSelect] = 1;
		m_aCntLevel[nCntSelect] = 0;
		m_aItemQuantity[nCntSelect] = 1;
		m_aCutSkill[nCntSelect] = 1;
	}
	for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
	{//���f���̐������J��Ԃ�
		m_apModel[nCntModel] = NULL;
	}
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{//���f���̐������J��Ԃ�
		m_aMotionAttack[nCntMotion].nAttackEndFrame = 0;
		m_aMotionAttack[nCntMotion].nAttackStartFrame = 0;
		m_aMotionAttack[nCntMotion].nNumFrame = 0;
	}
}
//=============================================================================
// �v���C���[�̃f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// �I�u�W�F�N�g�̃e�N�X�`���ǂݍ���
//=============================================================================
HRESULT CPlayer::Load(void)
{
	//�����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	return S_OK;
}

//=============================================================================
// �I�u�W�F�N�g�̃e�N�X�`���j��
//=============================================================================
void CPlayer::Unload(void)
{
	for (int nCntMesh = 0; nCntMesh < MAX_PLAYER_MODEL; nCntMesh++)
	{//���f���̐������J��Ԃ�
	 // ���b�V���̊J��
		if (m_pMesh[nCntMesh] != NULL)
		{
			m_pMesh[nCntMesh]->Release();
			m_pMesh[nCntMesh] = NULL;
		}
	}

	for (int nCntMat = 0; nCntMat < MAX_PLAYER_MODEL; nCntMat++)
	{//���f���̐������J��Ԃ�
	 // �}�e���A���̊J��
		if (m_pBuffMat[nCntMat] != NULL)
		{
			m_pBuffMat[nCntMat]->Release();
			m_pBuffMat[nCntMat] = NULL;
		}
	}
}

//=============================================================================
//�v���C���[�̐���
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer *pPlayer = NULL;					//�v���C���[�̃|�C���^
	if (pPlayer == NULL)
	{
		pPlayer = new CPlayer;					//�V�[���̓��I�m��
		pPlayer->Init();						//����������
		pPlayer->SetPos(pos);					//�ʒu�̐ݒu����
	}
	return pPlayer;					//�l��Ԃ�
}

//=============================================================================
// �v���C���[�̏���������
//=============================================================================
HRESULT CPlayer::Init(void)
{
	//�L�����N�^�[�̏���������
	CCharacter::Init();

	//���[�V��������ǂݍ���
	LoadMotion();
	m_bJump = false;							//�W�����v���Ă��Ȃ����
	m_nKey = 0;									//���݂̃L�[��������
	m_nNumKey = 2;								//�L�[�̑�����������
	m_nCountFrame = 0;							//�J�E���g�t���[����������
												//m_pMeshOrbit = CMeshOrbit::Create();
	m_state = PLAYER_STATE_NOMAL;				//�ʏ��Ԃɂ���
	m_nCntDamage = 0;							//�_���[�W�J�E���^�[
	m_nCounter = 0;								//�J�E���^�[
	SetColRange(15.0f);							//�����蔻��̑傫���ݒu����
	for (int nCntSelect = 0; nCntSelect < MAX_SELECT; nCntSelect++)
	{
		m_aSkillLevel[nCntSelect] = 1;	//�Ƃ������x���̏�����
		m_aCntLevel[nCntSelect] = 0;	//�J�E���g���x���̏�����
		m_aItemQuantity[nCntSelect] = 3;	//�A�C�e�����̏�����
	}
	//�G���S����������
	m_nNumDeathBird = 0;
	m_nNumDeathFrog = 0;

	return S_OK;
}

//=============================================================================
// �v���C���[�̏I������
//=============================================================================
void CPlayer::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
	{//���f���̐������J��Ԃ�
		if (m_apModel[nCntModel] != NULL)
		{
			//�I������
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = NULL;
		}
	}

	Release();
}

//=============================================================================
// �v���C���[�̍X�V����
//=============================================================================
void CPlayer::Update(void)
{
	//�ߋ��̈ʒu�ݒu����
	SetPosOld(GetPos());

	//���[�V�����̍X�V����
	UpdateMotion();

	//���݂̃��[�V�������ߋ��̃��[�V�������ɑ��
	m_OldMotion = m_motionType;

	switch (m_state)
	{
	case PLAYER_STATE_NOMAL:
		break;
	case PLAYER_STATE_DAMAGE:
		m_nCntDamage++;
		if (m_nCntDamage >= 100)
		{
			m_nCntDamage = 0;
			m_state = PLAYER_STATE_NOMAL;
		}
		break;
	}

	//���͏���
	Input();

	//�ړ�����
	Move();

	////�����蔻�菈��
	Collision();

#ifdef _DEBUG
	CDebugProc::Print(1, "�v���C���[�̈ʒu  : x[%.1f],y[%.1f],z[%.1f]\n", GetPos().x, GetPos().y, GetPos().z);
	CDebugProc::Print(1, "�v���C���[�̗̑�  : %d]\n", m_nLife);
	CDebugProc::Print(1, "�v���C���[�̂Ƃ��� : %d]\n", m_nSkill);
	CDebugProc::Print(1, "���[�V����OLD %d\n", m_OldMotion);

#endif
}

//=============================================================================
// �v���C���[�̕`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	//�����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	D3DXMATRIX mtxRot, mtxTrans;					//�v�Z�p�}�g���b�N�X

	if (m_pMeshOrbit != NULL)
	{
		//�I�[�r�b�g�̐ݒu����
		m_pMeshOrbit->SetMtxParent(&m_apModel[15]->GetMtxWorld());
	}
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	D3DXMATRIX	mtxShadow;		//�V���h�E�}�g���b�N�X
	D3DXPLANE	planeField;		//���ʏ��
	D3DXVECTOR4 vecLight;		//���C�g���
	D3DXVECTOR3	posNomal, normal;	//�ʒu�A�@��

									//���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP,
		D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND,
		D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,
		D3DBLEND_ONE);

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�V���h�E�}�g���b�N�X��������
	D3DXMatrixIdentity(&mtxShadow);

	//���C�g�̐ݒ�
	vecLight = D3DXVECTOR4(0.25f, 0.87f, -0.44f, 0.0f);

	//�ʒu�̐ݒ�
	posNomal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//�@���̐ݒ�
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//�ʒu�Ɩ@�����畽�ʂ��쐬����
	D3DXPlaneFromPointNormal(&planeField, &posNomal, &normal);

	//���ʂ̎ˉe
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	//���[���h�}�g���b�N�X�Ɗ|�����킹��
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	if (m_motionType == MOTION_ICRBRAKE)
	{
		for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
		{//���f���̐������J��Ԃ�
		 //�e�N�X�`������
		 //pDevice->SetTexture(0, NULL);
		 // ���f��(�p�[�c)�̕`��
		 //m_pMesh[nCntModel]->DrawSubset(0);
		 //�`�揈��
			if (m_apModel[nCntModel] != NULL)
			{
				m_apModel[nCntModel]->Draw();
			}
		}
	}
	else
	{
		for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL - 1; nCntModel++)
		{//���f���̐������J��Ԃ�
		 //�`�揈��
			if (m_apModel[nCntModel] != NULL)
			{
				m_apModel[nCntModel]->Draw();
			}
		}
	}

	//���̐ݒ�ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP,
		D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND,
		D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,
		D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);


	if (m_motionType == MOTION_ICRBRAKE)
	{
		for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
		{
			if (m_state == PLAYER_STATE_DAMAGE)
			{// ������Ă��Ȃ���Ԃ̏ꍇ
				if (m_nCntDamage % 5 == 0)
				{// �J�E���^�[�����̒l�ɂȂ�����
				 //���f���̕`��
					if (m_apModel[nCntModel] != NULL)
					{
						m_apModel[nCntModel]->Draw();
					}
				}
			}
			else
			{
				//���f���̕`��
				m_apModel[nCntModel]->Draw();
			}
		}
	}
	else
	{
		for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL - 1; nCntModel++)
		{
			if (m_state == PLAYER_STATE_DAMAGE)
			{// ������Ă��Ȃ���Ԃ̏ꍇ
				if (m_nCntDamage % 5 == 0)
				{// �J�E���^�[�����̒l�ɂȂ�����
				 //���f���̕`��
					if (m_apModel[nCntModel] != NULL)
					{
						m_apModel[nCntModel]->Draw();
					}
				}
			}
			else
			{
				//���f���̕`��
				if (m_apModel[nCntModel] != NULL)
				{
					m_apModel[nCntModel]->Draw();
				}
			}
		}
	}
}

//=============================================================================
// �v���C���[�̃��[�V�����X�V����
//=============================================================================
void CPlayer::UpdateMotion(void)
{
	KEY *pKey, *pKeyNext;		// �L�[�̏��
	float fRateMotion;			// ���Βl
	float fDiffMotion;			// ����
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����

	if (m_OldMotion != m_motionType)
	{//�O��̃��[�V�������Ⴄ�ꍇ
		m_nKey = 0;			//���݂̃L�[��������
		m_nCountFrame = 0;	//�J�E���g�t���[����������
		m_nCntNumFrame = 0;
	}
	m_nCntNumFrame++;
	if (m_nCntNumFrame >= m_aMotionAttack[m_motionType].nAttackStartFrame &&
		m_nCntNumFrame <= m_aMotionAttack[m_motionType].nAttackEndFrame)
	{
		switch (m_motionType)
		{
		case MOTION_COMBO0:
			m_bAction = true;
			break;
		case MOTION_COMBO1:
			m_bAction = true;
			break;
		case MOTION_ICRBRAKE:
			if (m_bAction == false)
			{
				m_bAction = true;
				SwitchIceSize();
			}
			break;
		case MOTION_REVOSLASH:
			RevoSlash();
			break;
		case MOTION_FULLSWING:
			SwitchWeaponSize();
			break;
		case MOTION_ITEM:
			UseItem();
			break;
		}
	}
	else
	{
		m_bAction = false;
		m_apModel[15]->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	}

	if (m_state != PLAYER_STATE_DEATH)
	{

		for (int nCntModel = 0; nCntModel < g_nNumModel; nCntModel++)
		{//���f���̐������J��Ԃ�
			if (m_apModel[nCntModel] != NULL)
			{//���f��������ꍇ
				pKey = &m_apMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nCntModel];
				pKeyNext = &m_apMotionInfo[m_motionType].aKeyInfo[(m_nKey + 1) % m_apMotionInfo[m_motionType].nNumKey].aKey[nCntModel];

#if 1
				//�ʒu���WX
				fRateMotion = (float)(m_nCountFrame) / (float)m_apMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame;	// ���Βl�̎擾
				fDiffMotion = pKeyNext->pos.x - pKey->pos.x;														// �����̎擾
				pos.x = pKey->pos.x + (fDiffMotion * fRateMotion);

				//�ʒu���WY
				fDiffMotion = pKeyNext->pos.y - pKey->pos.y;								// �����̎擾
				pos.y = pKey->pos.y + (fDiffMotion * fRateMotion);

				//�ʒu���WZ
				fDiffMotion = pKeyNext->pos.z - pKey->pos.z;								// �����̎擾
				pos.z = pKey->pos.z + (fDiffMotion * fRateMotion);

				//��Βl����
				D3DXVECTOR3 rotPlus, rotMinus;
				rotPlus.x = pKeyNext->rot.x + pKey->rot.x;
				rotPlus.y = pKeyNext->rot.y + pKey->rot.y;
				rotPlus.z = pKeyNext->rot.z + pKey->rot.z;
				rotMinus.x = pKeyNext->rot.x - pKey->rot.x;
				rotMinus.y = pKeyNext->rot.y - pKey->rot.y;
				rotMinus.z = pKeyNext->rot.z - pKey->rot.z;
				rotPlus.x = abs(rotPlus.x);
				rotPlus.y = abs(rotPlus.y);
				rotPlus.z = abs(rotPlus.z);
				rotMinus.x = abs(rotMinus.x);
				rotMinus.y = abs(rotMinus.y);
				rotMinus.z = abs(rotMinus.z);

				if (nCntModel == 0)
				{
					if (rotPlus.x < rotMinus.x)
					{
						//�������WX
						fDiffMotion = pKeyNext->rot.x + pKey->rot.x;								// �����̎擾
						rot.x = pKey->rot.x + (fDiffMotion * fRateMotion);
					}
					else
					{
						//�������WX
						fDiffMotion = pKeyNext->rot.x - pKey->rot.x;								// �����̎擾
						rot.x = pKey->rot.x + (fDiffMotion * fRateMotion);
					}

					if (rotPlus.y < rotMinus.y)
					{
						//�������WY
						fDiffMotion = pKeyNext->rot.y + pKey->rot.y;								// �����̎擾
						rot.y = pKey->rot.y + (fDiffMotion * fRateMotion);

					}
					else
					{
						//�������WY
						fDiffMotion = pKeyNext->rot.y - pKey->rot.y;								// �����̎擾
						rot.y = pKey->rot.y + (fDiffMotion * fRateMotion);
					}

					if (rotPlus.z < rotMinus.z)
					{
						//�������WZ
						fDiffMotion = pKeyNext->rot.z + pKey->rot.z;								// �����̎擾
						rot.z = pKey->rot.z + (fDiffMotion * fRateMotion);
					}
					else
					{
						//�������WZ
						fDiffMotion = pKeyNext->rot.z - pKey->rot.z;								// �����̎擾
						rot.z = pKey->rot.z + (fDiffMotion * fRateMotion);
					}
				}
				else
				{
					//�������WX
					fDiffMotion = pKeyNext->rot.x - pKey->rot.x;								// �����̎擾
					rot.x = pKey->rot.x + (fDiffMotion * fRateMotion);

					//�������WY
					fDiffMotion = pKeyNext->rot.y - pKey->rot.y;								// �����̎擾
					rot.y = pKey->rot.y + (fDiffMotion * fRateMotion);

					//�������WZ
					fDiffMotion = pKeyNext->rot.z - pKey->rot.z;								// �����̎擾
					rot.z = pKey->rot.z + (fDiffMotion * fRateMotion);

				}
#endif
				//�����̐ݒu����
				m_apModel[nCntModel]->SetRot(rot);
				m_apModel[nCntModel]->SetPos(D3DXVECTOR3(m_offsetPos[nCntModel].x + pos.x, m_offsetPos[nCntModel].y + pos.y, m_offsetPos[nCntModel].z + pos.z));
				//�ʒu�̐ݒu����
				//m_apModel[nCntModel]->SetPos(pos);

			}
		}

		if (m_nKey <= m_apMotionInfo[m_motionType].nNumKey - 1)
		{//���݂̃L�[���Ώۂ̃L�[�̑�����菬�����ꍇ�J�E���g�t���[�������Z
			m_nCountFrame++;
		}
		if (m_nCountFrame >= m_apMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame)
		{//���݂̃t���[�������Ώۂ̃L�[�t���[�����ȏ�̏ꍇ
		 //m_nKey = (m_nKey + (m_apMotionInfo[m_motionType].nNumKey - 1)) % m_apMotionInfo[m_motionType].nNumKey;
		 //���݂̃L�[�̐؂�ւ�
			m_nKey = (m_nKey + 1) % m_apMotionInfo[m_motionType].nNumKey;
			if (m_motionType == MOTION_ICRBRAKE && m_nKey == 5)
			{
				IceBreak();
			}
			else if (m_motionType == MOTION_REVOSLASH)
			{
				RevoSlash();
			}
			else if (m_motionType == MOTION_FULLSWING)
			{
				if (m_nKey >= 5 && m_nKey <= 6)
				{
					FullSwing();
				}
			}
			if (m_nKey == m_apMotionInfo[m_motionType].nNumKey - 1 && m_apMotionInfo[m_motionType].bLoop == false)
			{//���݂̃L�[���Ώۂ̃L�[�����ɒB�������[�v���Ȃ��ꍇ
				if (m_motionType == MOTION_JUMP && m_bJump == true)
				{//
					m_nKey = m_apMotionInfo[m_motionType].nNumKey - 1;
				}
				//if (m_motionType == MOTION_PANCH || m_motionType == MOTION_LANDING)
				else if (m_motionType != MOTION_MOVE || m_motionType != MOTION_NEUTRAL)
				{

					if (m_motionType == MOTION_DEATH)
					{
						m_nKey = m_apMotionInfo[m_motionType].nNumKey - 1;
						SetState(PLAYER_STATE_DEATH);
					}
					else
					{

						//���݂̃L�[��������
						m_nKey = 0;
						//�R�}���h�̎擾
						CCommand *pCommand;
						pCommand = CGame::GetCommad();
						//�j���[�g�����ɐ؂�ւ�
						//m_motionType = MOTION_NEUTRAL;
						if (m_bJump == false)
						{
							SetCountLevel(m_motionType);
							//if (m_motionType == MOTION_COMBO1)
							//{
							//	pCommand->SetCommandState(pCommand->COMMAND_SKILL);
							//}
							//else
							{
								//�R�}���h�����s���Ă��Ȃ���Ԃɂ���
								pCommand->SetCommandState(pCommand->COMMAND_END);
							}
						}
						m_bAction = false;
					}
				}
			}
			//�J�E���g�t���[����������
			m_nCountFrame = 0;
		}
	}

#ifdef _DEBUG
	CDebugProc::Print(1, "�v���C���[�ړ�[W,A,S,D]\n");
	CDebugProc::Print(1, "Key[%d / %d]\n", m_nKey, m_apMotionInfo[m_motionType].nNumKey);
	CDebugProc::Print(1, "Frame[%d / %d]\n", m_nCountFrame, m_apMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame);
	CDebugProc::Print(1, "���[�V����[%d]\n", m_motionType);
	CDebugProc::Print(1, "�J�E���^�[����[%d]\n", m_aMotionAttack[m_motionType].nNumFrame);

	switch (m_apMotionInfo[m_motionType].bLoop)
	{
	case false:
		CDebugProc::Print(1, "���[�v���Ȃ�\n");
		break;
	case true:
		CDebugProc::Print(1, "���[�v����\n");
		break;
	}
	switch (m_bJump)
	{
	case false:
		CDebugProc::Print(1, "�W�����v���Ă��Ȃ�\n");
		break;
	case true:
		CDebugProc::Print(1, "�W�����v���Ă���\n");
		break;
	}
	switch (m_bAction)
	{
	case false:
		CDebugProc::Print(1, "�����蔻��OFF\n");

		break;
	case true:
		CDebugProc::Print(1, "�����蔻��ON\n");
		break;
	}
#endif
}
//=============================================================================
// �v���C���[�̍s����Ԃ��擾
//=============================================================================
bool CPlayer::GetAction(void)
{
	return m_bAction;
}

//=============================================================================
// �v���C���[�̗͎̑擾����
//=============================================================================
int CPlayer::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
// �v���C���[�̂Ƃ����擾����
//=============================================================================
int CPlayer::GetSkill(void)
{
	return m_nSkill;
}

//=============================================================================
// �v���C���[�̗͍̑ő�l�擾����
//=============================================================================
int CPlayer::GetMaxLife(void)
{
	return m_nMaxLife;
}

//=============================================================================
// �v���C���[�̂Ƃ����ő�l�擾����
//=============================================================================
int CPlayer::GetMaxSkill(void)
{
	return m_nMaxSkill;
}

//=============================================================================
// �v���C���[�̍s����Ԃ̐ݒu����
//=============================================================================
void CPlayer::SetAction(bool action)
{
	m_bAction = action;
}

//=============================================================================
// �v���C���[�̗̑͐ݒu����
//=============================================================================
void CPlayer::SetLife(int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
// �v���C���[�̂Ƃ����ݒu����
//=============================================================================
void CPlayer::SetSkill(int nSkill)
{
	m_nSkill = nSkill;
}

//=============================================================================
// �v���C���[�̗͍̑ő�l�ݒu����
//=============================================================================
void CPlayer::SetMaxLife(int nMaxLife)
{
	m_nMaxLife = nMaxLife;
}

//=============================================================================
// �v���C���[�̏�Ԑݒu����
//=============================================================================
void CPlayer::SetState(PLAYER_STATE state)
{
	m_state = state;
}

//=============================================================================
// �v���C���[�̂Ƃ����ʉ��Z����
//=============================================================================
void CPlayer::AddSkillLevel(int nSelect, int nValue)
{
	m_aSkillLevel[nSelect] += nValue;
}

//=============================================================================
// �v���C���[�̂ǂ����ʉ��Z����
//=============================================================================
void CPlayer::AddItemQuantity(int nSelect, int nValue)
{
	m_aItemQuantity[nSelect] += nValue;
}

//=============================================================================
// �v���C���[�̂Ƃ����ʎ擾����
//=============================================================================
int CPlayer::GetSkillLevel(int nSelect)
{
	return m_aSkillLevel[nSelect];
}

//=============================================================================
// �v���C���[�̂ǂ����ʎ擾����
//=============================================================================
int CPlayer::GetItemQuantity(int nSelect)
{
	return m_aItemQuantity[nSelect];
}

//=============================================================================
// ���[�V�����̐ݒu����
//=============================================================================
void CPlayer::SetMotion(MOTION_TYPE type)
{
	m_motionType = type;
}

//=============================================================================
//�@�W�����v�����Ă��邩���擾
//=============================================================================
bool CPlayer::GetJump(void)
{
	return m_bJump;
}

//=============================================================================
//�@���[�V������ނ̎擾
//=============================================================================
CPlayer::MOTION_TYPE CPlayer::GetMotion(void)
{
	return m_motionType;
}

CPlayer::PLAYER_STATE CPlayer::GetState(void)
{
	return m_state;
}

//=============================================================================
//�@���͏���
//=============================================================================
void CPlayer::Input(void)
{
	float fDiffAngle;

	//�L�[�{�[�h�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();
	//�W���C�p�b�h�̎擾
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetJoypad();

	//�J�����̎擾
	CCamera *pCamera;
	pCamera = CManager::GetCamera();
	//�J�����̌����擾
	D3DXVECTOR3 cameraRot = pCamera->GetRot();

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();

	//�R�}���h�̎擾
	CCommand *pCommand;
	pCommand = CGame::GetCommad();

	//�X�e�[�^�X�����̎擾
	CStatus_Number *pStatus_Number;
	pStatus_Number = CGame::GetStatusNumber();

	CUI_LifeGauge *pUI_Life;
	pUI_Life = CGame::GetUI_LifeGauge();

	CUI_SkillGauge *pUI_Skill;
	pUI_Skill = CGame::GetUI_SkillGauge();

	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();

	if (m_state != PLAYER_STATE_DEATH)
	{//����ł��Ȃ��ꍇ
		if (pCommand != NULL)
		{//�R�}���h��NULL�ł͂Ȃ��ꍇ
			if (pCommand->GetCommandState() == pCommand->COMMAND_NONE || pCommand->GetCommandState() == pCommand->COMMAND_END /*|| pCommand->GetCommandState() == pCommand->COMMAND_SKILL*/)
			{//�R�}���h�����s����Ă��Ȃ��ꍇ

			 //�p�b�h�ł̑���
				if (pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_UP) == true ||
					pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_DOWN) == true ||
					pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_LEFT) == true ||
					pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_RIGHT) == true)
				{
					if (m_bJump == false)
					{//�W�����v�����Ă��Ȃ��ꍇ
					 //�ړ����[�V�����ɐ؂�ւ�
						m_motionType = MOTION_MOVE;
					}
					//�ړ�����
					move.x -= sinf(cameraRot.y - pInputJoypad->GetLeftAxiz()) * 2.0f;
					move.z -= cosf(cameraRot.y - pInputJoypad->GetLeftAxiz()) * 2.0f;
					float fDest = (cameraRot.y - pInputJoypad->GetLeftAxiz());

					fDiffAngle = fDest - rot.y;
					SetDiffAngle(fDiffAngle);
				}
				else
				{
					m_motionType = MOTION_NEUTRAL;
				}

				//�L�[�{�[�h�ł̑���
				if (pInputKeyboard->GetPress(DIK_A) == true)
				{// �L�[�{�[�h��[A]�L�[�������ꂽ�i���ړ��j
				 //�ړ����[�V�����ɂ���
					if (m_bJump == false)
					{//�W�����v�����Ă��Ȃ��ꍇ
					 //�ړ����[�V�����ɐ؂�ւ�
						m_motionType = MOTION_MOVE;
					}
					if (pInputKeyboard->GetPress(DIK_W) == true)
					{// �L�[�{�[�h��[W]�L�[�������ꂽ�i����ړ��j
						move.x += -sinf(cameraRot.y + D3DX_PI * 0.75f) * PLAYER_MOVE;
						move.z += -cosf(cameraRot.y + D3DX_PI * 0.75f) * PLAYER_MOVE;
						//rot.y = cameraRot.y + D3DX_PI * 0.75f;
						fDiffAngle = (cameraRot.y + D3DX_PI * 0.75f) - rot.y;
						SetDiffAngle(fDiffAngle);

					}
					else if (pInputKeyboard->GetPress(DIK_S) == true)
					{// �L�[�{�[�h��[S]�L�[�������ꂽ�i�����ړ��j
						move.x += -sinf(cameraRot.y + D3DX_PI * 0.25f) * PLAYER_MOVE;
						move.z += -cosf(cameraRot.y + D3DX_PI * 0.25f) * PLAYER_MOVE;
						fDiffAngle = (cameraRot.y + D3DX_PI * 0.25f) - rot.y;
						SetDiffAngle(fDiffAngle);

					}
					else
					{// �L�[�{�[�h��[A]�L�[�������ꂽ�i���ړ��j
						move.x += -sinf(cameraRot.y + D3DX_PI / 2) * PLAYER_MOVE;
						move.z += -cosf(cameraRot.y + D3DX_PI / 2) * PLAYER_MOVE;
						fDiffAngle = (cameraRot.y + D3DX_PI / 2) - rot.y;
						SetDiffAngle(fDiffAngle);

					}
				}
				else if (pInputKeyboard->GetPress(DIK_D) == true)
				{// �L�[�{�[�h��[D]�L�[�������ꂽ�i�E�ړ��j
					if (m_bJump == false)
					{//�W�����v�����Ă��Ȃ��ꍇ
					 //�ړ����[�V�����ɐ؂�ւ�
						m_motionType = MOTION_MOVE;
					}

					if (pInputKeyboard->GetPress(DIK_W) == true)
					{// �L�[�{�[�h��[W]�L�[�������ꂽ�i�E��ړ��j
						move.x += -sinf(cameraRot.y + D3DX_PI * -0.75f) * PLAYER_MOVE;
						move.z += -cosf(cameraRot.y + D3DX_PI * -0.75f) * PLAYER_MOVE;
						//rot.y = cameraRot.y + D3DX_PI * -0.75f;
						fDiffAngle = (cameraRot.y + D3DX_PI * -0.75f) - rot.y;
						SetDiffAngle(fDiffAngle);

					}
					else if (pInputKeyboard->GetPress(DIK_S) == true)
					{// �L�[�{�[�h��[S]�L�[�������ꂽ�i�E���ړ��j
						move.x += -sinf(cameraRot.y + D3DX_PI * -0.25f) * PLAYER_MOVE;
						move.z += -cosf(cameraRot.y + D3DX_PI * -0.25f) * PLAYER_MOVE;
						//rot.y = cameraRot.y + D3DX_PI * -0.25f;
						fDiffAngle = (cameraRot.y + D3DX_PI * -0.25f) - rot.y;
						SetDiffAngle(fDiffAngle);

					}
					else
					{// �L�[�{�[�h��[D]�L�[�������ꂽ�i�E�ړ��j
						move.x += -sinf(cameraRot.y + D3DX_PI / -2) * PLAYER_MOVE;
						move.z += -cosf(cameraRot.y + D3DX_PI / -2) * PLAYER_MOVE;
						//rot.y = cameraRot.y + D3DX_PI / -2;
						fDiffAngle = (cameraRot.y + D3DX_PI / -2) - rot.y;
						SetDiffAngle(fDiffAngle);

					}
				}
				else if (pInputKeyboard->GetPress(DIK_W) == true)
				{//�L�[�{�[�h��[W]�L�[�������ꂽ�i�O�ړ��j
					if (m_bJump == false)
					{//�W�����v�����Ă��Ȃ��ꍇ
					 //�ړ����[�V�����ɐ؂�ւ�
						m_motionType = MOTION_MOVE;
					}
					move.x -= sinf(cameraRot.y + D3DX_PI * 1)*PLAYER_MOVE;
					move.z -= cosf(cameraRot.y + D3DX_PI * 1)*PLAYER_MOVE;
					//	rot.y = cameraRot.y + D3DX_PI * 1;
					fDiffAngle = (cameraRot.y + D3DX_PI) - rot.y;
					SetDiffAngle(fDiffAngle);

				}
				else if (pInputKeyboard->GetPress(DIK_S) == true)
				{//�L�[�{�[�h��[S]�L�[�������ꂽ�i��ړ��j
					if (m_bJump == false)
					{//�W�����v�����Ă��Ȃ��ꍇ
					 //�ړ����[�V�����ɐ؂�ւ�
						m_motionType = MOTION_MOVE;
					}
					move.x += -sinf(cameraRot.y + D3DX_PI * 0)*PLAYER_MOVE;
					move.z += -cosf(cameraRot.y + D3DX_PI * 0)*PLAYER_MOVE;
					fDiffAngle = (cameraRot.y + D3DX_PI * 0) - rot.y;
					SetDiffAngle(fDiffAngle);

				}

			}
		}
	}
	SetMove(move);
}

//=============================================================================
// �v���C���[�̈ړ�����
//=============================================================================
void CPlayer::Move(void)
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	pos = GetPos();
	move = GetMove();

	//�d��
	move.y -= cosf(D3DX_PI * 0) * 0.5f;

	//�ړ����Z����
	//pos.y += move.y;

	pos += move;

	//��������
	move.x += (0.0f - move.x) * 0.8f;
	move.z += (0.0f - move.z) * 0.8f;
	if (m_bJump == false)
	{//�W�����v���Ă��Ȃ��ꍇ
	 //�n�ʂ̍����擾�ʒu���
		pos.y = CGame::GetMeshField()->GetHeight(pos);
		//�ړ��ʏ�����
		move.y = 0.0f;
	}
	else
	{//�W�����v���Ă���ꍇ
		if (pos.y <= CGame::GetMeshField()->GetHeight(pos))
		{//�n�ʂɂ��Ă���ꍇ
		 //�W�����v���Ă��Ȃ���Ԃɂ���
			m_bJump = false;
			//�j���[�g�������[�V�����ɂ���
			m_motionType = MOTION_NEUTRAL;
			//�ړ��ʂ�������
			move.y = 0.0f;
		}
	}
	SetPos(pos);
	SetMove(move);
	CDebugProc::Print(1, "�ړ��ʂ� %f\n", move.y);
}

//=============================================================================
// �v���C���[�̓����蔻�菈��
//=============================================================================
void CPlayer::Collision(void)
{
	int nattack = 0;
	float fRange = 30.0f;
	float fWeaponRange = 15.0f;
	float fLength = 0.0f;
	float fHit = 0.0f;
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posOld = GetPosOld();
	D3DXVECTOR3 move = GetMove();

	// �z�u���ƒn�ʂւ̃|�C���^�擾����
	CScene *pScene = NULL;               // �I�u�W�F�N�g�̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃I�u�W�F�N�g�̃|�C���^
	CObject *pObject = NULL;             // �z�u���̃|�C���^
	D3DXVECTOR3 Offset = D3DXVECTOR3(0.0f, 25.0f, 0.0f);
	D3DXVec3TransformCoord(&Offset, &Offset, &m_apModel[15]->GetMtxWorld());
	fHit = sqrtf((fRange + fWeaponRange) * (fRange + fWeaponRange));

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULL�ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{// �z�u����������
				pObject = (CObject*)pScene;
				if (pObject != NULL)
				{// �z�u�����擾�ł���
					if (pObject->Collision(&pos, &posOld, &move) == true)
					{// �z�u���̏�ɏ���Ă���
						m_bJump = false;
					}
				}
			}
			if (pScene->GetObjType() == OBJTYPE_ENEMY ||
				pScene->GetObjType() == OBJTYPE_BIRD ||
				pScene->GetObjType() == OBJTYPE_FROG)
			{
				CEnemy *pEnemy = (CEnemy*)pScene;
				if (pEnemy != NULL)
				{// �z�u�����擾�ł���
					if (m_bAction == true)
					{

						D3DXVECTOR3 EnemeyPos, EnemyMove;
						EnemeyPos = pEnemy->GetPos();
						EnemyMove = pEnemy->GetMove();
						fLength = sqrtf((Offset.x - EnemeyPos.x) * (Offset.x - EnemeyPos.x) + (Offset.y - EnemeyPos.y) * (Offset.y - EnemeyPos.y) + (Offset.z - EnemeyPos.z) * (Offset.z - EnemeyPos.z));

						if (fHit >= fLength)
						{
							int nDamage = rand() % 1 + 3;
							pEnemy->HitDamage(nDamage);
							float fAttackAngle = atan2f(EnemeyPos.x - pos.x, EnemeyPos.z - pos.z);// ���������p�x���v�Z
							EnemyMove.x = sinf((D3DX_PI *  fAttackAngle)) * -0.5f;
							EnemyMove.z = cosf((D3DX_PI *  fAttackAngle)) * 0.5f;
							if (pEnemy->GetObjType() == OBJTYPE_BIRD)
							{
								pEnemy->SetMove(EnemyMove);
							}
							nattack = 1;
							m_bAction = false;
						}

					}
					else
					{
						/*if (pEnemy->Collision(&pos, &posOld, &move, GetColRange()) == true)
						{

						}*/
					}
				}
			}
			// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
			pScene = pSceneNext;
		}
	}
	SetPos(pos);
	CDebugProc::Print(1, "�������Ă��邩�ǂ���%d\n", nattack);
}

//=============================================================================
// �v���C���[�̃��[�V�����J�E���^�[�ݒu�擾
//=============================================================================
void CPlayer::SetCountLevel(MOTION_TYPE type)
{
	//�Ƃ����̃��x���J�E���^�[���Z����
	switch (type)
	{
	case MOTION_ICRBRAKE:
		m_aCntLevel[0]++;
		break;
	case MOTION_REVOSLASH:
		m_aCntLevel[1]++;
		break;
	case MOTION_FULLSWING:
		m_aCntLevel[2]++;
		break;
	}

	//�J�E���^�[�����ƂɃ��x����ύX����
	for (int nCntLevel = 0; nCntLevel < MAX_SELECT; nCntLevel++)
	{
		switch (m_aCntLevel[nCntLevel])
		{
		case 1:
			m_aSkillLevel[nCntLevel] = 2;
			m_aCutSkill[nCntLevel] = 2;
			break;
		case 2:
			m_aSkillLevel[nCntLevel] = 3;
			m_aCutSkill[nCntLevel] = 3;
			break;
		case 4:
			m_aSkillLevel[nCntLevel] = 4;
			m_aCutSkill[nCntLevel] = 3;
			break;
		case 6:
			m_aSkillLevel[nCntLevel] = 5;
			m_aCutSkill[nCntLevel] = 4;
			break;
		case 8:
			m_aSkillLevel[nCntLevel] = 6;
			m_aCutSkill[nCntLevel] = 4;
			break;
		case 10:
			m_aSkillLevel[nCntLevel] = 7;
			m_aCutSkill[nCntLevel] = 5;
			break;
		case 12:
			m_aSkillLevel[nCntLevel] = 8;
			m_aCutSkill[nCntLevel] = 5;
			break;
		case 14:
			m_aSkillLevel[nCntLevel] = 9;
			m_aCutSkill[nCntLevel] = 6;

			break;
		case 16:
			m_aSkillLevel[nCntLevel] = 10;
			m_aCutSkill[nCntLevel] = 6;
			break;
		}
	}
}
//=============================================================================
// �v���C���[�̕���傫���؂�ւ�����
//=============================================================================
void CPlayer::SwitchWeaponSize(void)
{
	float fData = (float)m_aSkillLevel[2] / 5.0f;
	m_apModel[15]->SetSize(D3DXVECTOR3(1.0f + fData, 1.0f + fData, 1.0f + fData));

	for (int nCntParticle = 0; nCntParticle < 5 * fData; nCntParticle++)
	{
		m_fSpin += 50.0f;
		float fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
		D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
		movePar.x = sinf(fAngle) * (rand() % 10 * 0.75f);
		movePar.y = (rand() % 10 * 0.75f);
		movePar.z = cosf(fAngle) * (rand() % 10 * 0.75f);
		CDamageParticle::Create(D3DXVECTOR3(m_apModel[15]->GetMtxWorld()._41, m_apModel[15]->GetMtxWorld()._42, m_apModel[15]->GetMtxWorld()._43), movePar, D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f), rand() % 10 + 10.0f, 25);
	}
}

//=============================================================================
// �v���C���[�̕���傫���؂�ւ�����
//=============================================================================
void CPlayer::SwitchIceSize(void)
{
	D3DXVECTOR3 pos = GetPos();
	float fData = (float)m_aSkillLevel[0] / 5.0f;
	if (m_aSkillLevel[0] >= 0 && m_aSkillLevel[0] <= 4)
	{
		CIceCircle::Create(D3DXVECTOR3(pos.x, 2.0f, pos.z));
		float fAngle = D3DX_PI;	//�~����
		float fRadius = 40.0f;	//���a
		D3DXVECTOR3 ParPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�p�[�e�B�N���̈ʒu
		if (fAngle > D3DX_PI)
		{
			fAngle -= -D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{
			fAngle += D3DX_PI * 2.0f;
		}

		for (int nCntRad = 0; nCntRad < 3; nCntRad++)
		{
			for (int nCntPar = 0; nCntPar < 10 * fData; nCntPar++)
			{
				ParPos = D3DXVECTOR3(sinf(fAngle) * fRadius, 0.0f, cosf(fAngle) * fRadius);

				CSkill2Particle::Create(D3DXVECTOR3(ParPos.x + pos.x, 0.0f, ParPos.z + pos.z),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					rand() % 10 + 5.0f,
					50);

				fAngle += 15.0f;
			}
			fRadius += 30.0f;
		}
	}
	else if (m_aSkillLevel[0] >= 5 && m_aSkillLevel[1] <= 10)
	{
		CIcicle::Create(pos, CIcicle::ICICLE_RIGIHT);
		CIcicle::Create(pos, CIcicle::ICICLE_LEFT);

		CIceCircle::Create(D3DXVECTOR3(pos.x, 2.0f, pos.z));
		float fAngle = D3DX_PI;	//�~����
		float fRadius = 40.0f;	//���a
		D3DXVECTOR3 ParPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�p�[�e�B�N���̈ʒu
		if (fAngle > D3DX_PI)
		{
			fAngle -= -D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{
			fAngle += D3DX_PI * 2.0f;
		}

		for (int nCntRad = 0; nCntRad < 3; nCntRad++)
		{
			for (int nCntPar = 0; nCntPar < 5 * fData; nCntPar++)
			{
				ParPos = D3DXVECTOR3(sinf(fAngle) * fRadius, 0.0f, cosf(fAngle) * fRadius);

				CSnowParticle::Create(D3DXVECTOR3(ParPos.x + pos.x, 150.0f, ParPos.z + pos.z),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					rand() % 10 + 5.0f,
					150);

				fAngle += 10.0f;
			}
			fRadius += 30.0f;
		}
	}
}
//=============================================================================
// �v���C���[�̃_���[�W����
//=============================================================================
void CPlayer::HitDamage(int nDamage)
{
	D3DXVECTOR3 pos = GetPos();

	CCommand::COMMAND_STATE state;
	state = CGame::GetCommad()->GetCommandState();
	if (m_state != PLAYER_STATE_DAMAGE &&state != CCommand::COMMAND_PROCESS)
	{
		//�X�e�[�^�X�����̎擾
		CStatus_Number *pStatus_Number;
		pStatus_Number = CGame::GetStatusNumber();

		CUI_LifeGauge *pUI_Life;
		pUI_Life = CGame::GetUI_LifeGauge();

		CUI_SkillGauge *pUI_Skill;
		pUI_Skill = CGame::GetUI_SkillGauge();
		m_nLife -= nDamage;
		pUI_Life->CutLifeGauge(-1);
		pStatus_Number->AddStatus_Number(m_nLife, 0);

		if (m_nLife <= 0)
		{
			m_nLife = 0;
			SetMotion(MOTION_DEATH);
		}
		else
		{
			m_state = PLAYER_STATE_DAMAGE;
		}
		//�T�E���h�̎擾
		CSound *pSound;
		pSound = CManager::GetSound();
		pSound->PlaySound(CSound::SOUND_LABEL_SE_DAMAGE);
		for (int nCntParticle = 0; nCntParticle < 15; nCntParticle++)
		{
			m_fSpin += 50.0f;
			float fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
			D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
			movePar.x = sinf(fAngle) * (rand() % 10 * 0.75f);
			movePar.y = (rand() % 10 * 0.75f);
			movePar.z = cosf(fAngle) * (rand() % 10 * 0.75f);

			CDamageParticle::Create(pos, movePar, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), rand() % 10 + 10.0f, 25);
		}
	}

}

//=============================================================================
// �v���C���[�̂ǂ����ݒu����
//=============================================================================
void CPlayer::SetUseItem(USE_ITEM item)
{
	m_useItem = item;
}

//=============================================================================
// �v���C���[�̂ǂ����g�p����
//=============================================================================
void CPlayer::UseItem(void)
{
	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_SE_CURE);

	for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
	{
		m_fSpin += 50.0f;
		float fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
		D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
		movePar.x = sinf(fAngle) * (rand() % 10 * 0.75f);
		movePar.y = (rand() % 10 * 0.75f);
		movePar.z = cosf(fAngle) * (rand() % 10 * 0.75f);
		CDamageParticle::Create(D3DXVECTOR3(m_apModel[8]->GetMtxWorld()._41, m_apModel[8]->GetMtxWorld()._42, m_apModel[8]->GetMtxWorld()._43), movePar, D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f), rand() % 3 + 5.0f, 25);
	}

	switch (m_useItem)
	{
	case USE_ITEM0:
		m_nLife += 50;
		if (m_nLife >= 100)
		{
			m_nLife = 100;
		}
		break;
	case USE_ITEM1:
		m_nSkill += 20;
		if (m_nSkill >= 40)
		{
			m_nSkill = 40;
		}
		break;
	case USE_ITEM2:
		m_nLife += 50;
		m_nSkill += 20;

		if (m_nLife >= 100)
		{
			m_nLife = 100;
		}
		if (m_nSkill >= 40)
		{
			m_nSkill = 40;
		}
		break;
	}
}
//=============================================================================
// �v���C���[�̃X�L�����Y�ʎ擾����
//=============================================================================
int CPlayer::GetCutSkill(int nSkill)
{
	return m_aCutSkill[nSkill];
}

//=============================================================================
// �v���C���[�̃_���[�W����
//=============================================================================
void CPlayer::IceBreak(void)
{
	// �z�u���ƒn�ʂւ̃|�C���^�擾����
	CScene *pScene = NULL;               // �I�u�W�F�N�g�̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃I�u�W�F�N�g�̃|�C���^
	CObject *pObject = NULL;             // �z�u���̃|�C���^
	D3DXVECTOR3 pos = GetPos();

	//CIceCircle::Create(D3DXVECTOR3(pos.x, 2.0f, pos.z));
	float fAngle = D3DX_PI;	//�~����
	float fRadius = 10.0f;	//���a
	D3DXVECTOR3 ParPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�p�[�e�B�N���̈ʒu

	for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
	{
		m_fSpin += 50.0f;
		float fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
		D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
		movePar.x = sinf(fAngle) * (rand() % 10 * 1.0f);
		movePar.y = (rand() % 30 * 0.75f);
		movePar.z = cosf(fAngle) * (rand() % 10 * 1.0f);
		CFountainParticle::Create(pos, movePar, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), rand() % 10 + 10.0f, 25);
	}
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULL�ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_ENEMY ||
				pScene->GetObjType() == OBJTYPE_BIRD ||
				pScene->GetObjType() == OBJTYPE_FROG)
			{
				CEnemy *pEnemy = (CEnemy*)pScene;

				if (pEnemy != NULL)
				{// �z�u�����擾�ł���
					D3DXVECTOR3 EnemeyPos, EnemyMove;
					EnemeyPos = pEnemy->GetPos();
					EnemyMove = pEnemy->GetMove();

					float fLength = powf((EnemeyPos.x - pos.x)*(EnemeyPos.x - pos.x) + (EnemeyPos.z - pos.z)*(EnemeyPos.z - pos.z), 0.5f);	//�v���C���[�Ƃ̋���
					float fHit = powf(12.0f, 2.0f);	//���a

					if (fHit > fLength)
					{
						int nData = (int)m_aSkillLevel[0];
						nData = rand() % 2 + nData;
						pEnemy->HitDamage(nData);
						float fAttackAngle = atan2f(EnemeyPos.x - pos.x, EnemeyPos.z - pos.z);// ���������p�x���v�Z
						EnemyMove.x = sinf((D3DX_PI *  fAttackAngle)) * 25.0f;
						EnemyMove.z = cosf((D3DX_PI *  fAttackAngle)) * 25.0f;
						if (pEnemy->GetObjType() == OBJTYPE_BIRD)
						{
							pEnemy->SetMove(EnemyMove);
						}
					}

				}
			}
			// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
			pScene = pSceneNext;

		}
	}
}

//=============================================================================
// �v���C���[�̃_���[�W����
//=============================================================================
void CPlayer::RevoSlash(void)
{
	// �z�u���ƒn�ʂւ̃|�C���^�擾����
	CScene *pScene = NULL;               // �I�u�W�F�N�g�̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃I�u�W�F�N�g�̃|�C���^
	CObject *pObject = NULL;             // �z�u���̃|�C���^
	D3DXVECTOR3 pos = GetPos();

	float fData = (float)m_aSkillLevel[1] / 5.0f;

	for (int nCntParticle = 0; nCntParticle < 5 * fData; nCntParticle++)
	{
		m_fSpin += 50.0f;
		float fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
		D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
		movePar.x = sinf(fAngle) * (rand() % 10 * 0.75f);
		movePar.y = (rand() % 10 * 0.0f);
		movePar.z = cosf(fAngle) * (rand() % 10 * 0.75f);
		CDamageParticle::Create(D3DXVECTOR3(m_apModel[15]->GetMtxWorld()._41, m_apModel[15]->GetMtxWorld()._42, m_apModel[15]->GetMtxWorld()._43), movePar, D3DXCOLOR(rand() % 10 * 0.5f, rand() % 10 * 0.5f, 1.0f, 1.0f), rand() % 3 + 15.0f, 25);
	}

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULL�ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_ENEMY ||
				pScene->GetObjType() == OBJTYPE_BIRD ||
				pScene->GetObjType() == OBJTYPE_FROG)
			{
				CEnemy *pEnemy = (CEnemy*)pScene;

				if (pEnemy != NULL)
				{// �z�u�����擾�ł���
					D3DXVECTOR3 EnemeyPos, EnemyMove;
					EnemeyPos = pEnemy->GetPos();
					EnemyMove = pEnemy->GetMove();
					float fLength = powf((EnemeyPos.x - pos.x)*(EnemeyPos.x - pos.x) + (EnemeyPos.z - pos.z)*(EnemeyPos.z - pos.z), 0.5f);	//�v���C���[�Ƃ̋���
					float fHit = powf(8.0f, 2.0f);	//���a

					if (fHit > fLength)
					{
						int nData = (int)m_aSkillLevel[1];
						nData = rand() % 3 + nData;
						pEnemy->HitDamage(nData);
						float fAttackAngle = atan2f(EnemeyPos.x - pos.x, EnemeyPos.z - pos.z);// ���������p�x���v�Z
						EnemyMove.x = sinf((D3DX_PI *  fAttackAngle)) * 25.0f;
						EnemyMove.z = cosf((D3DX_PI *  fAttackAngle)) * 25.0f;
						if (pEnemy->GetObjType() == OBJTYPE_BIRD)
						{
							pEnemy->SetMove(EnemyMove);
						}
					}

				}
			}
			// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
			pScene = pSceneNext;

		}
	}
}

//=============================================================================
// �v���C���[�̃_���[�W����
//=============================================================================
void CPlayer::FullSwing(void)
{
	// �z�u���ƒn�ʂւ̃|�C���^�擾����
	CScene *pScene = NULL;               // �I�u�W�F�N�g�̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃I�u�W�F�N�g�̃|�C���^
	CObject *pObject = NULL;             // �z�u���̃|�C���^
	D3DXVECTOR3 pos = GetPos();

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULL�ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_ENEMY ||
				pScene->GetObjType() == OBJTYPE_BIRD ||
				pScene->GetObjType() == OBJTYPE_FROG)
			{
				CEnemy *pEnemy = (CEnemy*)pScene;

				if (pEnemy != NULL)
				{// �z�u�����擾�ł���
					D3DXVECTOR3 EnemeyPos, EnemyMove;
					EnemeyPos = pEnemy->GetPos();
					EnemyMove = pEnemy->GetMove();
					float fLength = powf((EnemeyPos.x - pos.x)*(EnemeyPos.x - pos.x) + (EnemeyPos.z - pos.z)*(EnemeyPos.z - pos.z), 0.75f);	//�v���C���[�Ƃ̋���
					float fHit = powf(6.0f, 4.0f);	//���a

					if (fHit > fLength)
					{
						int nData = (int)m_aSkillLevel[2];
						nData = 7;
						pEnemy->HitDamage(nData);
						float fAttackAngle = atan2f(EnemeyPos.x - pos.x, EnemeyPos.z - pos.z);// ���������p�x���v�Z
						EnemyMove.x = sinf((D3DX_PI *  fAttackAngle)) * 25.0f;
						EnemyMove.z = cosf((D3DX_PI *  fAttackAngle)) * 25.0f;
						if (pEnemy->GetObjType() == OBJTYPE_BIRD)
						{
							pEnemy->SetMove(EnemyMove);
						}

					}

				}
			}
			// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
			pScene = pSceneNext;

		}
	}
}
//=============================================================================
// ���G�̎��S�����Z����
//=============================================================================
void CPlayer::AddDeathBird(int nValue)
{
	m_nNumDeathBird += nValue;
}

//=============================================================================
// ������G�̎��S�����Z����
//=============================================================================
void CPlayer::AddDeathFrog(int nValue)
{
	m_nNumDeathFrog += nValue;
}

//=============================================================================
// ���G�̎��S�����Z����
//=============================================================================
int CPlayer::GetDeathBird(void)
{
	return m_nNumDeathBird;
}

//=============================================================================
// ������G�̎��S���擾����
//=============================================================================
int CPlayer::GetDeathFrog(void)
{
	return m_nNumDeathFrog;
}

//=============================================================================
// �U���̓����蔻�菈��
//=============================================================================
bool CPlayer::HitAttack(void)
{
	int nattack = 0;
	float fRange = 30.0f;
	float fWeaponRange = 15.0f;
	float fLength = 0.0f;
	float fHit = 0.0f;
	bool bHit = false;
	D3DXVECTOR3 pos = GetPos();

	// �z�u���ƒn�ʂւ̃|�C���^�擾����
	CScene *pScene = NULL;               // �I�u�W�F�N�g�̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃I�u�W�F�N�g�̃|�C���^
	CObject *pObject = NULL;             // �z�u���̃|�C���^
	D3DXVECTOR3 Offset = D3DXVECTOR3(0.0f, 25.0f, 0.0f);
	D3DXVec3TransformCoord(&Offset, &Offset, &m_apModel[15]->GetMtxWorld());
	fHit = sqrtf((fRange + fWeaponRange) * (fRange + fWeaponRange));

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULL�ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_ENEMY ||
				pScene->GetObjType() == OBJTYPE_BIRD ||
				pScene->GetObjType() == OBJTYPE_FROG)
			{
				CEnemy *pEnemy = (CEnemy*)pScene;
				if (pEnemy != NULL)
				{// �z�u�����擾�ł���
					if (m_bAction == true)
					{

						D3DXVECTOR3 EnemeyPos, EnemyMove;
						EnemeyPos = pEnemy->GetPos();
						EnemyMove = pEnemy->GetMove();
						fLength = sqrtf((Offset.x - EnemeyPos.x) * (Offset.x - EnemeyPos.x) + (Offset.y - EnemeyPos.y) * (Offset.y - EnemeyPos.y) + (Offset.z - EnemeyPos.z) * (Offset.z - EnemeyPos.z));

						if (fHit >= fLength)
						{
							int nDamage = rand() % 1 + 3;
							pEnemy->HitDamage(nDamage);
							float fAttackAngle = atan2f(EnemeyPos.x - pos.x, EnemeyPos.z - pos.z);// ���������p�x���v�Z
							EnemyMove.x = sinf((D3DX_PI *  fAttackAngle)) * -25.0f;
							EnemyMove.z = cosf((D3DX_PI *  fAttackAngle)) * 25.0f;
							if (pEnemy->GetObjType() == OBJTYPE_BIRD)
							{
								pEnemy->SetMove(EnemyMove);
								bHit = true;
							}
							m_nSkill += 1;
							if (m_nSkill >= 40)
							{
								m_nSkill = 40;
							}
							nattack = 1;
							m_bAction = false;
						}

					}
				}
			}
			// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
			pScene = pSceneNext;
		}
	}

	return bHit;
}

//=============================================================================
// �v���C���[�̃��[�V�����ǂݍ���
//=============================================================================
void CPlayer::LoadMotion(void)
{
	//�����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXVECTOR3 pos = GetPos();

	int nCntAttack = 0;
	FILE *pFile;	//�t�@�C���̃|�C���^

	pFile = fopen(MOTION_TXT_NAME, "r");	//�e�L�X�g�ǂݍ���
	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		char *pStrCur;						//������̐擪�ւ̃|�C���^
		int nCntModel = 0;					//���f���J�E���^�[
		int nCntMotion = 0;
		char *pStr = NULL;
		char *pLine = NULL;

		pStr = new char[256];
		pLine = new char[256];

		//************************************************************************
		// ���f���̑���&�t�@�C���̓ǂݍ���
		//************************************************************************
		while (1)
		{//���f���t�@�C����ǂݍ��ނ܂ŌJ��Ԃ�
			pStrCur = CLoadFileFunction::ReadLine(pFile, pLine);
			strcpy(pStr, pStrCur);				//����������o��

			if (CLoadFileFunction::Memcmp(pStrCur, TEXT_NUM_MODEL) == 0)
			{//�e�L�X�g�����f���̑����������s�̏ꍇ
				strcpy(pStr, pStrCur);
				g_nNumModel = CLoadFileFunction::ReadInt(pStrCur, TEXT_NUM_MODEL);	//���f���̑����ɑ��
			}
			else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_FILENAME_MODEL) == 0)
			{//���f���t�@�C�����������s�̏ꍇ
				if (nCntModel < g_nNumModel)
				{
					pStr = CLoadFileFunction::ReadString(pStrCur, pStr, TEXT_FILENAME_MODEL);
					strcpy(&g_aFilenameModel[nCntModel][0], pStr);	//���f���̖��O���擾

																	// X�t�@�C���̓ǂݍ���
					D3DXLoadMeshFromX(&g_aFilenameModel[nCntModel][0],
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&m_pBuffMat[nCntModel],
						NULL,
						&m_nNumMat[nCntModel],
						&m_pMesh[nCntModel]);
					nCntModel++;
				}
			}
			else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_CHARASET) == 0)
			{//�L�����N�^�[�ݒu���������s�̏ꍇ
				strcpy(pStr, pStrCur);								//����������o��
				ReadCharacterSet(pStrCur, pFile, pLine);
			}
			else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_MOTIONSET) == 0)
			{//���[�V�������ݒu�������s�̏ꍇ
				if (nCntMotion < MOTION_MAX)
				{
					ReadMotionSet(pStrCur, pFile, pLine, nCntMotion);
					nCntMotion++;
				}
			}
			else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_END_SCRIPT) == 0)
			{//�X�N���v�g�I���������s�̏ꍇ
				break;
			}
		}
		if (pStr != NULL)
		{
			delete[] pStr;
			pStr = NULL;
		}
		if (pLine != NULL)
		{
			delete[] pLine;
			pLine = NULL;
		}
	}
	fclose(pFile);	//�t�@�C�������
}

//=============================================================================
// �L�����N�^�[�ݒu���̓ǂݍ��ݏ���
//=============================================================================
void CPlayer::ReadCharacterSet(char * pStrCur, FILE * pFile, char * pLine)
{
	int nCntParts = 0;
	int nIdx = 0;

	while (1)
	{
		pStrCur = CLoadFileFunction::ReadLine(pFile, pLine);
		if (CLoadFileFunction::Memcmp(pStrCur, TEXT_NUM_PARTS) == 0)
		{//�p�[�c�̑����������s�̏ꍇ
			m_nNumParts = CLoadFileFunction::ReadInt(pStrCur, TEXT_NUM_PARTS);
		}
		else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_PARTSSET) == 0)
		{
			if (nCntParts < m_nNumParts)
			{
				while (1)
				{
					pStrCur = CLoadFileFunction::ReadLine(pFile, pLine);
					if (CLoadFileFunction::Memcmp(pStrCur, TEXT_INDEX) == 0)
					{//�e�L�X�g���C���f�b�N�X�������s�̏ꍇ
						nIdx = CLoadFileFunction::ReadInt(pStrCur, TEXT_INDEX);
					}
					else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_PARENT) == 0)
					{//�e�L�X�g���e�q�֌W�������s�̏ꍇ
						m_nIndexParent[nIdx] = CLoadFileFunction::ReadInt(pStrCur, TEXT_PARENT);
					}
					else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_POS) == 0)
					{//�e�L�X�g���ʒu�������s�̏ꍇ
						m_aKeyOffset[nCntParts].pos = CLoadFileFunction::ReadVector3(pStrCur, TEXT_POS);
					}
					else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_ROT) == 0)
					{//�e�L�X�g���ʒu�������s�̏ꍇ
						m_aKeyOffset[nCntParts].rot = CLoadFileFunction::ReadVector3(pStrCur, TEXT_ROT);

					}
					else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_END_PARTSSET) == 0)
					{//�e�L�X�g���p�[�c�̐ݒu�I���������s�̏ꍇ
						if (m_nIndexParent[nCntParts] == -1)
						{
							//m_apModel[nIdx] = CModel::Create(D3DXVECTOR3(pos.x + m_aKeyOffset[nIdx].pos.x, pos.y + m_aKeyOffset[nIdx].pos.y, pos.z + m_aKeyOffset[nIdx].pos.z));
							m_apModel[nIdx] = CModel::Create(D3DXVECTOR3(m_aKeyOffset[nIdx].pos));
							m_apModel[nIdx]->BindX(m_pMesh[nIdx], m_pBuffMat[nIdx], m_nNumMat[nIdx]);
							m_apModel[nIdx]->SetParent(NULL);
							//m_offsetPos[nIdx] = m_apModel[nIdx]->GetPos();
						}
						else if (m_nIndexParent[nCntParts] > -1)
						{
							//m_apModel[nIdx] = CModel::Create(D3DXVECTOR3(pos.x + m_aKeyOffset[nIdx].pos.x, pos.y + m_aKeyOffset[nIdx].pos.y, pos.z + m_aKeyOffset[nIdx].pos.z));
							m_apModel[nIdx] = CModel::Create(D3DXVECTOR3(m_aKeyOffset[nIdx].pos));
							m_apModel[nIdx]->BindX(m_pMesh[nIdx], m_pBuffMat[nIdx], m_nNumMat[nIdx]);
							m_apModel[nIdx]->SetParent(m_apModel[m_nIndexParent[nIdx]]);
						}
						m_offsetPos[nIdx] = m_apModel[nIdx]->GetPos();
						break;
					}
				}
				nCntParts++;	//�p�[�c�J�E���^�[���Z
			}
		}
		else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_END_CHARASET) == 0)
		{//�L�����N�^�[�ݒu���I���������s�̏ꍇ
			break;
		}
	}
}

//=============================================================================
// ���[�V�����ݒu���̓ǂݍ��ݏ���
//=============================================================================
void CPlayer::ReadMotionSet(char * pStrCur, FILE * pFile, char * pLine, int nCntMotion)
{
	int nCntKeySet = 0;
	int nCntKey = 0;
	while (1)
	{
		pStrCur = CLoadFileFunction::ReadLine(pFile, pStrCur);
		if (CLoadFileFunction::Memcmp(pStrCur, TEXT_NUM_KEY) == 0)
		{//�L�[���������s�̏ꍇ
			m_apMotionInfo[nCntMotion].nNumKey = CLoadFileFunction::ReadInt(pStrCur, TEXT_NUM_KEY);	//�L�[�̑�������
		}
		if (CLoadFileFunction::Memcmp(pStrCur, TEXT_LOOP) == 0)
		{//���[�v�������s�̏ꍇ
			m_apMotionInfo[nCntMotion].bLoop = CLoadFileFunction::ReadBool(pStrCur, TEXT_LOOP);
		}
		else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_COLLISION) == 0)
		{//�����蔻��������s�̏ꍇ
			pStrCur += strlen(TEXT_COLLISION);								//���o��
			pStrCur = CLoadFileFunction::GetLineTop(pStrCur);				//�󔒏���
			int nWord = 0;

			m_aMotionAttack[nCntMotion].nAttackStartFrame = atoi(pStrCur);	//X���W�̑��

			nWord = CLoadFileFunction::PopString(pStrCur, pStrCur);		//�������J�E���g

			pStrCur += nWord;												//�J�E���g���ꂽ�����o��
			m_aMotionAttack[nCntMotion].nAttackEndFrame = atoi(pStrCur);	//X���W�̑��
		}
		else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_KEYSET) == 0)
		{
			if (nCntKeySet < m_apMotionInfo[nCntMotion].nNumKey)
			{//�L�[�ݒu�J�E���^�[���L�[����菬�����ꍇ
				int nCntKey = 0;

				while (1)
				{
					pStrCur = CLoadFileFunction::ReadLine(pFile, pStrCur);

					if (CLoadFileFunction::Memcmp(pStrCur, TEXT_FRAME) == 0)
					{//�t���[�����������s�̏ꍇ
						m_apMotionInfo[nCntMotion].aKeyInfo[nCntKeySet].nFrame = CLoadFileFunction::ReadInt(pStrCur, TEXT_FRAME);	//�t���[���̑��
						m_aMotionAttack[nCntMotion].nNumFrame += m_apMotionInfo[nCntMotion].aKeyInfo[nCntKeySet].nFrame;
					}
					else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_KEY) == 0)
					{//�L�[�̊J�n�������s�̏ꍇ
						if (nCntKey < m_nNumParts)
						{
							while (1)
							{
								pStrCur = CLoadFileFunction::ReadLine(pFile, pLine);	//�s�̓ǂݍ���

								if (CLoadFileFunction::Memcmp(pStrCur, TEXT_POS) == 0)
								{//�ʒu�̏��������s�̏ꍇ
									m_apMotionInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].pos = CLoadFileFunction::ReadVector3(pStrCur, TEXT_POS);

								}
								else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_ROT) == 0)
								{//�����̏��������s�̏ꍇ
									m_apMotionInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].rot = CLoadFileFunction::ReadVector3(pStrCur, TEXT_ROT);
								}
								else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_END_KEY) == 0)
								{//�L�[�̏I���������s�̏ꍇ
									nCntKey++;
									break;
								}
							}
						}
					}
					else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_END_KEYSET) == 0)
					{//�L�[�ݒu���I���������s�̏ꍇ
						nCntKeySet++;
						break;
					}
				}
			}
		}
		else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_END_MOTIONSET) == 0)
		{//���[�V�����ݒu���I���������s�̏ꍇ
			break;
		}
	}
}