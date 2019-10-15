//*****************************************************************************
//
//     �}�b�v�̏���[map.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MAP_H_
#define _MAP_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define MAP_FILENAME           "MAP_FILENAME = "                // �}�b�v�̃t�@�C������ǂݍ��ލ��}
#define SAVEFILENAME_MAP       "data/TEXT/MAP/"                 // �}�b�v���ۑ���̃p�X��
#define SAVEFILENAME_GAMEFIELD "data/TEXT/MAP/GAMEFIELD/"       // �Q�[���t�B�[���h���ۑ���̃p�X��
#define SAVEFILENAME_MODELLIST "data/TEXT/MAP/MODELLIST/"       // ���f�����X�g���ۑ���̃p�X��
#define SAVEFILENAME_TEXLIST   "data/TEXT/MAP/TEXLIST/"         // �e�N�X�`�����X�g���ۑ���̃p�X��
#define SAVEFILENAME_LIGHT     "data/TEXT/MAP/LIGHT/"           // ���C�g���ۑ���̃p�X��
#define SAVEFILENAME_OBJECT    "data/TEXT/MAP/OBJECT/"          // �z�u�����ۑ���̃p�X��
#define SAVEFILENAME_ENEMYLIST "data/TEXT/MAP/ENEMYLIST/"       // �G�̐������ۑ���̃p�X��
#define MAX_PLAYER_RESPAWN     (2)                              // �v���C���[�̃��X�|�[���ʒu�̍ő�l
#define MAX_ENEMY_RESPAWN      (3)                              // �G�̃��X�|�[���ʒu�̍ő�l

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CFileLoader;
class CFileSaver;
class CTextureManager;
class CModelCreate;
class CLightManager;
class CCharacterManager;
class CEffectManager;
class CBlock;
class CRiver;
class CIceField;
class CDirectionalLight;
class CPointLight;
class CSpotLight;
class CMeshField;
class CRespawn;
class CHeadQuarters;
class CSky;
class CObject;
class CBillboardObject;
class CEmitter;

//*****************************************************************************
//    �G�̐������f�[�^�N���X�̒�`
//*****************************************************************************
class CEnemy_ListData
{
public:    // �N�ł��A�N�Z�X�\
	CEnemy_ListData();
	~CEnemy_ListData();

	void Cpy(CEnemy_ListData EnemyData);

	void SetRespawnIdx(const int nRespawnIdx);
	void SetEnemyType(const int nEnemyType);
	void SetRespawnTime(const int nRespawnTime);
	void SetItem(const bool bItem);
	void SetItemType(const int nItemType);
	void SetAI_BulletEva(const int nAI_BulletEva);
	void SetAI_MassEva(const int nAI_MassEva);
	void SetAI_DownEva(const int nAI_DownEva);

	int GetRespawnIdx(void);
	int GetEnemyType(void);
	int GetRespawnTime(void);
	bool GetItem(void);
	int GetItemType(void);
	int GetAI_BulletEva(void);
	int GetAI_MassEva(void);
	int GetAI_DownEva(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int m_nRespawnIdx;     // ���X�|�[���ʒu�̔ԍ�
	int m_nEnemyType;      // �G�̎��
	int m_nRespawnTime;    // �G���o�������鎞��(60FPS��Ŋ��Z)
	bool m_bItem;          // �|�������ɃA�C�e�����o�������邩
	int m_nItemType;       // �o������A�C�e���̎�ޔԍ�
	int m_nAI_BulletEva;   // �e��łۂ̃����_���]���l
	int m_nAI_MassEva;     // �}�X���ړ������ۂɌ�����ς������邩���肷��Ƃ��̃����_���]���l
	int m_nAI_DownEva;     // �������ɓ��������ǂ����𔻒肷��ۂ̃����_���]���l
};

//*****************************************************************************
//    �}�b�v�N���X�̒�`
//*****************************************************************************
class CMap
{
public:    // �N�ł��A�N�Z�X�\
	CMap();
	~CMap();

