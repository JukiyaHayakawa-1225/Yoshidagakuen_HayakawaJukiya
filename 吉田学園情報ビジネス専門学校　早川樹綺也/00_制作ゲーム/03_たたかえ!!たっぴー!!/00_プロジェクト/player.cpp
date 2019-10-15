//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "boxCollider.h"
#include "bullet.h"
#include "enemy.h"
#include "basemode.h"
#include "cameraManager.h"
#include "block.h"
#include "game.h"
#include "motion.h"
#include "goalCylinder.h"
#include "tutorial.h"
#include "title.h"
#include "effectManager.h"
#include "server.h"
#include "sound.h"
#include "river.h"
#include "icefield.h"
#include "meshfield.h"
#include "hinaarare.h"
#include "headquarters.h"
#include "map.h"
#include "modelcreate.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_MOVE                      (2.8f)     // �v���C���[�̈ړ���
#define PLAYER_MOVE_POWERUP              (4.2f)     // �p���[�A�b�v���̃v���C���[�̈ړ���
#define PLAYER_DEATH_EFFECT_IDX          (7)        // ���񂾂Ƃ��̃G�t�F�N�g�ԍ�
#define PLAYER_MOVE_EFFECT_IDX           (15)       // �ړ����Ă��鎞�̃G�t�F�N�g�ԍ�
#define PLAYER_INVINCIBLE_EFFECT_IDX     (16)       // ���G��Ԏ��̃G�t�F�N�g�ԍ�
#define PLAYER_INVINCIBLE_EFFECT_APPEAR  (5)        // ���G��Ԏ��̃G�t�F�N�g���o���Ԋu
#define PLAYER_SE_BULLET_IDX             (6)        // �e���ˎ��̉��ԍ�
#define PLAYER_SE_DEATH_IDX              (18)       // ���񂾂Ƃ��̃G�t�F�N�g�ԍ�
#define PLAYER_SE_DAMAGE_IDX             (12)       // �v���C���[�������Ȃ��Ȃ�U������������Ƃ��̉��ԍ�
#define PLAYER_COL_PRIORITY              (4)        // �R���W�������s���D�揇��
#define PLAYER_INVINCIBLE_TIME           (1200)     // ���G��Ԃ��ێ����鎞��

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �v���C���[�̃R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	m_pPlayer = NULL;			//�v���C���[�̃|�C���^
	m_state = STATE_NOMAL;		//���
	m_nCntState = 0;			//��ԃJ�E���^�[
	m_nMaxBullet = 0;			//�ő�e��
	m_nCntAbility = 0;			//�\�̓J�E���^�[
	m_bAllBlockDestroy = false;	//�S�Ẵu���b�N���󂹂邩�ǂ���
	m_bSplash = false;			//����Ă��邩�ǂ���
	m_nCntSplash = 0;			//����J�E���^�[
	m_motion = MOTION_NEUTAL;	//���[�V�������
	m_nCntBullet = 0;
}

//=============================================================================
// �v���C���[�̃R���X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}
//=============================================================================
// �v���C���[�̐���
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPlayerIdx, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, int nPriority)
{
	CPlayer *pPlayer = NULL;				//�v���C���[�̃|�C���^

	if (pPlayer == NULL)
	{//NULL�̏ꍇ
		pPlayer = new CPlayer(nPriority);	//���I�m��
		if (pPlayer != NULL)
		{//NULL�łȂ��ꍇ
			pPlayer->SetPos(pos);										//�ʒu�̐ݒu����
			pPlayer->SetRot(rot);										//�����̐ݒu����
			pPlayer->SetModel(apModel);									//���f���̐ݒu����
			pPlayer->SetMotionManager(pMotionManager);					//���[�V�����}�l�[�W���[�̐ݒu����
			pPlayer->SetNumPart(nNumParts);								//�p�[�c�����̐ݒu����
			pPlayer->SetColRange(D3DXVECTOR3(60.0f, 60.0f, 60.0f));		//�����蔻��̐ݒu����
			pPlayer->SetPlayerIdx(nPlayerIdx);							//�v���C���[�ԍ��̐ݒu����
			pPlayer->Init();											//����������
			pPlayer->SetPlayer(pPlayer);								//�v���C���[�|�C���^�̐ݒu����
		}
	}
	return pPlayer;
}
//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(void)
{
	//�ϐ��̃N���A����
	ClearVariable();

	//����������
	CCharacter::Init();

	SetAccel(PLAYER_MOVE);
	SetInertia(0.8f);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		if (CManager::GetGame() != NULL)
		{
			//�\�͂̏�����
		/*	m_nCntAbility = 0;
			m_ability = PLAYER_ABILITY_NOMAL;*/
			CManager::GetGame()->DeletePlayer(NULL, m_nPlayerIdx);
		}
	}
	if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		if (CManager::GetTutorial() != NULL)
		{
			CManager::GetTutorial()->DeletePlayer(NULL, m_nPlayerIdx);
		}
	}

	//�I������
	CCharacter::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	//�ߋ��̈ʒu�̐ݒu
	SetPosOld(GetPos());

	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂ�������
		// ���͂ɂ��ړ��ʌv�Z����
		InputMove_Game();

		//�ړ�����
		Move();

		if (GetMotionManager() != NULL)
		{//���[�V�����}�l�[�W���[��NULL�ł͂Ȃ��ꍇ
			GetMotionManager()->Update(GetModel());
		}

		//�����蔻��̏���
		Collision();
	}
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		// ���͂ɂ��ړ��ʌv�Z����
		InputMove_Tutorial();

		//�ړ�����
		Move();

		if (GetMotionManager() != NULL)
		{//���[�V�����}�l�[�W���[��NULL�ł͂Ȃ��ꍇ
			GetMotionManager()->Update(GetModel());
		}

		//�����蔻��̏���
		Collision();
	}

	// ��Ԃ̏���
	State();

}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	//�����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL) { return; }

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();
	if (pDevice == NULL) { return; }

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetMtxWorld(pDevice);

	// ���C�g�𖾂邭����
	D3DLIGHT9 Light;
	Light.Position = D3DXVECTOR3(GetPos().x, GetPos().y + 70.0f, GetPos().z);
	Light.Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Light.Attenuation0 = 0.2f;
	Light.Attenuation1 = 0.03f;
	Light.Attenuation2 = 0.0f;
	Light.Range = 500.0f;
	Light.Falloff = 1.0f;
	Light.Theta = D3DX_PI * 0.5f;
	Light.Phi = D3DX_PI * 2.0f;
	Light.Type = D3DLIGHT_SPOT;
	pDevice->SetLight(10, &Light);
	pDevice->LightEnable(10, true);

	if (m_state == STATE_RESPAWN || m_state == STATE_STOP)
	{//���X�|�[����Ԃ܂��͒�~��Ԃ̏ꍇ
		if (m_nCntState % 2 == 0)
		{
			//�L�����N�^�[�̕`�揈��
			CCharacter::Draw();
		}
	}
	else
	{
		//�L�����N�^�[�̕`�揈��
		CCharacter::Draw();
	}

	pDevice->LightEnable(10, false);

