//=============================================================================
//
// UI処理 [ui.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

#include "main.h"
#include "scene.h"

#define MAX_GAUGE_TEXTURE (2)		//UIゲージのテクスチャ最大数
#define MAX_GAUGE_WIDTH (300.0f)
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CUI
{
public:
	//メンバ関数
	CUI();
	~CUI();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 texture);
	void SetPos(D3DXVECTOR3 pos);
	void SetCol(D3DXCOLOR col);
	void SetWidth(float fWidth);
	void SetHeight(float fHeight);
	void SetVtxPos(D3DXVECTOR3 pos);
	void SetSideCut(int nType);

	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR	GetCol(void);
	float GetWidth(void);
	float GetHeight(void);

	//静的メンバ関数
	static CUI *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);

private:
	LPDIRECT3DTEXTURE9					m_pTexture;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9				m_pVtxBuff;			// 頂点バッファへのポインタ
	D3DXVECTOR3							m_pos;				// 位置
	D3DXCOLOR							m_col;				// 色
	float								m_fWidth;			// 幅
	float								m_fHeight;			// 高さ

protected:
};

//*****************************************************************************
// 体力ゲージのクラス定義
//*****************************************************************************
class CUI_LifeGauge : public CScene
{
public:
	//メンバ関数
	CUI_LifeGauge();
	~CUI_LifeGauge();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CutLifeGauge(int cut);
	void ResetLifeGauge(int nLife);

	//静的メンバ関数
	static CUI_LifeGauge *Create(void);
	static HRESULT Load(void);
	static void Unload(void);
	static CUI *GetLifeGauge(void);

private:
	//メンバ変数

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9			m_pTexture;	// テクスチャへのポインタ
	static CUI							*m_pUI;		// UIへのポインタ

protected:
};
//*****************************************************************************
// とくぎゲージのクラス定義
//*****************************************************************************
class CUI_SkillGauge : public CScene
{
public:
	//メンバ関数
	CUI_SkillGauge();
	~CUI_SkillGauge();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CutSkillGauge(int cut);

	//静的メンバ関数
	static CUI_SkillGauge *Create(void);
	static HRESULT Load(void);
	static void Unload(void);

private:
	//メンバ変数

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9			m_apTexture[MAX_GAUGE_TEXTURE];	// テクスチャへのポインタ
	static CUI							*m_apUI[MAX_GAUGE_TEXTURE];		// UIへのポインタ

protected:
};

//*****************************************************************************
// ダメージゲージのクラス定義
//*****************************************************************************
class CUI_DamageGauge : public CScene
{
public:
	typedef enum
	{
		STATE_NOMAL = 0,
		STATE_MINUS,
		STATE_MAX,
	}DAMAGE_STATE;

	//メンバ関数
	CUI_DamageGauge();
	~CUI_DamageGauge();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetDamageState(DAMAGE_STATE state);

	//静的メンバ関数
	static CUI_DamageGauge *Create(void);
	static HRESULT Load(void);
	static void Unload(void);

private:
	//メンバ変数
	DAMAGE_STATE m_state;
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9			m_pTexture;	// テクスチャへのポインタ
	static CUI							*m_pUI;		// UIへのポインタ

protected:
};

//*****************************************************************************
//	ベースゲージのクラス定義
//*****************************************************************************
class CUI_BaseGauge : public CScene
{
public:
	//メンバ関数
	CUI_BaseGauge();
	~CUI_BaseGauge();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CUI_BaseGauge *Create(void);
	static HRESULT Load(void);
	static void Unload(void);

private:
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9			m_pTexture;	// テクスチャへのポインタ
	static CUI							*m_pUI;		// UIへのポインタ

protected:
};

#endif