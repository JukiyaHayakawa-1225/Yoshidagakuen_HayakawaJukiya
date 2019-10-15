//*****************************************************************************
//
//     �L�����N�^�[�Ǌ��̏���[characterManager.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "characterManager.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "modelcreate.h"
#include "textureManager.h"
#include "fileLoader.h"
#include "functionlib.h"
#include "object.h"
#include "enemy.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
//#define CHARAMANAGER_LOAD_TEXTURE                 // �錾���e�N�X�`����ǂݍ���
#define CHARAMANAGER_LOAD_MODEL                   // �錾�����f����ǂݍ���
#define CHARAMANAGER_LOAD_CHARACTER               // �錾���L�����N�^�[�f�[�^��ǂݍ���
#define CHARAMANAGER_LOAD_MOTION                  // �錾�����[�V������ǂݍ���
#define CHARAMANAGER_MAX_LOAD_MOTIONDATA (30)     // �ǂݍ��߂郂�[�V�����f�[�^�̍ő吔

// �l�ǂݍ��݂�����ۂ̖ڈ�ƂȂ镶����
// ����
#define POS               "POS = "                // ���W��ǂݎ�鍇�}
#define ROT               "ROT = "                // ������ǂݎ�鍇�}
#define MOVE              "MOVE = "               // �ړ��ʂ�ǂݎ�鍇�}

// ���̑�
#define NUM_TEXTURE       "NUM_TEXTURE = "        // �ǂݍ��ރe�N�X�`���̐�
#define TEXTURE_FILENAME  "TEXTURE_FILENAME = "   // �ǂݍ��ރe�N�X�`���̃t�@�C����
#define NUM_MODEL         "NUM_MODEL = "          // �ǂݍ��ރ��f���̐�
#define MODEL_FILENAME    "MODEL_FILENAME = "     // �ǂݍ��ރ��f���̃t�@�C����

// �L�����N�^�[���
#define CHARACTERSET      "CHARACTERSET"          // �L�����N�^�[���ǂݍ��݊J�n�̍��}
#define END_CHARACTERSET  "END_CHARACTERSET"      // �L�����N�^�[���ǂݍ��ݏI���̍��}
#define JUMP              "JUMP = "               // �W�����v��
#define GRAVITY           "GRAVITY = "            // �d�͗�
#define INERTIA           "INERTIA = "            // ������
#define RIVISION_ROT      "RIVISION_ROT = "       // ������␳����{��
#define COL_HEIGHT        "COL_HEIGHT = "         // �����蔻����Ƃ鍂��
#define COL_RANGE         "COL_RANGE = "          // �����蔻����Ƃ�͈�
#define NUM_PARTS         "NUM_PARTS = "          // �L�����N�^�[�̃p�[�c��

// �p�[�c���
#define PARTSSET          "PARTSSET"              // �p�[�c���ǂݍ��݊J�n�̍��}
#define END_PARTSSET      "END_PARTSSET"          // �p�[�c���ǂݍ��ݏI���̍��}
#define INDEX             "INDEX = "              // �ԍ���ǂݎ�鍇�}
#define PARENT            "PARENT = "             // �e�p�[�c�̔ԍ���ǂݎ�鍇�}

// ���[�V�������
#define MOTIONSET         "MOTIONSET"             // ���[�V�������ǂݍ��݊J�n�̍��}
#define END_MOTIONSET     "END_MOTIONSET"         // ���[�V�������ǂݍ��ݏI���̍��}
#define COLLISIONSET      "COLLISIONSET"          // ���[�V�����̓����蔻����ǂݍ��݊J�n�̍��}
#define END_COLLISIONSET  "END_COLLISIONSET"      // ���[�V�����̓����蔻����ǂݍ��ݏI���̍��}
#define ORBITSET          "ORBITSET"              // ���[�V�����̋O�Տ��ǂݍ��݊J�n�̍��}
#define END_ORBITSET      "END_ORBITSET"          // ���[�V�����̋O�Տ��ǂݍ��ݏI���̍��}
#define RANGE             "RANGE = "              // �͈͂�ǂݎ�鍇�}
#define XBLOCK            "XBLOCK = "             // �O�Ղ̉��̕�������ǂݎ�鍇�}
#define ZBLOCK            "ZBLOCK = "             // �O�Ղ̉��s�̕�������ǂݎ�鍇�}
#define OFFSET            "OFFSET = "             // �I�t�Z�b�g��ǂݎ�鍇�}
#define LOOP              "LOOP = "               // ���[�V���������[�v�Đ����邩�ǂ�����ǂݎ�鍇�}
#define BLEND             "BLEND = "              // ���[�V�����u�����h�̔{����ǂݎ�鍇�}
#define NUM_KEY           "NUM_KEY = "            // ���[�V�����̃L�[�t���[���̐���ǂݎ�鍇�}

// �U�����[�V�������
#define ATTACKSET         "ATTACKSET"             // �U�����[�V�������ǂݍ��݊J�n�̍��}
#define END_ATTACKSET     "END_ATTACKSET"         // �U�����[�V�������ǂݍ��ݏI���̍��}
#define NEXT_MOTION       "NEXT_MOTION = "        // ���̃��[�V�����̔ԍ�

// �����蔻����
#define NUM_COL           "NUM_COL = "            // ���[�V�����̓����蔻����̐���ǂݎ�鍇�}
#define COLLISION         "COLLISION"             // �����蔻����ǂݍ��݊J�n�̍��}
#define DAMAGE            "DAMAGE = "             // ���������ꍇ�̃_���[�W
#define END_COLLISION     "END_COLLISION"         // �����蔻����ǂݍ��ݏI���̍��}