#ifdef _DEBUG
	//�I�u�W�F�N�g3D�̕`�揈��
	CObject3D::Draw();
#endif // !_DEBUG
}

//=============================================================================
// ���������Ƃ��̏���
//=============================================================================
void CPlayer::Hit(CScene *pScene)
{
	// ���������I�u�W�F�N�g�ɂ���ď����킯
	if (pScene->GetObjType() == OBJTYPE_BULLET && m_bHelmet == false)
	{// �e�����������G��ԂłȂ�
		CBullet *pBullet = (CBullet*)pScene;
		if (pBullet->GetType() == CBullet::TYPE_ENEMY)
		{// �G�̒e������
			// �G�t�F�N�g���o��
			SetDeathEffect();

			// �I������
			Uninit();
		}
		else if(pBullet->GetType() != m_nPlayerIdx)
		{// �Ⴄ�v���C���[�̒e������
			m_state = STATE_STOP;
			if (CManager::GetMode() == CManager::MODE_GAME)
			{// �Q�[����ʂ�������
				CGame *pGame = CManager::GetGame();
				if (pGame == NULL) { return; }
				pGame->HitBullet();
				pGame->AddJammer((int)pBullet->GetType());
			}
			else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
			{// �`���[�g���A����ʂ�������
				CTutorial *pTutorial = CManager::GetTutorial();
				if (pTutorial == NULL) { return; }
			}
		}
	}
}

//=============================================================================
// �v���C���[�̖��G��Ԃ̃G�t�F�N�g��������
//=============================================================================
void CPlayer::SetInvincibleEffect(void)
{
	D3DXVECTOR3 EffectPos = GetPos();
	CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
	if (pEffectManager != NULL)
	{
		pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, PLAYER_INVINCIBLE_EFFECT_IDX);
	}
}

//=============================================================================
// �v���C���[�̎��񂾂Ƃ��̃G�t�F�N�g��������
//=============================================================================
void CPlayer::SetDeathEffect(void)
{
	D3DXVECTOR3 EffectPos = GetPos();
	CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
	if (pEffectManager != NULL)
	{
		pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, PLAYER_DEATH_EFFECT_IDX);
	}

	// ����炷
	CManager::GetSound()->PlaySound(PLAYER_SE_DEATH_IDX);
}

//=============================================================================
// �v���C���[���ړ��������̃G�t�F�N�g��������
//=============================================================================
void CPlayer::SetMoveEffect(void)
{
	D3DXVECTOR3 EffectPosDef = GetPos();
	D3DXVECTOR3 EffectPos = EffectPosDef;
	D3DXVECTOR3 EffectRot = GetRot();
	CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
	if (pEffectManager != NULL)
	{
		EffectPos.x += sinf(EffectRot.y - (D3DX_PI * 0.5f)) * 20.0f + sinf(EffectRot.y) * 30.0f;
		EffectPos.z += cosf(EffectRot.y - (D3DX_PI * 0.5f)) * 20.0f + cosf(EffectRot.y) * 30.0f;
		pEffectManager->SetEffect(EffectPos, EffectRot, PLAYER_MOVE_EFFECT_IDX);
		EffectPos = EffectPosDef;
		EffectPos.x -= sinf(EffectRot.y - (D3DX_PI * 0.5f)) * 20.0f - sinf(EffectRot.y) * 30.0f;
		EffectPos.z -= cosf(EffectRot.y - (D3DX_PI * 0.5f)) * 20.0f - cosf(EffectRot.y) * 30.0f;
		pEffectManager->SetEffect(EffectPos, EffectRot, PLAYER_MOVE_EFFECT_IDX);
	}
}

