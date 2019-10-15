//*****************************************************************************
//
//     �}�b�v�̏���[map.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "map.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
#include "modelcreate.h"
#include "light.h"
#include "lightManager.h"
#include "characterManager.h"
#include "block.h"
#include "river.h"
#include "icefield.h"
#include "boxCollider.h"
#include "object3D.h"
#include "respawn.h"
#include "headquarters.h"
#include "meshfield.h"
#include "object.h"
#include "billboardObject.h"
#include "emitter.h"
#include "effectManager.h"
#include "basemode.h"
#include "sky.h"
#include "scene.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
// �I�[�g�Z�[�u�p�}�N��
#define AUTOSAVE                                                // �I�[�g�Z�[�u�����邩�ǂ���(�錾�� : �I�[�g�Z�[�u)
#define AUTOSAVEFILENAME_MAP       "autosave_map.txt"           // �}�b�v���I�[�g�Z�[�u�p�̃t�@�C����
#define AUTOSAVEFILENAME_GAMEFIELD "autosave_gamefield.txt"     // �Q�[���t�B�[���h���I�[�g�Z�[�u�p�̃t�@�C����
#define AUTOSAVEFILENAME_MODELLIST "autosave_modellist.txt"     // ���f�����X�g���I�[�g�Z�[�u�p�̃t�@�C����
#define AUTOSAVEFILENAME_TEXLIST   "autosave_texlist.txt"       // �e�N�X�`�����X�g���I�[�g�Z�[�u�p�̃t�@�C����
#define AUTOSAVEFILENAME_LIGHT     "autosave_light.txt"         // ���C�g���I�[�g�Z�[�u�p�̃t�@�C����
#define AUTOSAVEFILENAME_OBJECT    "autosave_object.txt"        // �z�u�����I�[�g�Z�[�u�p�̃t�@�C����
#define AUTOSAVEFILENAME_ENEMYLIST "autosave_enemylist.txt"     // �G�̐������I�[�g�Z�[�u�p�̃t�@�C����

// �X�N���v�g�ǂݍ��ݗp
#define BASE_POS "BASE_POS = "
#define PLAYER_RESPAWN "PLAYER_RESPAWN = "
#define ENEMY_RESPAWN "ENEMY_RESPAWN = "
#define MODELLIST_FILENAME "MODELLIST_FILENAME = "
#define TEXLIST_FILENAME "TEXLIST_FILENAME = "
#define MAPSET "MAPSET"
#define END_MAPSET "END_MAPSET"
#define LIGHT_FILENAME "LIGHT_FILENAME = "
#define GAMEFIELD_FILENAME "GAMEFIELD_FILENAME = "
#define OBJECT_FILENAME "OBJECT_FILENAME = "
#define ENEMYLIST_FILENAME "ENEMYLIST_FILENAME = "

// ���ʏ��
#define POS "POS = "
#define ROT "ROT = "

// ���f�����X�g���
#define NUM_MODEL "NUM_MODEL = "
#define MODEL_FILENAME "MODEL_FILENAME = "

// �e�N�X�`�����X�g���
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// ����
#define SKYSET "SKYSET"
#define END_SKYSET "END_SKYSET"
#define ROLLSPEED "ROLLSPEED = "
#define YBLOCK "YBLOCK = "
#define RADIUS "RADIUS = "

// ���C�g���
#define NUM_LIGHT "NUM_LIGHT = "
#define DIRECTIONALLIGHTSET "DIRECTIONALLIGHTSET"
#define END_DIRECTIONALLIGHTSET "END_DIRECTIONALLIGHTSET"
#define POINTLIGHTSET "POINTLIGHTSET"
#define END_POINTLIGHTSET "END_POINTLIGHTSET"
#define SPOTLIGHTSET "SPOTLIGHTSET"
#define END_SPOTLIGHTSET "END_SPOTLIGHTSET"
#define DIFFUSE "DIFFUSE = "
#define AMBIENT "AMBIENT = "
#define SPECULAR "SPECULAR = "
#define ATTENUATION1 "ATTENUATION1 = "
#define ATTENUATION2 "ATTENUATION2 = "
#define ATTENUATION3 "ATTENUATION3 = "
#define RANGE "RANGE = "
#define FALLOFF "FALLOFF = "
#define THETA "THETA = "
#define PHI "PHI = "
#define DIR "DIR = "

// �u���b�N���
#define BLOCKSET "BLOCKSET"
#define END_BLOCKSET "END_BLOCKSET"
#define BLOCKTYPE "TYPE = "
#define MODELIDX "MODELIDX = "
#define BREAK "BREAK = "
#define COLLISION "COLLISION"
#define END_COLLISION "END_COLLISION"
#define WIDTH "WIDTH = "
#define HEIGHT "HEIGHT = "
#define DEPTH "DEPTH = "

// �n�ʏ��
#define FIELDSET "FIELDSET"
#define TEX_IDX "TEX_IDX = "
#define XBLOCK "XBLOCK = "
#define ZBLOCK "ZBLOCK = "
#define END_FIELDSET "END_FIELDSET"

// ����
#define RIVERSET "RIVERSET"
#define END_RIVERSET "END_RIVERSET"

// �X���
#define ICESET "ICESET"
#define END_ICESET "END_ICESET"

// �z�u�����
#define NUM_OBJECT_DATA "NUM_OBJECT_DATA = "
#define OBJECT_FILENAME "OBJECT_FILENAME = "

// �z�u���f�����
#define MODELSET "MODELSET"
#define END_MODELSET "END_MODELSET"

// �z�u�r���{�[�h���
#define BILLBOARDSET "BILLBOARDSET"
#define END_BILLBOARDSET "END_BILLBOARDSET"
#define COL "COL = "
#define LIGHTING "LIGHTING = "
#define DRAW_ADDTIVE "DRAW_ADDTIVE = "

// �z�u�G�t�F�N�g���
#define EFFECTSET "EFFECTSET"
#define END_EFFECTSET "END_EFFECTSET"

// �G�̐������
#define NUM_ENEMYLIST "NUM_ENEMYLIST = "
#define ENEMYLISTSET "ENEMYLISTSET"
#define END_ENEMYLISTSET "END_ENEMYLISTSET"
#define RESPAWN "RESPAWN = "
#define TYPE "TYPE = "
#define TIME "TIME = "
#define ITEM "ITEM"
#define END_ITEM "END_ITEM"
#define AISET "AISET"
#define END_AISET "END_AISET"
#define BULLET "BULLET = "
#define MASS "MASS = "
#define DOWN "DOWN = "

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//*****************************************************************************
//    CMap�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CMap::CMap()
{
	m_nFieldTexIdx = 0;                    // �n�ʂɒ���t����e�N�X�`���̔ԍ�
	strcpy(m_aFileName, "\0");             // �ǂݍ��ރ}�b�v�f�[�^�̃t�@�C����
	strcpy(m_aModelListFileName, "\0");    // �ǂݍ��ރ��f�����X�g���̃t�@�C����
	strcpy(m_aTexListFileName, "\0");      // �ǂݍ��ރe�N�X�`�����X�g���̃t�@�C����
	strcpy(m_aLightFileName, "\0");        // �ǂݍ��ރ��C�g���̃t�@�C����
	strcpy(m_aGameFieldFileName, "\0");    // �ǂݍ��ރQ�[���t�B�[���h���̃t�@�C����
	strcpy(m_aObjectFileName, "\0");       // �ǂݍ��ޔz�u�����̃t�@�C����
	strcpy(m_aEnemyListFileName, "\0");    // �ǂݍ��ޓG�̐������̃t�@�C����
	m_pMeshField = NULL;                   // �n�ʃN���X�ւ̃|�C���^
	m_pSky = NULL;                         // ��N���X�ւ̃|�C���^
	m_pTextureManager = NULL;              // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	m_pModelCreate = NULL;                 // ���f���Ǌ��N���X�ւ̃|�C���^
	m_pLightManager = NULL;                // ���C�g�Ǌ��N���X�ւ̃|�C���^
	m_pObjectManager = NULL;               // �I�u�W�F�N�g�f�[�^�Ǌ��N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMap::~CMap()
{

}

//=============================================================================
//    ��������
//=============================================================================
CMap *CMap::Create(char *pFileName)
{
	CMap *pMap = NULL;  // �}�b�v�N���X�^�̃|�C���^
	if (pMap == NULL)
	{// ����������ɂȂ��Ă���
		pMap = new CMap;
		if (pMap != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pMap->Init(pFileName)))
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

	return pMap;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CMap::Init(char *pFileName)
{
	// �t�@�C�������R�s�[����
	strcpy(m_aFileName, pFileName);

	// �}�b�v�f�[�^��ǂݍ���
	if (FAILED(Load(m_aFileName)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CMap::Uninit(void)
{
#ifdef AUTOSAVE
	// �I�[�g�Z�[�u�̃t�@�C������ݒ肷��
	strcpy(m_aGameFieldFileName, AUTOSAVEFILENAME_GAMEFIELD);
	strcpy(m_aModelListFileName, AUTOSAVEFILENAME_MODELLIST);
	strcpy(m_aTexListFileName, AUTOSAVEFILENAME_TEXLIST);
	strcpy(m_aLightFileName, AUTOSAVEFILENAME_LIGHT);
	strcpy(m_aObjectFileName, AUTOSAVEFILENAME_OBJECT);
	strcpy(m_aEnemyListFileName, AUTOSAVEFILENAME_ENEMYLIST);

	// �f�[�^��ۑ�����
	Save(AUTOSAVEFILENAME_MAP);
#endif

	// �}�b�v���폜����
	DeleteMap();

	// �e�N�X�`���Ǌ��N���X�̔j��
	ReleaseTextureManager();

	// ���f���Ǌ��N���X�̔j��
	ReleaseModelManager();

	// �z�u���Ǌ��N���X�̔j��
	ReleaseObjectManager();
}

//=============================================================================
//    �}�b�v�f�[�^�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::Load(char *pLoadFileName)
{
	CFileLoader *pFileLoader = NULL;  // �t�@�C���ǂݍ��ݗp�N���X�ւ̃|�C���^
	pFileLoader = CFileLoader::Create(pLoadFileName);
	if (pFileLoader != NULL)
	{// �t�@�C�����ǂݍ��߂�
		char aStr[256];
		strcpy(aStr, pFileLoader->GetString(aStr));
		if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}��������
			LoadScript(aStr, pFileLoader);
		}

		// �������̊J��
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �}�b�v�f�[�^�ۑ�����
//=============================================================================
HRESULT CMap::Save(char *pSaveFileName)
{
	char aStr[256] = SAVEFILENAME_MAP;
	CFileSaver *pFileSaver = NULL;  // �t�@�C���ۑ��p�N���X�ւ̃|�C���^
	pFileSaver = CFileSaver::Create(strcat(aStr, pSaveFileName));
	if (pFileSaver != NULL)
	{// �t�@�C���𐶐��ł���
	    // �t�@�C���̖`��������������
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# �}�b�v���X�N���v�g�t�@�C�� [%s]\n", pSaveFileName);
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# ���̍s�͐�Ώ����Ȃ����ƁI\n", SCRIPT);
		pFileSaver->Print("\n");

		// ���f�����̃X�N���v�g�t�@�C������ۑ�
		SaveModelList(pFileSaver);

		// �e�N�X�`�����̃X�N���v�g�t�@�C������ۑ�
		SaveTexList(pFileSaver);

		// �i�ߕ��̈ʒu��ۑ�
		SaveHeadQuartersPos(pFileSaver);

		// �v���C���[�̃��X�|�[���ʒu��ۑ�
		SavePlayerRespawn(pFileSaver);

		// �G�̃��X�|�[���ʒu��ۑ�
		SaveEnemyRespawn(pFileSaver);

		// ����ۑ�
		SaveSky(pFileSaver);

		// �}�b�v����ۑ�
		SaveMap(pFileSaver);

		// �X�N���v�g�I���̍��}����������
		pFileSaver->Print("\n%s		# ���̍s�͐�Ώ����Ȃ����ƁI\n", END_SCRIPT);

		// �������̊J��
		if (pFileSaver != NULL)
		{
			pFileSaver->Uninit();
			delete pFileSaver;
			pFileSaver = NULL;
		}

		// �Q�[���t�B�[���h����ۑ�
		SaveGameField(m_aGameFieldFileName);

		// ���f�����X�g����ۑ�
		SaveModel(m_aModelListFileName);

		// �e�N�X�`�����X�g����ۑ�
		SaveTexture(m_aTexListFileName);

		// ���C�g����ۑ�
		SaveLight(m_aLightFileName);

		// �z�u������ۑ�
		SaveObject(m_aObjectFileName);

		// �G�̐�������ۑ�
		SaveEnemyList(m_aEnemyListFileName);
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �}�b�v�؂�ւ�����
//=============================================================================
HRESULT CMap::ChangeMap(char *pLoadFileName)
{
	// �t�@�C���I�[�v���ł��邩�`�F�b�N
	CFileLoader *pFileLoader = CFileLoader::Create(pLoadFileName);
	if (pFileLoader == NULL)
	{// �t�@�C���I�[�v���Ɏ��s(�������~)
		return E_FAIL;
	}
	else
	{// �t�@�C���I�[�v���ɐ���(���������J�����������s)
		pFileLoader->Uninit();
		delete pFileLoader;
		pFileLoader = NULL;
	}

	// ��������Ă���I�u�W�F�N�g�����ׂĔj������
	DeleteMap();

	// �V���Ƀ}�b�v�f�[�^��ǂݍ���
	m_nFieldTexIdx = 0;                       // �n�ʂ̃e�N�X�`���ԍ����Z�b�g
	strcpy(m_aFileName, pLoadFileName);       // �t�@�C�����R�s�[
	if (FAILED(Load(pLoadFileName)))
	{
		return E_FAIL;
	}

	return S_OK;
}


//=============================================================================
//    �X�N���v�g���ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadScript(char *pStr, CFileLoader *pFileLoader)
{
	int nCntLoadPlayerRes = 0;  // �v���C���[�̃��X�|�[���ʒu��ǂݍ��񂾉�
	int nCntLoadEnemyRes = 0;   // �G�̃��X�|�[���ʒu��ǂݍ��񂾉�
	char aStr[512];
	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, MODELLIST_FILENAME) == 0)
		{// ���f�����X�g��񂾂���
			LoadModelList(CFunctionLib::ReadString(pStr, aStr, MODELLIST_FILENAME), pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, TEXLIST_FILENAME) == 0)
		{// �e�N�X�`�����X�g��񂾂���
			LoadTextureList(CFunctionLib::ReadString(pStr, aStr, TEXLIST_FILENAME), pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, BASE_POS) == 0)
		{// ��n�̈ʒu��񂾂���
			LoadHeadQuartersPos(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYER_RESPAWN) == 0)
		{// �v���C���[�̃��X�|�[���ʒu��񂾂���
			LoadPlayerRespawn(pStr, nCntLoadPlayerRes);
			nCntLoadPlayerRes++;
		}
		else if (CFunctionLib::Memcmp(pStr, ENEMY_RESPAWN) == 0)
		{// �G�̃��X�|�[���ʒu��񂾂���
			LoadEnemyRespawn(pStr, nCntLoadEnemyRes);
			nCntLoadEnemyRes++;
		}
		else if (CFunctionLib::Memcmp(pStr, SKYSET) == 0)
		{// ���񂾂���
			LoadSky(pStr, pFileLoader);
		}
		else if (CFunctionLib::Memcmp(pStr, MAPSET) == 0)
		{// �}�b�v�Z�b�g��񂾂���
			LoadMap(pStr, pFileLoader);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��ݏI���̍��}��������
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    ���f�����X�g�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadModelList(char *pModelFileName, char *pStr)
{
	CFileLoader *pFileLoader = NULL;  // �t�@�C���ǂݍ��ݗp�N���X�ւ̃|�C���^
	pFileLoader = CFileLoader::Create(pModelFileName);
	if (pFileLoader != NULL)
	{// �t�@�C�����ǂݍ��߂�
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0 && m_pModelCreate == NULL)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}��������
			LoadModel(pStr, pFileLoader);
		}

		// �������̊J��
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}

		// �t�@�C�������R�s�[���Ă���
		strcpy(m_aModelListFileName, pModelFileName);
	}
	return S_OK;
}

//=============================================================================
//    ���f���ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadModel(char *pStr, CFileLoader *pFileLoader)
{
	// ���f���ǂݍ��ݗp�ϐ��錾
	int nCntModel = 0;             // ���ݓǂݍ��񂾃��f����
	LPD3DXMESH pMesh = NULL;       // ���b�V���ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = NULL;  // �}�e���A�����ւ̃|�C���^
	DWORD nNumMat = 0;             // �}�e���A�����̐�

	while (1)
	{// ������܂Ń��[�v
		char aStr[256];
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_MODEL) == 0)
		{// �ǂݍ��ރ��f������񂪂�����
			int nNumModel = CFunctionLib::ReadInt(pStr, NUM_MODEL);
			m_pModelCreate = CModelCreate::Create(nNumModel);
		}
		else if (CFunctionLib::Memcmp(pStr, MODEL_FILENAME) == 0)
		{// �ǂݍ��ރ��f���̃t�@�C������񂪂�����
		    // ���f���̃t�@�C���p�X����ǂݎ��
			pStr = CFunctionLib::ReadString(pStr, aStr, MODEL_FILENAME);

			// x�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(pStr, D3DXMESH_SYSTEMMEM, CManager::GetRenderer()->GetDevice(), NULL,
				&pBuffMat, NULL, &nNumMat, &pMesh);

			// ���f���Ǌ��N���X�Ƀ|�C���^��ݒ肷��
			m_pModelCreate->SetMesh(pMesh, nCntModel);
			m_pModelCreate->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);

			// �t�@�C������ݒ肷��
			m_pModelCreate->SetFileName(pStr, nCntModel);

			// �g�p�����ϐ������������Ă���
			pMesh = NULL;
			pBuffMat = NULL;
			nNumMat = 0;

			// �J�E���^�[�𑝂₷
			nCntModel++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��ݏI���̍��}��������
			break;
		}
	}
	return S_OK;
}

