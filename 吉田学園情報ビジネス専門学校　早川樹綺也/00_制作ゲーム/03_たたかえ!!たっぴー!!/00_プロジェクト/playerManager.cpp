//=============================================================================
//
// �v���C���[�}�l�[�W���[�̏��� [playerManager.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "playerManager.h"
#include "manager.h"
#include "renderer.h"
#include "fileLoader.h"
#include "functionlib.h"
#include "player.h"
#include "model.h"
#include "modelcreate.h"
#include "motion.h"
#include "player.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define NUM_MODEL "NUM_MODEL = "
#define MODEL_FILENAME "MODEL_FILENAME = "
#define CHARACTERSET "CHARACTERSET"
#define END_CHARACTERSET "END_CHARACTERSET"
#define MOTIONSET "MOTIONSET"
#define END_MOTIONSET "END_MOTIONSET"
#define NUM_PARTS "NUM_PARTS = "
#define PARTSSET "PARTSSET"
#define END_PARTSSET "END_PARTSSET"
#define INDEX "INDEX = "
#define PARENT "PARENT = "
#define POS "POS = "
#define ROT "ROT = "
#define NUM_PARTS "NUM_PARTS = "
#define LOOP "LOOP = "
#define NUM_KEY "NUM_KEY ="
#define KEYSET "KEYSET"
#define END_KEYSET "END_KEYSET"
#define FRAME "FRAME = "
#define KEY "KEY"
#define END_KEY "END_KEY"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayerManager::CPlayerManager()
{
	m_nNumPlayer = 1;			//�v���C���[����
	m_pModelCreate = NULL;		//���f�������̃|�C���^
	m_apModel = NULL;			//���f���̃|�C���^���
	m_pMotionManager = NULL;	//���[�V�����}�l�[�W���[�̃|�C���^���
	m_nNumParts = 0;			//�p�[�c�̑���
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayerManager::~CPlayerManager()
{

}

//=============================================================================
// ��������
//=============================================================================
CPlayerManager *CPlayerManager::Create(char *PlayerFileName)
{
	CPlayerManager *pPlayerManager = NULL;  //���f�������|�C���^
	if (pPlayerManager == NULL)
	{//NULL�̏ꍇ
		pPlayerManager = new CPlayerManager;		//���I�m��
		if (pPlayerManager != NULL)
		{//NULL�łȂ��ꍇ
			pPlayerManager->Init(PlayerFileName);	//����������
		}
	}
	return pPlayerManager;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayerManager::Init(char *PlayerFileName)
{
	//���f���������v���C���[�����I�m��
	m_pModelCreate = new CModelCreate*[m_nNumPlayer];

	for (int nCntPlayer = 0; nCntPlayer < m_nNumPlayer; nCntPlayer++)
	{//�v���C���[���J��Ԃ�
		m_pModelCreate[nCntPlayer] = NULL;				//���f��������NULL�ɂ���
	}

	/*for (int nCntMotion = 0; nCntMotion < CPlayer::MOTION_MAX; nCntMotion++)
	{
		if (m_pMotion[nCntMotion] != NULL)
		{
			m_pMotion[nCntMotion] = NULL;
		}
	}*/
	m_apModel = NULL;	//���f���̏���������
	char *pStr = NULL;
	char aStr[256];

	for (int nCntPlayer = 0; nCntPlayer < m_nNumPlayer; nCntPlayer++)
	{//�v���C���[���J��Ԃ�
		CFileLoader *pFileLoader = NULL;	//�t�H���_�ǂݍ��݂̃|�C���^
		if (pFileLoader == NULL)
		{//NULL�̏ꍇ
		 //�v���C���[�I���̏���
			pFileLoader = CFileLoader::Create(PlayerFileName);

			if (pFileLoader != NULL)
			{
				strcpy(aStr, pFileLoader->GetString(aStr));
				if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
				{
					LoadPlayer(aStr, pFileLoader, nCntPlayer);
				}

				//�t�@�C���ǂݍ��݂̏I������
				if (pFileLoader != NULL)
				{
					pFileLoader->Uninit();
					delete pFileLoader;
					pFileLoader = NULL;
				}
			}
		}
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayerManager::Uninit(void)
{
	//���f�������̏I������
	if (m_pModelCreate != NULL)
	{
		for (int nCntPlayer = 0; nCntPlayer < m_nNumPlayer; nCntPlayer++)
		{
			if (m_pModelCreate[nCntPlayer] != NULL)
			{
				m_pModelCreate[nCntPlayer]->Uninit();
				delete m_pModelCreate[nCntPlayer];
				m_pModelCreate[nCntPlayer] = NULL;
			}
		}
		delete[] m_pModelCreate;
		m_pModelCreate = NULL;
	}

	// �e�ԍ��̔j��
	if (m_pParent != NULL)
	{
		delete[] m_pParent;
		m_pParent = NULL;
	}

	if (m_pMotionManager != NULL)
	{
		m_pMotionManager->Uninit();
		delete m_pMotionManager;
		m_pMotionManager = NULL;
	}

	if (m_apModel != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{
			if (m_apModel[nCntModel] != NULL)
			{
				m_apModel[nCntModel]->Uninit();
				delete m_apModel[nCntModel];
				m_apModel[nCntModel] = NULL;
			}
		}
		delete[] m_apModel;
		m_apModel = NULL;
	}
}

//=============================================================================
// �v���C���[�ǂݍ��ݏ���
//=============================================================================
HRESULT CPlayerManager::LoadPlayer(char *pStr, CFileLoader *pFileLoader, int nPlayerIdx)
{
	//�����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	int				nNumModel = 0;						//���f���̑���
	int				nCntModel = 0;						//���f���̃J�E���^�[
	int				nCntPlayer = 0;						//�v���C���[�̃J�E���^�[
	int				nNumParts = 0;						//�p�[�c�̑���
	CMotion			*pMotion[CPlayer::MOTION_MAX] = {};	//���[�V�����̃|�C���^���
	LPD3DXMESH		pMesh = NULL;						//���b�V���̃|�C���^���
	LPD3DXBUFFER	pBuffMat = NULL;					//�}�e���A���̃|�C���^���
	DWORD			nNumMat = NULL;						//�}�e���A���̐�

	int				nCntMotion = 0;						//���[�V�����̃J�E���^�[

	while (1)
	{//���[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_MODEL) == 0)
		{//���f���̑�����������
			nNumModel = CFunctionLib::ReadInt(pStr, NUM_MODEL);
			if (m_pModelCreate[nPlayerIdx] == NULL)
			{
				m_pParent = new int[nNumModel];									//�e���f�����������I�m��
				m_pModelCreate[nPlayerIdx] = CModelCreate::Create(nNumModel);	//���f������������
			}
		}
		else if (CFunctionLib::Memcmp(pStr, MODEL_FILENAME) == 0)
		{//���f���t�@�C������������
			if (m_pModelCreate[nPlayerIdx] != NULL)
			{//���f�������̃|�C���^��NULL�łȂ��ꍇ
				if (nCntModel < nNumModel)
				{//���f���J�E���^�[�����f���̑�����菬�����ꍇ
					pStr = CFunctionLib::ReadString(pStr, pStr, MODEL_FILENAME);

					// X�t�@�C���̓ǂݍ���
					D3DXLoadMeshFromX(pStr,
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&pBuffMat,
						NULL,
						&nNumMat,
						&pMesh);

					//���f�������Ƀ��f�����ݒu����
					m_pModelCreate[nPlayerIdx]->SetMesh(pMesh, nCntModel);
					m_pModelCreate[nPlayerIdx]->SetMaterial(pDevice, pBuffMat, nNumMat, nCntModel);

					//���f�����Ŏg�p�����ϐ�������
					pBuffMat = NULL;
					pMesh = NULL;
					nNumMat = 0;

					//���f���J�E���^�[�̉��Z
					nCntModel++;
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStr, CHARACTERSET) == 0)
		{//�L�����N�^�[���̐ݒu��������
			m_apModel = ReadCharacterSet(pStr, pFileLoader, &nNumParts, m_pParent, nPlayerIdx);
		}
		else if (CFunctionLib::Memcmp(pStr, MOTIONSET) == 0)
		{//�L�����N�^�[���̐ݒu��������
			if (nCntMotion < CPlayer::MOTION_MAX)
			{//�J�E���^�[���v���C���[���[�V�������Ⴂ�ꍇ
				pMotion[nCntMotion] = ReadMotionSet(pStr, pFileLoader, &nNumParts, pMotion[nCntMotion], nCntMotion);
			}
			nCntMotion++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{//�X�N���v�g�̏I����������
			break;
		}
	}

	//���[�V�����}�l�[�W���[�Ƀ��[�V������ݒu
	if (m_pMotionManager == NULL)
	{//NULL�̏ꍇ
	 //���[�V�����}�l�[�W���[�̐���
		m_pMotionManager = CMotionManager::Create(CPlayer::MOTION_MAX, nNumParts);
		if (m_pMotionManager != NULL)
		{//NULL�ł͂Ȃ��ꍇ
			for (int nCntMotion = 0; nCntMotion < CPlayer::MOTION_MAX; nCntMotion++)
			{//���[�V�����̐������J��Ԃ�
				if (pMotion[nCntMotion] != NULL)
				{
					// ���[�V�����̐ݒu����
					m_pMotionManager->CpyMotion(pMotion[nCntMotion], nCntMotion);
				}
			}
		}
	}

	//���[�V�����̔j��
	if (pMotion != NULL)
	{
		for (int nCntMotion = 0; nCntMotion < CPlayer::MOTION_MAX; nCntMotion++)
		{
			if (pMotion[nCntMotion] != NULL)
			{
				pMotion[nCntMotion]->Uninit(nNumParts);
				delete pMotion[nCntMotion];
				pMotion[nCntMotion] = NULL;
			}
		}
	}
	return S_OK;
}

//=============================================================================
// �v���C���[�𐶐����鏈��
//=============================================================================
CPlayer *CPlayerManager::SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdx, int nPriority)
{
	CModel **apModelCpy = NULL;                // ���f���N���X�R�s�[�p
	CMotionManager *pMotionManagerCpy = NULL;  // ���[�V�����Ǌ��N���X�R�s�[�p

	// ���f���̐������������m��
	apModelCpy = new CModel*[m_nNumParts];
	if (apModelCpy == NULL) return NULL;
	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
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
	if (m_pMotionManager != NULL)
	{
		pMotionManagerCpy = CMotionManager::Create(CPlayer::MOTION_MAX, m_nNumParts);
		for (int nCntMotion = 0; nCntMotion < CPlayer::MOTION_MAX; nCntMotion++)
		{
			pMotionManagerCpy->CpyMotion(m_pMotionManager->GetMotion()[nCntMotion], nCntMotion);
		}
	}

	// �v���C���[�𐶐�����
	CPlayer *pPlayer = CPlayer::Create(pos, rot, nIdx, apModelCpy, pMotionManagerCpy, m_nNumParts, nPriority);

	return pPlayer;
}

//=============================================================================
// �L�����N�^�[�̓ǂݍ��ݏ���
//=============================================================================
CModel ** CPlayerManager::ReadCharacterSet(char * pStr, CFileLoader * pFileLoader, int * pNumParts, int * pParent, int nPlayerIdx)
{
	CModel **apModel = NULL;	//���f���̃|�C���^
	int	nNumParts = 0;			//�p�[�c�̑���
	int nCntParts = 0;			//�p�[�c�̃J�E���^�[

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_PARTS) == 0)
		{//�ԍ���������
		 //nNumParts = CFunctionLib::ReadInt(pStr, NUM_PARTS);
			*pNumParts = CFunctionLib::ReadInt(pStr, NUM_PARTS);
			m_nNumParts = CFunctionLib::ReadInt(pStr, NUM_PARTS);
			if (apModel == NULL)
			{
				apModel = new CModel*[*pNumParts];
			}
		}
		else if (CFunctionLib::Memcmp(pStr, PARTSSET) == 0)
		{//�p�[�c�̐ݒu��������
			if (nCntParts < *pNumParts)
			{//�p�[�c�J�E���^�[��������菭�Ȃ��ꍇ
			 //�p�[�c�ǂݍ��ݏ���
				apModel[nCntParts] = ReadParts(pStr, pFileLoader, pParent, apModel, m_pModelCreate[nPlayerIdx]);
				//�p�[�c�J�E���^�[���Z
				nCntParts++;
			}
		}
		else if (CFunctionLib::Memcmp(pStr, END_CHARACTERSET) == 0)
		{//�L�����N�^�[���̏I����������
			break;
		}

	}
	return apModel;
}

//=============================================================================
// �p�[�c�̓ǂݍ��ݏ���
//=============================================================================
CModel * CPlayerManager::ReadParts(char * pStr, CFileLoader * pFileLoader, int * pParent, CModel **apModel, CModelCreate *pModelCreate)
{
	CModel *pModel = NULL;							//���f���̃|�C���^���
	int nIdx = 0;									//�ԍ�
	D3DXVECTOR3 pos = INITIALIZE_D3DXVECTOR3;		//�ʒu
	D3DXVECTOR3 rot = INITIALIZE_D3DXVECTOR3;		//����

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, INDEX) == 0)
		{//�ԍ���������
			nIdx = CFunctionLib::ReadInt(pStr, INDEX);
		}
		else if (CFunctionLib::Memcmp(pStr, PARENT) == 0)
		{//�e�q�֌W��������
			pParent[nIdx] = CFunctionLib::ReadInt(pStr, PARENT);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{//�ʒu��������
			pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{//������������
			rot = CFunctionLib::ReadVector3(pStr, ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_PARTSSET) == 0)
		{//�p�[�c�̐ݒu�I����������
			break;
		}
	}

	if (pModel == NULL)
	{//���f���̐���
		pModel = CModel::Create(pos,
			rot,
			pModelCreate->GetMesh(nIdx),
			pModelCreate->GetBuffMat(nIdx),
			pModelCreate->GetNumMat(nIdx),
			pModelCreate->GetTexture(nIdx),
			pModelCreate->GetVtxMax(nIdx),
			pModelCreate->GetVtxMin(nIdx));
		if (pModel != NULL)
		{
			if (pParent[nIdx] != -1)
			{//�e��񂪂���ꍇ
				pModel->SetParent(apModel[pParent[nIdx]]);
			}
		}
	}
	return pModel;
}

//=============================================================================
// ���[�V�����̓ǂݍ��ݏ���
//=============================================================================
CMotion * CPlayerManager::ReadMotionSet(char *pStr, CFileLoader *pFileLoader, int *pNumParts, CMotion *pMotion, int nCntMotion)
{
	bool bLoop = false;				//���[�v���邩�ǂ���
	int nNumKey = false;			//�L�[����
	int nCntKeySet = 0;				//�L�[�ݒu�̃J�E���^�[
	int *pPlayBackKey = 0;			//�Đ��t���[����
	D3DXVECTOR3 **pAddPos = NULL;	//�ʒu
	D3DXVECTOR3 **pDestRot = NULL;	//����

	while (1)
	{//���[�v�J�n
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, LOOP) == 0)
		{//���[�v���邩�ǂ���
			bLoop = CFunctionLib::ReadBool(pStr, LOOP);
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_KEY) == 0)
		{//�L�[�̑�����������
			nNumKey = CFunctionLib::ReadInt(pStr, NUM_KEY);

			if (pPlayBackKey == NULL)
			{
				pPlayBackKey = new int[nNumKey];
			}

			if (pAddPos == NULL)
			{//NULL�̏ꍇ
				pAddPos = new D3DXVECTOR3*[nNumKey];	//�L�[�̑��������I�m��
				if (pAddPos != NULL)
				{//NULL�ł͂Ȃ��ꍇ
					for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
					{//�L�[�̑������J��Ԃ�
						pAddPos[nCntKey] = new D3DXVECTOR3[*pNumParts];	//�p�[�c���������I�m��
					}
				}
			}

			if (pDestRot == NULL)
			{//NULL�̏ꍇ
				pDestRot = new D3DXVECTOR3*[nNumKey];	//�L�[�̑��������I�m��
				if (pDestRot != NULL)
				{//NULL�ł͂Ȃ��ꍇ
					for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
					{//�L�[�̑������J��Ԃ�
						pDestRot[nCntKey] = new D3DXVECTOR3[*pNumParts];	//�p�[�c���������I�m��
					}
				}
			}

		}
		else if (CFunctionLib::Memcmp(pStr, KEYSET) == 0)
		{//�L�[�̐ݒu��������
			ReadKeySet(pStr, pFileLoader, nCntKeySet, pPlayBackKey, pAddPos[nCntKeySet], pDestRot[nCntKeySet]);
			nCntKeySet++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_MOTIONSET) == 0)
		{//���[�V�������̏I����������
			break;
		}
	}

	if (pMotion == NULL)
	{// ���������m�ۂł����Ԃł���
		pMotion = CMotion::Create(*pNumParts, nNumKey, 0.35f, bLoop, pPlayBackKey);
		if (pMotion != NULL)
		{// ���������m�ۂł���
			for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
			{// �L�[�t���[���̑������J��Ԃ�
				for (int nCntParts = 0; nCntParts < *pNumParts; nCntParts++)
				{// �L�[�t���[���̑������J��Ԃ�
					pMotion->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pAddPos[nCntKey][nCntParts]);
					pMotion->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pDestRot[nCntKey][nCntParts]);
				}
			}
		}
	}

	//�Đ��t���[���̔j��
	if (pPlayBackKey != NULL)
	{
		delete pPlayBackKey;
		pPlayBackKey = NULL;
	}

	//�ʒu�̔j��
	if (pAddPos != NULL)
	{
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{
			delete pAddPos[nCntKey];
			pAddPos[nCntKey] = NULL;
		}
		delete[] pAddPos;
		pAddPos = NULL;
	}

	//�����̔j��
	if (pDestRot != NULL)
	{
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{
			delete pDestRot[nCntKey];
			pDestRot[nCntKey] = NULL;
		}
		delete[] pDestRot;
		pDestRot = NULL;
	}

	return pMotion;
}