//=============================================================================
// �v���C���[���A�C�e�����擾���̏���
//=============================================================================
void CPlayer::SwitchItem(CItem *pItem)
{
	//�}�b�v�̎擾
	CMap *pMap = CManager::GetBaseMode()->GetMap();

	//�A�C�e���̎�ޕʏ���
	switch (pItem->GetType())
	{
	case CItem::TYPE_STAR:	//�X�^�[�̏ꍇ
		break;

	case CItem::TYPE_GRENADE: //�O���l�[�h�̏ꍇ
		break;

	case CItem::TYPE_1UP_TANK: //1UP�̏ꍇ
		break;

	case CItem::TYPE_SCOOP: //�X�R�b�v�̏ꍇ
		if (pMap != NULL)
		{
			//�i�ߕ��̎擾
			CHeadQuarters *pHeadQuarters = pMap->GetHeadQuarters();

			int nVirtical = 0; //�c�̔z�u���̏��

			for (int nCntSide = 0; nCntSide < 6; nCntSide++)
			{//
				if (nCntSide <= 1 || nCntSide >= 4)
				{//1�ȉ��܂���4�ȏ�̏ꍇ
					nVirtical = 4;
				}
				else if (nCntSide >= 2 || nCntSide <= 3)
				{//2�ȉ��܂���3�ȏ�̏ꍇ
					nVirtical = 2;
				}

				for (int nCntVirtical = 0; nCntVirtical < nVirtical; nCntVirtical++)
				{//�u���b�N�̐���
					CBlockScoop::Create(D3DXVECTOR3(-93.8f + (MASS_SIZE_X_HALF * nCntSide), 30.0f, -431.3f - (MASS_SIZE_Z_HALF * nCntVirtical)), INITIALIZE_D3DXVECTOR3,
						0, 0, pMap->GetModelCreate()->GetMesh(14), pMap->GetModelCreate()->GetBuffMat(14), pMap->GetModelCreate()->GetNumMat(14), pMap->GetModelCreate()->GetTexture(14)
						, MASS_SIZE_X_HALF, MASS_SIZE_Z_HALF * 2, MASS_SIZE_Z_HALF);
				}
			}
		}
		break;

	case CItem::TYPE_CLOCK:	//���v�̏ꍇ
		break;

	case CItem::TYPE_HELMET: //�w�����b�g�̏ꍇ
		m_bHelmet = true;
		break;
	}
}

//=============================================================================
// �v���C���[�̐ݒu����
//=============================================================================
void CPlayer::SetPlayer(CPlayer *pPlayer)
{
	m_pPlayer = pPlayer;
}

//=============================================================================
// �v���C���[�ԍ��̐ݒu����
//=============================================================================
void CPlayer::SetPlayerIdx(int nPlayerIdx)
{
	m_nPlayerIdx = nPlayerIdx;
}

//=============================================================================
// �v���C���[��Ԃ̐ݒu����
//=============================================================================
void CPlayer::SetState(STATE state)
{
	m_state = state;
}

//=============================================================================
// �v���C���[��ԊǗ��J�E���^�[�̐ݒu����
//=============================================================================
void CPlayer::SetStateCounter(int nStateCounter)
{
	m_nCntState = nStateCounter;
}

//=============================================================================
// �v���C���[�e�ő吔�̐ݒu����
//=============================================================================
void CPlayer::SetMaxBullet(int nMaxBullet)
{
	m_nMaxBullet = nMaxBullet;
}

//=============================================================================
// �v���C���[����Ă��邩�ǂ����ݒu����
//=============================================================================
void CPlayer::SetSplash(bool bSplash)
{
	m_bSplash = bSplash;
}

//=============================================================================
// �S�Ẵu���b�N���󂹂邩�ǂ����̐ݒu����
//=============================================================================
void CPlayer::SetAllBlockDestroy(bool bAllBlockDestroy)
{
	m_bAllBlockDestroy = bAllBlockDestroy;
}

//=============================================================================
// �w�����b�g���g�p���Ă��邩�ǂ����̐ݒu����
//=============================================================================
void CPlayer::SetHelmet(bool bHelmet)
{
	m_bHelmet = bHelmet;
}

//=============================================================================
// �v���C���[�̎擾����
//=============================================================================
CPlayer *CPlayer::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// �v���C���[�ԍ��̎擾����
//=============================================================================
int CPlayer::GetPlayerIdx(void)
{
	return m_nPlayerIdx;
}

//=============================================================================
// �v���C���[��Ԃ̎擾����
//=============================================================================
CPlayer::STATE CPlayer::GetState(void)
{
	return m_state;
}

//=============================================================================
// �v���C���[����Ă��邩�ǂ����̎擾����
//=============================================================================
bool CPlayer::GetSplash(void)
{
	return m_bSplash;
}

//=============================================================================
// �v���C���[�e�ő吔�̎擾����
//=============================================================================
int CPlayer::GetMaxBullet(void)
{
	return m_nMaxBullet;
}

//=============================================================================
// �S�Ẵu���b�N���󂹂邩�ǂ����̎擾����
//=============================================================================
bool CPlayer::GetAllBlockDestroy(void)
{
	return m_bAllBlockDestroy;
}

//=============================================================================
// �w�����b�g���g�p���Ă��邩�ǂ����擾����
//=============================================================================
bool CPlayer::GetHelmet(void)
{
	return m_bHelmet;
}

//=============================================================================
// �Q�[����ʂ̓��͈ړ�����
//=============================================================================
void CPlayer::InputMove_Game(void)
{
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// �I�����C���v���C��������
		if (m_nPlayerIdx == CManager::GetClient()->GetClientId())
		{
			InputMove();
		}
	}
	else
	{// �I�����C���v���C����Ȃ�������
		InputMove();
	}
}

//=============================================================================
// �`���[�g���A����ʂ̓��͈ړ�����
//=============================================================================
void CPlayer::InputMove_Tutorial(void)
{
	CTutorial *pTutorial = CManager::GetTutorial();
	if (pTutorial == NULL) { return; }

	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// �I�����C���v���C��������
		if (m_nPlayerIdx == CManager::GetClient()->GetClientId() && pTutorial->GetState() != CTutorial::STATE_END)
		{
			InputMove();
		}
	}
	else
	{// �I�����C���v���C����Ȃ�������
		if (pTutorial->GetState() != CTutorial::STATE_END)
		{
			InputMove();
		}
	}
}