// �O�Տ��
#define NUM_ORBIT         "NUM_ORBIT = "          // ���[�V�����̋O�Տ��̐���ǂݎ�鍇�}
#define ORBIT		      "ORBIT"                 // �O�Տ��ǂݍ��݊J�n�̍��}
#define END_ORBIT	      "END_ORBIT"             // �O�Տ��ǂݍ��ݏI���̍��}
#define TEX_INDEX         "TEX_INDEX = "          // �O�Ղɒ���t����e�N�X�`���̔ԍ���ǂݎ�鍇�}
#define OFFSET1           "OFFSET1 = "            // �P�ڂ̃I�t�Z�b�g��ǂݎ�鍇�}
#define OFFSET2           "OFFSET2 = "            // �Q�ڂ̃I�t�Z�b�g��ǂݎ�鍇�}
#define OFFSETAMP1        "OFFSETAMP1 = "         // �O�Ղ̃I�t�Z�b�g���������l��ǂݎ�鍇�}(�P��)
#define OFFSETAMP2        "OFFSETAMP2 = "         // �O�Ղ̃I�t�Z�b�g���������l��ǂݎ�鍇�}(�Q��)
#define COLUP             "COLUP = "              // �O�Ղ̒��_�㑤�̐F
#define COLDOWN           "COLDOWN = "            // �O�Ղ̒��_�����̐F
#define ALPHA_DECAY_UP    "ALPHA_DECAY_UP = "     // �O�Ղ̒��_�㑤�̓����x�����l
#define ALPHA_DECAY_DOWN  "ALPHA_DECAY_DOWN = "   // �O�Ղ̒��_�����̓����x�����l
#define START             "START = "              // �O�Ղ��o���n�߂�^�C�~���O��ǂݎ�鍇�}
#define FINISH            "FINISH = "             // �O�Ղ����܂��^�C�~���O��ǂݎ�鍇�}

// �L�[�t���[�����
#define KEYSET            "KEYSET"                // �L�[�t���[�����ǂݍ��݊J�n�̍��}
#define END_KEYSET        "END_KEYSET"            // �L�[�t���[�����ǂݍ��ݏI���̍��}
#define FRAME             "FRAME = "              // �L�[�t���[���̍Đ��t���[������ǂݎ�鍇�}

// �L�[���
#define KEY               "KEY"                   // �L�[���ǂݍ��݊J�n�̍��}
#define END_KEY           "END_KEY"               // �L�[���ǂݍ��ݏI���̍��}

//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CCharacterManager::CCharacterManager()
{
	// �e��l�̃N���A
	strcpy(m_aFileName, "\0");    // �v���C���[�f�[�^�̃X�N���v�g�t�@�C����
	m_nNumMotionData = 0;         // ���[�V�����f�[�^��ǂݍ��񂾉�
	m_pModelCreate = NULL;        // ���f���Ǌ��N���X�ւ̃|�C���^
	m_pTextureManager = NULL;     // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	m_pParent = NULL;             // �e���f���̔ԍ�
	m_pMotionManager = NULL;      // ���[�V�����Ǌ��N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CCharacterManager::~CCharacterManager()
{

}

//=============================================================================
//    ��������
//=============================================================================
CCharacterManager *CCharacterManager::Create(char *pFileName)
{
	CCharacterManager *pCharacterManager = NULL;  // �L�����N�^�[�Ǌ��N���X�^�̃|�C���^
	if (pCharacterManager == NULL)
	{// ����������ɂȂ��Ă���
		pCharacterManager = new CCharacterManager;
		if (pCharacterManager != NULL)
		{// �C���X�^���X�𐶐��ł���
			pCharacterManager->SetFileName(pFileName);
			if (FAILED(pCharacterManager->Init()))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}

	return pCharacterManager;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CCharacterManager::Init(void)
{
	// �t�@�C���I�[�v��
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = CFileLoader::Create(m_aFileName);
	if (pFileLoader == NULL)
	{
		return E_FAIL;
	}

	// �t�@�C����ǂݐi�߂�
	strcpy(aStr, pFileLoader->GetString(aStr));
	if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
	{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
		LoadScript(pFileLoader, aStr);
	}

	// �t�@�C�������
	if (pFileLoader != NULL)
	{
		pFileLoader->Uninit();
		delete pFileLoader;
		pFileLoader = NULL;
	}


	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CCharacterManager::Uninit(void)
{
	// �e���f���̔ԍ��̔j��
	if (m_pParent != NULL)
	{
		delete[] m_pParent;
		m_pParent = NULL;
	}

	// ���f���Ǌ��N���X�̔j��
	if (m_pModelCreate != NULL)
	{
		m_pModelCreate->Uninit();
		delete m_pModelCreate;
		m_pModelCreate = NULL;
	}

	// ���[�V�����Ǌ��N���X�̔j��
	if (m_pMotionManager != NULL)
	{
		m_pMotionManager->Uninit();
		delete m_pMotionManager;
		m_pMotionManager = NULL;
	}

	// ���f���N���X�̔j��
	if (m_apModel != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = NULL;
		}

		delete[] m_apModel;
		m_apModel = NULL;
	}

	// �e�N�X�`���Ǌ��N���X�̔j��
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}
}

//=============================================================================
//    �z�u�����쐬���鏈��
//=============================================================================
CObject *CCharacterManager::SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority)
{
	CModel **apModelCpy = NULL;                // ���f���N���X�R�s�[�p
	CMotionManager *pMotionManagerCpy = NULL;  // ���[�V�����Ǌ��N���X�R�s�[�p

	// ���f���̐������������m��
	apModelCpy = new CModel*[m_nNumModel];
	if (apModelCpy == NULL) return NULL;
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		// �������m��
		apModelCpy[nCntModel] = NULL;
		apModelCpy[nCntModel] = new CModel;

		// �l�R�s�[
		if (apModelCpy[nCntModel] != NULL)
		{
			apModelCpy[nCntModel]->Cpy(m_apModel[nCntModel]);
			if(m_pParent[nCntModel] != -1)
			{
				apModelCpy[nCntModel]->SetParent(apModelCpy[m_pParent[nCntModel]]);
			}
		}
	}

	// ���[�V�����f�[�^���R�s�[����
	if(m_nNumMotionData > 0)
	{
		pMotionManagerCpy = CMotionManager::Create(m_nNumMotionData, m_nNumModel);
		for (int nCntMotion = 0; nCntMotion < m_nNumMotionData; nCntMotion++)
		{
			pMotionManagerCpy->CpyMotion(m_pMotionManager->GetMotion()[nCntMotion], nCntMotion);
		}
	}

	// �z�u���𐶐�����
	CObject *pObject = CObject::Create(pos, rot, nType, nPriority);
	if (pObject != NULL)
	{
		pObject->SetNumModel(m_nNumModel);
		pObject->SetModel(apModelCpy);
		pObject->SetMotionManager(pMotionManagerCpy);
	}

	return pObject;
}