//=============================================================================
// �L�[�ݒu�̏���
//=============================================================================
void CPlayerManager::ReadKeySet(char *pStr, CFileLoader *pFileLoader, int nCntKeySet, int *pPlayBackKey, D3DXVECTOR3 *pAddPos, D3DXVECTOR3 *pDestRot)
{
	int nCntKey = 0;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, FRAME) == 0)
		{//�t���[������������
			pPlayBackKey[nCntKeySet] = CFunctionLib::ReadInt(pStr, FRAME);
		}
		else if (CFunctionLib::Memcmp(pStr, KEY) == 0)
		{//�L�[�̏�񂾂�����
			ReadKey(pStr, pFileLoader, nCntKey, pAddPos, pDestRot);
			nCntKey++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_KEYSET) == 0)
		{//�L�[�̐ݒu�I����������
			break;
		}
	}
}

//=============================================================================
// �L�[���̏���
//=============================================================================
void CPlayerManager::ReadKey(char *pStr, CFileLoader *pFileLoader, int nCntKey, D3DXVECTOR3 *pAddPos, D3DXVECTOR3 *pDestRot)
{
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));

		if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{//�ʒu��������
			pAddPos[nCntKey] = CFunctionLib::ReadVector3(pStr, POS);
			D3DXVECTOR3 addPos = pAddPos[nCntKey];
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{//������������
			pDestRot[nCntKey] = CFunctionLib::ReadVector3(pStr, ROT);
			D3DXVECTOR3 addRot = pAddPos[nCntKey];
		}
		else if (CFunctionLib::Memcmp(pStr, END_KEY) == 0)
		{//�L�[�̏I����������
			break;
		}
	}
}