//=============================================================================
// ���͈ړ�����
//=============================================================================
void CPlayer::InputMove(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 rot = CCharacter::GetRot();
	D3DXVECTOR3 move = CCharacter::GetMove();
	float fDiffAngle = 0.0f;
	bool bMoving = false;
	float fAccel = GetAccel();

	// �L�[�{�[�h�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	// �R���g���[���[�̎擾
	CXInput *pXInput = CManager::GetXInput();

	// �A�N�V��������
	InputAction(pInputKeyboard, pXInput);

	// �����Ȃ���ԂȂ珈�����Ȃ�
	if (m_state == STATE_STOP) { return; }

	// �L�[�{�[�h�ł̈ړ�����
	bMoving = InputMove_Keyboard(&move, &fDiffAngle, rot, fAccel, pInputKeyboard);
	if (bMoving == false)
	{
		// �R���g���[���[�ł̈ړ�����
		bMoving = InputMove_Controller(&move, &fDiffAngle, rot, fAccel, pXInput);
	}

	// �e��l�̐ݒ�
	SetMove(move);
	SetDiffAngle(fDiffAngle);
}

//=============================================================================
// ���̓A�N�V��������
//=============================================================================
void CPlayer::InputAction(CInputKeyboard *pKeyboard, CXInput *pXInput)
{
	// �e���˂̏���
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// �I�����C���v���C�Ȃ��
		if (pKeyboard->GetTrigger(DIK_RETURN) == true ||
			pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
		{// �e���˃{�^������
			CreateBullet();
		}
	}
	else if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL2P && m_nPlayerIdx == 0)
	{// 0�Ԃ̃v���C���[�Ȃ��
		if (pKeyboard->GetTrigger(DIK_RETURN) == true ||
			pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
		{// �e���˃{�^������
			CreateBullet();
		}
	}
	else if(CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL2P && m_nPlayerIdx != 0)
	{// 1�Ԃ̃v���C���[�Ȃ��
		if (pKeyboard->GetTrigger(DIK_NUMPADENTER) == true ||
			pXInput->GetTrigger(m_nPlayerIdx, CXInput::XIJS_BUTTON_11) == true)
		{// �e���˃{�^������
			CreateBullet();
		}
	}
	else
	{// 1�l�v���C�Ȃ�
		if (pKeyboard->GetTrigger(DIK_RETURN) == true ||
			pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
		{// �e���˃{�^������
			CreateBullet();
		}
	}
}

//=============================================================================
// �L�[�{�[�h�̓��͈ړ�����
//=============================================================================
bool CPlayer::InputMove_Keyboard(D3DXVECTOR3 *pMove, float *pDiffAngle, D3DXVECTOR3 rot, float fAccel, CInputKeyboard *pKeyboard)
{
	if (m_nPlayerIdx == 0 || CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// 0�Ԃ̃v���C���[�Ȃ��
		if (pKeyboard->GetPress(DIK_W) == true)
		{// ��ړ��{�^������
			pMove->z = fAccel;
			*pDiffAngle = (D3DX_PI)-rot.y;
			SetNowRotInfo(ROT_UP);	//���݂̌�������ɂ���
			SetMoveEffect();
			return true;
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// ���ړ��{�^������
			pMove->z = -fAccel;
			*pDiffAngle = (D3DX_PI * 0) - rot.y;
			SetNowRotInfo(ROT_DOWN);	//���݂̌��������ɂ���
			SetMoveEffect();
			return true;
		}
		else if (pKeyboard->GetPress(DIK_A) == true)
		{// ���ړ��{�^������
			pMove->x = -fAccel;
			*pDiffAngle = (D3DX_PI * 0.5f) - rot.y;
			SetNowRotInfo(ROT_LEFT);	//���݂̌��������ɂ���
			SetMoveEffect();
			return true;
		}
		else if (pKeyboard->GetPress(DIK_D) == true)
		{// �E�ړ��{�^������
			pMove->x = fAccel;
			*pDiffAngle = (D3DX_PI * -0.5f) - rot.y;
			SetNowRotInfo(ROT_RIGHT);	//���݂̌������E�ɂ���
			SetMoveEffect();
			return true;
		}
	}
	else
	{// 1�Ԃ̃v���C���[�Ȃ��
		if (pKeyboard->GetPress(DIK_UP) == true)
		{// ��ړ��{�^������
			pMove->z = fAccel;
			*pDiffAngle = (D3DX_PI)-rot.y;
			SetNowRotInfo(ROT_UP);	//���݂̌�������ɂ���
			SetMoveEffect();
			return true;
		}
		else if (pKeyboard->GetPress(DIK_DOWN) == true)
		{// ���ړ��{�^������
			pMove->z = -fAccel;
			*pDiffAngle = (D3DX_PI * 0) - rot.y;
			SetNowRotInfo(ROT_DOWN);	//���݂̌��������ɂ���
			SetMoveEffect();
			return true;
		}
		else if (pKeyboard->GetPress(DIK_LEFT) == true)
		{// ���ړ��{�^������
			pMove->x = -fAccel;
			*pDiffAngle = (D3DX_PI * 0.5f) - rot.y;
			SetNowRotInfo(ROT_LEFT);	//���݂̌��������ɂ���
			SetMoveEffect();
			return true;
		}
		else if (pKeyboard->GetPress(DIK_RIGHT) == true)
		{// �E�ړ��{�^������
			pMove->x = fAccel;
			*pDiffAngle = (D3DX_PI * -0.5f) - rot.y;
			SetNowRotInfo(ROT_RIGHT);	//���݂̌������E�ɂ���
			SetMoveEffect();
			return true;
		}
	}

	return false;
}

//=============================================================================
// �R���g���[���[�̓��͈ړ�����
//=============================================================================
bool CPlayer::InputMove_Controller(D3DXVECTOR3 *pMove, float *pDiffAngle, D3DXVECTOR3 rot, float fAccel, CXInput *pXInput)
{
	int nPlayerController = m_nPlayerIdx;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{
		nPlayerController = 0;
	}

	if (pXInput->GetPress(nPlayerController, CXInput::XIJS_BUTTON_0) == true ||
		pXInput->GetPress(nPlayerController, CXInput::XIJS_BUTTON_16) == true)
	{// ��ړ��{�^������
		pMove->z = fAccel;
		*pDiffAngle = (D3DX_PI)-rot.y;
		SetNowRotInfo(ROT_UP);	//���݂̌�������ɂ���
		SetMoveEffect();
		return true;
	}
	else if (pXInput->GetPress(nPlayerController, CXInput::XIJS_BUTTON_1) == true ||
		pXInput->GetPress(nPlayerController, CXInput::XIJS_BUTTON_17) == true)
	{// ���ړ��{�^������
		pMove->z = -fAccel;
		*pDiffAngle = (D3DX_PI * 0) - rot.y;
		SetNowRotInfo(ROT_DOWN);	//���݂̌��������ɂ���
		SetMoveEffect();
		return true;
	}
	else if (pXInput->GetPress(nPlayerController, CXInput::XIJS_BUTTON_2) == true ||
		pXInput->GetPress(nPlayerController, CXInput::XIJS_BUTTON_18) == true)
	{// ���ړ��{�^������
		pMove->x = -fAccel;
		*pDiffAngle = (D3DX_PI * 0.5f) - rot.y;
		SetNowRotInfo(ROT_LEFT);	//���݂̌��������ɂ���
		SetMoveEffect();
		return true;
	}
	else if (pXInput->GetPress(nPlayerController, CXInput::XIJS_BUTTON_3) == true ||
		pXInput->GetPress(nPlayerController, CXInput::XIJS_BUTTON_19) == true)
	{// �E�ړ��{�^������
		pMove->x = fAccel;
		*pDiffAngle = (D3DX_PI * -0.5f) - rot.y;
		SetNowRotInfo(ROT_RIGHT);	//���݂̌������E�ɂ���
		SetMoveEffect();
		return true;
	}

	return false;
}

//=============================================================================
// �ړ�����
//=============================================================================
void CPlayer::Move(void)
{
	//�ړ��ʂ̎擾
	D3DXVECTOR3 move = CCharacter::GetMove();

	//�ʒu�̎擾
	D3DXVECTOR3 pos = CCharacter::GetPos();

	//�����̎擾
	D3DXVECTOR3 rot = CCharacter::GetRot();

	//�����ʂ̎擾
	float fInertia = CCharacter::GetInertia();

	//�����蔻�蔠�̎擾����
	CBoxCollider *pCollider = NULL;
	pCollider = CObject3D::GetBoxCollider();

	//�����蔻�蔠�̈ʒu�ݒu����
	if (pCollider != NULL)
	{
		pCollider->SetPos(pos);
	}

	//�ʒu�̉��Z����
	pos += move;

	//�����̏���
	if (GetNowRotInfo() == ROT_UP ||
		GetNowRotInfo() == ROT_DOWN)
	{//�������O��̏ꍇ
		move.x = 0.0f;							//�΂߈ړ��������Ȃ����߂�x���̈ړ��ʂ�0�ɂ���
		move.z += (0.0f - move.z) * fInertia;	//z���̈ړ��ʉ��Z����
	}
	else if (GetNowRotInfo() == ROT_RIGHT ||
		GetNowRotInfo() == ROT_LEFT)
	{
		move.z = 0.0f;							//�΂߈ړ��������Ȃ����߂�z���̈ړ��ʂ�0�ɂ���
		move.x += (0.0f - move.x) * fInertia;	//x���̈ړ��ʉ��Z����
	}

	//�͈͊O�ɍs�����ꍇ���Ƃɖ߂�
	if (pos.x > ((MASS_SIZE_X * MASS_BLOCK_X) / 2) - 35.0f)
	{
		pos.x = ((MASS_SIZE_X * MASS_BLOCK_X) / 2) - 35.0f;
	}
	if (pos.x < -((MASS_SIZE_X * MASS_BLOCK_X) / 2) + 35.0f)
	{
		pos.x = -((MASS_SIZE_X * MASS_BLOCK_X) / 2) + 35.0f;
	}
	if (pos.z >((MASS_SIZE_Z * MASS_BLOCK_Z) / 2) - 35.0f)
	{
		pos.z = ((MASS_SIZE_Z * MASS_BLOCK_Z) / 2) - 35.0f;
	}
	if (pos.z < -((MASS_SIZE_Z * MASS_BLOCK_Z) / 2) + 35.0f)
	{
		pos.z = -((MASS_SIZE_Z * MASS_BLOCK_Z) / 2) + 35.0f;
	}

	if (move.x <= 0.1f && move.x >= -0.1f
		&& move.z <= 0.1f && move.z >= -0.1f)
	{// �ړ����Ă��Ȃ�
		if (m_motion == MOTION_MOVE)
		{
			m_motion = MOTION_NEUTAL;
			// ���[�V�����؂�ւ�����
			if (GetMotionManager() != NULL)
			{
				GetMotionManager()->SwitchMotion(GetModel(), MOTION_NEUTAL);
			}
		}
	}
	else
	{// �ړ����Ă���
		if (m_motion != MOTION_MOVE)
		{
			m_motion = MOTION_MOVE;
			// ���[�V�����؂�ւ�����
			if (GetMotionManager() != NULL)
			{
				GetMotionManager()->SwitchMotion(GetModel(), MOTION_MOVE);
			}
		}
	}

	//�ʒu�̐ݒu����
	SetPos(pos);

	//�����ݒu����
	SetRot(rot);

	//�ړ��̐ݒu����
	SetMove(move);
}

//=============================================================================
// �����蔻�菈��
//=============================================================================
void CPlayer::Collision(void)
{
	//�ߋ��̈ʒu�̎擾
	D3DXVECTOR3 posOld = GetPosOld();

	//�ړ��ʂ̎擾����
	D3DXVECTOR3 move = GetMove();

	//�����蔻��̑傫���̎擾
	D3DXVECTOR3 colRange = GetColRange();

	//�ʒu�̎擾
	D3DXVECTOR3 pos = GetPos();

	CScene *pScene = NULL;               // �I�u�W�F�N�g�̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃I�u�W�F�N�g�̃|�C���^
	bool bLand = false;					 // ���n���Ă��邩�ǂ���
	bool bIceLand = false;				 // �X�̏�ɂ��邩�ǂ���

	for (int nCntPriority = 0; nCntPriority < PLAYER_COL_PRIORITY + 1; nCntPriority++)
	{
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULL�ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_3D)
			{// �I�u�W�F�N�g3D��������
				bLand = CollisionObject3D(&pos, &posOld, &move, colRange, (CObject3D*)pScene);
			}
			else if (pScene->GetObjType() == OBJTYPE_BULLET)
			{// �e��������
				bLand = CollisionBullet(&pos, &posOld, &move, colRange / 2, (CBullet*)pScene);
			}
			else if (pScene->GetObjType() == OBJTYPE_ENEMY)
			{// �G��������
				bLand = CollisionEnemy(&pos, &posOld, &move, colRange / 2, (CEnemy*)pScene);
			}
			else if (pScene->GetObjType() == OBJTYPE_BLOCK)
			{// �u���b�N��������
					bLand = CollisionBlock(&pos, &posOld, &move, colRange / 2, (CBlock*)pScene);
			}
			else if (pScene->GetObjType() == OBJTYPE_ITEM)
			{// �A�C�e����������
				CItem *pItem = (CItem*)pScene;

				if (CollisionItem(&pos, &posOld, &move, colRange, pItem) == true)
				{// �A�C�e���ɓ������Ă���
					bLand = true;

					//�A�C�e���擾���̏���
					SwitchItem(pItem);

					//�R���N�^�[�̉��Z
					CManager::GetGame()->AddCollecter(m_nPlayerIdx);
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_GOALCYLINDER)
			{// �S�[���~����������
				CollisionGoalCylinder(&pos, (CGoalCylinder*)pScene);
			}
			else if (pScene->GetObjType() == OBJTYPE_RIVER)
			{// �삾������
				bLand = CollisionRiver(&pos, &posOld, &move, colRange, (CRiver*)pScene);
			}
			else if (pScene->GetObjType() == OBJTYPE_ICEFIELD)
			{// �X��������
				CollisionIceField(pos, (CIceField*)pScene, &bIceLand);
			}
			else if (pScene->GetObjType() == OBJTYPE_HEADQUARTERS)
			{// �i�ߕ���������
				bLand = CollisionHeadQuarters(&pos, &posOld, &move, colRange, (CHeadQuarters*)pScene);
			}
			else if (pScene->GetObjType() == OBJTYPE_HINAARARE)
			{// �ЂȂ���ꂾ������
				bLand = CollisionHinaarare(&pos, &posOld, &move, colRange, (CHinaarare*)pScene);
			}
			// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
			pScene = pSceneNext;
		}
	}
	switch (bLand)
	{
	case false:
		break;
	case true:
		break;
	}

	//�ʒu�̐ݒu����
	SetPos(pos);
	//�ߋ��̐ݒu����
	SetPosOld(posOld);
	//�ړ��ʂ̐ݒu����
	SetMove(move);
}