//=============================================================================
//    �G���쐬���鏈��
//=============================================================================
CEnemy *CCharacterManager::SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority)
{
	CModel **apModelCpy = NULL;                // ���f���N���X�R�s�[�p
	CMotionManager *pMotionManagerCpy = NULL;  // ���[�V�����Ǌ��N���X�R�s�[�p

	// ���f���̐������������m��
	apModelCpy = new CModel*[m_nNumModel];
	if (apModelCpy == NULL) return NULL;
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		// �������m��
		apModelCpy[nCntModel] = NULL;
		apModelCpy[nCntModel] = new CModel;

		// �l�R�s�[
		if (apModelCpy[nCntModel] != NULL)
		{
			apModelCpy[nCntModel]->Cpy(m_apModel[nCntModel]);
			if (m_pParent[nCntModel] != -1)
			{
				apModelCpy[nCntModel]->SetParent(apModelCpy[m_pParent[nCntModel]]);
			}
		}
	}

	// ���[�V�����f�[�^���R�s�[����
	if (m_nNumMotionData > 0)
	{
		pMotionManagerCpy = CMotionManager::Create(m_nNumMotionData, m_nNumModel);
		for (int nCntMotion = 0; nCntMotion < m_nNumMotionData; nCntMotion++)
		{
			pMotionManagerCpy->CpyMotion(m_pMotionManager->GetMotion()[nCntMotion], nCntMotion);
		}
	}

	// �z�u���𐶐�����
	CEnemy *pEnemy = NULL;
	switch (nType)
	{
	case CEnemy::TYPE_NORMAL:
		pEnemy = CEnemyNormal::Create(pos, rot, (CEnemy::TYPE)nType, ENEMY_PRIORITY);
		break;
	case CEnemy::TYPE_ARMORE:
		pEnemy = CEnemyArmore::Create(pos, rot, (CEnemy::TYPE)nType, ENEMY_PRIORITY);
		break;
	case CEnemy::TYPE_FAST:
		pEnemy = CEnemyFast::Create(pos, rot, (CEnemy::TYPE)nType, ENEMY_PRIORITY);
		break;
	case CEnemy::TYPE_HEAVY:
		pEnemy = CEnemyHeavy::Create(pos, rot, (CEnemy::TYPE)nType, ENEMY_PRIORITY);
		break;
	}
	if (pEnemy != NULL)
	{
		pEnemy->SetAccel(m_CharaData.fAccel);
		pEnemy->SetInertia(m_CharaData.fInertia);
		pEnemy->SetRivisionRot(m_CharaData.fRivisionRot);
		pEnemy->SetNumPart(m_CharaData.nNumParts);
		pEnemy->SetModel(apModelCpy);
		pEnemy->SetMotionManager(pMotionManagerCpy);
	}

	return pEnemy;
}

//=============================================================================
//    �X�N���v�g�t�@�C����ǂݍ��ޏ���
//=============================================================================
HRESULT CCharacterManager::LoadScript(CFileLoader *pFileLoader, char *pStr)
{
#ifdef CHARAMANAGER_LOAD_TEXTURE
	// �e�N�X�`����ǂݍ���
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, NUM_TEXTURE) == 0)
	{// �ǂݍ��ރe�N�X�`������񂾂���
		if (FAILED(LoadTextureSet(pFileLoader, pStr)))
		{
			return E_FAIL;
		}
	}
#endif

#ifdef CHARAMANAGER_LOAD_MODEL
	// ���f����ǂݍ���
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, NUM_MODEL) == 0)
	{// �ǂݍ��ރ��f������񂾂���
		if (FAILED(LoadModelSet(pFileLoader, pStr)))
		{
			return E_FAIL;
		}
	}
#endif

#ifdef CHARAMANAGER_LOAD_CHARACTER
	// �L�����N�^�[����ǂݍ���
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, CHARACTERSET) == 0)
	{// �L�����N�^�[���ǂݍ��݊J�n�̍��}������
		if (FAILED(LoadCharacterSet(pFileLoader, pStr)))
		{
			return E_FAIL;
		}
	}
#endif

