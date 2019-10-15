//=============================================================================
//
// オブジェクト処理 [scene.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

#define SCORE_DIGIT (6)	//桁数
#define MAX_SCENE (2000)	//シーンの最大数
#define NUM_PRIORITY (8)
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScene
{
public:	//誰からもアクセス可能
	//=========================================================================
	// 構造体の定義
	//=========================================================================
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,		//プレイヤー
		OBJTYPE_ENEMY,				//敵
		OBJTYPE_HUMAN,				//人間
		OBJTYPE_BOSS,				//ボス
		OBJTYPE_BULLET,				//弾
		OBJTYPE_EXPLOTION,			//爆発
		OBJTYPE_BG,					//背景
		OBJTYPE_BEANS,				//豆
		OBJTYPE_CLOUD,			//バリア
		OBJTYPE_FADE,				//フェード
		OBJTYPE_BARRIER,			//バリア
		OBJTYPE_MAX,
	} OBJTYPE;
	//デフォルトコンストラクタ
	//CScene();
	CScene(int nPriority = 3);
	//純粋仮想関数
	virtual ~CScene();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	void SetObjType(OBJTYPE objType);
	OBJTYPE GetObjType(void);

	//静的メンバ関数
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static CScene *GetScene(int nPriority,int nIdx);

private:	//自分だけがアクセス可能
	//静的メンバ変数
	static CScene *m_apScene[NUM_PRIORITY][MAX_SCENE];
	static int m_nNumAll;
	int  m_nID;
	OBJTYPE m_objType;
	int m_nPriority;	//優先順位の番号
protected:	//自分と派生クラスだけがアクセス可能
	void Release(void);
};
#endif