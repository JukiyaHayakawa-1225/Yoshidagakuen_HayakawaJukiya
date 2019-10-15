//*****************************************************************************
//
//     �A�C�e���̏���[item.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _ITEM_H_
#define _ITEM_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "object3D.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define ITEM_PRIORITY (4)

//*****************************************************************************
//    �A�C�e���N���X�̒�`
//*****************************************************************************
class CItem : public CObject3D
{
public:    // �N�ł��A�N�Z�X�\
	//-------------------
	//   ���
	//-------------------
	typedef enum
	{
		STATE_NORMAL = 0,   // �ʏ�
		STATE_DEATH,        // ���S
		STATE_MAX
	}STATE;

	//-------------------
	//   ���
	//-------------------
	typedef enum
	{
		TYPE_NONE = -1,    // ����
		TYPE_STAR,         // �X�^�[(�p���[�A�b�v3�i�K�܂�)
		TYPE_GRENADE,      // ��֒e(�G�S��)
		TYPE_1UP_TANK,     // �^���N(�c�@UP)
		TYPE_SCOOP,        // �X�R�b�v(��莞�Ԏi�ߕ����G)
		TYPE_CLOCK,        // ���v(��莞�ԓG���~�߂�)
		TYPE_HELMET,       // �w�����b�g(��莞�Ԗ��G)
		TYPE_MAX
	}TYPE;

	CItem(int nPriority = 3, OBJTYPE objType = OBJTYPE_ITEM);
	~CItem();

	static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(LPDIRECT3DDEVICE9 pDevice);
	void CreateBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth, bool bReturnFlag = true);
	virtual void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);
	void BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, D3DXVECTOR3 VtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3 VtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void OnlineUninit(void);

	void SetType(const int nType);
	void SetModelIdx(const int nModelIdx);
	void SetMesh(const LPD3DXMESH pMesh);
	void SetBuffMat(const LPD3DXBUFFER pBuffMat);
	void SetNumMat(const DWORD nNumMat);
	void SetTexture(LPDIRECT3DTEXTURE9 *pTexture);
	void SetVtxMax(const D3DXVECTOR3 VtxMax);
	void SetVtxMin(const D3DXVECTOR3 VtxMin);
	void SetAlpha(const float fAlpha);
	void SetIdx(const int nIdx);

	int GetType(void);
	int GetModelIdx(void);
	LPD3DXMESH GetMesh(void);
	LPD3DXBUFFER GetBuffMat(void);
	DWORD GetNumMat(void);
	LPDIRECT3DTEXTURE9 *GetTexture(void);
	D3DXVECTOR3 GetVtxMax(void);
	D3DXVECTOR3 GetVtxMin(void);
	float GetAlpha(void);
	int GetIdx(void);

	static int GetNumAll(void);
	static void ResetNumAll(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static int m_nNumAll;

	int                 m_nIdx;         // �ԍ�
	int                 m_nType;        // ��ޔԍ�
	int                 m_nModelIdx;    // �g�p���郂�f���̔ԍ�
	LPD3DXMESH          m_pMesh;        // ���b�V���ւ̃|�C���^
	LPD3DXBUFFER        m_pBuffMat;     // �}�e���A�����ւ̃|�C���^
	DWORD               m_nNumMat;      // �}�e���A�����̐�
	LPDIRECT3DTEXTURE9  *m_pTexture;    // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3         m_VtxMax;       // �ő�̒��_���W
	D3DXVECTOR3         m_VtxMin;       // �ŏ��̒��_���W
	float               m_fAlpha;       // ���f���̓����x
	D3DXVECTOR3         m_Scale;        // ���݂̃X�P�[��
	float               m_fChangeScale; // �X�P�[���̔{����ς���l
	int                 m_nLife;        // ����
	STATE               m_State;        // ���
};

//*****************************************************************************
//    �A�C�e��(�X�^�[)�N���X�̒�`
//*****************************************************************************
class CItemStar : public CItem
{
public:    // �N�ł��A�N�Z�X�\
	CItemStar(int nPriority = 3, OBJTYPE objType = OBJTYPE_ITEM);
	~CItemStar();

	static CItemStar *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\

};

//*****************************************************************************
//    �A�C�e��(��֒e)�N���X�̒�`
//*****************************************************************************
class CItemGrenade : public CItem
{
public:    // �N�ł��A�N�Z�X�\
	CItemGrenade(int nPriority = 3, OBJTYPE objType = OBJTYPE_ITEM);
	~CItemGrenade();

	static CItemGrenade *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\

};

//*****************************************************************************
//    �A�C�e��(�c�@UP)�N���X�̒�`
//*****************************************************************************
class CItem1up : public CItem
{
public:    // �N�ł��A�N�Z�X�\
	CItem1up(int nPriority = 3, OBJTYPE objType = OBJTYPE_ITEM);
	~CItem1up();

	static CItem1up *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\

};

//*****************************************************************************
//    �A�C�e��(�X�R�b�v)�N���X�̒�`
//*****************************************************************************
class CItemScoop : public CItem
{
public:    // �N�ł��A�N�Z�X�\
	CItemScoop(int nPriority = 3, OBJTYPE objType = OBJTYPE_ITEM);
	~CItemScoop();

	static CItemScoop *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\

};

//*****************************************************************************
//    �A�C�e��(���v)�N���X�̒�`
//*****************************************************************************
class CItemClock : public CItem
{
public:    // �N�ł��A�N�Z�X�\
	CItemClock(int nPriority = 3, OBJTYPE objType = OBJTYPE_ITEM);
	~CItemClock();

	static CItemClock *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\

};

//*****************************************************************************
//    �A�C�e��(�w�����b�g)�N���X�̒�`
//*****************************************************************************
class CItemHelmet : public CItem
{
public:    // �N�ł��A�N�Z�X�\
	CItemHelmet(int nPriority = 3, OBJTYPE objType = OBJTYPE_ITEM);
	~CItemHelmet();

	static CItemHelmet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\

};

#endif