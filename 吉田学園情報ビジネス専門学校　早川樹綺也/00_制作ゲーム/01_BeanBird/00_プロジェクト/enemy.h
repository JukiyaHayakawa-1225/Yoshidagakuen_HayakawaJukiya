//=============================================================================
//
// 敵の処理 [enemy.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "scene2D.h"

#define TEXTURE_NAME006 "data\\TEXTURE\\enemy000.png"			//テクスチャ名
#define TEXTURE_NAME010 "data\\TEXTURE\\enemy001.png"			//テクスチャ名

#define MAX_ENEMY_TEXTURE (2)
#define ENEMY_SIZE		  (50)
#define BOSS_SIZE		  (100)
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEnemy : public CScene2D
{
public:
	typedef enum
	{// 敵の状態
		ENEMYSTATE_NORMAL = 0,	// 通常状態
		ENEMYSTATE_DAMEGE,		// ダメージ状態
		ENEMYSTATE_MAX			// 敵の状態の種類の総数
	} ENEMYSTATE;
	typedef enum
	{
		ENEMY_TYPE_NOMAL = 0,
		ENEMY_TYPE_BOSS,
		ENEMY_TYPE_MAX,
	}ENEMY_TYPE;

	//メンバ関数
	CEnemy(int nPriority = 4);
	~CEnemy();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void HitDamage(int nDamage,ENEMY_TYPE type);
	void Collision(void);

private:
	float			m_fSpin;
	D3DXVECTOR3		m_pos;
	ENEMYSTATE		m_enemyState;
	int				m_nCountState;

	//静的メンバ変数
	static		    int m_nNumEnemy;
	static LPDIRECT3DTEXTURE9 m_pTexture;
protected :
	int				m_nLife;

};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEnemySlide : public CEnemy
{
public:
	typedef enum
	{
		SLIDE_R = 0,	//右
		SLIDE_L,		//左
		SLIDE_MAX,
	}SLIDE;
	//メンバ関数
	CEnemySlide();
	~CEnemySlide();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CEnemySlide *Create(D3DXVECTOR3 pos, float fWight, float fHeight,SLIDE slide);

private:
	D3DXVECTOR3		m_pos;
	D3DXVECTOR3		m_move;
	SLIDE			m_slide;
	static LPDIRECT3DTEXTURE9 m_pTexture;
};

//*****************************************************************************
// カーブ敵
//*****************************************************************************
class CEnemyCurve : public CEnemy
{
public:
	typedef enum
	{
		CURVE_R = 0,	//右
		CURVE_L,		//左
		CURVE_MAX,
	}CURVE;

	//メンバ関数
	CEnemyCurve();
	~CEnemyCurve();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CEnemyCurve *Create(D3DXVECTOR3 pos, float fWight, float fHeight,CURVE curve);

private:
	D3DXVECTOR3		m_pos;
	D3DXVECTOR3		m_move;
	int				m_nCounter;
	CURVE			m_curve;
	static LPDIRECT3DTEXTURE9 m_pTexture;
};
//*****************************************************************************
// 蛇行
//*****************************************************************************
class CEnemyMeandering : public CEnemy
{
public:
	typedef enum
	{
		MEANDERING_R = 0,	//右
		MEANDERING_L,		//左
		MEANDERING_MAX,
	}MEANDERING;

	//メンバ関数
	CEnemyMeandering();
	~CEnemyMeandering();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CEnemyMeandering *Create(D3DXVECTOR3 pos, float fWight, float fHeight, MEANDERING meandering);

private:
	D3DXVECTOR3		m_pos;
	D3DXVECTOR3		m_move;
	int				m_nCounter;
	MEANDERING		m_meandering;
	static LPDIRECT3DTEXTURE9 m_pTexture;
};
//*****************************************************************************
// 追尾
//*****************************************************************************
class CEnemyTracking : public CEnemy
{
public:
	//メンバ関数
	CEnemyTracking();
	~CEnemyTracking();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CEnemyTracking *Create(D3DXVECTOR3 pos, float fWight, float fHeight);

private:
	D3DXVECTOR3		m_pos;
	D3DXVECTOR3		m_move;
	int				m_nCounter;
	static LPDIRECT3DTEXTURE9 m_pTexture;
};

//*****************************************************************************
// 人間
//*****************************************************************************
class CEnemyHuman : public CEnemy
{
public:
	//メンバ関数
	CEnemyHuman();
	~CEnemyHuman();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CEnemyHuman *Create(D3DXVECTOR3 pos, float fWight, float fHeight);

private:
	D3DXVECTOR3		m_pos;
	D3DXVECTOR3		m_move;
	int				m_nCounter;
	static LPDIRECT3DTEXTURE9 m_pTexture;
};

//*****************************************************************************
// ボス
//*****************************************************************************
class CEnemyBoss : public CEnemy
{
public:
	//メンバ関数
	CEnemyBoss();
	~CEnemyBoss();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CEnemyBoss *Create(D3DXVECTOR3 pos, float fWight, float fHeight);

private:
	D3DXVECTOR3		m_pos;
	D3DXVECTOR3		m_move;
	int				m_nCounter;
	static LPDIRECT3DTEXTURE9 m_pTexture;
};
#endif