#ifdef CHARAMANAGER_LOAD_MOTION
	// ���[�V��������ǂݍ���
	CMotion *apMotion[CHARAMANAGER_MAX_LOAD_MOTIONDATA] = {};
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, MOTIONSET) == 0)
		{// ���[�V�������ǂݍ��݊J�n�̍��}������
			if (FAILED(LoadMotionSet(pFileLoader, pStr, apMotion)))
			{
				return E_FAIL;
			}
			m_nNumMotionData++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�t�@�C���ǂݍ��ݏI���̍��}������
			break;
		}
	}

	// ���[�V�����Ǌ��N���X�𐶐�
	if (m_nNumMotionData > 0)
	{// ���[�V�����f�[�^���ǂݍ��܂�Ă���
		m_pMotionManager = CMotionManager::Create(m_nNumMotionData, m_CharaData.nNumParts);
		if (m_pMotionManager == NULL)
		{
			delete m_pMotionManager;
			m_pMotionManager = NULL;
			return E_FAIL;
		}

		// ���[�V�����ւ̃|�C���^��ݒ肷��
		for (int nCntMotion = 0; nCntMotion < m_nNumMotionData; nCntMotion++)
		{
			m_pMotionManager->CpyMotion(apMotion[nCntMotion], nCntMotion);
		}

		// �ǂݍ��ݗp�̃��������J��
		for (int nCntMotion = 0; nCntMotion < CHARAMANAGER_MAX_LOAD_MOTIONDATA; nCntMotion++)
		{
			if (apMotion[nCntMotion] != NULL)
			{
				apMotion[nCntMotion]->Uninit(m_CharaData.nNumParts);
				delete apMotion[nCntMotion];
				apMotion[nCntMotion] = NULL;
			}
		}
	}
#endif

	return S_OK;
}

//=============================================================================
//    �e�N�X�`������ǂݍ��ޏ���
//=============================================================================
HRESULT CCharacterManager::LoadTextureSet(CFileLoader *pFileLoader, char *pStr)
{
	// �ǂݍ��ރe�N�X�`���̐���ݒ�
	m_nNumTexture = CFunctionLib::ReadInt(pStr, NUM_TEXTURE);

	// �ǂݍ��݂ɕK�v�ȃ��������m��
	m_pTextureManager = CTextureManager::Create(m_nNumTexture);
	if (m_pTextureManager == NULL)
	{
		delete m_pTextureManager;
		m_pTextureManager = NULL;
		return E_FAIL;
	}

	// �e�N�X�`���ǂݍ��݊J�n
	for (int nCntTex = 0; nCntTex < m_nNumTexture; nCntTex++)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEXTURE_FILENAME) == 0)
		{// �ǂݍ��ރe�N�X�`���̃t�@�C����񂾂���
			LoadTexture(pStr, nCntTex);
		}
	}

	return S_OK;
}

//=============================================================================
//    �e�N�X�`����ǂݍ��ޏ���
//=============================================================================
HRESULT CCharacterManager::LoadTexture(char *pStr, int nCntTex)
{
	// �e�N�X�`���ǂݍ��ݗp�ϐ��錾
	LPDIRECT3DTEXTURE9 pTexture = NULL;   // �e�N�X�`���ւ̃|�C���^

	// �e�N�X�`���̃t�@�C���p�X����ǂݎ��
	pStr = CFunctionLib::ReadString(pStr, pStr, TEXTURE_FILENAME);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pStr, &pTexture);

	// �e�N�X�`���Ǌ��N���X�ɒl��ݒ肷��
	m_pTextureManager->SetTexture(pTexture, nCntTex);
	m_pTextureManager->SetFileName(pStr, nCntTex);

	return S_OK;
}

//=============================================================================
//    ���f������ǂݍ��ޏ���
//=============================================================================
HRESULT CCharacterManager::LoadModelSet(CFileLoader *pFileLoader, char *pStr)
{
	// �ǂݍ��ރ��f������ݒ�
	m_nNumModel = CFunctionLib::ReadInt(pStr, NUM_MODEL);

	// �ǂݍ��݂ɕK�v�ȃ��������m��
	m_pModelCreate = CModelCreate::Create(m_nNumModel);
	if (m_pModelCreate == NULL)
	{
		delete m_pModelCreate;
		m_pModelCreate = NULL;
		return E_FAIL;
	}

	// ���f���ǂݍ��݊J�n
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, MODEL_FILENAME) == 0)
		{// �ǂݍ��ރ��f���̃t�@�C����񂾂���
			LoadModel(pStr, nCntModel);
		}
	}

	return S_OK;
}

//=============================================================================
//    ���f����ǂݍ��ޏ���
//=============================================================================
HRESULT CCharacterManager::LoadModel(char *pStr, int nCntModel)
{
	// ���f���ǂݍ��ݗp�ϐ��錾
	LPD3DXMESH pMesh = NULL;       // ���b�V���ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = NULL;  // �}�e���A�����ւ̃|�C���^
	DWORD nNumMat = 0;             // �}�e���A�����̐�

	// ���f���̃t�@�C���p�X����ǂݎ��
	pStr = CFunctionLib::ReadString(pStr, pStr, MODEL_FILENAME);

	// x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(pStr, D3DXMESH_SYSTEMMEM, CManager::GetRenderer()->GetDevice(), NULL,
		&pBuffMat, NULL, &nNumMat, &pMesh);

	// ���f���Ǌ��N���X�ɒl��ݒ肷��
	m_pModelCreate->SetMesh(pMesh, nCntModel);
	m_pModelCreate->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);
	m_pModelCreate->SetFileName(pStr, nCntModel);

	return S_OK;
}