	static CMap *Create(char *pFileName);

	HRESULT Init(char *pFileName);
	void Uninit(void);
	HRESULT Load(char *pLoadFileName);
	HRESULT Save(char *pSaveFileName);
	HRESULT ChangeMap(char *pLoadFileName);

	HRESULT LoadScript(char *pStr, CFileLoader *pFileLoader);
	HRESULT LoadModelList(char *pModelFileName, char *pStr);
	HRESULT LoadModel(char *pStr, CFileLoader *pFileLoader);
	HRESULT LoadTextureList(char *pTextureFileName, char *pStr);
	HRESULT LoadTexture(char *pStr, CFileLoader *pFileLoader);
	HRESULT LoadHeadQuartersPos(char *pStr);
	HRESULT LoadPlayerRespawn(char *pStr, int nCntPlayerRes);
	HRESULT LoadEnemyRespawn(char *pStr, int nCntEnemyRes);
	HRESULT LoadSky(char *pStr, CFileLoader *pFileLoader);
	HRESULT LoadMap(char *pStr, CFileLoader *pFileLoader);
	HRESULT LoadLight(char *pLightFileName, char *pStr);
	HRESULT LoadLightInfo(char *pStr, CFileLoader *pFileLoader);
	HRESULT LoadDirectionalLight(char *pStr, CFileLoader *pFileLoader, int nCntLight);
	HRESULT LoadPointLight(char *pStr, CFileLoader *pFileLoader, int nCntLight);
	HRESULT LoadSpotLight(char *pStr, CFileLoader *pFileLoader, int nCntLight);
	HRESULT LoadGameField(char *pGameFieldFileName, char *pStr);
	HRESULT LoadGameFieldInfo(char *pStr, CFileLoader *pFileLoader);
	HRESULT LoadObject(char *pObjectName, char *pStr);
	HRESULT LoadObjectInfo(char *pStr, CFileLoader *pFileLoader);
	HRESULT LoadEnemyList(char *pEnemylistName, char *pStr);
	HRESULT LoadEnemyListInfo(char *pStr, CFileLoader *pFileLoader, int nCntEnemyList);

	HRESULT SaveModelList(CFileSaver *pFileSaver);
	HRESULT SaveTexList(CFileSaver *pFileSaver);
	HRESULT SavePlayerRespawn(CFileSaver *pFileSaver);
	HRESULT SaveEnemyRespawn(CFileSaver *pFileSaver);
	HRESULT SaveHeadQuartersPos(CFileSaver *pFileSaver);
	HRESULT SaveSky(CFileSaver *pFileSaver);
	HRESULT SaveMap(CFileSaver *pFileSaver);
	HRESULT SaveModel(char *pModelFileName);
	HRESULT SaveTexture(char *pTextureFileName);
	HRESULT SaveLight(char *pLightFileName);
	HRESULT SaveGameField(char *pGameFieldFileName);
	HRESULT SaveObject(char *pObjectName);
	HRESULT SaveEnemyList(char *pEnemyListName);

	void CreateBlock(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxColWidth, float fBoxColHeight, float fBoxColDepth);

	void DeleteMap(void);
	void DeleteLight(void);
	void DeleteGameField(void);
	void DeleteObject(void);
	void DeleteEnemy(void);

	int GetFieldTexIdx(void);
	CMeshField *GetMeshField(void);
	CSky *GetSky(void);
	CTextureManager *GetTextureManager(void);
	CModelCreate *GetModelCreate(void);
	CLightManager *GetLightManager(void);
	CCharacterManager **GetObjectManager(void);
	CCharacterManager *GetObjectManager(int nIdx);
	char *GetModelListFileName(void);
	char *GetTexListFileName(void);
	char *GetLightFileName(void);
	char *GetGameFieldFileName(void);
	char *GetObjectFileName(void);
	CHeadQuarters *GetHeadQuarters(void);
	CRespawn *GetPlayerRespawn(int nIdx);
	CRespawn *GetEnemyRespawn(int nIdx);
	CEnemy_ListData *GetEnemyListData(int nIdx);
	int GetNumEnemyListData(void);