//=============================================================================
//    �e�N�X�`�����X�g�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadTextureList(char *pTextureFileName, char *pStr)
{
	CFileLoader *pFileLoader = NULL;  // �t�@�C���ǂݍ��ݗp�N���X�ւ̃|�C���^
	pFileLoader = CFileLoader::Create(pTextureFileName);
	if (pFileLoader != NULL)
	{// �t�@�C�����ǂݍ��߂�
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0 && m_pTextureManager == NULL)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}��������
			LoadTexture(pStr, pFileLoader);
		}

		// �������̊J��
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}

		// �t�@�C�������R�s�[���Ă���
		strcpy(m_aTexListFileName, pTextureFileName);
	}
	return S_OK;
}

//=============================================================================
//    �e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadTexture(char *pStr, CFileLoader *pFileLoader)
{
	// �e�N�X�`���ǂݍ��ݗp�ϐ��錾
	int nCntTex = 0;                      // ���ݓǂݍ��񂾃��f����
	LPDIRECT3DTEXTURE9 pTexture = NULL;   // �e�N�X�`���ւ̃|�C���^

	while (1)
	{// ������܂Ń��[�v
		char aStr[256];
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_TEXTURE) == 0)
		{// �ǂݍ��ރe�N�X�`������񂪂�����
			int nNumTex = CFunctionLib::ReadInt(pStr, NUM_TEXTURE);
			m_pTextureManager = CTextureManager::Create(nNumTex);
		}
		else if (CFunctionLib::Memcmp(pStr, TEXTURE_FILENAME) == 0)
		{// �ǂݍ��ރe�N�X�`���̃t�@�C������񂪂�����
		    // �e�N�X�`���̃t�@�C���p�X����ǂݎ��
			pStr = CFunctionLib::ReadString(pStr, aStr, TEXTURE_FILENAME);

			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pStr, &pTexture);

			// �e�N�X�`���Ǌ��N���X�Ƀ|�C���^��ݒ肷��
			m_pTextureManager->SetTexture(pTexture, nCntTex);

			// �t�@�C������ݒ肷��
			m_pTextureManager->SetFileName(pStr, nCntTex);

			// �g�p�����ϐ������������Ă���
			pTexture = NULL;

			// �J�E���^�[�𑝂₷
			nCntTex++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��ݏI���̍��}��������
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    ��n�̈ʒu�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadHeadQuartersPos(char *pStr)
{
	// �i�ߕ��̈ʒu��ݒ�
	int nWord = 0;
	int nAreaX = CFunctionLib::ReadInt(pStr, BASE_POS);
	pStr = CFunctionLib::HeadPutout(pStr, BASE_POS);
	nWord = CFunctionLib::PopString(pStr, " ");
	pStr += nWord;
	int nAreaZ = CFunctionLib::ReadInt(pStr, "");

	// �i�ߕ��𐶐�
	m_pHeadQuarters = CHeadQuarters::Create(nAreaX, nAreaZ, m_pModelCreate->GetMesh(0), m_pModelCreate->GetBuffMat(0),
		m_pModelCreate->GetNumMat(0), m_pModelCreate->GetTexture(0));

	return S_OK;
}

//=============================================================================
//    �v���C���[�̃��X�|�[���ʒu(2�l��)�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadPlayerRespawn(char *pStr, int nCntPlayerRes)
{
	// �v���C���[�̃��X�|�[���ʒu��ݒ�
	int nWord = 0;
	int nAreaX = CFunctionLib::ReadInt(pStr, PLAYER_RESPAWN);
	pStr = CFunctionLib::HeadPutout(pStr, PLAYER_RESPAWN);
	nWord = CFunctionLib::PopString(pStr, " ");
	pStr += nWord;
	int nAreaZ = CFunctionLib::ReadInt(pStr, "");
	D3DXVECTOR3 Pos = CFunctionLib::ReadVector3(pStr, PLAYER_RESPAWN);
	m_pPlayerRespawn[nCntPlayerRes] = CRespawn::Create(nAreaX, nAreaZ);

	return S_OK;
}

//=============================================================================
//    �G�̃��X�|�[���ʒu(3�ӏ���)�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadEnemyRespawn(char *pStr, int nCntEnemyRes)
{
	// �G�̃��X�|�[���ʒu��ǂݍ���
	int nWord = 0;
	int nAreaX = CFunctionLib::ReadInt(pStr, ENEMY_RESPAWN);
	pStr = CFunctionLib::HeadPutout(pStr, ENEMY_RESPAWN);
	nWord = CFunctionLib::PopString(pStr, " ");
	pStr += nWord;
	int nAreaZ = CFunctionLib::ReadInt(pStr, "");
	D3DXVECTOR3 Pos = CFunctionLib::ReadVector3(pStr, ENEMY_RESPAWN);
	m_pEnemyRespawn[nCntEnemyRes] = CRespawn::Create(nAreaX, nAreaZ);

	return S_OK;
}

//=============================================================================
//    ����ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadSky(char *pStr, CFileLoader *pFileLoader)
{
	int nSkyTexIdx = 0;
	D3DXVECTOR3 SkyPos = INITIALIZE_D3DXVECTOR3;
	D3DXVECTOR3 SkyRot = INITIALIZE_D3DXVECTOR3;
	float fSkyRollSpeed = 0.0f;
	int nSkyXBlock = 0;
	int nSkyYBlock = 0;
	float fSkyRadius = 0.0f;

	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// ���W��񂾂���
			nSkyTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ���W��񂾂���
			SkyPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{// ������񂾂���
			SkyRot = CFunctionLib::ReadVector3(pStr, ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, ROLLSPEED) == 0)
		{// ��]�X�s�[�h��񂾂���
			fSkyRollSpeed = CFunctionLib::ReadFloat(pStr, ROLLSPEED);
		}
		else if (CFunctionLib::Memcmp(pStr, XBLOCK) == 0)
		{// ���̕�������񂾂���
			nSkyXBlock = CFunctionLib::ReadInt(pStr, YBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, YBLOCK) == 0)
		{// �c�̕�������񂾂���
			nSkyYBlock = CFunctionLib::ReadInt(pStr, YBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, RADIUS) == 0)
		{// ���a��񂾂���
			fSkyRadius = CFunctionLib::ReadFloat(pStr, RADIUS);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SKYSET) == 0)
		{// ����I���̍��}��������
			m_pSky = CSky::Create(SkyPos, SkyRot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), fSkyRadius, nSkyXBlock, nSkyYBlock,
				nSkyTexIdx, fSkyRollSpeed);
			if (m_pSky != NULL) m_pSky->BindTexture(m_pTextureManager->GetTexture(nSkyTexIdx));
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    �}�b�v���ǂݍ��ݏ�����
//=============================================================================
HRESULT CMap::LoadMap(char *pStr, CFileLoader *pFileLoader)
{
	while (1)
	{// ������܂Ń��[�v
		char aStr[256];
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, LIGHT_FILENAME) == 0)
		{// ���C�g��񂾂���
			LoadLight(CFunctionLib::ReadString(pStr, aStr, LIGHT_FILENAME), pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, GAMEFIELD_FILENAME) == 0)
		{// �Q�[���t�B�[���h��񂾂���
			LoadGameField(CFunctionLib::ReadString(pStr, aStr, GAMEFIELD_FILENAME), pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, OBJECT_FILENAME) == 0)
		{// �z�u����񂾂���
			LoadObject(CFunctionLib::ReadString(pStr, aStr, OBJECT_FILENAME), pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, ENEMYLIST_FILENAME) == 0)
		{// �G�̐�����񂾂���
			LoadEnemyList(CFunctionLib::ReadString(pStr, aStr, ENEMYLIST_FILENAME), pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_MAPSET) == 0)
		{// �}�b�v�Z�b�g���I���̍��}��������
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    ���C�g���ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadLight(char *pLightFileName, char *pStr)
{
	CFileLoader *pFileLoader = NULL;  // �t�@�C���ǂݍ��ݗp�N���X�ւ̃|�C���^
	pFileLoader = CFileLoader::Create(pLightFileName);
	if (pFileLoader != NULL)
	{// �t�@�C�����ǂݍ��߂�
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}��������
			LoadLightInfo(pStr, pFileLoader);
		}

		// �������̊J��
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}

		// �t�@�C�������R�s�[���Ă���
		strcpy(m_aLightFileName, pLightFileName);
	}

	return S_OK;
}