//=============================================================================
//    �L�����N�^�[����ǂݍ��ޏ���
//=============================================================================
HRESULT CCharacterManager::LoadCharacterSet(CFileLoader *pFileLoader, char *pStr)
{
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, MOVE) == 0)
		{// �ړ��ʂ�����
			m_CharaData.fAccel = CFunctionLib::ReadFloat(pStr, MOVE);
		}
		else if (CFunctionLib::Memcmp(pStr, INERTIA) == 0)
		{// �����ʂ�����
			m_CharaData.fInertia = CFunctionLib::ReadFloat(pStr, INERTIA);
		}
		else if (CFunctionLib::Memcmp(pStr, RIVISION_ROT) == 0)
		{// ������␳����{��������
			m_CharaData.fRivisionRot = CFunctionLib::ReadFloat(pStr, RIVISION_ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_PARTS) == 0)
		{// �g�p����p�[�c����񂾂���
			m_CharaData.nNumParts = CFunctionLib::ReadInt(pStr, NUM_PARTS);
			break;
		}
	}

	// �p�[�c����ǂݍ���
	m_apModel = new CModel*[m_CharaData.nNumParts];
	m_pParent = new int[m_CharaData.nNumParts];
	for (int nCntParts = 0; nCntParts < m_CharaData.nNumParts; nCntParts++)
	{
		m_apModel[nCntParts] = NULL;
		m_pParent[nCntParts] = -1;
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, PARTSSET) == 0)
		{// �p�[�c���ǂݍ��݊J�n�̍��}������
			if (FAILED(LoadPartsSet(pFileLoader, pStr, nCntParts)))
			{
				return E_FAIL;
			}
		}
	}

	// �L�����N�^�[�ǂݍ��ݏI��
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, END_CHARACTERSET) != 0)
	{// �L�����N�^�[���ǂݍ��ݏI���̍��}������
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �p�[�c����ǂݎ�鏈��
//=============================================================================
HRESULT CCharacterManager::LoadPartsSet(CFileLoader *pFileLoader, char *pStr, int nCntParts)
{
	int nModelIdx = 0;                                     // �p�[�c�ԍ����
	D3DXVECTOR3 ModelPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���f���̃I�t�Z�b�g�ʒu���
	D3DXVECTOR3 ModelRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���f���̃I�t�Z�b�g�������

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, INDEX) == 0)
		{// �p�[�c�ԍ���ǂݍ���
			nModelIdx = CFunctionLib::ReadInt(pStr, INDEX);
		}
		else if (CFunctionLib::Memcmp(pStr, PARENT) == 0)
		{// �e���f���̔ԍ���ǂݍ���
			m_pParent[nCntParts] = CFunctionLib::ReadInt(pStr, PARENT);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �I�t�Z�b�g�ʒu����ǂݍ���
			ModelPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{// �I�t�Z�b�g��������ǂݍ���
			ModelRot = CFunctionLib::ReadVector3(pStr, ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_PARTSSET) == 0)
		{// �p�[�c���ǂݍ��ݏI���̍��}������
			break;
		}
	}

	// ���f���𐶐�����
	m_apModel[nCntParts] = CModel::Create(ModelPos, ModelRot, m_pModelCreate->GetMesh(nModelIdx),
		m_pModelCreate->GetBuffMat(nModelIdx), m_pModelCreate->GetNumMat(nModelIdx),
		m_pModelCreate->GetTexture(nModelIdx),
		m_pModelCreate->GetVtxMax(nModelIdx), m_pModelCreate->GetVtxMin(nModelIdx));
	if (m_apModel[nCntParts] != NULL && m_pParent[nCntParts] != -1)
	{// �|�C���^���m�ۂł��e���f�������݂���
		m_apModel[nCntParts]->SetParent(m_apModel[m_pParent[nCntParts]]);
	}

	return S_OK;
}

