//=============================================================================
//
// �^�C�g���̏��� [title.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "basemode.h"
#include "scene3D.h"

//�O���錾
class CFileLoader;
class CFileSaver;
class CScene2D;
class CNumber;
class CScene2DFlash;
class CScene2DPress;
class CSky;
class CMeshField;
class CLightManager;
class CPlayer;
class CPlayerManager;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TITLEBG_NUM (2)    // �^�C�g���p�w�i�̐�

//*****************************************************************************
// �^�C�g���p���S�N���X�̒�`
//*****************************************************************************
class CTitleLogo : public CScene3D
{
public:    // �N�ł��A�N�Z�X�\
	CTitleLogo(int nPriority = 3, OBJTYPE objtype = OBJTYPE_SCENE3D);
	~CTitleLogo();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetPolypos(const D3DXVECTOR3 Polypos);
	D3DXVECTOR3 GetPolypos(void);

	// �ÓI�����o�֐�
	static CTitleLogo *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority = 3, float fRot = 0.0f, float fStartTexU = 0.0f, float fStartTexV = 0.0f, float fAddTexU = 1.0f, float fAddTexV = 1.0f);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	D3DXVECTOR3 m_Polypos;
};

//*****************************************************************************
// �^�C�g���N���X�̒�`
//*****************************************************************************
class CTitle : public CBasemode
{
public:	// �N������A�N�Z�X�\
	//--------------
	// ���
	//--------------
	typedef enum
	{
		STATE_NONE = -1,
		STATE_TEAMLOGO,
		STATE_FADEOUT,
		STATE_FADEIN,
		STATE_NORMAL,
		STATE_GAMEMODE,
		STATE_MAX
	}STATE;

	//--------------
	// ���
	//--------------
	typedef enum
	{
		GAMEMODE_LOCAL1P = 0,
		GAMEMODE_LOCAL2P,
		GAMEMODE_ONLINE2P,
		GAMEMODE_MAX
	}GAMEMODE;

	// �����o�֐�
	CTitle();
	~CTitle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(const STATE state);
	STATE GetState(void);

	// �ÓI�����o�֐�
	static CTitle *Create(void);
	static int GetGameMode(void);