//=============================================================================
// ��Ԃ̏���
//=============================================================================
void CPlayer::State(void)
{
	// ���G��Ԏ��̏���
	if (m_bHelmet == true)
	{
		// �G�t�F�N�g���o��
		if (m_nCntHelmet % PLAYER_INVINCIBLE_EFFECT_APPEAR == 0)
		{
			SetInvincibleEffect();
		}

		// �J�E���^�[����
		m_nCntHelmet++;
		if (m_nCntHelmet >= PLAYER_INVINCIBLE_TIME)
		{
			m_bHelmet = false;
			m_nCntHelmet = 0;
		}
	}

	switch (m_state)
	{
	case STATE_NOMAL:	//�ʏ���

		break;
	case STATE_STOP:	//��~���
		m_nCntState++;	//��ԃJ�E���^�[�̉��Z
		if (m_nCntState == 1)
		{// ��~��ԂɂȂ��Ă��炷���̎��ɉ���炷
			CManager::GetSound()->PlaySound(PLAYER_SE_DAMAGE_IDX);
		}
		if (m_nCntState >= 80)
		{//��ԃJ�E���^�[��120�ȏ�̏ꍇ
			m_state = STATE_NOMAL;	//�ʏ��Ԃɖ߂�
			m_nCntState = 0;		//��ԃJ�E���^�[�̏�����
		}
		break;
	case STATE_RESPAWN: //���X�|�[�����
		m_nCntState++;	//��ԃJ�E���^�[�̉��Z
		if (m_nCntState >= 120)
		{//��ԃJ�E���^�[��120�ȏ�̏ꍇ
			m_state = STATE_NOMAL;	//�ʏ��Ԃɖ߂�
			m_nCntState = 0;		//��ԃJ�E���^�[�̏�����
		}
		break;
	case STATE_DEATH:	//���S���

		break;
	}
}