//=============================================================================
//    ���[�V��������ǂݎ�鏈��
//=============================================================================
HRESULT CCharacterManager::LoadMotionSet(CFileLoader *pFileLoader, char *pStr, CMotion **apMotion)
{
	int nCntKeySet = 0;                           // �L�[�t���[���t���[������ǂݍ��񂾉�
	CMotionAttack *pMotionAttack = NULL;          // �U�����[�V�����f�[�^�N���X�ւ̃|�C���^
	CMotionCollision **pMotionCollision = NULL;   // ���[�V�����̓����蔻��f�[�^�N���X�ւ̃|�C���^
	int nNumColData = 0;                          // �����蔻��f�[�^�̐�
	CMotionOrbit **pMotionOrbit = NULL;           // ���[�V�����̋O�Ճf�[�^�N���X�ւ̃|�C���^
	int nNumOrbitData = 0;                        // �O�Ճf�[�^�̐�
	bool bLoop = false;                           // ���[�v���邩���Ȃ���
	float fBlendMag = 0.0f;                       // �u�����h�{��
	int nNumKey = 0;                              // �L�[�t���[���̑���
	int *pPlaybackKey = NULL;                     // �Đ��t���[����(��������̂œ��I�m��)
	D3DXVECTOR3 **pAddPos = NULL;                 // ���f���̍��W
	D3DXVECTOR3 **pDestRot = NULL;                // ���f���̌���

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, LOOP) == 0)
		{// ���[�v���邩���Ȃ����̏�񂪂���
			bLoop = CFunctionLib::ReadBool(pStr, LOOP);
		}
		else if (CFunctionLib::Memcmp(pStr, BLEND) == 0)
		{// �u�����h�{���̏�񂪂���
			fBlendMag = CFunctionLib::ReadFloat(pStr, BLEND);
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_KEY) == 0)
		{// �L�[�t���[���̑����̏�񂪂���
			nNumKey = (CFunctionLib::ReadInt(pStr, NUM_KEY));
			break;
		}
	}

	// �ǂݍ��݂ɕK�v�ȃ��������m��
	if (nNumKey >= 1)
	{// 1�ȏ�L�[�t���[��������
	    // �K�v�ȕϐ��𓮓I�Ɋm�ۂ���
	    // �Đ��t���[����
		pPlaybackKey = new int[nNumKey];

		// ���W
		if (pAddPos == NULL)
		{// ���������m�ۂł����Ԃł���
			pAddPos = new D3DXVECTOR3*[nNumKey];
			if (pAddPos != NULL)
			{// ���������m�ۂł���
				for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
				{// �L�[�t���[���̑������J��Ԃ�
					pAddPos[nCntKey] = new D3DXVECTOR3[m_CharaData.nNumParts];
				}
			}
		}

		if (pDestRot == NULL)
		{// ���������m�ۂł����Ԃł���
		 // ����
			pDestRot = new D3DXVECTOR3*[nNumKey];
			if (pDestRot != NULL)
			{// ���������m�ۂł���
				for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
				{// �L�[�t���[���̑������J��Ԃ�
					pDestRot[nCntKey] = new D3DXVECTOR3[m_CharaData.nNumParts];
				}
			}
		}
	}


	// ���[�V�����f�[�^��ǂݍ���
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, ATTACKSET) == 0)
		{// �����蔻����ǂݍ��݊J�n�̍��}������
			pMotionAttack = LoadMotionAttackSet(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, COLLISIONSET) == 0)
		{// �����蔻����ǂݍ��݊J�n�̍��}������
			pMotionCollision = LoadMotionCollisionSet(pFileLoader, pStr, &nNumColData);
		}
		else if (CFunctionLib::Memcmp(pStr, ORBITSET) == 0)
		{// �O�Տ��ǂݍ��݊J�n�̍��}������
			pMotionOrbit = LoadMotionOrbitSet(pFileLoader, pStr, &nNumOrbitData);
		}
		else if (CFunctionLib::Memcmp(pStr, KEYSET) == 0)
		{// �L�[�t���[�����ǂݍ��݊J�n�̍��}������
			LoadKeySet(pFileLoader, pStr, nCntKeySet, pPlaybackKey, pAddPos[nCntKeySet], pDestRot[nCntKeySet]);
			nCntKeySet++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_MOTIONSET) == 0)
		{// ���[�V�������ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	apMotion[m_nNumMotionData] = CMotion::Create(m_CharaData.nNumParts, nNumKey, fBlendMag, bLoop, pPlaybackKey);
	if (apMotion[m_nNumMotionData] != NULL)
	{// ���������m�ۂł���
	    // �U�����[�V�����f�[�^�N���X��ݒ肷��
		apMotion[m_nNumMotionData]->SetAttack(pMotionAttack);

		// �����蔻��f�[�^�N���X��ݒ肷��
		apMotion[m_nNumMotionData]->SetCollision(pMotionCollision);
		apMotion[m_nNumMotionData]->SetNumColData(nNumColData);

		// �O�Ճf�[�^�N���X��ݒ肷��
		apMotion[m_nNumMotionData]->SetOrbit(pMotionOrbit);
		apMotion[m_nNumMotionData]->SetNumOrbitData(nNumOrbitData);

		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{// �L�[�t���[���̑������J��Ԃ�
			for (int nCntParts = 0; nCntParts < m_CharaData.nNumParts; nCntParts++)
			{// �L�[�t���[���̑������J��Ԃ�
				apMotion[m_nNumMotionData]->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pAddPos[nCntKey][nCntParts]);
				apMotion[m_nNumMotionData]->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pDestRot[nCntKey][nCntParts]);
			}
		}
	}

	// ���������m�ۂ������J������
	// �Đ��t���[����
	if (pPlaybackKey != NULL)
	{// ���������m�ۂ���Ă���
		delete[] pPlaybackKey;
		pPlaybackKey = NULL;
	}

	// ���W
	if (pAddPos != NULL)
	{// ���������m�ۂł���
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{// �L�[�t���[���̑������J��Ԃ�
			delete[] pAddPos[nCntKey];
			pAddPos[nCntKey] = NULL;
		}
		delete[] pAddPos;
		pAddPos = NULL;
	}

	// ����
	if (pDestRot != NULL)
	{// ���������m�ۂł���
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{// �L�[�t���[���̑������J��Ԃ�
			delete[] pDestRot[nCntKey];
			pDestRot[nCntKey] = NULL;
		}
		delete[] pDestRot;
		pDestRot = NULL;
	}

	return S_OK;
}

//=============================================================================
//    ���[�V�����̍U�����[�V��������ǂݎ�鏈��
//=============================================================================
CMotionAttack *CCharacterManager::LoadMotionAttackSet(CFileLoader *pFileLoader, char *pStr)
{
	CMotionAttack *pMotionAttack = NULL;   // �U�����[�V�����f�[�^�N���X�ւ̃|�C���^
	pMotionAttack = new CMotionAttack;     // ���������m�ۂ���
	if (pMotionAttack == NULL)
	{
		delete pMotionAttack;
		return NULL;
	}

	while (1)
	{// ���[�v�J�n
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NEXT_MOTION) == 0)
		{// ���̃��[�V�����̔ԍ���񂾂���
			pMotionAttack->SetNextMotionIdx(CFunctionLib::ReadInt(pStr, NEXT_MOTION));
		}
		else if (CFunctionLib::Memcmp(pStr, START) == 0)
		{// �O�Ղ��o���n�߂�^�C�~���O��񂾂���
			pMotionAttack->SetStartTiming(CFunctionLib::ReadInt(pStr, START));
		}
		else if (CFunctionLib::Memcmp(pStr, FINISH) == 0)
		{// �O�Ղ����܂��^�C�~���O��񂾂���
			pMotionAttack->SetFinishTiming(CFunctionLib::ReadInt(pStr, FINISH));
		}
		else if (CFunctionLib::Memcmp(pStr, END_ATTACKSET) == 0)
		{// �U�����[�V�����f�[�^�ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	return pMotionAttack;
}

