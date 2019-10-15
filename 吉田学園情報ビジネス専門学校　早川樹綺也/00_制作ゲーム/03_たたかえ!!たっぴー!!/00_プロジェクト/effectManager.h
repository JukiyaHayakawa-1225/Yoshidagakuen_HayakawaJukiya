//*****************************************************************************
//
//     �G�t�F�N�g�Ǌ��̏���[effectManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _EFFECTMANAGER_H_
#define _EFFECTMANAGER_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define MAX_EMMITER         (150)     // �G�~�b�^�f�[�^�̍ő吔
#define MAX_PARTICLEDATA    (150)     // �p�[�e�B�N���f�[�^�̍ő吔
#define MAX_RINGEFFECTDATA  (150)     // �����O�G�t�F�N�g�f�[�^�̍ő吔

//*****************************************************************************
//     �O���錾
//*****************************************************************************
class CFileLoader;
class CFileSaver;
class CParData;
class CEmitterData;
class CRingData;
class CTextureManager;
class CEmitter;

//*****************************************************************************
//     �G�t�F�N�g�}�l�[�W���[�N���X�̒�`
//*****************************************************************************
class CEffectManager : CScene
{
public:   // �N�ł��A�N�Z�X�\
	typedef enum
	{
		EFFECT_TYPE_PARTICLE = 0,  // �p�[�e�B�N��
		EFFECT_TYPE_RING,          // �����O
		EFFECT_TYPE_MAX
	}EFFECT_TYPE;

	CEffectManager(int nPriority = 3, OBJTYPE objType = OBJTYPE_EFFECTMANAGER);
	~CEffectManager();

	static CEffectManager *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CEmitter *SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);

	void SetFileName(char *pFileName);
	void SetEmitter(CEmitter *pEmitter);
	void SetEmitterData(CEmitterData *pEmitterData, const int nIdx);
	void SetParData(CParData *pParData, const int nIdx);
	void SetRingEffectData(CRingData *pRingData, const int nIdx);
	void SetTexManager(CTextureManager *pTexManager);
	void SetNumEmitterData(const int nNumEmitterData);
	void SetNumParData(const int nNumParData);
	void SetNumRingEffectData(const int nNumRingEffectData);

	CEmitter *GetEmitter(void);
	LPDIRECT3DTEXTURE9 GetTexture(const int nIdx);
	CEmitterData **GetEmitterData(void);
	CEmitterData *GetEmitterData(const int nIdx);
	CParData **GetParData(void);
	CParData *GetParData(const int nIdx);
	CRingData **GetRingEffectData(void);
	CRingData *GetRingEffectData(const int nIdx);
	CTextureManager *GetTexManager(void);
	int GetNumEmitterData(void);
	int GetNumParData(void);
	int GetNumRingEffectData(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CreateTextureManager(const int nNumTex);
	void CreateEmitterDataMemory(const int nNumEmitterData);
	void CreateParticleDataMemory(const int nNumParticleData);
	void CreateRingEffectDataMemory(const int nNumRingEffectData);

	void ReleaseTextureManager(void);
	void ReleaseEmitterData(void);
	void ReleaseParticleData(void);
	void ReleaseRingEffectData(void);

	HRESULT LoadScript(CFileLoader *pFileLoader, char *pStr);
	void LoadTexture(char *pStr, const int nCntTex);
	CEmitterData *LoadEmitterData(CFileLoader *pFileLoader, char *pStr, const int nCntEmitter);
	CParData *LoadParticleData(CFileLoader *pFileLoader, char *pStr, const int nCntParData);
	CRingData *LoadRingEffectData(CFileLoader *pFileLoader, char *pStr, const int nCntRingData);

	void Save(void);
	void SaveTextureData(CFileSaver *pFileSaver);
	void SaveEmitterData(CFileSaver *pFileSaver);
	void SaveParticleData(CFileSaver *pFileSaver);
	void SaveRingEffectData(CFileSaver *pFileSaver);

	char            m_aFileName[256];         // �X�N���v�g�t�@�C����
	CEmitter        *m_pEmitter;              // �Ō�ɐ��������G�~�b�^�N���X�ւ̃|�C���^
	int             m_nNumEmitterData;        // �G�~�b�^�f�[�^�̌�
	int             m_nNumParData;            // �p�[�e�B�N���f�[�^�̌�
	int             m_nNumRingEffectData;     // �����O�G�t�F�N�g�f�[�^�̌�
	CEmitterData    **m_apEmitterData;        // �G�~�b�^�f�[�^�N���X�ւ̃|�C���^
	CParData        **m_apParData;            // �p�[�e�B�N���f�[�^�N���X�ւ̃|�C���^
	CRingData       **m_apRingData;           // �����O�G�t�F�N�g�f�[�^�N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager;       // �e�N�X�`���Ǘ��N���X�ւ̃|�C���^
};

#endif