	static void SetHighScore(const int nHighScore);
	static int GetHighScore(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:	// �����������A�N�Z�X�\
	void ClearVariable(void);
	void ClearPress(void);
	void ClearGameModeBg(void);
	void ClearGameModeLogo(void);
	void ClearGameModeLogo_PolyNone(void);
	void ClearGameModeLogo_PolySelect(void);
	void ClearSky(void);
	void ClearField(void);
	void ClearLight(void);
	void ClearPlayer(void);
	void ClearHighScore(void);

	void CreateTexture(int nNumTex);
	void CreateCamera(void);
	void CreateFadePolygon(void);
	void CreateTeamBgPolygon(void);
	void CreatePress(void);
	void CreateGameModeBg(void);
	void CreateGameModeLogo(void);
	void CreateSky(void);
	void CreateField(void);
	void CreateLight(void);
	void CreatePlayer(void);
	void CreatePlayerManagerPointer(void);
	void CreatePlayerManager(void);
	void CreatePlayerDataPointer(void);
	void CreateLightDataPointer(void);
	void CreateHighScore(void);
	void CreateLoad(void);
	void CreateObject(void);

	void ReleaseFadePolygon(void);
	void ReleaseTeamLogoPolygon(void);
	void ReleaseTeamBgPolygon(void);
	void ReleaseTitleLogoPolygon(void);
	void ReleasePress(void);
	void ReleaseGameModeBg(void);
	void ReleaseGameModeLogo(void);
	void ReleaseSky(void);
	void ReleaseField(void);
	void ReleaseLight(void);
	void ReleasePlayer(void);
	void ReleasePlayerManagerPointer(void);
	void ReleasePlayerManager(void);
	void ReleasePlayerDataPointer(void);
	void ReleaseLightDataPointer(void);
	void ReleaseHighScore(void);

	void TeamLogoUpdate(void);
	void FadeOutUpdate(void);
	void FadeInUpdate(void);
	void NormalUpdate(void);
	void GameModeUpdate(void);
	void WaitInputToNormal(void);
	void WaitInputToGameMode(void);
	void SetNextMode_None(void);
	void SetNextMode_Push(void);
	void ChangeNoneGameModeLogoPoly(int nSelect);
	void ChangeSelectGameModeLogoPoly(int nSelect);
	void AddFieldTexUV(void);
	void CreateDashEffect(void);

	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadTexFileName(char *pStr, int nCntTex);
	void LoadModel(char *pStr, int nCntModel);
	void LoadEffectFileName(char *pStr);
	void LoadTitleLogo(CFileLoader *pFileLoader, char *pStr);
	void LoadPress(CFileLoader *pFileLoader, char *pStr);
	void LoadTeamLogo(CFileLoader *pFileLoader, char *pStr);
	void LoadGameModebg(CFileLoader *pFileLoader, char *pStr);
	void LoadGameModeLogo(CFileLoader *pFileLoader, char *pStr);
	void LoadGameModeLogo_PolyNone(CFileLoader *pFileLoader, char *pStr);
	void LoadGameModeLogo_PolySelect(CFileLoader *pFileLoader, char *pStr);
	void LoadPlayerDataFileName(char *pStr, int nCntPlayerData);
	void LoadPlayerData(CFileLoader *pFileLoader, char *pStr, int nCntPlayer);
	void LoadLight(CFileLoader *pFileLoader, char *pStr, int nCntLight);
	void LoadField(CFileLoader *pFileLoader, char *pStr);
	void LoadSky(CFileLoader *pFileLoader, char *pStr);

	static int m_nGameMode;            // �Q�[�����[�h(�e��v���O�����ōX�V����)
	static int m_nHighScore;           // �Q�[���̃n�C�X�R�A(�X�V��CGame�N���X���s��)
	STATE m_State;                     // ���
	CScene2D *m_pFade;                 // �t�F�[�h�Ɏg�p����|���S��
	CScene2D *m_pTeamLogo;             // �`�[�����S�p�|���S���N���X�^�̃|�C���^
	CScene2D *m_pTeamBg;               // �`�[�����S�w�i�p�|���S���N���X�^�̃|�C���^
	CScene2D *m_pDecide;               // ���胍�S�p�|���S���N���X�^�̃|�C���^
	CScene2D *m_pRetrun;               // ���ǂ郍�S�p�|���S���N���X�^�̃|�C���^
	CScene2D *m_pIcon;                 // �^�C�g���̃A�C�R���|���S���N���X�^�̃|�C���^
	CTitleLogo *m_pTitleLogo;          // �^�C�g�����S�N���X�^�̃|�C���^
	int m_nStateCounter;               // ��Ԃ��Ǘ�����J�E���^�[

	// �n�C�X�R�A�p�f�[�^
	CScene2D *m_pHighScoreLogo;
	CNumber *m_pHighScore;

	// �v���X�|���S���p�f�[�^
	typedef struct
	{
		int nTexIdx;
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
		float fWidth;
		float fHeight;
	}PRESS_DATA;
	PRESS_DATA m_PressData;
	CScene2DPress *m_pPress;

	// �Q�[�����[�h�w�i�|���S���p�f�[�^
	typedef struct
	{
		int nTexIdx;
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
		float fWidth;
		float fHeight;
	}GAMEMODEBG_DATA;
	GAMEMODEBG_DATA m_GameModeBgData;
	CScene2D *m_pGameLogoBg;

	// �Q�[�����[�h���S�|���S���p�f�[�^
	typedef struct
	{
		D3DXCOLOR col;
		float fWidth;
		float fHeight;
	}GAMEMODELOGO_POLYDATA;
	typedef struct
	{
		D3DXVECTOR3 pos;
		int nTexIdx;
		float fInterval;
	}GAMEMODELOGO_DATA;
	GAMEMODELOGO_DATA m_GameModeLogoData;
	GAMEMODELOGO_POLYDATA m_GameModeLogoPoly_NoneData;
	GAMEMODELOGO_POLYDATA m_GameModeLogoPoly_SelectData;
	CScene2DFlash *m_apGameModeLogo[GAMEMODE_MAX];

	// ��p�f�[�^
	typedef struct
	{
		int nTexIdx;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		D3DXCOLOR col;
		float fRadius;
		int nXBlock;
		int nZBlock;
		float fRollSpeed;
	}SKYDATA;
	SKYDATA m_SkyData;
	CSky *m_pSky;

	// �n�ʗp�f�[�^
	typedef struct
	{
		int nTexIdx;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		D3DXCOLOR col;
		float fWidth;
		float fDepth;
		int nXBlock;
		int nZBlock;
		int nTexUSplit;
		int nTexVSplit;
		bool bCulling;
		float fAddTex;
	}FIELDDATA;
	FIELDDATA m_FieldData;
	CMeshField *m_pMeshField;

	// ���C�g�p�f�[�^
	typedef struct
	{
		D3DXVECTOR3 Dir;
		D3DXCOLOR Diffuse;
		D3DXCOLOR Ambient;
		D3DXCOLOR Specular;
	}LIGHTDATA;
	int m_nNumLight;
	LIGHTDATA *m_pLightData;
	CLightManager *m_pLightManager;

	// �v���C���[�p�f�[�^
	typedef struct
	{
		int nType;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	}PLAYERDATA;
	int m_nNumPlayerData;
	char **m_aPlayerDataFileName;
	CPlayerManager	**m_pPlayerManager;
	int m_nNumPlayer;
	PLAYERDATA *m_pPlayerData;
	CPlayer **m_pPlayer;
};
#endif