//=============================================================================
// �ړI�̌����ݒu����
//=============================================================================
void CPlayer::SetDiffAngle(float fDiffAngle)
{
	//�����̎擾
	D3DXVECTOR3 rot = CCharacter::GetRot();

	if (fDiffAngle > D3DX_PI)
	{
		fDiffAngle -= D3DX_PI * 2.0f;
	}
	if (fDiffAngle < -D3DX_PI)
	{
		fDiffAngle += D3DX_PI * 2.0f;
	}
	rot.y += fDiffAngle * 0.8f;
	if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}

	//�����̐ݒu����
	SetRot(rot);
}

//=============================================================================
// �e�̐���
//=============================================================================
void CPlayer::CreateBullet(void)
{
	//�ʒu�̎擾
	D3DXVECTOR3 pos = CCharacter::GetPos();

	//�����̎擾
	D3DXVECTOR3 rot = CCharacter::GetRot();

	// �e��ł��Ă��邩�擾
	bool bShoot = GetShoot();

	//�e�̎�ނ̐؂�ւ�����
	CBullet::TYPE type;
	switch (m_nPlayerIdx)
	{
	case 0:
		type = CBullet::TYPE_PLAYER_0;
		break;
	case 1:
		type = CBullet::TYPE_PLAYER_1;
		break;
	}


	int nGetCntBullet = GetCntBullet();
	if (nGetCntBullet <= m_nMaxBullet)
	{
		// ����炷
		CManager::GetSound()->PlaySound(PLAYER_SE_BULLET_IDX);

		// �ړ��ʂ�ݒ�
		D3DXVECTOR3 BulletMove;

		if (m_ability == PLAYER_ABILITY_NOMAL)
		{
			switch (GetNowRotInfo())
			{
			case ROT_UP:	//��������Ă���ꍇ
				BulletMove = D3DXVECTOR3(0.0f, 0.0f, 8.0f);
				break;
			case ROT_DOWN:	//���������Ă���ꍇ
				BulletMove = D3DXVECTOR3(0.0f, 0.0f, -8.0f);
				break;
			case ROT_RIGHT:	//�E�������Ă���ꍇ
				BulletMove = D3DXVECTOR3(8.0f, 0.0f, 0.0f);
				break;
			case ROT_LEFT:	//���������Ă���ꍇ
				BulletMove = D3DXVECTOR3(-8.0f, 0.0f, 0.0f);
				break;
			}
		}
		else
		{
			switch (GetNowRotInfo())
			{
			case ROT_UP:	//��������Ă���ꍇ
				BulletMove = D3DXVECTOR3(0.0f, 0.0f, 15.0f);
				break;
			case ROT_DOWN:	//���������Ă���ꍇ
				BulletMove = D3DXVECTOR3(0.0f, 0.0f, -15.0f);
				break;
			case ROT_RIGHT:	//�E�������Ă���ꍇ
				BulletMove = D3DXVECTOR3(15.0f, 0.0f, 0.0f);
				break;
			case ROT_LEFT:	//���������Ă���ꍇ
				BulletMove = D3DXVECTOR3(-15.0f, 0.0f, 0.0f);
				break;
			}
		}

		// �e�𐶐�
		m_pBulletPlayer[nGetCntBullet] = CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, BulletMove, type, this);

		//�e�̐��ݒu����
		SetCntBullet(nGetCntBullet + 1);
	}
}