//=============================================================================
//    ���C�g�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadLightInfo(char *pStr, CFileLoader *pFileLoader)
{
	int nCntLight = 0;

	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_LIGHT) == 0)
		{// ���C�g�̐���񂾂���
			int nNumLight = CFunctionLib::ReadInt(pStr, NUM_LIGHT);
			m_pLightManager = CLightManager::Create(nNumLight);
		}
		else if (CFunctionLib::Memcmp(pStr, DIRECTIONALLIGHTSET) == 0)
		{// �f�B���N�V���i�����C�g��񂾂���
			LoadDirectionalLight(pStr, pFileLoader, nCntLight);
			nCntLight++;
		}
		else if (CFunctionLib::Memcmp(pStr, POINTLIGHTSET) == 0)
		{// �|�C���g���C�g��񂾂���
			LoadPointLight(pStr, pFileLoader, nCntLight);
			nCntLight++;
		}
		else if (CFunctionLib::Memcmp(pStr, SPOTLIGHTSET) == 0)
		{// �X�|�b�g���C�g��񂾂���
			LoadSpotLight(pStr, pFileLoader, nCntLight);
			nCntLight++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�I���̍��}��������
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    �f�B���N�V���i�����C�g�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadDirectionalLight(char *pStr, CFileLoader *pFileLoader, int nCntLight)
{
	D3DXVECTOR3 LightDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightAmbient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightSpecular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, DIFFUSE) == 0)
		{// ���C�g�̊g�U����񂾂���
			LightDiffuse = CFunctionLib::ReadVector4(pStr, DIFFUSE);
		}
		else if (CFunctionLib::Memcmp(pStr, AMBIENT) == 0)
		{// ���C�g�̊�����񂾂���
			LightAmbient = CFunctionLib::ReadVector4(pStr, AMBIENT);
		}
		else if (CFunctionLib::Memcmp(pStr, SPECULAR) == 0)
		{// ���C�g�̔��ˌ���񂾂���
			LightSpecular = CFunctionLib::ReadVector4(pStr, SPECULAR);
		}
		else if (CFunctionLib::Memcmp(pStr, DIR) == 0)
		{// ���C�g�̌�����񂾂���
			LightDir = CFunctionLib::ReadVector3(pStr, DIR);
		}
		else if (CFunctionLib::Memcmp(pStr, END_DIRECTIONALLIGHTSET) == 0)
		{// �f�B���N�V���i�����C�g���I���̍��}��������
			CDirectionalLight *pDirLight = CDirectionalLight::Create(LightDir, LightDiffuse,
				                                                     LightAmbient, LightSpecular);
			m_pLightManager->SettingLight(pDirLight, nCntLight);
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    �|�C���g���C�g�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadPointLight(char *pStr, CFileLoader *pFileLoader, int nCntLight)
{
	D3DXVECTOR3 LightPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightAmbient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightSpecular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	float fLightAttenuation0 = 0.0f;
	float fLightAttenuation1 = 0.0f;
	float fLightAttenuation2 = 0.0f;
	float fLightRange = 0.0f;

	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ���C�g�̍��W��񂾂���
			LightPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, DIFFUSE) == 0)
		{// ���C�g�̊g�U����񂾂���
			LightDiffuse = CFunctionLib::ReadVector4(pStr, DIFFUSE);
		}
		else if (CFunctionLib::Memcmp(pStr, AMBIENT) == 0)
		{// ���C�g�̊�����񂾂���
			LightAmbient = CFunctionLib::ReadVector4(pStr, AMBIENT);
		}
		else if (CFunctionLib::Memcmp(pStr, SPECULAR) == 0)
		{// ���C�g�̔��ˌ���񂾂���
			LightSpecular = CFunctionLib::ReadVector4(pStr, SPECULAR);
		}
		else if (CFunctionLib::Memcmp(pStr, ATTENUATION1) == 0)
		{// ���C�g�̐��`�����W����񂾂���
			fLightAttenuation0 = CFunctionLib::ReadFloat(pStr, ATTENUATION1);
		}
		else if (CFunctionLib::Memcmp(pStr, ATTENUATION2) == 0)
		{// ���C�g��2�������W����񂾂���
			fLightAttenuation1 = CFunctionLib::ReadFloat(pStr, ATTENUATION2);
		}
		else if (CFunctionLib::Memcmp(pStr, ATTENUATION3) == 0)
		{// ���C�g�̎w�������W����񂾂���
			fLightAttenuation2 = CFunctionLib::ReadFloat(pStr, ATTENUATION3);
		}
		else if (CFunctionLib::Memcmp(pStr, RANGE) == 0)
		{// ���C�g�͈̔͏�񂾂���
			fLightRange = CFunctionLib::ReadFloat(pStr, RANGE);
		}
		else if (CFunctionLib::Memcmp(pStr, END_POINTLIGHTSET) == 0)
		{// �|�C���g���C�g���I���̍��}��������
			CPointLight *pPointLight = CPointLight::Create(LightPos, LightDiffuse, LightAmbient, LightSpecular,
				                                           fLightAttenuation0, fLightAttenuation1,
				                                           fLightAttenuation2, fLightRange);
			m_pLightManager->SettingLight(pPointLight, nCntLight);
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    �X�|�b�g���C�g�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadSpotLight(char *pStr, CFileLoader *pFileLoader, int nCntLight)
{
	D3DXVECTOR3 LightPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 LightDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightAmbient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightSpecular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	float fLightAttenuation0 = 0.0f;
	float fLightAttenuation1 = 0.0f;
	float fLightAttenuation2 = 0.0f;
	float fLightRange = 0.0f;
	float fLightFalloff = 0.0f;
	float fLightTheta = 0.0f;
	float fLightPhi = 0.0f;

	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ���C�g�̍��W��񂾂���
			LightPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, DIR) == 0)
		{// ���C�g�̌�����񂾂���
			LightDir = CFunctionLib::ReadVector3(pStr, DIR);
		}
		else if (CFunctionLib::Memcmp(pStr, DIFFUSE) == 0)
		{// ���C�g�̊g�U����񂾂���
			LightDiffuse = CFunctionLib::ReadVector4(pStr, DIFFUSE);
		}
		else if (CFunctionLib::Memcmp(pStr, AMBIENT) == 0)
		{// ���C�g�̊�����񂾂���
			LightAmbient = CFunctionLib::ReadVector4(pStr, AMBIENT);
		}
		else if (CFunctionLib::Memcmp(pStr, SPECULAR) == 0)
		{// ���C�g�̔��ˌ���񂾂���
			LightSpecular = CFunctionLib::ReadVector4(pStr, SPECULAR);
		}
		else if (CFunctionLib::Memcmp(pStr, ATTENUATION1) == 0)
		{// ���C�g�̐��`�����W����񂾂���
			fLightAttenuation0 = CFunctionLib::ReadFloat(pStr, ATTENUATION1);
		}
		else if (CFunctionLib::Memcmp(pStr, ATTENUATION2) == 0)
		{// ���C�g��2�������W����񂾂���
			fLightAttenuation1 = CFunctionLib::ReadFloat(pStr, ATTENUATION2);
		}
		else if (CFunctionLib::Memcmp(pStr, ATTENUATION3) == 0)
		{// ���C�g�̎w�������W����񂾂���
			fLightAttenuation2 = CFunctionLib::ReadFloat(pStr, ATTENUATION3);
		}
		else if (CFunctionLib::Memcmp(pStr, RANGE) == 0)
		{// ���C�g�͈̔͏�񂾂���
			fLightRange = CFunctionLib::ReadFloat(pStr, RANGE);
		}
		else if (CFunctionLib::Memcmp(pStr, FALLOFF) == 0)
		{// ���C�g�̃t�H�[���I�t��񂾂���
			fLightFalloff = CFunctionLib::ReadFloat(pStr, FALLOFF);
		}
		else if (CFunctionLib::Memcmp(pStr, THETA) == 0)
		{// ���C�g�̓����R�[���̊p�x��񂾂���
			fLightTheta = CFunctionLib::ReadFloat(pStr, THETA);
			D3DXToRadian(fLightTheta);
		}
		else if (CFunctionLib::Memcmp(pStr, PHI) == 0)
		{// ���C�g�̊O���R�[���̊p�x��񂾂���
			fLightPhi = CFunctionLib::ReadFloat(pStr, PHI);
			D3DXToRadian(fLightPhi);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SPOTLIGHTSET) == 0)
		{// �X�|�b�g���C�g���I���̍��}��������
			CSpotLight *pSpotLight = CSpotLight::Create(LightPos, LightDir, LightDiffuse, LightAmbient,
				                                        LightSpecular, fLightAttenuation0, fLightAttenuation1,
				                                        fLightAttenuation2, fLightRange,
				                                        fLightFalloff, fLightTheta, fLightPhi);
			m_pLightManager->SettingLight(pSpotLight, nCntLight);
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    �Q�[���t�B�[���h���ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadGameField(char *pGameFieldFileName, char *pStr)
{
	CFileLoader *pFileLoader = NULL;  // �t�@�C���ǂݍ��ݗp�N���X�ւ̃|�C���^
	pFileLoader = CFileLoader::Create(pGameFieldFileName);
	if (pFileLoader != NULL)
	{// �t�@�C�����ǂݍ��߂�
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}��������
			LoadGameFieldInfo(pStr, pFileLoader);
		}

		// �������̊J��
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}

		// �t�@�C�������R�s�[���Ă���
		strcpy(m_aGameFieldFileName, pGameFieldFileName);
	}

	return S_OK;
}

//=============================================================================
//    �Q�[���t�B�[���h�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadGameFieldInfo(char *pStr, CFileLoader *pFileLoader)
{
	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, FIELDSET) == 0)
		{// �n�ʏ�񂾂���
			LoadField(pStr, pFileLoader);
		}
		else if (CFunctionLib::Memcmp(pStr, BLOCKSET) == 0)
		{// �u���b�N��񂾂���
			LoadBlock(pStr, pFileLoader);
		}
		else if (CFunctionLib::Memcmp(pStr, RIVERSET) == 0)
		{// ���񂾂���
			LoadRiver(pStr, pFileLoader);
		}
		else if (CFunctionLib::Memcmp(pStr, ICESET) == 0)
		{// �X��񂾂���
			LoadIce(pStr, pFileLoader);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�I���̍��}��������
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    �n�ʏ��ǂݍ��ݏ���
//=============================================================================
void CMap::LoadField(char *pStr, CFileLoader *pFileLoader)
{
	float fFieldWidth = 0.0f;
	float fFieldDepth = 0.0f;
	int nFieldXBlock = 0;
	int nFieldZBlock = 0;
	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���ԍ���񂾂���
			m_nFieldTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// 1�}�X���̒n�ʂ̉�����񂾂���
			fFieldWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, DEPTH) == 0)
		{// 1�}�X���̒n�ʂ̉��s��񂾂���
			fFieldDepth = CFunctionLib::ReadFloat(pStr, DEPTH);
		}
		else if (CFunctionLib::Memcmp(pStr, XBLOCK) == 0)
		{// ���̕�������񂾂���
			nFieldXBlock = CFunctionLib::ReadInt(pStr, XBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, ZBLOCK) == 0)
		{// ���s�̕�������񂾂���
			nFieldZBlock = CFunctionLib::ReadInt(pStr, ZBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, END_FIELDSET) == 0)
		{// �n�ʏ��I���̍��}��������
			m_pMeshField = CMeshField::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),fFieldWidth, fFieldDepth, nFieldXBlock, nFieldZBlock,
				1, 1, NULL, true);
			m_pMeshField->BindTexture(m_pTextureManager->GetTexture(m_nFieldTexIdx));
			break;
		}
	}
}

//=============================================================================
//    �u���b�N���ǂݍ��ݏ���
//=============================================================================
void CMap::LoadBlock(char *pStr, CFileLoader *pFileLoader)
{
	int nBlockType = 0;
	int nBlockModelIdx = 0;
	D3DXVECTOR3 BlockPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 BlockRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fBlockColWidth = 0.0f;
	float fBlockColHeight = 0.0f;
	float fBlockColDepth = 0.0f;

	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, BLOCKTYPE) == 0)
		{// ��ޔԍ���񂾂���
			nBlockType = CFunctionLib::ReadInt(pStr, BLOCKTYPE);
		}
		else if (CFunctionLib::Memcmp(pStr, MODELIDX) == 0)
		{// �g�p���郂�f���̔ԍ���񂾂���
			nBlockModelIdx = CFunctionLib::ReadInt(pStr, MODELIDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ���W��񂾂���
			BlockPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{// ������񂾂���
			BlockRot = CFunctionLib::ReadVector3(pStr, ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, COLLISION) == 0)
		{// �����蔻�����񂾂���
			LoadCollision(pStr, pFileLoader, &fBlockColWidth, &fBlockColHeight, &fBlockColDepth);
		}
		else if (CFunctionLib::Memcmp(pStr, END_BLOCKSET) == 0)
		{// �u���b�N���I���̍��}��������
			CreateBlock(BlockPos, BlockRot, nBlockType, nBlockModelIdx,
				m_pModelCreate->GetMesh(nBlockModelIdx), m_pModelCreate->GetBuffMat(nBlockModelIdx),
				m_pModelCreate->GetNumMat(nBlockModelIdx), m_pModelCreate->GetTexture(nBlockModelIdx),
				fBlockColWidth, fBlockColHeight, fBlockColDepth);
			break;
		}
	}
}

//=============================================================================
//    �����蔻����ǂݍ��ݏ���
//=============================================================================
void CMap::LoadCollision(char *pStr, CFileLoader *pFileLoader, float *pWidth, float *pHeight, float *pDepth)
{
	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �����蔻��̕���񂾂���
			*pWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �����蔻��̍�����񂾂���
			*pHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, DEPTH) == 0)
		{// �����蔻��̉��s��񂾂���
			*pDepth = CFunctionLib::ReadFloat(pStr, DEPTH);
		}
		else if (CFunctionLib::Memcmp(pStr, END_COLLISION) == 0)
		{// �u���b�N���I���̍��}��������
			break;
		}
	}
}

//=============================================================================
//    ����ǂݍ��ݏ���
//=============================================================================
void CMap::LoadRiver(char *pStr, CFileLoader *pFileLoader)
{
	int nRiverTexIdx = 0;
	D3DXVECTOR3 RiverPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nRiverXBlock = 0;
	int nRiverZBlock = 0;
	float fRiverColWidth = 0.0f;
	float fRiverColHeight = 0.0f;
	float fRiverColDepth = 0.0f;

	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ���񂾂���
			nRiverTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ���W��񂾂���
			RiverPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, XBLOCK) == 0)
		{// ���̕�������񂾂���
			nRiverXBlock = CFunctionLib::ReadInt(pStr, XBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, ZBLOCK) == 0)
		{// ���s�̕�������񂾂���
			nRiverZBlock = CFunctionLib::ReadInt(pStr, ZBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, COLLISION) == 0)
		{// �����蔻���񂾂���
			LoadCollision(pStr, pFileLoader, &fRiverColWidth, &fRiverColHeight, &fRiverColDepth);
		}
		else if (CFunctionLib::Memcmp(pStr, END_RIVERSET) == 0)
		{// ����I���̍��}��������
			SetRiver(nRiverTexIdx, RiverPos, nRiverXBlock, nRiverZBlock, fRiverColWidth, fRiverColHeight, fRiverColDepth);
			break;
		}
	}
}

//=============================================================================
//    �X���ǂݍ��ݏ���
//=============================================================================
void CMap::LoadIce(char *pStr, CFileLoader *pFileLoader)
{
	int nIceTexIdx = 0;
	D3DXVECTOR3 IcePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nIceXBlock = 0;
	int nIceZBlock = 0;
	float fIceColWidth = 0.0f;
	float fIceColHeight = 0.0f;
	float fIceColDepth = 0.0f;

	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ���񂾂���
			nIceTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ���W��񂾂���
			IcePos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, XBLOCK) == 0)
		{// ���̕�������񂾂���
			nIceXBlock = CFunctionLib::ReadInt(pStr, XBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, ZBLOCK) == 0)
		{// ���s�̕�������񂾂���
			nIceZBlock = CFunctionLib::ReadInt(pStr, ZBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, COLLISION) == 0)
		{// �����蔻���񂾂���
			LoadCollision(pStr, pFileLoader, &fIceColWidth, &fIceColHeight, &fIceColDepth);
		}
		else if (CFunctionLib::Memcmp(pStr, END_ICESET) == 0)
		{// �X���I���̍��}��������
			SetIce(nIceTexIdx, IcePos, nIceXBlock, nIceZBlock, fIceColWidth, fIceColHeight, fIceColDepth);
			break;
		}
	}
}

//=============================================================================
//    �n�ʂ������鏈��
//=============================================================================
void CMap::FieldDown(D3DXVECTOR3 pos, int nXBlock, int nZBlock, float fFieldDown)
{
	// ��̍��W����ǂ̃G���A�ɐ삪���邩���擾
	int nXArea = (int)((pos.x + (MASS_SIZE_X * MASS_BLOCK_X / 2)) / MASS_SIZE_X) + 1;
	int nZArea = (int)((-pos.z + (MASS_SIZE_X* MASS_BLOCK_Z / 2)) / MASS_SIZE_X) + 1;
	int nStartLeftVertex = (nXArea - 1) + ((MASS_BLOCK_X + 1) * (nZArea - 1));

	// ���������l�����Ė߂�
	nStartLeftVertex -= (nXBlock / 2);
	nStartLeftVertex -= ((nZBlock / 2)) * (MASS_BLOCK_X + 1);

	// ���_�o�b�t�@�̎擾
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < nZBlock + 1; nCntZ++)
	{// �c�̕�����+1���J��Ԃ�
		for (int nCntX = 0; nCntX < nXBlock + 1; nCntX++)
		{// ���̕�����+1���J��Ԃ�
			pVtx[nStartLeftVertex + nCntX].pos.y = fFieldDown;
		}
		nStartLeftVertex += (MASS_BLOCK_X + 1);
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();
}

//=============================================================================
//    ��z�u����
//=============================================================================
void CMap::SetRiver(int nTexIdx, D3DXVECTOR3 pos, int nXBlock, int nZBlock, float fBoxColWidth, float fBoxColHeight, float fBoxColDepth)
{
	// ��̍��W����n�ʂ̒��_��������
	FieldDown(pos, nXBlock, nZBlock, RIVER_DOWN_FIELD);

	// ���z�u
	CRiver::Create(pos, INITIALIZE_D3DXVECTOR3, m_pTextureManager->GetTexture(nTexIdx),
		fBoxColWidth, fBoxColHeight, fBoxColDepth, nXBlock, nZBlock, nTexIdx);
}