	void SetFieldTexIdx(const int nFieldTexIdx);
	void SetMeshField(CMeshField *pMeshField);
	void SetSky(CSky *pSky);
	void SetTextureManager(CTextureManager *pTextureManager);
	void SetModelCreate(CModelCreate *pModelCreate);
	void SetLightManager(CLightManager *pLightManager);
	void SetObjectManager(CCharacterManager **pObjectManager);
	void SetObjectManager(CCharacterManager *pObjectManager, int nIdx);
	void SetModelListFileName(char *pFileName);
	void SetTexListFileName(char *pFileName);
	void SetLightFileName(char *pFileName);
	void SetGameFieldFileName(char *pFileName);
	void SetObjectFileName(char *pFileName);
	void SetEnemyListFileName(char *pFileName);
	void SetHeadQuarters(CHeadQuarters *pHeadQuarters);
	void SetPlayerRespawn(CRespawn *pRespawn, int nIdx);
	void SetEnemyRespawn(CRespawn *pRespawn, int nIdx);
	void SetEnemyListData(CEnemy_ListData EnemyData, int nIdx);
	void SetNumEnemyListData(const int nNumEnemyData);
	void SetNumObjectData(const int nObjectData);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void LoadField(char *pStr, CFileLoader *pFileLoader);
	void LoadBlock(char *pStr, CFileLoader *pFileLoader);
	void LoadCollision(char *pStr, CFileLoader *pFileLoader, float *pWidth, float *pHeight, float *pDepth);
	void LoadRiver(char *pStr, CFileLoader *pFileLoader);
	void LoadIce(char *pStr, CFileLoader *pFileLoader);
	void LoadNumObjectData(char *pStr, CFileLoader *pFileLoader);
	void LoadObjectData(char *pStr, CFileLoader *pFileLoader, int nCntObjData);
	void LoadObjModel(char *pStr, CFileLoader *pFileLoader);
	void LoadObjBill(char *pStr, CFileLoader *pFileLoader);
	void LoadObjEffect(char *pStr, CFileLoader *pFileLoader, CEffectManager *pEffectManager);
	void LoadEnemyItem(char *pStr, CFileLoader *pFileLoader, int nCntEnemyList);
	void LoadEnemyAI(char *pStr, CFileLoader *pFileLoader, int nCntEnemyList);

	void FieldDown(D3DXVECTOR3 pos, int nXBlock, int nZBlock, float fFieldDown);
	void SetRiver(int nTexIdx, D3DXVECTOR3 pos, int nXBlock, int nZBlock, float fBoxColWidth, float fBoxColHeight, float fBoxColDepth);
	void SetIce(int nTexIdx, D3DXVECTOR3 pos, int nXBlock, int nZBlock, float fBoxColWidth, float fBoxColHeight, float fBoxColDepth);

	void SaveGameFieldInfo(CFileSaver *pFileSaver);
	void SaveField(CFileSaver *pFileSaver);
	void SaveBlock(CFileSaver *pFileSaver);
	void SaveBlockInfo(CBlock *pBlock, CFileSaver *pFileSaver);
	void SaveRiver(CFileSaver *pFileSaver);
	void SaveRiverInfo(CRiver *pRiver, CFileSaver *pFileSaver);
	void SaveIce(CFileSaver *pFileSaver);
	void SaveIceInfo(CIceField *pIce, CFileSaver *pFileSaver);
	void SaveLightInfo(CFileSaver *pFileSaver);
	void SaveDirectionalLight(CDirectionalLight *pDirLight, CFileSaver *pFileSaver);
	void SavePointLight(CPointLight *pPointLight, CFileSaver *pFileSaver);
	void SaveSpotLight(CSpotLight *pSpotLight, CFileSaver *pFileSaver);
	void SaveObjectInfo(CFileSaver *pFileSaver);
	void SaveObjModel(CFileSaver *pFileSaver);
	void SaveObjModelInfo(CObject *pObject, CFileSaver *pFileSaver);
	void SaveObjBill(CFileSaver *pFileSaver);
	void SaveObjBillInfo(CBillboardObject *pBillObj, CFileSaver *pFileSaver);
	void SaveObjEffect(CFileSaver *pFileSaver);
	void SaveObjEffectInfo(CEmitter *pEmitter, CFileSaver *pFileSaver);
	void SaveEnemyListInfo(CFileSaver *pFileSaver);