//=============================================================================
// �\�͂̐؂�ւ�����
//=============================================================================
void CPlayer::SwitchAbility(void)
{
	if (m_nCntAbility < PLAYER_ABILITY_MAX)
	{//�\�̓J�E���^�[���\�͂��ő�܂ŒB���Ă��Ȃ��ꍇ
		m_nCntAbility++;
	}

	// �A�r���e�B�㏸
	m_ability = (PLAYER_ABILITY)m_nCntAbility;

	switch (m_ability)
	{
	case PLAYER_ABILITY_NOMAL:
		break;
	case PLAYER_ABILITY_BULLET_SPEEDUP:
		break;
	case PLAYER_ABILITY_MOVE_SPEEDUP:
		SetAccel(PLAYER_MOVE_POWERUP);
		break;
	case PLAYER_ABILITY_DOUBLEBULLET:
		m_nMaxBullet = PLAYER_MAX_BULLET - 1;	//�ő�e�����Q���ɂ���
		break;
	case PLAYER_ABILITY_ALLBLOCKDESTROY:
		m_bAllBlockDestroy = true;
		break;
	}
}

//=============================================================================
// �ϐ����N���A���鏈��
//=============================================================================
void CPlayer::ClearVariable(void)
{
	m_pPlayer = NULL;			//�v���C���[�̃|�C���^
	m_state = STATE_NOMAL;		//���
	m_nCntState = 0;			//��ԃJ�E���^�[
	m_nMaxBullet = 0;			//�ő�e��
	m_nCntAbility = 0;			//�\�̓J�E���^�[
	m_bAllBlockDestroy = false;	//�S�Ẵu���b�N���󂹂邩�ǂ���
	m_bSplash = false;			//����Ă��邩�ǂ���
	m_nCntSplash = 0;			//����J�E���^�[
	m_motion = MOTION_NEUTAL;	//���[�V�������
	m_ability = PLAYER_ABILITY_NOMAL;	//�\�͂̏��
	m_bHelmet = false;			//�w�����b�g���g�p���Ă��邩�ǂ���
	m_nCntHelmet = 0;			//�w�����b�g�J�E���^�[
}