//=============================================================================
// �v���C���[��������
//=============================================================================
void CPlayerManager::CreatePlayer(CModel **apModel, CModelCreate *pModelCreate, CMotionManager *pMotionManager, int nNumParts, int *pParent, int nPlayer)
{
	CModel **apModelCopy = NULL;					//���f�������̃|�C���^���
	CMotionManager *pMotionManagerCopy = NULL;		//���[�V�����}�l�[�W���[�����̃|�C���^���
	CPlayer *pPlayer = NULL;						//�v���C���[�����̃|�C���^���
	CMotion *pMotionCopy[CPlayer::MOTION_MAX] = {};	//���[�V���������̃|�C���^���

	if (apModelCopy == NULL)
	{
		apModelCopy = new CModel*[nNumParts];		//�p�[�c�����I�m��
		if (apModelCopy != NULL)
		{
			for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
			{//�p�[�c�̑������J��Ԃ�
				apModelCopy[nCntParts] = NULL;
				if (apModelCopy[nCntParts] == NULL)
				{
					//���f�������̏���
					apModelCopy[nCntParts] = CModel::Create(apModel[nCntParts]->GetPos(),
						apModel[nCntParts]->GetRot(),
						pModelCreate->GetMesh(nCntParts),
						pModelCreate->GetBuffMat(nCntParts),
						pModelCreate->GetNumMat(nCntParts),
						pModelCreate->GetTexture(nCntParts),
						pModelCreate->GetVtxMax(nCntParts),
						pModelCreate->GetVtxMin(nCntParts));

					if (apModelCopy[nCntParts] != NULL)
					{
						if (pParent[nCntParts] != -1)
						{//�e��񂪂���ꍇ
							apModelCopy[nCntParts]->SetParent(apModelCopy[pParent[nCntParts]]);
						}
					}
				}
			}
		}
	}

	if (pMotionManagerCopy == NULL)
	{
		//���[�V�����}�l�[�W���[�̐���
		pMotionManagerCopy = CMotionManager::Create(CPlayer::MOTION_MAX, nNumParts);
		if (pMotionManagerCopy != NULL)
		{
			for (int nCntMotion = 0; nCntMotion < CPlayer::MOTION_MAX; nCntMotion++)
			{//���[�V�����̐���
				pMotionCopy[nCntMotion] = CMotion::Create(nNumParts,
					pMotionManager->GetMotion()[nCntMotion]->GetNumKey(),
					pMotionManager->GetMotion()[nCntMotion]->GetBlendMag(),
					pMotionManager->GetMotion()[nCntMotion]->GetLoop(),
					pMotionManager->GetMotion()[nCntMotion]->GetPlayBackKey());

				if (pMotionCopy != NULL)
				{
					for (int nCntKey = 0; nCntKey < pMotionManager->GetMotion()[nCntMotion]->GetNumKey(); nCntKey++)
					{
						for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
						{
							pMotionCopy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetAddPos());
							pMotionCopy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetDestRot());
						}
					}
					//pMotionManagerCopy->SetMotion(pMotionCopy[nCntMotion], nCntMotion);
					pMotionManagerCopy->CpyMotion(pMotionCopy[nCntMotion], nCntMotion);
				}
			}
		}
	}
	m_apModel = apModelCopy;				//���f���|�C���^�̑��
	m_pMotionManager = pMotionManagerCopy;	//���[�V�����}�l�[�W���[�̑��
	m_nNumParts = nNumParts;				//�p�[�c�̑��
}

//=============================================================================
// ���f�����̎擾����
//=============================================================================
CModel **CPlayerManager::GetModel(void)
{
	return m_apModel;
}
//=============================================================================
// ���[�V�����}�l�[�W���[�̎擾����
//=============================================================================
CMotionManager *CPlayerManager::GetMotionManager(void)
{
	return m_pMotionManager;
}

//=============================================================================
// �p�[�c���̎擾����
//=============================================================================
int CPlayerManager::GetParts(void)
{
	return m_nNumParts;
}

