//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
#include "scenebillboard.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CParticle : public CSceneBillboard
{
public:
	//メンバ関数
	CParticle();
	~CParticle();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetLife(int nLife);
	int GetLife(void);

	//静的メンバ関数
	static CParticle *Create(D3DXVECTOR3 pos,D3DXVECTOR3 move, D3DXCOLOR col,float fLength);

private:
	//メンバ変数
	int m_nLife;		//体力
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSkill2Particle : public CParticle
{
public:
	//メンバ関数
	CSkill2Particle();
	~CSkill2Particle();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CSkill2Particle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fLength,int nLife);

private:
	//メンバ変数
	D3DXVECTOR3 m_move;	//移動量
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CDamageParticle : public CParticle
{
public:
	//メンバ関数
	CDamageParticle();
	~CDamageParticle();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CDamageParticle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fLength, int nLife);

private:
	//メンバ変数
	D3DXVECTOR3 m_move;	//移動量
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSnowParticle : public CParticle
{
public:
	//メンバ関数
	CSnowParticle();
	~CSnowParticle();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CSnowParticle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fLength, int nLife);

private:
	//メンバ変数
	D3DXVECTOR3 m_move;	//移動量
	int	m_nTimer;
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CFountainParticle : public CParticle
{
public:
	//メンバ関数
	CFountainParticle();
	~CFountainParticle();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CFountainParticle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fLength, int nLife);

private:
	//メンバ変数
	D3DXVECTOR3 m_move;	//移動量
	int	m_nTimer;
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
};
#endif