	void CreateEnemyListData(void);

	void DeleteBlock(CBlock *pBlock);
	void DeleteRiver(CRiver *pRiver);
	void DeleteIce(CIceField *pIceField);
	void DeleteObjModel(CObject *pObject);
	void DeleteObjBillboard(CBillboardObject *pBillboard);
	void DeleteObjEffect(CEmitter *pEmitter);

	void ReleaseTextureManager(void);
	void ReleaseModelManager(void);
	void ReleaseLightManager(void);
	void ReleaseObjectManager(void);
	void ReleaseMeshField(void);
	void ReleaseSky(void);
	void ReleaseHeadQuarters(void);
	void ReleasePlayerRespawn(void);
	void ReleaseEnemyRespawn(void);
	void ReleaseEnemyListData(void);

	int             m_nFieldTexIdx;                          // �n�ʂɒ���t����e�N�X�`���̔ԍ�
	char            m_aFileName[256];                        // �ǂݍ��ރ}�b�v�f�[�^�̃X�N���v�g�t�@�C����
	char            m_aModelListFileName[256];               // �ǂݍ��ރ��f�����X�g���X�N���v�g�t�@�C����
	char            m_aTexListFileName[256];                 // �ǂݍ��ރe�N�X�`�����X�g���̃X�N���v�g�t�@�C����
	char            m_aLightFileName[256];                   // �ǂݍ��ރ��C�g���̃X�N���v�g�t�@�C����
	char            m_aGameFieldFileName[256];               // �ǂݍ��ރQ�[���t�B�[���h���̃X�N���v�g�t�@�C����
	char            m_aObjectFileName[256];                  // �ǂݍ��ޔz�u�����̃X�N���v�g�t�@�C����
	char            m_aEnemyListFileName[256];               // �ǂݍ��ޓG�̐������̃X�N���v�g�t�@�C����

	CMeshField        *m_pMeshField;                         // �n�ʃN���X�ւ̃|�C���^
	CSky              *m_pSky;                               // ��N���X�ւ̃|�C���^
	CHeadQuarters     *m_pHeadQuarters;                      // �i�ߕ��N���X�^�̃|�C���^
	CRespawn          *m_pPlayerRespawn[MAX_PLAYER_RESPAWN]; // �v���C���[�̃��X�|�[���ʒu(2�l��)
	CRespawn          *m_pEnemyRespawn[MAX_ENEMY_RESPAWN];   // �G�̃��X�|�[���ʒu(3�ӏ���)
	CTextureManager   *m_pTextureManager;                    // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	CModelCreate      *m_pModelCreate;                       // ���f���Ǌ��N���X�ւ̃|�C���^
	CLightManager     *m_pLightManager;                      // ���C�g�Ǌ��N���X�ւ̃|�C���^
	CCharacterManager **m_pObjectManager;                    // �z�u���̃f�[�^�Ǌ��N���X�ւ̃|�C���^
	int               m_nNumObjectData;                      // �z�u���̃f�[�^�̐�
	CEnemy_ListData   **m_pEnemyListData;                    // �G�̐������N���X�ւ̃|�C���^
	int               m_nNumEnemyListData;                   // �G�̐������̐�
};

#endif