//=============================================================================
// �I�u�W�F�N�g3D�����蔻��̏���
//=============================================================================
bool CPlayer::CollisionObject3D(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CObject3D *pObject3D)
{
	bool bLand = false;

	CBoxCollider *pBoxCollider = pObject3D->GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		if (pBoxCollider->Collision(pPos,pPosOld,pMove,colRange) == true)
		{
			bLand = true;
		}
	}
	return bLand;
}

//=============================================================================
// �e�����蔻��̏���
//=============================================================================
bool CPlayer::CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBullet *pBullet)
{
	bool bLand = false;
	if (CTitle::GetGameMode() != CTitle::GAMEMODE_ONLINE2P) { return false; }

	CBoxCollider *pBoxCollider = pBullet->GetBoxCollider();
	if (pBoxCollider == NULL) { return false; }

	int nBulletType = pBullet->GetType();
	if (nBulletType != CBulletPlayer::TYPE_PLAYER_0 && nBulletType != CBulletPlayer::TYPE_PLAYER_1) { return false; }


	if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange) == true)
	{
		if (nBulletType != m_nPlayerIdx)
		{// �����ȊO����������ނ̒e�ɓ�������
			Hit(pBullet);
		}
		bLand = true;
	}

	return bLand;
}

//=============================================================================
// �G�����蔻��̏���
//=============================================================================
bool CPlayer::CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CEnemy *pEnemy)
{
	bool bLand = false;

	CBoxCollider *pBoxCollider = pEnemy->GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange) == true)
		{
			bLand = true;
		}
	}
	return bLand;
}

//=============================================================================
// �u���b�N�����蔻��̏���
//=============================================================================
bool CPlayer::CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBlock *pBlock)
{
	bool bLand = false;

	CBoxCollider *pBoxCollider = pBlock->GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange) == true)
		{
			pBlock->Hit(this);
			bLand = true;
		}
	}
	return bLand;
}

//=============================================================================
// �A�C�e�������蔻��̏���
//=============================================================================
bool CPlayer::CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CItem *pItem)
{
	bool bLand = false;

	CBoxCollider *pBoxCollider = pItem->GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange / 2, NULL) == true)
		{
			pItem->Hit(this);
			bLand = true;
		}
	}
	return bLand;
}

//=============================================================================
// �S�[���~�������蔻��̏���
//=============================================================================
void CPlayer::CollisionGoalCylinder(D3DXVECTOR3 *pPos, CGoalCylinder *pGoalCylinder)
{
	if (pGoalCylinder->Collision(pPos) == true)
	{
		if (CManager::GetMode() == CManager::MODE_TUTORIAL)
		{
			CTutorial *pTutorial = CManager::GetTutorial();
			if (pTutorial->GetState() != CTutorial::STATE_END)
			{
				pTutorial->SetGoalPlayerIdx(m_nPlayerIdx);
				pTutorial->SetState(CTutorial::STATE_END);
			}
		}
	}
}

//=============================================================================
// �쓖���蔻��̏���
//=============================================================================
bool CPlayer::CollisionRiver(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CRiver *pRiver)
{
	bool bLand = false;
	if (pRiver != NULL)
	{
		CBoxCollider *pBoxCollider = pRiver->GetBoxCollider();
		if (pBoxCollider != NULL)
		{
			if (pBoxCollider->Collision(pPos,pPosOld,pMove, colRange / 2, NULL) == true)
			{
				bLand = true;
			}
		}
	}
	return bLand;
}

//=============================================================================
// �X�����蔻��̏���
//=============================================================================
void CPlayer::CollisionIceField(D3DXVECTOR3 pos, CIceField *pIceField, bool *pLandIce)
{
	bool  bIceLand = false;
	//���b�V���t�B�[���h�̎擾
	CMeshField *pMeshField = pIceField->GetMeshField();
	if (pMeshField != NULL)
	{//NULL�ł͂Ȃ��ꍇ
	 //���b�V���t�B�[���h�̃��[���h���W�ɂ����鍂���擾���鏈��
		pMeshField->GetPolyHeight(pos, &bIceLand, NULL);

		//�X�̏�ɂ��邩�ǂ����ʂ̏���
		switch (bIceLand)
		{
		case false:	//�X�̏�ɂ��Ȃ�
			if (*pLandIce == false)
			{
				SetInertia(0.8f);
			}
			break;
		case true:	//�X�̏�ɂ���
			*pLandIce = true;
			SetInertia(0.05f);
			break;
		}
	}

}

//=============================================================================
// �i�ߕ������蔻��̏���
//=============================================================================
bool CPlayer::CollisionHeadQuarters(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHeadQuarters *pHeadQuarters)
{
	bool bLand = false;
	if (pHeadQuarters != NULL)
	{
		CBoxCollider *pBoxCollider = pHeadQuarters->GetBoxCollider();
		if (pBoxCollider != NULL)
		{
			if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange / 2, NULL) == true)
			{
				bLand = true;
			}
		}
	}
	return bLand;
}

//=============================================================================
// �ЂȂ���ꓖ���蔻�菈��
//=============================================================================
bool CPlayer::CollisionHinaarare(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHinaarare *pHinaarare)
{
	bool bLand = false;
	if (pHinaarare != NULL)
	{
		CBoxCollider *pBoxCollider = pHinaarare->GetBoxCollider();
		if (pBoxCollider != NULL)
		{
			if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange / 2, NULL) == true)
			{
				pHinaarare->Hit(this);
				if (m_bHelmet == false)
				{
					SetState(STATE_STOP);
					bLand = true;
				}
			}
		}
	}
	return bLand;
}