//=============================================================================
//    �X�z�u����
//=============================================================================
void CMap::SetIce(int nTexIdx, D3DXVECTOR3 pos, int nXBlock, int nZBlock, float fBoxColWidth, float fBoxColHeight, float fBoxColDepth)
{
	// �X�̍��W����n�ʂ̒��_��������
	FieldDown(pos, nXBlock, nZBlock, RIVER_DOWN_FIELD);

	// �X��z�u
	CIceField::Create(pos, INITIALIZE_D3DXVECTOR3, m_pTextureManager->GetTexture(nTexIdx),
		fBoxColWidth, fBoxColHeight, fBoxColDepth, nXBlock, nZBlock, nTexIdx);
}

//=============================================================================
//    �z�u���ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadObject(char *pObjectName, char *pStr)
{
	CFileLoader *pFileLoader = NULL;  // �t�@�C���ǂݍ��ݗp�N���X�ւ̃|�C���^
	pFileLoader = CFileLoader::Create(pObjectName);
	if (pFileLoader != NULL)
	{// �t�@�C�����ǂݍ��߂�
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}��������
			LoadObjectInfo(pStr, pFileLoader);
		}

		// �������̊J��
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}

		// �t�@�C�������R�s�[���Ă���
		strcpy(m_aObjectFileName, pObjectName);
	}

	return S_OK;
}

//=============================================================================
//    �z�u�����ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadObjectInfo(char *pStr, CFileLoader *pFileLoader)
{
	int nCntObjData = 0;

	// �G�t�F�N�g�Ǌ��N���X�������Ă���
	CEffectManager *pEffectManager = NULL;
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_EFFECTMANAGER)
			{// �G�t�F�N�g�Ǌ��N���X�������N���X������
				pEffectManager = (CEffectManager*)pScene;
				break;
			}
			pScene = pSceneNext;
		}
	}

	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_OBJECT_DATA) == 0)
		{// �I�u�W�F�N�g�f�[�^�̐���񂾂���
			LoadNumObjectData(pStr, pFileLoader);
		}
		else if (CFunctionLib::Memcmp(pStr, OBJECT_FILENAME) == 0)
		{// �I�u�W�F�N�g�f�[�^�̃t�@�C����������
			LoadObjectData(pStr, pFileLoader, nCntObjData);
			nCntObjData++;
		}
		else if (CFunctionLib::Memcmp(pStr, MODELSET) == 0)
		{// �z�u����񂾂���
			LoadObjModel(pStr, pFileLoader);
		}
		else if (CFunctionLib::Memcmp(pStr, BILLBOARDSET) == 0)
		{// �z�u�r���{�[�h��񂾂���
			LoadObjBill(pStr, pFileLoader);
		}
		else if (CFunctionLib::Memcmp(pStr, EFFECTSET) == 0)
		{// �z�u�G�t�F�N�g��񂾂���
			LoadObjEffect(pStr, pFileLoader, pEffectManager);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g���ǂݍ��ݏI���̍��}��������
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    �z�u���f�[�^�̐����ǂݍ��ݏ���
//=============================================================================
void CMap::LoadNumObjectData(char *pStr, CFileLoader *pFileLoader)
{
	// �f�[�^�̐���ǂݍ���
	m_nNumObjectData = CFunctionLib::ReadInt(pStr, NUM_OBJECT_DATA);
	if (m_nNumObjectData <= 0)return;

	// ���������m�ۂ���
	m_pObjectManager = new CCharacterManager*[m_nNumObjectData];
	if (m_pObjectManager == NULL)return;

	// �������̒��g���N���A���Ă���
	for (int nCntObject = 0; nCntObject < m_nNumObjectData; nCntObject++)
	{
		m_pObjectManager[nCntObject] = NULL;
	}
}

//=============================================================================
//    �z�u���f�[�^�ǂݍ��ݏ���
//=============================================================================
void CMap::LoadObjectData(char *pStr, CFileLoader *pFileLoader, int nCntObjData)
{
	// �t�@�C������ǂݍ���
	char aObjFileName[256] = "\0";
	strcpy(aObjFileName, CFunctionLib::ReadString(pStr, aObjFileName, OBJECT_FILENAME));

	// �z�u���̃f�[�^��ǂݍ���
	if (m_pObjectManager == NULL)return;
	m_pObjectManager[nCntObjData] = CCharacterManager::Create(aObjFileName);
}

//=============================================================================
//    �z�u���f�����ǂݍ��ݏ���
//=============================================================================
void CMap::LoadObjModel(char *pStr, CFileLoader *pFileLoader)
{
	int nObjModelType = 0;
	D3DXVECTOR3 ObjModelPos = INITIALIZE_D3DXVECTOR3;
	D3DXVECTOR3 ObjModelRot = INITIALIZE_D3DXVECTOR3;
	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TYPE) == 0)
		{// �g�p����I�u�W�F�N�g�f�[�^�̎�ޏ�񂾂���
			nObjModelType = CFunctionLib::ReadInt(pStr, TYPE);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ���W������
			ObjModelPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{// ����������
			ObjModelRot = CFunctionLib::ReadVector3(pStr, ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_MODELSET) == 0)
		{// �z�u���f�����I���̍��}������
			break;
		}
	}

	// ���f����z�u����
	if (m_pObjectManager != NULL)
	{
		m_pObjectManager[nObjModelType]->SetObject(ObjModelPos, ObjModelRot, nObjModelType);
	}
}

//=============================================================================
//    �z�u�r���{�[�h���ǂݍ��ݏ���
//=============================================================================
void CMap::LoadObjBill(char *pStr, CFileLoader *pFileLoader)
{
	int nObjBillTexIdx = 0;
	D3DXVECTOR3 ObjBillPos = INITIALIZE_D3DXVECTOR3;
	D3DXCOLOR ObjBillCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	float fObjBillRot = 0.0f;
	float fObjBillWidth = 0.0f;
	float fObjBillHeight = 0.0f;
	bool bObjBillLighting = false;
	bool bObjBillDrawAddtive = false;
	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ���񂾂���
			nObjBillTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ���W������
			ObjBillPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �F������
			ObjBillCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{// ����������
			fObjBillRot = CFunctionLib::ReadFloat(pStr, ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ��������
			fObjBillWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ����������
			fObjBillHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, LIGHTING) == 0)
		{// ���C�e�B���O���邩�ǂ���������
			bObjBillLighting = CFunctionLib::ReadBool(pStr, LIGHTING);
		}
		else if (CFunctionLib::Memcmp(pStr, DRAW_ADDTIVE) == 0)
		{// ���Z�����ŕ`�悷�邩���邩�ǂ���������
			bObjBillDrawAddtive = CFunctionLib::ReadBool(pStr, DRAW_ADDTIVE);
		}
		else if (CFunctionLib::Memcmp(pStr, END_BILLBOARDSET) == 0)
		{// �z�u�r���{�[�h���I���̍��}������
			break;
		}
	}

	// �r���{�[�h��z�u����
	CBillboardObject *pObjBill = CBillboardObject::Create(ObjBillPos, ObjBillCol, fObjBillWidth, fObjBillWidth,
		fObjBillRot, bObjBillLighting, bObjBillDrawAddtive, nObjBillTexIdx);
	if (pObjBill != NULL && m_pTextureManager != NULL)
	{
		pObjBill->BindTexture(m_pTextureManager->GetTexture(nObjBillTexIdx));
	}
}

//=============================================================================
//    �z�u�G�t�F�N�g���ǂݍ��ݏ���
//=============================================================================
void CMap::LoadObjEffect(char *pStr, CFileLoader *pFileLoader, CEffectManager *pEffectManager)
{
	int nObjEffectType = 0;
	D3DXVECTOR3 ObjEffectPos = INITIALIZE_D3DXVECTOR3;
	D3DXVECTOR3 ObjEffectRot = INITIALIZE_D3DXVECTOR3;
	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TYPE) == 0)
		{// �g�p����I�u�W�F�N�g�f�[�^�̎�ޏ�񂾂���
			nObjEffectType = CFunctionLib::ReadInt(pStr, TYPE);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ���W������
			ObjEffectPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{// ����������
			ObjEffectRot = CFunctionLib::ReadVector3(pStr, ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_EFFECTSET) == 0)
		{// �z�u�G�t�F�N�g���I���̍��}������
			break;
		}
	}

	// �G�t�F�N�g��z�u����
	pEffectManager->SetEffect(ObjEffectPos, ObjEffectRot, nObjEffectType);
}

//=============================================================================
//    �G�̐������ǂݍ��݂̂��߂̃t�@�C���I�[�v������
//=============================================================================
HRESULT CMap::LoadEnemyList(char *pEnemylistName, char *pStr)
{
	int nCntEnemyList = 0;            // �G�̐�������ǂݍ��񂾉�
	CFileLoader *pFileLoader = NULL;  // �t�@�C���ǂݍ��ݗp�N���X�ւ̃|�C���^

	// �t�@�C���I�[�v��
	pFileLoader = CFileLoader::Create(pEnemylistName);
	if (pFileLoader == NULL) return E_FAIL;

	// �t�@�C���ǂݍ��݊J�n
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
	{// �X�N���v�g�ǂݍ��݊J�n�̍��}��������
		while (1)
		{// ������܂Ń��[�v
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, NUM_ENEMYLIST) == 0)
			{// �G�̐������̑�����񂾂���
				m_nNumEnemyListData = CFunctionLib::ReadInt(pStr, NUM_ENEMYLIST);
				CreateEnemyListData();
			}
			else if (CFunctionLib::Memcmp(pStr, ENEMYLISTSET) == 0)
			{// �G�̐�����񂾂���
				LoadEnemyListInfo(pStr, pFileLoader, nCntEnemyList);
				nCntEnemyList++;
			}
			else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
			{// �X�N���v�g���ǂݍ��ݏI���̍��}��������
				break;
			}
		}
	}

	// �������̊J��
	if (pFileLoader != NULL)
	{
		pFileLoader->Uninit();
		delete pFileLoader;
		pFileLoader = NULL;
	}

	// �t�@�C�������R�s�[���Ă���
	strcpy(m_aEnemyListFileName, pEnemylistName);

	return S_OK;
}

//=============================================================================
//    �G�̐������ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadEnemyListInfo(char *pStr, CFileLoader *pFileLoader, int nCntEnemyList)
{
	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, RESPAWN) == 0)
		{// ���X�|�[���ʒu�̔ԍ���񂾂���
			m_pEnemyListData[nCntEnemyList]->SetRespawnIdx(CFunctionLib::ReadInt(pStr, RESPAWN));
		}
		else if (CFunctionLib::Memcmp(pStr, TYPE) == 0)
		{// �G�̎�ނ̔ԍ���񂾂���
			m_pEnemyListData[nCntEnemyList]->SetEnemyType(CFunctionLib::ReadInt(pStr, TYPE));
		}
		else if (CFunctionLib::Memcmp(pStr, TIME) == 0)
		{// �G���o��������^�C�~���O��񂾂���
			m_pEnemyListData[nCntEnemyList]->SetRespawnTime(CFunctionLib::ReadInt(pStr, TIME));
		}
		else if (CFunctionLib::Memcmp(pStr, ITEM) == 0)
		{// �A�C�e����񂾂���
			m_pEnemyListData[nCntEnemyList]->SetItem(true);
			LoadEnemyItem(pStr, pFileLoader, nCntEnemyList);
		}
		else if (CFunctionLib::Memcmp(pStr, AISET) == 0)
		{// �G��AI��񂾂���
			LoadEnemyAI(pStr, pFileLoader, nCntEnemyList);
		}
		else if (CFunctionLib::Memcmp(pStr, END_ENEMYLISTSET) == 0)
		{// �G�̐������ǂݍ��ݏI���̍��}������
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    �G�̃A�C�e���������ǂݍ��ݏ���
//=============================================================================
void CMap::LoadEnemyItem(char *pStr, CFileLoader *pFileLoader, int nCntEnemyList)
{
	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TYPE) == 0)
		{// �o��������A�C�e���̎�ޔԍ�������
			m_pEnemyListData[nCntEnemyList]->SetItemType(CFunctionLib::ReadInt(pStr, TYPE));
		}
		else if (CFunctionLib::Memcmp(pStr, END_ITEM) == 0)
		{// �G�̃A�C�e���������ǂݍ��ݏI���̍��}��������
			break;
		}
	}
}

//=============================================================================
//    �G��AI����ǂݍ��ޏ���
//=============================================================================
void CMap::LoadEnemyAI(char *pStr, CFileLoader *pFileLoader, int nCntEnemyList)
{
	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, BULLET) == 0)
		{// �e��łۂ̃����_���]���l������
			m_pEnemyListData[nCntEnemyList]->SetAI_BulletEva(CFunctionLib::ReadInt(pStr, BULLET));
		}
		else if (CFunctionLib::Memcmp(pStr, MASS) == 0)
		{// ������ς������郉���_���]���l������
			m_pEnemyListData[nCntEnemyList]->SetAI_MassEva(CFunctionLib::ReadInt(pStr, MASS));
		}
		else if (CFunctionLib::Memcmp(pStr, DOWN) == 0)
		{// �������ɓ������郉���_���]���l������
			m_pEnemyListData[nCntEnemyList]->SetAI_DownEva(CFunctionLib::ReadInt(pStr, DOWN));
		}
		else if (CFunctionLib::Memcmp(pStr, END_AISET) == 0)
		{// �G��AI���ǂݍ��ݏI���̍��}��������
			break;
		}
	}
}

//=============================================================================
//    ���f�����X�g�̃t�@�C�������ۑ�����
//=============================================================================
HRESULT CMap::SaveModelList(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# ���f�����̃X�N���v�g�t�@�C����\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// ���f�����̃X�N���v�g�t�@�C��������������
	char aSaveFileName[256];
	strcpy(aSaveFileName, SAVEFILENAME_MODELLIST);
	pFileSaver->Print("%s%s\n\n", MODELLIST_FILENAME, strcat(aSaveFileName, m_aModelListFileName));

	return S_OK;
}

//=============================================================================
//    �e�N�X�`�����X�g�̃t�@�C�������ۑ�����
//=============================================================================
HRESULT CMap::SaveTexList(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �e�N�X�`�����̃X�N���v�g�t�@�C����\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �e�N�X�`�����̃X�N���v�g�t�@�C��������������
	char aSaveFileName[256];
	strcpy(aSaveFileName, SAVEFILENAME_TEXLIST);
	pFileSaver->Print("%s%s\n\n", TEXLIST_FILENAME, strcat(aSaveFileName, m_aTexListFileName));

	return S_OK;
}

//=============================================================================
//    ��n�̈ʒu���ۑ�����
//=============================================================================
HRESULT CMap::SaveHeadQuartersPos(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �i�ߕ��̈ʒu(�� : ���s)\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �i�ߕ��̈ʒu����������
	pFileSaver->Print("%s%d %d\n\n", BASE_POS, m_pHeadQuarters->GetAreaX(), m_pHeadQuarters->GetAreaZ());

	return S_OK;
}