//=============================================================================
//    ���[�V�����̓����蔻�����ǂݎ�鏈��
//=============================================================================
CMotionCollision **CCharacterManager::LoadMotionCollisionSet(CFileLoader *pFileLoader, char *pStr, int *pNumColData)
{
	CMotionCollision **pMotionCollision = NULL;  // ���[�V�����̓����蔻��f�[�^�N���X�ւ̃|�C���^

	// �����蔻����̐���ǂݍ���
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, NUM_COL) == 0)
	{// �����蔻��f�[�^�̐�������
		*pNumColData = CFunctionLib::ReadInt(pStr, NUM_COL);
	}

	// �f�[�^�ǂݍ��݂ɕK�v�ȃ��������m��
	if (*pNumColData >= 1)
	{// �����蔻��f�[�^���P�ȏ゠��
		pMotionCollision = new CMotionCollision*[*pNumColData];
		if (pMotionCollision != NULL)
		{// ���������m�ۂł���
			for (int nCntCol = 0; nCntCol < *pNumColData; nCntCol++)
			{// �����蔻��f�[�^�̐������J��Ԃ�
				pMotionCollision[nCntCol] = NULL;
				pMotionCollision[nCntCol] = new CMotionCollision;
			}
		}
	}

	// �f�[�^�ǂݍ��݊J�n
	for (int nCntCol = 0; nCntCol < *pNumColData; nCntCol++)
	{// �����蔻��f�[�^�̐������J��Ԃ�
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, COLLISION) == 0)
		{// �����蔻��f�[�^�ǂݍ��݊J�n�̍��}������
			// �����蔻�����郂�f���̔ԍ���ǂݍ���
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, INDEX) == 0)
			{
				pMotionCollision[nCntCol]->SetModelIdx(CFunctionLib::ReadInt(pStr, INDEX));
			}

			// �����蔻�����郂�f������̃I�t�Z�b�g��������ǂݍ���
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, OFFSET) == 0)
			{// �����蔻�����郂�f������̃I�t�Z�b�g������񂾂���
				pMotionCollision[nCntCol]->SetOffsetPos(CFunctionLib::ReadVector3(pStr, OFFSET));
			}

			// �����蔻������͈͏��ǂݍ���
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, RANGE) == 0)
			{
				pMotionCollision[nCntCol]->SetRange(CFunctionLib::ReadFloat(pStr, RANGE));
			}

			// �����蔻������n�߂�^�C�~���O���ǂݍ���
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, START) == 0)
			{
				pMotionCollision[nCntCol]->SetStartTiming(CFunctionLib::ReadInt(pStr, START));
			}

			// �����蔻����I������^�C�~���O���ǂݍ���
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, FINISH) == 0)
			{
				pMotionCollision[nCntCol]->SetFinishTiming(CFunctionLib::ReadInt(pStr, FINISH));
			}

			// �����������̃_���[�W�ʏ��ǂݍ���
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, DAMAGE) == 0)
			{
				pMotionCollision[nCntCol]->SetDamage(CFunctionLib::ReadInt(pStr, DAMAGE));
			}

			// �����蔻��f�[�^�ǂݍ��ݏI���̍��}�`�F�b�N
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, END_COLLISION) != 0)
			{
				return NULL;
			}
		}
	}

	// �����蔻����ǂݍ��ݏI���̍��}�`�F�b�N
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, END_COLLISIONSET) != 0)return NULL;

	return pMotionCollision;
}

