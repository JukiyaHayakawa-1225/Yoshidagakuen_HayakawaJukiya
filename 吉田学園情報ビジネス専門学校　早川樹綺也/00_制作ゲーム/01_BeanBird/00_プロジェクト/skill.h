//=============================================================================
//
// スキル処理 [skill.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _SKILL_H_
#define _SKILL_H_

#include "main.h"
#include "scene.h"

#define MAX_SKILL   (SKILL_MAX)               // スキルの総数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSkill : public CScene
{
public:
	//*************************************
	// 項目の状態
	//*************************************
	typedef enum
	{
		SKILL_STATE_NONE = 0,			// 選択されていない状態
		SKILL_STATE_USE,			    // 選択されている状態
		SKILL_STATE_MAX
	}SKILL_STATE;

	typedef enum
	{
		SKILL_2WAY = 0,		//2Way
		SKILL_SPEED,		//スピード
		SKILL_BARRIER,		//バリア
		SKILL_AVATAR,		//分身
		SKILL_MAX
	}SKILL;

	//メンバ関数
	CSkill();
	~CSkill();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void Unload(void);
	void SetSkill(SKILL skill, SKILL_STATE state);

	//静的メンバ関数
	static CSkill *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
private:
	//メンバ変数
	D3DXVECTOR3					m_pos;							//位置
	float						m_fWidth;						//幅
	float						m_fHeight;						//高さ
	D3DXCOLOR					m_aCol[MAX_SKILL];
	//static SKILL				m_skill;
	static SKILL_STATE			m_state[MAX_SKILL];
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;	// 頂点バッファへのポインタ
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_SKILL];
protected:
};
#endif