//=============================================================================
//    �v���C���[�̃��X�|�[���ʒu���ۑ�����
//=============================================================================
HRESULT CMap::SavePlayerRespawn(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �v���C���[�̃��X�|�[���ʒu(�� : ���s)\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �v���C���[�̃��X�|�[���ʒu����������
	for (int nCntRes = 0; nCntRes < MAX_PLAYER_RESPAWN; nCntRes++)
	{
		pFileSaver->Print("%s%d %d\n", PLAYER_RESPAWN, m_pPlayerRespawn[nCntRes]->GetAreaX(), m_pPlayerRespawn[nCntRes]->GetAreaZ());
	}
	pFileSaver->Print("\n");

	return S_OK;
}

//=============================================================================
//    �G�̃��X�|�[���ʒu���ۑ�����
//=============================================================================
HRESULT CMap::SaveEnemyRespawn(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �G�̃��X�|�[���ʒu(�� : ���s)\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �G�̃��X�|�[���ʒu����������
	for (int nCntRes = 0; nCntRes < MAX_ENEMY_RESPAWN; nCntRes++)
	{
		pFileSaver->Print("%s%d %d\n", ENEMY_RESPAWN, m_pEnemyRespawn[nCntRes]->GetAreaX(), m_pEnemyRespawn[nCntRes]->GetAreaZ());
	}
	pFileSaver->Print("\n");

	return S_OK;
}


//=============================================================================
//    ��ۑ�����
//=============================================================================
HRESULT CMap::SaveSky(CFileSaver *pFileSaver)
{
	if (m_pSky == NULL) return E_FAIL;

	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# ����\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	int nSkyTexIdx = m_pSky->GetTexIdx();
	D3DXVECTOR3 SkyPos = m_pSky->GetPos();
	D3DXVECTOR3 SkyRot = m_pSky->GetRot();
	float fSkyRollSpeed = m_pSky->GetRollSpeed();
	int nSkyXBlock = m_pSky->GetXBlock();
	int nSkyYBlock = m_pSky->GetYBlock();
	float fSkyRadius = m_pSky->GetRadius();

	pFileSaver->Print("%s\n", SKYSET);
	pFileSaver->Print("	%s%d						# ���\n", TEX_IDX, nSkyTexIdx);
	pFileSaver->Print("	%s%.1f %.1f %.1f	# ���W\n", POS, SkyPos.x, SkyPos.y, SkyPos.z);
	pFileSaver->Print("	%s%.1f %.1f %.1f				# ����\n", ROT, SkyRot.x, SkyRot.y, SkyRot.z);
	pFileSaver->Print("	%s%f			# ��]�X�s�[�h\n", ROLLSPEED, fSkyRollSpeed);
	pFileSaver->Print("	%s%d						# ���̕�����\n", XBLOCK, nSkyXBlock);
	pFileSaver->Print("	%s%d						# �c�̕�����\n", YBLOCK, nSkyYBlock);
	pFileSaver->Print("	%s%.1f				# ���a\n", RADIUS, fSkyRadius);
	pFileSaver->Print("%s\n", END_SKYSET);
	pFileSaver->Print("\n");

	return S_OK;
}

//=============================================================================
//    �}�b�v�Z�b�g���ۑ�����
//=============================================================================
HRESULT CMap::SaveMap(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �}�b�v���\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	char aSaveLightFileName[256];
	char aSaveGameFieldFileName[256];
	char aSaveObjectFileName[256];
	char aSaveEnemyListFileName[256];
	strcpy(aSaveLightFileName, SAVEFILENAME_LIGHT);
	strcpy(aSaveGameFieldFileName, SAVEFILENAME_GAMEFIELD);
	strcpy(aSaveObjectFileName, SAVEFILENAME_OBJECT);
	strcpy(aSaveEnemyListFileName, SAVEFILENAME_ENEMYLIST);
	pFileSaver->Print("%s\n", MAPSET);      // �}�b�v�Z�b�g���ǂݍ��݊J�n�̍��}����������
	pFileSaver->Print("	%s%s				# ���C�g���̃X�N���v�g�t�@�C����\n", LIGHT_FILENAME, strcat(aSaveLightFileName, m_aLightFileName));
	pFileSaver->Print("	%s%s	# �Q�[���t�B�[���h���̃X�N���v�g�t�@�C����\n", GAMEFIELD_FILENAME, strcat(aSaveGameFieldFileName, m_aGameFieldFileName));
	pFileSaver->Print("	%s%s			# �z�u�����̃X�N���v�g�t�@�C����\n", OBJECT_FILENAME, strcat(aSaveObjectFileName, m_aObjectFileName));
	pFileSaver->Print("	%s%s	# �G�̐��������̃X�N���v�g�t�@�C����\n", ENEMYLIST_FILENAME, strcat(aSaveEnemyListFileName, m_aEnemyListFileName));
	pFileSaver->Print("%s\n", END_MAPSET);  // �}�b�v�Z�b�g���ǂݍ��ݏI���̍��}����������

	return S_OK;
}

//=============================================================================
//    ���f�����ۑ�����
//=============================================================================
HRESULT CMap::SaveModel(char *pModelFileName)
{
	char aSaveFileName[256] = SAVEFILENAME_MODELLIST;
	CFileSaver *pFileSaver = NULL;  // �t�@�C���ۑ��p�N���X�ւ̃|�C���^
	pFileSaver = CFileSaver::Create(strcat(aSaveFileName, pModelFileName));
	if (pFileSaver != NULL)
	{// �t�@�C�����ǂݍ��߂�
	    // �t�@�C���̖`��������������
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# ���f�����X�g���X�N���v�g�t�@�C�� [%s]\n", pModelFileName);
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# ���̍s�͐�Ώ����Ȃ����ƁI\n", SCRIPT);
		pFileSaver->Print("\n");

		// �ǂݍ��ރ��f��������������
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("# �ǂݍ��ރ��f����\n");
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("%s%d\n\n", NUM_MODEL, m_pModelCreate->GetNumModel());

		// �ǂݍ��ރ��f���̃t�@�C��������������
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("# �ǂݍ��ރ��f���̃t�@�C����\n");
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		for (int nCntModel = 0; nCntModel < m_pModelCreate->GetNumModel(); nCntModel++)
		{// �ǂݍ��񂾃��f���̐������J��Ԃ�
			pFileSaver->Print("%s%s\n", MODEL_FILENAME, m_pModelCreate->GetFileName(nCntModel));
		}
		pFileSaver->Print("\n");

		// �X�N���v�g�I���̍��}����������
		pFileSaver->Print("%s		# ���̍s�͐�Ώ����Ȃ����ƁI\n", END_SCRIPT);

		// �������̊J��
		if (pFileSaver != NULL)
		{
			pFileSaver->Uninit();
			delete pFileSaver;
			pFileSaver = NULL;
		}
	}

	return S_OK;
}

//=============================================================================
//    �e�N�X�`�����ۑ�����
//=============================================================================
HRESULT CMap::SaveTexture(char *pTextureFileName)
{
	char aSaveFileName[256] = SAVEFILENAME_TEXLIST;
	CFileSaver *pFileSaver = NULL;  // �t�@�C���ۑ��p�N���X�ւ̃|�C���^
	pFileSaver = CFileSaver::Create(strcat(aSaveFileName, pTextureFileName));
	if (pFileSaver != NULL)
	{// �t�@�C�����ǂݍ��߂�
	    // �t�@�C���̖`��������������
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# �e�N�X�`�����X�g���X�N���v�g�t�@�C�� [%s]\n", pTextureFileName);
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# ���̍s�͐�Ώ����Ȃ����ƁI\n", SCRIPT);
		pFileSaver->Print("\n");

		// �ǂݍ��ރe�N�X�`��������������
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("# �ǂݍ��ރe�N�X�`����\n");
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("%s%d\n\n", NUM_TEXTURE, m_pTextureManager->GetNumTexture());

		// �ǂݍ��ރe�N�X�`���̃t�@�C��������������
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("# �ǂݍ��ރe�N�X�`���̃t�@�C����\n");
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		for (int nCntTex = 0; nCntTex < m_pTextureManager->GetNumTexture(); nCntTex++)
		{// �ǂݍ��񂾃e�N�X�`���̐������J��Ԃ�
			pFileSaver->Print("%s%s\n", TEXTURE_FILENAME, m_pTextureManager->GetFileName(nCntTex));
		}
		pFileSaver->Print("\n");

		// �X�N���v�g�I���̍��}����������
		pFileSaver->Print("%s		# ���̍s�͐�Ώ����Ȃ����ƁI\n", END_SCRIPT);

		// �������̊J��
		if (pFileSaver != NULL)
		{
			pFileSaver->Uninit();
			delete pFileSaver;
			pFileSaver = NULL;
		}
	}

	return S_OK;
}

//=============================================================================
//    ���C�g���ۑ��̂��߂̃t�@�C���I�[�v������
//=============================================================================
HRESULT CMap::SaveLight(char *pLightFileName)
{
	char aSaveFileName[256] = SAVEFILENAME_LIGHT;
	CFileSaver *pFileSaver = NULL;  // �t�@�C���ۑ��p�N���X�ւ̃|�C���^
	pFileSaver = CFileSaver::Create(strcat(aSaveFileName, pLightFileName));
	if (pFileSaver != NULL)
	{// �t�@�C�����ǂݍ��߂�
	    // �t�@�C���̖`��������������
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# ���C�g���X�N���v�g�t�@�C�� [%s]\n", pLightFileName);
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# ���̍s�͐�Ώ����Ȃ����ƁI\n", SCRIPT);
		pFileSaver->Print("\n");

		// ���C�g�̑���������������
		int nNumLight = 0;
		if (m_pLightManager != NULL)
		{
			nNumLight = m_pLightManager->GetNumLight();
		}
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("# ���C�g�̑���\n");
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("%s%d\n\n", NUM_LIGHT, nNumLight);

		// ���C�g������������
		SaveLightInfo(pFileSaver);

		// �X�N���v�g�I���̍��}����������
		pFileSaver->Print("%s		# ���̍s�͐�Ώ����Ȃ����ƁI\n", END_SCRIPT);

		// �������̊J��
		if (pFileSaver != NULL)
		{
			pFileSaver->Uninit();
			delete pFileSaver;
			pFileSaver = NULL;
		}
	}

	return S_OK;
}

//=============================================================================
//    ���C�g���ۑ�����
//=============================================================================
void CMap::SaveLightInfo(CFileSaver *pFileSaver)
{
	CLight *pLight = NULL;
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# ���C�g���\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	if (m_pLightManager != NULL)
	{
		for (int nCntLight = 0; nCntLight < m_pLightManager->GetNumLight(); nCntLight++)
		{// ���C�g�̑������J��Ԃ�
			pLight = m_pLightManager->GetLight(nCntLight);
			if (pLight->GetType() == CLight::TYPE_DIRECTIONAL)
			{// �f�B���N�V���i�����C�g������
				SaveDirectionalLight((CDirectionalLight*)pLight, pFileSaver);
			}
			else if (pLight->GetType() == CLight::TYPE_POINT)
			{// �|�C���g���C�g������
				SavePointLight((CPointLight*)pLight, pFileSaver);
			}
			else if (pLight->GetType() == CLight::TYPE_SPOT)
			{// �X�|�b�g���C�g������
				SaveSpotLight((CSpotLight*)pLight, pFileSaver);
			}
		}
	}
	pFileSaver->Print("\n");
}

//=============================================================================
//    �f�B���N�V���i�����C�g���ۑ�����
//=============================================================================
void CMap::SaveDirectionalLight(CDirectionalLight *pDirLight, CFileSaver *pFileSaver)
{
	// �e������擾
	D3DXCOLOR LightDiffuse = pDirLight->GetLight().Diffuse;
	D3DXCOLOR LightAmbient = pDirLight->GetLight().Ambient;
	D3DXCOLOR LightSpecular = pDirLight->GetLight().Specular;
	D3DXVECTOR3 LightDir = pDirLight->GetLight().Direction;

	// �e������e�L�X�g�t�@�C���ɕۑ�
	pFileSaver->Print("%s\n", DIRECTIONALLIGHTSET);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ���C�g�̊g�U��\n", DIFFUSE, LightDiffuse.r, LightDiffuse.g, LightDiffuse.b, LightDiffuse.a);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ���C�g�̊���\n", AMBIENT, LightAmbient.r, LightAmbient.g, LightAmbient.b, LightAmbient.a);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ���C�g�̔��ˌ�\n", SPECULAR, LightSpecular.r, LightSpecular.g, LightSpecular.b, LightSpecular.a);
	pFileSaver->Print("	%s%.2f %.2f %.2f					# ���C�g�̌���\n", DIR, LightDir.x, LightDir.y, LightDir.z);
	pFileSaver->Print("%s\n", END_DIRECTIONALLIGHTSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    �|�C���g���C�g���ۑ�����
//=============================================================================
void CMap::SavePointLight(CPointLight *pPointLight, CFileSaver *pFileSaver)
{
	// �e������擾
	D3DXVECTOR3 LightPos = pPointLight->GetLight().Position;
	D3DXCOLOR LightDiffuse = pPointLight->GetLight().Diffuse;
	D3DXCOLOR LightAmbient = pPointLight->GetLight().Ambient;
	D3DXCOLOR LightSpecular = pPointLight->GetLight().Specular;
	float fLightAttenuation0 = pPointLight->GetLight().Attenuation0;
	float fLightAttenuation1 = pPointLight->GetLight().Attenuation1;
	float fLightAttenuation2 = pPointLight->GetLight().Attenuation2;
	float fLightRange = pPointLight->GetLight().Range;

	// �e������e�L�X�g�t�@�C���ɕۑ�
	pFileSaver->Print("%s\n", POINTLIGHTSET);
	pFileSaver->Print("	%s%.2f %.2f %.2f					# ���C�g�̍��W\n", POS, LightPos.x, LightPos.y, LightPos.z);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ���C�g�̊g�U��\n", DIFFUSE, LightDiffuse.r, LightDiffuse.g, LightDiffuse.b, LightDiffuse.a);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ���C�g�̊���\n", AMBIENT, LightAmbient.r, LightAmbient.g, LightAmbient.b, LightAmbient.a);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ���C�g�̔��ˌ�\n", SPECULAR, LightSpecular.r, LightSpecular.g, LightSpecular.b, LightSpecular.a);
	pFileSaver->Print("	%s%.3f					# ���C�g�̐��`�����W��\n", ATTENUATION1, fLightAttenuation0);
	pFileSaver->Print("	%s%.3f					# ���C�g�̂Q�������W��\n", ATTENUATION2, fLightAttenuation1);
	pFileSaver->Print("	%s%.3f					# ���C�g�̎w�������W��\n", ATTENUATION3, fLightAttenuation2);
	pFileSaver->Print("	%s%.3f							# ���C�g�͈̔�\n", RANGE, fLightRange);
	pFileSaver->Print("%s\n", END_POINTLIGHTSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    �X�|�b�g���C�g���ۑ�����
//=============================================================================
void CMap::SaveSpotLight(CSpotLight *pSpotLight, CFileSaver *pFileSaver)
{
	// �e������擾
	D3DXVECTOR3 LightPos = pSpotLight->GetLight().Position;
	D3DXVECTOR3 LightDir = pSpotLight->GetLight().Direction;
	D3DXCOLOR LightDiffuse = pSpotLight->GetLight().Diffuse;
	D3DXCOLOR LightAmbient = pSpotLight->GetLight().Ambient;
	D3DXCOLOR LightSpecular = pSpotLight->GetLight().Specular;
	float fLightAttenuation0 = pSpotLight->GetLight().Attenuation0;
	float fLightAttenuation1 = pSpotLight->GetLight().Attenuation1;
	float fLightAttenuation2 = pSpotLight->GetLight().Attenuation2;
	float fLightRange = pSpotLight->GetLight().Range;
	float fLightFalloff = pSpotLight->GetLight().Falloff;
	float fLightTheta = pSpotLight->GetLight().Theta;
	float fLighPhi = pSpotLight->GetLight().Phi;

	// �e������e�L�X�g�t�@�C���ɕۑ�
	pFileSaver->Print("%s\n", SPOTLIGHTSET);
	pFileSaver->Print("	%s%.2f %.2f %.2f					# ���C�g�̍��W\n", POS, LightPos.x, LightPos.y, LightPos.z);
	pFileSaver->Print("	%s%.2f %.2f %.2f					# ���C�g�̌���\n", DIR, LightDir.x, LightDir.y, LightDir.z);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ���C�g�̊g�U��\n", DIFFUSE, LightDiffuse.r, LightDiffuse.g, LightDiffuse.b, LightDiffuse.a);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ���C�g�̊���\n", AMBIENT, LightAmbient.r, LightAmbient.g, LightAmbient.b, LightAmbient.a);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ���C�g�̔��ˌ�\n", SPECULAR, LightSpecular.r, LightSpecular.g, LightSpecular.b, LightSpecular.a);
	pFileSaver->Print("	%s%.3f					# ���C�g�̐��`�����W��\n", ATTENUATION1, fLightAttenuation0);
	pFileSaver->Print("	%s%.3f					# ���C�g�̂Q�������W��\n", ATTENUATION2, fLightAttenuation1);
	pFileSaver->Print("	%s%.3f					# ���C�g�̎w�������W��\n", ATTENUATION3, fLightAttenuation2);
	pFileSaver->Print("	%s%.3f							# ���C�g�͈̔�\n", RANGE, fLightRange);
	pFileSaver->Print("	%s%.3f							# ���C�g�̃t�H�[���I�t\n", FALLOFF, fLightFalloff);
	pFileSaver->Print("	%s%.3f							# ���C�g�̓����R�[���̊p�x\n", THETA, fLightTheta);
	pFileSaver->Print("	%s%.3f								# ���C�g�̊O���R�[���̊p�x\n", PHI, fLighPhi);
	pFileSaver->Print("%s\n", END_SPOTLIGHTSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    �Q�[���t�B�[���h���ۑ��̂��߂̃t�@�C���I�[�v������
//=============================================================================
HRESULT CMap::SaveGameField(char *pGameFieldFileName)
{
	char aSaveFileName[256] = SAVEFILENAME_GAMEFIELD;
	CFileSaver *pFileSaver = NULL;  // �t�@�C���ۑ��p�N���X�ւ̃|�C���^
	pFileSaver = CFileSaver::Create(strcat(aSaveFileName, pGameFieldFileName));
	if (pFileSaver != NULL)
	{// �t�@�C�����ǂݍ��߂�
	    // �t�@�C���̖`��������������
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# �Q�[���t�B�[���h���X�N���v�g�t�@�C�� [%s]\n", pGameFieldFileName);
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# ���̍s�͐�Ώ����Ȃ����ƁI\n", SCRIPT);
		pFileSaver->Print("\n");

		// �Q�[���t�B�[���h����ۑ�����
		SaveGameFieldInfo(pFileSaver);

		// �X�N���v�g�I���̍��}����������
		pFileSaver->Print("\n%s		# ���̍s�͐�Ώ����Ȃ����ƁI\n", END_SCRIPT);

		// �������̊J��
		if (pFileSaver != NULL)
		{
			pFileSaver->Uninit();
			delete pFileSaver;
			pFileSaver = NULL;
		}
	}

	return S_OK;
}

//=============================================================================
//    �Q�[���t�B�[���h����ۑ����鏈��
//=============================================================================
void CMap::SaveGameFieldInfo(CFileSaver *pFileSaver)
{
	// �n�ʏ�����������
	SaveField(pFileSaver);

	// �u���b�N������������
	SaveBlock(pFileSaver);

	// �������������
	SaveRiver(pFileSaver);

	// �X������������
	SaveIce(pFileSaver);
}

//=============================================================================
//    �n�ʂ̏���ۑ����鏈��
//=============================================================================
void CMap::SaveField(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �n�ʏ��\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �e������擾
	float fFieldWidth = m_pMeshField->GetWidth();
	float fFieldDepth = m_pMeshField->GetHeight();
	int nFieldXBlock = m_pMeshField->GetXBlock();
	int nFieldZBlock = m_pMeshField->GetZBlock();

	// �e������e�L�X�g�t�@�C���ɏ�������
	pFileSaver->Print("%s\n", FIELDSET);
	pFileSaver->Print("	%s%d		# �g�p����e�N�X�`���̔ԍ�(�e�N�X�`�����X�g�̔ԍ��Əƍ�)\n", TEX_IDX, m_nFieldTexIdx);
	pFileSaver->Print("	%s%.1f	# 1�}�X���̉���\n", WIDTH, fFieldWidth);
	pFileSaver->Print("	%s%.1f	# 1�}�X���̉��s\n", DEPTH, fFieldDepth);
	pFileSaver->Print("	%s%d		# ���̕�����\n", XBLOCK, nFieldXBlock);
	pFileSaver->Print("	%s%d		# ���s�̕�����\n", ZBLOCK, nFieldZBlock);
	pFileSaver->Print("%s\n\n", END_FIELDSET);
}

//=============================================================================
//    �u���b�N��ۑ����鏈��
//=============================================================================
void CMap::SaveBlock(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �u���b�N�z�u���\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK)
			{// �u���b�N�N���X������
				SaveBlockInfo((CBlock*)pScene, pFileSaver);
			}
			pScene = pSceneNext;
		}
	}

	// �ǐ��̂��߉��s
	pFileSaver->Print("\n");
}

//=============================================================================
//    �u���b�N�̏���ۑ����鏈��
//=============================================================================
void CMap::SaveBlockInfo(CBlock *pBlock, CFileSaver *pFileSaver)
{
	// �e������擾
	int nBlockType = pBlock->GetType();
	int nBlockModelIdx = pBlock->GetModelIdx();
	D3DXVECTOR3 BlockPos = pBlock->GetPos();
	D3DXVECTOR3 BlockRot = D3DXToDegree(pBlock->GetRot());
	float fBlockColWidth = pBlock->GetBoxCollider()->GetWidth();
	float fBlockColHeight = pBlock->GetBoxCollider()->GetHeight();
	float fBlockColDepth = pBlock->GetBoxCollider()->GetDepth();

	// �e������e�L�X�g�t�@�C���ɕۑ�
	pFileSaver->Print("%s\n", BLOCKSET);
	pFileSaver->Print("	%s%d				# ���\n", BLOCKTYPE, nBlockType);
	pFileSaver->Print("	%s%d			# �g�p���郂�f���̔ԍ�\n", MODELIDX, nBlockModelIdx);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# ���W\n", POS, BlockPos.x, BlockPos.y, BlockPos.z);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# ����\n", ROT, BlockRot.x, BlockRot.y, BlockRot.z);
	pFileSaver->Print("	%s\n", COLLISION);
	pFileSaver->Print("		%s%.1f		# �����蔻��̕�\n", WIDTH, fBlockColWidth);
	pFileSaver->Print("		%s%.1f		# �����蔻��̍���\n", HEIGHT, fBlockColHeight);
	pFileSaver->Print("		%s%.1f		# �����蔻��̉��s\n", DEPTH, fBlockColDepth);
	pFileSaver->Print("	%s\n", END_COLLISION);
	pFileSaver->Print("%s\n", END_BLOCKSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    ��ۑ�����
//=============================================================================
void CMap::SaveRiver(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# ��z�u���\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_RIVER)
			{// ��N���X������
				SaveRiverInfo((CRiver*)pScene, pFileSaver);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    ����ۑ�����
//=============================================================================
void CMap::SaveRiverInfo(CRiver *pRiver, CFileSaver *pFileSaver)
{
	// �e������擾
	int nRiverTexIdx = pRiver->GetTexIdx();
	D3DXVECTOR3 RiverPos = pRiver->GetPos();
	int nRiverXBlock = pRiver->GetMeshField()->GetXBlock();
	int nRiverZBlock = pRiver->GetMeshField()->GetZBlock();
	float fRiverColWidth = pRiver->GetBoxCollider()->GetWidth();
	float fRiverColHeight = pRiver->GetBoxCollider()->GetHeight();
	float fRiverColDepth = pRiver->GetBoxCollider()->GetDepth();

	// �e������e�L�X�g�t�@�C���ɕۑ�
	pFileSaver->Print("%s\n", RIVERSET);
	pFileSaver->Print("	%s%d					# �g�p����e�N�X�`���̔ԍ�\n", TEX_IDX, nRiverTexIdx);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# ���W\n", POS, RiverPos.x, RiverPos.y, RiverPos.z);
	pFileSaver->Print("	%s%d					# ���̕�����\n", XBLOCK, nRiverXBlock);
	pFileSaver->Print("	%s%d					# ���s�̕�����\n", ZBLOCK, nRiverZBlock);
	pFileSaver->Print("	%s\n", COLLISION);
	pFileSaver->Print("		%s%.1f			# �����蔻��̕�\n", WIDTH, fRiverColWidth);
	pFileSaver->Print("		%s%.1f			# �����蔻��̍���\n", HEIGHT, fRiverColHeight);
	pFileSaver->Print("		%s%.1f			# �����蔻��̉��s\n", DEPTH, fRiverColDepth);
	pFileSaver->Print("	%s\n", END_COLLISION);
	pFileSaver->Print("%s\n", END_RIVERSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    �X�ۑ�����
//=============================================================================
void CMap::SaveIce(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �X�z�u���\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ICEFIELD)
			{// �X�N���X������
				SaveIceInfo((CIceField*)pScene, pFileSaver);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    �X���ۑ�����
//=============================================================================
void CMap::SaveIceInfo(CIceField *pIce, CFileSaver *pFileSaver)
{
	// �e������擾
	int nIceTexIdx = pIce->GetTexIdx();
	D3DXVECTOR3 IcePos = pIce->GetPos();
	int nIceXBlock = pIce->GetMeshField()->GetXBlock();
	int nIceZBlock = pIce->GetMeshField()->GetZBlock();
	float fIceColWidth = pIce->GetBoxCollider()->GetWidth();
	float fIceColHeight = pIce->GetBoxCollider()->GetHeight();
	float fIceColDepth = pIce->GetBoxCollider()->GetDepth();

	// �e������e�L�X�g�t�@�C���ɕۑ�
	pFileSaver->Print("%s\n", ICESET);
	pFileSaver->Print("	%s%d					# �g�p����e�N�X�`���̔ԍ�\n", TEX_IDX, nIceTexIdx);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# ���W\n", POS, IcePos.x, IcePos.y, IcePos.z);
	pFileSaver->Print("	%s%d					# ���̕�����\n", XBLOCK, nIceXBlock);
	pFileSaver->Print("	%s%d					# ���s�̕�����\n", ZBLOCK, nIceZBlock);
	pFileSaver->Print("	%s\n", COLLISION);
	pFileSaver->Print("		%s%.1f			# �����蔻��̕�\n", WIDTH, fIceColWidth);
	pFileSaver->Print("		%s%.1f			# �����蔻��̍���\n", HEIGHT, fIceColHeight);
	pFileSaver->Print("		%s%.1f			# �����蔻��̉��s\n", DEPTH, fIceColDepth);
	pFileSaver->Print("	%s\n", END_COLLISION);
	pFileSaver->Print("%s\n", END_ICESET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    �z�u�����ۑ��̂��߂̃t�@�C���I�[�v������
//=============================================================================
HRESULT CMap::SaveObject(char *pObjectName)
{
	char aSaveFileName[256] = SAVEFILENAME_OBJECT;
	CFileSaver *pFileSaver = NULL;  // �t�@�C���ۑ��p�N���X�ւ̃|�C���^
	pFileSaver = CFileSaver::Create(strcat(aSaveFileName, pObjectName));
	if (pFileSaver != NULL)
	{// �t�@�C�����ǂݍ��߂�
	 // �t�@�C���̖`��������������
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# �z�u�����X�N���v�g�t�@�C�� [%s]\n", pObjectName);
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# ���̍s�͐�Ώ����Ȃ����ƁI\n", SCRIPT);
		pFileSaver->Print("\n");

		// �z�u���f�[�^�̐�����������
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("# �ǂݍ��ރI�u�W�F�N�g�f�[�^�̐�\n");
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("%s%d\n", NUM_OBJECT_DATA, m_nNumObjectData);
		pFileSaver->Print("\n");

		// �z�u���f�[�^�̃t�@�C��������������
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("# �ǂݍ��ރI�u�W�F�N�g�f�[�^�̃t�@�C����\n");
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		for (int nCntObject = 0; nCntObject < m_nNumObjectData; nCntObject++)
		{
			pFileSaver->Print("%s%s\n", OBJECT_FILENAME, m_pObjectManager[nCntObject]->GetFileName());
		}
		pFileSaver->Print("\n");

		// �z�u��������������
		SaveObjectInfo(pFileSaver);

		// �X�N���v�g�ǂݍ��ݏI���̍��}����������
		pFileSaver->Print("%s		# ���̍s�͐�Ώ����Ȃ����ƁI\n", END_SCRIPT);

		// �������̊J��
		if (pFileSaver != NULL)
		{
			pFileSaver->Uninit();
			delete pFileSaver;
			pFileSaver = NULL;
		}
	}
	return S_OK;
}

//=============================================================================
//    �z�u�����ۑ�����
//=============================================================================
void CMap::SaveObjectInfo(CFileSaver *pFileSaver)
{
	// ���f���z�u������������
	SaveObjModel(pFileSaver);

	// �r���{�[�h�z�u������������
	SaveObjBill(pFileSaver);

	// �G�t�F�N�g�z�u������������
	SaveObjEffect(pFileSaver);
}

//=============================================================================
//    �z�u���f���ۑ�����
//=============================================================================
void CMap::SaveObjModel(CFileSaver *pFileSaver)
{
	// �`��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# ���f���z�u���\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �z�u���f��������������
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT)
			{// �z�u���N���X������
				SaveObjModelInfo((CObject*)pScene, pFileSaver);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    �z�u���f�����ۑ�����
//=============================================================================
void CMap::SaveObjModelInfo(CObject *pObject, CFileSaver *pFileSaver)
{
	// �e������擾
	int nObjModelType = pObject->GetType();
	D3DXVECTOR3 ObjModelPos = pObject->GetPos();
	D3DXVECTOR3 ObjModelRot = pObject->GetRot();

	// �e������e�L�X�g�t�@�C���ɕۑ�
	pFileSaver->Print("%s\n", MODELSET);
	pFileSaver->Print("	%s%d					# ���\n", TYPE, nObjModelType);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# ���W\n", POS, ObjModelPos.x, ObjModelPos.y, ObjModelPos.z);
	pFileSaver->Print("	%s%.2f %.2f %.2f		# ����\n", ROT, ObjModelRot.x, ObjModelRot.y, ObjModelRot.z);
	pFileSaver->Print("%s\n", END_MODELSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    �z�u�r���{�[�h�ۑ�����
//=============================================================================
void CMap::SaveObjBill(CFileSaver *pFileSaver)
{
	// �`��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �r���{�[�h�z�u���\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �z�u�r���{�[�h������������
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_OBJBILLBOARD)
			{// �z�u�r���{�[�h�N���X������
				SaveObjBillInfo((CBillboardObject*)pScene, pFileSaver);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    �z�u�r���{�[�h���ۑ�����
//=============================================================================
void CMap::SaveObjBillInfo(CBillboardObject *pBillObj, CFileSaver *pFileSaver)
{
	// �e������擾
	int nObjBillTexIdx = pBillObj->GetTexIdx();
	D3DXVECTOR3 ObjBillPos = pBillObj->GetPos();
	D3DXCOLOR ObjBillCol = pBillObj->GetCol();
	float fObjBillRot = pBillObj->GetRot();
	float fObjBillWidth = pBillObj->GetWidth();
	float fObjBillHeight = pBillObj->GetHeight();
	bool bObjBillLighting = pBillObj->GetLighting();
	bool bObjBillDrawAddtive = pBillObj->GetDrawAddtive();

	// �e������e�L�X�g�t�@�C���ɕۑ�
	pFileSaver->Print("%s\n", BILLBOARDSET);
	pFileSaver->Print("	%s%d					# �g�p����e�N�X�`���̔ԍ�\n", TEX_IDX, nObjBillTexIdx);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# ���W\n", POS, ObjBillPos.x, ObjBillPos.y, ObjBillPos.z);
	pFileSaver->Print("	%s%.1f %.1f %.1f %.1f		# �F\n", COL, ObjBillCol.r, ObjBillCol.g, ObjBillCol.b, ObjBillCol.a);
	pFileSaver->Print("	%s%.1f					# ����\n", ROT, fObjBillRot);
	pFileSaver->Print("	%s%.1f				# ��\n", WIDTH, fObjBillWidth);
	pFileSaver->Print("	%s%.1f				# ����\n", HEIGHT, fObjBillHeight);
	pFileSaver->Print("	%s%d				# ���C�e�B���O���邩�ǂ���\n", LIGHTING, (int)bObjBillLighting);
	pFileSaver->Print("	%s%d			# ���Z�����ŕ`�悷�邩�ǂ���\n", DRAW_ADDTIVE, (int)bObjBillDrawAddtive);
	pFileSaver->Print("%s\n", END_BILLBOARDSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    �z�u�G�t�F�N�g�ۑ�����
//=============================================================================
void CMap::SaveObjEffect(CFileSaver *pFileSaver)
{
	// �`��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �G�t�F�N�g�z�u���\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �z�u�G�t�F�N�g������������
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_PAREMITTER || pScene->GetObjType() == CScene::OBJTYPE_RINGEMITTER)
			{// �G�~�b�^�N���X������
				SaveObjEffectInfo((CEmitter*)pScene, pFileSaver);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    �z�u�G�t�F�N�g���ۑ�����
//=============================================================================
void CMap::SaveObjEffectInfo(CEmitter *pEmitter, CFileSaver *pFileSaver)
{
	// �e������擾
	int nObjEffectType = pEmitter->GetType();
	D3DXVECTOR3 ObjEffectPos = pEmitter->GetPos();
	D3DXVECTOR3 ObjEffectRot = pEmitter->GetRot();

	// �e������e�L�X�g�t�@�C���ɕۑ�
	pFileSaver->Print("%s\n", EFFECTSET);
	pFileSaver->Print("	%s%d					# ���\n", TYPE, nObjEffectType);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# ���W\n", POS, ObjEffectPos.x, ObjEffectPos.y, ObjEffectPos.z);
	pFileSaver->Print("	%s%.2f %.2f %.2f		# ����\n", ROT, ObjEffectRot.x, ObjEffectRot.y, ObjEffectRot.z);
	pFileSaver->Print("%s\n", END_EFFECTSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    �G�̐�������ۑ�����
//=============================================================================
HRESULT CMap::SaveEnemyList(char *pEnemyListName)
{
	char aSaveFileName[256] = SAVEFILENAME_ENEMYLIST;
	CFileSaver *pFileSaver = NULL;  // �t�@�C���ۑ��p�N���X�ւ̃|�C���^
	pFileSaver = CFileSaver::Create(strcat(aSaveFileName, pEnemyListName));
	if (pFileSaver != NULL)
	{// �t�@�C�����ǂݍ��߂�
	    // �t�@�C���̖`��������������
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# �G�̐������X�N���v�g�t�@�C�� [%s]\n", pEnemyListName);
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# ���̍s�͐�Ώ����Ȃ����ƁI\n", SCRIPT);
		pFileSaver->Print("\n");

		// �G�̐���������������
		SaveEnemyListInfo(pFileSaver);

		// �X�N���v�g�ǂݍ��ݏI���̍��}����������
		pFileSaver->Print("%s		# ���̍s�͐�Ώ����Ȃ����ƁI\n", END_SCRIPT);

		// �������̊J��
		if (pFileSaver != NULL)
		{
			pFileSaver->Uninit();
			delete pFileSaver;
			pFileSaver = NULL;
		}
	}
	return S_OK;
}

//=============================================================================
//    �G�̐��������t�@�C���ɕۑ�����
//=============================================================================
void CMap::SaveEnemyListInfo(CFileSaver *pFileSaver)
{
	// ��������G�̐�����������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# ��������G�̐�\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("%s%d\n", NUM_ENEMYLIST, m_nNumEnemyListData);
	pFileSaver->Print("\n");

	// �G�̐���������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �G�̐������\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	for (int nCnt = 0; nCnt < m_nNumEnemyListData; nCnt++)
	{// ��������G�̐����J��Ԃ�
		pFileSaver->Print("%s\n", ENEMYLISTSET);
		pFileSaver->Print("	%s%d			# ���X�|�[���ʒu�̔ԍ�\n", RESPAWN, m_pEnemyListData[nCnt]->GetRespawnIdx());
		pFileSaver->Print("	%s%d			# ���\n", TYPE, m_pEnemyListData[nCnt]->GetEnemyType());
		pFileSaver->Print("	%s%d			# �o�������鎞��\n", TIME, m_pEnemyListData[nCnt]->GetRespawnTime());
		if (m_pEnemyListData[nCnt]->GetItem() == true)
		{// �A�C�e�����o��������Ȃ�
			pFileSaver->Print("	%s\n", ITEM);
			pFileSaver->Print("		%s%d		# �o��������A�C�e���̎��\n", TYPE, m_pEnemyListData[nCnt]->GetItemType());
			pFileSaver->Print("	%s\n", END_ITEM);
		}
		pFileSaver->Print("	%s\n", AISET);
		pFileSaver->Print("		%s%d	# �e��łۂ̃����_���]���l\n", BULLET, m_pEnemyListData[nCnt]->GetAI_BulletEva());
		pFileSaver->Print("		%s%d		# ������ς������郉���_���]���l\n", MASS, m_pEnemyListData[nCnt]->GetAI_MassEva());
		pFileSaver->Print("		%s%d		# �������ɓ������郉���_���]���l\n", DOWN, m_pEnemyListData[nCnt]->GetAI_DownEva());
		pFileSaver->Print("	%s\n", END_AISET);
		pFileSaver->Print("%s\n", END_ENEMYLISTSET);
		pFileSaver->Print("\n");
	}

	pFileSaver->Print("\n");
}

//=============================================================================
//    �u���b�N�𐶐����鏈��
//=============================================================================
void CMap::CreateBlock(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxColWidth, float fBoxColHeight, float fBoxColDepth)
{
	switch (nType)
	{// ��ނɂ���ď����킯
	case CBlock::TYPE_BREAK_BULLET_NOT_DIRTY:
		CBlockType0::Create(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxColWidth, fBoxColHeight, fBoxColDepth);
		break;
	case CBlock::TYPE_BREAK_TANK_DIRTY:
		CBlockType1::Create(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxColWidth, fBoxColHeight, fBoxColDepth);
		break;
	case CBlock::TYPE_NOT_BREAK_NOT_DIRTY:
		CBlockType2::Create(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxColWidth, fBoxColHeight, fBoxColDepth);
		break;
	case CBlock::TYPE_NOT_BREAK_TANK_DIRTY:
		CBlockType3::Create(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxColWidth, fBoxColHeight, fBoxColDepth);
		break;
	}
}

//=============================================================================
//    �G�̐������N���X�𐶐�����
//=============================================================================
void CMap::CreateEnemyListData(void)
{
	if (m_nNumEnemyListData > 0)
	{// 1�ȏ㐶������
		m_pEnemyListData = new CEnemy_ListData*[m_nNumEnemyListData];
		for (int nCnt = 0; nCnt < m_nNumEnemyListData; nCnt++)
		{
			m_pEnemyListData[nCnt] = new CEnemy_ListData;
		}
	}
}

//=============================================================================
//    �}�b�v��j�����鏈��
//=============================================================================
void CMap::DeleteMap(void)
{
	// �n�ʂ�j��
	ReleaseMeshField();

	// ���j��
	ReleaseSky();

	// �i�ߕ���j��
	ReleaseHeadQuarters();

	// �v���C���[�̃��X�|�[���ʒu��j��
	ReleasePlayerRespawn();

	// �G�̃��X�|�[���ʒu��j��
	ReleaseEnemyRespawn();

	// �G�̐�������j��
	ReleaseEnemyListData();

	// �L�����N�^�[����j��
	ReleaseObjectManager();

	// ���C�g��j��
	DeleteLight();

	// �Q�[���t�B�[���h�I�u�W�F�N�g��j��
	DeleteGameField();

	// �z�u����j��
	DeleteObject();

	// �G��j��
	DeleteEnemy();

	// �S�ẴI�u�W�F�N�g�̊J���`�F�b�N
	CScene::DeathCheck();
}

//=============================================================================
//    ���C�g��j�����鏈��
//=============================================================================
void CMap::DeleteLight(void)
{
	if (m_pLightManager != NULL)
	{
		// �`��f�o�C�X�̃��C�g��OFF�ɂ���
		for (int nCntLight = 0; nCntLight < m_pLightManager->GetNumLight(); nCntLight++)
		{
			CManager::GetRenderer()->GetDevice()->LightEnable(nCntLight, false);
		}
	}

	// ���C�g�Ǌ��N���X��j������
	ReleaseLightManager();
}

//=============================================================================
//    �Q�[���t�B�[���h�ɕK�v�ȃI�u�W�F�N�g��j�����鏈��
//=============================================================================
void CMap::DeleteGameField(void)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK)
			{// �u���b�N�N���X������
				DeleteBlock((CBlock*)pScene);
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_RIVER)
			{// ��N���X������
				DeleteRiver((CRiver*)pScene);
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_ICEFIELD)
			{// �X�N���X������
				DeleteIce((CIceField*)pScene);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    �u���b�N��j�����鏈��
//=============================================================================
void CMap::DeleteBlock(CBlock *pBlock)
{
	pBlock->Uninit();
	pBlock = NULL;
}

//=============================================================================
//    ���j�����鏈��
//=============================================================================
void CMap::DeleteRiver(CRiver *pRiver)
{
	pRiver->Uninit();
	pRiver = NULL;
}

//=============================================================================
//    �X��j�����鏈��
//=============================================================================
void CMap::DeleteIce(CIceField *pIceField)
{
	pIceField->Uninit();
	pIceField = NULL;
}

//=============================================================================
//    �z�u���ɕK�v�ȃI�u�W�F�N�g��j�����鏈��
//=============================================================================
void CMap::DeleteObject(void)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	CBlock *pBlock = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT)
			{// �u���b�N�N���X������
				DeleteObjModel((CObject*)pScene);
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJBILLBOARD)
			{// �r���{�[�h�N���X������
				DeleteObjBillboard((CBillboardObject*)pScene);
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_EMITTER || pScene->GetObjType() == CScene::OBJTYPE_PAREMITTER || pScene->GetObjType() == CScene::OBJTYPE_RINGEMITTER)
			{// �G�~�b�^�N���X������
				DeleteObjEffect((CEmitter*)pScene);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    �z�u���f����j�����鏈��
//=============================================================================
void CMap::DeleteObjModel(CObject *pObject)
{
	pObject->Uninit();
	pObject = NULL;
}

//=============================================================================
//    �z�u�r���{�[�h��j�����鏈��
//=============================================================================
void CMap::DeleteObjBillboard(CBillboardObject *pBillboard)
{
	pBillboard->Uninit();
	pBillboard = NULL;
}

//=============================================================================
//    �z�u�G�t�F�N�g��j�����鏈��
//=============================================================================
void CMap::DeleteObjEffect(CEmitter *pEmitter)
{
	pEmitter->Uninit();
	pEmitter = NULL;
}

//=============================================================================
//    �G��j�����鏈��
//=============================================================================
void CMap::DeleteEnemy(void)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// �G�N���X������
				pScene->Uninit();
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    �e�N�X�`���Ǌ��N���X���J�����鏈��
//=============================================================================
void CMap::ReleaseTextureManager(void)
{
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}
}

//=============================================================================
//    ���f���Ǌ��N���X���J�����鏈��
//=============================================================================
void CMap::ReleaseModelManager(void)
{
	if (m_pModelCreate != NULL)
	{
		m_pModelCreate->Uninit();
		delete m_pModelCreate;
		m_pModelCreate = NULL;
	}
}

//=============================================================================
//    ���C�g�Ǌ��N���X���J�����鏈��
//=============================================================================
void CMap::ReleaseLightManager(void)
{
	if (m_pLightManager != NULL)
	{
		m_pLightManager->Uninit();
		delete m_pLightManager;
		m_pLightManager = NULL;
	}
}

//=============================================================================
//    �z�u���̃f�[�^�Ǌ��N���X���J�����鏈��
//=============================================================================
void CMap::ReleaseObjectManager(void)
{
	if (m_pObjectManager != NULL)
	{
		for (int nCntObject = 0; nCntObject < m_nNumObjectData; nCntObject++)
		{
			if (m_pObjectManager[nCntObject] != NULL)
			{
				m_pObjectManager[nCntObject]->Uninit();
				delete m_pObjectManager[nCntObject];
				m_pObjectManager[nCntObject] = NULL;
			}
		}
		delete[] m_pObjectManager;
		m_pObjectManager = NULL;
	}
}

//=============================================================================
//    �n�ʂ��J������
//=============================================================================
void CMap::ReleaseMeshField(void)
{
	if (m_pMeshField != NULL)
	{
		m_pMeshField->Uninit();
		m_pMeshField = NULL;
	}
}

//=============================================================================
//    ����J������
//=============================================================================
void CMap::ReleaseSky(void)
{
	if (m_pSky != NULL)
	{
		m_pSky->Uninit();
		m_pSky = NULL;
	}
}

//=============================================================================
//    �i�ߕ����J������
//=============================================================================
void CMap::ReleaseHeadQuarters(void)
{
	if (m_pHeadQuarters != NULL)
	{
		m_pHeadQuarters->Uninit();
		m_pHeadQuarters = NULL;
	}
}

//=============================================================================
//    �v���C���[�̃��X�|�[���ʒu���J������
//=============================================================================
void CMap::ReleasePlayerRespawn(void)
{
	for (int nCntRes = 0; nCntRes < MAX_PLAYER_RESPAWN; nCntRes++)
	{
		if (m_pPlayerRespawn[nCntRes] != NULL)
		{
			m_pPlayerRespawn[nCntRes]->Uninit();
			m_pPlayerRespawn[nCntRes] = NULL;
		}
	}
}

//=============================================================================
//    �G�̃��X�|�[���ʒu���J������
//=============================================================================
void CMap::ReleaseEnemyRespawn(void)
{
	for (int nCntRes = 0; nCntRes < MAX_ENEMY_RESPAWN; nCntRes++)
	{
		if (m_pEnemyRespawn[nCntRes] != NULL)
		{
			m_pEnemyRespawn[nCntRes]->Uninit();
			m_pEnemyRespawn[nCntRes] = NULL;
		}
	}
}

//=============================================================================
//    �G�̐��������J������
//=============================================================================
void CMap::ReleaseEnemyListData(void)
{
	if (m_pEnemyListData != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumEnemyListData; nCnt++)
		{
			delete m_pEnemyListData[nCnt];
			m_pEnemyListData[nCnt] = NULL;
		}

		delete[] m_pEnemyListData;
		m_pEnemyListData = NULL;
	}
}

//=============================================================================
//    �n�ʂɒ���t����e�N�X�`���̔ԍ����擾����
//=============================================================================
int CMap::GetFieldTexIdx(void)
{
	return m_nFieldTexIdx;
}

//=============================================================================
//    �n�ʃN���X�ւ̃|�C���^���擾����
//=============================================================================
CMeshField *CMap::GetMeshField(void)
{
	return m_pMeshField;
}

//=============================================================================
//    ��N���X�ւ̃|�C���^���擾����
//=============================================================================
CSky *CMap::GetSky(void)
{
	return m_pSky;
}

//=============================================================================
//    �e�N�X�`���Ǌ��N���X�ւ̃|�C���^���擾����
//=============================================================================
CTextureManager *CMap::GetTextureManager(void)
{
	return m_pTextureManager;
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^���擾����
//=============================================================================
CModelCreate *CMap::GetModelCreate(void)
{
	return m_pModelCreate;
}

//=============================================================================
//    ���C�g�Ǌ��N���X�ւ̃|�C���^���擾����
//=============================================================================
CLightManager *CMap::GetLightManager(void)
{
	return m_pLightManager;
}

//=============================================================================
//    �ǂݍ��ރ��f�����X�g���̃X�N���v�g�t�@�C�������擾����
//=============================================================================
char *CMap::GetModelListFileName(void)
{
	return m_aModelListFileName;
}

//=============================================================================
//    �ǂݍ��ރe�N�X�`�����X�g���̃X�N���v�g�t�@�C�������擾����
//=============================================================================
char *CMap::GetTexListFileName(void)
{
	return m_aTexListFileName;
}

//=============================================================================
//    �ǂݍ��ރ��C�g���̃X�N���v�g�t�@�C�������擾����
//=============================================================================
char *CMap::GetLightFileName(void)
{
	return m_aLightFileName;
}

//=============================================================================
//    �z�u���f�[�^�Ǌ��N���X�ւ̃|�C���^���擾����
//=============================================================================
CCharacterManager **CMap::GetObjectManager(void)
{
	return m_pObjectManager;
}
CCharacterManager *CMap::GetObjectManager(int nIdx)
{
	return m_pObjectManager[nIdx];
}

//=============================================================================
//    �ǂݍ��ރQ�[���t�B�[���h���̃X�N���v�g�t�@�C�������擾����
//=============================================================================
char *CMap::GetGameFieldFileName(void)
{
	return m_aGameFieldFileName;
}

//=============================================================================
//    �ǂݍ��ޔz�u�����̃X�N���v�g�t�@�C�������擾����
//=============================================================================
char *CMap::GetObjectFileName(void)
{
	return m_aObjectFileName;
}

//=============================================================================
//    �i�ߕ��N���X�ւ̃|�C���^���擾����
//=============================================================================
CHeadQuarters *CMap::GetHeadQuarters(void)
{
	return m_pHeadQuarters;
}

//=============================================================================
//    �v���C���[�̃��X�|�[���N���X�ւ̃|�C���^���擾����
//=============================================================================
CRespawn *CMap::GetPlayerRespawn(int nIdx)
{
	return m_pPlayerRespawn[nIdx];
}

//=============================================================================
//    �G�̃��X�|�[���N���X�ւ̃|�C���^���擾����
//=============================================================================
CRespawn *CMap::GetEnemyRespawn(int nIdx)
{
	return m_pEnemyRespawn[nIdx];
}

//=============================================================================
//    �G�̐������f�[�^�N���X�ւ̃|�C���^���擾����
//=============================================================================
CEnemy_ListData *CMap::GetEnemyListData(int nIdx)
{
	return m_pEnemyListData[nIdx];
}

//=============================================================================
//    �G�̐������̑������擾����
//=============================================================================
int CMap::GetNumEnemyListData(void)
{
	return m_nNumEnemyListData;
}

//=============================================================================
//    �n�ʂɒ���t����e�N�X�`���̔ԍ���ݒ肷��
//=============================================================================
void CMap::SetFieldTexIdx(const int nFieldTexIdx)
{
	m_nFieldTexIdx = nFieldTexIdx;
}

//=============================================================================
//    �n�ʃN���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CMap::SetMeshField(CMeshField *pMeshField)
{
	m_pMeshField = pMeshField;
}

//=============================================================================
//    �e�N�X�`���Ǌ��N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CMap::SetTextureManager(CTextureManager *pTextureManager)
{
	m_pTextureManager = pTextureManager;
}

//=============================================================================
//    ��N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CMap::SetSky(CSky *pSky)
{
	m_pSky = pSky;
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CMap::SetModelCreate(CModelCreate *pModelCreate)
{
	m_pModelCreate = pModelCreate;
}

//=============================================================================
//    ���C�g�Ǌ��N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CMap::SetLightManager(CLightManager *pLightManager)
{
	m_pLightManager = pLightManager;
}

//=============================================================================
//    �z�u���f�[�^�Ǌ��N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CMap::SetObjectManager(CCharacterManager **pObjectManager)
{
	m_pObjectManager = pObjectManager;
}
void CMap::SetObjectManager(CCharacterManager *pObjectManager, int nIdx)
{
	m_pObjectManager[nIdx] = pObjectManager;
}

//=============================================================================
//    �ǂݍ��ރ��f�����X�g���̃X�N���v�g�t�@�C������ݒ肷��
//=============================================================================
void CMap::SetModelListFileName(char *pFileName)
{
	strcpy(m_aModelListFileName, pFileName);
}

//=============================================================================
//    �ǂݍ��ރe�N�X�`�����X�g���̃X�N���v�g�t�@�C������ݒ肷��
//=============================================================================
void CMap::SetTexListFileName(char *pFileName)
{
	strcpy(m_aTexListFileName, pFileName);
}

//=============================================================================
//    �ǂݍ��ރ��C�g���̃X�N���v�g�t�@�C������ݒ肷��
//=============================================================================
void CMap::SetLightFileName(char *pFileName)
{
	strcpy(m_aLightFileName, pFileName);
}

//=============================================================================
//    �ǂݍ��ރQ�[���t�B�[���h���̃X�N���v�g�t�@�C������ݒ肷��
//=============================================================================
void CMap::SetGameFieldFileName(char *pFileName)
{
	strcpy(m_aGameFieldFileName, pFileName);
}

//=============================================================================
//    �ǂݍ��ޔz�u�����̃X�N���v�g�t�@�C������ݒ肷��
//=============================================================================
void CMap::SetObjectFileName(char *pFileName)
{
	strcpy(m_aObjectFileName, pFileName);
}

//=============================================================================
//    �ǂݍ��ޓG�̐������̃X�N���v�g�t�@�C������ݒ肷��
//=============================================================================
void CMap::SetEnemyListFileName(char *pFileName)
{
	strcpy(m_aEnemyListFileName, pFileName);
}

//=============================================================================
//    �i�ߕ��N���X�^�̃|�C���^��ݒ肷��
//=============================================================================
void CMap::SetHeadQuarters(CHeadQuarters *pHeadQuarters)
{
	m_pHeadQuarters = pHeadQuarters;
}

//=============================================================================
//    �v���C���[�̃��X�|�[���N���X�^�̃|�C���^��ݒ肷��
//=============================================================================
void CMap::SetPlayerRespawn(CRespawn *pRespawn, int nIdx)
{
	m_pPlayerRespawn[nIdx] = pRespawn;
}

//=============================================================================
//    �G�̃��X�|�[���N���X�^�̃|�C���^��ݒ肷��
//=============================================================================
void CMap::SetEnemyRespawn(CRespawn *pRespawn, int nIdx)
{
	m_pEnemyRespawn[nIdx] = pRespawn;
}

//=============================================================================
//    �G�̐������f�[�^��ݒ肷��
//=============================================================================
void CMap::SetEnemyListData(CEnemy_ListData EnemyData, int nIdx)
{
	m_pEnemyListData[nIdx]->Cpy(EnemyData);
}

//=============================================================================
//    �G�̐������̑�����ݒ肷��
//=============================================================================
void CMap::SetNumEnemyListData(const int nNumEnemyData)
{
	m_nNumEnemyListData = nNumEnemyData;
}

//=============================================================================
//    �I�u�W�F�N�g�f�[�^�̑�����ݒ肷��
//=============================================================================
void CMap::SetNumObjectData(const int nObjectData)
{
	m_nNumObjectData = nObjectData;
}


//*****************************************************************************
//    CEnemy_ListData�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CEnemy_ListData::CEnemy_ListData()
{
	m_nRespawnIdx = 0;
	m_nEnemyType = 0;
	m_nRespawnTime = 0;
	m_bItem = false;
	m_nItemType = 0;
	m_nAI_BulletEva = 0;
	m_nAI_MassEva = 0;
	m_nAI_DownEva = 0;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CEnemy_ListData::~CEnemy_ListData()
{

}

//=============================================================================
//    �f�[�^���R�s�[���鏈��
//=============================================================================
void CEnemy_ListData::Cpy(CEnemy_ListData EnemyData)
{
	m_nRespawnIdx = EnemyData.GetRespawnIdx();
	m_nEnemyType = EnemyData.GetEnemyType();
	m_nRespawnTime = EnemyData.GetRespawnTime();
	m_bItem = EnemyData.GetItem();
	m_nItemType = EnemyData.GetItemType();
	m_nAI_BulletEva = EnemyData.GetAI_BulletEva();
	m_nAI_MassEva = EnemyData.GetAI_MassEva();
	m_nAI_DownEva = EnemyData.GetAI_DownEva();
}

//=============================================================================
//    �G�̃��X�|�[���ʒu�̔ԍ���ݒ肷��
//=============================================================================
void CEnemy_ListData::SetRespawnIdx(const int nRespawnIdx)
{
	m_nRespawnIdx = nRespawnIdx;
}

//=============================================================================
//    �G�̎�ޔԍ���ݒ肷��
//=============================================================================
void CEnemy_ListData::SetEnemyType(const int nEnemyType)
{
	m_nEnemyType = nEnemyType;
}

//=============================================================================
//    �G���o��������^�C�~���O��ݒ肷��
//=============================================================================
void CEnemy_ListData::SetRespawnTime(const int nRespawnTime)
{
	m_nRespawnTime = nRespawnTime;
}

//=============================================================================
//    �|�������ɃA�C�e�����o�������邩�ǂ����ݒ肷��
//=============================================================================
void CEnemy_ListData::SetItem(const bool bItem)
{
	m_bItem = bItem;
}

//=============================================================================
//    �o��������A�C�e���̎�ޔԍ���ݒ肷��
//=============================================================================
void CEnemy_ListData::SetItemType(const int nItemType)
{
	m_nItemType = nItemType;
}

//=============================================================================
//    �e��łۂ̃����_���]���l��ݒ肷��
//=============================================================================
void CEnemy_ListData::SetAI_BulletEva(const int nAI_BulletEva)
{
	m_nAI_BulletEva = nAI_BulletEva;
}

//=============================================================================
//    �}�X���ړ������ۂɌ�����ς������邩���肷�鎞�̃����_���]���l��ݒ肷��
//=============================================================================
void CEnemy_ListData::SetAI_MassEva(const int nAI_MassEva)
{
	m_nAI_MassEva = nAI_MassEva;
}

//=============================================================================
//    �������ɓ��������ǂ����𔻒肷��ۂ̃����_���]���l��ݒ肷��
//=============================================================================
void CEnemy_ListData::SetAI_DownEva(const int nAI_DownEva)
{
	m_nAI_DownEva = nAI_DownEva;
}

//=============================================================================
//    �G�̃��X�|�[���ʒu�̔ԍ����擾����
//=============================================================================
int CEnemy_ListData::GetRespawnIdx(void)
{
	return m_nRespawnIdx;
}

//=============================================================================
//    �G�̎�ޔԍ����擾����
//=============================================================================
int CEnemy_ListData::GetEnemyType(void)
{
	return m_nEnemyType;
}

//=============================================================================
//    �G���o��������^�C�~���O���擾����
//=============================================================================
int CEnemy_ListData::GetRespawnTime(void)
{
	return m_nRespawnTime;
}

//=============================================================================
//    �|�������ɃA�C�e�����o�������邩�ǂ����擾����
//=============================================================================
bool CEnemy_ListData::GetItem(void)
{
	return m_bItem;
}

//=============================================================================
//    �o��������A�C�e���̎�ޔԍ����擾����
//=============================================================================
int CEnemy_ListData::GetItemType(void)
{
	return m_nItemType;
}

//=============================================================================
//    �e��łۂ̃����_���]���l���擾����
//=============================================================================
int CEnemy_ListData::GetAI_BulletEva(void)
{
	return m_nAI_BulletEva;
}

//=============================================================================
//    �}�X���ړ������ۂɌ�����ς������邩���肷�鎞�̃����_���]���l���擾����
//=============================================================================
int CEnemy_ListData::GetAI_MassEva(void)
{
	return m_nAI_MassEva;
}

//=============================================================================
//    �������ɓ��������ǂ����𔻒肷��ۂ̃����_���]���l���擾����
//=============================================================================
int CEnemy_ListData::GetAI_DownEva(void)
{
	return m_nAI_DownEva;
}