//=============================================================================
//    ���[�V�����̋O�Տ���ǂݎ�鏈��
//=============================================================================
CMotionOrbit **CCharacterManager::LoadMotionOrbitSet(CFileLoader *pFileLoader, char *pStr, int *pNumOrbitData)
{
	CMotionOrbit **pMotionOrbit = NULL;  // ���[�V�����̋O�Ճf�[�^�N���X�ւ̃|�C���^

	// �O�Ճf�[�^�̐���ǂݍ���
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, NUM_ORBIT) == 0)
	{// �O�Ճf�[�^�̐�������
		*pNumOrbitData = CFunctionLib::ReadInt(pStr, NUM_ORBIT);
	}

	// �ǂݍ��݂ɕK�v�ȃ��������m��
	if (*pNumOrbitData >= 1)
	{// �����蔻��f�[�^���P�ȏ゠��
		pMotionOrbit = new CMotionOrbit*[*pNumOrbitData];
		if (pMotionOrbit != NULL)
		{// ���������m�ۂł���
			for (int nCntOrbit = 0; nCntOrbit < *pNumOrbitData; nCntOrbit++)
			{// �����蔻��f�[�^�̐������J��Ԃ�
				pMotionOrbit[nCntOrbit] = NULL;
				pMotionOrbit[nCntOrbit] = new CMotionOrbit;
			}
		}
	}

	// �f�[�^�ǂݍ��݊J�n
	for (int nCntOrbit = 0; nCntOrbit < *pNumOrbitData; nCntOrbit++)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, ORBIT) == 0)
		{// �����蔻��f�[�^�ǂݍ��݊J�n�̍��}������
		    // �O�Ղ��o�����f���̔ԍ����ǂݍ���
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, INDEX) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetModelIdx(CFunctionLib::ReadInt(pStr, INDEX));
			}

			// �O�Ղɒ���t����e�N�X�`���̔ԍ����ǂݍ���
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, TEX_INDEX) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetTexIdx(CFunctionLib::ReadInt(pStr, TEX_INDEX));
			}

			// �O�Ղ��o�����f������̃I�t�Z�b�g�������(�P��)�ǂݍ���
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, OFFSET1) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetOffsetPos1(CFunctionLib::ReadVector3(pStr, OFFSET1));
			}

			// �O�Ղ��o�����f������̃I�t�Z�b�g�������(�Q��)�ǂݍ���
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, OFFSET2) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetOffsetPos2(CFunctionLib::ReadVector3(pStr, OFFSET2));
			}

			// �O�Ղ̃I�t�Z�b�g���������l(1��)�ǂݍ���
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, OFFSETAMP1) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetOffsetAmp1(CFunctionLib::ReadVector3(pStr, OFFSETAMP1));
			}

			// �O�Ղ̃I�t�Z�b�g���������l(2��)�ǂݍ���
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, OFFSETAMP2) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetOffsetAmp2(CFunctionLib::ReadVector3(pStr, OFFSETAMP2));
			}

			// �O�Ղ̒��_�㑤�̐F�ǂݍ���
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, COLUP) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetColUp(CFunctionLib::ReadVector4(pStr, COLUP));
			}

			// �O�Ղ̒��_�����̐F�ǂݍ���
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, COLDOWN) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetColDown(CFunctionLib::ReadVector4(pStr, COLDOWN));
			}

			// �O�Ղ̒��_�㑤�̓����x�����l�ǂݍ���
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, ALPHA_DECAY_UP) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetAlphaDecayUp(CFunctionLib::ReadFloat(pStr, ALPHA_DECAY_UP));
			}

			// �O�Ղ̒��_�����̓����x�����l�ǂݍ���
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, ALPHA_DECAY_DOWN) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetAlphaDecayDown(CFunctionLib::ReadFloat(pStr, ALPHA_DECAY_DOWN));
			}

			// �O�Ղ̉��̕��������ǂݍ���
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, XBLOCK) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetXBlock(CFunctionLib::ReadInt(pStr, XBLOCK));
			}

			// �O�Ղ̏c�̕��������ǂݍ���
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, ZBLOCK) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetYBlock(CFunctionLib::ReadInt(pStr, ZBLOCK));
			}

			// �O�Ղ��o���n�߂�^�C�~���O���ǂݍ���
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, START) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetStartTiming(CFunctionLib::ReadInt(pStr, START));
			}

			// �O�Ղ����܂��^�C�~���O���ǂݍ���
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, FINISH) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetFinishTiming(CFunctionLib::ReadInt(pStr, FINISH));
			}

			// �O�Ճf�[�^�ǂݍ��ݏI���̍��}�`�F�b�N
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, END_ORBIT) != 0) return NULL;
		}
	}

	// �O�Տ��ǂݍ��ݏI���̍��}�`�F�b�N
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, END_ORBITSET) != 0) return NULL;

	return pMotionOrbit;
}

//=============================================================================
//    �L�[�t���[������ǂݎ�鏈��
//=============================================================================
HRESULT CCharacterManager::LoadKeySet(CFileLoader *pFileLoader, char *pStr, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
{
	int nCntKey = 0;

	// �Đ��t���[�������ǂݍ���
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, FRAME) == 0)
	{// �Đ��t���[����������
		pPlaybackKey[nCntKeySet] = CFunctionLib::ReadInt(pStr, FRAME);
	}

	// �L�[���ǂݍ���
	for (int nCntParts = 0; nCntParts < m_CharaData.nNumParts; nCntParts++)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, KEY) == 0)
		{// �L�[���ǂݍ��݊J�n�̍��}������
			LoadKey(pFileLoader, pStr, nCntParts, AddPos, DestRot);
		}
	}

	// �L�[�t���[�����ǂݍ��ݏI���̍��}�`�F�b�N
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, END_KEYSET) != 0) return E_FAIL;

	return S_OK;
}

//=============================================================================
//    �L�[����ǂݎ�鏈��
//=============================================================================
HRESULT CCharacterManager::LoadKey(CFileLoader *pFileLoader, char *pStr, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
{
	// �I�t�Z�b�g�ʒu�ɉ�����l��ǂݍ���
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, POS) == 0)
	{
		AddPos[nCntKey] = CFunctionLib::ReadVector3(pStr, POS);
	}

	// �ړI�̌�����ǂݍ���
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, ROT) == 0)
	{
		DestRot[nCntKey] = CFunctionLib::ReadVector3(pStr, ROT);
	}

	// �L�[���ǂݍ��ݏI���̍��}�`�F�b�N
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, END_KEY) != 0) return E_FAIL;


	return S_OK;
}

//=============================================================================
//    �L�����N�^�[�f�[�^�̃X�N���v�g�t�@�C������ݒ肷�鏈��
//=============================================================================
void CCharacterManager::SetFileName(char *pFileName)
{
	strcpy(m_aFileName, pFileName);
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^��ݒ肷�鏈��
//=============================================================================
void CCharacterManager::SetModelCreate(CModelCreate *pModelCreate)
{
	m_pModelCreate = pModelCreate;
}

//=============================================================================
//    �L�����N�^�[�f�[�^�̃X�N���v�g�t�@�C�������擾���鏈��
//=============================================================================
char *CCharacterManager::GetFileName(void)
{
	return m_aFileName;
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CModelCreate *CCharacterManager::GetModelCreate(void)
{
	return m_pModelCreate;
}
