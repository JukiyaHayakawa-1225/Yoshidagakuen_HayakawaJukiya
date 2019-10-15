//=============================================================================
//
// ゲームの処理 [game.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//前方宣言
class CMeshField;
class CPlayer;
class CCommand;
class CMap;
class CTimer;
class CScore;
class CStatus_Number;
class CUI_LifeGauge;
class CUI_SkillGauge;
class CUI_DamageGauge;
class CBillboardNumber;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGame
{
public:	//誰からもアクセス可能
	//メンバ関数
	CGame();
	~CGame();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CGame			*Create(void);
	static CMeshField		*GetMeshField(void);
	static CPlayer			*GetPlayer(void);
	static CCommand			*GetCommad(void);
	static CScore			*GetScore(void);
	static CStatus_Number	*GetStatusNumber(void);
	static CUI_LifeGauge	*GetUI_LifeGauge(void);
	static CUI_SkillGauge	*GetUI_SkillGauge(void);
	static CUI_DamageGauge	*GetUI_DamageGauge(void);
	static CBillboardNumber *GetBillborad_Number(void);

private:	//自分だけがアクセス可能

	//静的メンバ変数
	static CMeshField		*m_pMeshField;	//メッシュフィールドのポインタ
	static CPlayer			*m_pPlayer;		//プレイヤーのポインタ
	static CCommand			*m_pCommand;
	static CMap				*m_pMap;		//マップのポインタ
	static CTimer			*m_pTimer;		//タイマーのポインタ
	static CScore			*m_pScore;		//スコアのポインタ
	static CStatus_Number	*m_pStatuNumber;//ステータス数字のポインタ
	static CUI_LifeGauge	*m_pUI_LifeGauge;
	static CUI_SkillGauge	*m_pUI_SkillGauge;
	static CUI_DamageGauge	*m_pUI_DamageGauge;
	static CBillboardNumber *m_pBillboard_Number;
};
#endif